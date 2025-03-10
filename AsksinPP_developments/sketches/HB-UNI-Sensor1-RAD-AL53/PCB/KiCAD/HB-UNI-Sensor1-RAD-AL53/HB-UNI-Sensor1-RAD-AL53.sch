EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 2
Title "HB-UNI-SENSOR1-RAD-AL53"
Date "2021-09-18"
Rev "1.0"
Comp "glued carrier modules"
Comment1 "Mount PCB:  HB-UNI-SEN-BATT (modified)"
Comment2 "Creative Commons License"
Comment3 " (C) 2021 FUEL4EP"
Comment4 ""
$EndDescr
$Comp
L FUEL4EP:Tindie_Pro_Mini_XL_V2 Module5
U 1 1 6145CA3D
P 8750 3700
F 0 "Module5" H 8950 3400 50  0000 C CNN
F 1 "Tindie_Pro_Mini_XL_V2" V 9200 4700 50  0000 C CNB
F 2 "" H 8400 3900 50  0001 C CNN
F 3 "https://www.tindie.com/products/prominimicros/pro-mini-xl-v2-atmega-1284p/" H 8400 3900 50  0001 C CNN
	1    8750 3700
	1    0    0    -1  
$EndComp
$Sheet
S 4450 6500 2100 750 
U 6145C83E
F0 "ABLIC_S35770_DIP_PCB" 50
F1 "ABLIC_S35770_DIP_PCB.sch" 50
F2 "1C_RSTB_DIP_PCB" I L 4450 6650 50 
F3 "2C_NC_DIP_PCB" U L 4450 6800 50 
F4 "3C_CNTIN_DIP_PCB" I L 4450 6950 50 
F5 "4C_GND_DIP_PCB" I L 4450 7100 50 
F6 "1A_VDD_DIP_PCB" I R 6550 6650 50 
F7 "2A_SCL_DIP_PCB" I R 6550 6800 50 
F8 "3A_SDA_DIP_PCB" B R 6550 6950 50 
F9 "4A_LOOP_DIP_PCB" O R 6550 7100 50 
$EndSheet
$Comp
L FUEL4EP:ADS1115_ADC_module Module1
U 1 1 61475D66
P 750 4550
F 0 "Module1" H 900 4500 50  0000 C CNN
F 1 "ADS1115_ADC_module" H 1133 5824 50  0000 C CNB
F 2 "" H 900 4450 50  0001 C CNN
F 3 "https://www.ti.com/lit/ds/symlink/ads1115.pdf" H 900 4450 50  0001 C CNN
	1    750  4550
	1    0    0    -1  
$EndComp
$Comp
L FUEL4EP:MT3608_step_up Module3
U 1 1 614768FB
P 2000 5050
F 0 "Module3" H 1150 4700 50  0000 C CNN
F 1 "MT3608_step_up" H 1750 4900 50  0000 C CNB
F 2 "" H 1150 4650 50  0001 C CNN
F 3 "https://www.olimex.com/Products/Breadboarding/BB-PWR-3608/resources/MT3608.pdf" H 1150 4650 50  0001 C CNN
	1    2000 5050
	1    0    0    -1  
$EndComp
$Comp
L FUEL4EP:TPS61221_step_up_3.3V Module4
U 1 1 614771A3
P 4350 1750
F 0 "Module4" H 4500 1700 50  0000 C CNN
F 1 "TPS61221_step_up_3.3V" H 5100 2200 50  0000 C CNB
F 2 "" H 4500 2600 50  0001 C CNN
F 3 "https://www.ti.com/lit/ds/symlink/tps61221.pdf" H 4500 2600 50  0001 C CNN
	1    4350 1750
	1    0    0    -1  
$EndComp
$Comp
L FUEL4EP:Teviso_AL53_RAD_sensor Module2
U 1 1 61477CB0
P 1000 7250
F 0 "Module2" H 1150 7200 50  0000 C CNN
F 1 "Teviso_AL53_RAD_sensor" H 1500 7400 50  0000 C CNB
F 2 "" H 1050 7200 50  0001 C CNN
F 3 "https://www.teviso.com/file/pdf/al53-data-specification.pdf" H 1050 7200 50  0001 C CNN
	1    1000 7250
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x02 J4
U 1 1 6145DA22
P 6000 3300
F 0 "J4" H 6080 3292 50  0000 L CNN
F 1 "Conn_01x02" H 6080 3201 50  0000 L CNN
F 2 "" H 6000 3300 50  0001 C CNN
F 3 "~" H 6000 3300 50  0001 C CNN
	1    6000 3300
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x02 J5
U 1 1 614617F5
P 6000 3700
F 0 "J5" H 6080 3692 50  0000 L CNN
F 1 "Conn_01x02" H 6080 3601 50  0000 L CNN
F 2 "" H 6000 3700 50  0001 C CNN
F 3 "~" H 6000 3700 50  0001 C CNN
	1    6000 3700
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x02 J6
U 1 1 61461C15
P 6000 4050
F 0 "J6" H 6080 4042 50  0000 L CNN
F 1 "Conn_01x02" H 6080 3951 50  0000 L CNN
F 2 "" H 6000 4050 50  0001 C CNN
F 3 "~" H 6000 4050 50  0001 C CNN
	1    6000 4050
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x02 J7
U 1 1 61461FA6
P 6000 4450
F 0 "J7" H 6080 4442 50  0000 L CNN
F 1 "Conn_01x02" H 6080 4351 50  0000 L CNN
F 2 "" H 6000 4450 50  0001 C CNN
F 3 "~" H 6000 4450 50  0001 C CNN
	1    6000 4450
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x04 J2
U 1 1 61462AF2
P 5000 5550
F 0 "J2" H 4950 5200 50  0000 L CNN
F 1 "Conn_01x04" H 4750 5800 50  0000 L CNN
F 2 "" H 5000 5550 50  0001 C CNN
F 3 "~" H 5000 5550 50  0001 C CNN
	1    5000 5550
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x04 J3
U 1 1 614645E5
P 5500 5550
F 0 "J3" H 5450 5200 50  0000 L CNN
F 1 "Conn_01x04" H 5250 5800 50  0000 L CNN
F 2 "" H 5500 5550 50  0001 C CNN
F 3 "~" H 5500 5550 50  0001 C CNN
	1    5500 5550
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x04 J8
U 1 1 61464C46
P 6050 5550
F 0 "J8" H 5950 5200 50  0000 L CNN
F 1 "Conn_01x04" H 5800 5800 50  0000 L CNN
F 2 "" H 6050 5550 50  0001 C CNN
F 3 "~" H 6050 5550 50  0001 C CNN
	1    6050 5550
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x04 J9
U 1 1 61465283
P 6550 5550
F 0 "J9" H 6500 5200 50  0000 L CNN
F 1 "Conn_01x04" H 6300 5800 50  0000 L CNN
F 2 "" H 6550 5550 50  0001 C CNN
F 3 "~" H 6550 5550 50  0001 C CNN
	1    6550 5550
	1    0    0    -1  
