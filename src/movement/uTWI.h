

#ifndef __UTWI__
#define __UTWI__

#define TWI_ACK true
#define TWI_NACK false

#include <avr/io.h>
#include <util/twi.h>
#include <stdint.h>

namespace UTWI {
    void init();
    void readRegisters(uint8_t addr, uint8_t reg, uint8_t* buf, uint8_t size);
    void writeRegisters(uint8_t addr, uint8_t reg, uint8_t* buf, uint8_t size);
}

#endif