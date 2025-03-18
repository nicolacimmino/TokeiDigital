#include "TimeSetter.h"

namespace TimeSetter
{
    void init()
    {
        AMOS::subscribe(TimeSetter::eventHandler);
    }

    uint8_t getNextMode()
    {
        // Fall trough to the next case is intentional so that
        //  if a face cannot set certain value we move to the
        //  next till we find a suitable one.
        switch (Face::displayMode)
        {
        case DISPLAY_SET_HOUR:
            if (Face::canSet & CAN_SET_MINUTE)
                return DISPLAY_SET_MINUTE;
        case DISPLAY_SET_MINUTE:
            if (Face::canSet & CAN_SET_DOM)
                return DISPLAY_SET_DOM;
        case DISPLAY_SET_DOM:
            if (Face::canSet & CAN_SET_MONTH)
                return DISPLAY_SET_MONTH;
        case DISPLAY_SET_MONTH:
            if (Face::canSet & CAN_SET_YEAR)
                return DISPLAY_SET_YEAR;
        case DISPLAY_SET_YEAR:
            if (Face::canSet & CAN_SET_DOW)
                return DISPLAY_SET_DOW;
        default:
            return DISPLAY_SET_HOUR;
        }
    }

    uint8_t offsetOneBaseRange(uint8_t value, int8_t offset, uint8_t maxVal)
    {
        return (((value + maxVal - 1) + offset) % maxVal) + 1;
    }

    void processBCPress(uint8_t button)
    {
        if (button == BTNA_PIN)
        {
            Face::displayMode = getNextMode();
            return;
        }

        DateTime t = DS1307::getDateTime();

        int8_t offset = 0;
        if (button == BTNB_PIN)
        {
            offset = 1;
        }

        if (button == BTNC_PIN)
        {
            offset = -1;
        }

        switch (Face::displayMode)
        {
        case DISPLAY_SET_HOUR:
            t.h = (t.h + offset) % 24;
            break;
        case DISPLAY_SET_MINUTE:
            t.m = (t.m + offset) % 60;
            break;
        case DISPLAY_SET_DOM:
            t.dayOfMonth = offsetOneBaseRange(t.dayOfMonth, offset, 31);
            break;
        case DISPLAY_SET_MONTH:
            t.month = offsetOneBaseRange(t.month, offset, 12);
            break;
        case DISPLAY_SET_YEAR:
            t.year = offsetOneBaseRange(t.year, offset, 99);
            break;
        case DISPLAY_SET_DOW:
            t.dayOfWeek = offsetOneBaseRange(t.dayOfWeek, offset, 7);
            break;
        }

        DS1307::setDateTime(t);
    }

    void eventHandler(uint8_t event, uint32_t p0)
    {
        // Enter/exit setting mode.
        if (event == BUTTON_LONG_PRESS && p0 == BTNA_PIN)
        {
            if (!DISPLAY_MODE_IS_SETTING(Face::displayMode))
            {
                DS1307::haltClock();
                Face::displayMode = DISPLAY_SET_HOUR;
            }
            else
            {
                DS1307::resumeClock();
                Face::displayMode = DISPLAY_NORMAL;
            }
        }

        if (!DISPLAY_MODE_IS_SETTING(Face::displayMode))
        {
            return;
        }

        if (event == BUTTON_PRESS)
        {
            processBCPress(p0);
        }
    }
}