EESchema Schematic File Version 2
LIBS:power
LIBS:device
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:special
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
LIBS:Pong.0
LIBS:Pong.0-cache
EELAYER 27 0
EELAYER END
$Descr B 17000 11000
encoding utf-8
Sheet 2 2
Title ""
Date "6 sep 2013"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L AS1117 U?
U 1 1 52299B59
P 4400 2100
F 0 "U?" H 4700 1050 60  0000 C CNN
F 1 "AS1117" H 4200 3150 60  0000 C CNN
F 2 "" H 4400 2100 60  0000 C CNN
F 3 "" H 4400 2100 60  0000 C CNN
	1    4400 2100
	1    0    0    -1  
$EndComp
$Comp
L AS1117 U?
U 1 1 52299B66
P 4400 4550
F 0 "U?" H 4700 3500 60  0000 C CNN
F 1 "AS1117" H 4200 5600 60  0000 C CNN
F 2 "" H 4400 4550 60  0000 C CNN
F 3 "" H 4400 4550 60  0000 C CNN
	1    4400 4550
	1    0    0    -1  
$EndComp
$Comp
L AS1117 U?
U 1 1 52299B76
P 4400 7000
F 0 "U?" H 4700 5950 60  0000 C CNN
F 1 "AS1117" H 4200 8050 60  0000 C CNN
F 2 "" H 4400 7000 60  0000 C CNN
F 3 "" H 4400 7000 60  0000 C CNN
	1    4400 7000
	1    0    0    -1  
$EndComp
$Comp
L PCA9306 U?
U 1 1 52299DD8
P 2200 1700
F 0 "U?" H 2450 1250 60  0000 C CNN
F 1 "PCA9306" H 2050 2150 60  0000 C CNN
F 2 "" H 2650 1700 60  0000 C CNN
F 3 "" H 2650 1700 60  0000 C CNN
	1    2200 1700
	1    0    0    -1  
$EndComp
Wire Wire Line
	2850 1800 3750 1800
Wire Wire Line
	3750 1700 3300 1700
Wire Wire Line
	3300 1700 3300 2000
Wire Wire Line
	3300 2000 2850 2000
Text Notes 1900 2200 0    60   ~ 0
I2C Level\nTranslator
$EndSCHEMATC
