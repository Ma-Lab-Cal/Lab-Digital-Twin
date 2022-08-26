/**
 * @file rath_SHT35.h
 * @date 2022-02-08
 * 
 * @copyright Copyright (c) 2022
 *
 * Library for driving the Sensirion SHT35 humidity and temperature sensor.
 */

#pragma once

#include "rath_I2CDevice.h"

namespace rath {
  class SHT35 : public I2CDevice {
    public:
      // use default I2CDevice constructor
      using I2CDevice::I2CDevice;

      /**
       * @brief Initialize the I2C device. The microcontroller is in master mode.
       * 
       */
      void init();

      /**
       * @brief Turn off the SHT35 internal heater.
       * 
       */
      void disableHeater();

      /**
       * @brief Turn on the SHT35 internal heater.
       * 
       */
      void enableHeater();

      /**
       * @brief Read temperature readout from the sensor. 
       * Returns -274 if failed to get a valid sensor reading.
       * 
       * @return <float> temperature in degree Celsius
       */
      float getTemperature();
      
      /**
       * @brief Read temperature readout from the sensor. 
       * Returns -1 if failed to get a valid sensor reading.
       * 
       * @return <float> temperature in Kelvin
       */
      float getTemperatureK();

      /**
       * @brief Read humidity readout from the sensor.
       * Returns -1 if failed to get a valid sensor reading.
       * 
       * @return <float> humidity in range from 0.0 to 1.0
       */
      float getHumidity();
    
    private:
      Status _takeSingleMeasurement(uint8_t *buffer);

      // using const instead of define due to the rationale here: 
      // https://www.baldengineer.com/const-vs-define-when-do-you-them-and-why.html
      // and https://stackoverflow.com/questions/11779361/static-const-vs-define-in-c-differences-in-executable-size

      // because we are mapping uint16_t to two uint8_t as an array, and because
      // we are on an little-endian platform, MSB and LSB are reversed here.

      // single shot data acquisition mode
      static const uint16_t CLOCK_STRETCHING_ON_REPEATABILITY_HIGH = 0x062C;
      static const uint16_t CLOCK_STRETCHING_ON_REPEATABILITY_MEDIUM = 0x0D2C;
      static const uint16_t CLOCK_STRETCHING_ON_REPEATABILITY_LOW = 0x102C;
      static const uint16_t CLOCK_STRETCHING_OFF_REPEATABILITY_HIGH = 0x0024;
      static const uint16_t CLOCK_STRETCHING_OFF_REPEATABILITY_MEDIUM = 0x0B24;
      static const uint16_t CLOCK_STRETCHING_OFF_REPEATABILITY_LOW = 0x1624;

      // periodic data acquisition mode
      static const uint16_t MPS_0_5_REPEATABILITY_HIGH = 0x3220;
      static const uint16_t MPS_0_5_REPEATABILITY_MEDIUM = 0x2420;
      static const uint16_t MPS_0_5_REPEATABILITY_LOW = 0x2F20;
      static const uint16_t MPS_1_REPEATABILITY_HIGH = 0x3021;
      static const uint16_t MPS_1_REPEATABILITY_MEDIUM = 0x2621;
      static const uint16_t MPS_1_REPEATABILITY_LOW = 0x2D21;
      static const uint16_t MPS_2_REPEATABILITY_HIGH = 0x3622;
      static const uint16_t MPS_2_REPEATABILITY_MEDIUM = 0x2022;
      static const uint16_t MPS_2_REPEATABILITY_LOW = 0x2B22;
      static const uint16_t MPS_4_REPEATABILITY_HIGH = 0x3423;
      static const uint16_t MPS_4_REPEATABILITY_MEDIUM = 0x2223;
      static const uint16_t MPS_4_REPEATABILITY_LOW = 0x2923;
      static const uint16_t MPS_10_REPEATABILITY_HIGH = 0x3727;
      static const uint16_t MPS_10_REPEATABILITY_MEDIUM = 0x2127;
      static const uint16_t MPS_10_REPEATABILITY_LOW = 0x2A27;

      // accelerated response time command
      static const uint16_t ART = 0x322B;

      // break command (stop periodic data acquisition)
      static const uint16_t BREAK = 0x9330;

      static const uint16_t SOFT_RESET = 0xA230;

      static const uint16_t HEATER_ENABLE = 0x6D30;
      static const uint16_t HEATER_DISABLE = 0x6630;
      
      static const uint16_t READ_STATUS_REGISTER = 0x2DF3;
      static const uint16_t CLEAR_STATUS_REGISTER = 0x4130;
  };
};
