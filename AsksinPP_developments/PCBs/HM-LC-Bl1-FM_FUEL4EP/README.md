# HM-LC-Bl1-FM_FUEL4EP [![License: CC BY-NC-SA 4.0](https://img.shields.io/badge/License-CC%20BY--NC--SA%204.0-lightgrey.svg)](https://creativecommons.org/licenses/by-nc-sa/4.0/)[![Hits](https://hits.seeyoufarm.com/api/count/incr/badge.svg?url=https%3A%2F%2Fgithub.com%2FFUEL4EP%2FHomeAutomation%2Ftree%2Fmaster%2FAsksinPP_developments%2FPCBs%2FHM-LC-Bl1-FM_FUEL4EP&count_bg=%2379C83D&title_bg=%23555555&icon=&icon_color=%23E7E7E7&title=hits&edge_flat=false)](https://hits.seeyoufarm.com)  <a href='https://ko-fi.com/FUEL4EP' target='_blank'><img height='20' style='border:0px;height:20px;' src='https://cdn.ko-fi.com/cdn/kofi1.png?v=2' border='0' alt='Buy Me a Coffee at ko-fi.com' /></a>

**Platine für DIY AsksinPP für**
- 24V Polwendesteuerung für Velux Rolladen und Betrieb an einem Netzfreischalter
- Akkubetrieb im Standby, automatische Ladeschaltung
- am Beginn einer Rolladenfahrt wird die Netzspannung zugeschaltet und am Ende wieder abgeschaltet
- die Relay-Module werden mit 0,5mm² Adernendhülsen 'huckepack' auf die Platine gelötet, die Adernendhülsen zuerst auf die Relay-Module auflöten, dann in die Platine einstecken und in der Platine festlöten
- (fast) kein Standby-Verbrauch
- diese Schaltung und Platine darf ausschließlich von ausgebildeten elektrischen Fachkräften verwendet werden, siehe Disclaimer unten!
- diese Schaltung und Platine ist in der Version 1.0 validiert. Es sind aufgrund von Fehlern kleine Korrekturen vonnöten gewesen, die in der Version 1.3 korrigiert sind.
- ein geeigneter Sketch kann auf Nachfrage zur Verfügung gestellt werden.
    + per PN bei [FUEL4EP](https://homematic-forum.de/forum/ucp.php?i=pm&mode=compose&u=20685) im Homematic Forum nachfragen.
    + zur Zeit existiert ein funktionsfähiger Sketch, der noch um die Akkuladesoftware erweitert werden muss. Sobald die Entwicklung abgeschlossen sein wird, wird ein Release einschließlich Sketch auf Github gemacht werden.
- V1.3 der Platine behebt Fehler, die bei der Bestückung eines Prototypen erkannt wurden, bitte keine V1.0, V1.1 und V1.2 Platinen verwenden!
- **wichtige Aufbauhinweise:**
    + zuerst die Niederspannungsplatine HM-LC-Bl1-FM_FUEL4EP_PCB Schritt für Schritt aufbauen, in Betrieb nehmen und testen:
        + für den Arduino Pro Mini und den CC1101 Tranceiver **vor Einbau** in einem externen Testbed den Ruhestrom prüfen (<10uA) und einen Frequenztest durchführen!
        + das Modul 4 'LM2596 buck 24V->6.3V' zuerst außerhalb auf eine Ausgangsspannung von 6,3 Volt einstellen und dann erst einbauen!
    + die 230V Schaltungsteile erst dann anschließen, wenn der Rest vollständig geprüft wurde und die Software alle notwendigen Funktionen zeigt.
- Ein Diskussionsfaden ist im Homematic Forum unter [Nachbau HM-LC-Bl1-FM für Velux Rolladen und Netzfreischalter](https://homematic-forum.de/forum/viewtopic.php?f=76&t=71683&sid=71b7e84664ec8bed7b9e50bd442664ad) zu finden

## Versionen
- KiCad Schaltplan-Editor Version: 6.0.11-2627ca5db0 unter Kubuntu22.04.1
- KiCad Leiterplatteneditor Version: 6.0.11-2627ca5db0 unter Kubuntu22.04.1
- PCB Version 1.4 
- Schematics  1.2

## Disclaimer

- aufgrund der im Gerät frei geführten Netzspannung und des Netzspannungsanschlusses darf die Installation nur von Fachkräften durchgeführt werden, die aufgrund ihrer Ausbildung dazu befugt sind.
- die einschlägigen Sicherheits- und VDE-Bestimmungen sind unbedingt zu beachten. Durch eine unsachgemäße Installation können Sach- und Personenschäden verursacht werden, für die der Errichter haftet.
- Installationsarbeiten sowohl am primärseitigen Netzanschluss als auch am Sekundäranschluss dürfen nur **im stromlosen Zustand erfolgen. Dabei sind alle einschlägigen Vorschriften des Installationshandwerks zu beachten!** 
- die Nutzung der hier veröffentlichten Inhalte erfolgt vollständig auf eigenes Risiko und ohne jede Gewähr

## Lizenz 

**Creative Commons BY-NC-SA**<br>
Give Credit, NonCommercial, ShareAlike

<a rel="license" href="http://creativecommons.org/licenses/by-nc-sa/4.0/"><img alt="Creative Commons License" style="border-width:0" src="https://i.creativecommons.org/l/by-nc-sa/4.0/88x31.png" /></a><br />This work is licensed under a <a rel="license" href="http://creativecommons.org/licenses/by-nc-sa/4.0/">Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License</a>.


-EOF
	

