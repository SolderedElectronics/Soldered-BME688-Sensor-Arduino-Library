/**
 **************************************************
 *
 * @file        BME688-Soldered.cpp
 * @brief       Library for the BME688 environmental sensor
 *              (temperature, humidity, pressure, gas resistance)
 *
 * @copyright   GNU General Public License v3.0
 * @authors     Original Author: Saurav Sajeev (https://github.com/styropyr0)
 *              Modifications by: Josip Šimun Kuči @ Soldered.com
 * @date        Last modified: 2025-07-23
 ***************************************************/

#include <BME688-Soldered.h>
#include <Wire.h>

/**
 * @brief Constructor for BME688 sensor interface
 * 
 * @param address I2C address of the sensor
 */
BME688::BME688(uint8_t address) : _address(address) {}

/**
 * @brief Initialize the sensor with default settings
 * 
 * @return true if initialization succeeded, false otherwise
 */
bool BME688::begin()
{
    Wire.begin();
    if (isConnected())
    {
        i2c_execute(BME_688_CTRL_MEAS_HUM_REG, hum_oss);
        i2c_execute(BME_688_CTRL_MEAS_REG, temp_oss << 5 | press_oss << 2 | BME_688_FORCED_MODE);
        i2c_execute(BME_688_IIR_FILTER_REG, BME_688_IIR_FILTER_C15);
        readCalibParams();
    }
    else
        printLog(BME_688_CHECK_CONN_ERR);
    return isConnected();
}

/**
 * @brief Initialize the sensor with specified operation mode
 * 
 * @param mode Operation mode (BME_688_SLEEP_MODE to BME_688_PARALLEL_MODE)
 * @return true if initialization succeeded, false otherwise
 */
bool BME688::begin(uint8_t mode)
{
    Wire.begin();
    if (isConnected())
    {
        if (mode <= BME_688_PARALLEL_MODE)
        {
            i2c_execute(BME_688_CTRL_MEAS_HUM_REG, BME_688_OSS_1);
            i2c_execute(BME_688_CTRL_MEAS_REG, BME_688_OSS_1 << 5 | BME_688_OSS_1 << 2 | mode);
            i2c_execute(BME_688_IIR_FILTER_REG, BME_688_IIR_FILTER_C15);
            readCalibParams();
        }
        else
        {
            printLog(BME_688_VALUE_INVALID);
            return false;
        }
    }
    else
        printLog(BME_688_CHECK_CONN_ERR);
    return isConnected();
}

/**
 * @brief Initialize the sensor with specified operation mode and oversampling
 * 
 * @param mode Operation mode (BME_688_SLEEP_MODE to BME_688_PARALLEL_MODE)
 * @param oss Oversampling setting (BME_688_OSS_0 to BME_688_OSS_16)
 * @return true if initialization succeeded, false otherwise
 */
bool BME688::begin(uint8_t mode, uint8_t oss)
{
    Wire.begin();
    if (isConnected())
    {
        if (mode <= BME_688_PARALLEL_MODE && oss <= BME_688_OSS_16)
        {
            i2c_execute(BME_688_CTRL_MEAS_HUM_REG, oss);
            i2c_execute(BME_688_CTRL_MEAS_REG, oss << 5 | oss << 2 | mode);
            i2c_execute(BME_688_IIR_FILTER_REG, BME_688_IIR_FILTER_C15);
            readCalibParams();
        }
        else
        {
            printLog(BME_688_VALUE_INVALID);
            return false;
        }
    }
    else
        printLog(BME_688_CHECK_CONN_ERR);
    return isConnected();
}

/**
 * @brief Print log messages to serial if enabled
 * 
 * @param log Message to print
 */
void BME688::printLog(String log)
{
    if (printLogs)
        Serial.println(log);
}

/**
 * @brief Enable or disable logging
 * 
 * @param show true to enable logging, false to disable
 */
