/**
 * @file rath_PMSA003I.h
 * @date 2022-02-08
 *
 * @copyright Copyright (c) 2022
 *
 * Library for driving the PMSA003I digital universal partical concentration sensor.
 */

#pragma once

#include "rath_I2CDevice.h"

namespace rath {
  class PMSA003I : public I2CDevice {
    public:
      struct DataFrame {
        // difference between the std (standard particle) pm reading and the
        // raw (environmental) pm reading can be found here:
        // https://publiclab.org/questions/samr/04-07-2019/how-to-interpret-pms5003-sensor-values
        //
        // but basically, the std reading is used for reporting and cross-comparision,
        // while the raw is used for realtime monitoring and analysis

        uint16_t std_pm_1_0;
        uint16_t std_pm_2_5;
        uint16_t std_pm_10;
        uint16_t raw_pm_1_0;
        uint16_t raw_pm_2_5;
        uint16_t raw_pm_10;
        uint16_t num_particles_0_3_um;
        uint16_t num_particles_0_5_um;
        uint16_t num_particles_1_um;
        uint16_t num_particles_2_5_um;
        uint16_t num_particles_5_um;
        uint16_t num_particles_10_um;
        uint8_t version;
        uint8_t error_code;
      };

      // use default I2CDevice constructor
      PMSA003I() : I2CDevice(I2C0, I2C_ADDR) {};
      
      /**
       * @brief Read measurement data.
       *
       */
      DataFrame get();

    private:
      static const uint16_t I2C_ADDR = 0x12;

      DataFrame data_frame;
  };
};
