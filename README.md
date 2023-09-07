# HomeAutomation [![License: CC BY-NC-SA 4.0](https://img.shields.io/badge/License-CC%20BY--NC--SA%204.0-lightgrey.svg)](https://creativecommons.org/licenses/by-nc-sa/4.0/) [![Hits](https://hits.seeyoufarm.com/api/count/incr/badge.svg?url=https%3A%2F%2Fgithub.com%2FFUEL4EP%2FHomeAutomation&count_bg=%2379C83D&title_bg=%23555555&icon=&icon_color=%23E7E7E7&title=hits&edge_flat=false)](https://hits.seeyoufarm.com) <a href='https://ko-fi.com/FUEL4EP' target='_blank'><img height='20' style='border:0px;height:20px;' src='https://cdn.ko-fi.com/cdn/kofi1.png?v=2' border='0' alt='Buy Me a Coffee at ko-fi.com' /></a>

## Home Automation Repository von FUEL4EP


### Homebrew [Asksinpp](https://asksinpp.de/) Sensoren und benötigtes [CCU3/RM Addon](https://github.com/FUEL4EP/HomeAutomation/releases/latest):

#### Sensoren:


|  | Name | Beschreibung | 🔋Batterie-/<br/>⚡️Netzbetrieb |
|--------|--------|--------|:--------:|
|<img src="https://raw.githubusercontent.com/FUEL4EP/HomeAutomation/master/AsksinPP_developments/addons/hb-ep-devices-addon/CCU_RM/src/addon/www/config/img/devices/50/hb-uni-sensor-CO2-SCD30_thumb.png" width=25/> | [HB-UNI-SENSOR1-CO2-SCD30](https://github.com/FUEL4EP/HomeAutomation/tree/master/AsksinPP_developments/sketches/HB-UNI-Sensor1-CO2_SCD30) | genauer NDIR CO2 Sensor<br/>_basierend auf Sensirion SCD30<br/>Akku- oder Netzbetrieb<br/>und WebUI Offseteinstellung_ | 🔋 ⚡ |
|<img src="https://raw.githubusercontent.com/FUEL4EP/HomeAutomation/master/AsksinPP_developments/addons/hb-ep-devices-addon/CCU_RM/src/addon/www/config/img/devices/50/hb-uni-sensor-AQ-BME680_thumb.png" width=25/> | [HB-UNI-SENSOR1-AQ-BME680](https://github.com/FUEL4EP/HomeAutomation/tree/master/AsksinPP_developments/sketches/HB-UNI-Sensor1-AQ-BME680) | selbstkalibrierender Air Quality Sensor<br/>_basierend auf Bosch BME680<br/>und multipler linearer Regression<br/>und WebUI Offseteinstellung_ (aLF Version)| 🔋 |
|<img src="https://raw.githubusercontent.com/FUEL4EP/HomeAutomation/master/AsksinPP_developments/addons/hb-ep-devices-addon/CCU_RM/src/addon/www/config/img/devices/50/hb-uni-sensor-AQ-BME680_KF_thumb.png" width=25/> | [HB-UNI-SENSOR1-AQ-BME680_KF](https://github.com/FUEL4EP/HomeAutomation/tree/master/AsksinPP_developments/sketches/HB-UNI-Sensor1-AQ-BME680_KF) | selbst kalibrierender Air Quality Sensor<br/>basierend auf Bosch BME680<br/>und Kalman Filter<br/>und WebUI Offseteinstellung (aLF Version)| 🔋 |
|<img src="https://raw.githubusercontent.com/FUEL4EP/HomeAutomation/master/AsksinPP_developments/addons/hb-ep-devices-addon/CCU_RM/src/addon/www/config/img/devices/50/hb-uni-sensor-AQ-BME680_KF_DEBUG_thumb.png" width=25/> | [HB-UNI-SENSOR1-AQ-BME680_KF_DEBUG](https://github.com/FUEL4EP/HomeAutomation/tree/master/AsksinPP_developments/sketches/HB-UNI-Sensor1-AQ-BME680_KF_DEBUG) | selbst kalibrierender Air Quality Sensor<br/>basierend auf Bosch BME680<br/>und Kalman Filter<br/>und WebUI Offseteinstellung (aLF Version)<br/>nur für Debugzwecke!| 🔋 |
|<img src="https://raw.githubusercontent.com/FUEL4EP/HomeAutomation/master/AsksinPP_developments/addons/hb-ep-devices-addon/CCU_RM/src/addon/www/config/img/devices/50/hb-uni-sensor-TH-SHT75_thumb.png" width=25/> | [HB-UNI-SENSOR1-TH-SHT75](https://github.com/FUEL4EP/HomeAutomation/tree/master/AsksinPP_developments/sketches/HB-UNI-Sensor1-TH-SHT75)  | hoch genauer TH Sensor<br/>_basierend auf Sensirion SHT75<br/>NEU: und 24h Tagesmittelwerttemperatur<br/>und WebUI Offseteinstellung_ | 🔋 |
|<img src="https://raw.githubusercontent.com/FUEL4EP/HomeAutomation/master/AsksinPP_developments/addons/hb-ep-devices-addon/CCU_RM/src/addon/www/config/img/devices/50/hb-uni-sensor-TH-SHT75_thumb.png" width=25/> | [HB-UNI-SENSOR1-THPD-BME280](https://github.com/FUEL4EP/HomeAutomation/tree/master/AsksinPP_developments/sketches/HB-UNI-Sensor1-THPD-BME280) | THPD Sensor<br/>_basierend auf Bosch BME280<br/>mit Taupunkttemperatur<br/>und absoluter Luftfeuchte<br/>und WebUI Offseteinstellung_ | 🔋 |
|<img src="https://raw.githubusercontent.com/FUEL4EP/HomeAutomation/master/AsksinPP_developments/addons/hb-ep-devices-addon/CCU_RM/src/addon/www/config/img/devices/50/hb-uni-sensor-TH-SHT75_thumb.png" width=25/> | [HB-UNI-SENSOR1-THPD-SHT85](https://github.com/FUEL4EP/HomeAutomation/tree/master/AsksinPP_developments/sketches/HB-UNI-Sensor1-THPD-SHT85)  | hoch genauer THPD Sensor<br/>_basierend auf Sensirion SHT85 und Bosch BME280<br/>mit Taupunkttemperatur<br/>und absoluter Luftfeuchte<br/>NEU: und 24h- und 7 Tagesmittelwerttemperaturen<br/>und WebUI Offseteinstellung_ | 🔋 |
|<img src="https://raw.githubusercontent.com/FUEL4EP/HomeAutomation/master/AsksinPP_developments/addons/hb-ep-devices-addon/CCU_RM/src/addon/www/config/img/devices/50/hb-uni-sensor-RAD-AL53_thumb.png" width=25/> | [HB-UNI-SENSOR1-RAD-AL53](https://github.com/FUEL4EP/HomeAutomation/tree/master/AsksinPP_developments/sketches/HB-UNI-Sensor1-RAD-AL53) | Radioaktivitätssensor für Alpha-, Beta- und Gammastrahlung<br/>_basierend auf Teviso AL53 Sensor<br/>mit gleitender Mittelwertbildung über 7 Tage_ | 🔋 |
|<img src="https://raw.githubusercontent.com/FUEL4EP/HomeAutomation/master/AsksinPP_developments/addons/hb-ep-devices-addon/CCU_RM/src/addon/www/config/img/devices/50/hb-uni-sensor-AQ-BME680_KF_rLF_thumb.png" width=25/> | [HB-UNI-SENSOR1-AQ-BME680_KF_rLF](https://github.com/FUEL4EP/HomeAutomation/tree/master/AsksinPP_developments/sketches/HB-UNI-Sensor1-AQ-BME680_KF_rLF) <br/>- NEU -| selbstkalibrierender Air Quality Sensor<br/>basierend auf Bosch BME680<br/>und Kalman Filter<br/>und WebUI Offseteinstellung (rLF Version) | 🔋 |
|<img src="https://raw.githubusercontent.com/FUEL4EP/HomeAutomation/master/AsksinPP_developments/addons/hb-ep-devices-addon/CCU_RM/src/addon/www/config/img/devices/50/hb-uni-sensor-AQ-BME680_KF_rLF_DEBUG_thumb.png" width=25/> | [HB-UNI-SENSOR1-AQ-BME680_KF_rLF_DEBUG](https://github.com/FUEL4EP/HomeAutomation/tree/master/AsksinPP_developments/sketches/HB-UNI-Sensor1-AQ-BME680_KF_rLF_DEBUG) <br/>- NEU -| selbst kalibrierender Air Quality Sensor<br/>basierend auf Bosch BME680<br/>und Kalman Filter<br/>und WebUI Offseteinstellung (rLF Version)<br/>nur für Debugzwecke!| 🔋 |
|<img src="https://raw.githubusercontent.com/FUEL4EP/HomeAutomation/master/AsksinPP_developments/addons/hb-ep-devices-addon/CCU_RM/src/addon/www/config/img/devices/50/hb-uni-senact-4-4-SC_DS_thumb.png" width=25/> | [HB-UNI-SenAct-4-4-SC_DS](https://github.com/FUEL4EP/HomeAutomation/tree/master/AsksinPP_developments/sketches/HB-UNI-SenAct-4-4-SC_DS)<br/> - NEU -| 3x Schaltaktor für Betrieb an einem **Netzfreischalter** - Akkubetrieb im Standby, automatische Ladeschaltung - bei Aktivierung eines Aktors wird die Netzspannung zugeschaltet und am Ende wieder abgeschaltet - geringer Standby-Verbrauch von ca. 1 mA @ 5V| 🔋 ⚡ |
|<img src="https://raw.githubusercontent.com/FUEL4EP/HomeAutomation/master/AsksinPP_developments/addons/hb-ep-devices-addon/CCU_RM/src/addon/www/config/img/devices/50/hb-uni-senact-4-4-RC_DS_thumb.png" width=25/> | [HB-UNI-SenAct-4-4-RC_DS](https://github.com/FUEL4EP/HomeAutomation/tree/master/AsksinPP_developments/sketches/HB-UNI-SenAct-4-4-RC_DS)<br/> - NEU -| 3x Schaltaktor für Betrieb an einem **Netzfreischalter** - Akkubetrieb im Standby, automatische Ladeschaltung - bei Aktivierung eines Aktors wird die Netzspannung zugeschaltet und am Ende wieder abgeschaltet - geringer Standby-Verbrauch von ca. 0.7 mA @ 5V| 🔋 ⚡ |
<br/>
<br/>
<br/>

#### CCU3/RaspberryMatic Addon:


[hb-ep-devices-addon](https://github.com/FUEL4EP/HomeAutomation/releases/latest)

- bitte die .tgz Datei als Zusatzsoftware auf der CCU3/RaspberryMatic installieren

<br/>

#### Arduino Bibliotheken:


| Name | Beschreibung | Datenblatt |
|--------|--------|:--------:|
| [ABLIC_S35770](https://github.com/FUEL4EP/HomeAutomation/tree/master/AsksinPP_developments/libraries/ABLIC_S35770) | Arduino Bibliothek für ABLIC S-35770<br/>24-Bit Binärzähler mit I2C Schnittstelle | [datasheet](https://www.ablic.com/en/doc/datasheet/counter_ic/S35770_I_E.pdf)|
| [avr_stl](https://github.com/FUEL4EP/HomeAutomation/tree/master/AsksinPP_developments/libraries/avr_stl) | Arduino C++ Standard Library (ArduinoSTL)<br/>abgeleitet von [Giampaolo Mancini](https://github.com/manchoz/avr_stl.git) | [README.md](https://github.com/FUEL4EP/HomeAutomation/tree/master/AsksinPP_developments/libraries/avr_st/README.md)|

<br/>

#### PCBs:

| Name | Beschreibung | 3D Ansicht |
|--------|--------|:--------:|
| [HB-UNI-SEN-BATT_FUEL4EP](https://github.com/FUEL4EP/HomeAutomation/tree/master/AsksinPP_developments/PCBs/HB-UNI-SEN-BATT_FUEL4EP) | AsksinPP Platine basierend auf [Tindie Pro Mini XL - v2 - ATmega 1284p](https://www.tindie.com/products/prominimicros/pro-mini-xl-v2-atmega-1284p/) oder Arduino Pro Mini| [3D Ansicht](https://github.com/FUEL4EP/HomeAutomation/tree/master/AsksinPP_developments/PCBs/HB-UNI-SEN-BATT_FUEL4EP/PNGs/HB-UNI-SEN-BATT_FUEL4EP_top_view.png)|
| [HB-UNI-SEN-BATT_E07-868MS10_FUEL4EP](https://github.com/FUEL4EP/HomeAutomation/tree/master/AsksinPP_developments/PCBs/HB-UNI-SEN-BATT_E07-868MS10_FUEL4EP) | AsksinPP Platine basierend auf<br/> [Tindie Pro Mini XL - v2 - ATmega 1284p](https://www.tindie.com/products/prominimicros/pro-mini-xl-v2-atmega-1284p/) oder Arduino Pro Mini<br/> und [eByte E07-868MS10 Sendemodul](https://www.rcscomponents.kiev.ua/datasheets/e07-868ms10_usermanual_en_v1_20.pdf)| [3D Ansicht](https://github.com/FUEL4EP/HomeAutomation/tree/master/AsksinPP_developments/PCBs/HB-UNI-SEN-BATT_E07-868MS10_FUEL4EP/PNGs/HB-UNI-SEN-BATT_E07-868MS10_FUEL4EP_top_view.png)|
| [HB-UNI-SEN-BATT_ATMega1284P_E07-868MS10_FUEL4EP](https://github.com/FUEL4EP/HomeAutomation/tree/master/AsksinPP_developments/PCBs/HB-UNI-SEN-BATT_ATMega1284P_E07-868MS10_FUEL4EP) | AsksinPP Platine mit SMD Bestückung:<br/> ATMega1284P <br/>[eByte E07-868MS10 Sendemodul](https://www.rcscomponents.kiev.ua/datasheets/e07-868ms10_usermanual_en_v1_20.pdf)<br/>optionalem Aufwärtwandler TPS61221<br/>[optionalem e-Paper Waveshare 1,54 Zoll](https://www.waveshare.com/wiki/1.54inch_e-Paper_Module)| [3D Ansicht 1](https://github.com/FUEL4EP/HomeAutomation/tree/master/AsksinPP_developments/PCBs/HB-UNI-SEN-BATT_ATMega1284P_E07-868MS10_FUEL4EP/PNGs/HB-UNI-SEN-BATT_ATMega1284P_E07-868MS10_FUEL4EP_top_view.png)<br/>[3D Ansicht 2](https://github.com/FUEL4EP/HomeAutomation/tree/master/AsksinPP_developments/PCBs/HB-UNI-SEN-BATT_ATMega1284P_E07-868MS10_FUEL4EP/PNGs/HB-UNI-SEN-BATT_ATMega1284P_E07-868MS10_FUEL4EP_PCB_KiCAD_with_epaper_option.png)|
| [HB-UNI-SEN-BATT_ATMega1284P_E07-868MS10_FRAM_FUEL4EP](https://github.com/FUEL4EP/HomeAutomation/tree/master/AsksinPP_developments/PCBs/HB-UNI-SEN-BATT_ATMega1284P_E07-868MS10_FRAM_FUEL4EP) | AsksinPP Platine mit SMD Bestückung:<br/> ATMega1284P <br/>[eByte E07-868MS10 Sendemodul](https://www.rcscomponents.kiev.ua/datasheets/e07-868ms10_usermanual_en_v1_20.pdf)<br/>optionalem Aufwärtwandler TPS61221<br/>[optionalem e-Paper Waveshare 1,54 Zoll](https://www.waveshare.com/wiki/1.54inch_e-Paper_Module)<br/>optionalen 2x 2Mbit FRAM MB85RS2MTPF| [3D Ansicht 1](https://github.com/FUEL4EP/HomeAutomation/tree/master/AsksinPP_developments/PCBs/HB-UNI-SEN-BATT_ATMega1284P_E07-868MS10_FRAM_FUEL4EP/PNGs/HB-UNI-SEN-BATT_ATMega1284P_E07-868MS10_FRAM_FUEL4EP_top_view.png)<br/>[3D Ansicht 2](https://github.com/FUEL4EP/HomeAutomation/tree/master/AsksinPP_developments/PCBs/HB-UNI-SEN-BATT_ATMega1284P_E07-868MS10_FRAM_FUEL4EP/PNGs/HB-UNI-SEN-BATT_ATMega1284P_E07-868MS10_FRAM_FUEL4EP_PCB_KiCAD_with_epaper_option.png)|
| [HB-UNI-SenAct-4-4-SC_DS_FUEL4EP](https://github.com/FUEL4EP/HomeAutomation/tree/master/AsksinPP_developments/PCBs/HB-UNI-SenAct-4-4-SC_DS_FUEL4EP) | AsksinPP Platine für Durchsteckmontage für 3xSchaltaktor für Betrieb an Netzfreischalter| [3D Ansicht](https://github.com/FUEL4EP/HomeAutomation/tree/master/AsksinPP_developments/PCBs/HB-UNI-SenAct-4-4-SC_DS_FUEL4EP/PNGs/HB-UNI-SenAct-4-4-SC_DS_FUEL4EP_PCB_top_3D.png)|
| [HB-UNI-SEN-BATT_E07-900M10S_FUEL4EP](https://github.com/FUEL4EP/HomeAutomation/tree/master/AsksinPP_developments/PCBs/HB-UNI-SEN-BATT_E07-900M10S_FUEL4EP) - NEU -| AsksinPP Platine basierend auf<br/> [Tindie Pro Mini XL - v2 - ATmega 1284p](https://www.tindie.com/products/prominimicros/pro-mini-xl-v2-atmega-1284p/) oder Arduino Pro Mini<br/> und [eByte E07-900M10S Sendemodul](https://www.manualslib.com/manual/2937348/Ebyte-E07-900m10s.html)| [3D Ansicht](https://github.com/FUEL4EP/HomeAutomation/tree/master/AsksinPP_developments/PCBs/HB-UNI-SEN-BATT_E07-900M10S_FUEL4EP/PNGs/HB-UNI-SEN-BATT_E07-900M10S_FUEL4EP_PCB_3D_top.png)|
| [CC1101_868MHz_RF_Modul_FUEL4EP](https://github.com/FUEL4EP/HomeAutomation/tree/master/AsksinPP_developments/PCBs/CC1101_868MHz_RF_Modul_FUEL4EP) - NEU -| AsksinPP RF 868.3 MHz Funkmodule als Ersatz für eByte E07-868M10S| [3D Ansicht](https://github.com/FUEL4EP/HomeAutomation/tree/master/AsksinPP_developments/PCBs/CC1101_868MHz_RF_Modul_FUEL4EP/PNGs/CC1101_868MHz_RF_Modul_FUEL4EP_PCB_3D_top.png)|
| [CC1101_868MHz_UFL_RF_Modul_FUEL4EP](https://github.com/FUEL4EP/HomeAutomation/tree/master/AsksinPP_developments/PCBs/CC1101_868MHz_UFL_RF_Modul_FUEL4EP) - NEU -| AsksinPP RF 868.3 MHz Funkmodule als Ersatz für eByte E07-868M10S mit U.FL Antennenbuchse| [3D Ansicht](https://github.com/FUEL4EP/HomeAutomation/tree/master/AsksinPP_developments/PCBs/CC1101_868MHz_UFL_RF_Modul_FUEL4EP/PNGs/CC1101_868MHz_UFL_RF_Modul_FUEL4EP_PCB_3D_top.png)|
| [4Bit_I2C_Port_Expander_Module_FUEL4EP](https://github.com/FUEL4EP/HomeAutomation/tree/master/AsksinPP_developments/PCBs/4Bit_I2C_Port_Expander_Module_FUEL4EP) - NEU -| I2C Portexpander| [3D Ansicht](https://github.com/FUEL4EP/HomeAutomation/tree/master/AsksinPP_developments/PCBs/4Bit_I2C_Port_Expander_Module_FUEL4EP/PNGs/4Bit_I2C_Port_Expander_Module_FUEL4EP_PCB_3D_top.png)|
| [I2C_Module_BME280_FUEL4EP](https://github.com/FUEL4EP/HomeAutomation/tree/master/AsksinPP_developments/PCBs/I2C_Module_BME280_FUEL4EP) - NEU -| I2C BME280 Sensor Modul| [3D Ansicht](https://github.com/FUEL4EP/HomeAutomation/tree/master/AsksinPP_developments/PCBs/I2C_Module_BME280_FUEL4EP/PNGs/I2C_Module_BME280_FUEL4EP_PCB_3D_top.png)|

<br/>

#### Installation von Sensoren oder Platinen auf einem lokalen Computer

- Herunterladen des Sammelrepositories in ein lokales Zielverzeichnis mit dem Kommando:
	+	git clone https://github.com/FUEL4EP/HomeAutomation.git
	+	alternativ kann vom Github [Sammelrepository](https://github.com/FUEL4EP/HomeAutomation) die ZIP-Datei HomeAutomation-master.zip heruntergeladen und auf dem lokalen Rechner ausgepackt werden. Bitte dann regelmäßig auf Github nach Updates schauen.
- alle zu einem Sensor <Sensorname\> gehörenden Installationsdaten stehen dann unter
	+ HomeAutomation/AsksinPP_developments/sketches/<Sensorname\>
	* der Sensor Sketch
		+ HomeAutomation/AsksinPP_developments/sketches/<Sensorname\>/<Sensorname\>.ino<br/>kann direkt im Arduino IDE aufgerufen werden
- alle zu einer Platine <Platinenname\> gehörenden Installationsdaten stehen dann unter
	- HomeAutomation/AsksinPP_developments/PCBs/<Platinenname\>
  - die Platine
	+ HomeAutomation/AsksinPP_developments/PCBs/<Platinenname\>/<Platinenname\>.kicad_pro<br/>
  kann direkt mit KiCAD 6 aufgerufen werden
<br/>
<br/>

#### Installation von Bibliotheken auf lokalem Computer

- Herunterladen des Sammelrepositories in ein lokales Zielverzeichnis mit dem Kommando:
	+	git clone https://github.com/FUEL4EP/HomeAutomation.git
	+	alternativ kann vom Github [Sammelrepository](https://github.com/FUEL4EP/HomeAutomation) die ZIP-Datei HomeAutomation-master.zip heruntergeladen und auf dem lokalen Rechner ausgepackt werden. Bitte dann regelmäßig auf Github nach Updates schauen.
- bereitgestellte Bibliotheken sind als ganze Verzeichnisse von HomeAutomation/AsksinPP_developments/libraries in das lokale Bibliotheksverzeichnis der Arduino IDE Entwicklungsumgebung zu kopieren
<br/>

#### Fragen und Probleme

- Fragen und Probleme bitte als Github Issue [oben](https://github.com/FUEL4EP/HomeAutomation/issues) oder im [Homematic Forum](https://homematic-forum.de/forum/viewforum.php?f=76) adressieren.
<br/>
<br/>
<br/>

<a rel="license" href="http://creativecommons.org/licenses/by-nc-sa/4.0/"><img alt="Creative Commons Lizenzvertrag" style="border-width:0" src="https://i.creativecommons.org/l/by-nc-sa/4.0/88x31.png" /></a><br />Dieses Werk ist lizenziert unter einer <a rel="license" href="http://creativecommons.org/licenses/by-nc-sa/4.0/">Creative Commons Namensnennung - Nicht-kommerziell - Weitergabe unter gleichen Bedingungen 4.0 International Lizenz</a>.

<br/>
<br/>



#### Die verwendeten Icons sind "free for non-commercial use" von
 
- http://icons8.com
- https://icon-library.com
- https://thenounproject.com
- Vielen Dank an hmpatman für die Bereitstellung eines Icons
