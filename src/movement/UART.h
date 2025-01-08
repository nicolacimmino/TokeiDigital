/********************************************************************************************
 *
 *  Copyright (C) 2024 Nicola Cimmino
 *
 *  A non-generic UART driver tailored specifically for PHK-Phone.
 * 
 *  It provides a printf interface for sending and a full text line receiver that
 *      also capitalizes all received text to minimize parsing code.
 *  Additionally, in data mode, it acts as a classic RX buffer data can be processed,
 *      unaltered, one char at the time.
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

#ifndef __UART_H__
#define __UART_H__

#include <avr/io.h>
#include "amos.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>

#define UART_RX_BUF_SIZE 32
#define KEY_BACKSPACE 0x7F

#if F_CPU == 8000000
#define BAUDL 102
#endif

#if F_CPU == 16000000
#define BAUDL 204
#endif

#include "strings.h"

namespace UART
{
    void init();
    void printSiString(uint8_t index);
    void print(const char c);
    void print(const char *text);
    void printf(const char *__fmt, ...);
    bool isLineAvailable();
    char *getLine();
    void clearLine();
    bool isCharAvailable();
    char peekChar();
    char getChar();
}

#endif