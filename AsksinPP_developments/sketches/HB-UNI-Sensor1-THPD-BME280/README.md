# HB-UNI-Sensor1-THPD-BME280 [![License: CC BY-NC-SA 4.0](https://img.shields.io/badge/License-CC%20BY--NC--SA%204.0-lightgrey.svg)](https://creativecommons.org/licenses/by-nc-sa/4.0/) [![Hits](https://hits.seeyoufarm.com/api/count/incr/badge.svg?url=https%3A%2F%2Fgithub.com%2FFUEL4EP%2FHomeAutomation%2FAsksinPP_developments%2Fsketches%2FHB-UNI-Sensor1-THPD-BME280&count_bg=%2379C83D&title_bg=%23555555&icon=&icon_color=%23E7E7E7&title=hits&edge_flat=false)](https://hits.seeyoufarm.com)
<br/>


# Temperatur-, Luftfeuchte und Luftdrucksensor auf der Basis von dem [Bosch BME280 Sensor](https://www.bosch-sensortec.com/media/boschsensortec/downloads/datasheets/bst-bme280-ds002.pdf) (HB-UNI-Sensor1-THPD-BME280) mit Ausgabe von Taupunkttemperatur, absoluter Luftfeuchte und Batteriespannung und WebUI Offseteinstellung

