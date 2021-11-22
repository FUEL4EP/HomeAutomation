EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "HB-UNI-SenAct-4-4-SC_DS_FUEL4EP"
Date "2021-10-01"
Rev "1.0"
Comp "FUEL4EP"
Comment1 "3x Schaltaktor AsksinPP für Netzfreischalter Gehäusebauteile"
Comment2 "Creative Commons License, non-commercial"
Comment3 "Vorsicht 230V: Nutzung auf eigene Gefahr!"
Comment4 "Nur für ausgebildete Elektrofachleute"
$EndDescr
$Comp
L Graphic:Logo_Open_Hardware_Small LOGO1
U 1 1 615DEE46
P 10310 6870
F 0 "LOGO1" H 10310 7145 50  0001 C CNN
F 1 "Logo_Open_Hardware_Small" H 10310 6645 50  0001 C CNN
F 2 "FUEL4EP:CC-BY-ND-SA" H 10310 6870 50  0001 C CNN
F 3 "~" H 10310 6870 50  0001 C CNN
	1    10310 6870
	1    0    0    -1  
$EndComp
Text Notes 9825 4650 2    60   ~ 0
4x AA NiMH\n 2700mA  accu\nin battery holder\n
$Comp
L Device:Battery_Cell BT1
U 1 1 6194D1D7
P 8925 2950
F 0 "BT1" H 9043 3046 50  0000 L CNN
F 1 "2700mAh NiMH acccucell" H 9043 2955 50  0000 L CNN
F 2 "" V 8925 3010 50  0001 C CNN
F 3 "~" V 8925 3010 50  0001 C CNN
	1    8925 2950
	1    0    0    -1  
$EndComp
$Comp
L Device:Battery_Cell BT2
U 1 1 6194DAA8
P 8925 3325
F 0 "BT2" H 9043 3421 50  0000 L CNN
F 1 "2700mAh NiMH acccucell" H 9043 3330 50  0000 L CNN
F 2 "" V 8925 3385 50  0001 C CNN
F 3 "~" V 8925 3385 50  0001 C CNN
	1    8925 3325
	1    0    0    -1  
$EndComp
$Comp
L Device:Battery_Cell BT3
U 1 1 6194E236
P 8925 3700
F 0 "BT3" H 9043 3796 50  0000 L CNN
F 1 "2700mAh NiMH acccucell" H 9043 3705 50  0000 L CNN
F 2 "" V 8925 3760 50  0001 C CNN
F 3 "~" V 8925 3760 50  0001 C CNN
	1    8925 3700
	1    0    0    -1  
$EndComp
$Comp
L Device:Battery_Cell BT4
U 1 1 6194E845
P 8925 4075
F 0 "BT4" H 9043 4171 50  0000 L CNN
F 1 "2700mAh NiMH acccucell" H 9043 4080 50  0000 L CNN
F 2 "" V 8925 4135 50  0001 C CNN
F 3 "~" V 8925 4135 50  0001 C CNN
	1    8925 4075
	1    0    0    -1  
$EndComp
Wire Wire Line
	8925 3050 8925 3125
Wire Wire Line
	8925 3425 8925 3500
Wire Wire Line
	8925 3800 8925 3875
$Comp
L Connector_Generic:Conn_01x02 J6
U 1 1 619754A5
P 10850 3500
F 0 "J6" H 10890 3310 50  0000 R CNN
F 1 "Accumulator" H 11090 3610 50  0000 R CNN
F 2 "" H 10850 3500 50  0001 C CNN
F 3 "~" H 10850 3500 50  0001 C CNN
	1    10850 3500
	1    0    0    1   
$EndComp
Wire Wire Line
	8925 2750 10100 2750
Wire Wire Line
	8925 4175 10100 4175
Wire Wire Line
	10100 2750 10100 3400
Wire Notes Line
	8725 4325 8725 2625
Wire Notes Line
	8725 2625 10225 2625
Wire Notes Line
	10225 2625 10225 4325
