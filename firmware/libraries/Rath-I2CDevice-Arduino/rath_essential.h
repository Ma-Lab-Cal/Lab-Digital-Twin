/**
 * @file rath_essential.h
 * @date 2021-10-25
 * 
 * @copyright Copyright (c) 2021
 * 
 * This is a library for basic utilities.
 * 
 */

#pragma once

extern "C" {
  #define SET_BITS(REG, BIT)                    ((REG) |= (BIT))
  #define CLEAR_BITS(REG, BIT)                  ((REG) &= ~(BIT))
  #define READ_BITS(REG, BIT)                   ((REG) & (BIT))
  #define WRITE_BITS(REG, CLEARMASK, SETMASK)   ((REG) = (((REG) & (~(CLEARMASK))) | (SETMASK)))
}

namespace rath {
  enum Status {
    OK = 0U,
    ERROR,
    BUSY,
    TIMEOUT
  };
};
