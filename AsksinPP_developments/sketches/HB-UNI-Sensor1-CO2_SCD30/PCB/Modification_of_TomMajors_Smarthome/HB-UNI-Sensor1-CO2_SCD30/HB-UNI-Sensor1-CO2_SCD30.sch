EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 3
Title ""
Date "2020-08-16"
Rev ""
Comp "developped by FUEL4EP"
Comment1 "AsksinPP CO2 sensor based on Sensirion SCD30 sensor"
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Sheet
S 3300 2000 2150 1000
U 5F37F0F6
F0 "Power_Supply" 50
F1 "Power_Supply/Power_Supply.sch" 50
F2 "VBatt_CONN" I L 3300 2300 50 
F3 "VCC_CONN" I L 3300 2450 50 
F4 "GND_CONN" I L 3300 2600 50 
$EndSheet
$Sheet
S 3300 4050 2250 900 
U 5F37F134
F0 "HB-UNI-SEN-BATT_mod" 50
F1 "HB-UNI-SEN-BATT_mod/HB-UNI-SEN-BATT_mod.sch" 50
F2 "VBatt_CONN" U L 3300 4350 50 
F3 "VCC_CONN" U L 3300 4500 50 
F4 "GND_CONN" U L 3300 4650 50 
$EndSheet
$Comp
L Connector:Conn_01x03_Male J2
U 1 1 5F3A7F4E
P 2400 4500
F 0 "J2" H 2450 4250 50  0000 C CNN
F 1 "Conn_01x03_Male" H 2450 4800 50  0000 C CNN
F 2 "" H 2400 4500 50  0001 C CNN
F 3 "~" H 2400 4500 50  0001 C CNN
	1    2400 4500
	1    0    0    1   
$EndComp
Text Notes 3300 5450 0    50   ~ 0
parts  placed and conneted to modified HB-UNI-SEN-BATT PCB\nfrom Alexander Reinert\nunmodified original is available at\nhttps://github.com/alexreinert/PCB/tree/master/HB-UNI-SEN-BATT
$Comp
L Connector:Conn_01x03_Female J1
U 1 1 5F3966E6
P 2350 2450
F 0 "J1" H 2250 2700 50  0000 L CNN
F 1 "Conn_01x03_Female" H 1800 2200 50  0000 L CNN
F 2 "" H 2350 2450 50  0001 C CNN
F 3 "~" H 2350 2450 50  0001 C CNN
	1    2350 2450
	-1   0    0    -1  
$EndComp
Wire Wire Line
	3300 4350 2600 4350
Wire Wire Line
	2600 4350 2600 4400
Wire Wire Line
	3300 4650 2600 4650
Wire Wire Line
	2600 4650 2600 4600
Wire Wire Line
	3300 4500 2600 4500
Wire Wire Line
	3300 2450 2550 2450
Wire Wire Line
	3300 2300 2550 2300
Wire Wire Line
	2550 2300 2550 2350
Wire Wire Line
	3300 2600 2550 2600
Wire Wire Line
	2550 2600 2550 2550
Wire Wire Line
	2350 2350 1350 2350
Wire Wire Line
	1350 2350 1350 4400
Wire Wire Line
	2400 2450 1250 2450
Wire Wire Line
	1250 2450 1250 4500
Wire Wire Line
	2400 4400 1350 4400
Wire Wire Line
	2400 4500 1250 4500
Wire Wire Line
	2350 2550 1150 2550
Wire Wire Line
	1150 2550 1150 4600
Wire Wire Line
	1150 4600 2400 4600
Text Notes 3350 3350 0    50   ~ 0
3D printed holdings, the modules, glued bd two-component adhesive to\nthe top case
$EndSCHEMATC
