#include "amos.h"

namespace AMOS
{
    namespace
    {
        ScheduleEntry schedule[AMOS_MAX_SCHEDULE_ENTRIES];

        volatile unsigned long int milliseconds = 0;

        char buffer[AMOS_BUFF_SIZE];

        void (*dispatchEventHandler[AMOS_EVENTS_MAX_LISTENERS])(uint8_t, uint32_t);
    }

    void init()
    {
        for (uint8_t ix = 0; ix < AMOS_MAX_SCHEDULE_ENTRIES; ix++)
        {
            schedule[ix].callback = 0;
        }

        for (uint8_t ix = 0; ix < AMOS_EVENTS_MAX_LISTENERS; ix++)
        {
            dispatchEventHandler[ix] = NULL;
        }

        TCCR0A = 0;
        TCCR0B = 0;
        TCNT0 = 0;

        OCR0A = 65;              // 1.001mS at 16.0MHz and prescaler 256
        TCCR0A |= (1 << WGM01);  // CTC
        TCCR0B |= (1 << CS02);   // Prescaler = 256
        TIMSK0 |= (1 << OCIE0A); // A compare interrupt
        sei();
    }

    ISR(TIMER0_COMPA_vect)
    {
        milliseconds++;
    }

    void loop()
    {
        uint8_t ix;
        for (ix = 0; ix < AMOS_MAX_SCHEDULE_ENTRIES; ix++)
        {
            if (schedule[ix].callback != 0 && schedule[ix].at != 0 && schedule[ix].at <= millis())
            {
                (*schedule[ix].callback)();

                if (schedule[ix].every == 0)
                {
                    schedule[ix].callback = 0;
                }
                else
                {
                    schedule[ix].at = schedule[ix].every + millis();
                }
            }
        }
    }

    unsigned long int millis()
    {
        return milliseconds;
    }

    uint8_t queueOnEvent(uint8_t event, void (*callback)())
    {
        uint8_t ix;
        for (ix = 0; ix < AMOS_MAX_SCHEDULE_ENTRIES; ix++)
        {
            if (schedule[ix].callback == 0)
            {
                schedule[ix].at = 0;
                schedule[ix].every = 0;
                schedule[ix].onEvent = event | _BV(AMOS_EVENT_MASK_BIT);
                schedule[ix].callback = callback;
                return ix;
            }
        }

        AMOS_ERROR("AMOS,AMOS,ERR_SCH_EXAUST,QUEUE_ON\r\n", NULL)

        return AMOS_EVENTS_NO_SLOTS;
    }

    uint8_t queueInMs(uint16_t in, void (*callback)())
    {
        uint8_t ix;
        for (ix = 0; ix < AMOS_MAX_SCHEDULE_ENTRIES; ix++)
        {
            if (schedule[ix].callback == 0)
            {
                schedule[ix].at = millis() + in;
                schedule[ix].every = 0;
                schedule[ix].onEvent = 0;
                schedule[ix].callback = callback;
                return ix;
            }
        }

        AMOS_ERROR("AMOS,AMOS,ERR_SCH_EXAUST,QUEUE_IN\r\n", NULL)

        return AMOS_EVENTS_NO_SLOTS;
    }

    uint8_t queueEveryMs(uint16_t every, void (*callback)())
    {
        uint8_t ix;
        for (ix = 0; ix < AMOS_MAX_SCHEDULE_ENTRIES; ix++)
        {
            if (schedule[ix].callback == 0)
            {
                schedule[ix].at = millis() + every;
                schedule[ix].every = every;
                schedule[ix].onEvent = 0;
                schedule[ix].callback = callback;
                return ix;
            }
        }

        AMOS_ERROR("AMOS,ERR_SCH_EXAUST,QUEUE_EVERY\r\n", NULL)

        return AMOS_EVENTS_NO_SLOTS;
    }

    void stop(uint8_t handle)
    {
        if (handle < AMOS_MAX_SCHEDULE_ENTRIES)
        {
            schedule[handle].at = 0;
            schedule[handle].every = 0;
            schedule[handle].onEvent = 0;
            schedule[handle].callback = 0;
        }
        else
        {
            AMOS_ERROR("AMOS,STOP,BADHANDLE,%d\r\n", handle);
        }
    }

    uint8_t readBouncy(uint8_t pinReg, uint8_t pinBit)
    {
        uint8_t ix = 0;
        uint8_t reading = 0;
        for (ix = 0; ix < 8; ix++)
        {
            reading = (reading << 1) | (bit_is_set(pinReg, pinBit) ? 1 : 0);
        }

        return reading;
    }

    char *getBuffer()
    {
        return buffer;
    }

    void dispatchEvent(uint8_t event)
    {
        dispatchEvent(event, 0);
    }

    void dispatchEvent(uint8_t event, uint32_t arg0)
    {
        // Use for investigation only, it tends to be
        //  quite heavy and slow down things.
        //AMOS_TRACE("AMOS,EVENT,%d,%d\r\n", event, arg0)

        for (uint8_t ix = 0; ix < AMOS_MAX_SCHEDULE_ENTRIES; ix++)
        {
            schedule[ix].onEvent = schedule[ix].onEvent & ~_BV(AMOS_EVENT_MASK_BIT);
        }

        for (uint8_t ix = 0; ix < AMOS_MAX_SCHEDULE_ENTRIES; ix++)
        {
            if (schedule[ix].callback != 0 && schedule[ix].onEvent == event)
            {
                (*schedule[ix].callback)();
                schedule[ix].callback = 0;
                schedule[ix].onEvent = 0;
            }
        }

        for (uint8_t ix = 0; ix < AMOS_EVENTS_MAX_LISTENERS; ix++)
        {
            if (dispatchEventHandler[ix] != NULL)
            {
                dispatchEventHandler[ix](event, arg0);
            }
        }
    }

    uint8_t subscribe(void (*handler)(uint8_t, uint32_t))
    {
        for (uint8_t ix = 0; ix < AMOS_EVENTS_MAX_LISTENERS; ix++)
        {
            if (dispatchEventHandler[ix] == NULL)
            {
                dispatchEventHandler[ix] = handler;
                return ix;
            }
        }

        AMOS_ERROR("AMOS,EVENTS,NO_SLOTS\r\n", NULL)

        return AMOS_EVENTS_NO_SLOTS;
    }

    void unsubsribe(uint8_t slot)
    {
        if (slot < AMOS_EVENTS_MAX_LISTENERS)
        {
            dispatchEventHandler[slot] = NULL;
        }
    }
}