void BME688::showLogs(bool show)
{
    printLogs = show;
}

/**
 * @brief Read calibration parameters from sensor registers
 */
void BME688::readCalibParams()
{
    // Read temperature calibration parameters
    if (!(i2c_read_Xbit_LE(BME_688_TEMP_CALIB1_REG, &par_t16[0], 16) &&
          i2c_read_Xbit_LE(BME_688_TEMP_CALIB2_REG, &par_t16[1], 16) &&
          i2c_readByte(BME_688_TEMP_CALIB3_REG, &par_t3, 1)))
        printLog(BME_688_TEMP_CAL_EXCEPT);

    // Read pressure calibration parameters
    if (!(i2c_read_Xbit_LE(BME_688_PRES_CALIB1_REG, &par_p1, 16) &&
          i2c_read_Xbit_LE(BME_688_PRES_CALIB2_REG, &par_p16[1], 16) &&
          i2c_readByte(BME_688_PRES_CALIB3_REG, &par_p8[0], 1) &&
          i2c_read_Xbit_LE(BME_688_PRES_CALIB4_REG, &par_p16[2], 16) &&
          i2c_read_Xbit_LE(BME_688_PRES_CALIB5_REG, &par_p16[3], 16) &&
          i2c_readByte(BME_688_PRES_CALIB6_REG, &par_p8[1], 1) &&
          i2c_readByte(BME_688_PRES_CALIB7_REG, &par_p8[2], 1) &&
          i2c_read_Xbit_LE(BME_688_PRES_CALIB8_REG, &par_p16[4], 16) &&
          i2c_read_Xbit_LE(BME_688_PRES_CALIB9_REG, &par_p16[5], 16) &&
          i2c_readByte(BME_688_PRES_CALIB10_REG, &par_p10, 1)))
        printLog(BME_688_PRES_CAL_EXCEPT);

    // Read humidity calibration parameters
    if (!(i2c_read_Xbit_LE(BME_688_HUM_CALIB1_REG, &par_h16[0], 12) &&
          i2c_read_Xbit(BME_688_HUM_CALIB2_REG, &par_h16[1], 12) &&
          i2c_readByte(BME_688_HUM_CALIB3_REG, &par_h8[0], 1) &&
          i2c_readByte(BME_688_HUM_CALIB4_REG, &par_h8[1], 1) &&
          i2c_readByte(BME_688_HUM_CALIB5_REG, &par_h8[2], 1) &&
          i2c_readByte(BME_688_HUM_CALIB6_REG, &par_h6, 1) &&
          i2c_readByte(BME_688_HUM_CALIB7_REG, &par_h8[4], 1)))
        printLog(BME_688_HUM_CAL_EXCEPT);

    // Read gas calibration parameters
    if (!(i2c_readByte(BME_688_GAS_CALIB1_REG, &par_g1, 1) &&
          i2c_read_Xbit_LE(BME_688_GAS_CALIB2_REG, &par_g2, 16) &&
          i2c_readByte(BME_688_GAS_CALIB3_REG, &par_g3, 1) &&
          i2c_readByte(BME_688_GAS_HEAT_RANGE_REG, &res_heat_range, 1) &&
          i2c_readByte(BME_688_GAS_HEAT_VAL_REG, &res_heat_val, 1)))
        printLog(BME_688_TEMP_CAL_EXCEPT);

    setHeatProfiles();
}

/**
 * @brief Set heating profiles for gas measurements
 * 
 * @return true if successful
 */
bool BME688::setHeatProfiles()
{
    readTemperature();
    for (uint8_t i = 0; i < 9; i++)
    {
        yield();
        uint8_t gasTemp = readUCGas(BME_688_GAS_START_TEMP + i * 25);
        i2c_execute(BME_688_GAS_WAIT_PROFILE_REG + i, BME_688_GAS_WAIT_MULFAC1 << 6 | (uint8_t)(0.25 * gasTemp - 22));
        i2c_execute(BME_688_GAS_RES_HEAT_PROFILE_REG + i, gasTemp);
    }
    return true;
}

