Clock Mode
- Press SEL repeatedly until nneither of the SEL1,2,3 led is on
- Display will show the current time

Timer Selection
- Press SEL repeatedly until the SEL1,2,3 led for  the desired timer is on
- Display shows the time for the selected timer
- Any input and start/stop operations will affect the selected timer only
- Other timers keep running in background if active

Timer Setup (plus minus mode)
- Ensure NUM is off (FN+NUM to toggle) 
- Ensure you are not in a special PRE or PRG mode (both PRE and PRG leds should be off, can be toggled with FN+PRE and FN+PRG respectively)
- Press +10,+5,+1,-1 repeatedly in the hours, minutes, seconds column to set the time
- Enter any time other than 00:00:00 otherwise you will be in chronometer mode, this is confirmed by the CHRONO led for the selected timer being off.
- Press Start
- Timer will start to count down and the RUN led will be on

Timer Setup (numeric input mode)
- Ensure NUM is on (FN+NUM to toggle)
- Ensure you are not in a special PRE or PRG mode (both PRE and PRG leds should be off, can be toggled with FN+PRE and FN+PRG respectively)
- Use the 0-9 digits to enter the time, digits will appear from the right and shift left
- Enter any time other than 00:00:00 otherwise you will be in chronometer mode, this is confirmed by the CHRONO led for the selected timer being off.
- Press Start
- Timer will start to count down and the RUN led will be on

Timer Setup (preset load)
- Ensure PRE is on (FN+PRE to toggle)
- NUM will also light up confirming the keypad is in numeric input mode
- Use the 0-9 digits to load a time preset
- The time will show on the digits and PRE mode will exit (LED off)
- NUM will reset to what it was before loading the preset
- You can still change the loaded preset at this point acting as in the previous sections according to your input mode.
- Press Start
- Timer will start to count down and the RUN led will be on

Timer Operation
- With the timer in STOP state enter the start time with any of the modes describe above
- Press Start
- Timer will start to count down and the RUN led will be on
- While timer is running the corresponding RUN led is steady on
- In running state it's possible to STOP  the timer pressing the START/STOP/AD key
- In STOP mode the STOP and RUN led is on steady 
- In STOP mode the timer stops, pressing START/STOP/AD again resumes the countdown
- Pressing START/STOP/AD again resumes the count
- Pressing LAP/RESET while in STOP resets the timer to the previous start value
- Pressing LAP/RESET while in RUN keeps the timer running but freezes the display to the lap value, the reading on the display will be flashing to indicate it's a lap. RUN will stay on as the timer is running
- Pressing LAP/RESET again in lap mode will resume normal display
- When the timer reaches zero the buzzer will sound for 5 seconds and the T/O led for the relevant timer will flash. After 5 seconds the buzzer will stop and the T/O led will be on steady. RUN and STOP will be off as soon as T/O in reached.
- To re-start the time from the T/O state you need to either press LAP/RESET to reset to the previous value or enter a new start value with any of the 3 modes.

Chronometer Operation
- Any of T1,T2,T3 can function as a chronograph
- Set a start value of 00:00:00 with any input mode, the CHRONO led for the timer will be lit
- Press START/STOP the timer will start running counting up, RUN will be steady lit
- PRESS LAP/RESET while the timer is running to enter LAP mode, RUN will still be lit
- In LAP mode the display will freeze and the current count will flash 
- In LAP mode the chronograph keeps running in the background
- Press again LAP/RESET to resume normal display
- Press START/STOP while the chronometer is running to stop it, RUN and STOP will be lit
- Press START/STOP again to resume running (only RUN will be lit)
- Press LAP/RESET while the chronometer is stopped to reset it back to 00:00:00

Setting Presets
- Enter a time normally either in NUM or +/- mode
- Store the preset by first entering PRE mode (FN+PRE) then long pressing the number (0-9) of the preset you want to store
- The chosen preset number will flash in the center digits of the numeric display to confirm and after 2 seconds the PRE mode will be exited

Muting the Buzzer
	- Buzzer can be muted/unmuted while in any operation mode except PRE and PRG by pressing FN+MUTE
	- Buzzer is muted/unmuted for all timers
	- The MUTE led will be on if the buzzer is muted

Writing Memory
- Enter PRG mode (FN+PRG)
- NUM led will be on signalling the 0-9 and A-F (hexadecimal numbers) on the keypad are active
- In PRG mode all timers are inactive, all timer specific LEDs will be off to signal this, the display will show ADDR:DATA (4+2 digits)
- SEL/+ will act only as + as there is no timer selection
- START/STOP/AD will work as AD only
- LAP/RESET/DA will work as DA only
- Display will show 0000 (the address) and the last 2 digits will be the current content of location 0x0000
- Press AD to enter address, followed by the address. Inputted digits will appear on the right and scroll left at each new digit pressed. If you made a mistake keep typing until the address is correct. As you enter different addresses the right 2 digits will update to the value currently at that address.
- Press DA to enter data. Any hex number you type will enter from the rightmost digit and scroll left. Also here keep typing if you make a mistake until the shown number is correct. The value is saved as you type.
- Press + to advance the address staying in data input. This is convenient to store multiple consecutive values
- If you need to move to a different address repeat as in the start by pressing AD and typing in the new address followed by DA to start entering data.
- Once done exit PRG mode by selecting again FN+PRG
- NUM led will reset to that state it had before entring PRG mode.

Timers Sequencing
- Setup a timer start time
- Enter sequencer setup FN+SEQ
- Set the SEQ register either by pressing the key relative to the bit or inputting binary using the AD/DA keys as 0/1 (calculator style msb first)
- Exit SEQ mode
- Proceed to set the other timers as needed
- Sequencer bits:
	- T1 on T/O arm and trigger T1
	- T2 on T/O arm and trigger T2
	- T3 on T/O arm and trigger T3
	- ARMP3-0 which preset to arm (0 = timer default value, 1-15 P1-15)
	- NP (on trigger increment ARMP3-0), this will cause the next T/O to arm the following preset. The chain will stop if a preset has value 0

