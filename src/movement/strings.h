
#ifndef __STRINGS_H__
#define __STRINGS_H__

#include <avr/pgmspace.h>

#define S_SI_BANNER 0

const char s_si_banner[] PROGMEM =
    "TOKEI DIGITAL MOVEMENT 5037\r\n"
    "\r\n"
    "TSET HH:MM DOW DOM-MONTH-YEAR\r\n"    
    "SET CURRENT TIME AND DATE.\r\n"
    "\r\n"
    "WREG ADDR VAL.VAL...\r\n"
    "WRITE REGISTER(S).\r\n"
    "\r\n"
    "DREG\r\n"
    "DUMP ALL REGISTERS.\r\n"
    "\r\n"
    "EXIT\r\n"
    "EXIT INTERACTIVE MODE.\r\n"
    "OK\r\n";

PGM_P const si_strings[] PROGMEM =
    {
        s_si_banner,        
};

#endif