/**
 **************************************************
 * @file        BME688-Soldered.h
 * @brief       Header file for BME688 environmental sensor library
 *              All the logic is contained within the libs/folder
 *
 * @copyright   GNU General Public License v3.0
 * @authors     Original Author: Bosch
 *              Modifications by: Josip Šimun Kuči @ Soldered.com
 * @date        Last modified: 2025-07-23
 ***************************************************/

#ifndef BME688_SOLDERED_H
#define BME688_SOLDERED_H

#include "Arduino.h"
#include "libs/Bosch-BME68x-Library/bme68x/bme68x_defs.h"
#include "libs/Bosch-BME68x-Library/bme68xLibrary.h"
#include <Wire.h>

class BME688 : public Bme68x
{
  public:
    BME688() : Bme68x() {};
    void begin(uint8_t address = BME68X_I2C_ADDR_LOW, bme68x_delay_us_fptr_t idleTask = bme68xDelayUs)
    {
        Wire.begin();
        Bme68x::begin(address, Wire, idleTask);
    }
};

class BME688Data : public bme68xData
{
};

#endif // BME688_SOLDERED_H