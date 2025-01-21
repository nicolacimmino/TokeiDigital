#include "SerialInterface.h"

namespace SerialInterface
{
    namespace
    {
        bool outputEnabled = true;

        void printSingleRegister(int address, bool printNewLine = false)
        {
            UART::printf("%04X  %02X .", address, Registers::get(address));

            if (printNewLine)
            {
                UART::print("\r\n");
            }
        }

        void printTime()
        {
            DateTime t = DS1307::getDateTime();

            UART::printf("%02d:%02d ", t.h, t.m);
            UART::printf("%d ", t.dayOfWeek);
            UART::printf("%02d-%02d-%02d\r\n", t.dayOfMonth, t.month, t.year);
        }

        void setTime()
        {
            DateTime t;

            // eg T 15:42 1 6-1-24
            t.h = atoi(strtok(NULL, ":"));
            t.m = atoi(strtok(NULL, " "));
            t.s = 0;
            t.dayOfWeek = atoi(strtok(NULL, " "));
            t.dayOfMonth = atoi(strtok(NULL, "-"));
            t.month = atoi(strtok(NULL, "-"));
            t.year = (atoi(strtok(NULL, "\r\n")) % 100);

            DS1307::setDateTime(t);
        }

        void setRegisters(uint8_t address)
        {
            char *token;

            printSingleRegister(address);

            UART::clearLine();

            while (true)
            {
                wdt_reset();
                AMOS::loop();

                while (UART::isLineAvailable())
                {
                    token = strtok(UART::getLine(), " \r\n");

                    if (strncmp(token, "X", 1) == 0)
                    {
                        UART::print("\r\n");
                        UART::clearLine();
                        return;
                    }

                    if (strncmp(token, "", 1) != 0)
                    {
                        Registers::set(address, strtoul(token, NULL, 16));
                    }

                    printSingleRegister(address, true);

                    address = (address + 1) % REGISTERS_SIZE;

                    printSingleRegister(address);

                    UART::clearLine();
                }
            }
        }
    }

    void init()
    {
        AMOS::queueEveryMs(SI_INTERVAL_MS, SerialInterface::outputClockData);
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
            SerialInterface::outputEnabled = false;
        }

        if (UART::isLineAvailable())
        {
            if (SerialInterface::serveClient() == 0)
            {
                UART::print("OK\r\n");
            }
            else
            {
                UART::print("ERR\r\n");
            }
            UART::clearLine();
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
                         Registers::get(address),
                         ((address % SI_DUMP_PER_LINE) != (SI_DUMP_PER_LINE - 1)) ? "." : "\r\n");
        }

        if (end % SI_DUMP_PER_LINE != (SI_DUMP_PER_LINE - 1))
        {
            UART::print("\r\n");
        }
    }

    void outputClockData()
    {
        DateTime t = DS1307::getDateTime();

        if (SerialInterface::outputEnabled && Registers::get(SERIAL_ENABLE_REGISTER) == 1)
        {
            UART::printf("$V1!INFO0,%s,%s\r\n", FW_VER, FACE_ID);
            UART::printf("$V1!DATE,%02d-%02d-20%02d\r\n", t.dayOfMonth, t.month, t.year);
            UART::printf("$V1!TIME,%02d:%02d:%02d\r\n", t.h, t.m, t.s);
            UART::printf("$V1!DOW,%d\r\n", t.dayOfWeek);
            UART::printf("$V1!DST,%s", t.dst ? "ON" : "OFF");
            if (Registers::get(DST_REGISTER) == DST_AUTO)
            {
                UART::print(",AUTO");
            }
            UART::print("\r\n");
        }
    }

    uint8_t serveClient()
    {
        int p0 = 0;
        int p1 = REGISTERS_SIZE - 1;
        UART::print("\r\n");

        char *token = strtok(UART::getLine(), " \r\n");

        char cmd = *token;

        if (cmd != 'T')
        {
            token = strtok(NULL, " ");
            if (token != NULL)
            {
                p0 = strtoul(token, NULL, 16);
            }

            token = strtok(NULL, " ");
            if (token != NULL)
            {
                p1 = strtoul(token, NULL, 16);
            }
        }

        switch (cmd)
        {
        case 'H':
        case NULL:
            UART::printSiString(S_SI_BANNER);
            break;
        case 'N':
            printTime();
            break;
        case 'T':
            setTime();
            break;
        case 'S':
            setRegisters(p0);
            break;
        case 'R':
            dumpRegisters(p0, p1);
            break;
        case 'X':
            outputEnabled = true;
            return 0;
        default:
            return 1;
        }

        return 0;
    }
}