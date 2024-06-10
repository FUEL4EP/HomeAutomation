# Diamex Prog-S2 @ Arduino IDE ![License: CC BY-NC-SA 4.0](https://img.shields.io/badge/License-CC%20BY--NC--SA%204.0-lightgrey.svg)[![Hits](https://hits.seeyoufarm.com/api/count/incr/badge.svg?url=https%3A%2F%2Fgithub.com%2FFUEL4EP%2FHomeAutomation%2Ftree%2Fmaster%2FAsksinPP_developments%2FHowTos%2FISP%2520Programmer%2FDiamex%2520Prog-S2%2FLinux%2FArduino%2520IDE&count_bg=%2379C83D&title_bg=%23555555&icon=&icon_color=%23E7E7E7&title=hits&edge_flat=false)](https://hits.seeyoufarm.com) <a href='https://ko-fi.com/FUEL4EP' target='_blank'><img height='20' style='border:0px;height:20px;' src='https://cdn.ko-fi.com/cdn/kofi1.png?v=2' border='0' alt='Buy Me a Coffee at ko-fi.com' /></a>

# installation

- add [add_this_to_programmers.txt](./programmers.txt/add_this_to_programmers.txt) to your Arduino IDE installation's programmers.txt files
- e.g. located at ~/.arduino15/packages/MightyCore/hardware/avr/3.0.1/programmers.txt
- select in Arduino IDE as programmer: Tools => Programmer => DIAMEX-AVR :

![pic](./DIAMEX-AVR.png)

- before programming, select the port; see below

- program the Arduino in Arduino IDE: Sketch => Hochladen mit Programmer

# identify the COM port

- Linux already has the required drivers installed
- the virtual COM ports names are /dev/ttyUSB0, /dev/ttyACM0 or similar
- identify your system's port name with the command after having connected the USB cable of the progranmer  to your system:
> sudo dmesg -T

- example dmesg output is [here](./dmesg.log); virtual COM port is ttyACM0

- use this virtual port name in your avrdude scripts as port, e.g. :
> avrdude -v -pm1284p -P /dev/ttyACM0 -c stk500v2  -F -b 9600 -Ulfuse:w:0xFD:m -Uhfuse:w:0x91:m -Uefuse:w:0xFF:m  #8 MHz crystal OSC
> 

# select the COM port

- for programming select in the Arduino IDE: Werkzeuge => Ports the above identified port

![pic](./select_port.png)

- for debugging with the FTDI debugger, it may be necessary to switch the port, e.g. to '/dev/ttyUSB0' , before invoking 'Werkzeuge => Serieller Monitor'


# IMPORTANT

- before programming, select the correct supply voltage for Arduino (board):
![pic](./voltage_settings.png) 