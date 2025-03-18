/********************************************************************************************
 *
 *  Copyright (C) 2024 Nicola Cimmino
 *
 *  A non-generic UART driver tailored specifically for PHK-Phone.
 *  It provides a printf interface for sending and a full text line receiver that
 *      also capitalizes all received text to minimize parsing code.
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

#include "UART.h"

namespace UART
{
    namespace
    {
        char rxBuffer[UART_RX_BUF_SIZE];

        char *rxBufferWp;
        char *rxBufferRp;
    }

    void init()
    {
        // 9600 BAUD,n,8,1
        // Enable RX interrupt.
        UCSR0A = _BV(U2X0);
        UCSR0B |= _BV(RXEN0) | _BV(TXEN0) | _BV(RXCIE0);
        UCSR0C = _BV(UCSZ00) | _BV(UCSZ01);
        UBRR0H = 0;
        UBRR0L = BAUDL;

        rxBufferWp = rxBuffer;
        rxBufferRp = rxBuffer;
        memset(rxBuffer, 0, UART_RX_BUF_SIZE);
    }

    ISR(USART_RX_vect)
    {
        if (rxBufferWp - rxBuffer >= UART_RX_BUF_SIZE)
        {
            // RX Buffer full, just silently drop all data.
            clearLine();
            return;
        }

        while (!(UCSR0A & (1 << RXC0)))
            ;

        *(rxBufferWp++) = toupper(UDR0);

        while (bit_is_clear(UCSR0A, UDRE0))
            ;
            
        if(*(rxBufferWp - 1) == KEY_BACKSPACE) {
            UDR0 = KEY_DEL;
        }

        UDR0 = *(rxBufferWp - 1);

        if ((*(rxBufferWp - 1) == KEY_BACKSPACE || *(rxBufferWp - 1) == KEY_DEL) && (rxBufferWp - 2) >= rxBuffer)
        {
            rxBufferWp -= 2;
        }

        *rxBufferWp = '\0';
    }

    void print(const char c)
    {
        while (bit_is_clear(UCSR0A, UDRE0))
            ;

        UDR0 = c;
    }

    void printSiString(uint8_t index)
    {
        const char *menu_pgm_ptr = (PGM_P)pgm_read_word(&(si_strings[index]));

        char buffer[UART_TMP_BUF_SIZE];
        size_t offset = 0;
        
        while (true)
        {
            size_t srcLen = strlcpy_P(buffer, menu_pgm_ptr + offset, UART_TMP_BUF_SIZE);
            
            print(buffer);

            offset += strlen(buffer);    
            if (strlen(buffer) == srcLen)
            {
                break;
            }
        }
    }

    void print(const char *text)
    {
        printf(text, NULL);
    }

    void printf(const char *__fmt, ...)
    {
        char *buf = AMOS::getBuffer();

        va_list ap;
        va_start(ap, __fmt);
        vsprintf(buf, __fmt, ap);
        va_end(ap);

        while (*buf)
        {
            while (bit_is_clear(UCSR0A, UDRE0))
                ;

            UDR0 = *(buf++);
        }
    }

    bool isCharAvailable()
    {
        return (rxBufferRp != rxBufferWp);
    }

    char peekChar()
    {
        return *rxBufferRp;
    }

    char getChar()
    {
        char c = *rxBufferRp++;

        if ((rxBufferRp - rxBuffer) >= UART_RX_BUF_SIZE)
        {
            rxBufferRp = rxBuffer;
        }

        return c;
    }

    bool isLineAvailable()
    {
        return strchr(rxBuffer, '\r') != NULL;
    }

    char *getLine()
    {
        return rxBuffer;
    }

    void clearLine()
    {
        memset(rxBuffer, 0, UART_RX_BUF_SIZE);
        rxBufferWp = rxBuffer;
        rxBufferRp = rxBuffer;
    }
}