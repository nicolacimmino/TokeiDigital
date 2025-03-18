/********************************************************************************************
 *
 *   Copyright (C) 2023 Nicola Cimmino
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

#include "ws2812.h"

namespace WS2812
{
  namespace
  {
    uint32_t ledData[MAX_LED_COUNT];
  }

  void setColor(uint8_t index, Color::RGB rgb)
  {
    ledData[index] = (((uint32_t)rgb.b) << 16) | (((uint32_t)rgb.g) << 8) | (rgb.r);
  }

  Color::RGB getColor(uint8_t index)
  {
    return
    {
      .r = (ledData[index] >> 16 & 0xFF),
      .g = (ledData[index] >> 8 & 0xFF),
      .b = (ledData[index] & 0xFF)
    };
  }

  void refresh()
  {
    uint8_t curbyte, ctr, masklo;
    uint8_t sreg_prev;
    uint8_t maskhi = _BV(PIN_WS2812);

    DDRD |= maskhi; // Enable output

    masklo = ~maskhi & PORTD;
    maskhi |= PORTD;

    cli();
    sreg_prev = SREG;

    uint8_t ledix = 0;
    for (ledix = 0; ledix < MAX_LED_COUNT; ledix++)
    {
      uint8_t byteix = 0;
      for (byteix = 0; byteix < 3; byteix++)
      {
        curbyte = ledData[ledix] >> (8 * byteix);

        // Based on https://github.com/cpldcpu/light_ws2812
        // Simplified for fixed 16MHz clock speed.

        asm volatile(
            "       ldi   %0,8  \n\t"
            "       clt         \n\t"
            "loop%=:            \n\t"
            "       out   %2,%3 \n\t" //  '1' [01] '0' [01] - re

            "       brid .+0 \n\t"
            "       brid .+0 \n\t"

            "       sbrs  %1,7  \n\t" //  '1' [03] '0' [02]
            "       out   %2,%4 \n\t" //  '1' [--] '0' [03] - fe-low
            "       lsl   %1    \n\t" //  '1' [04] '0' [04]

            "       brid .+0 \n\t"
            "       brid .+0 \n\t"
            "       brid .+0 \n\t"
            "       brid .+0 \n\t"

            "       out   %2,%4 \n\t" //  '1' [+1] '0' [+1] - fe-high

            "       brid .+0 \n\t"

            "       dec   %0    \n\t" //  '1' [+2] '0' [+2]
            "       brne  loop%=\n\t" //  '1' [+3] '0' [+4]
            : "=&d"(ctr)
            : "r"(curbyte), "I"(_SFR_IO_ADDR(PORTD)), "r"(maskhi), "r"(masklo));
      }
    }

    SREG = sreg_prev;
    sei();

    _delay_us(300); // Reset
  }
}