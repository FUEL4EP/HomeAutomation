# HB-UNI-Sensor1-AQ-BME680_KF_DEBUG [![License: CC BY-NC-SA 4.0](https://img.shields.io/badge/License-CC%20BY--NC--SA%204.0-lightgrey.svg)](https://creativecommons.org/licenses/by-nc-sa/4.0/) [![Hits](https://hits.seeyoufarm.com/api/count/incr/badge.svg?url=https%3A%2F%2Fgithub.com%2FFUEL4EP%2FHomeAutomation%2FAsksinPP_developments%2Fsketches%2FHB-UNI-Sensor1-AQ-BME680_KF_DEBUG&count_bg=%2379C83D&title_bg=%23555555&icon=&icon_color=%23E7E7E7&title=hits&edge_flat=false)](https://hits.seeyoufarm.com) <a href='https://ko-fi.com/FUEL4EP' target='_blank'><img height='20' style='border:0px;height:20px;' src='https://cdn.ko-fi.com/cdn/kofi1.png?v=2' border='0' alt='Buy Me a Coffee at ko-fi.com' /></a>
<br/>

# Dieser Sensor HB-UNI-Sensor1-AQ-BME680_KF_DEBUG ist eine **DEBUG** Version und NICHT für den normalen Einsatz!
- dieser Sensor ist eine reine DEBUG Version von [HB-UNI-Sensor1-AQ-BME680_KF](https://github.com/FUEL4EP/HomeAutomation/tree/master/AsksinPP_developments/sketches/HB-UNI-Sensor1-AQ-BME680_KF)
- mit dieser DEBUG Version kann  das Einschwingen des benutzten Kalman Filters beobachtet werden.
- beobachtbar sind folgende Datenpunkte, z.B. im CCU Historian:
	+ TEMPERATURE
	+ AQ_LEVEL (linearer Luftgütegrad)
	+ AQ_LOG10 (logarithmischen Luftqualität, unkompensiert)
	+ AQ_COMP_GAS_RES_RAW  (kompensierter Gaswiderstands)
	+ AQ_COMP_GAS_RES_MIN  (untere Referenz für die Normalisierung des kompensierten Gaswiderstands)
	+ AQ_COMP_GAS_RES_MAX (obere Referenz für die Normalisierung des kompensierten Gaswiderstands)
	+ AQ_ALPHA (Regressionskoeffizient für die Temperatur)
	+ AQ_BETA (Regressionskoeffizient für die absolute Luftfeuchte)
- für AQ_COMP_GAS_RES_RAW, AQ_COMP_GAS_RES_MIN, AQ_COMP_GAS_RES_MAX, AQ_ALPHA, AQ_BETA können bei der Ausgabe der Datenpunkte Sättigungseffekte auftreten, wenn der unterstützte Wertebreich über- bzw. unterschritten wird. Dadurch werden Überlaufeffekte vermieden.
- alle anderen Datenpunkte sind nicht beobachtbar, da die maximale Payload einer Nachricht 17 Bytes beträgt.
- zur Verifizierung und besserem Verständnis des Kalman Filters wird ein Jupyter Notebook [Prove_of_Kalman_filter_with_synthesized_data.ipynb](./Kalman_Filter/Prove_of_Kalman_filter_with_synthesized_data.ipynb) zur Verfügung gestellt. Auf Github kann das Notebook direkt angesehen werden.
-	Die Debug-Version [HB-UNI-Sensor1-AQ-BME680_KF_DEBUG](https://github.com/FUEL4EP/HomeAutomation/tree/master/AsksinPP_developments/sketches/HB-UNI-Sensor1-AQ-BME680_KF_DEBUG) und die Normalversion [HB-UNI-Sensor1-AQ-BME680_KF](https://github.com/FUEL4EP/HomeAutomation/tree/master/AsksinPP_developments/sketches/HB-UNI-Sensor1-AQ-BME680_KF) nutzen diesselbe Struktur für die Abspeicherung und Restaurierung von EEPROM-Daten. Daher kann der Sensor zwischen diesen beiden Versionen umprogrammiert werden. Dabei bitte die Hinweise unter 'Neuprogrammierung (Flashen)' unten beachten.
-	hier Histogramme über 4 Monate von 3 direkt nebeneinander platzierten baugleichen Sensoren
    +	Histogramme der mit dem Kalman Filter berechneten Online Regressionskoeffizienten alpha (Temperatur) und beta (absolute Luftfeuchte)
     	![pic](Autocalibration/convergence_of_Kalman_absolute_humidity_regression_coefficient_alpha_over_4_months.png)
    + Histogramme der kompensierten Gaswiderstände (grün) und des oberen (rot) bzw. unteren (blau) Referenzwerts für saubere bzw. schlechte Luft 
        +  Sensor 1:
           ![pic](Autocalibration/4_months_histogram_of_compensated_gas_resistance_with_upper_and_lower_bounds_for_clean_and_poor_airquality___sensor_1.png)
        + Sensor 2:
          ![pic](Autocalibration/4_months_histogram_of_compensated_gas_resistance_with_upper_and_lower_bounds_for_clean_and_poor_airquality___sensor_2.png)
        + Sensor 3:
          ![pic](Autocalibration/4_months_histogram_of_compensated_gas_resistance_with_upper_and_lower_bounds_for_clean_and_poor_airquality___sensor_3.png)
- gut erkennbar ist, wie stark sich die Parameter auch nach Monaten noch ändern 


##  Aufbau und Programmierung, Hinweise

- siehe [HB-UNI-Sensor1-AQ-BME680_KF](https://github.com/FUEL4EP/HomeAutomation/tree/master/AsksinPP_developments/sketches/HB-UNI-Sensor1-AQ-BME680_KF)   

## Das angemeldete Gerät im RaspberryMatic WebUI

![pic](Images/WebUI.png)

## Das angemeldete Gerät in der CCU Historian Datenpunktliste


![pic](Images/CCU_Historian_DataPoints.png)

## AQ_LEVEL normierter Gaswiderstandsverlauf (Beispiel)


![pic](Images/AQ_LEVEL.png)

## AQ_LOG10 logarithmierte normierte Luftqualität

![pic](Images/AQ_LOG10_versus_NDIR_SCD30_CO2.png)

- zusätzlich zum AQ_LOG10 Luftqualitätsverlauf wird der CO2 Gehalt der Luft gemessen mit einem Sensirion SCD30 NDIR CO2 Sensor 


## Benötiger Sketch

[HB-UNI-Sensor1-AQ-BME680_KF_DEBUG](https://github.com/FUEL4EP/HomeAutomation/tree/master/AsksinPP_developments/sketches/HB-UNI-Sensor1-AQ-BME680_KF_DEBUG)

- restliche Vorgehensweise siehe [HB-UNI-Sensor1-AQ-BME680_KF](https://github.com/FUEL4EP/HomeAutomation/tree/master/AsksinPP_developments/sketches/HB-UNI-Sensor1-AQ-BME680_KF), dort sinngemäß 'HB-UNI-Sensor1-AQ-BME680_KF' durch 'HB-UNI-Sensor1-AQ-BME680_KF_DEBUG' ersetzen


## Lizenz

**Creative Commons BY-NC-SA**<br>
Give Credit, NonCommercial, ShareAlike

<a rel="license" href="http://creativecommons.org/licenses/by-nc-sa/4.0/"><img alt="Creative Commons License" style="border-width:0" src="https://i.creativecommons.org/l/by-nc-sa/4.0/88x31.png" /></a><br />This work is licensed under a <a rel="license" href="http://creativecommons.org/licenses/by-nc-sa/4.0/">Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License</a>.
