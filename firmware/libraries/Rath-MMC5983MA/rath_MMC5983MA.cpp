/**
 * @file rath_MMC5983MA.cpp
 * @date 2022-02-08
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "rath_MMC5983MA.h"

using namespace rath;

void MMC5983MA::init() {
  I2CDevice::init();

  reset();
}

void MMC5983MA::reset() {
  uint8_t buf = INTERNAL_CONTROL_1_SW_RESET_MSK;
  writeMemory(REG_INTERNAL_CONTROL_1, &buf, 1, 0);

  // the reset will take 10 ms to complete
  HAL_delay(11);
  
  calcOffset();
}

uint8_t MMC5983MA::getID() {
  uint8_t buf;
  readMemory(REG_PRODUCT_ID, &buf, 1, 0);
  return buf;
}

void MMC5983MA::performSET() {
  uint8_t buf = INTERNAL_CONTROL_0_SET_MSK;
  writeMemory(REG_INTERNAL_CONTROL_0, &buf, 1, 0);
}

void MMC5983MA::performRESET() {
  uint8_t buf = INTERNAL_CONTROL_0_RESET_MSK;
  writeMemory(REG_INTERNAL_CONTROL_0, &buf, 1, 0);
}

MMC5983MA::DataFrame MMC5983MA::performMeasurement() {
  uint8_t buffer[7];
  buffer[0] = INTERNAL_CONTROL_0_TM_M_MSK;
  writeMemory(REG_INTERNAL_CONTROL_0, buffer, 1, 0);
  
  // read status to see if measurement is done
  readMemory(REG_STATUS, buffer, 1, 0);
  while (!READ_BITS(buffer[0], STATUS_MEAS_M_DONE_MSK)) {
    readMemory(REG_STATUS, buffer, 1, 0);
  }

  Status status = readMemory(REG_X_OUT0, buffer, 7, 0);

  uint32_t x = ((uint32_t)buffer[0] << 10) | ((uint32_t)buffer[1] << 2) | ((uint32_t)READ_BITS(buffer[6], XYZ_OUT2_X_OUT_MSK) >> 6);
  uint32_t y = ((uint32_t)buffer[2] << 10) | ((uint32_t)buffer[3] << 2) | ((uint32_t)READ_BITS(buffer[6], XYZ_OUT2_X_OUT_MSK) >> 6);
  uint32_t z = ((uint32_t)buffer[4] << 10) | ((uint32_t)buffer[5] << 2) | ((uint32_t)READ_BITS(buffer[6], XYZ_OUT2_X_OUT_MSK) >> 6);
  
  measurement.status = status;
  measurement.x = (float)x * resolution;
  measurement.y = (float)y * resolution;
  measurement.z = (float)z * resolution;

  return measurement;
}

MMC5983MA::DataFrame MMC5983MA::calcOffset() {
  performSET();
  performMeasurement();
  float x = measurement.x;
  float y = measurement.y;
  float z = measurement.z;

  HAL_delay(1);

  performRESET();
  performMeasurement();
  offset.x = -.5 * (measurement.x + x);
  offset.y = -.5 * (measurement.y + y);
  offset.z = -.5 * (measurement.z + z);
  
  HAL_delay(1);

  return offset;
}

MMC5983MA::DataFrame MMC5983MA::get() {
  performMeasurement();

  measurement.x += offset.x;
  measurement.y += offset.y;
  measurement.z += offset.z;
  
  return measurement;
}

MMC5983MA::DataFrame MMC5983MA::getOffset() {
  return offset;
}

float MMC5983MA::getX() {
  return get().x;
}

float MMC5983MA::getY() {
  return get().y;
}

float MMC5983MA::getZ() {
  return get().z;
}

float MMC5983MA::getTemperature() {
  uint8_t buf = INTERNAL_CONTROL_0_TM_T_MSK;
  writeMemory(REG_INTERNAL_CONTROL_0, &buf, 1, 0);

  // read status to see if measurement is done
  readMemory(REG_STATUS, &buf, 1, 0);
  while (!READ_BITS(buf, STATUS_MEAS_T_DONE_MSK)) {
    readMemory(REG_STATUS, &buf, 1, 0);
  }

  readMemory(REG_T_OUT, &buf, 1, 0);

  float temp = -75. + 0.8 * buf;
  
  return temp;
}
