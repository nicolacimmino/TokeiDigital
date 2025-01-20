#ifndef __SerialInterface_H__
#define __SerialInterface_H__

#include <stdint.h>
#include <avr/wdt.h>
#include "Registers.h"
#include "UART.h"
#include "DS1307.h"
#include "faceDrivers/Face.h"
#include "firmware.h"
#include "AMOS.h"

#define SI_INTERVAL_MS 1000

#define SI_DUMP_PER_LINE 8

#define SI_XON 0x11
#define SI_XOFF 0x13

namespace SerialInterface
{
    void init();
    void printStartup();
    void printStartupDone();
    void processSerialInterface();
    void dumpRegisters(int start, int end);
    void outputClockData();
    uint8_t serveClient();
}

#endif