#include "BerlinUhr.h"

#ifdef FACE_BERLIN_UHR

namespace Face
{
    namespace
    {       
        void showDigit(uint8_t positionsOn,
                       uint8_t ledOffset,
                       uint8_t ledsPerPosition,
                       uint8_t positions,
                       Color::RGB color,
                       uint8_t color1Every,
                       Color::RGB color1)
        {
            for (uint8_t ix = 0; ix < positions * ledsPerPosition; ix++)
            {
                uint8_t position = floor(ix / ledsPerPosition);
                Color::RGB colorIfOn = (position % color1Every == color1Every - 1) ? color1 : color;
                WS2812::setColor(
                    ledOffset + ix,
                    (positionsOn > position) ? colorIfOn : Registers::getAsColor(BERLIN_UHR_COLOR_OFF));
            }
        }

         void showDigit(uint8_t positionsOn,
                       uint8_t ledOffset,
                       uint8_t ledsPerPosition,
                       uint8_t positions,
                       Color::RGB color)
        {
            showDigit(positionsOn,
                      ledOffset,
                      ledsPerPosition,
                      positions,
                      color,
                      0,
                      Registers::getAsColor(BERLIN_UHR_COLOR_OFF));
        }
    }

    void showTimeImpl(DateTime t)
    {
        if (Face::displayMode == DISPLAY_TEST)
        {
            t.h = 24;
            t.m = 59;
            t.s = 1;
        }

        if (Face::displayMode == DISPLAY_SET_HOUR && (AMOS::millis() % 1000 < 500))
        {
            t.h = 0;
        }

        if (Face::displayMode == DISPLAY_SET_MINUTE && (AMOS::millis() % 1000 < 500))
        {
            t.m = 0;
        }

        if ((t.s & 0x80) == 0)
        {
            WS2812::setColor(0, (t.s % 2) ? Registers::getAsColor(BERLIN_UHR_COLOR_YELLOW) : Registers::getAsColor(BERLIN_UHR_COLOR_OFF));
        }
        else
        {
            WS2812::setColor(0, Registers::getAsColor(BERLIN_UHR_COLOR_RED));
        }

        showDigit(floor(t.h / 5), 1, 2, 4, Registers::getAsColor(BERLIN_UHR_COLOR_RED));
        showDigit(floor(t.h % 5), 9, 2, 4, Registers::getAsColor(BERLIN_UHR_COLOR_RED));

        showDigit(floor(t.m / 5), 17, 1, 11, Registers::getAsColor(BERLIN_UHR_COLOR_YELLOW), 3, Registers::getAsColor(BERLIN_UHR_COLOR_RED));
        showDigit(floor(t.m % 5), 28, 2, 4, Registers::getAsColor(BERLIN_UHR_COLOR_YELLOW));

        WS2812::refresh();
    }
}

#endif