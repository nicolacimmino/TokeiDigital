#include "registers.h"

namespace Registers
{
    Color::RGB getPersistentAsColor(uint8_t address)
    {
        return {
            .r = getPersistent(address),
            .g = getPersistent(address + 1),
            .b = getPersistent(address + 2)};
    }

    uint8_t getPersistent(uint8_t address)
    {
        return eeprom_read_byte((uint8_t *)address);
    }

    void setPersistent(uint8_t address, uint8_t value)
    {
        eeprom_write_byte((uint8_t *)address, value);
    }
}