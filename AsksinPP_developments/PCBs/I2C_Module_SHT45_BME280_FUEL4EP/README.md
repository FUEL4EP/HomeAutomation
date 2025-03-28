# I2C_Module_SHT45_BME280_FUEL4EP ![License: CC BY-NC-SA 4.0](https://img.shields.io/badge/License-CC%20BY--NC--SA%204.0-lightgrey.svg) <a href='https://ko-fi.com/FUEL4EP' target='_blank'><img height='20' style='border:0px;height:20px;' src='https://cdn.ko-fi.com/cdn/kofi1.png?v=2' border='0' alt='Buy Me a Coffee at ko-fi.com' /></a>


## I2C SHT45 und BME280 Sensor Aufsteckplatine für DIY AsksinPP-Projekte

- basierend auf [Sensirion SHT45](https://sensirion.com/media/documents/33FD6951/64D3B030/Sensirion_Datasheet_SHT4x.pdf) für die genaue Temperatur- und Luftfeuchtemessung
- basierend auf [Bosch BME280](https://www.bosch-sensortec.com/media/boschsensortec/downloads/datasheets/bst-bme280-ds002.pdf) für die Luftdruckmessung



- passend zu den folgenden Mutterplatinen:
	* [HB-UNI-SEN-BATT_ATMega1284P_E07-868MS10_FUEL4EP](https://github.com/FUEL4EP/HomeAutomation/tree/master/AsksinPP_developments/PCBs/HB-UNI-SEN-BATT_ATMega1284P_E07-868MS10_FUEL4EP)
	* [HB-UNI-SEN-BATT_ATMega1284P_E07-868MS10_FRAM_FUEL4EP](https://github.com/FUEL4EP/HomeAutomation/tree/master/AsksinPP_developments/PCBs/HB-UNI-SEN-BATT_ATMega1284P_E07-868MS10_FRAM_FUEL4EP)
	* [HB-UNI-SEN-BATT_FUEL4EP](https://github.com/FUEL4EP/HomeAutomation/tree/master/AsksinPP_developments/PCBs/HB-UNI-SEN-BATT_FUEL4EP)
	* [HB-UNI-SEN-BATT_ATMega1284P_E07-868MS10_FUEL4EP](https://github.com/FUEL4EP/HomeAutomation/tree/master/AsksinPP_developments/PCBs/HB-UNI-SEN-BATT_ATMega1284P_E07-868MS10_FUEL4EP)
	* Befestigung mit 2 Stiftleisten J1 und J2 auf die Mutterplatine
		
- VCC max. ist 3,3 Volt !!
- die voreingestellte Standard-I2C-Adresse des BME280 Sensors ist 0x76
- für die I2C-Adresse 0x77 des BME280 Sensors ist der Jumper JP1 aufzutrennen und der Jumper JP2 mit Lötzinn zu überbrücken
- die voreingestellte I2C-Adresse des SHT45 Sensors ist 0x44

- Infos zu AsksinPP sind [hier](https://asksinpp.de) und [hier](https://asksinpp.de/Grundlagen/01_hardware.html#verdrahtung) zu finden

![pic](PNGs/I2C_Module_SHT45_BME280_FUEL4EP_PCB_3D_top.png)
![pic](PNGs/I2C_Module_SHT45_BME280_FUEL4EP_PCB_KiCAD.png)
![pic](PNGs/I2C_Module_SHT45_BME280_FUEL4EP_top_silkscreen.png)

- Bilder der bei JLCPCB gefertigten und assemblierten Prototypplatine V1.1
![pic](Pictures_of_JLCPCB_prototypes/I2C_Module_SHT45_BME280_FUEL4EP_PCB_top_JLCPCB_assembled_prototype_V1.1.png)
![pic](Pictures_of_JLCPCB_prototypes/I2C_Module_SHT45_BME280_FUEL4EP_PCB_bottom_JLCPCB_assembled_prototype_V1.1.png)
- - V1.1 der Platine hatte noch falsche I2C-Pullup-Widerstände, die ausgelötet werden mussten
- in der aktuellen Version auf Github ist dieser Fehler behoben


## Platinenmaße

- Breite: 30,48 mm
- Weite: 17 mm
- Platinendicke 1,6 mm (bei der Bestellung bei JLCPCB auswählen)

## Stromlaufplan

- ist [hier](./Schematics/I2C_Module_SHT45_BME280_FUEL4EP.pdf)

## Status

- die Platine V1.1 wurde bei JLCPCB gefertigt und erfolgreich validiert
	- V1.1 hatte noch falsche I2C-Pullup-Widerstände, die ausgelötet werden mussten
	- in der aktuellen Version der Platine auf Github ist dieser Fehler behoben


## Rückmeldungen

- und Verbesserungsvorschläge sind willkommen.

### KiCAD Plugin
- für die Erzeugung der JLCPCB Produktionsdaten wurde das Plugin [KiCAD JLCPCB tools](https://github.com/bouni/kicad-jlcpcb-tools) verwendet.


## Daten für die Bestellung bei JLCPCB

- die Daten für die Bestellung liegen im Verzeichnis [./jlcpcb/production_files/](./jlcpcb/production_files/)
- Gerber-Daten als Zip-Datei
- BOM- und CPL-Daten als CSV-Datei

- die Rotation von U1 (BME280) ist bei der Plazierungsprüfansicht von JLCPCB  falsch und muss händisch korrigiert und geprüft werden. Bitte die entsprechende Option 'Confirm Parts Placement'  beim Bestellen wählen.
- - die Rotation von U2 (SHT45) ist bei der Plazierungsprüfansicht von JLCPCB  falsch und muss händisch korrigiert und geprüft werden. Bitte die entsprechende Option 'Confirm Parts Placement'  beim Bestellen wählen.
- wahrscheinlich muss der SHT45-Sensor bei JLCPCB vorbestellt werden ('Pre-Order')

## Sammelbestellung

- wer Interesse an einer gemeinsamen Bestellung von Platinen und/oder Gehäusen bei JLCPCB hat, fragt bitte per per PN bei bei [FUEL4EP](https://homematic-forum.de/forum/ucp.php?i=pm&mode=compose&u=20685) an.
- wenn genügend Interesse (>= 20 bestückte Platinen und/oder 3D gedruckten Gehäusen) zustande kommt, biete ich an, Platinen  und Gehäuse gegen Vorkasse per PayPal zum Selbstkostenpreis + Versandkosten + 3 Euro Aufwandspauschale pro Versand bei JLCPCB zu bestellen und dann weiterzuverkaufen. Vor einer Bestellung wird ein Angebot bei JLCPCB eingeholt und an die Interessenten zur Zustimmung verteilt.

## lokale Installation des Github Releases auf Deinem Computer

- gehe bitte in Dein Zielinstallationsverzeichnis, wo Du mit der I2C_Module_SHT45_BME280_FUEL4EP Platine mit KiCAD arbeiten möchtest

  - gebe dort 'git clone https://github.com/FUEL4EP/HomeAutomation.git' ein
	  + damit lädst Du mein [Sammelrepository](https://github.com/FUEL4EP/HomeAutomation) für alle meine auf Github freigegebenen HomeBrew Entwicklungen herunter.
  - dann findest Du ein neues Verzeichnis 'HomeAutomation' auf Deinem Rechner, das alle meine auf Github freigebenen Entwicklungen enthält, siehe [README.md](https://github.com/FUEL4EP/HomeAutomation/blob/master/README.md)
  	+ mache bitte regelmäßig ein Update mit 'git pull'
 -	die Platine I2C_Module_SHT45_BME280_FUEL4EP findest Du unter './HomeAutomation/tree/master/AsksinPP_developments/PCBs/I2C_Module_SHT45_BME280_FUEL4EP'
 
- alternativ kann vom Github [Sammelrepository](https://github.com/FUEL4EP/HomeAutomation) die ZIP-Datei HomeAutomation-master.zip heruntergeladen und auf dem lokalen Rechner ausgepackt werden. Bitte dann regelmäßig auf Github nach Updates schauen.


## Versionsverlauf

-   V1.0 09. Sep 2023: Initiale Veröffentlichung auf Github
-   V1.1 15. Dez 2023: SHT45-AD1B-R2 statt SHT45-AD1F, da JLCPCB SHT45-AD1F
nicht mehr assembliert
- 	S V1.3 08. Feb 2024: Fix der I2C Pull-Up Widerstände
	L V1.2 08. Feb 2024: Fix der I2C Pull-Up Widerstände
-	S V1.4 16. Apr 2024: Ersetzen von R3 durch JP2 wegen Ruhestrom; Migration auf KiCAD 7
-	L V1.3 16. Apr 2024: Ersetzen von R3 durch JP2 wegen Ruhestrom; Migration auf KiCAD 7
-	S V1.5 30. Sep 2024: Migration auf KiCAD 8
-	L V1.4 30. Sep 2024: Migration auf KiCAD 8; Fix von DRC Regeln
-	L V1.5 02. Okt 2024: Einfügen einer versehentlich gelöschten redundanten VCC Leitung
- KiCad Schaltplan-Editor Version: 8.0.5-8.0.5-0~ubuntu22.04.1, release build unter Kubuntu22.04.4
- KiCad Leiterplatteneditor Version: 8.0.5-8.0.5-0~ubuntu22.04.1, release build unter Kubuntu22.04.4
- PCB Version 1.5
- Schematics  1.5

## Disclaimer

-   die Nutzung der hier veröffentlichten Inhalte erfolgt vollständig auf eigenes Risiko und ohne jede Gewähr.


## Lizenz 

**Creative Commons BY-NC-SA**<br>
Give Credit, NonCommercial, ShareAlike

<a rel="license" href="http://creativecommons.org/licenses/by-nc-sa/4.0/"><img alt="Creative Commons License" style="border-width:0" src="https://i.creativecommons.org/l/by-nc-sa/4.0/88x31.png" /></a><br />This work is licensed under a <a rel="license" href="http://creativecommons.org/licenses/by-nc-sa/4.0/">Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License</a>.


-EOF
	

