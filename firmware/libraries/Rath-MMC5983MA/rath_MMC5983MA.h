/**
 * @file rath_MMC5983MA.h
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
  class MMC5983MA : public I2CDevice {
    public:
      struct DataFrame {
        Status status;
        float x;
        float y;
        float z;
      };

      // use default I2CDevice constructor
      MMC5983MA() : I2CDevice(I2C0, I2C_ADDR) {};

      void init();

      void reset();

      uint8_t getID();

      void performSET();
      
      void performRESET();

      DataFrame performMeasurement();
      DataFrame calcOffset();
      DataFrame get();
      DataFrame getOffset();

      float getX();
      float getY();
      float getZ();

      float getTemperature();

    private:
      static const uint16_t I2C_ADDR = 0x30;

      static const uint8_t REG_X_OUT0 = 0x00;
      static const uint8_t REG_X_OUT1 = 0x01;
      static const uint8_t REG_Y_OUT0 = 0x02;
      static const uint8_t REG_Y_OUT1 = 0x03;
      static const uint8_t REG_Z_OUT0 = 0x04;
      static const uint8_t REG_Z_OUT1 = 0x05;
      static const uint8_t REG_XYZ_OUT2 = 0x06;
      static const uint8_t REG_T_OUT = 0x07;
      static const uint8_t REG_STATUS = 0x08;
      static const uint8_t REG_INTERNAL_CONTROL_0 = 0x09;
      static const uint8_t REG_INTERNAL_CONTROL_1 = 0x0A;
      static const uint8_t REG_INTERNAL_CONTROL_2 = 0x0B;
      static const uint8_t REG_INTERNAL_CONTROL_3 = 0x0C;
      static const uint8_t REG_PRODUCT_ID = 0x2F;
      
      static const uint8_t XYZ_OUT2_X_OUT_MSK = 0xC0;
      static const uint8_t XYZ_OUT2_Y_OUT_MSK = 0x30;
      static const uint8_t XYZ_OUT2_Z_OUT_MSK = 0x0C;

      static const uint8_t STATUS_MEAS_M_DONE_MSK = 0x01;
      static const uint8_t STATUS_MEAS_T_DONE_MSK = 0x02;
      static const uint8_t STATUS_OTP_READ_DONE_MSK = 0x10;

      static const uint8_t INTERNAL_CONTROL_0_TM_M_MSK = 0x01;
      static const uint8_t INTERNAL_CONTROL_0_TM_T_MSK = 0x02;
      static const uint8_t INTERNAL_CONTROL_0_INT_MEAS_DONE_EN_MSK = 0x04;
      static const uint8_t INTERNAL_CONTROL_0_SET_MSK = 0x08;
      static const uint8_t INTERNAL_CONTROL_0_RESET_MSK = 0x10;
      static const uint8_t INTERNAL_CONTROL_0_AUTO_SR_EN_MSK = 0x20;
      static const uint8_t INTERNAL_CONTROL_0_OTP_READ_MSK = 0x40;
      
      static const uint8_t INTERNAL_CONTROL_1_BW0_MSK = 0x01;
      static const uint8_t INTERNAL_CONTROL_1_BW1_MSK = 0x02;
      static const uint8_t INTERNAL_CONTROL_1_X_INHIBIT_MSK = 0x04;
      static const uint8_t INTERNAL_CONTROL_1_YZ_INHIBIT_MSK = 0x08;
      static const uint8_t INTERNAL_CONTROL_1_SW_RESET_MSK = 0x80;

      static const uint8_t INTERNAL_CONTROL_2_CM_FREQ_MSK = 0x01;
      static const uint8_t INTERNAL_CONTROL_2_CMM_EN_MSK = 0x08;
      static const uint8_t INTERNAL_CONTROL_2_PRD_SET_MSK = 0x10;
      static const uint8_t INTERNAL_CONTROL_2_EN_PRD_SET_MSK = 0x80;

      static const uint8_t INTERNAL_CONTROL_3_ST_ENP_MSK = 0x02;
      static const uint8_t INTERNAL_CONTROL_3_ST_ENM_MSK = 0x04;
      static const uint8_t INTERNAL_CONTROL_3_SPI_3W_MSK = 0x40;

      static constexpr float resolution = 100.f / 16384.f;
      DataFrame measurement;
      DataFrame offset;
  };
};
