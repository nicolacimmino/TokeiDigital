# TokeiDigital

TokeiDigital [to̞ke̞ːdigital] is a line of clocks for geeks, nerds, and makers that come in a kit to assemble. The clock faces are unconventional and, true to the spirit of this project, always abound with colorful, blinking lights. These clocks are for you if:

* You believe a UART is a cool interface to setup a clock.
* You believe setting registers HEX values in a terminal is a good way to change LED colors and tweak settings.
* You believe a clock that just shows time with two hands is boring.

All models come with full schematics, PBCs, source code and all tools needed to program the clock. The project is OSHWA certified (PL000019).
![image](https://github.com/user-attachments/assets/ee7bf6c8-7b72-4825-88ab-eacb489856fa)

Additionally you can buy them as kits ready to assemble on Tindie: https://www.tindie.com/stores/nicola_cimmino/

### Models

|Model|Description|Time|Date|DST|LEDs|Movement|
|-----|-----------|--------|----|---|----|----|
| [TD-FBUH01](board/TD-FBUH01) |Reproduction of the iconic Berlin Uhr.|X||X*1|36 RGB|TD-M5037|
| [TD-FDCF01](board/TD-FDCF01) |60 LEDs display time encoded as DCF77 bitstream.|X|X|X|60 RGB|TD-M5037|

<sub>*1) How can DST work if there's no date? The movment, internally, tracks the date, but the clock face doesn't have LEDs to show it. For DST Auto to work correctly you will need to set the date using the serial interface.</sub>

### Movements

|Model|Description|UART|Backup<br>Battery|W (mm)|H (mm)|
|-----|-----------|--------|----|---|----|
| [TD-M5037](board/TD-M5037)|Basic but powerful movement.|X|CR2032|50|37|
