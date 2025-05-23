
# HB-UNI-Sensor1-TH-SHT75 [![License: CC BY-NC-SA 4.0](https://img.shields.io/badge/License-CC%20BY--NC--SA%204.0-lightgrey.svg)](https://creativecommons.org/licenses/by-nc-sa/4.0/)  <a href='https://ko-fi.com/FUEL4EP' target='_blank'><img height='20' style='border:0px;height:20px;' src='https://cdn.ko-fi.com/cdn/kofi1.png?v=2' border='0' alt='Buy Me a Coffee at ko-fi.com' /></a>

# Genauer Temperatur- und Luftfeuchtesensor auf der Basis von dem Sensirion SHT75 Sensor (HB-UNI-Sensor1-TH-SHT75)

- abgeleitet von Toms ([TomMajor](https://github.com/TomMajor)) [HB-UNI-Sensor1](https://github.com/TomMajor/SmartHome/tree/master/HB-UNI-Sensor1)
- ein herzliches Dankeschön für die Basisarbeit geht an Tom (TomMajor)
- ein herzliches Dankeschön an alle, die im Homematic Forum geholfen haben, meine Probleme zu lösen
- hilfreich ist auch die Diskussion zu den [rftypes XMLs](https://homematic-forum.de/forum/viewtopic.php?f=76&t=62578&sid=cf0f4cd99f7ee2bf070e9f39391ee652)

![pic](Images/HB-UNI-Sensor1_SHT75.png)	

Der Sensirion SHT75 ist ein ein wenig betagter Temperatur- und Luftfeuchtesensor mit hoher Genauigkeit: **SHT7x reached end-of-life. Last order date: 31.12.2018 / Last delivery date: 31.12.2019 SHT7x is replaced by SHT85. For more information, please contact info@sensirion.com.**

Das Datenblatt des SHT75 Sensors ist [hier](https://www.sensirion.com/fileadmin/user_upload/customers/sensirion/Dokumente/2_Humidity_Sensors/Datasheets/Sensirion_Humidity_Sensors_SHT7x_Datasheet.pdf) zu finden.

Die Genauigkeit der Temperaturmessung des SHT75 Sensors ist typisch +-0.3 Grad Celsius.
Die Genauigkeit der Luftfeuchtemessung des SHT75 Sensors ist typisch +-1.8% rH.

**Es gibt von Sensirion inzwischen das Nachfolgemodell SHT85.** Der Übergang von SHT75 auf SHT85 ist [hier](https://media.digikey.com/pdf/Data%20Sheets/Sensirion%20PDFs/HT_AN_SHT85_Transitioning_SHT7x_to_SHT85_V0.9_D1.pdf) beschrieben. Der SHT85 Sensor nutzt einen I2C Bus und ist daher **NICHT** Software kompatibelzu SHT75!. 
Inzwischen gibt es auch dafür einen Sketch [HB-UNI-Sensor1-THPD-SHT85](https://github.com/FUEL4EP/HomeAutomation/tree/master/AsksinPP_developments/sketches/HB-UNI-Sensor1-THPD-SHT85)

**Update 04. März 2024:** Da der Sensirion SHT75 Sensor de facto nicht mehr käuflich ist, wurde als Ersatz der neue Sketch [HB-UNI-Sensor1-THD-SHT45](https://github.com/FUEL4EP/HomeAutomation/tree/master/AsksinPP_developments/sketches/HB-UNI-Sensor1-THD-SHT45) entwickelt.


## Update auf Firmware 1.1 verfügbar

- die neue Firmwareversion 1.1 unterstützt die Berechnung eines gleitenden Mittelwerts der gemessenen Temperatur über 24 Stunden
- hier am Beispiel der Oberflächentemperatur eines Grundofens, der einmal pro Tag befeuert wird:
![pic](Images/24h_moving_average_example.png)
- da mit dieser Option der RAM Speicherplatz fast vollständig ausgeknautscht wird, bitte keine weiteren Code-Änderungen vornehmen.
- nach erfolgreichem Anlernen an die Zentrale und Test, bitte die NDEBUG Option aktivieren (siehe unten).
- vor dem Aufspielen der neuen Firmware bitte den Sensor in der Zentrale komplett ablernen und dann mit der neu geflashten Firmware wieder anlernen
- der 24h gleitende Mittelwert braucht 24 Stunden zum Einschwingen. Nach einem Reset startet der gleitende Mittelwert bei Null.

## Neue Eigenschaften im Vergleich zum HB-UNI-Sensor1 Sensor

- die relative Luftfeuchtigkeit wird mit 0,1 % rLF Genauigkeit ausgegeben
- die Batteriespannung wird mit 10 mV Genauigkeit und jeden Zyklus ausgegeben 
- alle wichtigen Sensorparameter können interaktiv ohne Neuprogrammierung im WebUI der [RaspberryMatic](https://github.com/jens-maus/RaspberryMatic) / [CCU3](https://de.elv.com/smart-home-zentrale-ccu3-inklusive-aio-creator-neo-lizenz-ccu-plugin-151965?fs=2591490946) eingegeben werden:
	+ [Startseite > Einstellungen > Geräte > Geräte-/ Kanalparameter einstellen](Images/Setting_of_device_parameters_in_WebUI.png)
	

## Schaltung

- basierend auf einer ATmega328P MCU mit CC1101 Sendemodul, Eigenbau möglich
- eine mögliche und von mir empfohlene Hardwarebasis sieht so aus:
	+ Universalplatine für DIY-Bausätze von Alexander Reinert
		* Github [HB-UNI-SEN-BATT](https://github.com/alexreinert/PCB#hb-uni-sen-batt)
		    + direkt bei JLCPCB oder bei Alexander Reichert nachfragen
		    + kommerzielle Bezugsquelle: [Smartkram WebShop](https://smartkram.de/produkt/universalplatine-fuer-diy-bausatze-von-alex-reinert/)
		* oder alternativ [HB-UNI-SEN-BATT_FUEL4EP](https://github.com/FUEL4EP/HomeAutomation/tree/master/AsksinPP_developments/PCBs/HB-UNI-SEN-BATT_FUEL4EP)
		  + der Abstandsfehler für einen Batteriehalter in [HB-UNI-SEN-BATT](https://github.com/alexreinert/PCB#hb-uni-sen-batt) ist korrigiert
		  + als Batteriehalter wird der Keystone 2460 verwendet, ein Goobay 1xAA Batteeriehalter passt auch
		  + statt des Tindie Pro Mini XL V2 bitte einen Arduino Pro Mini einsetzen, er passt auch, für den Tindie Pro Mini XL V2 müsste der Sketch angepasst werden
		  + Bestellung direkt bei JLCPCB oder per PN bei bei [FUEL4EP](https://homematic-forum.de/forum/ucp.php?i=pm&mode=compose&u=20685) anfragen
	  * oder alternativ [HB-UNI-SEN-BATT_E07-868MS10_FUEL4EP](https://github.com/FUEL4EP/HomeAutomation/tree/master/AsksinPP_developments/PCBs/HB-UNI-SEN-BATT_E07-868MS10_FUEL4EP) mit einem Ebyte E07 868MS10 Funkmodul (rote Platine), das ein wenig teurer als das noname grüne CC1101 Funkmodul ist, aber weniger Ärger macht
		  + der Abstandsfehler für einen Batteriehalter in [HB-UNI-SEN-BATT](https://github.com/alexreinert/PCB#hb-uni-sen-batt) ist korrigiert
		  + als Batteriehalter wird der Keystone 2460 verwendet, ein Goobay 1xAA Batteeriehalter passt auch
		  + statt des Tindie Pro Mini XL V2 bitte einen Arduino Pro Mini einsetzen, er passt auch, für den Tindie Pro Mini XL V2 müsste der Sketch angepasst werden
		  + Bestellung direkt bei JLCPCB oder per PN bei bei [FUEL4EP](https://homematic-forum.de/forum/ucp.php?i=pm&mode=compose&u=20685) anfragen				
    * Sensorgehäuse
	    * 3D-Druck [HB-UNI-SEN-BATT snap Gehaeuse und Deckel](https://www.thingiverse.com/thing:3512767)	auf Thingiverse
	    * oder [fertiges Gehäuse](https://smartkram.de/produkt/sensorgehaeuse-passend-fuer-platine-von-alexander-reinert/) von Smartkram Webshop
	+ Bauteile
	    + Reichelt [Bestellliste](https://www.reichelt.de/my/1882914
)
           + Arduino Pro Mini, CC1101 Tranceiver, Batteriehalter und SHT75 müssen separat bestellt werden, den BME280 bitte weglassen

	    + oder Smartkram Webshop [HB-UNI-SEN-BATT](https://smartkram.de/produkt/bauteile-fuer-homematic-diy-projekt-thermometer-hydrometer/). Der BME280 ist dann übrig :-(
       + Arduino Pro Mini und CC1101 Tranceiver sind gesockelt
       + Sensirion SHT75 Sensor
	       + Bezugsquelle: [Conrad](https://www.conrad.de/de/p/temperatursensoren-sht-75-humidity-temperature-sensor-1-8-rh-0-3-c-sbus-sil4-503493.html)
	+ Aufbau entsprechend siehe [Technikkram](https://technikkram.net/blog/2018/05/30/homematic-diy-projekt-thermometer-und-hydrometer-fertige-platine-im-eigenbau/), bitte geeignet anpassen
	+ bitte darauf achten, dass der Arduino Pro Mini die 3,3V/8MHz Version ist
	+ die Verbindungen zwischen HB-UNI-SEN-BATT PCB und SHT75 mit flexiblem Flachbandkabel ausführen
	+ Schnittstelle des SHT75 Sensors zur MCU isst kein I2C. Details sind im [Datenblatt](https://www.sensirion.com/fileadmin/user_upload/customers/sensirion/Dokumente/2_Humidity_Sensors/Datasheets/Sensirion_Humidity_Sensors_SHT7x_Datasheet.pdf) in dem Kapitel 2, Seite 4 zu finden. Notwendige Verbindungen sind:
		* PCB SCL <-> SHT75 Pin 1
		* PCB SDA <-> SHT75 Pin 4
		* PCB VCC <-> SHT75 Pin 2
		* PCB GND <-> SHT75 Pin 3
	+ 1x 10 kOhm Abschlusswiderstand für SDA auf HB-UNI-SEN-BATT PCB einlöten
	
## Frequenztest des CC1101 RF Moduls

- Bitte vor dem Aufspielen des eigentlichen Sketches HB-UNI-Sensor1-TH-SHT75 UNBEDINGT einen [Frequenztest ATMega328P](https://github.com/pa-pa/AskSinPP/blob/master/examples/FreqTest/FreqTest.ino) durchführen. Viele CC1101 Module lassen sich ohne diesen Frequenztest nicht anlernen!
- Eine Beschreibung des Frequenztests ist [hier](https://asksinpp.de/Grundlagen/FAQ/Fehlerhafte_CC1101.html#ermittlung-der-cc1101-frequenz) zu finden.
		
## Verringerung des Ruhestroms: [siehe auch Referenz von TomMajor](https://github.com/TomMajor/SmartHome/tree/master/Info/Ruhestrom)

- auf dem Arduino Pro Mini sollte der LDO Spannungsregler (die Stelle ist im Bild mit 1 markiert) sowie die Power-LED (2) entfernt werden, um den Ruhestrom wesentlich zu verringern.  
- weiterhin kann die zweite LED am Arduino Pin 13 (SCK) entfernt werden (3). Deren Einfluß auf den Ruhestrom ist kleiner als bei LDO und Power-LED, trotzdem kann es sich auf lange Sicht lohnen da diese bei jeder CC1101 Kommunikation für kurze Zeit aktiv ist, besonders im BurstDetector Mode.

![pic](Images/ProMini_LDO_LED.png)	

- Brown-Out_Detektor des ATmega328P  (BOD) ausschalten, siehe Einstellungen Arduino IDE unten unter Punkt **Benötigter Sketch**
- bitte danach den [SleepTest](https://github.com/TomMajor/SmartHome/blob/master/Info/Ruhestrom/SleepTest/SleepTest.ino) durchführen. Der gemessene Ruhestrom im Sleep-Modus sollte < 10 uA betragen. Ist der gemessene Strom nach ein paar Sekunden höher, ist ein Bauteil defekt und muss ausgetauscht werden. Da das Auslöten immer schwierig ist, ist ein gesockelter Einbau zu empfehlen.
	       

## Das angemeldete Gerät im RaspberryMatic WebUI

![pic](Images/WebUI.png)


## Vor dem Aufspielen von Software

- bitte macht Euch zuerst mit den Grundlagen von AsksinPP [hier](https://asksinpp.de/Grundlagen/) vertraut.

## Bitte immer die aktuellste Version von AsksinPP nutzen

- [AsksinPP Master](https://github.com/pa-pa/AskSinPP/tree/master)

## Bitte genau diese Abfolge beim Einspielen von Software beachten:

- Details sind gegebenenfalls weiter unten zu finden

1. bitte zuerst nochmals vergewissern, dass der verbaute Arduino Pro Mini ein 3,3V/8MHz Typ ist.
2. das benötigte Addon [hb-ep-devices-addon](https://github.com/FUEL4EP/HomeAutomation/releases/latest) auf Eure CCU3/RaspberryMatic installieren.
3. für das Programmierern und Setzen der Fuses des ATmega328P ist ein ISP Programmer empfohlen. Eine Anleitung ist [hier](https://asksinpp.de/Grundlagen/04-isp.html) zu finden. Dabei bitte unbedingt den ISP Programmer auf 3,3V einstellen!
4. Setzten der richtigen Fuses mit dem [avrdude script](avrdude/avrdude_328P.bsh) (LINUX version) oder mit [AVRDUDESS](https://blog.zakkemble.net/avrdudess-a-gui-for-avrdude/) (Windows Version): Fuse Setting: Low Byte:0xFF  High Byte:0xD2  Extended Byte:0xFF. Dafür bitte einen ISP Programmer verwenden, siehe [hier](https://asksinpp.de/Grundlagen/04-isp.html). Ich persönlich nutze diesen [ISP Programmer](https://www.amazon.de/Diamex-Programmer-XMEGA-ATMEGA-ATtiny/dp/B0064LLRB0).
5. dann den [Frequenztest](https://asksinpp.de/Grundlagen/FAQ/Fehlerhafte_CC1101.html#ermittlung-der-cc1101-frequenz) durchführen. Dazu den [FreqTest.ino Sketch](https://github.com/pa-pa/AskSinPP/blob/master/examples/FreqTest/FreqTest.ino) ausführen und dabei auf ein erfolgreiches Beenden achten.
6. gegebenenfalls die Parameter cDEVICE_und cDEVICE_in [Device_SHT75.h](Device_SHT75.h) ändern. Jeder Sensor muss ein eineindeutiges cDEVICE_und cDEVICE haben!
7. die aktuelle Version des [HB-UNI-Sensor1-TH-SHT75](https://github.com/FUEL4EP/HomeAutomation/tree/master/AsksinPP_developments/sketches/HB-UNI-Sensor1-TH-SHT75) Sketches herunterladen, siehe unten.
8. die benötigten Libraries installieren, siehe unten unter 'Benötigte Libraries'.
9. dann den Sketch [HB-UNI-Sensor1-TH-SHT75.ino](https://github.com/FUEL4EP/HomeAutomation/tree/master/AsksinPP_developments/sketches/HB-UNI-Sensor1-TH-SHT75/HB-UNI-Sensor1-TH-SHT75.ino) kompilieren und mit dem ISP Programmer auf den Arduino Pro Mini hochladen.
10. Den neuen Sensor in der CCU/RaspBerryMatic anlernen.
11. Den neuen Sensor in der CCU3/RaspberryMatic dem Gewerk 'Wetter' zuordnen.


## Benötiger Sketch

[HB-UNI-Sensor1_SHT75](https://github.com/FUEL4EP/HomeAutomation/tree/master/AsksinPP_developments/sketches/HB-UNI-Sensor1-TH-SHT75)

- bitte alle Unterverzeichnisse mit kopieren:

### Zur lokalen Installation des Github Releases auf Deinem Computer

- gehe bitte in Dein Zielinstallationsverzeichnis, wo Du mit dem HB-UNI-Sensor1-TH-SHT75 Sensor arbeiten möchtest

  - gebe dort 'git clone https://github.com/FUEL4EP/HomeAutomation.git' ein
	  + damit lädst Du mein [Sammelrepository](https://github.com/FUEL4EP/HomeAutomation) für alle meine auf Github freigegebenen HomeBrew Sensoren herunter.
  - dann findest Du ein neues Verzeichnis 'HomeAutomation' auf Deinem Rechner, das alle meine auf Github freigebenen Sensoren enthält, siehe [README.md](https://github.com/FUEL4EP/HomeAutomation/blob/master/README.md)
  	+ mache bitte regelmäßig ein Update mit 'git pull'
 -	enthalten ist auch das notwendige Addon '[ep-hb-devices-addon](https://github.com/FUEL4EP/HomeAutomation/releases/latest)'
 -	den HB-UNI-Sensor1-TH-SHT75 Sensor findest Du unter './HomeAutomation/tree/master/AsksinPP_developments/sketches/HB-UNI-Sensor1-TH-SHT75'
 
- alternativ kann vom Github [Sammelrepository](https://github.com/FUEL4EP/HomeAutomation) die ZIP-Datei HomeAutomation-master.zip heruntergeladen und auf dem lokalen Rechner ausgepackt werden. Bitte dann regelmäßig auf Github nach Updates schauen.

- nach erfolgreicher Inbetriebnahme können die Debugausgaben im serial Monitor ausgeschaltet werden. Dazu bitte im Sketch HB-UNI-Sensor1-TH-SHT75.ino auskommentieren:

> // !! NDEBUG sollte aktiviert werden wenn die Sensorentwicklung und die Tests abgeschlossen sind und das Gerät in den 'Produktionsmodus' geht.<br/>
> // Insbesondere die RAM-Einsparungen sind wichtig für die Stabilität / dynamische Speicherzuweisungen etc.<br/>
> // Dies beseitigt dann auch die mögliche Arduino-Warnung 'Low memory available, stability problems may occur'.<br/>
> //<br/>
> //#define NDEBUG

- als Taktfrequenz des ATmega328P 8 MHz @ 3.3V externer Quarz einstellen
- Der Sketch verwendet 27636 Bytes (89%) des Programmspeicherplatzes. Das Maximum sind 30720 Bytes.
Globale Variablen verwenden 1876 Bytes (91%) des dynamischen Speichers, 172 Bytes für lokale Variablen verbleiben. Das Maximum sind 2048 Bytes.
- Beim Kompilieren und Linken gibt es eine Warnung:
    + Wenig Arbeitsspeicher verfügbar, es können Stabilitätsprobleme auftreten.

- [Fuses Calculator](http://eleccelerator.com/fusecalc/fusecalc.php); select ATmega328P

- [avrdude script](avrdude/avrdude_328P.bsh) (LINUX version)
	- wichtig ist dass dieser Skript **VOR** den Flashen des Programmcodes ausgeführt wird.  Das EESAVE Konfigurationsbit des Atmega328P muss gesetzt sein (Preserve EEPROM memory through the Chip Erase cycle; [EESAVE=1])


- Die Programmierung erfolgt mit einem ISP Programmer, z.B. Diamex ISP USB Programmer. Dazu dienen die Signale VCC, GND, MOSI, SCK, MISO,
RSET an der Steckerleiste unten rechts in der Basisplatine. Dort eine Steckerleiste einlöten.
- ISP Programmer auf 3,3V einstellen!
- Einstellungen Arduino IDE: 8MHz, 3.3V
- Hochladen des kompilierten Sketchs im Arduino IDE mit: Sketch => Hochladen mit Programmer
- Debugging wird über den seriellen Monitor mit einem FTDI Adapter USB zu TTL Serial für
3,3V und 5V für Arduino gemacht. Als Baudrate des seriellen Monitors bitte **57600** Baud einstellen.



## Benötigte Libraries

+ [AskSinPP Library](https://github.com/pa-pa/AskSinPP)</br>
+ [EnableInterrupt](https://github.com/GreyGnome/EnableInterrupt)</br>
+ [Low-Power](https://github.com/rocketscream/Low-Power)</br>
+ [spease Sensirion](https://github.com/spease/Sensirion)


## Benötigtes Addon

[hb-ep-devices-addon](https://github.com/FUEL4EP/HomeAutomation/releases/latest)

- bitte dieses Addon 'hb-ep-devices-addon.tgz 'vor dem Anlernen des HB-UNI-Sensor1-TH-SHT75 Sensors auf der RaspberryMatic / CCU3 installieren (kein unzip vonnöten!)
- bitte gegebenenfalls das Addon nach einem Update der CCU3/RaspberryMatic Firmware erneut installieren, falls Geräteparameter fehlen.
- die minimale Version des Addons ist 1.13.

## Verringerung der Tx Sendeleistung

- nur Experten wird empfohlen, die Tx Sendeleistung zu verringern. Die Beschreibung ist [hier](./Reduction_of_Tx_RF_power/README.md).

## Lizenz

**Creative Commons BY-NC-SA**<br>
Give Credit, NonCommercial, ShareAlike

<a rel="license" href="http://creativecommons.org/licenses/by-nc-sa/4.0/"><img alt="Creative Commons License" style="border-width:0" src="https://i.creativecommons.org/l/by-nc-sa/4.0/88x31.png" /></a><br />This work is licensed under a <a rel="license" href="http://creativecommons.org/licenses/by-nc-sa/4.0/">Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License</a>.
