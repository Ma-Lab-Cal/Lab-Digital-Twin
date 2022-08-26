/**
 * @file rath_SHT35.cpp
 * @date 2022-02-08
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "rath_SHT35.h"

using namespace rath;

void SHT35::init() {
  I2CDevice::init();
}

// side note:
// because we use two methods to pass I2C data ----
// single byte value if only transmitting one byte, or an array if we are transmitting multiple
// I found it necessary to distinguish these two to prevent the possible confusion
// which there are already plenty even when writing this library
// thus,
// we will use `buf` or `b` to denote a reference to value, and `buffer` to denote
// a reference to array / pointer
// idea is that the first one seems shorter, so represent a single byte, while the 
// latter one seems longer, and thus a complex array.


Status SHT35::_takeSingleMeasurement(uint8_t *buffer) {
  // take a measurement in single shot mode with clock-streching (blocking) and with highest precision
  *(uint16_t *)buffer = CLOCK_STRETCHING_ON_REPEATABILITY_HIGH;

  // send measurement command
  transmit(buffer, 2, 0);

  // receive measurement in MSB first order
  return receive(buffer, 6, 0);
}

void SHT35::disableHeater() {
  uint16_t buf = HEATER_DISABLE;
  transmit((uint8_t *)&buf, 2, 0);
}

void SHT35::enableHeater() {
  uint16_t buf = HEATER_ENABLE;
  transmit((uint8_t *)&buf, 2, 0);
}

float SHT35::getTemperature() {
  uint8_t buffer[6];
  
  if (_takeSingleMeasurement(buffer) != OK) {
    // if I2C transmission failed
    return -274.;
  }

  uint16_t st_readout = (buffer[0] << 8) | buffer[1];  // [0:1] bytes are temperature readout, [2] is checksum

  // convertion equation from datasheet pg. 14: https://dfimg.dfrobot.com/nobody/wiki/88b31350da4f54d00989c74c6fa392f7.pdf
  float temperature = -45. + 175. * ((float)st_readout / 0xFFFF);

  return temperature;
}

float SHT35::getTemperatureK() {
  float temperature_c = getTemperature();

  if (temperature_c == -274) {
    return -1;
  }
  
  return temperature_c + 273.15;
}

float SHT35::getHumidity() {
  uint8_t buffer[6];

  if (_takeSingleMeasurement(buffer) != OK) {
    // if I2C transmission failed
    return -1.;
  }

  uint16_t srh_readout = (buffer[0] << 8) | buffer[1];  // [3:4] bytes are humidity readout, [5] is checksum

  // convertion equation from datasheet pg. 14: https://dfimg.dfrobot.com/nobody/wiki/88b31350da4f54d00989c74c6fa392f7.pdf
  float humidity = (float)srh_readout / 0xFFFF;

  return humidity;
}
