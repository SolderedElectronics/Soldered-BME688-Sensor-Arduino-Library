/**
 **************************************************
 * @file        BME688-Soldered.h
 * @brief       Header file for BME688 environmental sensor library
 *              (temperature, humidity, pressure, gas resistance)
 *
 * @copyright   GNU General Public License v3.0
 * @authors     Original Author: Saurav Sajeev (https://github.com/styropyr0)
 *              Modifications by: Josip Šimun Kuči @ Soldered.com
 * @date        Last modified: 2025-07-23
 ***************************************************/

#ifndef BME688_SOLDERED_H
#define BME688_SOLDERED_H

#include "Arduino.h"

#ifdef __cplusplus

// I2C Addresses
#define BME688_I2C_ADDR_PRIMARY   0x76 ///< Primary I2C address for BME688
#define BME688_I2C_ADDR_SECONDARY 0x76 ///< Secondary I2C address for BME688 (same as primary)

// Status Codes
#define BME688_OK                     0   ///< Operation successful
#define BME688_E_NULL_PTR             -1  ///< Null pointer error
#define BME688_E_COM_FAIL             -2  ///< Communication failure
#define BME688_E_DEV_NOT_FOUND        -3  ///< Device not found
#define BME688_E_INVALID_LENGTH       -4  ///< Invalid data length
#define BME688_E_DEV_NOT_POWERED      -5  ///< Device not powered
#define BME688_E_USER_REG             -6  ///< User register error
#define BME688_E_I2C                  -7  ///< I2C error
#define BME688_E_I2C_TIMEOUT          -8  ///< I2C timeout
#define BME688_E_I2C_FAIL             -9  ///< I2C operation failed
#define BME688_E_SENSOR_NOT_SUPPORTED -10 ///< Sensor not supported
#define BME688_E_SENSOR_NOT_ENABLED   -11 ///< Sensor not enabled
#define BME688_E_SENSOR_NOT_POWERED   -12 ///< Sensor not powered

// Control Registers
#define BME_688_CTRL_MEAS_REG     0x74 ///< Measurement control register
#define BME_688_CTRL_MEAS_HUM_REG 0x72 ///< Humidity measurement control register

// Oversampling Settings
#define BME_688_OSS_NONE 0x00 ///< No oversampling
#define BME_688_OSS_1    0x01 ///< 1x oversampling
#define BME_688_OSS_2    0x02 ///< 2x oversampling
#define BME_688_OSS_4    0x03 ///< 4x oversampling
#define BME_688_OSS_8    0x04 ///< 8x oversampling
#define BME_688_OSS_16   0x05 ///< 16x oversampling

// Operation Modes
#define BME_688_SLEEP_MODE    0x00 ///< Sleep mode (low power)
#define BME_688_FORCED_MODE   0x01 ///< Forced mode (single measurement)
#define BME_688_PARALLEL_MODE 0x02 ///< Parallel mode (continuous measurement)

// Data Registers
#define BME_688_TEMP_RAW_REG  0x22 ///< Raw temperature data register
#define BME_688_PRES_RAW_REG  0x1F ///< Raw pressure data register
#define BME_688_HUM_RAW_REG   0x25 ///< Raw humidity data register
#define BME_688_CTRL_GAS_REG  0x71 ///< Gas sensor control register
#define BME_688_GAS_RAW_REG   0x2C ///< Raw gas resistance data register
#define BME_688_GAS_RANGE_REG 0x2C ///< Gas range register
#define BME_688_GAS_ADC_REG   0x2C ///< Gas ADC data register

// Temperature Calibration Registers
#define BME_688_TEMP_CALIB1_REG 0xE9 ///< Temperature calibration parameter 1
#define BME_688_TEMP_CALIB2_REG 0x8A ///< Temperature calibration parameter 2
#define BME_688_TEMP_CALIB3_REG 0x8C ///< Temperature calibration parameter 3

