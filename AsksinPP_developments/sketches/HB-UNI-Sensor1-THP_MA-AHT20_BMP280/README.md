# HB-UNI-Sensor1-THP_MA-AHT20_BMP280: Temperatur, Luftfeuchte und Luftdrucksensor für langfristige Klimaerfassung zur Beobachtung des Klimawandels[![License: CC BY-NC-SA 4.0](https://img.shields.io/badge/License-CC%20BY--NC--SA%204.0-lightgrey.svg)](https://creativecommons.org/licenses/by-nc-sa/4.0/) [![Hits](https://hits.seeyoufarm.com/api/count/incr/badge.svg?url=https%3A%2F%2Fgithub.com%2FFUEL4EP%2FHomeAutomation%2FAsksinPP_developments%2Fsketches%2FHB-UNI-Sensor1-THP_MA-AHT20_BMP280&count_bg=%2379C83D&title_bg=%23555555&icon=&icon_color=%23E7E7E7&title=hits&edge_flat=false)](https://hits.seeyoufarm.com)
<br/>

# Eigenschaften

## Ausgabe der aktuellen Messwerte von

- Temperatur
- relativen Luftfeuchte
- Luftdruck 
- Messintervall: 4 Minuten

## Ausgabe der gleitenden Temperaturmittelwerte über
 
- die letzten 24 Stunden
- die letzte Woche (7 Tage)
- den letzten Monat (31 Tage)
- das letzte Jahr (365 Tage)

## lokale gleitende Mittelwertbildung

- nichtflüchtige ferromagnetische Speicher FRAM zur lokalen Messwertspeicherung
	+ Abspeicherung der Temperaturmesswerte in 2x 2MBit FRAM in Ringpuffern
