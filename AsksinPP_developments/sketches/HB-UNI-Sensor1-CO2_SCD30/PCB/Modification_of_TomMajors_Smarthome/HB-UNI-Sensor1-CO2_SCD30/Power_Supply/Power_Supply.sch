EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date "2020-08-16"
Rev ""
Comp "FUEL4EP"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L HB-UNI-Sensor1-CO2_SCD30:Step_Down_LM2596 Module2
U 1 1 5F38115A
P 4650 4700
AR Path="/5F38115A" Ref="Module2"  Part="1" 
AR Path="/5F37F0F6/5F38115A" Ref="Module2"  Part="1" 
F 0 "Module2" H 4650 5065 50  0000 C CNN
F 1 "Step_Down_LM2596" H 4650 4974 50  0000 C CNN
F 2 "" H 4650 5150 50  0001 C CNN
F 3 "https://www.ti.com/lit/ds/symlink/lm2596.pdf" H 4650 5150 50  0001 C CNN
	1    4650 4700
	1    0    0    -1  
$EndComp
$Comp
L HB-UNI-Sensor1-CO2_SCD30:Step_Up_HW_668_SX1308 Module3
U 1 1 5F3814CC
P 4650 6350
F 0 "Module3" H 4650 6715 50  0000 C CNN
F 1 "Step_Up_HW_668_SX1308" H 4650 6624 50  0000 C CNN
F 2 "" H 4650 6850 50  0001 C CNN
F 3 "https://datasheetspdf.com/pdf-file/921054/Suosemi/SX1308/1" H 4650 6850 50  0001 C CNN
	1    4650 6350
	1    0    0    -1  
$EndComp
$Comp
L Diode:1N5819 D1
U 1 1 5F3827A1
P 5700 4600
F 0 "D1" H 5700 4817 50  0000 C CNN
F 1 "1N5819" H 5700 4726 50  0000 C CNN
F 2 "Diode_THT:D_DO-41_SOD81_P10.16mm_Horizontal" H 5700 4425 50  0001 C CNN
F 3 "http://www.vishay.com/docs/88525/1n5817.pdf" H 5700 4600 50  0001 C CNN
	1    5700 4600
	-1   0    0    -1  
$EndComp
$Comp
L Switch:SW_Push_DPDT SW1
U 1 1 5F3816A2
P 3200 3450
F 0 "SW1" H 3200 3935 50  0000 C CNN
F 1 "SW_Push_DPDT" H 3200 3844 50  0000 C CNN
F 2 "" H 3200 3650 50  0001 C CNN
F 3 "~" H 3200 3650 50  0001 C CNN
	1    3200 3450
	1    0    0    -1  
$EndComp
$Comp
L Device:LED D2
U 1 1 5F38212B
P 6600 3650
F 0 "D2" H 6400 3450 50  0000 C CNN
F 1 "LED  red" H 6700 3450 50  0000 C CNN
F 2 "" H 6600 3650 50  0001 C CNN
F 3 "~" H 6600 3650 50  0001 C CNN
	1    6600 3650
	0    -1   1    0   
$EndComp
$Comp
L Connector:Conn_Coaxial_Power J3
U 1 1 5F381ADB
P 1900 4600
F 0 "J3" H 1988 4596 50  0000 L CNN
F 1 "6.0V external power plug" H 1988 4505 50  0000 L CNN
F 2 "" H 1900 4550 50  0001 C CNN
F 3 "~" H 1900 4550 50  0001 C CNN
	1    1900 4600
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R1
U 1 1 5F394B2D
P 6600 3200
F 0 "R1" H 6670 3246 50  0000 L CNN
F 1 "270" H 6670 3155 50  0000 L CNN
F 2 "" V 6530 3200 50  0001 C CNN
F 3 "~" H 6600 3200 50  0001 C CNN
	1    6600 3200
	1    0    0    -1  
$EndComp
Text HLabel 1800 1300 0    50   Input ~ 0
VBatt_CONN
Text HLabel 1800 1550 0    50   Input ~ 0
VCC_CONN
Text HLabel 1800 1800 0    50   Input ~ 0
GND_CONN
$Comp
L HB-UNI-Sensor1-CO2_SCD30:NiMH_Charger_NIMHCRTA_2S Module1
U 1 1 5F393DEF
P 4600 3250
AR Path="/5F393DEF" Ref="Module1"  Part="1" 
AR Path="/5F37F0F6/5F393DEF" Ref="Module1"  Part="1" 
F 0 "Module1" H 4600 3615 50  0000 C CNN
F 1 "NiMH_Charger_NIMHCRTA_2S" H 4600 3524 50  0000 C CNN
F 2 "" H 4600 3250 50  0001 C CNN
F 3 "https://datasheetspdf.com/pdf-file/1257631/CONSONANCE/CN3085/1" H 4600 3250 50  0001 C CNN
	1    4600 3250
	1    0    0    -1  
$EndComp
Wire Wire Line
	3700 3350 3700 3150
Wire Wire Line
	2700 1800 2700 4800
Wire Wire Line
	4050 4800 2700 4800
