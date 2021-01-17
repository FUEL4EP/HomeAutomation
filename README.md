# HomeAutomation
## Home Automation Repository von FUEL4EP


### Homebrew [Asksinpp](https://asksinpp.de/) Sensoren und benötigtes [CCU3/RM Addon](https://github.com/FUEL4EP/HomeAutomation/releases/latest):

#### Sensors:


|  | Name | Beschreibung | 🔋Batterie-/<br/>⚡️Netzbetrieb |
|--------|--------|--------|:--------:|
|<img src="https://raw.githubusercontent.com/FUEL4EP/HomeAutomation/master/AsksinPP_developments/addons/hb-ep-devices-addon/CCU_RM/src/addon/www/config/img/devices/50/hb-uni-sensor-CO2-SCD30_thumb.png" width=25/> | [HB-UNI-SENSOR1-CO2-SCD30](https://github.com/FUEL4EP/HomeAutomation/tree/master/AsksinPP_developments/sketches/HB-UNI-Sensor1-CO2_SCD30) | genauer NDIR CO2 Sensor<br/>_basierend auf Sensirion SCD30<br/>Akku- oder Netzbetrieb<br/>und WebUI Offseteinstellung_ | 🔋 ⚡ |
|<img src="https://raw.githubusercontent.com/FUEL4EP/HomeAutomation/master/AsksinPP_developments/addons/hb-ep-devices-addon/CCU_RM/src/addon/www/config/img/devices/50/hb-uni-sensor-AQ-BME680_thumb.png" width=25/> | [HB-UNI-SENSOR1-AQ-BME680](https://github.com/FUEL4EP/HomeAutomation/tree/master/AsksinPP_developments/sketches/HB-UNI-Sensor1-AQ-BME680) | selbstkalibrierender Air Quality Sensor<br/>_basierend auf Bosch BME680<br/>und mulipler linearer Regression<br/>und WebUI Offseteinstellung_ | 🔋 |
|<img src="https://raw.githubusercontent.com/FUEL4EP/HomeAutomation/master/AsksinPP_developments/addons/hb-ep-devices-addon/CCU_RM/src/addon/www/config/img/devices/50/hb-uni-sensor-AQ-BME680_KFthumb.png" width=25/> | [HB-UNI-SENSOR1-AQ-BME680_KF](https://github.com/FUEL4EP/HomeAutomation/tree/master/AsksinPP_developments/sketches/HB-UNI-Sensor1-AQ-BME680_KF) | selbstkalibrierender Air Quality Sensor<br/>_basierend auf Bosch BME680<br/>und Kalman Filter<br/>und WebUI Offseteinstellung_ | 🔋 |
|<img src="https://raw.githubusercontent.com/FUEL4EP/HomeAutomation/master/AsksinPP_developments/addons/hb-ep-devices-addon/CCU_RM/src/addon/www/config/img/devices/50/hb-uni-sensor-TH-SHT75_thumb.png" width=25/> | [HB-UNI-SENSOR1-TH-SHT75](https://github.com/FUEL4EP/HomeAutomation/tree/master/AsksinPP_developments/sketches/HB-UNI-Sensor1-TH-SHT75) | hochgenauer TH Sensor<br/>_basierend auf Sensirion SHT75<br/>und WebUI Offseteinstellung_ | 🔋 |
|<img src="https://raw.githubusercontent.com/FUEL4EP/HomeAutomation/master/AsksinPP_developments/addons/hb-ep-devices-addon/CCU_RM/src/addon/www/config/img/devices/50/hb-uni-sensor-TH-SHT75_thumb.png" width=25/> | [HB-UNI-SENSOR1-THPD-BME280](https://github.com/FUEL4EP/HomeAutomation/tree/master/AsksinPP_developments/sketches/HB-UNI-Sensor1-THPD-BME280) | THPD Sensor<br/>_basierend auf Bosch BME280<br/>mit Taupunkttemperatur<br/>und absoluter Luftfeuchte<br/>und WebUI Offseteinstellung_ | 🔋 |

<br/>
<br/>
<br/>

#### CCU3/RaspberryMatic Addon:


[hb-ep-devices-addon](https://github.com/FUEL4EP/HomeAutomation/releases/latest)

- bitte als Zusatzsoftware auf der CCU3/RaspberryMatic installieren

<br/>
<br/>
<br/>


#### Installation von Sensoren auf lokalem Computer

- Herunterladen des Sammelrepositories in ein lokales Zielverzeichnis mit dem Kommando:
	+	git clone https://github.com/FUEL4EP/HomeAutomation.git
- alle zu einem Sensor <Sensorname\> gehörenden Installationsdaten stehen dann unter
	+ HomeAutomation/AsksinPP_developments/sketches/<Sensorname\>
- der Sensor Sketch
	+ HomeAutomation/AsksinPP_developments/sketches/<Sensorname\>/<Sensorname\>.ino<br/><br/>
  kann direkt im Arduino IDE aufgerufen werden

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