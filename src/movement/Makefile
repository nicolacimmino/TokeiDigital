
######################################################################
## You need to specify the active face in the make, eg:
## make flash EXTRAFLAGS=-DFACE_BERLIN_UHR
######################################################################

DEVICE     = atmega328p
CLOCK      = 16000000UL
PROGRAMMER = -c USBasp
OBJECTS    = Registers.o\
				SerialInterface.o\
				Button.o\
				AMOS.o\
				DS1307.o\
				main.o\
				uTWI.o\
				UART.o\
				ws2812.o\
				TimeSetter.o\
				faceDrivers\BerlinUhr.o\
				faceDrivers\DCF77.o\
				faceDrivers\Face.o\
				Color.o
				
 FUSES      = -U lfuse:w:0xFF:m -U hfuse:w:0xDE:m -U efuse:w:0x05:m
######################################################################
######################################################################

# Tune the lines below only if you know what you are doing:

AVRDUDE = avrdude $(PROGRAMMER) -p $(DEVICE)
COMPILE = C:\Users\nicola\AppData\Local\Arduino15\packages\arduino\tools\avr-gcc\7.3.0-atmel3.6.1-arduino5\bin\avr-gcc -std=c++11 -Wall -Os -DF_CPU=$(CLOCK) -mmcu=$(DEVICE) $(EXTRAFLAGS)

# symbolic targets:
all:	main.hex

.cpp.o:
	$(COMPILE) -c $< -o $@

.S.o:
	$(COMPILE) -x assembler-with-cpp -c $< -o $@
# "-x assembler-with-cpp" should not be necessary since this is the default
# file type for the .S (with capital S) extension. However, upper case
# characters are not always preserved on Windows. To ensure WinAVR
# compatibility define the file type manually.

.c.s:
	$(COMPILE) -S $< -o $@

flash:	all
	$(AVRDUDE) -U flash:w:main.hex:i
	$(AVRDUDE) -U eeprom:w:faceDrivers/eeprom/$(EEPROM_FILE):i
	
fuse:
	$(AVRDUDE) $(FUSES)

install: flash fuse

clean:
	rm -f main.hex main.elf main.c.lst main.lss main.map $(OBJECTS)

# file targets:
main.elf: $(OBJECTS)
	$(COMPILE) -o main.elf $(OBJECTS)

main.hex: main.elf
	rm -f main.hex
	avr-objcopy -j .text -j .data -O ihex main.elf main.hex
	avr-objcopy\
		-j .eeprom\
		--set-section-flags=.eeprom="alloc,load"\
		--change-section-lma .eeprom=0\
		-O ihex main.elf main.eeprom.hex

# Targets for code debugging and analysis:
disasm:	main.elf
	avr-objdump -d main.elf

cpp:
	$(COMPILE) -E main.c