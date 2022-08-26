/**
 * @file example_1_read_data.ino
 * @date 2022-03-08
 *
 * @copyright Copyright (c) 2022
 *
 * Read magnetic field data from MMC5983MA.
 *
 */

#include "rath_TSL2591.h"

rath::TSL2591 sensor = rath::TSL2591();  // I2C address for PMSA003I is 0x12

void setup() {
  Serial.begin(115200);

  sensor.init();

  sensor.setGain(rath::TSL2591::Gain::GAIN_MEDIUM);
  sensor.setIntegrationTime(rath::TSL2591::IntegrationTime::T_500_MS);
  
  Serial.println("ready.");
}

void loop() {
//  Serial.print("Sensor ID: "); Serial.println(sensor.getID());

  float data = sensor.get();

  Serial.print("data: "); Serial.println(data);
  
  delay(100);
}