// Pressure Calibration Registers
#define BME_688_PRES_CALIB1_REG  0x8E ///< Pressure calibration parameter 1
#define BME_688_PRES_CALIB2_REG  0x90 ///< Pressure calibration parameter 2
#define BME_688_PRES_CALIB3_REG  0x92 ///< Pressure calibration parameter 3
#define BME_688_PRES_CALIB4_REG  0x94 ///< Pressure calibration parameter 4
#define BME_688_PRES_CALIB5_REG  0x96 ///< Pressure calibration parameter 5
#define BME_688_PRES_CALIB6_REG  0x99 ///< Pressure calibration parameter 6
#define BME_688_PRES_CALIB7_REG  0x98 ///< Pressure calibration parameter 7
#define BME_688_PRES_CALIB8_REG  0x9C ///< Pressure calibration parameter 8
#define BME_688_PRES_CALIB9_REG  0x9E ///< Pressure calibration parameter 9
#define BME_688_PRES_CALIB10_REG 0xA0 ///< Pressure calibration parameter 10

// Humidity Calibration Registers
#define BME_688_HUM_CALIB1_REG 0xE2 ///< Humidity calibration parameter 1
#define BME_688_HUM_CALIB2_REG 0xE1 ///< Humidity calibration parameter 2
#define BME_688_HUM_CALIB3_REG 0xE4 ///< Humidity calibration parameter 3
#define BME_688_HUM_CALIB4_REG 0xE5 ///< Humidity calibration parameter 4
#define BME_688_HUM_CALIB5_REG 0xE6 ///< Humidity calibration parameter 5
#define BME_688_HUM_CALIB6_REG 0xE7 ///< Humidity calibration parameter 6
#define BME_688_HUM_CALIB7_REG 0xE8 ///< Humidity calibration parameter 7

// Gas Calibration Registers
#define BME_688_GAS_CALIB1_REG     0xED ///< Gas calibration parameter 1
#define BME_688_GAS_CALIB2_REG     0xEB ///< Gas calibration parameter 2
#define BME_688_GAS_CALIB3_REG     0xEE ///< Gas calibration parameter 3
#define BME_688_GAS_HEAT_RANGE_REG 0x02 ///< Gas heater range register
#define BME_688_GAS_HEAT_VAL_REG   0x00 ///< Gas heater value register

// IIR Filter Settings
#define BME_688_IIR_FILTER_REG  0x75 ///< IIR filter coefficient register
#define BME_688_IIR_FILTER_C0   0x00 ///< Filter coefficient 0 (off)
#define BME_688_IIR_FILTER_C1   0x01 ///< Filter coefficient 1
#define BME_688_IIR_FILTER_C3   0x02 ///< Filter coefficient 3
#define BME_688_IIR_FILTER_C7   0x03 ///< Filter coefficient 7
#define BME_688_IIR_FILTER_C15  0x04 ///< Filter coefficient 15
#define BME_688_IIR_FILTER_C31  0x05 ///< Filter coefficient 31
#define BME_688_IIR_FILTER_C63  0x06 ///< Filter coefficient 63
#define BME_688_IIR_FILTER_C127 0x07 ///< Filter coefficient 127

// Gas Measurement Status
#define BME_688_GAS_MEAS_STATUS_REG0 0x2E ///< Gas measurement status register 0
#define BME_688_GAS_MEAS_STATUS_REG1 0x2D ///< Gas measurement status register 1
#define BME_688_GAS_HEAT_STAB_MASK   0x10 ///< Gas heater stability mask
#define BME_688_GAS_VALID_REG_MASK   0x20 ///< Gas valid data mask
#define BME_688_GAS_NEW_DATA_MASK    0x80 ///< New data available mask
#define BME_688_GAS_MEAS_MASK        0x40 ///< Gas measurement in progress mask
#define BME_688_MEAS_MASK            0x20 ///< Measurement in progress mask
#define BME_688_HEAT_RANGE_MASK      0x18 ///< Heater range mask
#define BME_688_GAS_RANGE_REG_MASK   0x0F ///< Gas range register mask
#define BME_688_GAS_MEAS_INDEX_MASK  0x0F ///< Gas measurement index mask
#define BME_688_GAS_RANGE_VAL_MASK   0x0F ///< Gas range value mask
#define BME_688_GAS_RUN              0x20 ///< Gas measurement run bit

