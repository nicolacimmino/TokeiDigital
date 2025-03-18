
#ifndef __BUTTON_H__
#define __BUTTON_H__

#include <stdint.h>
#include <avr/io.h>
#include "AMOS.h"
#include "events.h"

// All buttons are on the same port in the Movment so we can keep things simpler.
#define BUTTONS_PORT PORTB
#define BUTTONS_PIN_REG PINB

class Button
{
private:
    uint8_t pin;
    unsigned long pressStartedAt = 0;
    bool buttonPressed();
    bool stopEvents = false;
public:
    Button(uint8_t pin);
    void loop();        
};

#endif