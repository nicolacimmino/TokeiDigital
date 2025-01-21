
#ifndef __DS_1307_H__
#define __DS_1307_H__

#include <stdint.h>
#include <avr/io.h>
#include <util/twi.h>
#include "uTWI.h"
#include "Registers.h"

// DS1307 Registers
#define DS1307_ADDR 0x68
#define DS1307_TIME_REG_BASE 0x00
#define DS1307_RAM_BASE 0x08

struct DateTime
{
    uint8_t h;
    uint8_t m;
    uint8_t s;
    uint8_t dayOfWeek;
    uint8_t dayOfMonth;
    uint8_t month;
    uint8_t year;
    bool dst;
    bool dstChangeAnnounce;
};

namespace DS1307
{
    DateTime getDateTime();
    void setDateTime(DateTime t);
    void haltClock();
    void resumeClock();
}

#endif