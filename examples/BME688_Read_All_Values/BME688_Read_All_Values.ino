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

BME688 sensor;  // Create an instance of the BME688 sensor object

void setup() {
    // Initialize serial communication at 115200 baud rate
    Serial.begin(115200);
    
    // Wait for serial port to connect (needed for native USB)
    while (!Serial) {
        delay(10);
    }

    // Initialize the BME688 sensor
    if (sensor.begin()) {
        Serial.println("BME688 Initialized Successfully!");
    } else {
        Serial.println("Failed to initialize BME688!");
        // Halt program execution if initialization fails
        while (1);
    }
}

void loop() {
    // Read and display temperature
    Serial.print("Temperature: ");
    Serial.print(sensor.readTemperature());  // Read temperature in °C
    Serial.println(" °C");

    // Read and display pressure
    Serial.print("Pressure: ");
    Serial.print(sensor.readPressure());  // Read pressure in Pascals
    Serial.println(" Pa");

    // Read and display humidity
    Serial.print("Humidity: ");
    Serial.print(sensor.readHumidity());  // Read relative humidity in %
    Serial.println(" %");

    // Read and display gas resistance (using profile 0)
    Serial.print("Gas Resistance: ");
    Serial.print(sensor.readGas(0));  // Read gas resistance in ohms
    Serial.println(" Ω");

    // Add a separator line between readings
    Serial.println("-----------------------");

    // Wait 2 seconds before next reading
    delay(2000);
}