Wire Wire Line
	1800 1800 2700 1800
Wire Wire Line
	6600 3350 6600 3450
Wire Wire Line
	4600 3650 4600 3750
Wire Wire Line
	4600 3750 6400 3750
Wire Wire Line
	6400 3750 6400 3450
Wire Wire Line
	6400 3450 6600 3450
Connection ~ 6600 3450
Wire Wire Line
	6600 3450 6600 3500
Wire Wire Line
	4500 3650 4500 3850
Wire Wire Line
	4500 3850 6600 3850
Wire Wire Line
	6600 3850 6600 3800
Wire Wire Line
	6600 3050 6600 2950
Wire Wire Line
	6600 2950 6300 2950
Wire Wire Line
	4700 3650 6300 3650
Wire Wire Line
	6300 2950 6300 3650
Wire Wire Line
	2700 4800 2700 5150
Wire Wire Line
	5250 5150 5250 4800
Connection ~ 2700 4800
$Comp
L power:GND #PWR01
U 1 1 5F587D8F
P 2700 5350
F 0 "#PWR01" H 2700 5100 50  0001 C CNN
F 1 "GND" H 2705 5177 50  0000 C CNN
F 2 "" H 2700 5350 50  0001 C CNN
F 3 "" H 2700 5350 50  0001 C CNN
	1    2700 5350
	1    0    0    -1  
$EndComp
Wire Wire Line
	2700 5350 2700 5150
Connection ~ 2700 5150
Wire Wire Line
	4050 6250 3100 6250
Wire Wire Line
	3100 6100 3100 6250
Wire Wire Line
	4050 6450 3550 6450
Wire Wire Line
	3100 6450 3100 6600
Wire Wire Line
	3100 6600 1950 6600
Wire Wire Line
	1950 6600 1950 6550
$Comp
L Device:Solar_Cells SC1
U 1 1 5F3AED30
P 1950 6350
F 0 "SC1" H 2058 6396 50  0000 L CNN
F 1 "Solar_Cells" H 2058 6305 50  0000 L CNN
F 2 "" V 1950 6410 50  0001 C CNN
F 3 "~" V 1950 6410 50  0001 C CNN
	1    1950 6350
	1    0    0    -1  
$EndComp
Wire Wire Line
	3100 6100 2250 6100
Wire Wire Line
	1950 6100 1950 6150
Wire Wire Line
	3550 5150 3550 6450
Connection ~ 3550 5150
Wire Wire Line
	3550 5150 5250 5150
Connection ~ 3550 6450
Wire Wire Line
	3550 6450 3100 6450
Wire Wire Line
	3550 6450 3550 6750
Wire Wire Line
	3550 6750 5600 6750
Wire Wire Line
	5600 6750 5600 6450
Wire Wire Line
	5600 6450 5250 6450
Wire Wire Line
	5850 4600 6300 4600
Wire Wire Line
	6300 4600 6300 6250
Wire Wire Line
	5350 1300 5350 3150
Wire Wire Line
	5350 3150 5200 3150
Wire Wire Line
	1800 1300 5350 1300
Wire Wire Line
	4000 3350 3900 3350
Wire Wire Line
	3900 3350 3900 1800
Wire Wire Line
	3900 1800 2700 1800
Connection ~ 2700 1800
Wire Wire Line
	6300 4600 6300 4250
Wire Wire Line
	6300 4050 2850 4050
Wire Wire Line
	2850 4050 2850 3650
Connection ~ 6300 4600
Wire Wire Line
	2850 3650 3000 3650
Wire Wire Line
	3600 3550 3600 1550
Text Label 6550 4600 0    50   ~ 0
VCC_top_case
Wire Wire Line
	6550 4600 6300 4600
Text Notes 2900 4000 0    39   ~ 0
VCC_top_case is disconnected from VCC_CONN by SW1 during charging of the NiMH accumulators\nwith Module1 NiMH_Charger_NIMHCRTA 2S in order to avoid cirular currents
Text Notes 1400 6800 0    50   ~ 0
Solar cell panel 70 mm x 70 mm 3V 100 mA
Text Notes 6350 4900 0    39   ~ 0
adjust VCC_top_case in open loop (detach J12 from J1) to\n\na) 3.9V at potentiometer of Module3 Step_UP_HW_66_SX1308 for direct intensive sunshine to the solar panel SC1\nb) 3.8V  at potentiomenter of Module2 Step_Down_LM2596 when 6.0V are supplied at J2 6.0V external power plug
Text Notes 1350 3250 0    50   ~ 0
push in SW1 for charging\nthe NiMH accumulators\nensure that 6.0 V are supplied via\nthe J2 6.0V external power plug\nrelease SW1 by pushing again\nwhen charging is finished
Text Notes 7000 3700 0    50   ~ 0
LED D2 is flashing during charging\nof the NiMH accumulators with about 1Hz
Text Notes 7000 3200 0    50   ~ 0
solder R1 in parallel to resistor on Module1 NiMH_Charger_NIMHCRTA_2S\n
Text Notes 4100 2650 0    50   ~ 0
desolder red LED on Module1\nNiMH_Charger_NIMHCRTA_2S\nattach external LED D1 instead
$Comp
L power:PWR_FLAG #FLG03
U 1 1 5F42ED1E
P 3050 4450
F 0 "#FLG03" H 3050 4525 50  0001 C CNN
F 1 "PWR_FLAG" H 3050 4623 50  0000 C CNN
F 2 "" H 3050 4450 50  0001 C CNN
F 3 "~" H 3050 4450 50  0001 C CNN
	1    3050 4450
	1    0    0    -1  
