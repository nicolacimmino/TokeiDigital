

#ifndef __REGISTERS_H__
#define __REGISTERS_H__

#include <stdint.h>
#include <avr/eeprom.h>
#include "Color.h"

#define REGISTERS_PERSISTENT_SIZE 128

namespace Registers
{
    Color::RGB getPersistentAsColor(uint8_t address);
    uint8_t getPersistent(uint8_t address);
    void setPersistent(uint8_t address, uint8_t value);
}

#endif