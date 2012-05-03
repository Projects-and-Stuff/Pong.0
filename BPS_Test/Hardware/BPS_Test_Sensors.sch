EESchema Schematic File Version 2  date 5/2/2012 11:05:33 PM
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
LIBS:BPS_Test
LIBS:BPS_Test-cache
EELAYER 25  0
EELAYER END
$Descr A4 11700 8267
encoding utf-8
Sheet 2 3
Title "BPS Test Sensors"
Date "3 may 2012"
Rev "1.0"
Comp "Projects and Stuff LLC"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Connection ~ 4700 2600
Wire Wire Line
	4700 4650 4700 1900
Wire Wire Line
	4700 1900 3150 1900
Wire Wire Line
	3150 1900 3150 1800
Connection ~ 4700 4000
Wire Wire Line
	4700 3300 3150 3300
Wire Wire Line
	3150 3300 3150 3200
Wire Wire Line
	2800 3750 3000 3750
Wire Wire Line
	2800 2350 3000 2350
Wire Wire Line
	3950 5900 4100 5900
Wire Wire Line
	4100 5900 4100 5800
Wire Wire Line
	2200 3050 2300 3050
Wire Wire Line
	2200 1650 2300 1650
Wire Wire Line
	4100 5000 4100 5300
Wire Wire Line
	4000 4650 4200 4650
Wire Wire Line
	4100 4800 4100 4650
Connection ~ 4100 4650
Wire Wire Line
	4000 5150 4100 5150
Connection ~ 4100 5150
Wire Wire Line
	2200 2350 2300 2350
Wire Wire Line
	2200 3750 2300 3750
Wire Wire Line
	2800 1650 3000 1650
Wire Wire Line
	2800 3050 3000 3050
Wire Wire Line
	3150 3900 3150 4000
Wire Wire Line
	3150 4000 4700 4000
Wire Wire Line
	3150 2500 3150 2600
Wire Wire Line
	3150 2600 4700 2600
Connection ~ 4700 3300
$Comp
L CAPSENSOR X3Y0
U 1 1 4FA1E531
P 3150 3750
F 0 "X3Y0" H 3110 3870 60  0000 C CNN
F 1 "CAPSENSOR" V 3260 3740 40  0000 C CNN
	1    3150 3750
	1    0    0    -1  
$EndComp
$Comp
L CAPSENSOR X2Y0
U 1 1 4FA1E52F
P 3150 3050
F 0 "X2Y0" H 3110 3170 60  0000 C CNN
F 1 "CAPSENSOR" V 3260 3040 40  0000 C CNN
	1    3150 3050
	1    0    0    -1  
$EndComp
$Comp
L CAPSENSOR X1Y0
U 1 1 4FA1E528
P 3150 2350
F 0 "X1Y0" H 3110 2470 60  0000 C CNN
F 1 "CAPSENSOR" V 3260 2340 40  0000 C CNN
	1    3150 2350
	1    0    0    -1  
$EndComp
$Comp
L CAPSENSOR X0Y0
U 1 1 4FA1E50B
P 3150 1650
F 0 "X0Y0" H 3110 1770 60  0000 C CNN
F 1 "CAPSENSOR" V 3260 1640 40  0000 C CNN
	1    3150 1650
	1    0    0    -1  
$EndComp
$Comp
L R RYB0
U 1 1 4FA1E1CF
P 4100 5550
F 0 "RYB0" V 4180 5550 50  0000 C CNN
F 1 "470k" V 4100 5550 50  0000 C CNN
	1    4100 5550
	-1   0    0    1   
$EndComp
Text HLabel 3950 5900 0    60   BiDi ~ 0
SMP
$Comp
L CSMALL CS0
U 1 1 4FA1E0FA
P 4100 4900
F 0 "CS0" H 4125 4950 30  0000 L CNN
F 1 "4.7nF" H 4125 4850 30  0000 L CNN
	1    4100 4900
	1    0    0    -1  
$EndComp
$Comp
L R RY0
U 1 1 4FA1E0DF
P 4450 4650
F 0 "RY0" V 4530 4650 50  0000 C CNN
F 1 "1k" V 4450 4650 50  0000 C CNN
	1    4450 4650
	0    -1   -1   0   
$EndComp
Text HLabel 4000 5150 0    60   BiDi ~ 0
Y0B
$Comp
L R RX3
U 1 1 4FA1E026
P 2550 3750
F 0 "RX3" V 2630 3750 50  0000 C CNN
F 1 "1k" V 2550 3750 50  0000 C CNN
	1    2550 3750
	0    -1   -1   0   
$EndComp
$Comp
L R RX2
U 1 1 4FA1E01D
P 2550 3050
F 0 "RX2" V 2630 3050 50  0000 C CNN
F 1 "1k" V 2550 3050 50  0000 C CNN
	1    2550 3050
	0    -1   -1   0   
$EndComp
$Comp
L R RX1
U 1 1 4FA1E019
P 2550 2350
F 0 "RX1" V 2630 2350 50  0000 C CNN
F 1 "1k" V 2550 2350 50  0000 C CNN
	1    2550 2350
	0    -1   -1   0   
$EndComp
$Comp
L R RX0
U 1 1 4FA1E001
P 2550 1650
F 0 "RX0" V 2630 1650 50  0000 C CNN
F 1 "1k" V 2550 1650 50  0000 C CNN
	1    2550 1650
	0    -1   -1   0   
$EndComp
Text HLabel 4000 4650 0    60   BiDi ~ 0
Y0A
Text HLabel 2200 3750 0    60   BiDi ~ 0
X3
Text HLabel 2200 3050 0    60   BiDi ~ 0
X2
Text HLabel 2200 2350 0    60   BiDi ~ 0
X1
Text HLabel 2200 1650 0    60   BiDi ~ 0
X0
$EndSCHEMATC
