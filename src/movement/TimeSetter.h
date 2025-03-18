#ifndef __TIMESETTER_H__
#define __TIMESETTER_H__

#include <stdint.h>
#include "faceDrivers/Face.h"
#include "hardware.h"
#include "events.h"


namespace TimeSetter
{
    void init();
    void eventHandler(uint8_t event, uint32_t p0);
}

#endif