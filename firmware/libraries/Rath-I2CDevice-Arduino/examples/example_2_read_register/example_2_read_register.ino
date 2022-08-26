/**
 * @file example_2_read_register.ino
 * @date 2021-10-29
 *
 * @copyright Copyright (c) 2021
 *
 * Reads a register from connected I2C device
 *
 */

#include "rath_I2CDevice.h"

uint8_t device_addr = 0x1;
uint8_t reg_addr = 0x2;

rath::I2CDevice i2c_device = rath::I2CDevice(rath::I2C0, device_addr);

void setup() {
  Serial.begin(115200);

  i2c_device.init();
}

void loop() {
  uint8_t buffer;

  rath::Status status = i2c_device.readMemory(reg_addr, &buffer, 1, 0);

  // if device does not respond
  if (status != rath::OK) {
    Serial.println("Error reading I2C device");
    delay(2000);  // we wait a bit longer
    return;  // and try again
  }

  Serial.print("read register value: ");
  Serial.println(buffer, HEX);

  delay(1000);
}