- Datenerhalt bei Batteriewechsel
- Vorbelegung der Ringpuffer bei Kaltstart mit interpolierten Temperaturwerten aus den historischen Monatsmittelwerten des vergangenen Jahres, z.B. [von Frankfurt](https://www.wetterkontor.de/de/wetter/deutschland/monatswerte-station.asp?id=10637)
	- Referenzzeitpunkt: Kompilationszeitpunkt des Sketches

## Sensoren:

- [Aosong AHT20](https://files.seeedstudio.com/wiki/Grove-AHT20_I2C_Industrial_Grade_Temperature_and_Humidity_Sensor/AHT20-datasheet-2020-4-16.pdf) (Fehlertoleranz der Messung: ±0.3°C, ±2% RH)
- [Bosch BME280](https://www.bosch-sensortec.com/media/boschsensortec/downloads/datasheets/bst-bme280-ds002.pdf)

## weitere Eigenschaften

- Ausgabe der Batteriespannung
- WebUI Offseteinstellung
- geringe Stromaufnahme von ca. 12 uA Stromaufnahme im Sleep-Mode


## WebUI

![pic](./Images/WebUI.png)

## Konfigurieren der Geräte- und Kanalparameter

![pic](./Images/setting_of_device_parameters_in_WebUI.png)

## Posteingang nach erfolgreichem Anlernen

![pic](./Images/successful_teach_in.png)


## benötigte Hardware

- [HB-UNI-SEN-BATT_ATMega1284P_E07-868MS10_FRAM_FUEL4EP](https://github.com/FUEL4EP/HomeAutomation/tree/master/AsksinPP_developments/PCBs/HB-UNI-SEN-BATT_ATMega1284P_E07-868MS10_FRAM_FUEL4EP)

![pic](./Images/HB-UNI-Sensor1-THP_MA-AHT20_BMP280.png)

## erste Messaufzeichnungen nach Boot

![pic](./Images/moving_averages_after_boot.png)

- in diesem Diagramm wurde am 19. Juni 2024 um ca. 9:10 Uhr (Peak in der Temperaturkurve durch Hereinholen des Sensors von draußen  in  das Haus) ein Batteriewechsel ohne Datenverlust durchgeführt, d.h. ein Warmstart.

## aktueller Status

- **Vorabfreigabe** für Testzwecke
- die Grundfunktionen sind bereits erfolgreich validiert
- Validierung ist noch im Gange
	+ Temperatur, Luftfeuchte und Luftdruckmessung
	+ Vorbelegung der Ringpuffer im FRAM mit historischen Temperaturwerten
	+ Mittelwertbildung
	+ Kaltstart bei Werksreset (Config-Taste > 6 Sekunden drücken)
	+ Warmstart bei Batteriewechsel ohne Datenverlust

## Addon

- eine Vorabfreigabe des benötigten Addons hb-ep-devices-addon V1.17 für Testzwecke ist [hier](./addon/) lokal beigefügt

## benötigte Bibliotheken

- AskSin++                  2016 papa (Creative Commons)
- Adafruit BMP280           2020 Adafruit https://github.com/adafruit/Adafruit_BMP280_Library (no license mentioned)
- finitespace/BME280        2016 finitespace https://github.com/finitespace/BME280  (GNU General Public License v3.0) for EnvironmentCalculations
- AHT20                     2020 dvarrel https://github.com/dvarrel/AHT20 (Creative Commons Zero v1.0 Universal)
- Adafruit_FRAM_SPI         2024 FUEL4EP fork of Adafruit's Adafruit_FRAM_SPI: added low power mode; https://github.com/FUEL4EP/HomeAutomation/tree/master/AsksinPP_developments/libraries/Adafruit_FRAM_SPI (BSD License)
- Time                      2012 PaulStoffregen https://github.com/PaulStoffregen (no license mentioned)

## Speicherbedarf auf einem ATMega1284P (mit Debug)

Der Sketch verwendet 58708 Bytes (44%) des Programmspeicherplatzes. Das Maximum sind 130560 Bytes.
Globale Variablen verwenden 2592 Bytes (15%) des dynamischen Speichers, 13792 Bytes für lokale Variablen verbleiben. Das Maximum sind 16384 Bytes.

## Gehäuse

- geplant ist ein Einbau in eine [TFA Schutzhülle mit Solar-Lüfter für Thermo-/Hygrosensoren, Innenmaße (Ø x H): 80 x 170 mm ](https://de.elv.com/tfa-schutzhuelle-mit-solar-luefter-fuer-thermo-hygrosensoren-innenmasse-oe-x-h-80-x-170-mm-253915)
	- solarbetriebener Lüfter

![pic](./Images/outdoor_case.png)

- **HINWEIS**: Bei meinem Exemplar lief im Lieferzustand der eingebaute Lüfter falsch herum. Dies führte bei Sonneneinstrahlung zu einer hohen Temperaturüberhöhung (ca. 2..4K). Nach einem Umlöten der Lüfteranschlüsse an die Solarzelle war die Temperaturüberhöhung gegenüber einer Referenzmessung deutlich besser (ca. 0,4..0,8 K). Dieser Verdrahtungsfehler wurde an TFA Dostmann berichtet.
- die Platine [HB-UNI-SEN-BATT_ATMega1284P_E07-868MS10_FRAM_FUEL4EP](https://github.com/FUEL4EP/HomeAutomation/tree/master/AsksinPP_developments/PCBs/HB-UNI-SEN-BATT_ATMega1284P_E07-868MS10_FRAM_FUEL4EP) wird in ein 3D-gedrucktes Innengehäuse mit ePTFE Membranen kommen

## Lizenz

**Creative Commons BY-NC-SA**<br>
Give Credit, NonCommercial, ShareAlike

<a rel="license" href="http://creativecommons.org/licenses/by-nc-sa/4.0/"><img alt="Creative Commons License" style="border-width:0" src="https://i.creativecommons.org/l/by-nc-sa/4.0/88x31.png" /></a><br />This work is licensed under a <a rel="license" href="http://creativecommons.org/licenses/by-nc-sa/4.0/">Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License</a>.
