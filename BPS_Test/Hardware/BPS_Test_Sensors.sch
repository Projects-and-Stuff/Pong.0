EESchema Schematic File Version 2  date 5/5/2012 11:24:09 AM
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
$Descr User 14000 11000
encoding utf-8
Sheet 2 3
Title "BPS Test Sensors"
Date "5 may 2012"
Rev "1.0"
Comp "Projects and Stuff LLC"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text GLabel 7300 7350 2    60   Input ~ 0
GND
Wire Wire Line
	7150 7350 7300 7350
Connection ~ 7900 4400
Wire Wire Line
	7900 3700 6350 3700
Wire Wire Line
	6350 3700 6350 3600
Wire Wire Line
	7900 5100 6350 5100
Wire Wire Line
	6350 5100 6350 5000
Wire Wire Line
	6000 4150 6200 4150
Wire Wire Line
	6000 2750 6200 2750
Wire Wire Line
	5400 4850 5500 4850
Wire Wire Line
	5400 3450 5500 3450
Connection ~ 7300 6250
Wire Wire Line
	7200 6250 7300 6250
Connection ~ 7300 5750
Wire Wire Line
	7300 5900 7300 5750
Wire Wire Line
	7200 5750 7400 5750
Wire Wire Line
	7300 6100 7300 6400
Wire Wire Line
	5400 2750 5500 2750
Wire Wire Line
	5400 4150 5500 4150
Wire Wire Line
	7300 6900 7300 7000
Wire Wire Line
	7300 7000 7150 7000
Wire Wire Line
	6000 3450 6200 3450
Wire Wire Line
	6000 4850 6200 4850
Wire Wire Line
	6350 4300 6350 4400
Wire Wire Line
	6350 4400 7900 4400
Connection ~ 7900 5100
Wire Wire Line
	6350 2900 6350 3000
Wire Wire Line
	6350 3000 7900 3000
Wire Wire Line
	7900 3000 7900 5750
Connection ~ 7900 3700
Text HLabel 7150 7350 0    60   BiDi ~ 0
VREF
$Comp
L CAPSENSOR X3Y0
U 1 1 4FA1E531
P 6350 4850
F 0 "X3Y0" H 6310 4970 60  0000 C CNN
F 1 "CAPSENSOR" V 6460 4840 40  0000 C CNN
	1    6350 4850
	1    0    0    -1  
$EndComp
$Comp
L CAPSENSOR X2Y0
U 1 1 4FA1E52F
P 6350 4150
F 0 "X2Y0" H 6310 4270 60  0000 C CNN
F 1 "CAPSENSOR" V 6460 4140 40  0000 C CNN
	1    6350 4150
	1    0    0    -1  
$EndComp
$Comp
L CAPSENSOR X1Y0
U 1 1 4FA1E528
P 6350 3450
F 0 "X1Y0" H 6310 3570 60  0000 C CNN
F 1 "CAPSENSOR" V 6460 3440 40  0000 C CNN
	1    6350 3450
	1    0    0    -1  
$EndComp
$Comp
L CAPSENSOR X0Y0
U 1 1 4FA1E50B
P 6350 2750
F 0 "X0Y0" H 6310 2870 60  0000 C CNN
F 1 "CAPSENSOR" V 6460 2740 40  0000 C CNN
	1    6350 2750
	1    0    0    -1  
$EndComp
$Comp
L R RYB0
U 1 1 4FA1E1CF
P 7300 6650
F 0 "RYB0" V 7380 6650 50  0000 C CNN
F 1 "470k" V 7300 6650 50  0000 C CNN
	1    7300 6650
	-1   0    0    1   
$EndComp
Text HLabel 7150 7000 0    60   BiDi ~ 0
SMP
$Comp
L CSMALL CS0
U 1 1 4FA1E0FA
P 7300 6000
F 0 "CS0" H 7325 6050 30  0000 L CNN
F 1 "4.7nF" H 7325 5950 30  0000 L CNN
	1    7300 6000
	1    0    0    -1  
$EndComp
$Comp
L R RY0
U 1 1 4FA1E0DF
P 7650 5750
F 0 "RY0" V 7730 5750 50  0000 C CNN
F 1 "1k" V 7650 5750 50  0000 C CNN
	1    7650 5750
	0    -1   -1   0   
$EndComp
Text HLabel 7200 6250 0    60   BiDi ~ 0
Y0B
$Comp
L R RX3
U 1 1 4FA1E026
P 5750 4850
F 0 "RX3" V 5830 4850 50  0000 C CNN
F 1 "1k" V 5750 4850 50  0000 C CNN
	1    5750 4850
	0    -1   -1   0   
$EndComp
$Comp
L R RX2
U 1 1 4FA1E01D
P 5750 4150
F 0 "RX2" V 5830 4150 50  0000 C CNN
F 1 "1k" V 5750 4150 50  0000 C CNN
	1    5750 4150
	0    -1   -1   0   
$EndComp
$Comp
L R RX1
U 1 1 4FA1E019
P 5750 3450
F 0 "RX1" V 5830 3450 50  0000 C CNN
F 1 "1k" V 5750 3450 50  0000 C CNN
	1    5750 3450
	0    -1   -1   0   
$EndComp
$Comp
L R RX0
U 1 1 4FA1E001
P 5750 2750
F 0 "RX0" V 5830 2750 50  0000 C CNN
F 1 "1k" V 5750 2750 50  0000 C CNN
	1    5750 2750
	0    -1   -1   0   
$EndComp
Text HLabel 7200 5750 0    60   BiDi ~ 0
Y0A
Text HLabel 5400 4850 0    60   BiDi ~ 0
X3
Text HLabel 5400 4150 0    60   BiDi ~ 0
X2
Text HLabel 5400 3450 0    60   BiDi ~ 0
X1
Text HLabel 5400 2750 0    60   BiDi ~ 0
X0
$EndSCHEMATC
