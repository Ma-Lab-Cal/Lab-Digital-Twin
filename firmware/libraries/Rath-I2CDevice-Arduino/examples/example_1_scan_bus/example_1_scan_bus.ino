/**
 * @file example_1_scan_bus.ino
 * @date 2021-10-29
 *
 * @copyright Copyright (c) 2021
 *
 * Scans the I2C bus to discover connected devices' addresses.
 *
 */

#include "rath_I2CDevice.h"

// Arduino currently only support 7-bit I2C address
const uint16_t ADDR_START = 1;
const uint16_t ADDR_END = 127;

void setup() {
  Serial.begin(115200);
}

void loop() {
  // a dummy byte for I2C payload
  uint8_t dummy = 0;

  Serial.println("Start scanning...");

  for (uint16_t device_addr = ADDR_START; device_addr < ADDR_END; device_addr += 1) {
    Serial.print("Testing address 0x");
    if (device_addr < 16) Serial.print("0");
    Serial.print(device_addr, HEX);
    Serial.println("...");

    rath::I2CDevice i2c_device = rath::I2CDevice(rath::I2C0, device_addr);
    
    i2c_device.init();
    
    rath::Status status = i2c_device.transmit(&dummy, 1, 0);

    if (status == rath::OK) {
      Serial.print("I2C device found at address 0x");
      if (device_addr < 16) Serial.print("0");
      Serial.print(device_addr, HEX);
      Serial.println("");
    }
  }

  Serial.println("Finish scanning.");

  delay(5000);
}