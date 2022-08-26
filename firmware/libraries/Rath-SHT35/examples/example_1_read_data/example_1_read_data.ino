/**
 * @file example_1_read_data.ino
 * @date 2022-02-08
 *
 * @copyright Copyright (c) 2022
 *
 * Read temperature and humidity data from SHT35.
 *
 */

#include "rath_SHT35.h"

rath::SHT35 sensor = rath::SHT35(rath::I2C0, 0x45);  // I2C address for SHT35 is 0x44 or 0x45

void setup() {
  Serial.begin(115200);

  sensor.init();
  sensor.disableHeater();

  Serial.println("ready.");
}

void loop() {
  float temperature = sensor.getTemperature();
  float humidity = sensor.getHumidity();

  Serial.print("temperature: ");
  Serial.print(temperature);
  Serial.print(" C\t");
  Serial.print("humidity: ");
  Serial.print(humidity * 100);  // convert to percentage
  Serial.print(" %");
  Serial.println();

  delay(1000);
}
