# CC1101_868MHz_UFL_RF_Modul_FUEL4EP ![License: CC BY-NC-SA 4.0](https://img.shields.io/badge/License-CC%20BY--NC--SA%204.0-lightgrey.svg) <a href='https://ko-fi.com/FUEL4EP' target='_blank'><img height='20' style='border:0px;height:20px;' src='https://cdn.ko-fi.com/cdn/kofi1.png?v=2' border='0' alt='Buy Me a Coffee at ko-fi.com' /></a>

## 868,3 MHz Funkmodul für AsksinPP DIY Projekte mit U.FL Antennenbuuchse

- Infos zu AsksinPP sind [hier](https://asksinpp.de) und [hier](https://asksinpp.de/Grundlagen/01_hardware.html#verdrahtung) zu finden

![pic](PNGs/CC1101_868MHz_UFL_RF_Modul_FUEL4EP_PCB_3D_top..png)
![pic](PNGs/CC1101_868MHz_UFL_RF_Modul_FUEL4EP_PCB_KiCAD.png)
![pic](PNGs/CC1101_868MHz_UFL_RF_Modul_FUEL4EP_top_silkscreen.png)
![pic](Pictures_of_JLCPCB_prototypes/CC1101_868MHz_UFL_RF_Modul_FUEL4EP_PCB_top_JLCPCB_assembled_prototype_V1.5.png)
![pic](PNGs/prototype_3_on_HB-UNI-SEN-BATT_ATMega1284P_E07-868MS10_FRAM_FUEL4EP.png)


## Platinenmaße

- Breite: 19 mm
- Weite: 13 mm
- Platinendicke **1,2 mm** (bei der Bestellung bei JLCPCB auswählen)

## Stromlaufplan

- ist [hier](./Schematics/CC1101_868MHz_UFL_RF_Modul_FUEL4EP.pdf)

## Danksagung

- diese Platine ist abgeleitet von [CC1101_Module](https://github.com/jp112sdl/CC1101_Module)
- mein besonderer Dank geht an [Jérôme](https://github.com/jp112sdl) und [papa](https://github.com/pa-pa) für ihre Vorarbeit und alle, die im [Forum](https://homematic-forum.de/forum/viewtopic.php?f=76&t=79770&sid=9a4abe34301506f133d423b0580074a5) zu dieser Platine beigetragen haben.

## Status

- diese Platine wurde bei JLCPCB gefertigt und wurde erfolgreich validiert
- die Validierungsergebnisse sind [hier](./Prototype_Validation) zu finden

## Messergebnisse von 868 MHz Antennen mit einem Vector Network Analyzer
- sind [hier](../../antennas) zu finden
- die für die Validierung verwendete Antenne von Technikkram hat ein schlechtes VSWR. Dies erklärt die Validierungsergebnisse.


## Ersatz für das eByte E07 868MS10 Modul mit zusätzlicher U.FL Antennenbuchse

- da das eByte E07 868MS10 Modul nicht mehr oder nur sehr schwer käuflich ist, wurde dieses Modul als Ersatz entworfen. Es ist antennenseitig nicht pinkompatibel und hat zusätzlich eine U.FL Antennenbuchse.

## Rückmeldungen

- und Verbesserungsvorschläge sind [hier](https://homematic-forum.de/forum/viewtopic.php?f=76&t=79770&sid=9a4abe34301506f133d423b0580074a5) willkommen.

## Daten für die Bestellung bei JLCPCB

- die Daten für die Bestellung liegen im Verzeichnis [./jlcpcb/production_files/](./jlcpcb/production_files/)
- Gerber-Daten als Zip-Datei
- BOM- und CPL-Daten als CSV-Datei


## Hinweis für Bestückung bei JLCPCB

- die U.FL Buchse wird im Online Viewer [falsch](./Pictures_of_JLCPCB_prototypes/CC1101_868MHz_UFL_RF_Modul_FUEL4EP_JLCPCB_assembly_wrong_placement_of_U.FL_connector.png) positioniert. Das ist ein Dummy-Fehler.
- - bei der Bestellung bitte angeben '[specify a location](https://jlcpcb.com/help/article/50-How-to-remove-order-number-from-your-PCB)'

![pic](./Pictures_of_JLCPCB_prototypes/specify_an_order_number.png)

- der  Dummy-Text 'JLCJLCJLCJLC' ist auf der Rückseite der Platine
- C8 und C9 sollen 1% C0G Kondensatoren sein
- **als Platinendicke unbedingt bei der Bestellung angeben: 1,2 mm (bitte nicht ändern)**

## lokale Installation des Github Releases auf Deinem Computer

- gehe bitte in Dein Zielinstallationsverzeichnis, wo Du mit der CC1101_868MHz_UFL_RF_Modul_FUEL4EP Platine mit KiCAD arbeiten möchtest

  - gebe dort 'git clone https://github.com/FUEL4EP/HomeAutomation.git' ein
	  + damit lädst Du mein [Sammelrepository](https://github.com/FUEL4EP/HomeAutomation) für alle meine auf Github freigegebenen HomeBrew Entwicklungen herunter.
  - dann findest Du ein neues Verzeichnis 'HomeAutomation' auf Deinem Rechner, das alle meine auf Github freigebenen Entwicklungen enthält, siehe [README.md](https://github.com/FUEL4EP/HomeAutomation/blob/master/README.md)
  	+ mache bitte regelmäßig ein Update mit 'git pull'
 -	die Platine CC1101_868MHz_UFL_RF_Modul_FUEL4EP findest Du unter './HomeAutomation/tree/master/AsksinPP_developments/PCBs/CC1101_868MHz_UFL_RF_Modul_FUEL4EP'
 
- alternativ kann vom Github [Sammelrepository](https://github.com/FUEL4EP/HomeAutomation) die ZIP-Datei HomeAutomation-master.zip heruntergeladen und auf dem lokalen Rechner ausgepackt werden. Bitte dann regelmäßig auf Github nach Updates schauen.

## Versionsverlauf

-   V1.3 01. Sep 2023: Anpassung der Silkscreen-Texte and JLCPCB-Regeln
-   V1.4 21. Sep 2023: Verbesserung der Entkopplungskondensatoren mit kürzeren GND-Stromschleifen zur Impedanzverringerung
-   V1.5 29. Sep 2023: Verringerung der parasitischen Kapazitäten des Oszillators; 1% CoG Kapazitäten für C8 und C9
-   S V1.5 30. Sep 2024: Migration auf KiCAD 8
-   L V1.6 30. Sep 2024: Migration auf KiCAD 8
- KiCad Schaltplan-Editor Version: 8.0.5-8.0.5-0~ubuntu22.04.1, release build unter Kubuntu22.04.1
- KiCad Leiterplatteneditor Version: 8.0.5-8.0.5-0~ubuntu22.04.1, release build unter Kubuntu22.04.1
- PCB Version 1.6
- Schematics  1.5

## Sammelbestellung

- wer Interesse an einer gemeinsamen Bestellung von Platinen und/oder Gehäusen bei JLCPCB hat, fragt bitte per per PN bei bei [FUEL4EP](https://homematic-forum.de/forum/ucp.php?i=pm&mode=compose&u=20685) an.
- wenn genügend Interesse (>= 20 bestückte Platinen und/oder 3D gedruckten Gehäusen) zustande kommt, biete ich an, Platinen  und Gehäuse gegen Vorkasse per PayPal zum Selbstkostenpreis + Versandkosten + 3 Euro Aufwandspauschale pro Versand bei JLCPCB zu bestellen und dann weiterzuverkaufen. Vor einer Bestellung wird ein Angebot bei JLCPCB eingeholt und an die Interessenten zur Zustimmung verteilt.

## Disclaimer

-   die Nutzung der hier veröffentlichten Inhalte erfolgt vollständig auf eigenes Risiko und ohne jede Gewähr.
-  der Nutzer dieser Platine ist selbst dafür verantwortlich, dass die relevanten Richtlinien für Funkanlagen, z.B. in der EU die Funkanlagenrichtlinie (2014/53/EU, auch bekannt als RED), eingehalten wird.

## Lizenz 

**Creative Commons BY-NC-SA**<br>
Give Credit, NonCommercial, ShareAlike

<a rel="license" href="http://creativecommons.org/licenses/by-nc-sa/4.0/"><img alt="Creative Commons License" style="border-width:0" src="https://i.creativecommons.org/l/by-nc-sa/4.0/88x31.png" /></a><br />This work is licensed under a <a rel="license" href="http://creativecommons.org/licenses/by-nc-sa/4.0/">Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License</a>.


-EOF
	

