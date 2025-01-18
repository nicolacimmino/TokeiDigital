#include "SerialInterface.h"

namespace SerialInterface
{
    namespace
    {
        bool outputEnabled = true;
    }

    void init()
    {
        AMOS::queueEveryMs(SI_INTERVAL_MS, sendSerialInterface);
        AMOS::queueEveryMs(100, processSerialInterface);
        UART::init();
    }

    void printStartup()
    {
        UART::printSiString(S_SI_STARTUP);
    }

    void printStartupDone()
    {
        UART::printSiString(S_SI_STARTUP_DONE);
    }

    void processSerialInterface()
    {
        if (UART::isCharAvailable())
        {
            outputEnabled = false;
        }

        if (UART::isLineAvailable())
        {
            UART::print("\r\n");

            char *token = strtok(UART::getLine(), " \r\n");

            if (strcmp(token, "") == 0)
            {
                UART::printSiString(S_SI_BANNER);
            }
            else if (strcmp(token, "TSET") == 0)
            {
                DateTime t;

                // eg TSET 15:42 1 6-1-24
                t.h = atoi(strtok(NULL, ":"));
                t.m = atoi(strtok(NULL, " "));
                t.s = 0;
                t.dayOfWeek = atoi(strtok(NULL, " "));
                t.dayOfMonth = atoi(strtok(NULL, "-"));
                t.month = atoi(strtok(NULL, "-"));
                t.year = atoi(strtok(NULL, "\r\n"));

                DS1307::setDateTime(t);

                UART::print("OK\r\n");
            }
            else if (strcmp(token, "WREG") == 0)
            {
                uint8_t address = strtoul(strtok(NULL, " "), NULL, 16);

                char *nextToken;
                do
                {
                    nextToken = strtok(NULL, ".");
                    if (nextToken != NULL)
                    {
                        Registers::setPersistent(address++, strtoul(nextToken, NULL, 16));
                    }
                } while (nextToken != NULL);

                UART::print("OK\r\n");
            }
            else if (strcmp(token, "DREG") == 0)
            {
                dumpRegisters(0, REGISTERS_PERSISTENT_SIZE - 1);
                UART::print("OK\r\n");
            }
            else if (strcmp(token, "EXIT") == 0)
            {
                outputEnabled = true;
            }
            else
            {
                UART::print("ERR\r\n");
            }
            UART::clearLine();
        }
    }

    void sendSerialInterface()
    {
        if (outputEnabled)
        {
            DateTime t = DS1307::getDateTime();

            UART::printf("INFO0.%s.%s\r\n", FW_VER, FACE_ID);
            UART::printf("LDATE.20%02d-%02d-%02d\r\n", t.year, t.month, t.dayOfMonth);
            UART::printf("LTIME.%02d:%02d:%02d\r\n", t.h, t.m, t.s);
            UART::printf("DST.%s", t.dst ? "ON" : "OFF");
            if (Registers::getPersistent(DST_REGISTER) == DST_AUTO)
            {
                UART::print(".AUTO");
            }
            UART::print("\r\n");
        }
    }

    void dumpRegisters(int start, int end)
    {
        for (int address = start - (start % SI_DUMP_PER_LINE); address < end + 1; address++)
        {
            if (address % SI_DUMP_PER_LINE == 0)
            {
                UART::printf("%04X  ", address);
            }

            if (address < start)
            {
                UART::printf("   ");
                continue;
            }

            UART::printf("%02X%s",
                         Registers::getPersistent(address),
                         ((address % SI_DUMP_PER_LINE) != (SI_DUMP_PER_LINE - 1)) ? "." : "\r\n");
        }

        if (end % SI_DUMP_PER_LINE != (SI_DUMP_PER_LINE - 1))
        {
            UART::print("\r\n");
        }
    }
}