$EndComp
$Comp
L Device:LED D3
U 1 1 61466EC2
P 7650 2700
F 0 "D3" H 7450 2800 50  0000 C CNN
F 1 "LED" H 7643 2826 50  0000 C CNN
F 2 "" H 7650 2700 50  0001 C CNN
F 3 "~" H 7650 2700 50  0001 C CNN
	1    7650 2700
	1    0    0    -1  
$EndComp
$Comp
L Device:Battery_Cell BT1
U 1 1 61467B7F
P 850 1750
F 0 "BT1" H 968 1846 50  0000 L CNN
F 1 "2700 mAh NiMH Accumulator cell" H 968 1755 50  0000 L CNN
F 2 "" V 850 1810 50  0001 C CNN
F 3 "~" V 850 1810 50  0001 C CNN
	1    850  1750
	1    0    0    -1  
$EndComp
$Comp
L Device:Battery_Cell BT2
U 1 1 614702F4
P 850 2050
F 0 "BT2" H 968 2146 50  0000 L CNN
F 1 "2700 mAh NiMH Accumulator cell" H 968 2055 50  0000 L CNN
F 2 "" V 850 2110 50  0001 C CNN
F 3 "~" V 850 2110 50  0001 C CNN
	1    850  2050
	1    0    0    -1  
$EndComp
$Comp
L Device:R R1
U 1 1 61484341
P 8100 2700
F 0 "R1" V 7893 2700 50  0000 C CNN
F 1 "470" V 7984 2700 50  0000 C CNN
F 2 "" V 8030 2700 50  0001 C CNN
F 3 "~" H 8100 2700 50  0001 C CNN
	1    8100 2700
	0    1    1    0   
$EndComp
Wire Wire Line
	7950 2700 7800 2700
$Comp
L power:GND #PWR0101
U 1 1 6148E6D3
P 7050 2800
F 0 "#PWR0101" H 7050 2550 50  0001 C CNN
F 1 "GND" H 7055 2627 50  0000 C CNN
F 2 "" H 7050 2800 50  0001 C CNN
F 3 "" H 7050 2800 50  0001 C CNN
	1    7050 2800
	1    0    0    -1  
$EndComp
Wire Wire Line
	7500 2700 7400 2700
Wire Wire Line
	7050 2700 7050 2800
Text Notes 7600 2850 0    50   ~ 0
red
$Comp
L Switch:SW_DIP_x01 SW1
U 1 1 61490989
P 7700 3100
F 0 "SW1" H 7800 2900 50  0000 C CNN
F 1 "Config Button" H 7700 3276 50  0000 C CNN
F 2 "" H 7700 3100 50  0001 C CNN
F 3 "~" H 7700 3100 50  0001 C CNN
	1    7700 3100
	-1   0    0    -1  
$EndComp
Wire Wire Line
	7400 3100 7400 2700
Connection ~ 7400 2700
Wire Wire Line
	7400 2700 7050 2700
Wire Wire Line
	8350 2500 8550 2500
Wire Wire Line
	8550 2700 8250 2700
Wire Wire Line
	8550 3100 8000 3100
Text Label 8000 3100 0    50   ~ 0
CONFIG
Text Label 8400 2700 0    50   ~ 0
LED
Wire Wire Line
	4800 5450 5300 5450
Wire Wire Line
	5850 5450 6350 5450
Wire Wire Line
	5850 5450 5300 5450
Connection ~ 5850 5450
Connection ~ 5300 5450
Wire Wire Line
	4800 5550 5300 5550
Wire Wire Line
	5300 5550 5850 5550
Connection ~ 5300 5550
Wire Wire Line
	5850 5550 6350 5550
Connection ~ 5850 5550
Wire Wire Line
	4800 5650 5300 5650
Wire Wire Line
	5300 5650 5850 5650
Connection ~ 5300 5650
Wire Wire Line
	5850 5650 6200 5650
Connection ~ 5850 5650
Wire Wire Line
	4800 5750 5300 5750
Wire Wire Line
	5300 5750 5850 5750
Connection ~ 5300 5750
Wire Wire Line
	5850 5750 6350 5750
Connection ~ 5850 5750
$Comp
L Connector:Conn_01x02_Male J1
U 1 1 61462B9A
P 700 1200
F 0 "J1" H 700 1050 50  0000 C CNN
F 1 "Conn_01x02_Male" H 850 1350 50  0000 C CNN
F 2 "" H 700 1200 50  0001 C CNN
F 3 "~" H 700 1200 50  0001 C CNN
	1    700  1200
	1    0    0    -1  
$EndComp
Wire Wire Line
	850  2150 850  2400
Wire Wire Line
	850  2400 700  2400
Wire Wire Line
	700  2400 700  1450
Wire Wire Line
	700  1450 1200 1450
Wire Wire Line
	1200 1450 1200 1300
Wire Wire Line
	1200 1300 900  1300
Wire Wire Line
	900  1200 1650 1200
Wire Wire Line
	1650 1550 850  1550
$Comp
L Device:R R4
U 1 1 6146137D
P 7200 5650
F 0 "R4" V 7100 5650 50  0000 C CNN
F 1 "10k" V 7200 5650 50  0000 C CNN
F 2 "" V 7130 5650 50  0001 C CNN
F 3 "~" H 7200 5650 50  0001 C CNN
	1    7200 5650
	0    1    1    0   
