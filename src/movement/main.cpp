/********************************************************************************************
 *
 *  Copyright (C) 2024 Nicola Cimmino
 *
 *    This program is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program.  If not, see http://www.gnu.org/licenses/.
 *
 *
 *******************************************************************************************/

#include <stdbool.h>
#include <avr/io.h>
#include <avr/wdt.h>
#include "AMOS.h"
#include "faceDrivers/Face.h"
#include "DS1307.h"
#include "hardware.h"
#include "Button.h"
#include "Registers.h"
#include "SerialInterface.h"
#include "TimeSetter.h"

void refreshFace()
{
    Face::showTime(DS1307::getDateTime());
}

int main(void)
{
    wdt_enable(WDTO_8S);

    AMOS::init();
    SerialInterface::init();

    SerialInterface::printStartup();

    UTWI::init();
    Face::init();
    TimeSetter::init();

    AMOS::queueEveryMs(200, refreshFace);

    Button buttonA = Button(BTNA_PIN);
    Button buttonB = Button(BTNB_PIN);
    Button buttonC = Button(BTNC_PIN);

    // Just hit the DS1307 once before we confirm the
    //  boot was successful, if there's a communiction
    //  issue the WD will reset us and we get a change
    //  to re-initialize everything.
    DS1307::getDateTime();

    SerialInterface::printStartupDone();

    while (1)
    {
        AMOS::loop();

        buttonA.loop();
        buttonB.loop();
        buttonC.loop();

        wdt_reset();
    }
}