// Gas Measurement States
#define BME_688_GAS_MEAS_FINISH          0x30  ///< Gas measurement completed
#define BME_688_GAS_HEATING_INSUFFICIENT 0x10  ///< Gas heater not stable
#define BME_688_GAS_RESULT_NOT_READY     0x00  ///< Gas measurement not ready
#define BME_688_GAS_PROFILE_START        0x00  ///< Starting gas profile
#define BME_688_HEAT_PLATE_MAX_TEMP      0x1A9 ///< Maximum safe temperature (425°C)
#define BME_688_HEAT_PLATE_ULTRA_TEMP    0x258 ///< Absolute maximum temperature (600°C)

// Gas Wait Time Registers
#define BME_688_GAS_WAIT_PROFILE_REG     0x64 ///< Base register for gas wait times
#define BME_688_GAS_RES_HEAT_PROFILE_REG 0x5A ///< Base register for heater resistance
#define BME_688_GAS_START_TEMP           0xC8 ///< Default start temperature (200°C)

// Gas Wait Time Multiplication Factors
#define BME_688_GAS_WAIT_MULFAC1 0x00 ///< Multiplication factor 1
#define BME_688_GAS_WAIT_MULFAC2 0x01 ///< Multiplication factor 2
#define BME_688_GAS_WAIT_MULFAC3 0x02 ///< Multiplication factor 3
#define BME_688_GAS_WAIT_MULFAC4 0x03 ///< Multiplication factor 4

// Predefined Gas Wait Times (in ms)
#define BME_688_GAS_WAIT_PROFILE1  0x3C ///< Profile 1 wait time (60ms)
#define BME_688_GAS_WAIT_PROFILE2  0x50 ///< Profile 2 wait time (80ms)
#define BME_688_GAS_WAIT_PROFILE3  0x64 ///< Profile 3 wait time (100ms)
#define BME_688_GAS_WAIT_PROFILE4  0x90 ///< Profile 4 wait time (144ms)
#define BME_688_GAS_WAIT_PROFILE5  0xC0 ///< Profile 5 wait time (192ms)
#define BME_688_GAS_WAIT_PROFILE6  0xD2 ///< Profile 6 wait time (210ms)
#define BME_688_GAS_WAIT_PROFILE7  0xE0 ///< Profile 7 wait time (224ms)
#define BME_688_GAS_WAIT_PROFILE8  0xF0 ///< Profile 8 wait time (240ms)
#define BME_688_GAS_WAIT_PROFILE9  0xFA ///< Profile 9 wait time (250ms)
#define BME_688_GAS_WAIT_PROFILE10 0xFF ///< Profile 10 wait time (255ms)

// Predefined Heater Temperatures (in °C)
#define BME_688_GAS_HEAT_PROFILE1  200 ///< Profile 1 target temperature
#define BME_688_GAS_HEAT_PROFILE2  220 ///< Profile 2 target temperature
#define BME_688_GAS_HEAT_PROFILE3  240 ///< Profile 3 target temperature
#define BME_688_GAS_HEAT_PROFILE4  260 ///< Profile 4 target temperature
#define BME_688_GAS_HEAT_PROFILE5  280 ///< Profile 5 target temperature
#define BME_688_GAS_HEAT_PROFILE6  300 ///< Profile 6 target temperature
#define BME_688_GAS_HEAT_PROFILE7  320 ///< Profile 7 target temperature
#define BME_688_GAS_HEAT_PROFILE8  340 ///< Profile 8 target temperature
#define BME_688_GAS_HEAT_PROFILE9  360 ///< Profile 9 target temperature
#define BME_688_GAS_HEAT_PROFILE10 380 ///< Profile 10 target temperature