$EndComp
$Comp
L Device:R R5
U 1 1 61461AD8
P 7200 5800
F 0 "R5" V 7300 5800 50  0000 C CNN
F 1 "10k" V 7200 5800 50  0000 C CNN
F 2 "" V 7130 5800 50  0001 C CNN
F 3 "~" H 7200 5800 50  0001 C CNN
	1    7200 5800
	0    1    1    0   
$EndComp
Wire Wire Line
	6350 5750 6800 5750
Wire Wire Line
	6800 5750 6800 5800
Wire Wire Line
	6800 5800 7050 5800
Connection ~ 6350 5750
Wire Wire Line
	6350 5650 7050 5650
Connection ~ 6350 5650
Wire Wire Line
	7350 5800 7550 5800
Wire Wire Line
	7550 5800 7550 5650
Wire Wire Line
	7550 5450 6350 5450
Connection ~ 6350 5450
Wire Wire Line
	7350 5650 7550 5650
Connection ~ 7550 5650
Wire Wire Line
	7550 5650 7550 5450
Text Notes 6850 6050 0    50   ~ 0
I2C pullup resistors
Wire Wire Line
	4800 5650 4300 5650
Connection ~ 4800 5650
Connection ~ 4800 5750
Text Label 4550 5750 0    50   ~ 0
SDA
Text Label 4550 5650 0    50   ~ 0
SCL
$Comp
L power:GND #PWR0102
U 1 1 6148658B
P 3800 6200
F 0 "#PWR0102" H 3800 5950 50  0001 C CNN
F 1 "GND" H 3805 6027 50  0000 C CNN
F 2 "" H 3800 6200 50  0001 C CNN
F 3 "" H 3800 6200 50  0001 C CNN
	1    3800 6200
	1    0    0    -1  
$EndComp
$Comp
L Device:Q_NMOS_GDS Q1
U 1 1 614A50F0
P 3400 1850
F 0 "Q1" V 3650 1900 50  0000 L CNN
F 1 "IRLU024N" V 3300 1900 50  0000 L CNN
F 2 "" H 3600 1950 50  0001 C CNN
F 3 "~" H 3400 1850 50  0001 C CNN
	1    3400 1850
	0    -1   1    0   
$EndComp
$Comp
L Diode:BZX384-xxx D1
U 1 1 614A7F20
P 3000 7400
F 0 "D1" V 2954 7480 50  0000 L CNN
F 1 "BZX384-3V3" V 3045 7480 50  0000 L CNN
F 2 "Diode_SMD:D_SOD-323" H 3000 7225 50  0001 C CNN
F 3 "https://assets.nexperia.com/documents/data-sheet/BZX384_SERIES.pdf" H 3000 7400 50  0001 C CNN
	1    3000 7400
	0    1    1    0   
$EndComp
$Comp
L Diode:1N4148 D2
U 1 1 614AA9AE
P 3300 7200
F 0 "D2" H 3300 7417 50  0000 C CNN
F 1 "1N4148" H 3300 7326 50  0000 C CNN
F 2 "Diode_THT:D_DO-35_SOD27_P7.62mm_Horizontal" H 3300 7025 50  0001 C CNN
F 3 "https://assets.nexperia.com/documents/data-sheet/1N4148_1N4448.pdf" H 3300 7200 50  0001 C CNN
	1    3300 7200
	-1   0    0    -1  
$EndComp
$Comp
L Device:R R15
U 1 1 614BCC72
P 1100 5750
F 0 "R15" V 893 5750 50  0000 C CNN
F 1 "1.8k" V 984 5750 50  0000 C CNN
F 2 "" V 1030 5750 50  0001 C CNN
F 3 "~" H 1100 5750 50  0001 C CNN
	1    1100 5750
	0    1    1    0   
$EndComp
$Comp
L Device:R R16
U 1 1 614BD5BF
P 1500 5750
F 0 "R16" V 1293 5750 50  0000 C CNN
F 1 "1.8k" V 1384 5750 50  0000 C CNN
F 2 "" V 1430 5750 50  0001 C CNN
F 3 "~" H 1500 5750 50  0001 C CNN
	1    1500 5750
	0    1    1    0   
$EndComp
$Comp
L Device:C C2
U 1 1 614BE0F8
P 1300 6000
F 0 "C2" H 1415 6046 50  0000 L CNN
F 1 "4.7u" H 1415 5955 50  0000 L CNN
F 2 "" H 1338 5850 50  0001 C CNN
F 3 "~" H 1300 6000 50  0001 C CNN
	1    1300 6000
	1    0    0    -1  
$EndComp
$Comp
L Device:C C3
U 1 1 614BED9C
P 1900 6000
F 0 "C3" H 2015 6046 50  0000 L CNN
F 1 "4.7u" H 2015 5955 50  0000 L CNN
F 2 "" H 1938 5850 50  0001 C CNN
F 3 "~" H 1900 6000 50  0001 C CNN
	1    1900 6000
	1    0    0    -1  
$EndComp
$Comp
L Device:CP C4
U 1 1 614BFBA9
P 2350 6000
F 0 "C4" H 2468 6046 50  0000 L CNN
F 1 "22u" H 2468 5955 50  0000 L CNN
F 2 "" H 2388 5850 50  0001 C CNN
F 3 "~" H 2350 6000 50  0001 C CNN
	1    2350 6000
	1    0    0    -1  
$EndComp
$Comp
L Device:R R13
U 1 1 614D0BE3
P 4000 7400
F 0 "R13" H 4070 7446 50  0000 L CNN
F 1 "220k" H 4070 7355 50  0000 L CNN
F 2 "" V 3930 7400 50  0001 C CNN
F 3 "~" H 4000 7400 50  0001 C CNN
	1    4000 7400
	1    0    0    -1  
$EndComp
Wire Wire Line
	2950 7200 3000 7200
Wire Wire Line
	3000 7250 3000 7200
Connection ~ 3000 7200
Wire Wire Line
	3000 7200 3150 7200
