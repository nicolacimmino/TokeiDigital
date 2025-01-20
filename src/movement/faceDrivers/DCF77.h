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

#ifndef __DCF77_H__
#define __DCF77_H__

#include "../ws2812.h"
#include "../DS1307.h"
#include "../AMOS.h"
#include "../Registers.h"
#include "Face.h"

#ifdef FACE_DCF77

#define FACE_ID "DCF77"

#define DCF77_LED_COUNT 60

#define DCF77_COLOR_OFF FACE_REG_BASE + 0
#define DCF77_COLOR_MIN FACE_REG_BASE + 3
#define DCF77_COLOR_HOUR FACE_REG_BASE + 6
#define DCF77_COLOR_DOM FACE_REG_BASE + 9
#define DCF77_COLOR_WK FACE_REG_BASE + 12
#define DCF77_COLOR_MON FACE_REG_BASE + 15
#define DCF77_COLOR_YEAR FACE_REG_BASE + 18
#define DCF77_COLOR_FLAGS FACE_REG_BASE + 21
#define DCF77_COLOR_MARK FACE_REG_BASE + 24
#define DCF77_COLOR_CIVIL FACE_REG_BASE + 27
#define DCF77_COLOR_SECOND_HAND FACE_REG_BASE + 30

namespace Face
{
    void initImpl();
    void showTimeImpl(DateTime t);
};

#endif

#endif