# HB-UNI-SEN-BATT_ATMega1284P_E07-868MS10_FUEL4EP [![License: CC BY-NC-SA 4.0](https://img.shields.io/badge/License-CC%20BY--NC--SA%204.0-lightgrey.svg)](https://creativecommons.org/licenses/by-nc-sa/4.0/) <a href='https://ko-fi.com/FUEL4EP' target='_blank'><img height='20' style='border:0px;height:20px;' src='https://cdn.ko-fi.com/cdn/kofi1.png?v=2' border='0' alt='Buy Me a Coffee at ko-fi.com' /></a>

**Platine für DIY AsksinPP für**
- eingebauter SMD TQFP-44 [ATMega1284P](https://ww1.microchip.com/downloads/en/DeviceDoc/doc8059.pdf) mit 128kByte Programmspeicher, 16kByte SRAM, 4kByte EEPROM
- [eByte E07-868MS10 Sendemodul](https://www.rcscomponents.kiev.ua/datasheets/e07-868ms10_usermanual_en_v1_20.pdf)
- Bestückungsoption für [Waveshare ePaper Modul (1.54inch (B)](https://www.waveshare.com/product/1.54inch-e-paper-module.htm)
- Bestückungsoption für [TPS61221 Aufwärtswandler](https://www.ti.com/lit/ds/symlink/tps61221.pdf?ts=1634546965861&ref_url=https%253A%252F%252Fwww.ti.com%252Fproduct%252FTPS61221) bei NiMH Akkumulatorbetrieb
- alle SMD Bausteine sind mit dem JLCPCB SMT Bestückungsservice kostengünstig und qualitativ hochwertig bestückbar, das ist also auch für Grobmotoriker wie mich geeignet :-)
- die Bestückungsoption ePaper wurde noch nicht validiert!

![pic](PNGs/HB-UNI-SEN-BATT_ATMega1284P_E07-868MS10_FUEL4EP_top_view.png)

![pic](PNGs/HB-UNI-SEN-BATT_ATMega1284P_E07-868MS10_FUEL4EP_PCB_top_3D_with_epaper_option.png)

## Hinweis

- da das Funkmodul eByte E07-868MS10 (fast) nicht mehr  käuflich zu erhalten ist, wurde die Platine [HB-UNI-SEN-BATT_ATMega1284P_E07-900MM10S_FUEL4EP](https://github.com/FUEL4EP/HomeAutomation/tree/master/AsksinPP_developments/PCBs/HB-UNI-SEN-BATT_ATMega1284P_E07-900MM10S_FUEL4EP) basierend auf dem gut erhältlichen Funkmodul eByte E07-900MM10S entwickelt.

## Credits

- diese Platine ist von Alexander Reinerts Platine [HB-UNI-SEN-BATT](https://github.com/alexreinert/PCB/tree/master/HB-UNI-SEN-BATT) abgeleitet.
- vielen Dank an Alexander Reinert für die Vorarbeit.

## Platine wurde nach Fertigung bei JLCPCB erfolgreich validiert

- entwickelt mit KiCAD 6.0.11
- die Platine wurde bei JLCPCB gefertigt und mit den SMD Bauteilen bestückt
![pic](Pictures_of_JLCPCB_prototypes/HB-UNI-SEN-BATT_ATMega1284P_E07-868MS10_FUEL4EP_PCB.png); die nicht SMD Bauteile müssen von Hand verlötet werden
- die gefertigte Platine wurde erfolgreich **ohne** Aufwärtswandler mit dem Sketch [HB-UNI-SENSOR1-AQ-BME680_KF](https://github.com/FUEL4EP/HomeAutomation/tree/master/AsksinPP_developments/sketches/HB-UNI-Sensor1-AQ-BME680_KF) validiert:
![pic](Pictures_of_JLCPCB_prototypes/HB-UNI-Sensor1-AQ-BME680_KF_build_with_HB-UNI-SEN-BATT_ATMega1284P_E07-868MS10_FUEL4EP_PCB.png)
- **Update 21. Aug 2023:** Die gefertigte Platine wurde erfolgreich **mit** dem Aufwärtswandler TPS61221 validiert:
![pic](Pictures_of_JLCPCB_prototypes/HB-UNI-SEN-BATT_ATMega1284P_E07-868MS10_FUEL4EP_PCB_with_boost_converterand_dummy_AA_battery.png)
	+ die Platine wurde bei JLCPCB gefertigt und einschließlich der Induktivität mit den SMD Bauteilen bestückt 
	+ im Bild wurde ein AA-Akkumulator wurde durch eine AA-Dummy- Batterie ersetzt
	+ Betrieb ist mit einer oder zwei AA Akkumulatorbatterien möglich
	+ die Ausgangsspannung des Aufwärtswandlers beträgt 3.3 Volt
	+ der Jumper JP2 ist bei Betrieb mit einem Aufwärtswandler zu durchtrennen
	+ der Jumper JP3 ist ist bei Betrieb mit einem Aufwärtswandler mit einer Lötbrücke zu versehen


## Eigenschaften

- abgeleitet aus [HB-UNI-SEN-BATT](https://github.com/alexreinert/PCB/tree/master/HB-UNI-SEN-BATT)
- kompletter Datensatz für KiCAD 6 auf Github verfügbar:
    + 'git clone https://github.com/FUEL4EP/HomeAutomation.git' oder als [ZIP-Datei](https://github.com/FUEL4EP/HomeAutomation/archive/refs/heads/master.zip)
    + die Platinendaten sind dann unter 'HomeAutomation/AsksinPP_developments/PCBs/HB-UNI-SEN-BATT_ATMega1284P_E07-868MS10_FUEL4EP' direkt mit KiCAD aufrufbar
- neue Eigenschaften:
    + SMD Bestückung, wo möglich
    + SMD TQFP-44 [ATMega1284P](https://ww1.microchip.com/downloads/en/DeviceDoc/doc8059.pdf) mit 128kByte Programmspeicher, 16kByte SRAM, 4kByte EEPROM
    + Bestückungsoption für [Waveshare ePaper Modul (1.54inch (B)](https://www.waveshare.com/product/1.54inch-e-paper-module.htm)
    + Bestückungsoption für [TPS61221 Aufwärtswandler](https://www.ti.com/lit/ds/symlink/tps61221.pdf?ts=1634546965861&ref_url=https%253A%252F%252Fwww.ti.com%252Fproduct%252FTPS61221) bei NiMH Akkumulatorbetrieb
    + alle SMD Bausteine sind mit dem JLCPCB SMT Bestückungsservice kostengünstig und qualitativ hochwertig bestückbar, das ist also auch für Grobmotoriker wie mich gegeignet :-)
    + Stiftleiste J10 mit Versorgungsspannungen zum Gehäuseoberteil oder Akkulademodul
    + Korrektur der Abstandfehlers bei einem Batteriehalter. Beim [HB-UNI-SEN-BATT](https://github.com/alexreinert/PCB/tree/master/HB-UNI-SEN-BATT) hat das schon mal gerne zu thermischen Ausfällen durch mechanische Spannungen geführt.
    + als Funkmodul wird das [eByte E07-868MS10 Sendemodul](https://www.rcscomponents.kiev.ua/datasheets/e07-868ms10_usermanual_en_v1_20.pdf) verwendet. Dieses ist qualitativ viel besser als die üblichen grünen noname 868 MHz Funkmodule.
    + das schafft Platz für eine 8-polige Stiftleiste J11 zum optionalen Anschluss eines [Waveshare ePaper Moduls (1.54inch (B)](https://www.waveshare.com/product/1.54inch-e-paper-module.htm). Der ATMega1284P hat auch genügend RAM für die Programmierung eines 200 Pixel x 200 Pixel e-Papermoduls.
    + JLCPCB BOM and CPL Dateien für SMT Service werden [bereitgestellt](./jlcpcb/production_files)

### Benötigte Bauteile:

- bedingt durch die SMT Service von JLCPCB sind nur noch wenige Durchsteckbauteile von Hand zu verlöten:
* [eByte E07-868MS10 Sendemodul 868MHz](https://www.elecbee.com/de-24376-E07-868MS10-CC1101-Stamp-Hole-Antenna-IoT-UHF-1000m-868MHz-SPI-10dBm-Wireless-Transmitter-Transceiver-RF-Module) oder alternativ das [Elecbee CC1101 Wireless-Modul 868 MHz](https://www.elecbee.com/de-24507-CC1101-Wireless-Module-868MHz-Digital-Transmission-Receiving-Industrial-grade-RF-Communication-Transparent-Transmission) Modul
* e-Paper [Waveshare ePaper Moduls (1.54inch (B)](https://www.waveshare.com/product/1.54inch-e-paper-module.htm) (optional, Anschluss an J11)
* 2x Keystone 2460 Batteriehalter 1xAA
* Stiftleiste RM 1.27mm 1x8 Pin (für eByte E07-868MS10 Sendemodul 868MHz)
* Stiftleiste RM 2mm 1x8 Pin (für ePaper)
* Stiftleisten RM 2,54mm (nach Bedarf)
* IRLU024N (optional für Verpolungsschutz)
* Versorgung durch NiMH Akkumulatoren und Aufwärtswandler ist vorbereitet (JP2)
* Bestückungsoption Aufwärtswandler siehe [Schaltplan](Schematics/HB-UNI-SEN-BATT_ATMega1284P_E07-868MS10_FUEL4EP.pdf), unter KiCAD sind alle Referenzen eingetragen


### Aufbauhinweise

- es wird dringend angeraten, vor dem Einbau der I2C Sensoren durchzuführen
    + einen Frequenztest. Hier der Frequenztest für einen ATmega 1284p : [FreqTest_1284P.ino](../Supplements/FreqTest_1284P/FreqTest_1284P.ino)
    + einen Leckagetest. Hier der Leckagetest für einen ATmega 1284p : [SleepTest_1284P.ino](../Supplements/SleepTest_1284P/SleepTest_1284P.ino)
        + der Leckagestrom im Sleepmode sollte < 10 uA betragen

### Passende AsksinPP Projekte von FUEL4EP
   + [HB-UNI-Sensor1-AQ-BME680](https://github.com/FUEL4EP/HomeAutomation/tree/master/AsksinPP_developments/sketches/HB-UNI-Sensor1-AQ-BME680)
   + [HB-UNI-Sensor1-AQ-BME680_KF](https://github.com/FUEL4EP/HomeAutomation/tree/master/AsksinPP_developments/sketches/HB-UNI-Sensor1-AQ-BME680_KF)
   + [HB-UNI-Sensor1-AQ-BME680_KF_DEBUG](https://github.com/FUEL4EP/HomeAutomation/tree/master/AsksinPP_developments/sketches/HB-UNI-Sensor1-AQ-BME680_KF_DEBUG)
   + [HB-UNI-Sensor1-RAD-AL53](https://github.com/FUEL4EP/HomeAutomation/tree/master/AsksinPP_developments/sketches/HB-UNI-Sensor1-RAD-AL53)
   + [HB-UNI-Sensor1-THPD-SHT85](https://github.com/FUEL4EP/HomeAutomation/tree/master/AsksinPP_developments/sketches/HB-UNI-Sensor1-THPD-SHT85)
   
### Notwendige Änderung der Betriebsspannungsmessung in Sketchen
- da die Platine einen fest verdrahteten Spannungsteiler R7 und R8 für die Betriebsspannung / Akkuspannungsmessung verwendet, muss in Sketchen folgende Anpassung gegebenenfalls durchgeführt werden:

- nur notwendig, wenn die Akkuspannung / Betriebsspannung größer als 3,5 Volt ist ( 2x AA Alkaline Batterie)

- von bisher
```
// 1) Standard: tmBattery, UBatt = Betriebsspannung AVR
#define BAT_SENSOR tmBattery
```
- nach
```
// 2) für StepUp/StepDown: tmBatteryResDiv, sense pin PA0 (=pin 37), activation pin PB3 (=pin 43) , Faktor = Rges/Rlow*1000, z.B. (100kOhm+47kOhm/100kOhm), Faktor (147k)/(100k)*1000 = 1470 (ATMega1284P)
// start with Faktor = 1470
// check in serial monitor the reported V2='accumulator voltage(MCU ADC) (x1000.0)' and compare with the reported V1='accumulator batteries voltage (x1000.0)'
// calculate voltage correction factor V1/V2
// calculate total Faktor = 1470 * V2/V1 = 1470 * 3141 / 3157 = 1463
#define BAT_SENSOR tmBatteryResDiv<A0, PB3, 1463>
```
- zur Berechnung von Faktor in tmBatteryResDiv wird ein Libreoffice Calc Sheet [tmBattery_tmBatteryResDiv_calculator.ods](../../tools/tmBattery_tmBatteryResDiv_calculator.ods) zur Verfügung gestellt
- in der Version 1.0 war R8 = 200kOhm, daher Faktor = 3000 (ohne Kalibrierung)
- in der Version 1.1 ist R8 = 47kOhm, daher Faktor = 1470 (ohne Kalibrierung)

### Anschluss eines FTDI Debuggers

- zum Anschluss eines FTDI Debuggers an die 6-polige Stiftleiste J12 bitte ein [6 Pin Dupontkabel 2.54mm Buchse-Buchse 20 cm](https://www.roboter-bausatz.de/p/6-pin-dupontkabel-2.54mm-buchse-buchse-20-cm) verwenden

### Passendes Gehäuse
* Camdenboss CBRS01SWH und CBRS01VWH
* oder [Gehäuse HB-UNI-SEN-BATT snap in V2 von wpatrick](https://www.thingiverse.com/thing:3540481) für 3D-Druck
* bei Einbau eines e-Paper Moduls muss das Gehäuseoberteil angepasst werden (noch zu tun)

### Hinweise
* Achtung: Kein Überspannungsschutz vorhanden
* der Verpolungsschutz ist optional. Dafür Bauteil Q1 (IRLU024N) und R3 (100KOhm Widerstand) bestücken. Alternativ JP1 auf der Platinenrückseite brücken um ohne Verpolungsschutz zu arbeiten.
* bei der Nutzung des JLCPCB SMT Bestückungsservice ist darauf zu achten, dass **alle** benötigten SMD Bauteile als im Lager **verfügbar** angezeigt werden. Sonst bitte **NICHT** bestellen!

### KiCAD Plugin
- für die Erzeugung der JLCPCB Produktionsdaten wurde das Plugin [KiCAD JLCPCB tools](https://github.com/bouni/kicad-jlcpcb-tools) verwendet.


## Bestellen von Platinen

- die Platine kann entweder direkt bei JLCPCB bestellt werden
- die notwendigen Produktionsdaten stehen [hier](./jlcpcb/production_files/)
- oder per PN bei [FUEL4EP](https://homematic-forum.de/forum/ucp.php?i=pm&mode=compose&u=20685) im Homematic Forum nachfragen. Aus der Bestellung der Prototypen sind noch wenige Platinen kostengünstig abzugeben (Stand Juli 2023).

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


## Disclaimer

-   die Nutzung der hier veröffentlichten Inhalte erfolgt vollständig auf eigenes Risiko und ohne jede Gewähr.

## Versionsverlauf

-    V1.1 10. Jan 2022: R8 auf 47 kOhm geändert, zusätzliche Aufdrucktexte
-    V1.2 15. Jul 2023: Migration der Datenbasen zu KiCAD 6, als Induktivität    	                     für den Aufwärtswandler wird nun LQH3NPN4R7MM0L von
                         Murata Electronics verwendet, die bei JLCPCB bestellbar ist  (eventuell ist eine Vorbestellung notwendig)
-	 V1.3 24. Jul 2023:	KiCAD 6 bedingte Verbesserungen der Datenbasis
-	 V1.4 15. Aug 2023:	Fix of En pin of TPS61221 boost converter (must be tied to VIN)
     S V1.3 17. Feb 2024 Fix der falschen LCSC-Nummer von R7 / Layout bleibt unverändert
     V1.5   26. Sep 2024 Migration der Datenbasen zu KiCAD 8, Fix von LCSC-Nummern (R2, R4, R5 , Fixes für KiCAD 8

- 	KiCad Schaltplan-Editor   Version: 8.0.5-8.0.5-0~ubuntu22.04.1, release build
 unter Kubuntu22.04.1
- 	KiCad Leiterplatteneditor Version: 8.0.5-8.0.5-0~ubuntu22.04.1, release build
 unter Kubuntu22.04.1
- 	PCB Version 1.5
- 	Schematics  1.5

## Lizenz 

**Creative Commons BY-NC-SA**<br>
Give Credit, NonCommercial, ShareAlike

<a rel="license" href="http://creativecommons.org/licenses/by-nc-sa/4.0/"><img alt="Creative Commons License" style="border-width:0" src="https://i.creativecommons.org/l/by-nc-sa/4.0/88x31.png" /></a><br />This work is licensed under a <a rel="license" href="http://creativecommons.org/licenses/by-nc-sa/4.0/">Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License</a>.


-EOF
	

