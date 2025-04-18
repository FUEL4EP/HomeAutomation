# HB_TH_Sensor_SHT45_AllInOne_FUEL4EP [![License: CC BY-NC-SA 4.0](https://img.shields.io/badge/License-CC%20BY--NC--SA%204.0-lightgrey.svg)](https://creativecommons.org/licenses/by-nc-sa/4.0/) <a href='https://ko-fi.com/FUEL4EP' target='_blank'><img height='20' style='border:0px;height:20px;' src='https://cdn.ko-fi.com/cdn/kofi1.png?v=2' border='0' alt='Buy Me a Coffee at ko-fi.com' /></a>


**Platine für hochgenauen und kleinen DIY AsksinPP TH Sensor**
- integrierter hochgenauer [Sensirion SHT45-AD1B-R2](https://sensirion.com/products/catalog/SHT45) SMD TH-Sensor (±1.0% RH / ±0.1°C digitaler Luftfeuchte- und Temperatursensor )
- kann bis auf den Batteriehalter vollständig bei JLCPCB assembliert werden
- kleine Platine 75 mm x 25 mm
- **Platinendicke: 0,8 mm (bitte nicht ändern)**
- Versorgung mit 1x AA-Alkaline-Batterie (1,5V) oder 1x NiMH-Akkumulator (1,2V)
- TPS61221 Hochsetzwandler 1,2V .. 1,5V auf 3,3V
- [integrierte 868 MHz PCB Antenne](https://www.ti.com/lit/an/swra416/swra416.pdf)
- 10 mm x 10 mm kleines Sendemodul [eByte E07 900MM10S](https://datasheet.lcsc.com/lcsc/2305061757_Chengdu-Ebyte-Elec-Tech-E07-900MM10S_C5844212.pdf)
- SMD TQFP-44 [ATMega1284P](https://ww1.microchip.com/downloads/en/DeviceDoc/doc8059.pdf) mit 128kByte Programmspeicher, 16kByte SRAM, 4kByte EEPROM
	+ ohne Quarz, Betrieb mit 8 MHz interner RC-Oszillator
	+ erlaubt Speicherung der Temperaturwerte über 7 Tage im RAM für die Bildung von gleitenden Mittelwerten 
- kleines 10mmx10mm [eByte E07-900MM10S Sendemodul](https://ebyteiot.com/products/ebyte-e07-900MM10Sti-cc1101-wireless-rf-module-868mhz-915mhz-ipex-stamp?_pos=2&_sid=bbf1d47dc&_ss=r)
- Programmierung und Serial Monitor mit 2,54 mm Pogo-Pin-Adapter oder Stiftleisten
- alle SMD Bausteine sind mit dem JLCPCB SMT Bestückungsservice kostengünstig und qualitativ hochwertig bestückbar
- die JLCPCB Assemblierung des Funkmoduls eByte E07-900MM10S geht nur als sehr teures Standard PCBA, daher wird eine Verlötung von Hand empfohlen

![pic](PNGs/HB_TH_Sensor_SHT45_AllInOne_FUEL4EP_top_view.png)

![pic](PNGs/HB_TH_Sensor_SHT45_AllInOne_FUEL4EP_PCB_top_3D.png)
![pic](PNGs/HB_TH_Sensor_SHT45_AllInOne_FUEL4EP_PCB_back_3D.png)
- Bilder der bei JLCPCB gefertigten und assemblierten Prototypplatine V1.3
![pic](Pictures_of_JLCPCB_prototypes/HB_TH_Sensor_SHT45_AllInOne_FUEL4EP_PCB_top_JLCPCB_assembled_prototype_V1.3.png)
![pic](Pictures_of_JLCPCB_prototypes/HB_TH_Sensor_SHT45_AllInOne_FUEL4EP_PCB_bottom_JLCPCB_assembled_prototype_V1.3.png)


## Credits

- mein besonderer Dank geht an papa für [AsksinPP](https://github.com/pa-pa/AskSinPP) und [Jérôme jp112sdl](https://github.com/jp112sdl) für die stete Unterstützung.

## Platine V1.3 wurde bei JLCPCB gefertigt und erfolgreich validiert
- entwickelt mit KiCAD 6.0.11
- V1.3 hatte noch falsche I2C-Pullup-Widerstände, die ausgelötet werden mussten
- in der aktuellen Version V1.4 der Platine ist dieser Fehler behoben


## Eigenschaften

- kompletter Datensatz für KiCAD 8 auf Github verfügbar:
    + 'git clone https://github.com/FUEL4EP/HomeAutomation.git' oder als [ZIP-Datei](https://github.com/FUEL4EP/HomeAutomation/archive/refs/heads/master.zip)
    + die Platinendaten sind dann unter 'HomeAutomation/AsksinPP_developments/PCBs/HB_TH_Sensor_SHT45_AllInOne_FUEL4EP' direkt mit KiCAD aufrufbar
- neue Eigenschaften:
    + SMD Bestückung, wo möglich
    + fast alle SMD Bausteine sind mit dem JLCPCB SMT Bestückungsservice kostengünstig und qualitativ hochwertig bestückbar
    + JLCPCB BOM and CPL Dateien für SMT Service werden [bereitgestellt](./jlcpcb/production_files)

### Benötigte Bauteile:

- bei Anspruchsnahme des SMT Bestückungsservice von JLCPCB sind nur noch zwei Bauteile von Hand zu verlöten:
	* 1x Keystone 2460 Batteriehalter 1xAA
	- die JLCPCB Assemblierung des Funkmoduls eByte E07-900MM10S geht nur als sehr teures Standard PCBA, daher wird eine Verlötung von Hand empfohlen. Das geht mit einer feinen Lötspitze noch ganz gut.


### Aufbauhinweise

- es wird dringend angeraten, als ersten Schritt der Inbetriebnahme einen Frequenztest durchzuführen. Vorher bitte die Fuses mit avrdude setzen.

## Setzen der Fuses des ATMega1284P Mikrokontrollers

- **nie die Fuses auf einen externen Quarzoszillator / Resonator umschalten!**
- ein passender avrdude Skript zum Lesen und korrekten Setzen der Fuses ist [hier](./avrdude)

## Prüfwerkzeuge

- ein Frequenztest ist [hier](./FreqTest_1284P/)
- ein Test des SHT45 Sensors ist [hier](./SHT45_Sensor_test)
- ein Test des SHT45 Sensors ohne AsksinPP ist [hier](./SHT45_Sensor_test/SHT45_Sensor_test.ino)
    
### Passendes AsksinPP Projekt von FUEL4EP
   + [HB-UNI-Sensor1-THD-SHT45](https://github.com/FUEL4EP/HomeAutomation/tree/master/AsksinPP_developments/sketches/HB-UNI-Sensor1-THD-SHT45)
    

### Anschluss eines FTDI Debuggers und/oder ISP-Programmiergeräts

- Anschluss über 6-Pin 2,54 mm Pogo-Pin-Adapter oder eingelötete 6-Pin Stiftleiste, bitte die Beschriftung beachten und vor dem Anlegen eines Pogo-Pin-Adapters querprüfen!
- den ISP-Programmierer auf 3,3V Programmierspannung einstellen.
- zuerst immer die Fuses mit avrdude setzen!
- Bild des Pogo-Pin_Adapters für den FTDI Debugger:
![pic](PNGs/FTDI_debugger_with_pogo_pin_adapter.png)
- Bild des Pogo-Pin_Adapters für den Diamex ISP Programmieradapter:
![pic](PNGs/ISP_programmer_with_pogo_pin_adapter.png)


### Passendes Gehäuse
![pic](./3D_Printer/case/HB_TH_Sensor_SHT45_AllInOne_FUEL4EP.png)

- die Konstruktionsdaten sind [hier](./3D_Printer/case/) im STL- und OpenSCAD-Format zu finden
	+ gegebenenfalls ist in der OpenSCAD-Datei [HB_TH_Sensor_SHT45_AllInOne_FUEL4EP.scad](./3D_Printer/case/HB_TH_Sensor_SHT45_AllInOne_FUEL4EP.scad) die Variable 'gap' an die Toleranzen des 3D-Druckers anzupassen

### Hinweise
* Achtung: Kein Überspannungsschutz oder Verpolungsschutz vorhanden
* bei der Nutzung des JLCPCB SMT Bestückungsservice ist darauf zu achten, dass **alle** benötigten SMD Bauteile als im Lager **verfügbar** angezeigt werden. Sonst bitte **NICHT** bestellen!

### KiCAD Plugin
- für die Erzeugung der JLCPCB Produktionsdaten wurde das Plugin [KiCAD JLCPCB tools](https://github.com/bouni/kicad-jlcpcb-tools) verwendet.


## Bestellen von Platinen

- die Platine kann direkt bei JLCPCB bestellt und dort auch assembliert werden
  + **als Platinendicke unbedingt bei der Bestellung angeben: 0,8 mm (bitte nicht ändern, sonst geht die PCB-Antenne nicht gut)**
- die notwendigen Produktionsdaten stehen [hier](./jlcpcb/production_files/)
- die folgenden Bauteile müssen [vorab bei JLCPCB bestellt werden](https://jlcpcb.com/help/article/53-what-is-jlcpcb-parts-pre-order-service):
	+ [TI TPS61221DCKT](https://jlcpcb.com/parts/componentSearch?searchTxt=TPS61221) (zur Zeit teuer, ist aber meist billiger als angegeben)
	+ Sensirion SHT45-AD1B-R2
	+ eByte E07-900MM10S
	+ Murata LQH3NPN4R7MMEL 4.7 uH SMD Induktivität
- die JLCPCB Assemblierung des Funkmoduls eByte E07-900MM10S geht nur als das sehr teure Standard PCBA, daher wird eine Verlötung von Hand empfohlen
- **die Platine V1.3  wurde durch einen Prototypen erfolgreich validiert**

## Sammelbestellung

- wer Interesse an einer gemeinsamen Bestellung von Platinen und/oder Gehäusen bei JLCPCB hat, fragt bitte per per PN bei bei [FUEL4EP](https://homematic-forum.de/forum/ucp.php?i=pm&mode=compose&u=20685) an.
- wenn genügend Interesse (>= 20 bestückte Platinen und/oder 3D gedruckten Gehäusen) zustande kommt, biete ich an, Platinen  und Gehäuse bei JLCPCB zu bestellen und gegen Vorkasse per PayPal zum Selbstkostenpreis + Versandkosten innerhalb Deutschland + 3 Euro Aufwandspauschale pro Versand weiterzuverkaufen. Vor einer Bestellung wird ein Angebot bei JLCPCB eingeholt und an die Interessenten zur Zustimmung verteilt.
- je mehr sich an einer Sammelbestellung beteiligen, desto geringer werden die einmaligen Kosten wie Rüstkosten, Versand, etc.

## Disclaimer

-   die Nutzung der hier veröffentlichten Inhalte erfolgt vollständig auf eigenes Risiko und ohne jede Gewähr.

## Versionsverlauf

-   V1.0 15. Dez 2023: Initiale Vorabfreigabe
-	S V1.1 15. Dez 2023: SHT45-AD1B-R2 statt SHT45-AD1F, da JLCPCB SHT45-AD1F
nicht mehr assembliert
-	S V1.2	 08. Jan 2024: Korrektur von falschen LCSC-Teilenummern für R1, D1 und R4
-	L V1.2 12. Jan 2024: Verbesserung des PCB-Layouts für den Schaltregler
-	S V1.3 13. Jan 2024: Korrektur der falschen LCSC-Teilenummer für Schaltregler TPS61221
-	S V1.4 21. Jan 2024: Änderung der LCSC-Nummern für bessere Verfügbarkeit, Änderung der Steckerleistenbelegung
-	L V1.3 21. Jan 2024: Änderung der Ausrichtung von Steckerleisten, zusätzliche Bestückungsdrucke
-	S V1.5 08. Feb 2024: Fix der I2C Pull-Up Widerstände
-	L V1.4 08. Feb 2024: Fix der I2C Pull-Up Widerstände und Beschriftung
-	S V1.6 27. Sep 2024: Migration zu KiCAD 8
-	L V1.5 27. Sep 2024: Migration zu KiCAD 8, Verbesserung der Handlötbarkeit des RF-Moduls durch Verschiebung von Entkoppelkondensatoren
- 	KiCad Schaltplan-Editor   Version: 8.0.5-8.0.5-0~ubuntu22.04.1, release build unter Kubuntu22.04.1
- 	KiCad Leiterplatteneditor Version: 8.0.5-8.0.5-0~ubuntu22.04.1, release build unter Kubuntu22.04.1
- 	PCB Version 1.5
- 	Schematics  1.6

## Lizenz 

**Creative Commons BY-NC-SA**<br>
Give Credit, NonCommercial, ShareAlike

<a rel="license" href="http://creativecommons.org/licenses/by-nc-sa/4.0/"><img alt="Creative Commons License" style="border-width:0" src="https://i.creativecommons.org/l/by-nc-sa/4.0/88x31.png" /></a><br />This work is licensed under a <a rel="license" href="http://creativecommons.org/licenses/by-nc-sa/4.0/">Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License</a>.


-EOF
	

