# HB-UNI-Sensor1-RAD-AL53 [![License: CC BY-NC-SA 4.0](https://img.shields.io/badge/License-CC%20BY--NC--SA%204.0-lightgrey.svg)](https://creativecommons.org/licenses/by-nc-sa/4.0/) [![Hits](https://hits.seeyoufarm.com/api/count/incr/badge.svg?url=https%3A%2F%2Fgithub.com%2FFUEL4EP%2FHomeAutomation%2FAsksinPP_developments%2Fsketches%2FHB-UNI-Sensor1-RAD-AL53&count_bg=%2379C83D&title_bg=%23555555&icon=&icon_color=%23E7E7E7&title=hits&edge_flat=false)](https://hits.seeyoufarm.com)

# Under construction

## Coming soon, first prototype is now running well
- here first snapshot of software status
- documentation is missing, will be provided later on

## AsksinPP DIY Homematic sensor for alpha, beta, and gamma radiation
- Picture of current prototype ![pic](Images/HB-UNI-Sensor1-RAD-AL53.png)
- Based on
    - universal AsksinPP PCB for DIY-kits from Alexander Reinert [HB-UNI-SEN-BATT](https://github.com/alexreinert/PCB#hb-uni-sen-batt)
	- [Teviso AL53](https://www.teviso.com/file/pdf/al53-data-specification.pdf) radiation sensor
    	+ IMPORTANT: amplitude of output pulse is Vsupply, a voltage divider is needed to interface to the counter ABLIC S-35770
	- I2C counter [ABLIC S-35770](https://www.ablic.com/en/doc/datasheet/counter_ic/S35770_I_E.pdf)
	    + an Arduino library is [here](https://github.com/FUEL4EP/HomeAutomation/tree/master/AsksinPP_developments/libraries/ABLIC_S35770)
	- [Tindie Pro Mini XL - v2](https://www.tindie.com/products/prominimicros/pro-mini-xl-v2-atmega-1284p/)
	    + an ATMega328P Pro Mini will NOT work
	- ADS1115 ADC, e.g. from Amazon
	- supply by 2x NiMH accumulator batteries 2700 mAH (currently use of external charger)
	- MT3608 boost converter, e.g. from Amazon, for creating supply voltage of AL53 sensor, max 15V!!
	- [TPS61221 boost converter](https://www.tindie.com/products/closedcube/tps61221-low-input-from-07v-boost-33v-breakout/) for 3.3V supply of ATMega1284P (Tindie Pro Mini XL - v2)
	- Ultra low power
	- [AsksinPP](https://github.com/pa-pa/AskSinPP)
	- Addon [hb-ep-devices-addon](https://github.com/FUEL4EP/HomeAutomation/tree/master/AsksinPP_developments/addons/hb-ep-devices-addon) > version 1.8
	- e-paper display will be added later on