// Chip Identification
#define BME_688_CHIP_ID_REG 0xD0 ///< Chip ID register address
#define BME_688_CHIP_ID     0x61 ///< Expected chip ID value

// Correction Factors
#define BME_688_GAS_CORRECTION     1.3801 ///< Gas resistance correction factor
#define BME_688_GAS_CORRECTION_NIL 1.0    ///< No correction factor

// ------------ ERROR MESSAGES -----------------
#define BME_688_CHECK_CONN_ERR  "BME688 is disconnected. Check connections or make sure it is working."
#define BME_688_TEMP_CAL_EXCEPT "Exception: Failed to read temperature calibration parameters"
#define BME_688_PRES_CAL_EXCEPT "Exception: Failed to read pressure calibration parameters"
#define BME_688_HUM_CAL_EXCEPT  "Exception: Failed to read humidity calibration parameters"
#define BME_688_VALUE_INVALID   "Invalid value. Use a value within the range."
#define BME_688_READ_FAILURE    "Exception: Failed to read from BME688"
#define BME_688_GAS_MEAS_FAILURE                                                                                       \
    "Exception: Gas measurement incomplete.\nTemperature not achieved or heating might be too high for the provided "  \
    "wait time."
#define BME_688_TEMP_WARNING                                                                                           \
    "Warning: Higher temperatures will degrade the lifespan of the sensor. \nThis operation has been automatically "   \
    "denied for safety. \nIf you still wish to use high temperatures, call "                                           \
    "ignoreUnsafeTemperatureWarnings(false)\nIn safe mode, Temperature limit is 425°C. Bypassing this protection "     \
    "will raise the limit to 600°C."
#define BME_688_TEMP_EXCEED_MAX_LIMIT "Exception: Operation blocked. The temperature value exceeds maximum limit."
#define BME_688_PROFILE_OUT_OF_RANGE  "Exception: Operation blocked. Profile value should be between 0 and 9."
#define BME_688_TEMP_UNSAFE_WARNING                                                                                    \
    "Warning: Higher temperatures will degrade the lifespan of the sensor. It is recommended to use a value under "    \
    "425°C"


/**
 * @class BME688
 * @brief A driver class for interfacing with the BME688 sensor.
 *
 * This class provides functions to read temperature, pressure, humidity, and gas resistance
 * from the Bosch BME688 environmental sensor. It supports different oversampling settings
 * and operation modes.
 *
 * @author Saurav Sajeev
 */
class BME688
{
  public:
    /**
     * @brief Default constructor for BME688.
     */
    BME688(uint8_t address = BME688_I2C_ADDR_PRIMARY);

    /**
     * @brief Initializes the BME688 sensor with default settings.
     * @return True if the sensor is successfully initialized, false otherwise.
     */
    bool begin();

    /**
     * @brief Initializes the BME688 sensor with a specific operation mode.
     * @param mode The operating mode for the sensor.
     * @return True if initialization is successful, false otherwise.
     */
    bool begin(uint8_t mode);

    /**
     * @brief Initializes the BME688 sensor with a specific mode and oversampling settings.
     * @param mode The operating mode.
     * @param oss The oversampling setting.
     * @return True if initialization is successful, false otherwise.
     */
    bool begin(uint8_t mode, uint8_t oss);

    /**
     * @brief Reads the current temperature from the sensor.
     * @return Temperature in degrees Celsius.
     */
    double readTemperature();

    /**
     * @brief Reads the current atmospheric pressure.
     * @return Pressure in Pascals (Pa).
     */
    double readPressure();

    /**
     * @brief Reads the relative humidity from the sensor.
     * @return Humidity as a percentage (%).
     */
    double readHumidity();

    /**
     * @brief Reads gas resistance for a given target temperature.
     * @param temperature The target temperature in degrees Celsius.
     * @return Gas resistance in ohms (Ω).
     */
    double readGasForTemperature(uint16_t temperature);

