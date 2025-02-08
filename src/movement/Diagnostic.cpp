
#include "Diagnostic.h"

namespace Diagnostic
{
    namespace
    {
        void heartBeat()
        {
            if (AMOS::millis() % 1000 < 500)
            {
                DIAG_LED_PORT |= _BV(DIAG_LED_PIN);
            }
            else
            {
                DIAG_LED_PORT &= ~_BV(DIAG_LED_PIN);
            }
        }
    }

    void init()
    {
        DIAG_LED_DDR != _BV(DIAG_LED_PIN);
        DIAG_LED_PORT |= _BV(DIAG_LED_PIN);
        _delay_ms(500);
        DIAG_LED_PORT &= ~_BV(DIAG_LED_PIN);

        AMOS::queueEveryMs(500, heartBeat);
    }
}
