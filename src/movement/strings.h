
#ifndef __STRINGS_H__
#define __STRINGS_H__

#include <avr/pgmspace.h>

#define S_SI_STARTUP 0
#define S_SI_STARTUP_DONE 1
#define S_SI_BANNER 2

const char s_si_startup[] PROGMEM =
    "TOKEI DIGITAL MOVEMENT 5037\r\n"
    "STARTING....\r\n";

const char s_si_startup_done[] PROGMEM =
    "DONE.\r\n";

const char s_si_banner[] PROGMEM =
    "TOKEI DIGITAL MOVEMENT 5037\r\n"
    "\r\n"
    "H                HELP\r\n"
    "N                PRINT NOW\r\n"
    "R                DUMP REGISTERS\r\n"
    "S [START]        SET REGISTERS\r\n"
    "T [H:M WD D-M-Y] PRINT DATE/TIME\r\n"    
    "X                EXIT\r\n";
    
PGM_P const si_strings[] PROGMEM =
    {
        s_si_startup,
        s_si_startup_done,
        s_si_banner,
};

#endif