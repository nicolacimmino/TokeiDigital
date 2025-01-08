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

#define DCF77_COLOR_OFF 0
#define DCF77_COLOR_MIN 1
#define DCF77_COLOR_HOUR 2
#define DCF77_COLOR_DOM 3
#define DCF77_COLOR_WK 4
#define DCF77_COLOR_MON 5
#define DCF77_COLOR_YEAR 6
#define DCF77_COLOR_FLAGS 7
#define DCF77_COLOR_MARK 8
#define DCF77_COLOR_CIVIL 9
#define DCF77_COLOR_SECOND_HAND 10

namespace Face
{
    void initImpl();
    void showTimeImpl(DateTime t);
};

#endif

#endif