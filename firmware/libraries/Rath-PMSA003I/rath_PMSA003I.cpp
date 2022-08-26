/**
 * @file rath_PMSA003I.cpp
 * @date 2022-02-08
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "rath_PMSA003I.h"

using namespace rath;

PMSA003I::DataFrame PMSA003I::get() {
  uint8_t buffer[32];
  
  receive(buffer, 32, 0);

  if (buffer[0] != 0x42 || buffer[1] != 0x4D) {
    data_frame.error_code = 1;
    return data_frame;
  }

  uint16_t checksum = 0;
  for (uint16_t i=0; i<30; i+=1) {
    checksum += buffer[i];
  }
  if (checksum != (((uint16_t)buffer[30] << 8) | buffer[31])) {
    data_frame.error_code = 2;
    return data_frame;
  }

  data_frame.std_pm_1_0 = (((uint16_t)buffer[0x04] << 8) | buffer[0x05]);
  data_frame.std_pm_2_5 = (((uint16_t)buffer[0x06] << 8) | buffer[0x07]);
  data_frame.std_pm_10 = (((uint16_t)buffer[0x08] << 8) | buffer[0x09]);
  data_frame.raw_pm_1_0 = (((uint16_t)buffer[0x0A] << 8) | buffer[0x0B]);
  data_frame.raw_pm_2_5 = (((uint16_t)buffer[0x0C] << 8) | buffer[0x0D]);
  data_frame.raw_pm_10 = (((uint16_t)buffer[0x0E] << 8) | buffer[0x0F]);
  data_frame.num_particles_0_3_um = (((uint16_t)buffer[0x10] << 8) | buffer[0x11]);
  data_frame.num_particles_0_5_um = (((uint16_t)buffer[0x12] << 8) | buffer[0x13]);
  data_frame.num_particles_1_um = (((uint16_t)buffer[0x14] << 8) | buffer[0x15]);
  data_frame.num_particles_2_5_um = (((uint16_t)buffer[0x16] << 8) | buffer[0x17]);
  data_frame.num_particles_5_um = (((uint16_t)buffer[0x18] << 8) | buffer[0x19]);
  data_frame.num_particles_10_um = (((uint16_t)buffer[0x1A] << 8) | buffer[0x1B]);
  data_frame.version = buffer[0x1C];
  data_frame.error_code = buffer[0x1D];

  return data_frame;
}
