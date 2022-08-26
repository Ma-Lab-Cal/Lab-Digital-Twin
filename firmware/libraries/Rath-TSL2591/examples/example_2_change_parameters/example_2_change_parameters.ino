/**
 * @file example_2_change_parameters.ino
 * @date 2022-07-09
 *
 * @copyright Copyright (c) 2022
 *
 * Read light intensity data from TSL2591.
 *
 */

#include "rath_TSL2591.h"

rath::TSL2591 sensor = rath::TSL2591();

void setup() {
  Serial.begin(115200);

  sensor.init();

  sensor.setGain(rath::TSL2591::Gain::GAIN_MEDIUM);
  sensor.setIntegrationTime(rath::TSL2591::IntegrationTime::T_600_MS);
  
  Serial.println("ready.");

  Serial.print("Sensor ID: "); Serial.println(sensor.getID());
}

void loop() {
  float lux = sensor.get();

  Serial.print("lux: ");  Serial.print(lux);  Serial.print(" lx\t");
  Serial.println();

  delay(100);
}