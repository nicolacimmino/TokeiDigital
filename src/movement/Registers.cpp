#include "registers.h"

namespace Registers
{
    Color::RGB getAsColor(uint8_t address)
    {
        return {
            .r = get(address),
            .g = get(address + 1),
            .b = get(address + 2)};
    }

    uint8_t get(uint8_t address)
    {
        return eeprom_read_byte((uint8_t *)address);
    }

    void set(uint8_t address, uint8_t value)
    {
        eeprom_write_byte((uint8_t *)address, value);
    }
}