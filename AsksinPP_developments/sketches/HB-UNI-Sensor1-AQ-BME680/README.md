
# Universeller Luftgütesensor auf der Basis von dem Bosch BME680 Sensor (HB-UNI-Sensor1-AQ-BME680)

- abgeleitet von Jérômes ([jp112sdl](https://github.com/jp112sdl)) [HB-UNI-Sen-IAQ](https://github.com/jp112sdl/HB-UNI-Sen-IAQ)
- ein herzliches Dankeschön für die Basisarbeit geht an Jérôme (jp112sdl)
- ein herzliches Dankeschön an alle, die im Homematic Forum geholfen haben, meine Probleme zu lösen
- der Homematic Forum [Diskussionsstrang](https://homematic-forum.de/forum/viewtopic.php?t=49422)

## Neue Eigenschaften im Vergleich zum HB-UNI-Sen-IAQ Sensor

- die relative Luftfeuchtigkeit wird mit 0,1 % rLF Genauigkeit ausgegeben
- die Batteriespannung wird mit 10 mV Genauigkeit und jeden Zyklus ausgegeben 
- alle wichtigen Sensorparameter können interaktiv ohne Neuprogrammierung im WebUI der [RaspberryMatic](https://github.com/jens-maus/RaspberryMatic) / [CCU3](https://de.elv.com/smart-home-zentrale-ccu3-inklusive-aio-creator-neo-lizenz-ccu-plugin-151965?fs=2591490946) eingegeben werden:
	+ [Startseite > Einstellungen > Geräte > Geräte-/ Kanalparameter einstellen](Images/Setting_of_device_parameters_in_WebUI.png)
	
- Ausgabe des Luftgütegrads AQ_LEVEL: normierter BME680 Gaswiderstand 0..100% (ohne Kompensation der Einflüsse von Temperatur und absoluter Luftfeuchte auf die gemessene Luftgüte)
- Ausgabe der logarithmischen Luftqualität AQ_LOG10: logarithmierte normierte und kompensierte Luftqualität 0.0..4.0 (mit Kompensation der Einflüsse von Temperatur und absoluter Luftfeuchte auf die gemessene Luftqualität)
	+ die logarithmischen Luftqualität AQ_LOG10 korreliert in der Regel gut mit dem CO2 Gehalt der Luft
- Autokalibrierung für den Luftgütegrad und die logarithmische Luftqualität: Zur Kalibrierung muss der Sensor nur:  

	+ ca. 15 min schlechter Luft, z.B. einem mit Schnaps getränkten Papiertaschentuch ausgesetzt werden
	+ ca. 15 .. 20 min guter Außenluft, am besten bei Wind, ausgesetzt werden
	+ diese Kalibrierung sollte ca. alle 1..2 Monate wiederholt werden
	
- Kompensation der Einflüsse von Temperatur und absoluter Luftfeuchte auf die gemessene Luftgüte LOG10 durch [multiple lineare Regression](https://de.wikipedia.org/wiki/Multiple_lineare_Regression)
	+ die absolute Luftfeuchte wird im Sensor auf der ATmega1284P MCU aus Temperatur und relativer Luftfeuchte [berechnet](https://www.kompf.de/weather/vent.html) 
	+ Historiendaten von Gaswiderstand, Temperatur und relativer Luftfeuchtigkeit werden mit dem [CCU-Historian](https://github.com/mdzio/ccu-historian) aufgezeichnet und als CSV Datei gespeichert
	+ die Historiendaten soltten ausreichend viele Wechsel von guter und schlechter Luft beinhalten, z.B. Aufzeichnung über 2 Wochen oder länger
	+ externe Berechnung der multiplen Regressionsparameter mittels eines interaktiven Python Notebooks in [JupyterLab](http://jupyterlab.io/)
	+ Eingabe der berechneten Regressionparameter in das [WebUI](Images/Setting_of_device_parameters_in_WebUI.png) (siehe oben)
	+ Wiederholung der Parameterberechnung aus Historiendaten ca. alle 6 Monate
	 


## Schaltung

- basierend auf einer ATmega1284P MCU mit CC1101 Sendemodul, Eigenbau möglich
- eine mögliche und von mir empfohlene Hardwarebasis sieht so aus:
	+ Universalplatine für DIY-Bausätze von Alexander Reinert
		* Github [HB-UNI-SEN-BATT](https://github.com/alexreinert/PCB#hb-uni-sen-batt)
		* Bezugsquelle: [Smartkram WebShop](https://smartkram.de/produkt/universalplatine-fuer-diy-bausatze-von-alex-reinert/)
    + Sensorgehäuse
	    * 3D-Druck [HB-UNI-SEN-BATT snap Gehaeuse und Deckel](https://www.thingiverse.com/thing:3512767)	auf Thingiverse
	    * oder [fertiges Gehäuse](https://smartkram.de/produkt/sensorgehaeuse-passend-fuer-platine-von-alexander-reinert/) von Smartkram Webshop
	+ Bosch BME 680 Breakout Board (bitte keinen China Billigclone)
		* Bezugsquelle: Pimoroni [BME680 Breakout - Air Quality, Temperature, Pressure, Humidity Sensor](https://shop.pimoroni.com/products/bme680-breakout)
	+ Tindie [Pro Mini XL - v2 - ATmega 1284p](https://www.tindie.com/products/prominimicros/pro-mini-xl-v2-atmega-1284p/)
	+ Bauteile [HB-UNI-SEN-BATT](https://smartkram.de/produkt/bauteile-fuer-homematic-diy-projekt-thermometer-hydrometer/) von Smartkram Webshop
		* Der gelieferte Arduino Pro Mini wird durch Tindie Pro Mini XL - v2 - ATmega 1284p ersetzt
		* Das gelieferte BME280 Sensor Breakout wird durch das BME680 Breakout ersetzt
	+ Aufbau entsprechend siehe [Technikkram](https://technikkram.net/blog/2018/05/30/homematic-diy-projekt-thermometer-und-hydrometer-fertige-platine-im-eigenbau/), bitte geeignet abändern
	+ die I2C-Verbindungen zwischen HB-UNI-SEN-BATT PCB und BME680 Breakout mit flexiblem Flachbandhabel
	+ 2x 10 kOhm I2C Abschlusswiderstände auf HB-UNI-SEN-BATT PCB einlöten
	+ im Unterverzeichnis 3D_Druck ist eine 3D-Druck [STL Datei](3D_Druck/BME680_protection.stl) für eine BME680 Halterung zu finden
		* wird auf das HB-UNI-SEN-BATT PCB mit 2-Komponentenkleber aufgeklebt
		* eine Nase muss mit einer Flachfeile entfernt werden
	
	       

## Das angemeldete Gerät im RaspberryMatic WebUI

![pic](Images/WebUI.png)


## Benötiger Sketch

[HB-UNI-Sensor1-AQ-BME680](https://github.com/FUEL4EP/HomeAutomation/tree/master/AsksinPP_developments/sketches/HB-UNI-Sensor1-AQ-BME680)

- bitte alle Unterverzeichnisse mit kopieren
- nach erfolgreicher Inbetriebnahme können die Debugausgaben im serial Monitor ausgeschaltet werden. Dazu bitte im Sketch HB-UNI-Sensor1-AQ-BME680.ino auskommentieren:

> 	//#define NDEBUG   // disable all serial debug messages

- als Taktfrequenz des ATmega1284P 20 MHz externer Quartz einstellen

- [Fuses Calculator](http://eleccelerator.com/fusecalc/fusecalc.php?chip=atmega1284phttp://eleccelerator.com/fusecalc/fusecalc.php?chip=atmega1284p); select ATmega1284P
- [avrdude script](avrdude/avrdude_m1284p_ext_20MHz.bsh) zum Setzen der Fuses für 20 MHz externer Quartz (Linux version)

- Die Programmierung erfolgt mit einem ISP Programmer, z.B. Diamex ISP USB Programmer. Dazu dienen die Signale VCC, GND, MOSI, SCK, MISO,
RSET an der Steckerleiste unten rechts in der Basisplatine. Dort eine Steckerleiste einlöten. Einstellungen Arduino IDE [Werkzeuge](Images/Arduino_IDE_Tools.png)
- Als zusätzliche Boardverwalter-URLs unter Arduino IDEs Voreinstellungen einstellen (falls noch nicht getan):

> https://mcudude.github.io/MightyCore/package_MCUdude_MightyCore_index.json

- Hochladen des kompilierten Sketchs im Arduino IDE mit: Sketch => Hochladen mit Programmer
- Debugging wird über den seriellen Monitor mit einem FTDI Adapter USB zu TTL Serial für
3,3V und 5V für Arduino gemacht. 



## Benötige Libraries

[AskSinPP Library](https://github.com/pa-pa/AskSinPP)</br>
[EnableInterrupt](https://github.com/GreyGnome/EnableInterrupt)</br>
[Low-Power](https://github.com/rocketscream/Low-Power)</br>
[ClosedCube_BME680_Arduino](https://github.com/FUEL4EP/ClosedCube_BME680_Arduino/tree/implement_Bosch_datasheet_integer_formulas)

## Benötiges Addon

[hb-ep-devices-addon](https://github.com/FUEL4EP/HomeAutomation/blob/master/AsksinPP_developments/addons/hb-ep-devices-addon/CCU_RM/hb-ep-devices-addon.tgz)

- bitte dieses Addon vor dem Anlernen des HB-UNI-Sensor1-AQ-BME680 Sensors auf der RaspberryMatic / CCU3 installieren (kein unzip vonnöten!)

## Autokalibrierung

- Eine detaillierte Beschreibung der Vorgehensweise ist im Unterverzeichnis Multiple_Linear_Regression im [README.md](Multiple_Linear_Regression/README.md) zu finden
- Vor der Autokalibrierung bitte die Offsets genau in dieser Reihenfolge

	+ der Temperatur
	+ der relativen Luftfeuchte
- bestimmen und im WebUI setzten. Dabei zuerst das Einschwingen der korrigierten Temperatur abwarten, bevor die Luftfeuchte korrigiert wird. Zum Bestimmen der Offsets eine 24h Aufzeichnung mit dem CCU-Historian mit einer genauen Referenztemperatur / -luftfeuchte machen ('golden' TH-Sensor daneben stellen)

## Alterung des BME680 Sensors

-	der MOX Sensor Bosch BME680 altert über die Lebensdauer, am Anfang der Betriebszeit am stärksten
-	die ersten Wochen nach Inbetriebnahme sind die Messwerte unzuverlässig(er)
-	der Gaswiderstandswert nimmt im Laufe der Zeit zu
-	die Alterung sollte im CCU-Historian verfolgt werden
-	solange der mittlere Gaswiderstand einen sichtbaren Gradienten zu höheren Werten aufweist, sollte die Kalibrierung öfters wiederholt werden. Dazu den Gaswiderstand über einen Zeitraum von 1..2 Monaten als Trend-Diagram im CCU-Historian darstellen.
-	der Gaswiderstand des BME680 Sensors variiert stark: Werte von 10 kOhm bis 1 M Ohm sind OK
-	zur Zeit ist der maximale Gaswiderstand im Sketch auf 65365*20 Ohm = 1.3 MOhm beschränkt.
-	sollte Dein Sensor höhere Widerstandswerte aufweisen, melde Dich bitte bei mir. Dann muss ein Softwareparameter geändert werden.


## Lizenz

**Creative Commons BY-NC-SA**<br>
Give Credit, NonCommercial, ShareAlike

<a rel="license" href="http://creativecommons.org/licenses/by-nc-sa/4.0/"><img alt="Creative Commons License" style="border-width:0" src="https://i.creativecommons.org/l/by-nc-sa/4.0/88x31.png" /></a><br />This work is licensed under a <a rel="license" href="http://creativecommons.org/licenses/by-nc-sa/4.0/">Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License</a>.