Wire Wire Line
	3450 7200 3600 7200
Wire Wire Line
	4000 7200 4000 6950
Wire Wire Line
	4000 6950 4450 6950
Wire Wire Line
	4000 7250 4000 7200
Connection ~ 4000 7200
Wire Wire Line
	4450 7100 4300 7100
Wire Wire Line
	4300 7100 4300 7550
Wire Wire Line
	4300 7550 4000 7550
Wire Wire Line
	3000 7550 2550 7550
Wire Wire Line
	2550 7550 2550 7000
Wire Wire Line
	2550 7000 2450 7000
Wire Wire Line
	3000 7550 4000 7550
Connection ~ 3000 7550
Connection ~ 4000 7550
$Comp
L Device:R R19
U 1 1 614C0CD4
P 2800 7200
F 0 "R19" V 2593 7200 50  0000 C CNN
F 1 "270k" V 2684 7200 50  0000 C CNN
F 2 "" V 2730 7200 50  0001 C CNN
F 3 "~" H 2800 7200 50  0001 C CNN
	1    2800 7200
	0    1    1    0   
$EndComp
Wire Wire Line
	1250 5750 1300 5750
Wire Wire Line
	1300 5850 1300 5750
Connection ~ 1300 5750
Wire Wire Line
	1300 5750 1350 5750
Wire Wire Line
	2550 6150 2350 6150
Connection ~ 2550 7000
Wire Wire Line
	2350 6150 1900 6150
Connection ~ 2350 6150
Wire Wire Line
	1900 6150 1300 6150
Connection ~ 1900 6150
Wire Wire Line
	800  5250 800  6150
Wire Wire Line
	800  6150 1300 6150
Connection ~ 1300 6150
Wire Wire Line
	950  5750 700  5750
Wire Wire Line
	700  5750 700  5150
Wire Wire Line
	700  5150 800  5150
Wire Wire Line
	1650 5750 1900 5750
Wire Wire Line
	1900 5750 1900 5850
Wire Wire Line
	1900 5750 2350 5750
Wire Wire Line
	2350 5750 2350 5850
Connection ~ 1900 5750
Wire Wire Line
	2350 5750 2650 5750
Wire Wire Line
	2650 5750 2650 6500
Wire Wire Line
	2650 7100 2450 7100
Connection ~ 2350 5750
Wire Wire Line
	2650 7200 2450 7200
Wire Notes Line
	2800 5600 2800 6300
Wire Notes Line
	2800 6300 1700 6300
Wire Notes Line
	1700 6300 1700 5600
Wire Notes Line
	1700 5600 2800 5600
Text Notes 1700 6400 0    50   ~ 0
place close to the
Text Notes 1700 6500 0    50   ~ 0
AL53 power pins
Text Notes 4000 6950 0    50   ~ 0
max. 3.3V
Text Notes 800  4900 0    50   ~ 0
IMPORTANT: adjust VOUT+ to\n8.0V before connecting \nthe AL53 sensor!
Text Notes 550  700  0    50   ~ 0
connector to charging module\n=> drill two extra holes into PCB for a plug connector
Text Label 4000 7100 0    50   ~ 0
CNT
Wire Wire Line
	6200 5650 6200 6350
Wire Wire Line
	6200 6350 6700 6350
Wire Wire Line
	6700 6350 6700 6800
Wire Wire Line
	6700 6800 6550 6800
Connection ~ 6200 5650
Wire Wire Line
	6200 5650 6350 5650
Wire Wire Line
	6350 5750 6350 6250
Wire Wire Line
	6350 6250 6800 6250
Wire Wire Line
	6800 6250 6800 6950
Wire Wire Line
	6800 6950 6550 6950
Connection ~ 2550 6150
Wire Wire Line
	7550 5450 7700 5450
Wire Wire Line
	7700 6250 6900 6250
Wire Wire Line
	6900 6250 6900 6650
Wire Wire Line
	6900 6650 6550 6650
Wire Wire Line
	7700 5450 7700 6250
Connection ~ 7550 5450
Wire Wire Line
	4200 5750 4200 3800
Wire Wire Line
	4200 3800 1500 3800
Wire Wire Line
	4200 5750 4800 5750
Wire Wire Line
	4300 5650 4300 3700
Wire Wire Line
	4300 3700 1500 3700
$Comp
L power:VCC #PWR0103
U 1 1 6171C433
P 6550 1200
F 0 "#PWR0103" H 6550 1050 50  0001 C CNN
F 1 "VCC" V 6565 1328 50  0000 L CNN
F 2 "" H 6550 1200 50  0001 C CNN
F 3 "" H 6550 1200 50  0001 C CNN
	1    6550 1200
	0    1    1    0   
$EndComp
Text Notes 2600 7700 0    50   ~ 0
limit level of AL53 output signal to 3.3V
Text Notes 1100 6350 0    50   ~ 0
supply filter \nfor AL53
Wire Wire Line
	2550 6150 2550 7000
Text Label 2650 6650 0    50   ~ 0
max8.0V
Text Label 10200 3350 0    50   ~ 0
SCL
Text Label 10200 3450 0    50   ~ 0
SDA
Wire Wire Line
	9950 3350 10200 3350
Wire Wire Line
	9950 3450 10200 3450
$Comp
L Connector_Generic:Conn_01x06 J10
U 1 1 61465AD3
P 10700 6200
F 0 "J10" V 10750 5650 50  0000 L CNN
F 1 "ISP Programmer" V 10850 5900 50  0000 L CNN
F 2 "" H 10700 6200 50  0001 C CNN
F 3 "~" H 10700 6200 50  0001 C CNN
	1    10700 6200
	0    1    1    0   
$EndComp
$Comp
L power:VCC #PWR0104
U 1 1 6179DE9F
P 10950 3050
F 0 "#PWR0104" H 10950 2900 50  0001 C CNN
F 1 "VCC" H 10965 3223 50  0000 C CNN
F 2 "" H 10950 3050 50  0001 C CNN
F 3 "" H 10950 3050 50  0001 C CNN
	1    10950 3050
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0105
U 1 1 6179E7B5
P 10950 3350
F 0 "#PWR0105" H 10950 3100 50  0001 C CNN
F 1 "GND" H 10955 3177 50  0000 C CNN
F 2 "" H 10950 3350 50  0001 C CNN
F 3 "" H 10950 3350 50  0001 C CNN
	1    10950 3350
	1    0    0    -1  
