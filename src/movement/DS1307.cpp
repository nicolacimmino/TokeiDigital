/********************************************************************************************
 *
 *  Copyright (C) 2024 Nicola Cimmino
 *
 * This is a driver for the DS1307 Real Time Clock. It offers only time set/get.
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
#include "DS1307.h"

namespace DS1307
{
    namespace
    {
        uint8_t _twiBuf[7] = {0, 0, 0, 0, 0, 0, 0};
    }

    /**
     * Get the current time.
     */
    DateTime getDateTime()
    {
        DateTime dateTime;
    
        UTWI::readRegisters(DS1307_ADDR, DS1307_TIME_REG_BASE, _twiBuf, 7);
     
        dateTime.year = (10 * (_twiBuf[6] >> 4)) + (_twiBuf[6] & 0b1111);
        dateTime.month = (10 * (_twiBuf[5] >> 4)) + (_twiBuf[5] & 0b1111);
        dateTime.dayOfMonth = (10 * (_twiBuf[4] >> 4)) + (_twiBuf[4] & 0b1111);
        dateTime.dayOfWeek = _twiBuf[3];
        dateTime.h = (10 * ((_twiBuf[2] >> 4) & 0b11)) + (_twiBuf[2] & 0b1111);
        dateTime.m = (10 * (_twiBuf[1] >> 4)) + (_twiBuf[1] & 0b1111);
        dateTime.s = (10 * (_twiBuf[0] >> 4)) + (_twiBuf[0] & 0b1111);

        UTWI::readRegisters(DS1307_ADDR, DS1307_RAM_BASE, _twiBuf, 1);
        bool setTimeIsDst = (_twiBuf[0] == 1);

        if (Registers::get(DST_REGISTER) == DST_AUTO)
        {
            uint8_t daysToNextSunday = ((7 - dateTime.dayOfWeek + 6) % 7) + 1;

            dateTime.dst = false;
            dateTime.dstChangeAnnounce = false;
            if (dateTime.month > 4 && dateTime.month < 9)
            {
                dateTime.dst = true;
            }
            else if (dateTime.month == 3)
            {
                // If the next sunday is next month then this is DST already.
                // Unless it's Sunday then it depends on the current time.
                if ((dateTime.dayOfMonth + daysToNextSunday) > 31)
                {
                    if (dateTime.dayOfWeek != 7 || dateTime.h >= 2)
                    {
                        dateTime.dst = true;
                    }

                    if (dateTime.dayOfWeek == 7 && dateTime.h == 2)
                    {
                        dateTime.dstChangeAnnounce = true;
                    }
                }
            }
            else if (dateTime.month == 10)
            {
                // If the next sunday is still in this month then this is still DST.
                if ((dateTime.dayOfMonth + daysToNextSunday) <= 31)
                {
                    dateTime.dst = true;
                }

                // If the next sunday is next month and its sunday then we need to check the time,
                //  this is the DST change day. Also, because we live the same 2AM twice we need to
                //  check the setTimeIsDst stored in the DS RAM. The first time we live 2AM it's still
                //  DST while the second not.
                if ((dateTime.dayOfMonth + daysToNextSunday) > 31 && dateTime.dayOfWeek == 7)
                {
                    if (dateTime.h < 3 && setTimeIsDst)
                    {
                        if (dateTime.h == 2)
                        {
                            dateTime.dstChangeAnnounce = true;
                        }
                        dateTime.dst = true;
                    }
                }
            }
        }
        else
        {
            dateTime.dstChangeAnnounce = false;
            dateTime.dst = (Registers::get(DST_REGISTER) == DST_ON);
        }

        // If there has been a change in DST on/off since the last time we set the time we need to set the time.
        if (dateTime.dst != setTimeIsDst)
        {
            // Store the fact that the time in memory is the current DST on/off.
            _twiBuf[0] = (dateTime.dst) ? 1 : 0;
            UTWI::writeRegisters(DS1307_ADDR, DS1307_RAM_BASE, _twiBuf, 1);

            // Move clock forward/backward 1h (no date wrap needed as this never happens at midnight/one)
            dateTime.h += (dateTime.dst) ? 1 : -1;
            setDateTime(dateTime);
        }

        return dateTime;
    }

    /**
     * Set the time.
     */
    void setDateTime(DateTime dt)
    {
        _twiBuf[0] = ((dt.s / 10) << 4) | (dt.s % 10);
        _twiBuf[1] = ((dt.m / 10) << 4) | (dt.m % 10);
        _twiBuf[2] = ((dt.h / 10) << 4) | (dt.h % 10);
        _twiBuf[3] = dt.dayOfWeek;
        _twiBuf[4] = ((dt.dayOfMonth / 10) << 4) | (dt.dayOfMonth % 10);
        _twiBuf[5] = ((dt.month / 10) << 4) | (dt.month % 10);
        _twiBuf[6] = ((dt.year / 10) << 4) | (dt.year % 10);

        UTWI::writeRegisters(DS1307_ADDR, DS1307_TIME_REG_BASE, _twiBuf, 7);
    }

    void haltClock()
    {
        DateTime dt = getDateTime();
        dt.s = 0x80;
        setDateTime(dt);
    }

    void resumeClock()
    {
        DateTime dt = getDateTime();
        dt.s = 0x00;
        setDateTime(dt);
    }
}
