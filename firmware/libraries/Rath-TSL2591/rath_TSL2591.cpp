/**
 * @file rath_TSL2591.cpp
 * @date 2022-02-08
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "rath_TSL2591.h"

using namespace rath;

void TSL2591::init() {
  I2CDevice::init();
}

void TSL2591::reset() {
  uint8_t buf = CONTROL_SRESET_MSK;
  writeMemory(COMMAND_CMD_NORMAL_MSK | REG_CONTROL, &buf, 1, 0);
  _updateSettings();
}

uint8_t TSL2591::getID() {
  uint8_t buf;
  readMemory(COMMAND_CMD_NORMAL_MSK | REG_ID, &buf, 1, 0);
  return buf;
}

void TSL2591::disable() {
  uint8_t buf = 0;
  writeMemory(COMMAND_CMD_NORMAL_MSK | REG_ENABLE, &buf, 1, 0);
}

void TSL2591::enable() {
  uint8_t buf = ENABLE_PON_MSK | ENABLE_AEN_MSK;
  writeMemory(COMMAND_CMD_NORMAL_MSK | REG_ENABLE, &buf, 1, 0);

  // // Enable the device by setting the control bit to 0x01
  // write8(TSL2591_COMMAND_BIT | TSL2591_REGISTER_ENABLE,
  //        TSL2591_ENABLE_POWERON | TSL2591_ENABLE_AEN | TSL2591_ENABLE_AIEN |
  //            TSL2591_ENABLE_NPIEN);
}

void TSL2591::setGain(TSL2591::Gain gain) {
  _gain = gain;
  _updateSettings();
}

void TSL2591::setIntegrationTime(TSL2591::IntegrationTime time) {
  _integration_time = time;
  _updateSettings();
}

float TSL2591::get() {
  enable();

  
  delay((100 * (unsigned int)_integration_time) + 150);
  
  uint8_t buffer[4];
  if (readMemory(COMMAND_CMD_NORMAL_MSK | REG_C0DATAL, buffer, 4, 0) != OK) {
    return -1;
  }
  uint16_t ch0_data = ((uint16_t)buffer[1] << 8) | (uint16_t)buffer[0];
  uint16_t ch1_data = ((uint16_t)buffer[3] << 8) | (uint16_t)buffer[2];
  disable();

  if ((ch0_data == 0xFFFF) || (ch1_data == 0xFFFF)) {
    // Signal an overflow
    return -1;
  }

  // Note: This algorithm is based on preliminary coefficients
  // provided by AMS and may need to be updated in the future

  float again;
  float atime;

  switch (_gain) {
    case Gain::GAIN_LOW:
      again = 1.0F;
      break;
    case Gain::GAIN_MEDIUM:
      again = 25.0F;
      break;
    case Gain::GAIN_HIGH:
      again = 428.0F;
      break;
    case Gain::GAIN_MAXIMUM:
      again = 9876.0F;
      break;
    default:
      again = 1.0F;
  }

  switch (_integration_time) {
    case IntegrationTime::T_100_MS:
      atime = 100.0F;
      break;
    case IntegrationTime::T_200_MS:
      atime = 200.0F;
      break;
    case IntegrationTime::T_300_MS:
      atime = 300.0F;
      break;
    case IntegrationTime::T_400_MS:
      atime = 400.0F;
      break;
    case IntegrationTime::T_500_MS:
      atime = 500.0F;
      break;
    case IntegrationTime::T_600_MS:
      atime = 600.0F;
      break;
    default: // 100ms
      atime = 100.0F;
  }

  // cpl = (ATIME * AGAIN) / DF
  float cpl = (atime * again) / LUX_COEFFICIENT;

  // Original lux calculation (for reference sake)
  // lux1 = ( (float)ch0 - (TSL2591_LUX_COEFB * (float)ch1) ) / cpl;
  // lux2 = ( ( TSL2591_LUX_COEFC * (float)ch0 ) - ( TSL2591_LUX_COEFD *
  // (float)ch1 ) ) / cpl; lux = lux1 > lux2 ? lux1 : lux2;

  // Alternate lux calculation 1
  // See: https://github.com/adafruit/Adafruit_TSL2591_Library/issues/14
  float lux = (((float)ch0_data - (float)ch1_data)) * (1.0F - ((float)ch1_data / (float)ch0_data)) / cpl;

  return lux;
}

void TSL2591::_updateSettings() {
  uint8_t buf = ((uint8_t)_gain << 4) | ((uint8_t)_integration_time);

  writeMemory(COMMAND_CMD_NORMAL_MSK | REG_CONTROL, &buf, 1, 0);
}
