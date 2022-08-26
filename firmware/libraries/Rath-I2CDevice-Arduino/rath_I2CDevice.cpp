/**
 * @file rath_I2CDevice.cpp
 * @date 2021-10-25
 * 
 * @copyright Copyright (c) 2021
 * 
 */

/* I was hesitated on whether making I2C an interface object or a device object
  The native Arduino implements I2C as an interface object, so we can directly call
  I2C.method(). However, this apporach has two pitfalls:
  
  1. It will be difficult to implement multi-port I2C support. An example would be 
  STM32 devices, which has multiple I2C peripherals. To make things even worse, they
  support mapping a single I2C peripheral to different sets of GPIO pins (AFIO functionality).
  
  2. It will be hard to implement actual I2C device libraries based on this structure, partially
  due to the reason above.

  Thus, I've decided that it would be more optimal to implement I2C as device objects. Each actual
  I2C device will inherit from the I2CDevice class, and instantiate its own instance.

  A downside of this method would be that memory usage becomes a concern, especially in cases
  like `example_1_scan_bus`, where we need to iterate over all possible I2C addresses.
  This also makes it nearly impossible to implement functionalities like auto-searching for 
  I2C device for some particular I2C devices. I may consider adding a class method for such 
  cases, if necessary later on.
*/

#include "rath_I2CDevice.h"

using namespace rath;

I2CDevice::I2CDevice(I2CPort port, uint16_t device_addr) {
  _device_addr = device_addr;
}

void I2CDevice::init() {
  Wire.begin();
}

Status I2CDevice::receive(uint8_t *buffer, uint16_t size, uint32_t timeout) {
  //twi_readFrom(_device_addr, buffer, size, 1);

  uint8_t status = Wire.requestFrom((int)_device_addr, size);
  if (!status) return ERROR;
  while (Wire.available() < size) {}
  for (int i=0; i<size; i+=1) {
      buffer[i] = Wire.read();
  }
  return OK;
}
    
Status I2CDevice::transmit(uint8_t *buffer, uint16_t size, uint32_t timeout) {
  //twi_writeTo(_device_addr, buffer, size, 1, 1);

  Wire.beginTransmission((int)_device_addr);
  for (int i=0; i<size; i+=1) {
      Wire.write(buffer[i]);
  }
  uint8_t status = Wire.endTransmission();
  return status == 0 ? OK : ERROR;
}

Status I2CDevice::readMemory(uint8_t mem_addr, uint8_t *buffer, uint16_t size, uint32_t timeout) {
  Status status = transmit(&mem_addr, 1, timeout);
  if (status != OK) return status;

  status = receive(buffer, size, timeout);
  return status;
}

Status I2CDevice::writeMemory(uint8_t mem_addr, uint8_t *buffer, uint16_t size, uint32_t timeout) {
  uint8_t buf[size+1];
  buf[0] = mem_addr;
  for (int i=0; i<size; i+=1) {
      buf[i+1] = buffer[i];
  }
  Status status = transmit(buf, size + 1, timeout);
  return status;
}