Wire Notes Line
	10225 4325 8725 4325
$Comp
L Switch:SW_Push SW1
U 1 1 6197BAC3
P 2625 4740
F 0 "SW1" H 2425 4790 50  0000 C CNN
F 1 "Heating" H 2850 4840 50  0000 C CNN
F 2 "" H 2625 4940 50  0001 C CNN
F 3 "~" H 2625 4940 50  0001 C CNN
	1    2625 4740
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push SW2
U 1 1 6197C522
P 2625 4965
F 0 "SW2" H 2425 5015 50  0000 C CNN
F 1 "Monitor" H 2825 5065 50  0000 C CNN
F 2 "" H 2625 5165 50  0001 C CNN
F 3 "~" H 2625 5165 50  0001 C CNN
	1    2625 4965
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push SW3
U 1 1 6197CFB4
P 2625 5190
F 0 "SW3" H 2425 5240 50  0000 C CNN
F 1 "Hifiberry" H 2850 5290 50  0000 C CNN
F 2 "" H 2625 5390 50  0001 C CNN
F 3 "~" H 2625 5390 50  0001 C CNN
	1    2625 5190
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push SW4
U 1 1 6197D943
P 2625 5390
F 0 "SW4" H 2425 5440 50  0000 C CNN
F 1 "AC supply" H 2875 5490 50  0000 C CNN
F 2 "" H 2625 5590 50  0001 C CNN
F 3 "~" H 2625 5590 50  0001 C CNN
	1    2625 5390
	1    0    0    -1  
$EndComp
$Comp
L Device:LED D4
U 1 1 6197F175
P 2600 7225
F 0 "D4" H 2450 7275 50  0000 C CNN
F 1 "LED orange" H 2850 7300 50  0000 C CNN
F 2 "" H 2600 7225 50  0001 C CNN
F 3 "~" H 2600 7225 50  0001 C CNN
	1    2600 7225
	1    0    0    -1  
$EndComp
$Comp
L Device:Fuse F1
U 1 1 61981757
P 6220 6060
F 0 "F1" V 6300 6010 50  0000 L CNN
F 1 "1A flink" V 6130 5910 50  0000 L CNN
F 2 "" V 6150 6060 50  0001 C CNN
F 3 "~" H 6220 6060 50  0001 C CNN
	1    6220 6060
	0    -1   -1   0   
$EndComp
$Comp
L Connector_Generic:Conn_01x06 J5
U 1 1 6197DE85
P 1675 5065
F 0 "J5" H 1675 4665 50  0000 C CNN
F 1 "4x Push Buttons" H 1825 5390 50  0000 C CNN
F 2 "" H 1675 5065 50  0001 C CNN
F 3 "~" H 1675 5065 50  0001 C CNN
	1    1675 5065
	-1   0    0    1   
$EndComp
$Comp
L Connector_Generic:Conn_01x07 J4
U 1 1 6197EA9D
P 1650 7125
F 0 "J4" H 1650 6725 50  0000 C CNN
F 1 "5x LEDs" H 1675 7550 50  0000 C CNN
F 2 "" H 1650 7125 50  0001 C CNN
F 3 "~" H 1650 7125 50  0001 C CNN
	1    1650 7125
	-1   0    0    1   
$EndComp
Wire Wire Line
	1875 4965 2425 4965
Wire Wire Line
	2425 5190 2175 5190
Wire Wire Line
	2175 5190 2175 5065
Wire Wire Line
	2175 5065 1875 5065
Wire Wire Line
	2075 5165 1875 5165
Wire Wire Line
	1875 4865 2175 4865
Wire Wire Line
	2175 4865 2175 4740
Wire Wire Line
	2175 4740 2425 4740
Wire Wire Line
	3200 5565 1875 5565
Wire Wire Line
	1875 5565 1875 5265
Wire Wire Line
	2825 5190 3200 5190
Wire Wire Line
	2825 4965 3200 4965
