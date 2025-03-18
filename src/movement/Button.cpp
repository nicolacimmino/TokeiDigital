
#include "Button.h"

Button::Button(uint8_t pin)
{
    BUTTONS_PORT |= _BV(pin);
    this->pin = pin;
}

void Button::loop()
{
    bool pressed = buttonPressed();

    if (pressed && this->pressStartedAt == 0)
    {
        this->pressStartedAt = AMOS::millis();
        return;
    }

    if (!stopEvents && pressed && (AMOS::millis() - this->pressStartedAt) > 1500)
    {
        AMOS::dispatchEvent(BUTTON_LONG_PRESS, this->pin);
        stopEvents = true;

        return;
    }

    if (!pressed && this->pressStartedAt != 0)
    {
        // Debounce
        if (AMOS::millis() - this->pressStartedAt < 500)
        {
            return;
        }

        if (AMOS::millis() - this->pressStartedAt < 1000)
        {
            AMOS::dispatchEvent(BUTTON_PRESS, this->pin);
        }

        this->pressStartedAt = 0;
        stopEvents = false;
    }
}

bool Button::buttonPressed()
{
    return AMOS::readBouncy(BUTTONS_PIN_REG, this->pin) == 0;
}
