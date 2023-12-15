# HB_TH_Sensor_SHT45_AllInOne_FUEL4EP [![License: CC BY-NC-SA 4.0](https://img.shields.io/badge/License-CC%20BY--NC--SA%204.0-lightgrey.svg)](https://creativecommons.org/licenses/by-nc-sa/4.0/)[![Hits](https://hits.seeyoufarm.com/api/count/incr/badge.svg?url=https%3A%2F%2Fgithub.com%2FFUEL4EP%2FHomeAutomation%2Ftree%2Fmaster%2FAsksinPP_developments%2FPCBs%2FHB_TH_Sensor_SHTXX_AllInOne_FUEL4EP&count_bg=%2379C83D&title_bg=%23555555&icon=&icon_color=%23E7E7E7&title=hits&edge_flat=false)](https://hits.seeyoufarm.com) <a href='https://ko-fi.com/FUEL4EP' target='_blank'><img height='20' style='border:0px;height:20px;' src='https://cdn.ko-fi.com/cdn/kofi1.png?v=2' border='0' alt='Buy Me a Coffee at ko-fi.com' /></a>

**Platine für hochgenauen und kleinen DIY AsksinPP TH Sensor**
- integrierter hochgenauer [Sensirion SHT45](https://sensirion.com/products/catalog/SHT45-AD1F) SMD TH-Sensor (±1.0% RH / ±0.1°C digitaler Luftfeuchte- und Temperatursensor ) mit PTFE-Membran
- kann bis auf den Batteriehalter vollständig bei JLCPCB assembliert werden
- kleine Platine 75 mm x 25 mm
- Versorgung mit 1x AA-Batterie oder 1x NiMH-Akkumulator
- TPS61221 Hochsetzwandler 1,2V .. 1,5V auf 3,3V
- [integrierte 868 MHz PCB Antenne](https://www.ti.com/lit/an/swra416/swra416.pdf)
- 10 mm x 10 mm kleines Sendemodul [eByte E07 900MM10S](https://datasheet.lcsc.com/lcsc/2305061757_Chengdu-Ebyte-Elec-Tech-E07-900MM10S_C5844212.pdf)
- SMD TQFP-44 [ATMega1284P](https://ww1.microchip.com/downloads/en/DeviceDoc/doc8059.pdf) mit 128kByte Programmspeicher, 16kByte SRAM, 4kByte EEPROM
	+ ohne Quarz, Betrieb mit 8 MHz interner RC-Oszillator
	+ erlaubt Speicherung der Temperaturwerte über 7 Tage im RAM für die Bildung von gleitenden Mittelwerten 
- kleines 10mmx10mm [eByte E07-900MM10S Sendemodul](https://ebyteiot.com/products/ebyte-e07-900MM10Sti-cc1101-wireless-rf-module-868mhz-915mhz-ipex-stamp?_pos=2&_sid=bbf1d47dc&_ss=r)
- Programmierung und Serial Monitor mit 2,54 mm Pogo-Pin-Adapter oder Stiftleisten
- alle SMD Bausteine sind mit dem JLCPCB SMT Bestückungsservice kostengünstig und qualitativ hochwertig bestückbar, das ist also auch für Grobmotoriker wie mich geeignet :-)

![pic](PNGs/HB_TH_Sensor_SHT45_AllInOne_FUEL4EP_top_view.png)

![pic](PNGs/HB_TH_Sensor_SHT45_AllInOne_FUEL4EP_PCB_top_3D.png)
![pic](PNGs/HB_TH_Sensor_SHT45_AllInOne_FUEL4EP_PCB_top_3D_back.png)




## Credits

- mein besonderer Dank geht an papa für [AsksinPP](https://github.com/pa-pa/AskSinPP) und [Jérôme jp112sdl](https://github.com/jp112sdl) für die stete Unterstützung.

## Platine wurde noch nicht bei JLCPCB gefertigt und auch noch nicht validiert

- entwickelt mit KiCAD 6.0.11
- zur Zeit eine Vorabfreigabe
- die Platine wurde noch nicht bei JLCPCB gefertigt und ist daher noch nicht validiert


## Eigenschaften

- kompletter Datensatz für KiCAD 6 auf Github verfügbar:
    + 'git clone https://github.com/FUEL4EP/HomeAutomation.git' oder als [ZIP-Datei](https://github.com/FUEL4EP/HomeAutomation/archive/refs/heads/master.zip)
    + die Platinendaten sind dann unter 'HomeAutomation/AsksinPP_developments/PCBs/HB_TH_Sensor_SHT45_AllInOne_FUEL4EP' direkt mit KiCAD aufrufbar
- neue Eigenschaften:
    + SMD Bestückung, wo möglich
    + alle SMD Bausteine sind mit dem JLCPCB SMT Bestückungsservice kostengünstig und qualitativ hochwertig bestückbar
    + JLCPCB BOM and CPL Dateien für SMT Service werden [bereitgestellt](./jlcpcb/production_files)

### Benötigte Bauteile:

- bei Anspruchnahme des SMT Bestückungsservice von JLCPCB ist nur noch ein Durchsteckbauteil von Hand zu verlöten:
	* 1x Keystone 2460 Batteriehalter 1xAA


### Aufbauhinweise

- es wird dringend angeraten, als ersten Schritt der Inbetriebnahme einen Frequenztest durchzuführen.
    
### Passende AsksinPP Projekte von FUEL4EP
   + [HB-UNI-Sensor1-THPD-SHT45](https://github.com/FUEL4EP/HomeAutomation/tree/master/AsksinPP_developments/sketches/HB-UNI-Sensor1-THPD-SHT45) (wird bald veröffentlicht werden)
    

### Anschluss eines FTDI Debuggers und/oder ISP-Programmiergeräts

- Anschluss über 6-Pin 2,54 mm Pogo-Pin-Adapter oder eingelötete 6-Pin Stiftleiste

### Passendes Gehäuse
- in Arbeit, wird später hier bereitgestellt

### Hinweise
* Achtung: Kein Überspannungsschutz oder Verpolungsschutz vorhanden
* bei der Nutzung des JLCPCB SMT Bestückungsservice ist darauf zu achten, dass **alle** benötigten SMD Bauteile als im Lager **verfügbar** angezeigt werden. Sonst bitte **NICHT** bestellen!

### KiCAD Plugin
- für die Erzeugung der JLCPCB Produktionsdaten wurde das Plugin [KiCAD JLCPCB tools](https://github.com/bouni/kicad-jlcpcb-tools) verwendet.


## Bestellen von Platinen

- die Platine kann direkt bei JLCPCB bestellt und dort auch assembliert werden
- die notwendigen Produktionsdaten stehen [hier](./jlcpcb/production_files/)
- die folgenden Bauteile müssen [vorab bei JLCPCB bestellt werden](https://jlcpcb.com/help/article/53-what-is-jlcpcb-parts-pre-order-service):
	+ TI TPS61221DCK (zur Zeit sehr teuer)
	+ Sensirion SHT45-AD1F-R2
	+ eByte E07-900MM10S
- **die Platine ist noch nicht durch einen Prototypen evaluiert!**

## Disclaimer

-   die Nutzung der hier veröffentlichten Inhalte erfolgt vollständig auf eigenes Risiko und ohne jede Gewähr.

## Versionsverlauf

-   V1.0 15. Dez 2023: Initiale Vorabfreigabe

- 	KiCad Schaltplan-Editor   Version: 6.0.11-2627ca5db0 unter Kubuntu22.04.1
- 	KiCad Leiterplatteneditor Version: 6.0.11-2627ca5db0 unter Kubuntu22.04.1
- 	PCB Version 1.0
- 	Schematics  1.0

## Lizenz 

**Creative Commons BY-NC-SA**<br>
Give Credit, NonCommercial, ShareAlike

<a rel="license" href="http://creativecommons.org/licenses/by-nc-sa/4.0/"><img alt="Creative Commons License" style="border-width:0" src="https://i.creativecommons.org/l/by-nc-sa/4.0/88x31.png" /></a><br />This work is licensed under a <a rel="license" href="http://creativecommons.org/licenses/by-nc-sa/4.0/">Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License</a>.


-EOF
	

