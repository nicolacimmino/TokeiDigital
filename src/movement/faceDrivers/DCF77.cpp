#include "DCF77.h"

#ifdef FACE_DCF77

namespace Face
{
    namespace
    {
        uint8_t weights[] = {1, 2, 4, 8, 10, 20, 40, 80};

        uint8_t positionColor[] = {
            DCF77_COLOR_MARK,  // 0: Start of minute
            DCF77_COLOR_CIVIL, // 1: Civil Warning
            DCF77_COLOR_CIVIL, // 2: Civil Warning
            DCF77_COLOR_CIVIL, // 3: Civil Warning
            DCF77_COLOR_CIVIL, // 4: Civil Warning
            DCF77_COLOR_CIVIL, // 5: Civil Warning
            DCF77_COLOR_CIVIL, // 6: Civil Warning
            DCF77_COLOR_CIVIL, // 7: Civil Warning
            DCF77_COLOR_CIVIL, // 8: Civil Warning
            DCF77_COLOR_CIVIL, // 9: Civil Warning
            DCF77_COLOR_CIVIL, // 10: Civil Warning
            DCF77_COLOR_CIVIL, // 11: Civil Warning
            DCF77_COLOR_CIVIL, // 12: Civil Warning
            DCF77_COLOR_CIVIL, // 13: Civil Warning
            DCF77_COLOR_CIVIL, // 14: Civil Warning
            DCF77_COLOR_FLAGS, // 15: R abnormal TX operation
            DCF77_COLOR_FLAGS, // 16: Summer time changeover announce
            DCF77_COLOR_FLAGS, // 17: CEST
            DCF77_COLOR_FLAGS, // 18: CET
            DCF77_COLOR_FLAGS, // 19: Leap second announce
            DCF77_COLOR_MARK,  // 20: Start of encoded time
            DCF77_COLOR_MIN,   // 21: Minute 1
            DCF77_COLOR_MIN,   // 22: Minute 2
            DCF77_COLOR_MIN,   // 23: Minute 4
            DCF77_COLOR_MIN,   // 24: Minute 8
            DCF77_COLOR_MIN,   // 25: Minute 10
            DCF77_COLOR_MIN,   // 26: Minute 20
            DCF77_COLOR_MIN,   // 27: Minute 40
            DCF77_COLOR_FLAGS, // 28: Minute Parity (P1)
            DCF77_COLOR_HOUR,  // 29: Hour 1
            DCF77_COLOR_HOUR,  // 30: Hour 2
            DCF77_COLOR_HOUR,  // 31: Hour 4
            DCF77_COLOR_HOUR,  // 32: Hour 8
            DCF77_COLOR_HOUR,  // 33: Hour 10
            DCF77_COLOR_HOUR,  // 34: Hour 20
            DCF77_COLOR_FLAGS, // 35: Hour Parity (P2)
            DCF77_COLOR_DOM,   // 36: Day of Month 1
            DCF77_COLOR_DOM,   // 37: Day of Month 2
            DCF77_COLOR_DOM,   // 38: Day of Month 4
            DCF77_COLOR_DOM,   // 39: Day of Month 8
            DCF77_COLOR_DOM,   // 40: Day of Month 10
            DCF77_COLOR_DOM,   // 41: Day of Month 20
            DCF77_COLOR_WK,    // 42: Day of Week 1
            DCF77_COLOR_WK,    // 43: Day of Week 2
            DCF77_COLOR_WK,    // 44: Day of Week 4
            DCF77_COLOR_MON,   // 45: Month 1
            DCF77_COLOR_MON,   // 46: Month 2
            DCF77_COLOR_MON,   // 47: Month 4
            DCF77_COLOR_MON,   // 48: Month 8
            DCF77_COLOR_MON,   // 49: Month 10
            DCF77_COLOR_YEAR,  // 50: Year 1
            DCF77_COLOR_YEAR,  // 51: Year 2
            DCF77_COLOR_YEAR,  // 52: Year 4
            DCF77_COLOR_YEAR,  // 53: Year 8
            DCF77_COLOR_YEAR,  // 54: Year 10
            DCF77_COLOR_YEAR,  // 55: Year 20
            DCF77_COLOR_YEAR,  // 56: Year 40
            DCF77_COLOR_YEAR,  // 57: Year 80
            DCF77_COLOR_FLAGS, // 58: Date Parity (P3)
            DCF77_COLOR_MARK,  // 59: Minute Mark
        };

