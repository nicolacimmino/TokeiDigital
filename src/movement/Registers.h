

#ifndef __REGISTERS_H__
#define __REGISTERS_H__

#include <stdint.h>
#include <avr/eeprom.h>
#include "Color.h"

#define REGISTERS_SIZE 128

// Configuation registers
#define DST_REGISTER 0x00
#define DST_AUTO 0
#define DST_OFF 1
#define DST_ON 2
// 1,2 are reserved for future plans to have configurable DST rules (EU, US etc)
#define SERIAL_ENABLE_REGISTER 0x03
#define SERIAL_OFF 0
#define SERIAL_V1 1

namespace Registers
{
    Color::RGB getAsColor(uint8_t address);
    uint8_t get(uint8_t address);
    void set(uint8_t address, uint8_t value);
}

#endif