$EndComp
Wire Wire Line
	10950 3150 10950 3050
Wire Wire Line
	9950 3150 10950 3150
Wire Wire Line
	10950 3250 10950 3350
Wire Wire Line
	9950 3250 10950 3250
$Comp
L power:VCC #PWR0107
U 1 1 617B4FBA
P 10950 1800
F 0 "#PWR0107" H 10950 1650 50  0001 C CNN
F 1 "VCC" H 10965 1973 50  0000 C CNN
F 2 "" H 10950 1800 50  0001 C CNN
F 3 "" H 10950 1800 50  0001 C CNN
	1    10950 1800
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0108
U 1 1 617BC556
P 10950 2000
F 0 "#PWR0108" H 10950 1750 50  0001 C CNN
F 1 "GND" H 10955 1827 50  0000 C CNN
F 2 "" H 10950 2000 50  0001 C CNN
F 3 "" H 10950 2000 50  0001 C CNN
	1    10950 2000
	1    0    0    -1  
$EndComp
Wire Wire Line
	9950 1850 10050 1850
Wire Wire Line
	10950 1850 10950 1800
Wire Wire Line
	10950 1950 10950 2000
Wire Wire Line
	9950 2150 10050 2150
Wire Wire Line
	10050 2150 10050 1850
Connection ~ 10050 1850
Wire Wire Line
	10050 1850 10950 1850
Text Notes 8950 1250 0    50   ~ 0
FTDI connector
Wire Wire Line
	8550 2400 7050 2400
Wire Wire Line
	7050 2400 7050 2700
Connection ~ 7050 2700
$Comp
L power:GND #PWR0109
U 1 1 617E6817
P 6550 1550
F 0 "#PWR0109" H 6550 1300 50  0001 C CNN
F 1 "GND" V 6555 1422 50  0000 R CNN
F 2 "" H 6550 1550 50  0001 C CNN
F 3 "" H 6550 1550 50  0001 C CNN
	1    6550 1550
	0    -1   -1   0   
$EndComp
Wire Wire Line
	5100 1950 5100 2050
Wire Wire Line
	5100 2050 5000 2050
Wire Wire Line
	5000 2050 5000 1950
Text Notes 4850 2150 0    50   ~ 0
Don't forget
Wire Wire Line
	1500 3900 1650 3900
Wire Wire Line
	1650 3900 1650 3600
Connection ~ 1650 3600
NoConn ~ 1500 4000
NoConn ~ 1500 4400
NoConn ~ 9050 3900
NoConn ~ 9150 3900
NoConn ~ 9250 3900
NoConn ~ 9000 1300
NoConn ~ 9100 1300
NoConn ~ 9200 1300
NoConn ~ 9300 1300
NoConn ~ 9400 1300
NoConn ~ 9500 1300
$Comp
L Device:R R3
U 1 1 614A62EC
P 3400 1400
F 0 "R3" H 3470 1446 50  0000 L CNN
F 1 "100k" H 3470 1355 50  0000 L CNN
F 2 "" V 3330 1400 50  0001 C CNN
F 3 "~" H 3400 1400 50  0001 C CNN
	1    3400 1400
	1    0    0    -1  
$EndComp
$Comp
L Jumper:SolderJumper_2_Open JP1
U 1 1 614D6D02
P 3400 2400
F 0 "JP1" H 3400 2550 50  0000 C CNN
F 1 "SolderJumper_2_Open" H 3250 2250 50  0000 C CNN
F 2 "" H 3400 2400 50  0001 C CNN
F 3 "~" H 3400 2400 50  0001 C CNN
	1    3400 2400
	1    0    0    -1  
$EndComp
Wire Wire Line
	850  2400 3200 2400
Connection ~ 850  2400
Wire Wire Line
	3200 2400 3200 1950
Connection ~ 3200 2400
Wire Wire Line
	3200 2400 3250 2400
Text Label 4300 2400 0    50   ~ 0
GND
Wire Wire Line
	3600 1950 3600 2400
Connection ~ 3600 2400
Wire Wire Line
	3600 2400 3550 2400
Wire Wire Line
	4150 1200 3400 1200
Wire Wire Line
	3400 1200 3400 1250
Wire Wire Line
	3400 1550 3400 1650
$Comp
L power:PWR_FLAG #FLG0101
U 1 1 615D8F0E
P 3400 1100
F 0 "#FLG0101" H 3400 1175 50  0001 C CNN
F 1 "PWR_FLAG" H 3400 1273 50  0000 C CNN
F 2 "" H 3400 1100 50  0001 C CNN
F 3 "~" H 3400 1100 50  0001 C CNN
	1    3400 1100
	1    0    0    -1  
$EndComp
Wire Wire Line
	3400 1100 3400 1200
Connection ~ 3400 1200
Connection ~ 1650 1200
Wire Wire Line
	1650 1200 1650 1550
Text Label 1950 1200 0    50   ~ 0
Vaccu
Text Notes 2750 1650 0    50   ~ 0
reverse battery\nprotection\n
Wire Wire Line
	2600 5150 2700 5150
Wire Wire Line
	2700 5150 2700 4650
Text Label 700  5500 1    50   ~ 0
8.0Vraw
Wire Wire Line
	6000 1550 6100 1550
Wire Wire Line
	6000 1200 6200 1200
Wire Wire Line
	3800 6200 3800 6150
Wire Wire Line
	3800 5550 4800 5550
Connection ~ 4800 5550
Connection ~ 3800 6150
Wire Wire Line
	3800 6150 3800 5550
Connection ~ 700  5150
Wire Wire Line
	6100 2400 6100 1550
Connection ~ 6100 1550
Wire Wire Line
	6100 1550 6400 1550
Wire Wire Line
	6200 2500 6200 1200
Connection ~ 6200 1200
Wire Wire Line
	3600 2400 3800 2400
