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

#ifndef __BERLIN_UHR_H__
#define __BERLIN_UHR_H__

#include "../ws2812.h"
#include "../DS1307.h"
#include "../AMOS.h"
#include "Face.h"

#ifdef FACE_BERLIN_UHR

#define BERLIN_UHR_LED_COUNT 36

#define BERLIN_UHR_LUM_LEVELS 4

#define BERLIN_UHR_COLOR_OFF FACE_REG_BASE + 0
#define BERLIN_UHR_COLOR_RED FACE_REG_BASE + 3
#define BERLIN_UHR_COLOR_YELLOW FACE_REG_BASE + 6

namespace Face
{
    void showTimeImpl(DateTime t);
    void nextDisplayOprionImpl();
};

#endif

#endif