        uint8_t showNumber(uint8_t number, uint8_t startIndex, uint8_t bits)
        {
            uint8_t oneBits = 0;
            for (int8_t ix = bits - 1; ix >= 0; ix--)
            {
                if (number >= weights[ix])
                {
                    WS2812::setColor(startIndex + ix, Registers::getAsColor(positionColor[startIndex + ix]));
                    number -= weights[ix];
                    oneBits++;
                }
                else
                {
                    WS2812::setColor(startIndex + ix, Registers::getAsColor(DCF77_COLOR_OFF));
                }
            }
            return oneBits;
        }
    }

    void initImpl()
    {
        Face::canSet = CAN_SET_HOUR |
                       CAN_SET_MINUTE |
                       CAN_SET_DOW |
                       CAN_SET_DOM |
                       CAN_SET_MONTH |
                       CAN_SET_YEAR;
    }

    void showTimeImplDCF(DateTime t)
    {
        if ((t.s & 0x80) == 0)
        {
            // RTC is running, blink seconds (M, minute marker, not DCF protocol like but to show activity).
            WS2812::setColor(0, (t.s % 2) ? Registers::getAsColor(positionColor[0]) : Registers::getAsColor(DCF77_COLOR_OFF));
        }
        else
        {
            // RTC is halted, steady M on.
            WS2812::setColor(0, Registers::getAsColor(positionColor[0]));
        }

        showNumber(0, 1, 8);                            // Civil Warning
        showNumber(0, 9, 6);                            // Civil Warning
        showNumber(0, 15, 1);                           // R
        showNumber(t.dstChangeAnnounce ? 1 : 0, 16, 1); // A1, DST change announce
        showNumber(t.dst ? 1 : 0, 17, 1);               // CEST
        showNumber(t.dst ? 0 : 1, 18, 1);               // CET
        showNumber(0, 19, 1);                           // Leap Second announce
        showNumber(1, 20, 1);                           // Time Start marker

        int8_t parityBits = showNumber(t.m, 21, 7);
        showNumber(parityBits & 0x1, 28, 1);

        parityBits = showNumber(t.h, 29, 6);
        showNumber(parityBits & 0x1, 35, 1);

        parityBits = showNumber(t.dayOfMonth, 36, 6);
        parityBits += showNumber(t.dayOfWeek, 42, 3);
        parityBits += showNumber(t.month, 45, 5);
        parityBits += showNumber(t.year, 50, 8);
        showNumber(parityBits & 0x1, 58, 1);

        showNumber(1, 59, 1); // Minute Marker
    }

    void showTimeImpl(DateTime t)
    {
        if (Face::displayMode == DISPLAY_TEST)
        {
            for (uint8_t ix = 0; ix < DCF77_LED_COUNT; ix++)
            {
                WS2812::setColor(ix, Registers::getAsColor(positionColor[ix]));
            }

            WS2812::refresh();

            return;
        }

        t.h = (Face::displayMode == DISPLAY_SET_HOUR && (AMOS::millis() % 1000 < 500)) ? 0 : t.h;
        t.m = (Face::displayMode == DISPLAY_SET_MINUTE && (AMOS::millis() % 1000 < 500)) ? 0 : t.m;
        t.dayOfMonth = (Face::displayMode == DISPLAY_SET_DOM && (AMOS::millis() % 1000 < 500)) ? 0 : t.dayOfMonth;
        t.month = (Face::displayMode == DISPLAY_SET_MONTH && (AMOS::millis() % 1000 < 500)) ? 0 : t.month;
        t.year = (Face::displayMode == DISPLAY_SET_YEAR && (AMOS::millis() % 1000 < 500)) ? 0 : t.year;
        t.dayOfWeek = (Face::displayMode == DISPLAY_SET_DOW && (AMOS::millis() % 1000 < 500)) ? 0 : t.dayOfWeek;

        showTimeImplDCF(t);

        Color::RGB secondsHandColor = Registers::getAsColor(DCF77_COLOR_SECOND_HAND);
        if (secondsHandColor.r != 0 || secondsHandColor.g != 0 || secondsHandColor.b != 0)
        {
            WS2812::setColor(t.s, secondsHandColor);
        }

        WS2812::refresh();
    }
}

#endif