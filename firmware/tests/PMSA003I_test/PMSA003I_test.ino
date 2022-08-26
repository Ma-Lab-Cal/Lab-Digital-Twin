/**
 * @file example_1_read_data.ino
 * @date 2022-03-08
 *
 * @copyright Copyright (c) 2022
 *
 * Read air partical concentration data from PMSA003I.
 *
 */

#include "rath_PMSA003I.h"

rath::PMSA003I sensor = rath::PMSA003I();  // I2C address for PMSA003I is 0x12

void setup() {
  Serial.begin(115200);

  sensor.init();

  Serial.println("ready.");
}

void loop() {
  rath::PMSA003I::DataFrame data = sensor.get();

  if (data.error_code != 0) {
    Serial.print("Error reading sensor, error code: ");
    Serial.println(data.error_code);
    return;
  }

  Serial.println("================================");
  Serial.println(" Partical Concentration Reading");
  Serial.println("================================");
  Serial.println("Concentration Units (standard)");
  Serial.print("  PM 1.0: ");   Serial.println(data.std_pm_1_0);
  Serial.print("  PM 2.5: ");   Serial.println(data.std_pm_2_5);
  Serial.print("  PM 10: ");    Serial.println(data.std_pm_10);
  Serial.println();
  Serial.println("Concentration Units (raw)");
  Serial.print("  PM 1.0: ");   Serial.println(data.raw_pm_1_0);
  Serial.print("  PM 2.5: ");   Serial.println(data.raw_pm_2_5);
  Serial.print("  PM 10: ");    Serial.println(data.raw_pm_10);
  Serial.println();
  Serial.print("Particles > 0.3um / 0.1L air:"); Serial.println(data.num_particles_0_3_um);
  Serial.print("Particles > 0.5um / 0.1L air:"); Serial.println(data.num_particles_0_5_um);
  Serial.print("Particles > 1.0um / 0.1L air:"); Serial.println(data.num_particles_1_um);
  Serial.print("Particles > 2.5um / 0.1L air:"); Serial.println(data.num_particles_2_5_um);
  Serial.print("Particles > 5.0um / 0.1L air:"); Serial.println(data.num_particles_5_um);
  Serial.print("Particles > 10 um / 0.1L air:"); Serial.println(data.num_particles_10_um);
  Serial.println("================================");
  Serial.println();
  Serial.println();
  
  delay(1000);
}