Wire Wire Line
	2550 6150 3800 6150
Wire Wire Line
	4800 5450 3950 5450
Wire Wire Line
	3950 2500 6200 2500
Connection ~ 4800 5450
Wire Wire Line
	3800 5550 3800 5250
Wire Wire Line
	2600 5250 3800 5250
Connection ~ 3800 5550
Wire Wire Line
	3800 5250 3800 4950
Wire Wire Line
	1650 3600 3800 3600
Connection ~ 3800 5250
Wire Wire Line
	1500 3500 1950 3500
Connection ~ 3950 3500
Wire Wire Line
	3950 3500 3950 2500
Wire Wire Line
	3800 3600 3800 3400
Connection ~ 3800 3600
Connection ~ 3800 2400
Wire Wire Line
	3800 2400 6100 2400
Wire Wire Line
	4150 1550 3800 1550
Wire Wire Line
	3800 1550 3800 2400
Text Label 4300 2500 0    50   ~ 0
VCC
Text Notes 1800 1050 0    50   ~ 0
IMPORTANT: cut in original PCB the\nangular supply wire from +pole of\naccu cell with a sharp blade
Wire Wire Line
	5800 3400 5500 3400
Connection ~ 3800 3400
Wire Wire Line
	3800 3400 3800 2400
Wire Wire Line
	5800 3800 5500 3800
Wire Wire Line
	5500 3800 5500 3400
Connection ~ 5500 3400
Wire Wire Line
	5500 3400 3800 3400
Wire Wire Line
	5800 4150 5500 4150
Wire Wire Line
	5500 4150 5500 3800
Connection ~ 5500 3800
Wire Wire Line
	5800 4550 5500 4550
Wire Wire Line
	5500 4550 5500 4150
Connection ~ 5500 4150
Wire Wire Line
	5800 4050 5650 4050
Wire Wire Line
	5800 4450 5650 4450
Wire Wire Line
	9950 2250 10200 2250
Wire Wire Line
	9950 2350 10200 2350
Wire Wire Line
	9950 2450 10200 2450
Text Label 10200 2250 0    50   ~ 0
A3
Text Label 10200 2350 0    50   ~ 0
A2
Text Label 10200 2450 0    50   ~ 0
A1
Text Label 5650 3300 0    50   ~ 0
A0
Text Label 5650 3700 0    50   ~ 0
A1
Text Label 5650 4050 0    50   ~ 0
A2
Text Label 5650 4450 0    50   ~ 0
A3
$Comp
L Device:R R17
U 1 1 6191A928
P 1600 4950
F 0 "R17" V 1650 5150 50  0000 C CNN
F 1 "2M" V 1600 4950 50  0000 C CNN
F 2 "" V 1530 4950 50  0001 C CNN
F 3 "~" H 1600 4950 50  0001 C CNN
	1    1600 4950
	0    1    1    0   
$EndComp
$Comp
L Device:R R12
U 1 1 6191BBCF
P 3450 4950
F 0 "R12" V 3500 5100 50  0000 C CNN
F 1 "220k" V 3450 4950 50  0000 C CNN
F 2 "" V 3380 4950 50  0001 C CNN
F 3 "~" H 3450 4950 50  0001 C CNN
	1    3450 4950
	0    1    1    0   
$EndComp
$Comp
L Device:R R6
U 1 1 6191CBCB
P 3000 4650
F 0 "R6" V 3050 4800 50  0000 C CNN
F 1 "220k" V 3000 4650 50  0000 C CNN
F 2 "" V 2930 4650 50  0001 C CNN
F 3 "~" H 3000 4650 50  0001 C CNN
	1    3000 4650
	0    1    1    0   
$EndComp
$Comp
L Device:R R11
U 1 1 6191DB38
P 3450 4650
F 0 "R11" V 3500 4800 50  0000 C CNN
F 1 "220k" V 3450 4650 50  0000 C CNN
F 2 "" V 3380 4650 50  0001 C CNN
F 3 "~" H 3450 4650 50  0001 C CNN
	1    3450 4650
	0    1    1    0   
$EndComp
Connection ~ 3800 4650
Wire Wire Line
	3800 4650 3800 4200
Wire Wire Line
	3600 4650 3800 4650
Wire Wire Line
	3600 4950 3800 4950
Connection ~ 3800 4950
Wire Wire Line
	3800 4950 3800 4650
Wire Wire Line
	1450 4950 700  4950
Wire Wire Line
	700  4950 700  5150
Wire Wire Line
	3300 4650 3250 4650
Wire Wire Line
	2850 4650 2700 4650
Connection ~ 2700 4650
$Comp
L Device:R R9
U 1 1 61996100
P 3450 4000
F 0 "R9" V 3500 4150 50  0000 C CNN
F 1 "220k" V 3450 4000 50  0000 C CNN
F 2 "" V 3380 4000 50  0001 C CNN
F 3 "~" H 3450 4000 50  0001 C CNN
	1    3450 4000
	0    1    1    0   
$EndComp
$Comp
L Device:R R10
U 1 1 619CE803
P 3450 4200
F 0 "R10" V 3500 4350 50  0000 C CNN
F 1 "220k" V 3450 4200 50  0000 C CNN
F 2 "" V 3380 4200 50  0001 C CNN
F 3 "~" H 3450 4200 50  0001 C CNN
	1    3450 4200
	0    1    1    0   
$EndComp
Wire Wire Line
	3600 4200 3800 4200
Connection ~ 3800 4200
Wire Wire Line
	3800 4200 3800 3600
Wire Wire Line
	3600 4000 3950 4000
Wire Wire Line
	3950 3500 3950 4000
Connection ~ 3950 4000
Wire Wire Line
	3950 4000 3950 5450
Wire Wire Line
	3300 4000 3250 4000
$Comp
L Device:C C5
U 1 1 619FAA72
P 1800 3350
F 0 "C5" V 1700 3500 50  0000 C CNN
F 1 "1u" V 1639 3350 50  0000 C CNN
F 2 "" H 1838 3200 50  0001 C CNN
F 3 "~" H 1800 3350 50  0001 C CNN
	1    1800 3350
	0    1    1    0   
