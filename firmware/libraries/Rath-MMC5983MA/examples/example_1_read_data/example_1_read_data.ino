/**
 * @file example_1_read_data.ino
 * @date 2022-03-08
 *
 * @copyright Copyright (c) 2022
 *
 * Read magnetic field data from MMC5983MA.
 *
 */

#include "rath_MMC5983MA.h"

rath::MMC5983MA sensor = rath::MMC5983MA();

void setup() {
  Serial.begin(115200);

  sensor.init();

  Serial.println("ready.");

  Serial.print("Sensor ID: "); Serial.println(sensor.getID());
}

void loop() {
  rath::MMC5983MA::DataFrame data = sensor.get();

  if (data.status != rath::OK) {
    Serial.print("Error reading sensor, error code: ");
    Serial.println(data.status);
    return;
  }
  
  float x = data.x;
  float y = data.y;
  float z = data.z;
  
  float temperature = sensor.getTemperature();

  Serial.print("x: ");  Serial.print(x);  Serial.print(" mT\t");
  Serial.print("y: ");  Serial.print(y);  Serial.print(" mT\t");
  Serial.print("z: ");  Serial.print(z);  Serial.print(" mT\t");
  Serial.print("temperature: ");  Serial.print(temperature);  Serial.print(" C");
  Serial.println();

  delay(100);
}