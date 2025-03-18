
#include "uTWI.h"

namespace UTWI
{
    namespace
    {
        void start()
        {
            TWCR = (_BV(TWINT) | _BV(TWEN) | _BV(TWSTA));
            while ((TWCR & _BV(TWINT)) == 0)
                ;
        }

        void stop()
        {
            TWCR = (_BV(TWINT) | _BV(TWEN) | _BV(TWSTO));
        }

        void write(uint8_t data)
        {
            TWDR = data;
            TWCR = _BV(TWINT) | _BV(TWEN);
            while ((TWCR & _BV(TWINT)) == 0)
                ;
        }

        uint8_t read(bool ack)
        {
            TWCR = (_BV(TWINT) | _BV(TWEN) | (ack ? _BV(TWEA) : 0));
            while ((TWCR & _BV(TWINT)) == 0)
                ;

            return TWDR;
        }
    }

    void init()
    {
        PORTC |= _BV(PC4) | _BV(PC5); // Pull-ups        
        TWBR = 72;                    // 72 =  100KHz @16MHz clock
        TWCR |= (1 << TWEN);          // Enable TWI
    }

    void readRegisters(uint8_t addr, uint8_t reg, uint8_t *buf, uint8_t size)
    {
        start();
        write((addr << 1) | TW_WRITE);
        write(reg);
        start();
        write((addr << 1) | TW_READ);

        uint8_t ix = 0;
        while (ix < size - 1)
        {
            buf[ix++] = read(TWI_ACK);
        }
        buf[ix] = read(TWI_NACK);

        stop();
    }

    void writeRegisters(uint8_t addr, uint8_t reg, uint8_t *buf, uint8_t size)
    {
        start();        
        write((addr << 1) | TW_WRITE);        
        write(reg);
        
        uint8_t ix = 0;
        do
        {
            write(buf[ix++]);
        } while (ix < size);

        stop();
    }
}
