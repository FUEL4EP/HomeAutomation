
# ABLIC_S35770 [![License: CC BY-NC-SA 4.0](https://img.shields.io/badge/License-CC%20BY--NC--SA%204.0-lightgrey.svg)](https://creativecommons.org/licenses/by-nc-sa/4.0/)[![Hits](https://hits.seeyoufarm.com/api/count/incr/badge.svg?url=https%3A%2F%2Fgithub.com%2FFUEL4EP%2FHomeAutomation%2Ftree%2Fmaster%2FAsksinPP_developments%2Flibraries%2FABLIC_S35770&count_bg=%2379C83D&title_bg=%23555555&icon=&icon_color=%23E7E7E7&title=hits&edge_flat=false)](https://hits.seeyoufarm.com)  <a href='https://ko-fi.com/FUEL4EP' target='_blank'><img height='20' style='border:0px;height:20px;' src='https://cdn.ko-fi.com/cdn/kofi1.png?v=2' border='0' alt='Buy Me a Coffee at ko-fi.com' /></a>
<br/>

# Arduino library for the ABLIC S-35770 24-bit binary counter IC with I2C interface

- The data sheet of the ABLIC S-35770 is available [here](https://www.ablic.com/en/doc/datasheet/counter_ic/S35770_I_E.pdf)
- An Arduino example is provided at [ABLIC_S35770_Demo.ino](./ABLIC_S35770_Demo/ABLIC_S35770_Demo.ino)
- This example sketch requires this [hardware setup](./ABLIC_S35770_Demo/Hardware_setup_for_example_sketch.png)
- The expected output of the example sketch is given [here](./ABLIC_S35770_Demo/Expected_output_of_serial_monitor)
- This Arduino library is used in the AsksinPP DIY radiation sensor [HB-UNI-Sensor1-RAD-AL53](../../sketches/HB-UNI-Sensor1-RAD-AL53/README.md)
- **IMPORTANT**: As documented in the data sheet, the ABLIC S-35770 counter stops counting between an I2C start and I2C stop condition, even if this I2C traffic is related to another I2C slave. Therefore, it is strongly recommended to refrain from I2C traffic during the counting periods of the ABLIC S-35770 counter!The provided example sketch is proving this unexpected behaviour.


## Lizenz

**Creative Commons BY-NC-SA**<br>
Give Credit, NonCommercial, ShareAlike

<a rel="license" href="http://creativecommons.org/licenses/by-nc-sa/4.0/"><img alt="Creative Commons License" style="border-width:0" src="https://i.creativecommons.org/l/by-nc-sa/4.0/88x31.png" /></a><br />This work is licensed under a <a rel="license" href="http://creativecommons.org/licenses/by-nc-sa/4.0/">Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License</a>.