$EndComp
Wire Wire Line
	3050 4450 3050 4600
Connection ~ 3050 4600
Wire Wire Line
	3050 4600 2600 4600
$Comp
L power:+6V #PWR02
U 1 1 5F430916
P 3350 4450
F 0 "#PWR02" H 3350 4300 50  0001 C CNN
F 1 "+6V" H 3365 4623 50  0000 C CNN
F 2 "" H 3350 4450 50  0001 C CNN
F 3 "" H 3350 4450 50  0001 C CNN
	1    3350 4450
	1    0    0    -1  
$EndComp
Wire Wire Line
	3350 4450 3350 4600
Wire Wire Line
	3050 4600 3350 4600
$Comp
L power:PWR_FLAG #FLG02
U 1 1 5F432B23
P 2250 6000
F 0 "#FLG02" H 2250 6075 50  0001 C CNN
F 1 "PWR_FLAG" H 2250 6173 50  0000 C CNN
F 2 "" H 2250 6000 50  0001 C CNN
F 3 "~" H 2250 6000 50  0001 C CNN
	1    2250 6000
	1    0    0    -1  
$EndComp
Wire Wire Line
	2250 6000 2250 6100
Connection ~ 2250 6100
Wire Wire Line
	2250 6100 1950 6100
Wire Wire Line
	3900 3350 3900 3600
Connection ~ 3900 3350
NoConn ~ 3400 3750
NoConn ~ 3400 3150
Wire Wire Line
	3600 3550 3400 3550
$Comp
L power:PWR_FLAG #FLG01
U 1 1 5F4676D1
P 1600 4350
F 0 "#FLG01" H 1600 4425 50  0001 C CNN
F 1 "PWR_FLAG" H 1600 4523 50  0000 C CNN
F 2 "" H 1600 4350 50  0001 C CNN
F 3 "~" H 1600 4350 50  0001 C CNN
	1    1600 4350
	1    0    0    -1  
$EndComp
Wire Wire Line
	1800 4600 1600 4600
Wire Wire Line
	1600 4600 1600 4350
$Comp
L power:PWR_FLAG #FLG06
U 1 1 5F46DDFF
P 5350 4500
F 0 "#FLG06" H 5350 4575 50  0001 C CNN
F 1 "PWR_FLAG" H 5350 4673 50  0000 C CNN
F 2 "" H 5350 4500 50  0001 C CNN
F 3 "~" H 5350 4500 50  0001 C CNN
	1    5350 4500
	1    0    0    -1  
$EndComp
$Comp
L power:PWR_FLAG #FLG07
U 1 1 5F46E376
P 6600 4250
F 0 "#FLG07" H 6600 4325 50  0001 C CNN
F 1 "PWR_FLAG" V 6600 4378 50  0000 L CNN
F 2 "" H 6600 4250 50  0001 C CNN
F 3 "~" H 6600 4250 50  0001 C CNN
	1    6600 4250
	0    1    1    0   
$EndComp
Wire Wire Line
	6600 4250 6300 4250
Connection ~ 6300 4250
Wire Wire Line
	6300 4250 6300 4050
Wire Wire Line
	5350 4500 5350 4600
Wire Wire Line
	5250 4600 5350 4600
Connection ~ 5350 4600
Wire Wire Line
	5350 4600 5550 4600
Wire Wire Line
	3700 3350 3400 3350
Wire Wire Line
	4000 3150 3700 3150
Wire Wire Line
	5250 6250 6300 6250
NoConn ~ 3250 3350
$Comp
L power:PWR_FLAG #FLG05
U 1 1 5F6CC239
P 3700 3000
F 0 "#FLG05" H 3700 3075 50  0001 C CNN
F 1 "PWR_FLAG" V 3700 3300 50  0000 C CNN
F 2 "" H 3700 3000 50  0001 C CNN
F 3 "~" H 3700 3000 50  0001 C CNN
	1    3700 3000
	1    0    0    -1  
$EndComp
Wire Wire Line
	3700 3000 3700 3150
Connection ~ 3700 3150
Wire Wire Line
	5350 3600 5350 3350
Wire Wire Line
	3900 3600 5350 3600
Wire Wire Line
	5350 3350 5200 3350
Wire Wire Line
	2700 5150 3550 5150
Wire Wire Line
	3350 4600 4050 4600
Connection ~ 3350 4600
Wire Wire Line
	1800 1550 3600 1550
Wire Wire Line
	2600 3250 2600 4600
Wire Wire Line
	2600 3250 3000 3250
Connection ~ 2600 4600
Wire Wire Line
	2600 4600 2100 4600
$EndSCHEMATC