Wire Wire Line
	3200 4965 3200 5190
Connection ~ 3200 5190
Wire Wire Line
	2825 4740 3200 4740
Connection ~ 3200 4965
NoConn ~ 1875 4765
Wire Wire Line
	3200 5190 3200 5390
Wire Wire Line
	2075 5390 2425 5390
Wire Wire Line
	2075 5165 2075 5390
Wire Wire Line
	2825 5390 3200 5390
Connection ~ 3200 5390
Wire Wire Line
	3200 5390 3200 5565
Wire Wire Line
	3200 4740 3200 4965
Text Label 1950 5565 0    50   ~ 0
GND
$Comp
L Device:LED D5
U 1 1 619EF23E
P 2600 7425
F 0 "D5" H 2450 7475 50  0000 C CNN
F 1 "LED rot" H 2850 7500 50  0000 C CNN
F 2 "" H 2600 7425 50  0001 C CNN
F 3 "~" H 2600 7425 50  0001 C CNN
	1    2600 7425
	1    0    0    -1  
$EndComp
$Comp
L Device:LED D3
U 1 1 619EFA57
P 2600 7025
F 0 "D3" H 2450 7075 50  0000 C CNN
F 1 "LED grün" H 2850 7100 50  0000 C CNN
F 2 "" H 2600 7025 50  0001 C CNN
F 3 "~" H 2600 7025 50  0001 C CNN
	1    2600 7025
	1    0    0    -1  
$EndComp
$Comp
L Device:LED D2
U 1 1 619F0411
P 2600 6825
F 0 "D2" H 2450 6875 50  0000 C CNN
F 1 "LED gelb" H 2850 6900 50  0000 C CNN
F 2 "" H 2600 6825 50  0001 C CNN
F 3 "~" H 2600 6825 50  0001 C CNN
	1    2600 6825
	1    0    0    -1  
$EndComp
$Comp
L Device:LED D1
U 1 1 619F164D
P 2600 6625
F 0 "D1" H 2450 6675 50  0000 C CNN
F 1 "LED blau" H 2850 6700 50  0000 C CNN
F 2 "" H 2600 6625 50  0001 C CNN
F 3 "~" H 2600 6625 50  0001 C CNN
	1    2600 6625
	1    0    0    -1  
$EndComp
Wire Wire Line
	2450 7425 1975 7425
Wire Wire Line
	1975 7425 1975 7325
Wire Wire Line
	1975 7325 1850 7325
Wire Wire Line
	2450 7225 1850 7225
Wire Wire Line
	2450 6625 2025 6625
Wire Wire Line
	2025 6625 2025 6925
Wire Wire Line
	2025 6925 1850 6925
Wire Wire Line
	2450 6825 2125 6825
Wire Wire Line
	2125 6825 2125 7025
Wire Wire Line
	2125 7025 1850 7025
Wire Wire Line
	2450 7025 2225 7025
Wire Wire Line
	2225 7025 2225 7125
Wire Wire Line
	2225 7125 1850 7125
Wire Wire Line
	1850 6825 1925 6825
Wire Wire Line
	1925 6825 1925 6425
Wire Wire Line
	1925 6425 3150 6425
Wire Wire Line
	3150 6425 3150 6625
Wire Wire Line
	3150 7425 2750 7425
Wire Wire Line
	2750 7025 3150 7025
Connection ~ 3150 7025
Wire Wire Line
	2750 6825 3150 6825
Connection ~ 3150 6825
Wire Wire Line
	3150 6825 3150 7025
Wire Wire Line
	2750 6625 3150 6625
Connection ~ 3150 6625
Wire Wire Line
	3150 6625 3150 6825
Text Label 2025 6425 0    50   ~ 0
V_relais
Wire Wire Line
	4500 7200 4500 7325
Wire Wire Line
	4500 7325 4570 7325
Wire Wire Line
	4650 7325 4650 7200
Wire Wire Line
	1875 3050 4050 3050
