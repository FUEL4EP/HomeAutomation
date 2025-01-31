# avr_stl
This is an implementation of a C++ standard library packaged as an Arduino library (arduinoSTL). The library is based on the library from Mike Matera:

git clone https://github.com/manchoz/avr_stl.git

which is a fork of

https://github.com/mike-matera/ArduinoSTL

**Changes are:**

remove avr_stl/src/del_ops.cpp<br/>
remove avr_stl/src/del_opvs.cpp<br/>
remove avr_stl/src/new_handler.cpp

for avoiding duplicated definitions

## Using printf() and scanf()
The ArduinoSTL header file contains code to bind a serial port of your choice to
the stdio primitives. This happens automatically but the user must still call
Serial.begin()

```c++
#include <ArduinoSTL.h>

void setup() {
  Serial.begin(9600); 
  printf("Hello World\n");
}
```

## Using ```cin``` an ```cout```
When you include this header file you automatically get cin and cout based on ```Serial```. See below for how to specify your own device. Here's an example sketch using ```cin``` and ```cout``` .

```c++
#include <ArduinoSTL.h>

using namespace std;

void setup() {
  Serial.begin(9600);
  cout << "Feed me an integers." << endl;
}

void loop() {
  int foo;
  if (cin >> foo) { 
    cout << "You fed me " << foo << endl;
  }else{
    cin.clear();
    cin.ignore();
  }
}
```
## Changing the Serial Port 
You can change what serial port that ```cin```, ```cout``` and ```printf()``` use. You can use built-in serial ports (e.g. ```Serial1``` on Leonardo) or you can use software serial ports that implement ```Stream```. 

### Using a Built-in Port 
In ```src/ArduinoSTL.cpp``` change the value of ```ARDUINOSTL_DEFAULT_SERIAL```. Leave the other defaults uncommented. 

### Using a SoftwareSerial port. 
Set ```ARDUINO_DEFAULT_SERAL``` to ```NULL```. Comment out the other defaults. 

Here's an example sketch that uses SofwareSerial:

```c++
#include <ArduinoSTL.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerial(0, 1);

namespace std { 
  ohserialstream cout(mySerial);
  ihserialstream cin(mySerial);
}

void setup() {
  mySerial.begin(9600);
  ArduinoSTL_Serial.connect(mySerial);
}
```

## Avoiding Instantiation of ```cin``` and ```cout```
Comment out ```ARDUINOSTL_DEFAULT_CIN_COUT``` and nothing will be instantiated. You must comment out this flag if you intend to select a non-default serial port. There's no appreciable overhead for using ```printf()``` so you cannot currently avoid initializaing it.

## Known Issues

Printing of floats and doubles using ```cout``` ignores format specifiers. 

uClibc seems to be fairly complete. Strings and vectors both work, even with the limited amount of heap available to Arduino. The uClibc++ status page can be found here: 

https://cxx.uclibc.org/status.html

Always use the latest Arduino IDE. This library uses the Arduino IDE Library Specification rev.2.1 with features only available on Arduino 1.6.10 and higher. The specification can be found here:

https://github.com/arduino/Arduino/wiki/Arduino-IDE-1.5:-Library-specification

## License

The uClibc++ library is licensed under the LGPL. This project adopts the LGPL to be compatible with the bulk of the code that it uses. Unless otherwise noted all code is licensed under the LGPL. There's one exception: 

  - src/serstream is licensed under the BSD license according to Andy Brown's wishes here: http://andybrown.me.uk/terms-and-conditions/
