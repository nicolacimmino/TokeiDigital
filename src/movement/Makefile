######################################################################
## Usage:
## make flash EXTRAFLAGS=-DFACE_BERLIN_UHR
######################################################################

DEVICE     = atmega328p
CLOCK      = 16000000UL
PROGRAMMER = -c USBasp

BUILD_DIR := .build

FUSES = -U lfuse:w:0xFF:m -U hfuse:w:0xDE:m -U efuse:w:0x05:m

######################################################################
# Toolchain

AVRDUDE = avrdude $(PROGRAMMER) -p $(DEVICE)
COMPILE = avr-gcc -std=c++11 -Wall -Os -DF_CPU=$(CLOCK) -mmcu=$(DEVICE) $(EXTRAFLAGS)

OBJCOPY = avr-objcopy
OBJDUMP = avr-objdump

######################################################################
# Recursive wildcard (pure GNU Make, Windows-safe)

rwildcard = \
  $(foreach d,$(wildcard $1*), \
    $(call rwildcard,$d/,$2)) \
  $(filter $(subst *,%,$2),$(wildcard $1$2))

SRC_CPP := $(call rwildcard,./,*.cpp)

OBJECTS := $(patsubst ./%, $(BUILD_DIR)/%, $(SRC_CPP:.cpp=.o))

######################################################################
# Targets

all: $(BUILD_DIR)/main.hex

flash: all
	$(AVRDUDE) -U flash:w:$(BUILD_DIR)/main.hex:i
	$(AVRDUDE) -U eeprom:w:faceDrivers/eeprom/$(EEPROM_FILE):i

fuse:
	$(AVRDUDE) $(FUSES)

install: fuse flash

clean:
	rm -rf $(BUILD_DIR)

######################################################################
# Linking

$(BUILD_DIR)/main.elf: $(OBJECTS)
	$(COMPILE) -o $@ $(OBJECTS)

######################################################################
# HEX / EEPROM

$(BUILD_DIR)/main.hex: $(BUILD_DIR)/main.elf
	rm -f $@
	$(OBJCOPY) -j .text -j .data -O ihex $< $@
	$(OBJCOPY) \
		-j .eeprom \
		--set-section-flags=.eeprom="alloc,load" \
		--change-section-lma .eeprom=0 \
		-O ihex $< $(BUILD_DIR)/main.eeprom.hex

######################################################################
# Compilation rule

$(BUILD_DIR)/%.o: %.cpp
	mkdir -p $(dir $@)
	$(COMPILE) -c $< -o $@

######################################################################
# Debug helpers

disasm: $(BUILD_DIR)/main.elf
	$(OBJDUMP) -d $<

cpp:
	$(COMPILE) -E main.cpp
