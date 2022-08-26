/**
 * @file rath_TSL2591.h
 * @date 2022-02-08
 *
 * @copyright Copyright (c) 2022
 *
 * Library for driving the PMSA003I digital universal partical concentration sensor.
 */

#pragma once

#include "rath_time.h"
#include "rath_I2CDevice.h"

namespace rath {
  class TSL2591 : public I2CDevice {
    public:
      enum class Gain {
        GAIN_LOW = 0b00,  // Can't use "LOW" here, as LOW and HIGH are defined as Arduino macros...
        GAIN_MEDIUM = 0b01,
        GAIN_HIGH = 0b10,
        GAIN_MAXIMUM = 0b11,
      };
      enum class IntegrationTime {
        T_100_MS = 0b000,  // maximum 36863 count
        T_200_MS = 0b001,  // maximum 65535 count
        T_300_MS = 0b010,  // maximum 65535 count
        T_400_MS = 0b011,  // maximum 65535 count
        T_500_MS = 0b100,  // maximum 65535 count
        T_600_MS = 0b101,  // maximum 65535 count
      };

      // use default I2CDevice constructor
      TSL2591() : I2CDevice(I2C0, I2C_ADDR) {};

      /**
       * @brief Initialize the I2C device. The microcontroller is in master mode.
       *
       */
      void init();

      /**
       * @brief Perform a software reset. It will clear all registers and read OTP
       * as part of its startup routine. The power on time is 10 ms.
       * 
       */
      void reset();

      /**
       * @brief Read measurement data.
       *
       */
      uint8_t getID();

      void disable();
      
      void enable();

      void setGain(Gain gain);

      void setIntegrationTime(IntegrationTime time);
      
      float get();
      

    private:
      static const uint16_t I2C_ADDR = 0x29;

      static constexpr float LUX_COEFFICIENT = 408.0F; // Lux coefficient

      static const uint8_t REG_ENABLE = 0x00;
      static const uint8_t REG_CONTROL = 0x01;
      static const uint8_t REG_AILTL = 0x04;
      static const uint8_t REG_AILTH = 0x05;
      static const uint8_t REG_AIHTL = 0x06;
      static const uint8_t REG_AIHTH = 0x07;
      static const uint8_t REG_NPAILTL = 0x08;
      static const uint8_t REG_NPAILTH = 0x09;
      static const uint8_t REG_NPAIHTL = 0x0A;
      static const uint8_t REG_NPAIHTH = 0x0B;
      static const uint8_t REG_PERSIST = 0x0C;
      static const uint8_t REG_PID = 0x11;
      static const uint8_t REG_ID = 0x12;
      static const uint8_t REG_STATUS = 0x13;
      static const uint8_t REG_C0DATAL = 0x14;
      static const uint8_t REG_C0DATAH = 0x15;
      static const uint8_t REG_C1DATAL = 0x16;
      static const uint8_t REG_C1DATAH = 0x17;
      
      static const uint8_t COMMAND_CMD_NORMAL_MSK = 0xA0;
      
      static const uint8_t ENABLE_PON_MSK = 0x01;
      static const uint8_t ENABLE_AEN_MSK = 0x02;
      static const uint8_t ENABLE_AIEN_MSK = 0x10;
      static const uint8_t ENABLE_SAI_MSK = 0x40;
      static const uint8_t ENABLE_NPIEN_MSK = 0x80;

      static const uint8_t CONTROL_ATIME_MSK = 0x07;
      static const uint8_t CONTROL_AGAIN_MSK = 0x30;
      static const uint8_t CONTROL_SRESET_MSK = 0x80;
      

      static constexpr float resolution = 100.f / 16384.f;

      Gain _gain = Gain::GAIN_LOW;
      IntegrationTime _integration_time = IntegrationTime::T_100_MS;

      void _updateSettings();
  };
};
