/**
 * @file example_3_heater_control.ino
 * @date 2022-02-08
 *
 * @copyright Copyright (c) 2022
 *
 * Control the builtin heater.
 *
 */

#include "rath_SHT35.h"

rath::SHT35 sensor = rath::SHT35(rath::I2C0, 0x45);  // I2C address for SHT35 is 0x44 or 0x45

void setup() {
  Serial.begin(115200);

  sensor.init();

  Serial.println("ready.");
}

int counter = 0;
bool is_heater_on = false;

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

  // toggle the heater every 10 seconds
  if (counter > 100) {
    if (is_heater_on) {
      sensor.disableHeater();
      is_heater_on = false;
      Serial.println("Heater OFF");
    }
    else {
      sensor.enableHeater();
      is_heater_on = true;
      Serial.println("Heater ON");
    }
    counter = 0;
  }

  counter += 1;

  delay(100);
}
