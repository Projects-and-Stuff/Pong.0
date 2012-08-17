EESchema Schematic File Version 2  date 8/16/2012 10:38:15 PM
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
LIBS:Pong.0_Test
LIBS:Pong.0_Test-cache
EELAYER 25  0
EELAYER END
$Descr A4 11700 8267
encoding utf-8
Sheet 3 4
Title ""
Date "17 aug 2012"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Wire Wire Line
	1500 1150 1500 1250
Wire Wire Line
	7200 1100 7150 1100
Wire Wire Line
	6350 1200 6300 1200
Wire Wire Line
	6350 1000 6300 1000
Wire Wire Line
	1500 1500 1300 1500
Wire Wire Line
	1300 1500 1300 1450
Connection ~ 1500 1200
Wire Wire Line
	1300 1200 1650 1200
Wire Wire Line
	1300 1200 1300 1250
Wire Wire Line
	1700 2800 1650 2800
Connection ~ 1650 3650
Wire Wire Line
	1650 3700 1650 3450
Wire Wire Line
	1650 3450 1700 3450
Wire Wire Line
	1650 2000 1650 1950
Wire Wire Line
	1650 1950 1700 1950
Wire Wire Line
	1050 1900 1050 1950
Wire Wire Line
	1700 1450 1650 1450
Wire Wire Line
	1650 1350 1700 1350
Wire Wire Line
	3600 1350 3650 1350
Wire Wire Line
	3600 1550 3650 1550
Wire Wire Line
	3600 1450 3650 1450
Wire Wire Line
	3600 1650 3650 1650
Wire Wire Line
	3600 2050 3650 2050
Wire Wire Line
	3600 1850 3650 1850
Wire Wire Line
	3600 1950 3650 1950
Wire Wire Line
	3600 1750 3650 1750
Wire Wire Line
	3600 2600 3650 2600
Wire Wire Line
	3600 2800 3650 2800
Wire Wire Line
	3600 2700 3650 2700
Wire Wire Line
	3600 2500 3650 2500
Wire Wire Line
	3600 2300 3650 2300
Wire Wire Line
	3600 2400 3650 2400
Wire Wire Line
	3600 2200 3650 2200
Wire Wire Line
	3600 3350 3650 3350
Wire Wire Line
	3600 3550 3650 3550
Wire Wire Line
	3600 3450 3650 3450
Wire Wire Line
	3600 3650 3650 3650
Wire Wire Line
	3600 3250 3650 3250
Wire Wire Line
	3600 3050 3650 3050
Wire Wire Line
	3600 3150 3650 3150
Wire Wire Line
	3600 2950 3650 2950
Connection ~ 1650 1350
Wire Wire Line
	1050 1000 1050 950 
Wire Wire Line
	1050 1700 1050 1600
Connection ~ 1050 1650
Wire Wire Line
	1650 2200 1650 2250
Wire Wire Line
	1650 3650 1700 3650
Wire Wire Line
	1700 3550 1650 3550
Connection ~ 1650 3550
Wire Wire Line
	1700 2700 1650 2700
Wire Wire Line
	1700 1650 1050 1650
Wire Wire Line
	1650 1450 1650 1200
Wire Wire Line
	1500 1550 1500 1450
Connection ~ 1500 1500
Wire Wire Line
	6350 1100 6300 1100
Wire Wire Line
	7150 1000 7200 1000
Wire Wire Line
	7150 1200 7200 1200
Text Label 6300 1200 2    60   ~ 0
RESET
Text Label 6300 1100 2    60   ~ 0
SCK_2
Text Label 6300 1000 2    60   ~ 0
MISO_2
Text Label 7200 1200 0    60   ~ 0
GND
Text Label 7200 1100 0    60   ~ 0
MOSI_2
Text Label 7200 1000 0    60   ~ 0
VCC
$Comp
L CONN_3X2 P2
U 1 1 502DA4D5
P 6750 1150
F 0 "P2" H 6750 1400 50  0000 C CNN
F 1 "ISP_2" V 6750 1200 40  0000 C CNN
	1    6750 1150
	1    0    0    -1  
