/**
 * @file rath_I2CDevice.h
 * @date 2021-10-25
 * 
 * @copyright Copyright (c) 2021
 * 
 * Library for driving the I2C slave devices on Arduino platform.
 * 
 */

#pragma once

#include "rath_essential.h"
#include "Wire.h"

namespace rath {
  enum I2CPort {
    I2C0,
    I2C1
  };

  class I2CDevice {
    public:
      /**
       * @brief Construct a new I2CDevice object
       * 
       * @param device_addr 
       * @param port 
       */
      I2CDevice(I2CPort port, uint16_t device_addr);

      /**
       * @brief Initialize the I2C device. The microcontroller is in master mode.
       * 
       */
      void init();

      /**
       * @brief Receive `size` number of bytes from the I2C device. The result will be put into `buffer`.
       * 
       * @param buffer buffer to hold received data
       * @param size number of bytes to receive
       * @param timeout timeout, not implemented
       * @return Status 
       */
      Status receive(uint8_t *buffer, uint16_t size, uint32_t timeout);
      
      /**
       * @brief Transmit `size` number of bytes to the I2C device. The data to transmit will be read from `buffer`.
       * 
       * @param buffer data to be transmitted
       * @param size number of bytes to send
       * @param timeout timeout, not implemented
       * @return Status
       */
      Status transmit(uint8_t *buffer, uint16_t size, uint32_t timeout);

      /**
       * @brief Read `size` number of bytes from register at `mem_addr` of the I2C device. The result will be put into `buffer`.
       * 
       * @param mem_addr target memory address to read from
       * @param buffer buffer to hold received data
       * @param size number of bytes to receive
       * @param timeout timeout, not implemented
       * @return Status 
       */
      Status readMemory(uint8_t mem_addr, uint8_t *buffer, uint16_t size, uint32_t timeout);

      /**
      * @brief Write `size` number of bytes to register at `mem_addr` of the I2C device. The data to transmit will be read from `buffer`.
      * 
      * @param mem_addr target memory address to write to
      * @param buffer data to be transmitted
      * @param size number of bytes to receive
      * @param timeout timeout, not implemented
      * @return Status 
      */
      Status writeMemory(uint8_t mem_addr, uint8_t *buffer, uint16_t size, uint32_t timeout);
    
    // private:
      uint16_t _device_addr;
  };
};
