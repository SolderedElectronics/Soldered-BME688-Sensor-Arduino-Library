/**
 **************************************************
 *
 * @file        BME688_Read_All_Values.ino
 * 
 * @brief       example demonstrates how to read temperature, pressure, humidity,
 *              and gas resistance from the BME688 sensor and display the values
 *              via serial monitor.
 *
 * @link        solde.red/333203
 *
 * @authors     Josip Šimun Kuči @ soldered.com
 ***************************************************/
#include "BME688-Soldered.h"  // Include the BME688 library

BME688 bme;  // Create an instance of the BME688 sensor object

BME688Data data; // Create an instance of the class which stores data recorded by the sensor

void setup(void)
{
    // Initialize serial so we can print to the monitor
	Serial.begin(115200);
	
		
	// Initialize the BME sensor via I2C at the default address
	bme.begin();

    // Check if the sensor initialization was successfull
	if(bme.checkStatus())
	{
		if (bme.checkStatus() == BME68X_ERROR)
		{
			Serial.println("Sensor error:" + bme.statusString());
			return;
		}
		else if (bme.checkStatus() == BME68X_WARNING)
		{
			Serial.println("Sensor Warning:" + bme.statusString());
		}
	}
	
	// Set the default configuration for temperature, pressure and humidity 
	bme.setTPH();

	// Set the heater configuration to 300 deg C for 100ms for Forced mode 
	bme.setHeaterProf(300, 100);

	Serial.println("TimeStamp(ms), Temperature(deg C), Pressure(Pa), Humidity(%), Gas resistance(ohm), Status");
}

void loop(void)
{
    //Set the BME into forced operating mode
	bme.setOpMode(BME68X_FORCED_MODE);
	delayMicroseconds(bme.getMeasDur());

    // See if there's data available from the sensor
	if (bme.fetchData())
	{
		bme.getData(data);
		Serial.print(String(millis()) + ", ");
		Serial.print(String(data.temperature) + ", ");
		Serial.print(String(data.pressure) + ", ");
		Serial.print(String(data.humidity) + ", ");
		Serial.print(String(data.gas_resistance) + ", ");
		Serial.println(data.status, HEX);
	}
}