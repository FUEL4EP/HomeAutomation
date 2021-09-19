EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 2 2
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text HLabel 3550 3150 0    50   Input ~ 0
1C_RSTB_DIP_PCB
Text HLabel 3500 3400 0    50   UnSpc ~ 0
2C_NC_DIP_PCB
Text HLabel 3550 3650 0    50   Input ~ 0
3C_CNTIN_DIP_PCB
Text HLabel 3550 3900 0    50   Input ~ 0
4C_GND_DIP_PCB
Text HLabel 6900 3150 2    50   Input ~ 0
1A_VDD_DIP_PCB
Text HLabel 6900 3400 2    50   Input ~ 0
2A_SCL_DIP_PCB
Text HLabel 6900 3650 2    50   BiDi ~ 0
3A_SDA_DIP_PCB
Text HLabel 6900 3900 2    50   Output ~ 0
4A_LOOP_DIP_PCB
Wire Wire Line
	3550 3150 4050 3150
Wire Wire Line
	4050 3150 4050 3300
Wire Wire Line
	4050 3300 4300 3300
Wire Wire Line
	3500 3400 4300 3400
Wire Wire Line
	3550 3650 4050 3650
Wire Wire Line
	4050 3650 4050 3500
Wire Wire Line
	4050 3500 4300 3500
Wire Wire Line
	3550 3900 4200 3900
Wire Wire Line
	4200 3900 4200 3600
Wire Wire Line
	4200 3600 4300 3600
Text Notes 3700 4850 0    50   ~ 0
\nSOP8 SO8 SOIC8 SMD to DIP8 Adapter PCB Board Convertor Double Sides\n\nsee https://www.amazon.de/SOIC8-Adapter-Board-Convertor-Double/dp/B00JK8EYTG
Wire Notes Line
	3500 4450 6950 4450
Wire Notes Line
	6950 4450 6950 2650
Wire Notes Line
	6950 2650 3500 2650
Wire Notes Line
	3500 2650 3500 4450
$Comp
L FUEL4EP:ABLIC_S35770 IC?
U 1 1 61460AC3
P 4500 3700
F 0 "IC?" H 4550 3600 50  0000 C CNN
F 1 "ABLIC_S35770" H 5150 3950 50  0000 C CNB
F 2 "" H 4550 3600 50  0001 C CNN
F 3 "https://www.ablic.com/en/doc/datasheet/counter_ic/S35770_I_E.pdf" H 4550 3600 50  0001 C CNN
	1    4500 3700
	1    0    0    -1  
$EndComp
Wire Wire Line
	5950 3600 6100 3600
Wire Wire Line
	6100 3600 6100 3900
Wire Wire Line
	6100 3900 6900 3900
Wire Wire Line
	5950 3500 6300 3500
Wire Wire Line
	6300 3500 6300 3650
Wire Wire Line
	6300 3650 6900 3650
Wire Wire Line
	5950 3400 6900 3400
Wire Wire Line
	5950 3300 6300 3300
Wire Wire Line
	6300 3300 6300 3150
Wire Wire Line
	6300 3150 6900 3150
$EndSCHEMATC