/**
 * @brief Check if sensor is connected
 * 
 * @return true if sensor is connected and responds with correct chip ID
 */
bool BME688::isConnected()
{
    uint8_t pid = 0;
    if (is_sensor_connected() && i2c_readByte(BME_688_CHIP_ID_REG, &pid, 1))
        return pid == BME_688_CHIP_ID;
    return false;
}

/**
 * @brief Set temperature oversampling
 * 
 * @param oss Oversampling setting (BME_688_OSS_0 to BME_688_OSS_16)
 * @return true if setting was valid and applied
 */
bool BME688::setTemperatureOversampling(uint8_t oss)
{
    if (oss <= BME_688_OSS_16)
        temp_oss = oss;
    else
    {
        printLog(BME_688_VALUE_INVALID);
        return false;
    }
    return true;
}

/**
 * @brief Read raw temperature value from sensor
 * 
 * @return int32_t Raw temperature value
 */
int32_t BME688::readRawTemp()
{
    int32_t raw = 0;
    if (!i2c_read_Xbit(BME_688_TEMP_RAW_REG, &raw, 20))
        printLog(BME_688_READ_FAILURE);
    return raw;
}

/**
 * @brief Read raw pressure value from sensor
 * 
 * @return int32_t Raw pressure value
 */
int32_t BME688::readRawPres()
{
    int32_t raw = 0;
    if (!i2c_read_Xbit(BME_688_PRES_RAW_REG, &raw, 20))
        printLog(BME_688_READ_FAILURE);
    return raw;
}

/**
 * @brief Read raw humidity value from sensor
 * 
 * @return int16_t Raw humidity value
 */
int16_t BME688::readRawHum()
{
    int16_t raw = 0;
    if (!i2c_read_Xbit(BME_688_HUM_RAW_REG, &raw, 16))
        printLog(BME_688_READ_FAILURE);
    return raw;
}

/**
 * @brief Read raw gas resistance value from sensor
 * 
 * @return int16_t Raw gas resistance value
 */
int16_t BME688::readRawGas()
{
    int16_t raw = 0;
    if (!i2c_read_Xbit(BME_688_HUM_RAW_REG, &raw, 16))
        printLog(BME_688_READ_FAILURE);
    return raw;
}

/**
 * @brief Convert raw temperature ADC value to degrees Celsius
 * 
 * @param adc_T Raw temperature value
 * @return double Temperature in °C
 */
double BME688::readUCTemp(int32_t adc_T)
{
    double var1 = (((double)adc_T / 16384.0) - ((double)par_t16[0] / 1024.0)) * (double)par_t16[1];
    double var2 = ((((double)adc_T / 131072.0) - ((double)par_t16[0] / 8192.0)) *
                   (((double)adc_T / 131072.0) - ((double)par_t16[0] / 8192.0))) *
                  ((double)par_t3 * 16.0);

    t_fine = var1 + var2;
    return t_fine / 5120.0;
}

/**
 * @brief Convert raw pressure ADC value to Pascals
 * 
 * @param adc_P Raw pressure value
 * @return double Pressure in Pa
 */