$EndComp
Wire Wire Line
	1650 3350 1650 3600
Wire Wire Line
	1950 3350 1950 3500
Wire Wire Line
	1500 3600 1650 3600
Connection ~ 1950 3500
Wire Wire Line
	1950 3500 3950 3500
Text Notes 1550 3100 0    50   ~ 0
place close\nto ADS1115
Wire Wire Line
	1500 4300 3250 4300
Wire Wire Line
	3250 4300 3250 4650
Connection ~ 3250 4650
Wire Wire Line
	3250 4650 3150 4650
Wire Wire Line
	1500 4200 3250 4200
Wire Wire Line
	3250 4000 3250 4200
Connection ~ 3250 4200
Wire Wire Line
	3250 4200 3300 4200
Text Label 1650 4100 0    50   ~ 0
VsenseAL53supply
Text Label 1650 4200 0    50   ~ 0
VsenseVCC
Text Label 1650 4300 0    50   ~ 0
VsenseVaccu
Wire Wire Line
	1750 4950 2450 4950
Wire Wire Line
	2450 4100 2450 4950
Wire Wire Line
	1500 4100 2450 4100
Connection ~ 2450 4950
Wire Wire Line
	2450 4950 3300 4950
Wire Wire Line
	1650 1200 2700 1200
Connection ~ 2700 1200
Wire Wire Line
	2700 1200 3400 1200
$Comp
L Device:R R8
U 1 1 61B65E40
P 2950 3300
F 0 "R8" V 2850 3150 50  0000 C CNN
F 1 "47k" V 2950 3300 50  0000 C CNN
F 2 "" V 2880 3300 50  0001 C CNN
F 3 "~" H 2950 3300 50  0001 C CNN
	1    2950 3300
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R14
U 1 1 61B67363
P 5150 3700
F 0 "R14" V 5050 3850 50  0000 C CNN
F 1 "47k" V 5150 3700 50  0000 C CNN
F 2 "" V 5080 3700 50  0001 C CNN
F 3 "~" H 5150 3700 50  0001 C CNN
	1    5150 3700
	0    1    1    0   
$EndComp
$Comp
L Device:R R7
U 1 1 61B6881B
P 3300 3300
F 0 "R7" V 3400 3450 50  0000 C CNN
F 1 "47k" V 3300 3300 50  0000 C CNN
F 2 "" V 3230 3300 50  0001 C CNN
F 3 "~" H 3300 3300 50  0001 C CNN
	1    3300 3300
	0    1    1    0   
$EndComp
Wire Wire Line
	4900 3300 5800 3300
Wire Wire Line
	5300 3700 5800 3700
Wire Wire Line
	5000 3700 4900 3700
Wire Wire Line
	4900 3700 4900 3300
Wire Wire Line
	3100 3300 3150 3300
Wire Wire Line
	2700 1200 2700 3300
Wire Wire Line
	2800 3300 2700 3300
Connection ~ 2700 3300
Wire Wire Line
	2700 3300 2700 4650
Wire Wire Line
	4900 3300 3450 3300
Connection ~ 4900 3300
Text Notes 4800 3950 0    50   ~ 0
activation pin A1\nsee dev_AL53.h
Text Notes 4800 3250 0    50   ~ 0
sense pin A0\nsee dev_AL53.h
Text Notes 4150 3050 0    50   ~ 0
detect exhaustive discharge\nof NiMH accumulator batteries
Wire Notes Line
	4150 3050 4150 3300
$Comp
L power:GND #PWR0110
U 1 1 61CAF748
P 8600 4450
F 0 "#PWR0110" H 8600 4200 50  0001 C CNN
F 1 "GND" H 8605 4277 50  0000 C CNN
F 2 "" H 8600 4450 50  0001 C CNN
F 3 "" H 8600 4450 50  0001 C CNN
	1    8600 4450
	-1   0    0    1   
$EndComp
$Comp
L Device:CP C1
U 1 1 6146D5D1
P 8350 4600
F 0 "C1" V 8500 4600 50  0000 C CNN
F 1 "10u" V 8200 4600 50  0000 C CNN
F 2 "" H 8388 4450 50  0001 C CNN
F 3 "~" H 8350 4600 50  0001 C CNN
	1    8350 4600
	0    -1   1    0   
$EndComp
Wire Wire Line
	6200 1200 6550 1200
Text Notes 10300 5150 0    50   ~ 0
8.6 cm isolated wire
NoConn ~ 5200 1950
Wire Wire Line
	10950 1950 9950 1950
$Comp
L power:PWR_FLAG #FLG0102
U 1 1 614A5855
P 6400 1650
F 0 "#FLG0102" H 6400 1725 50  0001 C CNN
F 1 "PWR_FLAG" H 6400 1823 50  0000 C CNN
F 2 "" H 6400 1650 50  0001 C CNN
F 3 "~" H 6400 1650 50  0001 C CNN
	1    6400 1650
	1    0    0    1   
$EndComp
Wire Wire Line
	6400 1650 6400 1550
Connection ~ 6400 1550
Wire Wire Line
	6400 1550 6550 1550
$Comp
L power:PWR_FLAG #FLG0103
U 1 1 614F0AF4
P 2750 6500
F 0 "#FLG0103" H 2750 6575 50  0001 C CNN
F 1 "PWR_FLAG" V 2750 6628 50  0000 L CNN
F 2 "" H 2750 6500 50  0001 C CNN
F 3 "~" H 2750 6500 50  0001 C CNN
	1    2750 6500
	0    1    1    0   
$EndComp
Wire Wire Line
	2750 6500 2650 6500
Connection ~ 2650 6500
Wire Wire Line
	2650 6500 2650 7100
Wire Wire Line
	6550 7100 6800 7100
Wire Wire Line
	4500 6800 4000 6800
NoConn ~ 6800 7100
$Comp
L Diode:1N4148 D4
U 1 1 6157E728
P 3600 6850
F 0 "D4" V 3646 6770 50  0000 R CNN
F 1 "1N4148" V 3555 6770 50  0000 R CNN
F 2 "Diode_THT:D_DO-35_SOD27_P7.62mm_Horizontal" H 3600 6675 50  0001 C CNN
F 3 "https://assets.nexperia.com/documents/data-sheet/1N4148_1N4448.pdf" H 3600 6850 50  0001 C CNN
	1    3600 6850
	0    -1   -1   0   