$Comp
L Connector_Generic:Conn_01x06 J3
U 1 1 6197FBD3
P 1675 3250
F 0 "J3" H 1625 2850 50  0000 L CNN
F 1 "4x Relais" H 1500 3575 50  0000 L CNN
F 2 "" H 1675 3250 50  0001 C CNN
F 3 "~" H 1675 3250 50  0001 C CNN
	1    1675 3250
	-1   0    0    1   
$EndComp
$Comp
L Connector:Conn_WallSocket_Earth WP1
U 1 1 61AA9383
P 6680 2300
F 0 "WP1" H 6805 2610 50  0000 L CNN
F 1 "Steckdose Heizung" H 6795 2165 50  0000 L CNN
F 2 "" H 6380 2400 50  0001 C CNN
F 3 "~" H 6380 2400 50  0001 C CNN
	1    6680 2300
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_WallSocket_Earth WP2
U 1 1 61AAA13F
P 6690 3500
F 0 "WP2" H 6820 3815 50  0000 L CNN
F 1 "Steckdose Monitor" H 6795 3385 50  0000 L CNN
F 2 "" H 6390 3600 50  0001 C CNN
F 3 "~" H 6390 3600 50  0001 C CNN
	1    6690 3500
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_WallSocket_Earth WP3
U 1 1 61AAACFE
P 6680 4700
F 0 "WP3" H 6830 5030 50  0000 L CNN
F 1 "Steckdose Hifiberry" H 6780 4590 50  0000 L CNN
F 2 "" H 6380 4800 50  0001 C CNN
F 3 "~" H 6380 4800 50  0001 C CNN
	1    6680 4700
	1    0    0    -1  
$EndComp
Text Label 2400 3350 0    50   ~ 0
AC_supply
Text Label 2400 3250 0    50   ~ 0
Hifiberry
Text Label 2400 3150 0    50   ~ 0
Monitor
Text Label 2400 3050 0    50   ~ 0
Heating
Text Label 2400 3450 0    50   ~ 0
V_relais
Text Label 2400 2950 0    50   ~ 0
GND
Wire Wire Line
	4050 3150 1875 3150
Wire Wire Line
	4050 3250 1875 3250
Wire Wire Line
	4050 3350 1875 3350
$Comp
L FUEL4EP:4_channel_relay_module Module1
U 1 1 61985CC4
P 4250 1850
F 0 "Module1" H 4950 2037 60  0000 C CNN
F 1 "4_channel_relay_module" H 4950 1931 60  0000 C CNN
F 2 "" H 4240 1800 60  0001 C CNN
F 3 "http://s3.amazonaws.com/s3.image.smart/download/101-70-100/2%2Brelay.rar" H 4240 1800 60  0001 C CNN
	1    4250 1850
	1    0    0    -1  
$EndComp
Text Notes 1160 2980 0    50   ~ 0
GND\n
Text Notes 1150 3375 0    50   ~ 0
AC suppy\n
Text Notes 1150 3275 0    50   ~ 0
Hifiberry\n
Text Notes 1150 3175 0    50   ~ 0
Monitor\n
Text Notes 1150 3075 0    50   ~ 0
Heating
Text Notes 1150 3470 0    50   ~ 0
V_relais\n
Text Notes 1150 5290 0    50   ~ 0
GND\n
Text Notes 1150 5190 0    50   ~ 0
AC suppy\n
Text Notes 1150 5090 0    50   ~ 0
Hifiberry\n
Text Notes 1150 4990 0    50   ~ 0
Monitor\n
Text Notes 1150 4890 0    50   ~ 0
Heating
Text Notes 1150 4790 0    50   ~ 0
V_relais\n
Text Notes 1150 7450 0    50   ~ 0
GND\n
Text Notes 1150 7350 0    50   ~ 0
AC suppy\n
Text Notes 1150 7250 0    50   ~ 0
Hifiberry\n
Text Notes 1150 7150 0    50   ~ 0
Monitor\n
Text Notes 1150 7050 0    50   ~ 0
Heating
Text Notes 1150 6850 0    50   ~ 0
V_relais\n
Text Notes 1150 6950 0    50   ~ 0
Charging\n
Wire Wire Line
	3150 7025 3150 7225
