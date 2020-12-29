# ep-hb-devices-addon [![License: CC BY-NC-SA 4.0](https://img.shields.io/badge/License-CC%20BY--NC--SA%204.0-lightgrey.svg)](https://creativecommons.org/licenses/by-nc-sa/4.0/) [![Github All Releases](https://img.shields.io/github/downloads/FUEL4EP/HomeAutomation/total.svg)](https://github.com/FUEL4EP/HomeAutomation/releases) [![GitHub issues](https://img.shields.io/github/issues/FUEL4EP/HomeAutomation.svg)](https://github.com/FUEL4EP/HomeAutomation/issues) 

**:heavy_check_mark: lauffähig unter CCU / RaspberryMatic Firmware 2.31.x - 2.53.27 | 3.37.x - 3.53.x**<br/>
Die jeweils aktuellste Version ist bei den [Releases](https://github.com/FUEL4EP/HomeAutomation/releases/latest) zu finden.

<hr/>

**Das Wichtigste vorweg:**<br/>

**Dieses Addon und diese README.md Datei sind abgeleitet von [Jérômes](https://github.com/jp112sdl) Addon [JP-HB-Devices-addon](https://github.com/jp112sdl/JP-HB-Devices-addon). Herzlichen Dank dafür an Jérôme.**

Dieses Addon integriert die unten aufgelisteten HomeBrew Projekte in die CCU-Firmware.</font><br/>
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
|<img src="CCU_RM/src/addon/www/config/img/devices/50/hb-uni-sensor-CO2-SCD30_thumb.png" width=25/> | [HB-UNI-SENSOR-CO2-SCD30](https://github.com/FUEL4EP/HomeAutomation/tree/master/AsksinPP_developments/sketches/HB-UNI-Sensor1-CO2_SCD30) | NDIR CO2 Sensor<br/>_basierend auf Sensirion SCD30_ | 🔋 ⚡ |
|<img src="CCU_RM/src/addon/www/config/img/devices/50/hb-uni-sensor-AQ-BME680_thumb.png" width=25/> | [HB-UNI-SENSOR-AQ-BME680](https://github.com/FUEL4EP/HomeAutomation/tree/master/AsksinPP_developments/sketches/HB-UNI-Sensor1-AQ-BME680) | Air Quality Sensor<br/>_basierend auf Bosch BME680_ | 🔋 |
|<img src="CCU_RM/src/addon/www/config/img/devices/50/hb-uni-sensor-TH-SHT75_thumb.png" width=25/> | [HB-UNI-SENSOR-TH-SHT75](https://github.com/FUEL4EP/HomeAutomation/tree/master/AsksinPP_developments/sketches/HB-UNI-Sensor1-TH-SHT75) | high accuracy TH Sensor<br/>_basierend auf Sensirion SHT75_ | 🔋 |




**Hinweis für Entwickler** _(verwendete Device Model meiner bzw. fremd-eingebundener HB-Geräte)_ **:**

| Device Model | Gerät |
|--------|--------|
|F6 01 | HB-UNI-SENSOR-CO2-SCD30 |
|F6 02 | HB-UNI-SENSOR-AQ-BME680 |
|F6 03 | HB-UNI-SENSOR-TH-SHT75 |



<a rel="license" href="http://creativecommons.org/licenses/by-nc-sa/4.0/"><img alt="Creative Commons Lizenzvertrag" style="border-width:0" src="https://i.creativecommons.org/l/by-nc-sa/4.0/88x31.png" /></a><br />Dieses Werk ist lizenziert unter einer <a rel="license" href="http://creativecommons.org/licenses/by-nc-sa/4.0/">Creative Commons Namensnennung - Nicht-kommerziell - Weitergabe unter gleichen Bedingungen 4.0 International Lizenz</a>.


#### Die verwendeten Icons sind "free for non-commercial use" von
 
- http://icons8.com
- https://icon-library.com
- https://thenounproject.com
