# HB-UNI-SenAct-4-4-SC_DS_FUEL4EP [![License: CC BY-NC-SA 4.0](https://img.shields.io/badge/License-CC%20BY--NC--SA%204.0-lightgrey.svg)][![Hits](https://hits.seeyoufarm.com/api/count/incr/badge.svg?url=https%3A%2F%2Fgithub.com%2FFUEL4EP%2FHomeAutomation%2Ftree%2Fmaster%2FAsksinPP_developments%2FPCBs%2FHB-UNI-SenAct-4-4-SC_FUEL4EP&count_bg=%2379C83D&title_bg=%23555555&icon=&icon_color=%23E7E7E7&title=hits&edge_flat=false)](https://hits.seeyoufarm.com)

**Platine für DIY AsksinPP für**
- 3x Schaltaktor für Betrieb an einem Netzfreischalter
- Akkubetrieb im Standby, automatische Ladeschaltung
- bei Aktivierung eines Aktors wird die Netzspannung zugeschaltet und am Ende wieder abgeschaltet
- (fast) kein Standby-Verbrauch
- ein handelsübliches [4 Kanal Relais Modul](https://www.amazon.de/dp/B078Q8S9S9/) wird mit Distanzhaltern "huckepack" auf die Basisplatine montiert
- die angestrebte Gehäusegröße beträgt 15 cm (Breite) x 11 cm (Tiefe) x 5,5 cm (Höhe), ein passendes 3D-Druckgehäuse ist noch zu entwerfen
- 3x Einbausteckdosen 45 mm x 45 mm, z.B. DELOCK 81321 Easy 45 Schutzkontakt-Steckdose, werden an der Rückseite des Gehäuses angebracht
- **diese Schaltung und Platine darf ausschließlich von ausgebildeten elektrischen Fachkräften verwendet werden, siehe Disclaimer unten!**
- diese Schaltung und Platine ist noch nicht validiert!
- ein geeigneter Sketch kann auf Nachfrage zur Verfügung gestellt werden:
    - per PN bei [FUEL4EP](https://homematic-forum.de/forum/ucp.php?i=pm&mode=compose&u=20685) im Homematic Forum nachfragen.
- die Platine V1.2 behebt Fehler, die bei der Validierung eines Prototypen festgestellt und korrigiert wurden. Bitte keine Platinen der Versionen V1.0 und V1.1 verwenden.
- **wichtige Aufbauhinweise:**
    + zuerst die Niederspannungsplatine HB-UNI-SenAct-4-4-SC_DS_FUEL4EP_PCB Schritt für Schritt aufbauen, in Betrieb nehmen und testen:
        + für den Arduino Pro Mini und den CC1101 Tranceiver **vor Einbau** in einem externen Testbed den Ruhestrom prüfen (<10uA) und einen Frequenztest durchführen!
        + das 'Huckepack'-4-Kanal-Relaismodul noch nicht anbringen
    + die 230V Schaltungsteile erst dann anschließen, wenn der Rest vollständig geprüft wurde und die Software alle notwendigen Funktionen zeigt.
        * als Erstes dann das 'Huckepack'-4-Kanal-Relaismodul anbringen und nur die Steuersignale verbinden, noch keine 230V Spannungen an den Relaisausgängen anschließen
        * dann die Funktion testen
        * erst als Letztes die 230V Verdrahtung mit der gebotenen Vorsicht anbringen
        * die Sicherungen nicht vergessen
        * Zugentlastungen für das Netzkabel anbringen!


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
	

