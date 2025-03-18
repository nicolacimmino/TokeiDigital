#!/bin/bash

tput clear      # clear the screen

function flash() {
	SEL_FACE_LCASE=`echo $SEL_FACE | tr '[:upper:]' '[:lower:]'`
    make clean && make fuse && make flash EXTRAFLAGS=-DFACE_$SEL_FACE EEPROM_FILE=$SEL_FACE_LCASE.eeprom.hex
    read -p "Press a key to continue"
    tput clear    
}

GREEN='\e[32m'
BLUE='\e[34m'
CLEAR='\e[0m'
CLEAR_TO_END='\033[0K'
YELLOW='\e[33m'
RED='\e[31m'
SEL_FACE="NONE"

menu(){
	tput cup 1 0
	echo -e $YELLOW"Clock Movement Flasher ($SEL_FACE)"$CLEAR_TO_END	
    tput cup 4 0;echo -e $YELLOW"------ FACE ------"$CLEAR
	tput cup 5 0;echo -e $GREEN"1)"$CLEAR" None"
    tput cup 6 0;echo -e $GREEN"2)"$CLEAR" Berlin Uhr"        
	tput cup 7 0;echo -e $GREEN"3)"$CLEAR" DCF77"        
    tput cup 9 0;echo -e $YELLOW"-----------------------"$CLEAR
	tput cup 11 0;echo -e $GREEN"9)"$CLEAR" Flash"
    tput cup 12 0;echo -e $GREEN"0)"$CLEAR" Exit"
	tput cup 13 0;echo -ne $BLUE":"$CLEAR
	read -rN1 option
	tput cup 13 0;echo -ne "                      ";
        tput cup 13 0
	case $option in
		1) SEL_FACE=NONE ; menu ;;	
        2) SEL_FACE=BERLIN_UHR ; menu ;;                 		
		3) SEL_FACE=DCF77 ; menu ;;                 		
		9) flash; menu ;;
        0) tput ed; exit 0 ;;
		*) menu ;;
	esac
}

menu