$EndComp
Wire Wire Line
	3600 7000 3600 7200
Connection ~ 3600 7200
Wire Wire Line
	3600 7200 4000 7200
Wire Wire Line
	8550 3200 8200 3200
Wire Wire Line
	8200 3200 8200 3500
Wire Wire Line
	8200 3500 7700 3500
Text Label 7700 3500 0    50   ~ 0
PC3
Text Notes 7300 3700 0    50   ~ 0
to 1N4148 diode D4\n(test pulses for S35770)
Wire Wire Line
	3600 6700 3600 6600
Text Label 3600 6600 0    50   ~ 0
PC3
$Comp
L Device:R R18
U 1 1 615D44DC
P 3950 6500
F 0 "R18" H 3850 6300 50  0000 L CNN
F 1 "10k" V 3950 6400 50  0000 L CNN
F 2 "" V 3880 6500 50  0001 C CNN
F 3 "~" H 3950 6500 50  0001 C CNN
	1    3950 6500
	1    0    0    -1  
$EndComp
Wire Wire Line
	3950 6650 4300 6650
Connection ~ 3950 5450
Wire Wire Line
	8450 3000 8450 3900
Wire Wire Line
	8450 3900 7700 3900
Text Label 7700 3900 0    50   ~ 0
PD7
Text Notes 7300 4000 0    50   ~ 0
to RSTB pin of S35770
Wire Wire Line
	3950 5450 3950 6350
Wire Wire Line
	4300 6650 4300 6250
Connection ~ 4300 6650
Wire Wire Line
	4300 6650 4450 6650
Wire Wire Line
	8450 3000 8550 3000
Text Label 4300 6250 0    50   ~ 0
PD7
NoConn ~ 4000 6800
Text Notes 3200 2700 0    50   ~ 0
J8 on PCB
Text Notes 10450 5250 0    50   ~ 0
as antenna
Text Notes 10450 5050 0    50   ~ 0
connect an
Text Label 8350 2500 0    50   ~ 0
D2
Wire Wire Line
	9950 2550 10200 2550
Wire Wire Line
	9950 2650 10200 2650
Wire Wire Line
	9950 2750 10200 2750
Wire Wire Line
	9950 2850 10200 2850
Wire Wire Line
	9950 2950 10200 2950
Text Label 10200 2550 0    50   ~ 0
A0
Text Label 10200 2650 0    50   ~ 0
SCK
Text Label 10200 2750 0    50   ~ 0
MISO
Text Label 10200 2850 0    50   ~ 0
MOSI
Text Label 10200 2950 0    50   ~ 0
SS
$Comp
L FUEL4EP:CC1101_transceiver_module Module6
U 1 1 6145F643
P 9000 5650
F 0 "Module6" H 9150 5600 50  0000 C CNN
F 1 "CC1101_transceiver_module" V 9450 6200 50  0000 C CNB
F 2 "" H 9200 5550 50  0001 C CNN
F 3 "https://www.ti.com/lit/ds/symlink/cc1101.pdf" H 9200 5550 50  0001 C CNN
	1    9000 5650
	1    0    0    -1  
$EndComp
Wire Notes Line
	10050 5100 10250 5100
Wire Wire Line
	10400 6000 10400 5800
Wire Wire Line
	10500 6000 10500 5800
Wire Wire Line
	10600 6000 10600 5800
Wire Wire Line
	10700 6000 10700 5800
Wire Wire Line
	10800 6000 10800 5800
Wire Wire Line
	10900 6000 10900 5800
Wire Wire Line
	8800 4900 8600 4900
Wire Wire Line
	8800 5000 8600 5000
Wire Wire Line
	8800 5100 8600 5100
Wire Wire Line
	8800 5200 8600 5200
Wire Wire Line
	8800 5300 8600 5300
Wire Wire Line
	9950 2050 10200 2050
Text Label 10400 5800 1    50   ~ 0
VCC
Text Label 10500 5800 1    50   ~ 0
GND
Text Label 10600 5800 1    50   ~ 0
MOSI
Text Label 10700 5800 1    50   ~ 0
SCK
Text Label 10800 5800 1    50   ~ 0
MISO
Text Label 10900 5800 1    50   ~ 0
RSET
Text Label 10200 2050 0    50   ~ 0
RSET
Wire Wire Line
	8800 5400 8600 5400
Wire Wire Line
	8800 5500 8600 5500
$Comp
L power:VCC #PWR0111
U 1 1 61CD0DE5
P 8050 4500
F 0 "#PWR0111" H 8050 4350 50  0001 C CNN
F 1 "VCC" H 8065 4673 50  0000 C CNN
F 2 "" H 8050 4500 50  0001 C CNN
F 3 "" H 8050 4500 50  0001 C CNN
	1    8050 4500
	1    0    0    -1  
$EndComp
Text Label 8600 5500 2    50   ~ 0
SS
Text Label 8600 5300 2    50   ~ 0
D2
Text Label 8600 5200 2    50   ~ 0
MISO
Text Label 8600 5100 2    50   ~ 0
SCK
Text Label 8600 5000 2    50   ~ 0
MOSI
Wire Wire Line
	8050 4800 8050 4600
Wire Wire Line
	8050 4800 8800 4800
Wire Wire Line
	8200 4600 8050 4600
Connection ~ 8050 4600
Wire Wire Line
	8050 4600 8050 4500
Wire Wire Line
	8600 4900 8600 4600
Wire Wire Line
	8600 4600 8500 4600
Connection ~ 8600 4600
Wire Wire Line
	8600 4600 8600 4450
NoConn ~ 8600 5400
NoConn ~ 8550 2100
NoConn ~ 8550 2200
NoConn ~ 8550 2300
NoConn ~ 8550 2600
NoConn ~ 8550 2800
NoConn ~ 8550 2900
NoConn ~ 8850 3900
NoConn ~ 8950 3900
$EndSCHEMATC
