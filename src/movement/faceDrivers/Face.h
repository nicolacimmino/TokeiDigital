/********************************************************************************************
 *
 *  Copyright (C) 2024 Nicola Cimmino
 * 
 *    This program is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program.  If not, see http://www.gnu.org/licenses/.
 *
 *
 *******************************************************************************************/

#ifndef __FACE_H__
#define __FACE_H__

#include <stdint.h>
#include "../hardware.h"
#include "../events.h"
#include "DCF77.h"
#include "BerlinUhr.h"

#define FACE_REG_BASE 0x40

#define DISPLAY_NORMAL 1
#define DISPLAY_TEST 2
#define DISPLAY_SET_HOUR 0x80
#define DISPLAY_SET_MINUTE 0x81
#define DISPLAY_SET_DOW 0x82
#define DISPLAY_SET_DOM 0x83
#define DISPLAY_SET_MONTH 0x84
#define DISPLAY_SET_YEAR 0x85

#define CAN_SET_HOUR 1
#define CAN_SET_MINUTE 2
#define CAN_SET_DOW 4
#define CAN_SET_DOM 8
#define CAN_SET_MONTH 16
#define CAN_SET_YEAR 32

#define DISPLAY_MODE_IS_SETTING(X) ((X & 0x80) != 0)

namespace Face
{
    extern uint8_t displayMode;
    extern uint8_t canSet;
    void init();
    void eventHandler(uint8_t event, uint32_t p0);
    void showTime(DateTime t);
    void nextDisplayOption();
    void initImpl() __attribute__((weak));
    void showTimeImpl(DateTime t) __attribute__((weak));
    void nextDisplayOprionImpl() __attribute__((weak));
};

#endif