- abgeleitet von Toms ([TomMajor](https://github.com/TomMajor)) [HB-UNI-Sensor1](https://github.com/TomMajor/SmartHome/tree/master/HB-UNI-Sensor1)
- ein herzliches Dankeschön für die Basisarbeit geht an Tom (TomMajor)
- ein herzliches Dankeschön an alle, die im Homematic Forum geholfen haben, meine Probleme zu lösen
- hilfreich ist auch die Diskussion zu den [rftypes XMLs](https://homematic-forum.de/forum/viewtopic.php?f=76&t=62578&sid=cf0f4cd99f7ee2bf070e9f39391ee652)

Das Datenblatt des BME280 Sensors ist [hier](https://www.bosch-sensortec.com/media/boschsensortec/downloads/datasheets/bst-bme280-ds002.pdf) zu finden.

Der Diskussionsstrang im Homematic Forum dazu ist [hier](https://homematic-forum.de/forum/viewtopic.php?f=76&t=65320&p=642213#p642213) zu finden. Bitte dort auch Fragen stellen.

## Bitte Addon 'ep-hb-devices-addon' auf die Version 1.11 updaten

- Bitte gegebenenfalls das Addon nach einem Update der CCU3/RaspberryMatic Firmware erneut installieren, falls Geräteparameter fehlen.


## Hinweis: Alternativer schneller und hoch genauer HomeBrew Sensor
Wer in seiner Applikation einen schnellen und/oder hoch genauen Sensor braucht, kann den neuen Sensor [HB-UNI-Sensor1-THPD-SHT85](https://github.com/FUEL4EP/HomeAutomation/tree/master/AsksinPP_developments/sketches/HB-UNI-Sensor1-THPD-SHT85) nehmen. Er ist zusätzlich mit einem Sension SHT85 Sensor bestückt, sonst aber funktional identisch. Ein Vergleich der unterschiedlich schnellen Temperaturtransienten ist unter [HB-UNI-Sensor1-THPD-SHT85](https://github.com/FUEL4EP/HomeAutomation/tree/master/AsksinPP_developments/sketches/HB-UNI-Sensor1-THPD-SHT85) zu finden.




## Neue Eigenschaften im Vergleich zum [HB-UNI-Sensor1](https://github.com/TomMajor/SmartHome/tree/master/HB-UNI-Sensor1) Sensor

- die relative Luftfeuchtigkeit wird mit 0.1 % rLF Genauigkeit ausgegeben
- der Luftdruck wird mit 0.1 hPa Genauigkeit ausgegeben
- die Taupunkttemperatur wird mit 0.1 K Genauigkeit ausgegeben
- die absolute Luftfeuchte wird mit 0.01 g/m³ Genauigkeit ausgegeben
- die Batteriespannung wird mit 10 mV Genauigkeit und jeden Zyklus ausgegeben 
- alle wichtigen Sensorparameter können interaktiv ohne Neuprogrammierung im WebUI der [RaspberryMatic](https://github.com/jens-maus/RaspberryMatic) / [CCU3](https://de.elv.com/smart-home-zentrale-ccu3-inklusive-aio-creator-neo-lizenz-ccu-plugin-151965?fs=2591490946) eingegeben werden:
	+ [Startseite > Einstellungen > Geräte > Geräte-/ Kanalparameter einstellen](Images/BME280_Setting_of_device_parameters_in_WebUI.png)
	+ Alle drei Offsetwerte (T, rLF, P) müssen für die Eingabe mit dem Faktor 10 multipliziert werden.
- die Tx Sendeleistung des 868MHz Funkmoduls kann eingestellt werden (nur für Experten!)
	

## Taupunkttemperatur und absolute Luftfeuchtigkeit

- Die angezeigte Taupunkttemperatur kann zur Bewertung der Gefährdung einer Schimmelbildung in einem Raum verwendet werden: Der kälteste Punkt eines Raums (in der Regel die untere oder oberere Ecke einer Außenwand) sollte eine mit einem Infrarotthermometer gemessene Temperatur haben, die mindestens 3..4 Grad Celsius über der angezeigten Taupunkttemperatur liegt. Ein Online-Luftfeuchterechner ist z.B. [hier](http://www.thestorff.de/luftfeuchte-rechner.php) zu finden.
- Die angezeigte absolute Luftfeuchtigkeit kann dazu verwendet werden zu entscheiden, ob das Öffnen der Fenster eine Entfeuchtung eines Raums bewirkt: Wenn die absolute Luftfeuchte außen um mindestens 1 g/m³ kleiner ist als im Innenraum, dann wird beim Lüften der Raum entfeuchtet. 


## Kalibrierung von Temperatur, relativer Luftfeuchtigkeit   

- Um eine Offsetkalibrierung der Temperatur- und relativen Luftfeuchtemessung des BME280 Sensors durchzuführen, zeichne diese beiden Messgrößen z.B. im CCU Historian und vergleiche sie mit den entsprechenden Messwerten eines 'goldenen' Referenzsensors. Extrahiere die Differenzen zwischen den Referenzmesswerten des 'goldenen' Referenzsensors und den entsprechenden des BME280 Sensors, am Besten in einer Messreihe über Nacht mit geringen Messwertschwankungen und daher flachen Messverläufen. Gebe die Differenzen in das [WebUI Startseite > Einstellungen > Geräte > Geräte-/ Kanalparameter einstellen](Images/Setting_of_device_parameters_in_WebUI.png) ein.
- Bitte führe die Offsetkalibrierung in zwei getrennten Schritten durch:
	+ Kalibriere die BME280 Temperatur in einem ersten Schritt (erste Nacht)
	+ Kalibriere die BME280 relative Luftfeuchtigkeit in einem zweiten Schritt (zweite Nacht)
	+ Kalibriere die BME280 Temperatur und relative Luftfeuchtigkeit NICHT gemeinsam in einem Schritt, da diese beiden Größen physikalisch voneinander abhängen
	
## Schaltung

- basierend auf einer ATmega328P MCU (3,3V/8MHz) mit CC1101 Sendemodul, Eigenbau möglich
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
       + Reichelt [Bestelliste](https://www.reichelt.de/my/1882914
)
           + Arduino Pro Mini, CC1101 Tranceiver, Batteriehalter und eventuell BME280 müssen separat bestellt werden
           + Arduino Pro Mini un CC1101 Tranceiver sind gesockelt
	+ Aufbau entsprechend siehe [Technikkram](https://technikkram.net/blog/2018/05/30/homematic-diy-projekt-thermometer-und-hydrometer-fertige-platine-im-eigenbau/)
	+ 2x 10 kOhm I2C Abschlusswiderstände für SCL und SDA auf HB-UNI-SEN-BATT PCB einlöten
	+ Bitte darauf achten, dass der Arduino Pro Mini die 3,3V/8MHz Version ist!
	
## Frequenztest des CC1101 RF Moduls

- Bitte vor dem Aufspielen des eigentlichen Sketches HB-UNI-Sensor1-THPD-BME280 UNBEDINGT einen [Frequenztest ATMega328P](https://github.com/pa-pa/AskSinPP/blob/master/examples/FreqTest/FreqTest.ino) durchführen. Viele CC1101 Module lassen sich ohne diesen Frequenztest nicht anlernen!
- Eine Beschreibung des Frequenztests ist [hier](https://asksinpp.de/Grundlagen/FAQ/Fehlerhafte_CC1101.html#ermittlung-der-cc1101-frequenz) zu finden.
		
## Verringerung des Ruhestroms: [siehe auch Referenz von TomMajor](https://github.com/TomMajor/SmartHome/tree/master/Info/Ruhestrom)

- Auf dem Arduino Pro Mini sollte der LDO Spannungsregler (die Stelle ist im Bild mit 1 markiert) sowie die Power-LED (2) entfernt werden, um den Ruhestrom wesentlich zu verringern.  
- Weiterhin kann die zweite LED am Arduino Pin 13 (SCK) entfernt werden (3). Deren Einfluß auf den Ruhestrom ist kleiner als bei LDO und Power-LED, trotzdem kann es sich auf lange Sicht lohnen da diese bei jeder CC1101 Kommunikation für kurze Zeit aktiv ist, besonders im BurstDetector Mode.

![pic](Images/ProMini_LDO_LED.png)	

- Brown-Out_Detektor des ATmega328P  (BOD) ausschalten, siehe Einstellungen Arduino IDE unten unter Punkt **Benötigter Sketch**
- Bitte danach den [SleepTest](https://github.com/TomMajor/SmartHome/blob/master/Info/Ruhestrom/SleepTest/SleepTest.ino) durchführen. Der gemessene Ruhestrom im Sleep-Modus sollte < 10 uA betragen. Ist der gemessene Strom nach ein paar Sekunden höher, ist ein Bauteil defekt und muss ausgetauscht werden. Da das Auslöten immer schwierig ist, ist ein gesockelter Einbau zu empfehlen.
	       

## Das angemeldete Gerät im RaspberryMatic WebUI

![pic](Images/WebUI.png)


## Vor dem Aufspielen von Software

- Bitte macht Euch zuerst mit den Grundlagen von AsksinPP [hier](https://asksinpp.de/Grundlagen/) vertraut.

## Bitte immer die aktuellste Version von AsksinPP nutzen

-  [AsksinPP Master](https://github.com/pa-pa/AskSinPP/tree/master)

## Bitte genau diese Abfolge beim Einspielen von Software beachten:

- Details sind gegebenenfalls weiter unten zu finden

1. Bitte zuerst nochmals vergewissern, dass der verbaute Arduino Pro Mini ein 3,3V/8MHz Typ ist.
2. Das benötigte Addon [hb-ep-devices-addon](https://github.com/FUEL4EP/HomeAutomation/releases/latest) auf Eure CCU3/RaspberryMatic installieren.
3. Für das Programmierern und Setzen der Fuses des ATmega328P ist ein ISP Programmer empfohlen. Eine Anleitung ist [hier](https://asksinpp.de/Grundlagen/04-isp.html) zu finden. Dabei bitte unbedingt den ISP Programmer auf 3,3V einstellen!
4. Setzten der richtigen Fuses mit dem [avrdude script](avrdude/avrdude_328P.bsh) (LINUX version) oder mit [AVRDUDESS](https://blog.zakkemble.net/avrdudess-a-gui-for-avrdude/) (Windows Version): Fuse Setting: Low Byte:0xFF  High Byte:0xD2  Extended Byte:0xFF. Dafür bitte einen ISP Programmer verwenden, siehe [hier](https://asksinpp.de/Grundlagen/04-isp.html). Ich persönlich nutze diesen [ISP Programmer](https://www.amazon.de/Diamex-Programmer-XMEGA-ATMEGA-ATtiny/dp/B0064LLRB0).
5. Dann den [Frequenztest](https://asksinpp.de/Grundlagen/FAQ/Fehlerhafte_CC1101.html#ermittlung-der-cc1101-frequenz) durchführen. Dazu den [FreqTest.ino Sketch](https://github.com/pa-pa/AskSinPP/blob/master/examples/FreqTest/FreqTest.ino) ausführen und dabei auf ein erfolgreiches Beenden achten.
6. Den Config-Taster ganz lange drücken (ca. 6..8 Sekunden) bis die rote LED erlischt und im seriellen Monitor 'RESET' ausgegeben wird. Damit wird ein 'Werksreset' durchgeführt und das EEPROM gelöscht. Damit gehen auch alle im EEPROM gespeicherten Einstellungen verloren.  Die ermittelte Frequenzeinstellung des Frequenztests bleibt aber erhalten.
7. Gegebenenfalls die Parameter cDEVICE_und cDEVICE_in [Device_BME280.h](Cfg/Device_BME280.h) ändern. Jeder Sensor muss ein eineindeutiges cDEVICE_als auch ein eindeutiges cDEVICE haben!
8. Die aktuelle Version des [HB-UNI-Sensor1-THPD-BME280](https://github.com/FUEL4EP/HomeAutomation/tree/master/AsksinPP_developments/sketches/HB-UNI-Sensor1-THPD-BME280) Sketches herunterladen, siehe unten.
9. Die benötigten Libraries installieren, siehe unten unter 'Benötigte Libraries'.
10. Dann den Sketch [HB-UNI-Sensor1-THPD-BME280.ino](https://github.com/FUEL4EP/HomeAutomation/tree/master/AsksinPP_developments/sketches/HB-UNI-Sensor1-THPD-BME280/HB-UNI-Sensor1-THPD-BME280.ino) kompilieren und mit dem ISP Programmer auf den Arduino Pro Mini hochladen.
11. Den neuen Sensor in der CCU/RaspBerryMatic anlernen.
12. Den neuen Sensor in der CCU3/RaspberryMatic dem Gewerk 'Wetter' zuordnen.
 

## Benötiger Sketch

[HB-UNI-Sensor1-THPD-BME280](https://github.com/FUEL4EP/HomeAutomation/tree/master/AsksinPP_developments/sketches/HB-UNI-Sensor1-THPD-BME280)

- bitte alle Unterverzeichnisse mit kopieren:

### Zur lokalen Installation des Github Releases auf Deinem Computer

- gehe bitte in Dein Zielinstallationsverzeichnis, wo Du mit dem HB-UNI-Sensor1-THPD-BME280 Sensor arbeiten möchtest

  - gebe dort 'git clone https://github.com/FUEL4EP/HomeAutomation.git' ein
	  + damit lädst Du mein [Sammelrepository](https://github.com/FUEL4EP/HomeAutomation) für alle meine auf Github freigegebenen HomeBrew Sensoren herunter.
  - dann findest Du ein neues Verzeichnis 'HomeAutomation' auf Deinem Rechner, das alle meine auf Github freigebenen Sensoren enthält, siehe [README.md](https://github.com/FUEL4EP/HomeAutomation/blob/master/README.md)
  	+ mache bitte regelmäßig ein Update mit 'git pull'
 -	enthalten ist auch das notwendige Addon '[ep-hb-devices-addon](https://github.com/FUEL4EP/HomeAutomation/releases/latest)'
 -	den HB-UNI-Sensor1-THPD-BME280 Sensor findest Du unter './HomeAutomation/tree/master/AsksinPP_developments/sketches/HB-UNI-Sensor1-THPD-BME280'
 
- alternativ kann vom Github [Sammelrepository](https://github.com/FUEL4EP/HomeAutomation) die ZIP-Datei HomeAutomation-master.zip heruntergeladen und auf dem lokalen Rechner ausgepackt werden. Bitte dann regelmäßig auf Github nach Updates schauen.

- nach erfolgreicher Inbetriebnahme können die Debugausgaben im serial Monitor ausgeschaltet werden. Dazu bitte im Sketch HB-UNI-Sensor1-THPD-BME280.ino auskommentieren:

> // !! NDEBUG sollte aktiviert werden wenn die Sensorentwicklung und die Tests abgeschlossen sind und das Gerät in den 'Produktionsmodus' geht.<br/>
> // Insbesondere die RAM-Einsparungen sind wichtig für die Stabilität / dynamische Speicherzuweisungen etc.<br/>
> // Dies beseitigt dann auch die mögliche Arduino-Warnung 'Low memory available, stability problems may occur'.<br/>
> //<br/>
> //#define NDEBUG

- als Taktfrequenz des ATmega328P 8 MHz @ 3.3V externer Quarz einstellen
- der Sketch verwendet 29994 Bytes (97%) des Programmspeicherplatzes. Das Maximum sind 30720 Bytes. Globale Variablen verwenden 1163 Bytes (56%) des dynamischen Speichers, 885 Bytes für lokale Variablen verbleiben. Das Maximum sind 2048 Bytes.

- [Fuses Calculator](http://eleccelerator.com/fusecalc/fusecalc.php); select ATmega328P

- [avrdude script](avrdude/avrdude_328P.bsh) (LINUX version)
	- wichtig ist dass dieser Skript **VOR** dem Flashen des Programmcodes ausgeführt wird.  Das EESAVE Konfigurationsbit des Atmega328P muss gesetzt sein (Preserve EEPROM memory through the Chip Erase cycle; [EESAVE=1])

- die Programmierung erfolgt mit einem ISP Programmer, z.B. Diamex ISP USB Programmer. Dazu dienen die Signale VCC, GND, MOSI, SCK, MISO,
RSET an der Steckerleiste unten rechts in der Basisplatine. Dort eine Steckerleiste einlöten.
- ISP Programmer auf 3.3V einstellen!
- Einstellungen Arduino IDE: 8MHz, 3.3V
- Hochladen des kompilierten Sketchs im Arduino IDE mit: Sketch => Hochladen mit Programmer
- Debugging wird über den seriellen Monitor mit einem FTDI Adapter USB zu TTL Serial für
3,3V und 5V für Arduino gemacht. Als Baudrate des seriellen Monitors bitte **57600** Baud einstellen.
- **WICHTIG:** Wenn der Config-Taster des Sensors (unbeabsichtigt) länger als 6..8 Sekunden gedrückt wurde, muss der Sensor danach neu an der Zentrale angelernt werden. Beim Reset wird die Adresse der Zentrale (CCU3/RaspberryMatic) im EEPROM des 328P gelöscht. Damit funktioniert LazyConfig nicht mehr. Zum erneuten Anlernen, zuerst den Sensor in der CCU3/RaspberryMatic ablernen und dann normal wieder anlernen. Die Offseteinstellungen müssen dann im WebUI erneut vorgenommen werden!


## Benötigte Libraries

+ [AskSinPP Library](https://github.com/pa-pa/AskSinPP)</br>
+ [EnableInterrupt](https://github.com/GreyGnome/EnableInterrupt)</br>
+ [Low-Power](https://github.com/rocketscream/Low-Power)</br>
+ [finitespace/BME280](https://github.com/finitespace/BME280)

- **WICHTIG:** Es darf nur die [finitespace BME280](https://github.com/finitespace/BME280) Bibliothek im Suchpfad der Arduino IDE sein! Gegebenenfalls müssen andere BME280 Bibliotheken temporär anders wohin verschoben werden.




## Benötigtes Addon

[hb-ep-devices-addon](https://github.com/FUEL4EP/HomeAutomation/releases/latest)

- bitte dieses Addon 'hb-ep-devices-addon.tgz 'vor dem Anlernen des HB-UNI-Sensor1-THPD-BME280 Sensors auf der RaspberryMatic / CCU3 installieren (kein unzip vonnöten!)

## Verringerung der Tx Sendeleistung

- nur Experten wird empfohlen, die Tx Sendeleistung zu verringern. Die Beschreibung ist [hier](./Reduction_of_Tx_RF_power/README.md).

## Lizenz

**Creative Commons BY-NC-SA**<br>
Give Credit, NonCommercial, ShareAlike

<a rel="license" href="http://creativecommons.org/licenses/by-nc-sa/4.0/"><img alt="Creative Commons License" style="border-width:0" src="https://i.creativecommons.org/l/by-nc-sa/4.0/88x31.png" /></a><br />This work is licensed under a <a rel="license" href="http://creativecommons.org/licenses/by-nc-sa/4.0/">Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License</a>.
