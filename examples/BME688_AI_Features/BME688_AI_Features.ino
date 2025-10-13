/**
 **************************************************
 *
 * @file        BME688_AIFeatures.ino
 * 
 * @brief       Example which enables the use of bosch AI Studio features,
 *              generates a .bmerawdata file which can be read by the BME AI Studio
 *              and used to configure the sensor to detect different smells
 *              See the tutorial here: https://www.bosch-sensortec.com/software/bme/docs/overview/getting-started.html
 *
 * @link        solde.red/333203
 *
 * @authors     Josip Šimun Kuči @ soldered.com
 ***************************************************/
#include "BME688-Soldered.h"
#include <ArduinoJson.h>
#include <WiFi.h>
#include <time.h>
#include <SPI.h>
#include <SD.h>

// Sensor configuration
#define MEAS_DUR 140 // Measure duration in ms
#define NEW_GAS_MEAS (BME68X_GASM_VALID_MSK | BME68X_HEAT_STAB_MSK | BME68X_NEW_DATA_MSK)
#define SD_CS 5 // CS pin used by the SD card
#define LOG_DURATION_MS 10 * 60000UL  // 10 minutes
#define SENSOR_ID 683422375UL // Unique ID for your board, feel free to change to desired value, used by Bosch AI studio

// WiFi credentials
const char *ssid = "YOUR_SSID_HERE";
const char *password = "YOUR_PASSWORD_HERE";


BME688 bme;  // Create an instance of the BME688 sensor object

BME688Data data; // Create an instance of the class which stores data recorded by the sensor

// Set the heater profiles as well as how long they will last
uint16_t tempProf[10] = {320, 100, 100, 100, 200, 200, 200, 320, 320, 320};
uint16_t mulProf[10] = {5, 2, 10, 30, 5, 5, 5, 5, 5, 5};
const int HEATER_LEN = 10;

// Check if the sensor was successfully initialized
void checkSensorStatus() {
  int8_t status = bme.checkStatus();
  if (status == BME68X_ERROR) {
    Serial.println("Sensor error: " + bme.statusString());
    while (1);
  } else if (status == BME68X_WARNING) {
    Serial.println("Sensor warning: " + bme.statusString());
  }
}