double BME688::readUCPres(int32_t adc_P)
{
    double var1 = 0.0, var2 = 0.0, var3 = 0.0;
    double press_comp = 0.0;

    var1 = ((double)t_fine / 2.0) - 64000.0;
    var2 = var1 * var1 * ((double)par_p8[1] / 131072.0);
    var2 = var2 + (var1 * (double)par_p16[3] * 2.0);
    var2 = (var2 / 4.0) + ((double)par_p16[2] * 65536.0);
    var1 = ((((double)par_p8[0] * var1 * var1) / 16384.0) + ((double)par_p16[1] * var1)) / 524288.0;
    var1 = (1.0 + (var1 / 32768.0)) * (double)par_p1;
    press_comp = 1048576.0 - (double)adc_P;
    press_comp = ((press_comp - (var2 / 4096.0)) * 6250.0) / var1;
    var1 = ((double)par_p16[5] * press_comp * press_comp) / 2147483648.0;
    var2 = press_comp * ((double)par_p16[4] / 32768.0);
    var3 = (press_comp / 256.0) * (press_comp / 256.0) * (press_comp / 256.0) * ((double)par_p10 / 131072.0);
    p_fine = press_comp + (var1 + var2 + var3 + ((double)par_p8[2] * 128.0)) / 16.0;
    return p_fine;
}

/**
 * @brief Convert raw humidity ADC value to percentage
 * 
 * @param adc_H Raw humidity value
 * @return double Relative humidity in %
 */
double BME688::readUCHum(int16_t adc_H)
{
    double t_fine = this->t_fine / 5120.0;
    double var1 = 0, var2 = 0, var3 = 0, var4 = 0;

    var1 = adc_H - (((double)par_h16[0] * 16.0) + (((double)par_h8[0] / 2.0) * t_fine));
    var2 = var1 * (((double)par_h16[1] / 262144.0) *
                   (1.0 + (((double)par_h8[1] / 16384.0) * t_fine) +
                    (((double)par_h8[2] / 1048576.0) * t_fine * t_fine)));
    var3 = (double)par_h6 / 16384.0;
    var4 = (double)par_h8[4] / 2097152.0;
    h_fine = var2 + ((var3 + (var4 * t_fine)) * var2 * var2);
    return h_fine;
}

/**
 * @brief Calculate gas heater temperature for target temperature
 * 
 * @param target_temp Target temperature in °C
 * @return uint8_t Calculated heater temperature
 */
uint8_t BME688::readUCGas(uint16_t target_temp)
{
    double t_fine = this->t_fine / 5120.0;
    double var1 = 0, var2 = 0, var3 = 0, var4 = 0, var5 = 0;

    var1 = ((double)par_g1 / 16.0) + 49.0;
    var2 = (((double)par_g2 / 32768.0) * 0.0005) + 0.00235;
    var3 = (double)par_g3 / 1024.0;
    var4 = var1 * (1.0 + (var2 * (double)target_temp));
    var5 = var4 + (var3 * (double)t_fine);
    g_fine = (uint8_t)(3.4 * ((var5 * (4.0 / (4.0 + (double)((res_heat_range & BME_688_HEAT_RANGE_MASK) >> 4))) * (1.0 / (1.0 + ((double)res_heat_val * 0.002)))) - 25));
    return g_fine;
}

/**
 * @brief Check if gas measurement is complete
 * 
 * @return true if gas measurement is finished
 */
bool BME688::checkGasMeasurementCompletion()
{
    uint8_t m_Complete = 0;
    i2c_readByte(BME_688_GAS_MEAS_STATUS_REG1, &m_Complete, 1);
    m_Complete &= BME_688_GAS_HEAT_STAB_MASK | BME_688_GAS_VALID_REG_MASK;
    return m_Complete == BME_688_GAS_MEAS_FINISH;
}

/**
 * @brief Read temperature from sensor
 * 
 * @return double Temperature in °C
 */
double BME688::readTemperature()
{
    i2c_execute(BME_688_CTRL_MEAS_REG, temp_oss << 5 | press_oss << 2 | mode);
    delay(10);
    return readUCTemp(readRawTemp());
}

/**
 * @brief Read pressure from sensor
 * 
 * @return double Pressure in Pa
 */
double BME688::readPressure()
{
    i2c_execute(BME_688_CTRL_MEAS_REG, temp_oss << 5 | press_oss << 2 | mode);
    delay(10);
    float ps = readUCPres(readRawPres());
    return ps;
}