Wire Wire Line
	2750 7225 3150 7225
Connection ~ 3150 7225
Wire Wire Line
	3150 7225 3150 7425
Connection ~ 6270 2300
Wire Wire Line
	6480 4700 6270 4700
Wire Wire Line
	6270 2300 6480 2300
Wire Wire Line
	6370 3700 6490 3700
Wire Wire Line
	6370 3700 6370 4900
Wire Wire Line
	6370 4900 6480 4900
Connection ~ 6370 3700
Wire Wire Line
	6180 5860 5850 5860
Wire Wire Line
	5850 4710 6180 4710
Connection ~ 6180 4710
Wire Wire Line
	6180 4710 6180 5860
Wire Wire Line
	5850 3510 6180 3510
Connection ~ 6180 3510
Wire Wire Line
	6180 3510 6180 4710
Connection ~ 6180 2310
Wire Wire Line
	6180 2310 6180 3510
NoConn ~ 5850 2110
Wire Wire Line
	5850 2310 6180 2310
Wire Wire Line
	6090 2100 6090 2510
Wire Wire Line
	6090 2510 5900 2510
$Comp
L Converter_ACDC:HLK-PM01 PS1
U 1 1 61BD40F0
P 6830 5760
F 0 "PS1" H 6830 6085 50  0000 C CNN
F 1 "Tracopower 5V 1A TIW 06-105" H 6890 5990 50  0000 C CNN
F 2 "Converter_ACDC:Converter_ACDC_HiLink_HLK-PMxx" H 6830 5460 50  0001 C CNN
F 3 "http://www.hlktech.net/product_detail.php?ProId=54" H 7230 5410 50  0001 C CNN
	1    6830 5760
	1    0    0    -1  
$EndComp
Wire Wire Line
	6090 4910 6090 4500
Wire Wire Line
	6090 4500 6480 4500
NoConn ~ 5850 4510
NoConn ~ 5850 3310
Wire Wire Line
	5850 3710 5890 3710
Wire Wire Line
	6090 3710 6090 3300
Wire Wire Line
	6090 3300 6490 3300
Wire Wire Line
	6270 1250 6270 2300
$Comp
L Connector:Conn_WallPlug_Earth WS1
U 1 1 61C0BE05
P 10190 1150
F 0 "WS1" H 10120 1360 50  0000 R CNN
F 1 "Schukostecker" H 10220 885 50  0000 R CNN
F 2 "" H 10590 1150 50  0001 C CNN
F 3 "~" H 10590 1150 50  0001 C CNN
	1    10190 1150
	-1   0    0    -1  
$EndComp
Wire Wire Line
	6180 1050 6180 2310
Wire Wire Line
	6070 6060 5945 6060
Wire Wire Line
	10100 3400 10650 3400
Text Label 10240 3400 0    50   ~ 0
plusBAT
Wire Wire Line
	10100 3500 10650 3500
Wire Wire Line
	10100 3500 10100 4175
Text Label 10240 3500 0    50   ~ 0
minusBAT
Text Notes 11070 1310 1    50   ~ 0
230V AC\n
Text Notes 10935 1190 2    50   ~ 0
Schutzkontakt\n
Text Notes 10635 1100 2    50   ~ 0
Phase\n
Text Notes 10640 1295 2    50   ~ 0
Masse\n
Wire Wire Line
	10900 6255 10890 6255
$Comp
L Connector_Generic:Conn_01x02 J1
U 1 1 61CCF4E2
P 10705 5760
F 0 "J1" H 10705 5550 50  0000 C CNN
F 1 "5V supply" H 10710 5875 50  0000 C CNN
F 2 "" H 10705 5760 50  0001 C CNN
F 3 "~" H 10705 5760 50  0001 C CNN
	1    10705 5760
	1    0    0    1   