// Function which formats the datetime into iso8601 format
String iso8601(time_t t) {
  struct tm tm_info;
  gmtime_r(&t, &tm_info);
  char buf[32];
  strftime(buf, sizeof(buf), "%Y-%m-%dT%H:%M:%S+00:00", &tm_info);
  return String(buf);
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("\nBME68x JSON Logger");

  // Setting up wifi and NTP
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("\nWiFi connected!");
  configTime(0, 0, "pool.ntp.org");
  time_t now = 0;
  while ((now = time(nullptr)) < 1000000000UL) {
    Serial.print(".");
    delay(200);
  }
  Serial.println("\nTime synced via NTP");
  String isoNow = iso8601(now);

  // Initialize the SD card over SPI
  if (!SD.begin(SD_CS)) {
    Serial.println("SD card init failed!");
    while (1);
  }
  Serial.println("SD card ready");

  // Initialize the sensor over I2C at address 0x76
  bme.begin(0x76);
  checkSensorStatus();

  // Set the default Temperature, pressure and humidity oversampling
  bme.setTPH();
  checkSensorStatus();

  uint16_t sharedHeatrDur = MEAS_DUR - (bme.getMeasDur(BME68X_PARALLEL_MODE) / 1000);
  bme.setHeaterProf(tempProf, mulProf, sharedHeatrDur, HEATER_LEN);
  checkSensorStatus();

  bme.setOpMode(BME68X_PARALLEL_MODE);
  checkSensorStatus();

  // Create the file on the SD card
  String filename = "/BME_" + String(now) + ".bmerawdata";
  File file = SD.open(filename, FILE_WRITE);
  if (!file) {
    Serial.println("Could not open file!");
    while (1);
  }

  // Initialize the json format
  DynamicJsonDocument doc(80000);

  JsonObject cfgHead = doc.createNestedObject("configHeader");
  cfgHead["dateCreated_ISO"] = isoNow;
  cfgHead["appVersion"] = "3.1.0";
  cfgHead["boardType"] = "board_8";
  cfgHead["boardMode"] = "burn_in";
  cfgHead["boardLayout"] = "grouped";

  JsonObject cfgBody = doc.createNestedObject("configBody");
  JsonArray heaters = cfgBody.createNestedArray("heaterProfiles");
  JsonObject heater = heaters.createNestedObject();
  heater["id"] = "heater_354";
  heater["timeBase"] = 140;
  JsonArray tempTime = heater.createNestedArray("temperatureTimeVectors");
  for (int i = 0; i < HEATER_LEN; i++) {
    JsonArray pair = tempTime.createNestedArray();
    pair.add(tempProf[i]);
    pair.add(mulProf[i]);
  }

  JsonArray dutyArr = cfgBody.createNestedArray("dutyCycleProfiles");
  JsonObject duty = dutyArr.createNestedObject();
  duty["id"] = "duty_5_10";
  duty["numberScanningCycles"] = 5;
  duty["numberSleepingCycles"] = 10;

  JsonArray sensors = cfgBody.createNestedArray("sensorConfigurations");
  JsonObject sensorCfg = sensors.createNestedObject();
  sensorCfg["sensorIndex"] = 0;
  sensorCfg["heaterProfile"] = "heater_354";
  sensorCfg["dutyCycleProfile"] = "duty_5_10";

  JsonObject rawHead = doc.createNestedObject("rawDataHeader");
  rawHead["counterPowerOnOff"] = 0;
  rawHead["seedPowerOnOff"] = random(100000, 9999999);
  rawHead["counterFileLimit"] = 0;
  rawHead["dateCreated"] = (int)now;
  rawHead["dateCreated_ISO"] = isoNow;
  rawHead["firmwareVersion"] = "1.5.0";
  rawHead["boardId"] = SENSOR_ID;

  JsonObject rawBody = doc.createNestedObject("rawDataBody");

  JsonArray cols = rawBody.createNestedArray("dataColumns");
  const char *keys[][4] = {
    {"Sensor Index","","integer","sensor_index"},
    {"Sensor ID","","integer","sensor_id"},
    {"Time Since PowerOn","Milliseconds","integer","timestamp_since_poweron"},
    {"Real time clock","Unix Timestamp: seconds since Jan 01 1970. (UTC); 0 = missing","integer","real_time_clock"},
    {"Temperature","DegreesCelcius","float","temperature"},
    {"Pressure","Hectopascals","float","pressure"},
    {"Relative Humidity","Percent","float","relative_humidity"},
    {"Resistance Gassensor","Ohms","float","resistance_gassensor"},
    {"Heater Profile Step Index","","integer","heater_profile_step_index"},
    {"Scanning Mode Enabled","","integer","scanning_mode_enabled"},
    {"Label Tag","","integer","label_tag"},
    {"Error Code","","integer","error_code"}};
  for (auto &k : keys) {
    JsonObject c = cols.createNestedObject();
    c["name"] = k[0];
    c["unit"] = k[1];
    c["format"] = k[2];
    c["key"] = k[3];
  }

  JsonArray block = rawBody.createNestedArray("dataBlock");

  // Take measurements
  Serial.println("Starting measurements:");
  unsigned long start = millis();
  uint8_t nFieldsLeft = 0;
  while (millis() - start < LOG_DURATION_MS) {
    delay(MEAS_DUR);
    // Check if data is available
    if (bme.fetchData()) {
      do {
        // Get the data and parse it into the json file
        nFieldsLeft = bme.getData(data);
        Serial.println("T: "+String(data.temperature)+"°C | P:"+String(data.pressure)+"Pa | H: "+String(data.humidity)+"%");
        if (data.status == NEW_GAS_MEAS) {
          JsonArray row = block.createNestedArray();
          row.add(0);
          row.add(SENSOR_ID);
          row.add(millis() - start);
          row.add((int)time(nullptr));
          row.add(data.temperature);
          row.add(data.pressure);
          row.add(data.humidity);
          row.add(data.gas_resistance);
          row.add(data.gas_index);
          row.add(1);
          row.add(0);
          row.add(bme.checkStatus());
        }
        checkSensorStatus();
      } while (nFieldsLeft);
    }
  }

  // Serialize the json into text format
  serializeJson(doc, file);
  file.close();
  Serial.println("Measurement complete, JSON saved!");
  Serial.print("File saved as: ");
  Serial.println(filename);
  WiFi.disconnect(true);
}

void loop() {}
