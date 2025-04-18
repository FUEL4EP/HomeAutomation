# HB_Stamp_IO_EXT_debug_adapter_FUEL4EP [![License: CC BY-NC-SA 4.0](https://img.shields.io/badge/License-CC%20BY--NC--SA%204.0-lightgrey.svg)](https://creativecommons.org/licenses/by-nc-sa/4.0/) <a href='https://ko-fi.com/FUEL4EP' target='_blank'><img height='20' style='border:0px;height:20px;' src='https://cdn.ko-fi.com/cdn/kofi1.png?v=2' border='0' alt='Buy Me a Coffee at ko-fi.com' /></a>


**Adapterplatine für das FTDI-Debuggen**
- passende Adapterplatine für das FTDI-Debuggen mittels eines [Pogo-Pins Testadapters](./PNGs/Pogo_pin_adapter_with_reduced_bottom_plate.png):
	+	[HB_Stamp_IO_ATMega1284P_FUEL4EP](https://github.com/FUEL4EP/HomeAutomation/tree/master/AsksinPP_developments/PCBs/HB_Stamp_IO_ATMega1284P_FUEL4EP)
	+   [HB_Stamp_IO_EXT_LEDs_Buttons_FUEL4EP](https://github.com/FUEL4EP/HomeAutomation/tree/master/AsksinPP_developments/PCBs/HB_Stamp_IO_EXT_LEDs_Buttons_FUEL4EP)
	+	für das FTDI-Debuggen wird diese Adapterplatine zwischen diese beiden Platinen gesteckt, so dass der Pogo-Pins Testadapter auf die Platine HB_Stamp_IO_ATMega1284P_FUEL4EP geklemmt werden kann.
-
	![pic](PNGs/HB_Stamp_IO_EXT_debug_adapter_FUEL4EP_PCB_KiCAD.png)

	![pic](PNGs/HB_Stamp_IO_EXT_debug_adapter_FUEL4EP_PCB_top_3D.png)
	![pic](PNGs/HB_Stamp_IO_EXT_debug_adapter_FUEL4EP_PCB_back_3D.png)
	![pic](PNGs/FTDI_debugger_with_pogo_pin_adapter.png)


## Status

- die Platine V1.0 wurde noch nicht bei JLCPCB gefertigt und ist daher noch nicht validiert


## Eigenschaften

- kompletter Datensatz für KiCAD 8 auf Github verfügbar:
    + 'git clone https://github.com/FUEL4EP/HomeAutomation.git' oder als [ZIP-Datei](https://github.com/FUEL4EP/HomeAutomation/archive/refs/heads/master.zip)
    + die Platinendaten sind dann unter 'HomeAutomation/AsksinPP_developments/PCBs/HB_Stamp_IO_EXT_debug_adapter_FUEL4EP' direkt mit KiCAD aufrufbar


### Passende AsksinPP Platinen von FUEL4EP
- [HB_Stamp_IO_ATMega1284P_FUEL4EP](https://github.com/FUEL4EP/HomeAutomation/tree/master/AsksinPP_developments/PCBs/HB_Stamp_IO_ATMega1284P_FUEL4EP)
- [HB_Stamp_IO_EXT_LEDs_Buttons_FUEL4EP](https://github.com/FUEL4EP/HomeAutomation/tree/master/AsksinPP_developments/PCBs/HB_Stamp_IO_EXT_LEDs_Buttons_FUEL4EP)
   

### KiCAD Plugin
- für die Erzeugung der JLCPCB Produktionsdaten wurde das Plugin [KiCAD JLCPCB tools](https://github.com/bouni/kicad-jlcpcb-tools) verwendet.


## Bestellen von Platinen

- die Platine kann direkt bei JLCPCB bestellt werden.
- die notwendigen Produktionsdaten stehen [hier](./jlcpcb/production_files/)

## Aufbau
- J1 und J4 sind mit Stiftleiten RM 1,27 mm auf der Platinenunterseite zu bestücken
- J2 und J3 sind mit Buchsenleisten RM 1,27 mm auf der Platinenoberseite zu bestücken


## Disclaimer

-   die Nutzung der hier veröffentlichten Inhalte erfolgt vollständig auf eigenes Risiko und ohne jede Gewähr.



## Versionsverlauf

-   V1.0   17. Apr 2025: Initiale Vorabfreigabe
- 	KiCad Schaltplan-Editor   8.0.8-8.0.8-0~ubuntu22.04.1, release build unter Kubuntu22.04.1
- 	KiCad Leiterplatteneditor Version: 8.0.8-8.0.8-0~ubuntu22.04.1, release build unter Kubuntu22.04.1
- 	PCB Version 1.0
- 	Schematics  1.0


## Lizenz 

**Creative Commons BY-NC-SA**<br>
Give Credit, NonCommercial, ShareAlike

<a rel="license" href="http://creativecommons.org/licenses/by-nc-sa/4.0/"><img alt="Creative Commons License" style="border-width:0" src="https://i.creativecommons.org/l/by-nc-sa/4.0/88x31.png" /></a><br />This work is licensed under a <a rel="license" href="http://creativecommons.org/licenses/by-nc-sa/4.0/">Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License</a>.


-EOF
	

