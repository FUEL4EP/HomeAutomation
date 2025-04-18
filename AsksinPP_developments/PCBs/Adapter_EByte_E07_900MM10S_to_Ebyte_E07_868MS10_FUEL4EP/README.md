# Adapter_EByte_E07_900MM10S_to_Ebyte_E07_868MS10_FUEL4EP ![License: CC BY-NC-SA 4.0](https://img.shields.io/badge/License-CC%20BY--NC--SA%204.0-lightgrey.svg) <a href='https://ko-fi.com/FUEL4EP' target='_blank'><img height='20' style='border:0px;height:20px;' src='https://cdn.ko-fi.com/cdn/kofi1.png?v=2' border='0' alt='Buy Me a Coffee at ko-fi.com' /></a>

## Ersatz für das eByte E07 868MS10 Modul 

- da das eByte E07 868MS10 Modul nicht mehr oder nur sehr schwer käuflich ist, wurde dieser Adapter als Ersatz entworfen. Es wird ein verfügbares EByte E07 900MM10S Funkmodul verbaut.


## Adapter für 868.3 MHz Funkmodul Ebyte E07 900MM10S auf Ebyte E07 868MS10

- Infos zu AsksinPP sind [hier](https://asksinpp.de) und [hier](https://asksinpp.de/Grundlagen/01_hardware.html#verdrahtung) zu finden

![pic](PNGs/Adapter_EByte_E07_900MM10S_to_Ebyte_E07_868MS10_FUEL4EP_PCB_3D_top.png)
![pic](PNGs/Adapter_EByte_E07_900MM10S_to_Ebyte_E07_868MS10_FUEL4EP_PCB_KiCAD.png)
![pic](PNGs/Adapter_EByte_E07_900MM10S_to_Ebyte_E07_868MS10_FUEL4EP_top_silkscreen.png)



## Platinenmaße

- Breite: 19 mm
- Weite: 13 mm

## Stromlaufplan

- ist [hier](./Schematics/Adapter_EByte_E07_900MM10S_to_Ebyte_E07_868MS10_FUEL4EP.pdf)

## Status

- die Platine V1.1 wurde bei JLCPCB gefertigt und bestückt.
- der Frequenztest wurde am 25. Sep 2024 mit einem PCB HB-UNI-SEN-BATT_ATMega1284P_E07-868MS10_FUEL4EP erfolgreich durchgeführt.
- die Protokolldatei des Frequenztests ist [hier](./Log_files_as_reference/FreqTest_1284P_serial_monitor.log)
- Bilder der gefertigten Prototypen:
![pic](Pictures_of_JLCPCB_prototypes/Adapter_EByte_E07_900MM10S_to_Ebyte_E07_868MS10_FUEL4EP_PCB_top_V1.1.png)
![pic](Pictures_of_JLCPCB_prototypes/Adapter_EByte_E07_900MM10S_to_Ebyte_E07_868MS10_FUEL4EP_PCB_bottom_V1.1.png)



## Rückmeldungen

- sind willkommen.

## Daten für die Bestellung bei JLCPCB

- die Daten für die Bestellung liegen im Verzeichnis [./jlcpcb/production_files/](./jlcpcb/production_files/)
- Gerber-Daten als Zip-Datei
- BOM- und CPL-Daten als CSV-Datei


## Hinweis für Bestückung bei JLCPCB

- bei der Bestellung bitte angeben '[specify a location](https://jlcpcb.com/help/article/50-How-to-remove-order-number-from-your-PCB)'

![pic](./Pictures_of_JLCPCB_prototypes/specify_an_order_number.png)

- der  Dummy-Text 'JLCJLCJLCJLC' ist auf der Rückseite der Platine

- Bestückung des Funkmoduls Ebyte E07 900MM10S geht bei JLCPCB nur als teures Standard-PCBA. Handlöten ist eine günstigere Alternative.

## Montage

- die Platine wird mit einer Stiftleiste RM 1.27mm 1x8 Pin auf die Basisplatine gelötet

## lokale Installation des Github Releases auf Deinem Computer

- gehe bitte in Dein Zielinstallationsverzeichnis, wo Du mit der Adapter_EByte_E07_900MM10S_to_Ebyte_E07_868MS10_FUEL4EP Platine mit KiCAD arbeiten möchtest

  - gebe dort 'git clone https://github.com/FUEL4EP/HomeAutomation.git' ein
	  + damit lädst Du mein [Sammelrepository](https://github.com/FUEL4EP/HomeAutomation) für alle meine auf Github freigegebenen HomeBrew Entwicklungen herunter.
  - dann findest Du ein neues Verzeichnis 'HomeAutomation' auf Deinem Rechner, das alle meine auf Github freigebenen Entwicklungen enthält, siehe [README.md](https://github.com/FUEL4EP/HomeAutomation/blob/master/README.md)
  	+ mache bitte regelmäßig ein Update mit 'git pull'
 -	die Platine Adapter_EByte_E07_900MM10S_to_Ebyte_E07_868MS10_FUEL4EP findest Du unter './HomeAutomation/tree/master/AsksinPP_developments/PCBs/Adapter_EByte_E07_900MM10S_to_Ebyte_E07_868MS10_FUEL4EP'
 
- alternativ kann vom Github [Sammelrepository](https://github.com/FUEL4EP/HomeAutomation) die ZIP-Datei HomeAutomation-master.zip heruntergeladen und auf dem lokalen Rechner ausgepackt werden. Bitte dann regelmäßig auf Github nach Updates schauen.

## Versionsverlauf

-   V1.0 24. Jun 2024: Initiale Version
-   V1.1 23. Sep 2024: Migration auf KiCAD 8; zusätzliche Vias
- KiCad Schaltplan-Editor Version: 8.0.5-8.0.5-0~ubuntu22.04.1 unter Kubuntu22.04.1
- KiCad Leiterplatteneditor Version: 8.0.5-8.0.5-0~ubuntu22.04.1 unter Kubuntu22.04.1
- PCB Version 1.1
- Schematics  1.1


## Disclaimer

-   die Nutzung der hier veröffentlichten Inhalte erfolgt vollständig auf eigenes Risiko und ohne jede Gewähr.
-  der Nutzer dieser Platine ist selbst dafür verantwortlich, dass die relevanten Richtlinien für Funkanlagen, z.B. in der EU die Funkanlagenrichtlinie (2014/53/EU, auch bekannt als RED), eingehalten wird.

## Lizenz 

**Creative Commons BY-NC-SA**<br>
Give Credit, NonCommercial, ShareAlike

<a rel="license" href="http://creativecommons.org/licenses/by-nc-sa/4.0/"><img alt="Creative Commons License" style="border-width:0" src="https://i.creativecommons.org/l/by-nc-sa/4.0/88x31.png" /></a><br />This work is licensed under a <a rel="license" href="http://creativecommons.org/licenses/by-nc-sa/4.0/">Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License</a>.


-EOF
	

