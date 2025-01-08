
#include "Face.h"

namespace Face
{
    uint8_t displayMode = DISPLAY_NORMAL;
    uint8_t canSet = CAN_SET_HOUR | CAN_SET_MINUTE;

    void init()
    {
        AMOS::subscribe(Face::eventHandler);

        if (initImpl)
        {
            Face::initImpl();
        }
        // else do nothing, specialized face doesn't have init.
    }

    void eventHandler(uint8_t event, uint32_t p0)
    {
        if (DISPLAY_MODE_IS_SETTING(Face::displayMode))
        {
            return;
        }

        if (event == BUTTON_PRESS && p0 == BTNA_PIN)
        {
            Face::nextDisplayOption();
        }

        if (event == BUTTON_LONG_PRESS && p0 == BTNC_PIN)
        {
            if (Face::displayMode == DISPLAY_NORMAL)
            {
                Face::displayMode = DISPLAY_TEST;
            }
            else
            {
                Face::displayMode = DISPLAY_NORMAL;
            }
        }
    }

    void showTime(DateTime t)
    {
        if (showTimeImpl)
        {
            Face::showTimeImpl(t);
        }
        else
        {
            // Always output something if the face is not defined.
            // This avoids users having to ponder if they hardware is defective or their code broken.
            WS2812::setColor(0, {0x11, 0x33, 0xFF});
            WS2812::refresh();
        }
    }

    void nextDisplayOption()
    {
        if (nextDisplayOprionImpl)
        {
            Face::nextDisplayOprionImpl();
        }
        // else do nothing, default face has only one level.
    }
};