/**
 * @brief Read humidity from sensor
 * 
 * @return double Relative humidity in %
 */
double BME688::readHumidity()
{
    i2c_execute(BME_688_CTRL_MEAS_HUM_REG, hum_oss);
    i2c_execute(BME_688_CTRL_MEAS_REG, temp_oss << 5 | press_oss << 2 | mode);
    delay(10);
    return readUCHum(readRawHum());
}

/**
 * @brief Read gas resistance for specific temperature
 * 
 * @param temperature Target temperature in °C
 * @return double Gas resistance in ohms or -1 if error
 */
double BME688::readGasForTemperature(uint16_t temperature)
{
    if (allowHighTemps || temperature <= BME_688_HEAT_PLATE_MAX_TEMP)
    {
        if (temperature < BME_688_HEAT_PLATE_ULTRA_TEMP)
        {
            uint8_t t_temp = readUCGas((uint16_t)temperature);
            uint8_t t_wait = (uint8_t)(0.25 * t_temp - 17);
            i2c_execute(BME_688_CTRL_GAS_REG, 0x20);
            i2c_execute(BME_688_GAS_WAIT_PROFILE_REG, t_wait);
            i2c_execute(BME_688_GAS_RES_HEAT_PROFILE_REG, t_temp);
            return startGasMeasurement(BME_688_GAS_PROFILE_START, t_wait + 5);
        }
        else
            printLog(BME_688_TEMP_EXCEED_MAX_LIMIT);
    }
    else
        printLog(BME_688_TEMP_WARNING);

    return -1.0;
}

/**
 * @brief Read gas resistance using specified profile
 * 
 * @param profile Profile number (0-9)
 * @return double Gas resistance in ohms or -1 if error
 */
double BME688::readGas(uint8_t profile)
{
    if (profile >= 0 && profile < 10)
        return startGasMeasurement(profile, (uint8_t)(0.25 * readUCGas(BME_688_GAS_START_TEMP * profile) - 17));
    else
        printLog(BME_688_PROFILE_OUT_OF_RANGE);
    return -1.0;
}

/**
 * @brief Start gas measurement and read results
 * 
 * @param profile Profile number to use
 * @param waitTime Time to wait for measurement in ms
 * @return double Gas resistance in ohms or error code
 */
double BME688::startGasMeasurement(uint8_t profile, uint8_t waitTime)
{
    i2c_execute(BME_688_CTRL_GAS_REG, BME_688_GAS_RUN | profile);
    i2c_execute(BME_688_CTRL_MEAS_REG, mode);
    delay(waitTime);
    if (!checkGasMeasurementCompletion())
    {
        printLog(BME_688_GAS_MEAS_FAILURE);
        return -2.0;
    }
    uint16_t gas_adc = 0;
    uint8_t gas_range = 0;

    i2c_read_Xbit(BME_688_GAS_ADC_REG, &gas_adc, 10);
    i2c_readByte(BME_688_GAS_RANGE_REG, &gas_range, 1);

    gas_range &= BME_688_GAS_RANGE_VAL_MASK;

    uint32_t var1 = int32_t(262144) >> gas_range;
    int32_t var2 = (int32_t)gas_adc - int32_t(512);
    var2 *= int32_t(3);
    var2 = int32_t(4096) + var2;
    g_res = 1000000.0f * (float)var1 / (float)var2;
    cf_p = BME_688_GAS_CORRECTION;
    return g_res;
}

/**
 * @brief Enable/disable warnings for unsafe temperatures
 * 
 * @param ignore true to ignore warnings, false to enable them
 */
void BME688::ignoreUnsafeTemperatureWarnings(bool ignore)
{
    allowHighTemps = ignore;
    printLog(BME_688_TEMP_UNSAFE_WARNING);
}

// I2C communication methods

/**
 * @brief Write single byte to I2C register
 * 
 * @param reg Register address
 * @param data Data to write
 */
