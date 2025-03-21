
#ifndef __STRINGS_H__
#define __STRINGS_H__

#include <avr/pgmspace.h>
#include "faceDrivers/Face.h"
#include "faceDrivers/DCF77.h"
#include "faceDrivers/BerlinUhr.h"
#include "firmware.h"

#define S_SI_STARTUP 0
#define S_SI_STARTUP_DONE 1
#define S_SI_BANNER 2

#ifdef FACE_DCF77
#define FACE_ID "DCF77"
#endif
#ifdef FACE_BERLIN_UHR
#define FACE_ID "BERUHR"
#endif
#ifndef FACE_ID
#define FACE_ID "NONE"
#endif

const char s_si_startup[] PROGMEM =
    "TOKEI DIGITAL MOVEMENT 5037\r\n"
    "STARTING....\r\n";

const char s_si_startup_done[] PROGMEM =
    "DONE.\r\n";

const char s_si_banner[] PROGMEM =
    "TOKEI DIGITAL MOVEMENT 5037\r\n"
    "VER: " FW_VER "\r\n"
    "FACE DRIVER: " FACE_ID "\r\n"
    "\r\n"
    "H                HELP\r\n"
    "N                PRINT NOW\r\n"
    "R [START] [END]  DUMP REGISTERS\r\n"
    "S [START]        SET REGISTERS\r\n"
    "T H:M WD D-M-Y   SET DATE/TIME\r\n"
    "X                EXIT\r\n";

PGM_P const si_strings[] PROGMEM =
    {
        s_si_startup,
        s_si_startup_done,
        s_si_banner,
};

#endif