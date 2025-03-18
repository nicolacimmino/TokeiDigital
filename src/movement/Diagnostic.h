
#ifndef __DIAGNOSTIC_H__
#define __DIAGNOSTIC_H__

#include "AMOS.h"

#define DIAG_LED_DDR DDRD
#define DIAG_LED_PIN PD7
#define DIAG_LED_PORT PORTD

namespace Diagnostic
{
    void init();
}

#endif