    /**
     * @brief Reads gas resistance for a specific gas profile.
     * @param profile The gas measurement profile index.
     * @return Gas resistance in ohms (Ω).
     */
    double readGas(uint8_t profile);

    /**
     * @brief Enables or disables logging for debugging purposes.
     * @param show Set to true to enable logs, false to disable.
     */
    void showLogs(bool show);

    /**
     * @brief Sets the temperature oversampling setting.
     * @param oss Oversampling setting value.
     * @return True if the setting was applied successfully, false otherwise.
     */
    bool setTemperatureOversampling(uint8_t oss);

    /**
     * @brief Sets the pressure oversampling setting.
     * @param oss Oversampling setting value.
     * @return True if the setting was applied successfully, false otherwise.
     */
    bool setPressureOversampling(uint8_t oss);

    /**
     * @brief Sets the humidity oversampling setting.
     * @param oss Oversampling setting value.
     * @return True if the setting was applied successfully, false otherwise.
     */
    bool setHumidityOversampling(uint8_t oss);

    /**
     * @brief Allows ignoring unsafe temperature warnings.
     * @param ignore Set to true to ignore warnings, false to keep them.
     */
    void ignoreUnsafeTemperatureWarnings(bool ignore);

    /**
     * @brief Checks if the sensor is connected and responding.
     * @return True if the sensor is connected, false otherwise.
     */
    bool isConnected();

  private:
    uint8_t temp_oss = BME_688_OSS_1, press_oss = BME_688_OSS_1, hum_oss = BME_688_OSS_1, mode = BME_688_FORCED_MODE;

    bool printLogs = false;

    uint8_t _address;

    // CALIBRATION CONSTANTS
    int16_t par_t16[2] = {0};
    int8_t par_t3 = 0;
    uint16_t par_p1 = 0;
    int16_t par_p16[6] = {0};
    int8_t par_p8[4] = {0};
    uint16_t par_h16[2] = {0};
    int8_t par_h8[5] = {0};
    uint8_t par_h6 = 0;
    uint8_t par_p10 = 0, res_heat_range = 0;
    int8_t par_g1 = 0, par_g3 = 0, res_heat_val = 0;
    int16_t par_g2 = 0;
    bool allowHighTemps = false;

    // CALIBRATED READINGS
    double t_fine = 0, p_fine = 0, h_fine = 0, g_fine = 0, g_res = 0;

    // Gas Sensor Profile data
    uint8_t measProfile = 0, targetTemp = 0, targetWaitTime = 0;

    // Pressure correction factor
    float cf_p = BME_688_GAS_CORRECTION_NIL;

    int32_t readRawTemp();
    int32_t readRawPres();
    int16_t readRawHum();
    int16_t readRawGas();
    double readUCTemp(int32_t adc_T);
    double readUCPres(int32_t adc_P);
    double readUCHum(int16_t adc_H);
    uint8_t readUCGas(uint16_t adc_G);
    double startGasMeasurement(uint8_t profile, uint8_t waitTime);
    bool setHeatProfiles();
    bool checkGasMeasurementCompletion();
    void printLog(String log);
    void readCalibParams();
    // I2C communication methods
    void i2c_execute(uint8_t reg, uint8_t data);
    void i2c_execute_16bit(uint8_t reg, uint16_t data);
    bool i2c_readByte(uint8_t reg, uint8_t *const data, uint8_t length = 1);
    bool i2c_readByte(uint8_t reg, int8_t *const data, uint8_t length = 1);
    void startTransmission(uint8_t reg);
    bool is_sensor_connected();

    // Template methods for reading various bit lengths
    template <typename T> bool i2c_read_Xbit_LE(uint8_t reg, T *const data, uint8_t length);

    template <typename T> bool i2c_read_Xbit(uint8_t reg, T *const data, uint8_t length);
};

#endif // __cplusplus
#endif // BME688_H