void BME688::i2c_execute(uint8_t reg, uint8_t data)
{
    Wire.beginTransmission(_address);
    Wire.write(reg);
    Wire.write(data);
    Wire.endTransmission(true);
}

/**
 * @brief Write 16-bit value to I2C register
 * 
 * @param reg Register address
 * @param data 16-bit data to write
 */
void BME688::i2c_execute_16bit(uint8_t reg, uint16_t data)
{
    Wire.beginTransmission(_address);
    Wire.write(reg);
    Wire.write(data >> 8);
    Wire.write(data & 0xFF);
    Wire.endTransmission(true);
}

/**
 * @brief Read bytes from I2C register (unsigned)
 * 
 * @param reg Register address
 * @param data Buffer to store read data
 * @param length Number of bytes to read
 * @return true if read succeeded
 */
bool BME688::i2c_readByte(uint8_t reg, uint8_t *const data, uint8_t length)
{
    startTransmission(reg);
    Wire.requestFrom(_address, length);
    if (Wire.available() < length)
        return false;
    for (uint8_t i = 0; i < length; i++)
        data[i] = Wire.read();
    return true;
}

/**
 * @brief Read bytes from I2C register (signed)
 * 
 * @param reg Register address
 * @param data Buffer to store read data
 * @param length Number of bytes to read
 * @return true if read succeeded
 */
bool BME688::i2c_readByte(uint8_t reg, int8_t *const data, uint8_t length)
{
    startTransmission(reg);
    Wire.requestFrom(_address, length);
    if (Wire.available() < length)
        return false;
    for (uint8_t i = 0; i < length; i++)
        data[i] = Wire.read();
    return true;
}

/**
 * @brief Start I2C transmission for reading
 * 
 * @param reg Register address to read from
 */
void BME688::startTransmission(uint8_t reg)
{
    Wire.beginTransmission(_address);
    Wire.write(reg);
    Wire.endTransmission(false);
}

/**
 * @brief Check if sensor is connected to I2C bus
 * 
 * @return true if sensor responds
 */
bool BME688::is_sensor_connected()
{
    Wire.beginTransmission(_address);
    bool result = Wire.endTransmission() == 0;
    Serial.println(result);
    return true;
}

/**
 * @brief Read X bits from register (little-endian)
 * 
 * @tparam T Data type for result
 * @param reg Register address
 * @param data Pointer to store result
 * @param length Number of bits to read
 * @return true if read succeeded
 */
template <typename T>
bool BME688::i2c_read_Xbit_LE(uint8_t reg, T *const data, uint8_t length)
{
    uint8_t l = length % 8 ? (length + (8 - length % 8)) / 8 : length / 8;
    startTransmission(reg);
    Wire.requestFrom(_address, l);
    if (Wire.available() == l)
    {
        T tempData = 0;
        for (int i = 0; i < l; i++)
            tempData |= (T)Wire.read() << 8 * i;

        if (length % 8)
            tempData >>= (8 - length % 8);

        *data = (T)tempData;
    }
    else
        return false;
    return true;
}

/**
 * @brief Read X bits from register (big-endian)
 * 
 * @tparam T Data type for result
 * @param reg Register address
 * @param data Pointer to store result
 * @param length Number of bits to read
 * @return true if read succeeded
 */
template <typename T>
bool BME688::i2c_read_Xbit(uint8_t reg, T *const data, uint8_t length)
{
    uint8_t l = length % 8 ? (length + (8 - length % 8)) / 8 : length / 8;
    startTransmission(reg);
    Wire.requestFrom(_address, l);
    if (Wire.available() == l)
    {
        T tempData = 0;
        for (int i = 0; i < l; i++)
            tempData |= (T)Wire.read() << 8 * (l - 1 - i);

        if (length % 8)
            tempData >>= (8 - length % 8);

        *data = (T)tempData;
    }
    else
        return false;
    return true;
}