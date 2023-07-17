# ep-hb-devices-addon [![License: CC BY-NC-SA 4.0](https://img.shields.io/badge/License-CC%20BY--NC--SA%204.0-lightgrey.svg)](https://creativecommons.org/licenses/by-nc-sa/4.0/) [![Github All Releases](https://img.shields.io/github/downloads/FUEL4EP/HomeAutomation/total.svg)](https://github.com/FUEL4EP/HomeAutomation/releases) ![GitHub release (by tag)](https://img.shields.io/github/downloads/FUEL4EP/HomeAutomation/latest/total) [![GitHub issues](https://img.shields.io/github/issues/FUEL4EP/HomeAutomation.svg)](https://github.com/FUEL4EP/HomeAutomation/issues) <a href='https://ko-fi.com/FUEL4EP' target='_blank'><img height='20' style='border:0px;height:20px;' src='https://cdn.ko-fi.com/cdn/kofi1.png?v=2' border='0' alt='Buy Me a Coffee at ko-fi.com' /></a>

**:heavy_check_mark: lauffähig unter CCU3 / RaspberryMatic Firmware 3.47.10 - 3.69.7**<br/>
Die jeweils aktuellste Version ist bei den [Releases](https://github.com/FUEL4EP/HomeAutomation/releases/latest) zu finden.

<hr/>

**Das Wichtigste vorweg:**<br/>

**Dieses Addon und diese README.md Datei sind abgeleitet von [Jérômes](https://github.com/jp112sdl) Addon [JP-HB-Devices-addon](https://github.com/jp112sdl/JP-HB-Devices-addon). Herzlichen Dank dafür an Jérôme.**

Dieses Addon integriert die unten aufgelisteten HomeBrew Projekte in die CCU3/RaspberryMatic-Firmware.</font><br/>
Ich stelle es 

- kostenfrei 
- ohne jeglichen Anspruch auf vollständige Funktion 
- auf eigene Gefahr (z.B. unerwünschte Nebeneffekte in der CCU-Firmware)

zur Verfügung.<br/><br/>
Da es sich um ein reines Hobby-Projekt handelt, kann ich **weder (umfangreichen) Support** leisten, **noch auf individuelle Wünsche eingehen** oder **zeitnah die neueste CCU-Firmware unterstützen**!<br/>
Es sollte klar sein, dass mit (m)einer eventuellen Aufgabe dieses Hobbys, mittel-/langfristig (teilweise) keine Verwendung u.g. Geräte mehr möglich ist. <i>Wie bei Open Source Projekten üblich, kann sich jedoch jeder an der Entwicklung beteiligen!</i>

<hr/>

Nach der Installation wird automatisch ein Neustart der Zentrale durchgeführt.<br/>
Die eigentliche Installation des Addons erfolgt nach diesem Neustart, während die Zentrale hochfährt.<br/>
In dieser Zeit werden die Dienste **ReGaHss** und **RFD** gestoppt, so dass es **_(nicht bei RaspberryMatic!)_** im Browser zu der Meldung<br/>`Eine Komponente der Homematic Zentrale reagiert nicht mehr.` kommen kann.<br/>
_Bitte etwas Geduld haben - je nach Geschwindigkeit der SD Karte kann die Installation u.U. mehrere Minuten in Anspruch nehmen!_<br/><br/>

❗ Sollten anschließend keine Geräte-Icons und/oder -beschreibungen in der WebUI angezeigt werden, muss der Browser-Cache gelöscht werden.

Nach einem Upgrade der CCU/RaspberryMatic Firmware kann es notwendig sein, dieses Addon erneut zu installieren. Dafür die betroffenen Geräte **NICHT** ablernen, sondern nur das Addon nochmals neu installieren.

#### Dieses Addon wird benötigt, um die Kompatibilität der folgenden HomeMatic Selbstbaugeräte herzustellen:

|  | Name | Beschreibung | 🔋Batterie-/<br/>⚡️Netzbetrieb |
|--------|--------|--------|:--------:|
|<img src="CCU_RM/src/addon/www/config/img/devices/50/hb-uni-sensor-CO2-SCD30_thumb.png" width=25/> | [HB-UNI-SENSOR1-CO2-SCD30](https://github.com/FUEL4EP/HomeAutomation/tree/master/AsksinPP_developments/sketches/HB-UNI-Sensor1-CO2_SCD30) | NDIR CO2 Sensor<br/>_basierend auf Sensirion SCD30_ | 🔋 ⚡ |
|<img src="CCU_RM/src/addon/www/config/img/devices/50/hb-uni-sensor-AQ-BME680_thumb.png" width=25/> | [HB-UNI-SENSOR1-AQ-BME680](https://github.com/FUEL4EP/HomeAutomation/tree/master/AsksinPP_developments/sketches/HB-UNI-Sensor1-AQ-BME680) | Air Quality Sensor<br/>basierend auf Bosch BME680 <br/>und multipler linearer Regression | 🔋 |
|<img src="CCU_RM/src/addon/www/config/img/devices/50/hb-uni-sensor-AQ-BME680_KF_thumb.png" width=25/> | [HB-UNI-SENSOR1-AQ-BME680_KF](https://github.com/FUEL4EP/HomeAutomation/tree/master/AsksinPP_developments/sketches/HB-UNI-Sensor1-AQ-BME680_KF) | selbstkalibrierender Air Quality Sensor<br/>basierend auf Bosch BME680<br/>und Kalman Filter <br/>aLF Version| 🔋 |
|<img src="CCU_RM/src/addon/www/config/img/devices/50/hb-uni-sensor-AQ-BME680_KF_DEBUG_thumb.png" width=25/> | [HB-UNI-SENSOR1-AQ-BME680_KF_DEBUG](https://github.com/FUEL4EP/HomeAutomation/tree/master/AsksinPP_developments/sketches/HB-UNI-Sensor1-AQ-BME680_KF_DEBUG) | selbstkalibrierender Air Quality Sensor<br/>basierend auf Bosch BME680<br/>und Kalman Filter <br/>aLF Version (DEBUG) | 🔋 |
|<img src="CCU_RM/src/addon/www/config/img/devices/50/hb-uni-sensor-TH-SHT75_thumb.png" width=25/> | [HB-UNI-SENSOR1-TH-SHT75](https://github.com/FUEL4EP/HomeAutomation/tree/master/AsksinPP_developments/sketches/HB-UNI-Sensor1-TH-SHT75) | hoch genauer TH Sensor<br/>basierend auf Sensirion SHT75<br/>NEU: optional: gleitender Mittelwert der Temperatur über 24 Stunden | 🔋 |
|<img src="CCU_RM/src/addon/www/config/img/devices/50/hb-uni-sensor-THPD-BME280_thumb.png" width=25/> | [HB-UNI-SENSOR1-THPD-BME280](https://github.com/FUEL4EP/HomeAutomation/tree/master/AsksinPP_developments/sketches/HB-UNI-Sensor1-THPD-BME280) | THPD Sensor<br/>basierend auf Bosch BME280<br/>mit Taupunkttemperatur<br/>und absoluter Luftfeuchte| 🔋 |
|<img src="CCU_RM/src/addon/www/config/img/devices/50/hb-uni-sensor-THPD-BME280_thumb.png" width=25/> | [HB-UNI-SENSOR1-THPD-SHT85](https://github.com/FUEL4EP/HomeAutomation/tree/master/AsksinPP_developments/sketches/HB-UNI-Sensor1-THPD-SHT85) | hoch genauer und sehr schneller THPD Sensor<br/>basierend auf Sensirion SHT85 und Bosch BME280<br/>mit Taupunkttemperatur<br/>und absoluter Luftfeuchte<br/>NEU: optional: gleitender Mittelwert der Temperatur über 24 Stunden und 7 Tage bei ATMega1284P| 🔋 |
|<img src="CCU_RM/src/addon/www/config/img/devices/50/hb-uni-sensor-RAD-AL53_thumb.png" width=25/> | [HB-UNI-SENSOR1-RAD-AL53](https://github.com/FUEL4EP/HomeAutomation/tree/master/AsksinPP_developments/sketches/HB-UNI-Sensor1-RAD-AL53)<br/>| Radioaktivitätssensor mit PIN-Dioden Detektor<br/>basierend auf Teviso AL53<br/>für Alpha-, Beta- und Gammastrahlung| 🔋 |
|<img src="CCU_RM/src/addon/www/config/img/devices/50/hb-uni-sensor-AQ-BME680_KF_rLF_thumb.png" width=25/> | [HB-UNI-SENSOR1-AQ-BME680_KF_rLF](https://github.com/FUEL4EP/HomeAutomation/tree/master/AsksinPP_developments/sketches/HB-UNI-Sensor1-AQ-BME680_KF_rLF)<br/>- NEU -  | selbstkalibrierender Air Quality Sensor<br/>basierend auf Bosch BME680<br/>und Kalman Filter<br/>rLF Version | 🔋 |
|<img src="CCU_RM/src/addon/www/config/img/devices/50/hb-uni-sensor-AQ-BME680_KF_rLF_DEBUG_thumb.png" width=25/> | [HB-UNI-SENSOR1-AQ-BME680_KF_rLF_DEBUG](https://github.com/FUEL4EP/HomeAutomation/tree/master/AsksinPP_developments/sketches/HB-UNI-Sensor1-AQ-BME680_KF_rLF_DEBUG)<br/>- NEU - | selbstkalibrierender Air Quality Sensor<br/>basierend auf Bosch BME680<br/>und Kalman Filter <br/>rLF Version (DEBUG) | 🔋 |
|<img src="https://raw.githubusercontent.com/FUEL4EP/HomeAutomation/master/AsksinPP_developments/addons/hb-ep-devices-addon/CCU_RM/src/addon/www/config/img/devices/50/hb-uni-senact-4-4-SC_DS_thumb.png" width=25/> | [HB-UNI-SenAct-4-4-SC_DS](https://github.com/FUEL4EP/HomeAutomation/tree/master/AsksinPP_developments/sketches/HB-UNI-SenAct-4-4-SC_DS)<br/> - NEU -| 3x Schaltaktor für Betrieb an einem **Netzfreischalter** - Akkubetrieb im Standby, automatische Ladeschaltung - bei Aktivierung eines Aktors wird die Netzspannung zugeschaltet und am Ende wieder abgeschaltet - geringer Standby-Verbrauch von ca. 1 mA @ 5V| 🔋 ⚡ |
Alle hier aufgeführten Homematic Selbstbaugeräte sind bei mir selbst im Einsatz und im täglichen Betrieb validiert. Meine Zentrale läuft unter RaspberryMatic in der neuesten verfügbaren Version auf einem Raspberry Pi 3B+ .


**Hinweis für Entwickler** _(verwendete Device Model meiner bzw. fremd-eingebundener HB-Geräte)_ **:**

| Device Model | Gerät |
|--------|--------|
|F6 01 | HB-UNI-SENSOR1-CO2-SCD30 |
|F6 02 | HB-UNI-SENSOR1-AQ-BME680 |
|F6 03 | HB-UNI-SENSOR1-TH-SHT75 |
|F6 04 | HB-UNI-Sensor1-THPD-BME280 und HB-UNI-Sensor1-THPD-SHT85 |
|F6 05 | HB-UNI-SENSOR1-CO2-SCD30-PLUGGED_IN |
|F6 06 | HB-UNI-SENSOR1-AQ-BME680_KF |
|F6 07 | HB-UNI-SENSOR1-AQ-BME680_KF_DEBUG |
|F6 08 | HB-UNI-SENSOR1-RAD-AL53 |
|F6 09 | HB-UNI-SENSOR1-THPD-SHT85 |
|F6 0A | HB-UNI-SENSOR1-THPD-SHT75-MA |
|F6 0B | HB-UNI-SENSOR1-AQ-BME680_KF_rLF |
|F6 0C | HB-UNI-SENSOR1-AQ-BME680_KF_DEBUG_rLF |
|F6 0D | HB-LC-Bl1-FM-HYBRID (in Entwicklung) |
|F6 0E | HB-UNI-SENACT-4-4-SC_DS (in Prototyp Evaluationsphase) |
|F6 0F | HB-UNI-Sensor1-THPD_MA-AHT20 (in Entwicklung) |
<br/>



<a rel="license" href="http://creativecommons.org/licenses/by-nc-sa/4.0/"><img alt="Creative Commons Lizenzvertrag" style="border-width:0" src="https://i.creativecommons.org/l/by-nc-sa/4.0/88x31.png" /></a><br />Dieses Werk ist lizenziert unter einer <a rel="license" href="http://creativecommons.org/licenses/by-nc-sa/4.0/">Creative Commons Namensnennung - Nicht-kommerziell - Weitergabe unter gleichen Bedingungen 4.0 International Lizenz</a>.


#### Die verwendeten Icons sind "free for non-commercial use" von
 
- http://icons8.com
- https://icon-library.com
- https://thenounproject.com
- https://www.flaticon.com
- Icon made by Freepik from www.flaticon.com
