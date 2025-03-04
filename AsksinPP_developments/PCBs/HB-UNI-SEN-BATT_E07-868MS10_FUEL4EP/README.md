# HB-UNI-SEN-BATT_E07-868MS10_FUEL4EP [![License: CC BY-NC-SA 4.0](https://img.shields.io/badge/License-CC%20BY--NC--SA%204.0-lightgrey.svg)](https://creativecommons.org/licenses/by-nc-sa/4.0/)[![Hits](https://hits.seeyoufarm.com/api/count/incr/badge.svg?url=https%3A%2F%2Fgithub.com%2FFUEL4EP%2FHomeAutomation%2Ftree%2Fmaster%2FAsksinPP_developments%2FPCBs%2FHB-UNI-SEN-BATT_E07-868MS10_FUEL4EP&count_bg=%2379C83D&title_bg=%23555555&icon=&icon_color=%23E7E7E7&title=hits&edge_flat=false)](https://hits.seeyoufarm.com) <a href='https://ko-fi.com/FUEL4EP' target='_blank'><img height='20' style='border:0px;height:20px;' src='https://cdn.ko-fi.com/cdn/kofi1.png?v=2' border='0' alt='Buy Me a Coffee at ko-fi.com' /></a>

**Platine für DIY AsksinPP für**
- [Tindie Pro Mini XL - v2 - ATmega 1284p](https://www.tindie.com/products/prominimicros/pro-mini-xl-v2-atmega-1284p/) **oder Arduino Pro Mini**
- [eByte E07-868MS10 Sendemodul](https://www.rcscomponents.kiev.ua/datasheets/e07-868ms10_usermanual_en_v1_20.pdf)
- Steckerleiste für [Waveshare ePaper Modul (1.54inch (B)](https://www.waveshare.com/product/1.54inch-e-paper-module.htm)

![pic](PNGs/HB-UNI-SEN-BATT_E07-868MS10_FUEL4EP_top_view.png)

## Credits

- diese Platine ist von Alexander Reinerts Platine [HB-UNI-SEN-BATT](https://github.com/alexreinert/PCB/tree/master/HB-UNI-SEN-BATT) abgeleitet.
- vielen Dank an Alexander Reinert für die Vorarbeit.

## Platine wurde nach Fertigung bei JLCPCB erfolgreich validiert

- entwickelt mit KiCAD 5.1.10
- die Platine wurde bei JLCPCB gefertigt
![pic](Pictures_of_JLCPCB_prototypes/HB-UNI-SEN-BATT_E07-868MS10_FUEL4EP_PCB.png)
- die gefertigte Platine wurde erfolgreich mit dem Sketch [HB-UNI-SENSOR1-THPD-SHT85](https://github.com/FUEL4EP/HomeAutomation/tree/master/AsksinPP_developments/sketches/HB-UNI-Sensor1-THPD-SHT85) validiert:
![pic](Pictures_of_JLCPCB_prototypes/HB-UNI-Sensor1-THPD-SHT85_build_with_HB-UNI-SEN-BATT_E07-868MS10_FUEL4EP_PCB.png)


## Eigenschaften

- abgeleitet aus [HB-UNI-SEN-BATT](https://github.com/alexreinert/PCB/tree/master/HB-UNI-SEN-BATT)
- kompletter Datensatz für KiCAD 8 auf Github verfügbar:
    + 'git clone https://github.com/FUEL4EP/HomeAutomation.git' oder als [ZIP-Datei](https://github.com/FUEL4EP/HomeAutomation/archive/refs/heads/master.zip)
    + die Platinendaten sind dann unter 'HomeAutomation/AsksinPP_developments/PCBs/HB-UNI-SEN-BATT_E07-868MS10_FUEL4EP' direkt mit KiCAD aufrufbar
- der Arduino Pro Mini ist durch einen [Tindie Pro Mini XL - v2 - ATmega 1284p](https://www.tindie.com/products/prominimicros/pro-mini-xl-v2-atmega-1284p/) ersetzt
- alternativ kann stattdessen ein Arduino Pro Mini verbaut werden
- neue Eigenschaften:
    + nur eine LED
    + dafür optionale Unterstützung von Akkumulatorbetrieb mit einem 'Huckepack' Aufwärtswandlermodul (siehe z.B. [HB-UNI-SENSOR1-CO2-SCD30](https://github.com/FUEL4EP/HomeAutomation/tree/master/AsksinPP_developments/sketches/HB-UNI-Sensor1-CO2_SCD30): Bitte für Akkubetrieb den Lötjumper JP2 auftrennen
    + Stiftleiste J10 mit Versorgungspannungen zum Gehäuseoberteil oder Akkulademodul
    + Korrektur der Abstandfehlers bei einem Batteriehalter. Beim [HB-UNI-SEN-BATT](https://github.com/alexreinert/PCB/tree/master/HB-UNI-SEN-BATT) hat das schon mal gerne zu thermischen Ausfällen durch mechanische Spannungen geführt.
- als Funkmodul wird das [eByte E07-868MS10 Sendemodul](https://www.rcscomponents.kiev.ua/datasheets/e07-868ms10_usermanual_en_v1_20.pdf) verwendet
    - hier ein [Vergleich](https://nurazur.wordpress.com/2020/10/06/tino-mit-cc1101-funkmodul-link-budget/) des [eByte E07-868MS10 Sendemodul](https://www.rcscomponents.kiev.ua/datasheets/e07-868ms10_usermanual_en_v1_20.pdf) mit dem grünen noname CC1101 868 MHz Funkmodul
    - das schafft Platz für eine 8-polige Stiftleiste J11 zum optionalen Anschluss eines [Waveshare ePaper Moduls (1.54inch (B)](https://www.waveshare.com/product/1.54inch-e-paper-module.htm). Der ATMega1284P hat auch genügend RAM für die Programmierung eines 200 Pixel x 200 Pixel e-Papermoduls.

### Benötigte Bauteile:
* [Tindie Pro Mini XL - v2 - ATmega 1284p](https://www.tindie.com/products/prominimicros/pro-mini-xl-v2-atmega-1284p/)
    - mit deutlich mehr Programm-, RAM- und EEPROM-Speicher als Arduino Pro Mini: 128kByte Programmspeicher, 16kByte SRAM, 4kByte EEPROM
* oder alternativ ein Arduino Pro Mini
* [eByte E07-868MS10 Sendemodul 868MHz](https://www.elecbee.com/de-24376-E07-868MS10-CC1101-Stamp-Hole-Antenna-IoT-UHF-1000m-868MHz-SPI-10dBm-Wireless-Transmitter-Transceiver-RF-Module) oder alternativ das [Elecbee CC1101 Wireless-Modul 868 MHz](https://www.elecbee.com/de-24507-CC1101-Wireless-Module-868MHz-Digital-Transmission-Receiving-Industrial-grade-RF-Communication-Transparent-Transmission) Modul
* e-Paper [Waveshare ePaper Moduls (1.54inch (B)](https://www.waveshare.com/product/1.54inch-e-paper-module.htm) (optional, Anschluss an J11)
* 2x Keystone 2460 Batteriehalter 1xAA (oder Goobay 1xAA Batteriehalter)
* 1x LEDs 3mm Low Current (Pin 4)
* 1x 470Ohm Widerstände (Vorwiderstand für LED)
* Elko 10uF 1,5mm
* Mikroschalter 6*6mm
* Stiftleiste RM 1.27mm 1x8 Pin (für eByte E07-868MS10 Sendemodul 868MHz)
* Stiftleiste RM 2.0mm 1x8 Pin (für e-Paper [Waveshare ePaper Moduls (1.54inch (B)](https://www.waveshare.com/product/1.54inch-e-paper-module.htm))
* Stiftleisten RM 2,54mm (nach Bedarf)
* IRLU024N (optional für Verpolungsschutz)
* 100KOhm Widerstand (optional für Verpolungsschutz)
* 2x 10KOhm Widerstand (Pullup für I2C)
* Versorgung durch NiMH Akkumulatoren und Aufwärtswandler ist vorbereitet (JP2)


### Aufbauhinweise

- es wird dringend angeraten, vor dem Einbau der I2C Sensoren durchzuführen
    + einen Frequenztest. Hier der Frequenztest für einen [Tindie Pro Mini XL - v2 - ATmega 1284p](https://www.tindie.com/products/prominimicros/pro-mini-xl-v2-atmega-1284p/) : [FreqTest_1284P.ino](../Supplements/FreqTest_1284P/FreqTest_1284P.ino)
    + einen Leckstromtest. Hier der Leckstromtest für einen [Tindie Pro Mini XL - v2 - ATmega 1284p](https://www.tindie.com/products/prominimicros/pro-mini-xl-v2-atmega-1284p/) : [SleepTest_1284P.ino](../Supplements/SleepTest_1284P/SleepTest_1284P.ino)
        + der Leckstrom im Sleepmode sollte < 10 uA betragen
    + für einen Arduino Pro Mini die entsprechenden Standardsketche für den Frequenztest und den SleepTest verwenden.
    + am besten diese beiden Tests in einem Testaufbau mit Stecksockeln vor dem Einlöten durchführen
    
### Passende AsksinPP Projekte von FUEL4EP
- Arduino Pro Mini ATMega328P basierend:
    + [HB-UNI-Sensor1-THPD-BME280](https://github.com/FUEL4EP/HomeAutomation/tree/master/AsksinPP_developments/sketches/HB-UNI-Sensor1-THPD-BME280)
    + [HB-UNI-Sensor1-TH-SHT75](https://github.com/FUEL4EP/HomeAutomation/tree/master/AsksinPP_developments/sketches/HB-UNI-Sensor1-TH-SHT75)
     + [HB-UNI-Sensor1-CO2_SCD30](https://github.com/FUEL4EP/HomeAutomation/tree/master/AsksinPP_developments/sketches/HB-UNI-Sensor1-CO2_SCD30)
- [Tindie Pro Mini XL - v2 - ATmega 1284p](https://www.tindie.com/products/prominimicros/pro-mini-xl-v2-atmega-1284p/) basierend:
     + [HB-UNI-Sensor1-AQ-BME680](https://github.com/FUEL4EP/HomeAutomation/tree/master/AsksinPP_developments/sketches/HB-UNI-Sensor1-AQ-BME680)
    + [HB-UNI-Sensor1-AQ-BME680_KF](https://github.com/FUEL4EP/HomeAutomation/tree/master/AsksinPP_developments/sketches/HB-UNI-Sensor1-AQ-BME680_KF)
    + [HB-UNI-Sensor1-AQ-BME680_KF_DEBUG](https://github.com/FUEL4EP/HomeAutomation/tree/master/AsksinPP_developments/sketches/HB-UNI-Sensor1-AQ-BME680_KF_DEBUG)
    + [HB-UNI-Sensor1-RAD-AL53](https://github.com/FUEL4EP/HomeAutomation/tree/master/AsksinPP_developments/sketches/HB-UNI-Sensor1-RAD-AL53)
    + [HB-UNI-Sensor1-THPD-SHT85](https://github.com/FUEL4EP/HomeAutomation/tree/master/AsksinPP_developments/sketches/HB-UNI-Sensor1-THPD-SHT85)

### Passendes Gehäuse
* Camdenboss CBRS01SWH und CBRS01VWH
* oder [Gehäuse HB-UNI-SEN-BATT snap in V2 von wpatrick](https://www.thingiverse.com/thing:3540481) für 3D-Druck
* bei Einbau eines e-Paper Moduls muss das Gehäuseoberteil angepasst werden (noch zu tun)

### Hinweise
* Achtung: Kein Überspannungschutz vorhanden
* der Verpolungsschutz ist optional. Dafür Bauteil Q1 (IRLU024N) und R3 (100KOhm Widerstand) bestücken. Alternativ JP1 auf der Platinenrückseite brücken um ohne Verpolungsschutz zu arbeiten.

### Abkündigung des Ebyte Sendemoduls eByte E07-868MS10
- EByte hat die Produktion des Sendemoduls eByte E07-868MS10 abgekündigt. Inzwischen ist das Sendemodul eByte E07-868MS10 auf dem Markt kaum noch erhältlich.
- der Nachfolger ist das Modul EByte E07-900M10S 
- dafür wird die neu entwickelte Platine [HB-UNI-SEN-BATT_E07-900M10S_FUEL4EP](https://github.com/FUEL4EP/HomeAutomation/tree/master/AsksinPP_developments/PCBs/HB-UNI-SEN-BATT_E07-900M10S_FUEL4EP) angeboten.


## Bestellen von Platinen

- die Platine kann entweder direkt bei JLCPCB bestellt werden
- oder per PN bei [FUEL4EP](https://homematic-forum.de/forum/ucp.php?i=pm&mode=compose&u=20685) im Homematic Forum nachfragen. Aus der Bestellung der Prototypen sind noch wenige Platinen kostengünstig abzugeben

## passende DIY AsksinPP Funkmodule 868.3 MHz für die Fertigung bei JLCPCB
- als DIY-Ersatz für das eByte E07 868M10S Funkmodul stehen zur Verfügung:
	-  [CC1101_868MHz_RF_Modul_FUEL4EP](https://github.com/FUEL4EP/HomeAutomation/tree/master/AsksinPP_developments/PCBs/CC1101_868MHz_RF_Modul_FUEL4EP) mit Drahtantenne
	- [CC1101_868MHz_UFL_RF_Modul_FUEL4EP](https://github.com/FUEL4EP/HomeAutomation/tree/master/AsksinPP_developments/PCBs/CC1101_868MHz_UFL_RF_Modul_FUEL4EP) mit U.FL Antennenbuchse
- alternativ für das Funkmodul eByte E07 868M10S kann der Adapter [Adapter_EByte_E07_900MM10S_to_Ebyte_E07_868MS10_FUEL4EP](https://github.com/FUEL4EP/HomeAutomation/tree/master/AsksinPP_developments/PCBs/Adapter_EByte_E07_900MM10S_to_Ebyte_E07_868MS10_FUEL4EP) bestückt mit einem Funkmodul eByte E07 900MM10S verwendet werden 
	
## passende DIY I2C-Sensormodule für die Fertigung bei JLCPCB
- als DIY-Ersatz für kommerzielle Sensormodule stehen folgende passende Adapterplatinen zur Verfügung:
	- [I2C_Module_BME280_FUEL4EP](https://github.com/FUEL4EP/HomeAutomation/tree/master/AsksinPP_developments/PCBs/I2C_Module_BME280_FUEL4EP)
	- [I2C_Module_BME680_FUEL4EP](https://github.com/FUEL4EP/HomeAutomation/tree/master/AsksinPP_developments/PCBs/I2C_Module_BME680_FUEL4EP)
	- [I2C_Module_SHT85_BME280_FUEL4EP](https://github.com/FUEL4EP/HomeAutomation/tree/master/AsksinPP_developments/PCBs/I2C_Module_SHT85_BME280_FUEL4EP)
	- [I2C_Module_SHT45_BME280_FUEL4EP](https://github.com/FUEL4EP/HomeAutomation/tree/master/AsksinPP_developments/PCBs/I2C_Module_SHT45_BME280_FUEL4EP)
	- [4Bit_I2C_Port_Expander_Module_FUEL4EP](https://github.com/FUEL4EP/HomeAutomation/tree/master/AsksinPP_developments/PCBs/4Bit_I2C_Port_Expander_Module_FUEL4EP)


## Versionen
- KiCad Schaltplan-Editor Version: 8.0.5-8.0.5-0~ubuntu22.04.1, release build unter Kubuntu22.04.1
- KiCad Leiterplatteneditor Version: 8.0.5-8.0.5-0~ubuntu22.04.1, release build unter Kubuntu22.04.1 1.
- PCB Version 1.3 (Migration auf KiCAD 8 am 30. Sep 2024)
- Schematics  1.2 (Migration auf KiCAD 8 am 30. Sep 2024)

## Disclaimer

-   die Nutzung der hier veröffentlichten Inhalte erfolgt vollständig auf eigenes Risiko und ohne jede Gewähr.

## Lizenz 

**Creative Commons BY-NC-SA**<br>
Give Credit, NonCommercial, ShareAlike

<a rel="license" href="http://creativecommons.org/licenses/by-nc-sa/4.0/"><img alt="Creative Commons License" style="border-width:0" src="https://i.creativecommons.org/l/by-nc-sa/4.0/88x31.png" /></a><br />This work is licensed under a <a rel="license" href="http://creativecommons.org/licenses/by-nc-sa/4.0/">Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License</a>.


-EOF
	