$EndComp
Wire Wire Line
	7230 5660 10505 5660
Wire Wire Line
	7550 5760 7550 5860
Wire Wire Line
	7550 5860 7230 5860
Text Label 10015 5660 0    50   ~ 0
5V
Text Notes 10970 5680 2    50   ~ 0
+5V\n
Text Notes 11155 5860 2    50   ~ 0
minusBAT\n\n
Text Notes 10975 3425 1    50   ~ 0
+
Text Notes 10975 3530 2    50   ~ 0
-\n
Wire Wire Line
	6090 2100 6480 2100
NoConn ~ 1850 7425
$Comp
L power:PWR_FLAG #FLG0102
U 1 1 61D3C4C3
P 4570 7325
F 0 "#FLG0102" H 4570 7400 50  0001 C CNN
F 1 "PWR_FLAG" H 4570 7498 50  0000 C CNN
F 2 "" H 4570 7325 50  0001 C CNN
F 3 "~" H 4570 7325 50  0001 C CNN
	1    4570 7325
	-1   0    0    1   
$EndComp
Connection ~ 4570 7325
Wire Wire Line
	4570 7325 4650 7325
$Comp
L power:PWR_FLAG #FLG0104
U 1 1 61D4C054
P 6995 1050
F 0 "#FLG0104" H 6995 1125 50  0001 C CNN
F 1 "PWR_FLAG" H 6995 1223 50  0000 C CNN
F 2 "" H 6995 1050 50  0001 C CNN
F 3 "~" H 6995 1050 50  0001 C CNN
	1    6995 1050
	1    0    0    -1  
$EndComp
Wire Wire Line
	5850 4910 5940 4910
$Comp
L power:PWR_FLAG #FLG0105
U 1 1 61D67CA1
P 5940 4910
F 0 "#FLG0105" H 5940 4985 50  0001 C CNN
F 1 "PWR_FLAG" H 5940 5083 50  0000 C CNN
F 2 "" H 5940 4910 50  0001 C CNN
F 3 "~" H 5940 4910 50  0001 C CNN
	1    5940 4910
	-1   0    0    1   
$EndComp
Connection ~ 5940 4910
Wire Wire Line
	5940 4910 6090 4910
$Comp
L power:PWR_FLAG #FLG0106
U 1 1 61D71E31
P 5945 6060
F 0 "#FLG0106" H 5945 6135 50  0001 C CNN
F 1 "PWR_FLAG" H 5945 6233 50  0000 C CNN
F 2 "" H 5945 6060 50  0001 C CNN
F 3 "~" H 5945 6060 50  0001 C CNN
	1    5945 6060
	-1   0    0    1   
$EndComp
Connection ~ 5945 6060
Wire Wire Line
	5945 6060 5850 6060
Wire Wire Line
	6270 4700 6270 5860
Wire Wire Line
	6270 5860 6430 5860
Connection ~ 6270 4700
Wire Wire Line
	6480 2500 6370 2500
Connection ~ 6370 2500
Wire Wire Line
	6370 2500 6370 3700
Wire Wire Line
	9240 1050 6995 1050
Connection ~ 6995 1050
Wire Wire Line
	6995 1050 6180 1050
Wire Wire Line
	9540 1050 9890 1050
Wire Wire Line
	9890 1250 6270 1250
Wire Wire Line
	9890 1450 6990 1450
Wire Wire Line
	6370 1450 6370 2500
$Comp
L power:PWR_FLAG #FLG0103
U 1 1 61DCD741
P 6990 1450
F 0 "#FLG0103" H 6990 1525 50  0001 C CNN
F 1 "PWR_FLAG" H 6990 1623 50  0000 C CNN
F 2 "" H 6990 1450 50  0001 C CNN
F 3 "~" H 6990 1450 50  0001 C CNN
	1    6990 1450
	-1   0    0    1   
