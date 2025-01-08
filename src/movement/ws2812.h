/********************************************************************************************
 *
 *  Copyright (C) 2023 Nicola Cimmino
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
 *******************************************************************************************/

#ifndef __WS2812_H__
#define __WS2812_H__

#include <stdbool.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <util/delay.h>

#include "Color.h"

#define PIN_WS2812 5
#define MAX_LED_COUNT 64

namespace WS2812
{    
    void setColor(uint8_t index, Color::RGB rgb);
    Color::RGB getColor(uint8_t index);
    void refresh();
};

#endif