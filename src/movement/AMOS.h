#ifndef __AMOS_H__
#define __AMOS_H__

#include <avr/io.h>
#include <avr/interrupt.h>
#include "UART.h"

#define AMOS_MAX_SCHEDULE_ENTRIES 16

#define AMOS_NO_ERR 0
#define AMOS_SCHEDULER_EXAUSTION_ERR 1

#define AMOS_BUFF_SIZE 32

#define AMOS_EVENTS_MAX_LISTENERS 8
#define AMOS_EVENTS_NO_SLOTS 255

#define AMOS_ENABLE_TRACE
#define AMOS_ENABLE_INFO
#define AMOS_ENABLE_ERROR

#ifdef AMOS_ENABLE_TRACE
#define AMOS_TRACE(X, ...) UART::printf(X, __VA_ARGS__);
#else
#define AMOS_TRACE(X, ...)
#endif

#ifdef AMOS_ENABLE_INFO
#define AMOS_INFO(X, ...) UART::printf(X, __VA_ARGS__);
#else
#define AMOS_INFO(X, ...)
#endif

#ifdef AMOS_ENABLE_ERROR
#define AMOS_ERROR(X, ...) UART::printf(X, __VA_ARGS__);
#else
#define AMOS_ERROR(X, ...)
#endif

#define AMOS_TEST_TRACE(X, ...) UART::printf(X, __VA_ARGS__);

#define AMOS_EVENT_MASK_BIT 7

namespace AMOS
{
    typedef struct ScheduleEntries
    {
        unsigned long at;
        uint16_t every;
        uint8_t onEvent;
        void (*callback)();
    } ScheduleEntry;

    void init();
    void loop();

    // Timer
    unsigned long int millis();
    void registerMsTick(void (*callback)());
    uint8_t queueInMs(uint16_t in, void (*callback)());
    uint8_t queueOnEvent(uint8_t event, void (*callback)());
    uint8_t queueEveryMs(uint16_t every, void (*callback)());
    void stop(uint8_t handle);

    // Events
    uint8_t subscribe(void (*handler)(uint8_t, uint32_t));
    void unsubsribe(uint8_t slot);
    void dispatchEvent(uint8_t event);
    void dispatchEvent(uint8_t event, uint32_t arg0);

    // Misc
    uint8_t readBouncy(uint8_t pinReg, uint8_t pinBit);
    char *getBuffer();    
}

#endif