$EndComp
Connection ~ 6990 1450
Wire Wire Line
	6990 1450 6370 1450
$Comp
L power:PWR_FLAG #FLG0107
U 1 1 61DD5DDA
P 5900 2510
F 0 "#FLG0107" H 5900 2585 50  0001 C CNN
F 1 "PWR_FLAG" H 5900 2683 50  0000 C CNN
F 2 "" H 5900 2510 50  0001 C CNN
F 3 "~" H 5900 2510 50  0001 C CNN
	1    5900 2510
	-1   0    0    1   
$EndComp
Connection ~ 5900 2510
Wire Wire Line
	5900 2510 5850 2510
$Comp
L power:PWR_FLAG #FLG0108
U 1 1 61DD7AC4
P 5890 3710
F 0 "#FLG0108" H 5890 3785 50  0001 C CNN
F 1 "PWR_FLAG" H 5890 3883 50  0000 C CNN
F 2 "" H 5890 3710 50  0001 C CNN
F 3 "~" H 5890 3710 50  0001 C CNN
	1    5890 3710
	-1   0    0    1   
$EndComp
Connection ~ 5890 3710
Wire Wire Line
	5890 3710 6090 3710
Wire Wire Line
	6270 2300 6270 3500
Wire Wire Line
	6490 3500 6270 3500
Connection ~ 6270 3500
Wire Wire Line
	6270 3500 6270 4700
NoConn ~ 5850 5660
$Comp
L power:PWR_FLAG #FLG0109
U 1 1 619C0A73
P 2150 3450
F 0 "#FLG0109" H 2150 3525 50  0001 C CNN
F 1 "PWR_FLAG" H 2150 3623 50  0000 C CNN
F 2 "" H 2150 3450 50  0001 C CNN
F 3 "~" H 2150 3450 50  0001 C CNN
	1    2150 3450
	-1   0    0    1   
$EndComp
Connection ~ 2150 3450
Wire Wire Line
	2150 3450 4050 3450
Wire Wire Line
	1875 3450 2150 3450
Wire Wire Line
	7550 5760 10505 5760
Wire Wire Line
	1875 2950 2150 2950
Text Label 10010 5760 0    50   ~ 0
minusBAT
$Comp
L power:PWR_FLAG #FLG0101
U 1 1 619AC6EE
P 2150 2950
F 0 "#FLG0101" H 2150 3025 50  0001 C CNN
F 1 "PWR_FLAG" H 2150 3123 50  0000 C CNN
F 2 "" H 2150 2950 50  0001 C CNN
F 3 "~" H 2150 2950 50  0001 C CNN
	1    2150 2950
	1    0    0    -1  
$EndComp
Connection ~ 2150 2950
Wire Wire Line
	2150 2950 4050 2950
$Comp
L Device:Fuse F2
U 1 1 619FC656
P 9390 1050
F 0 "F2" V 9193 1050 50  0000 C CNN
F 1 "5A flink" V 9284 1050 50  0000 C CNN
F 2 "" V 9320 1050 50  0001 C CNN
F 3 "~" H 9390 1050 50  0001 C CNN
	1    9390 1050
	0    1    1    0   
$EndComp
Wire Wire Line
	6370 6060 6370 5660
Wire Wire Line
	6370 5660 6430 5660
$Comp
L power:PWR_FLAG #FLG0110
U 1 1 61A0DFAA
P 6370 6060
F 0 "#FLG0110" H 6370 6135 50  0001 C CNN
F 1 "PWR_FLAG" V 6370 6188 50  0000 L CNN
F 2 "" H 6370 6060 50  0001 C CNN
F 3 "~" H 6370 6060 50  0001 C CNN
	1    6370 6060
	0    1    1    0   
$EndComp
Connection ~ 6370 6060
Text Notes 10595 775  0    50   ~ 0
max.5A
Text Notes 4215 1565 0    50   ~ 0
mounted on top of base PCB by spacers
$EndSCHEMATC