$EndComp
$Comp
L CSMALL C40
U 1 1 502DA363
P 1300 1350
F 0 "C40" H 1325 1400 30  0000 L CNN
F 1 "10uF" H 1325 1300 30  0000 L CNN
	1    1300 1350
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR38
U 1 1 502DA2D1
P 1650 3700
F 0 "#PWR38" H 1650 3700 30  0001 C CNN
F 1 "GND" H 1650 3630 30  0001 C CNN
	1    1650 3700
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR37
U 1 1 502DA282
P 1650 2250
F 0 "#PWR37" H 1650 2250 30  0001 C CNN
F 1 "GND" H 1650 2180 30  0001 C CNN
	1    1650 2250
	1    0    0    -1  
$EndComp
$Comp
L CSMALL C42
U 1 1 502DA27F
P 1650 2100
F 0 "C42" H 1675 2150 30  0000 L CNN
F 1 ".1uF" H 1675 2050 30  0000 L CNN
	1    1650 2100
	1    0    0    -1  
$EndComp
Text Label 1050 950  0    60   ~ 0
VCC
$Comp
L GND #PWR35
U 1 1 502DA1C5
P 1050 1950
F 0 "#PWR35" H 1050 1950 30  0001 C CNN
F 1 "GND" H 1050 1880 30  0001 C CNN
	1    1050 1950
	1    0    0    -1  
$EndComp
$Comp
L CSMALL C39
U 1 1 502DA1C2
P 1050 1800
F 0 "C39" H 1075 1850 30  0000 L CNN
F 1 ".1uF" H 1075 1750 30  0000 L CNN
	1    1050 1800
	1    0    0    -1  
$EndComp
Text Label 1500 1150 0    60   ~ 0
VCC
$Comp
L GND #PWR36
U 1 1 502DA18A
P 1500 1550
F 0 "#PWR36" H 1500 1550 30  0001 C CNN
F 1 "GND" H 1500 1480 30  0001 C CNN
	1    1500 1550
	1    0    0    -1  
$EndComp
$Comp
L CSMALL C41
U 1 1 502DA16F
P 1500 1350
F 0 "C41" H 1525 1400 30  0000 L CNN
F 1 ".1uF" H 1525 1300 30  0000 L CNN
	1    1500 1350
	1    0    0    -1  
$EndComp
$Comp
L INDUCTOR L2
U 1 1 502DA13B
P 1050 1300
F 0 "L2" V 1000 1300 40  0000 C CNN
F 1 "33uH" V 1150 1300 40  0000 C CNN
	1    1050 1300
	1    0    0    -1  
$EndComp
Text Label 3650 3250 0    60   ~ 0
X3_2
Text Label 3650 3150 0    60   ~ 0
X2_2
Text Label 3650 3050 0    60   ~ 0
X1_2
Text Label 3650 2950 0    60   ~ 0
X0_2
Text Label 3650 2500 0    60   ~ 0
YB1_2
Text Label 3650 2400 0    60   ~ 0
YB0_2
Text Label 3650 2300 0    60   ~ 0
YA1_2
Text Label 3650 2200 0    60   ~ 0
YA0_2
Text Label 3650 2600 0    60   ~ 0
SMP_2
Text Label 3650 2800 0    60   ~ 0
RESET
Text Label 3650 3550 0    60   ~ 0
AIN0_2
Text Label 3650 2050 0    60   ~ 0
XTAL2_2
Text Label 3650 1950 0    60   ~ 0
XTAL1_2
Text Label 3650 1850 0    60   ~ 0
SCK_2
Text Label 3650 1750 0    60   ~ 0
MISO_2
Text Label 3650 1650 0    60   ~ 0
MOSI_2
Text Label 3650 1550 0    60   ~ 0
~SS_2
$Comp
L ATMEGA168PA-A IC2
U 1 1 4FF471A9
P 2600 2450
F 0 "IC2" H 1900 3700 50  0000 L BNN
F 1 "ATMEGA168PA-A" H 2800 1050 50  0000 L BNN
F 2 "TQFP32" H 2050 1100 50  0001 C CNN
	1    2600 2450
	1    0    0    -1  
$EndComp
$EndSCHEMATC
