//---------------------------------------------------------
// Device_Example.h
// 2019-10-09 Tom Major (Creative Commons)
// https://creativecommons.org/licenses/by-nc-sa/4.0/
// You are free to Share & Adapt under the following terms:
// Give Credit, NonCommercial, ShareAlike
// +++
// AskSin++ 2016-10-31 papa Creative Commons
//---------------------------------------------------------

// clang-format off

#ifndef _DEVICE_CFG_H_
#define _DEVICE_CFG_H_


//---------------------------------------------------------
// Definition von Device ID und Device Serial
// Bei mehreren Geräten des gleichen Typs (HB-UNI-Sensor1) muss Device ID und Device Serial unterschiedlich sein!
#define cDEVICE_ID      { 0xF6, 0x01, 0x01 }       //change second 0x01 to your sensor's numeration index
#define cDEVICE_SERIAL  "SCD30SENS1"               //change 'SENS' to your name's abbreviation
                    //   1234567890   IMPORTANT: exact 10 characters are required!


//---------------------------------------------------------
// Aktivierung der verwendeten Sensoren
// Über diese defines werden die real angeschlossenen Sensoren aktiviert.
// Andernfalls verwendet der Sketch Dummy-Werte als Messwerte (zum Testen der Anbindung an HomeMatic/RaspberryMatic/FHEM)
//

#define SENSOR_SCD30   


//---------------------------------------------------------
// Pin Definitionen Allgemein
#define CONFIG_BUTTON_PIN   8
#define LED_PIN             4


//---------------------------------------------------------
// Pin und Address Definitionen Sensoren



//---------------------------------------------------------
// Über diese defines wird die Clock festgelegt
// CLOCK_SYSCLOCK: 8MHz Quarz an XTAL oder 8MHz int. RC-Oszillator, Sleep Strom ca. 4uA
// CLOCK_RTC:      8MHz int. RC-Oszillator, 32.768kHz Quarz an XTAL, Sleep Strom ca. 1uA
#define CLOCK_SYSCLOCK
//#define CLOCK_RTC


//---------------------------------------------------------
// Schaltungsvariante und Pins für Batteriespannungsmessung, siehe README
//------------
// 1) Standard: tmBattery, UBatt = Betriebsspannung AVR
//#define BAT_SENSOR tmBattery
//------------
// 2) für StepUp/StepDown: tmBatteryResDiv, sense pin A2, activation pin A3, Faktor = Rges/Rlow*1000, z.B. 94k/47k, Faktor (3*47k)/(2*47k)*1000 = 3333; correction factor2 = 2653/2986, total factor = 3333 * 2653/2986
#define BAT_SENSOR tmBatteryResDiv<A2, A3, 2961>
//------------
// 3) Echte Batteriespannungsmessung unter Last, siehe README und Thema "Babbling Idiot Protection"
// tmBatteryLoad: sense pin A0, activation pin D9, Faktor = Rges/Rlow*1000, z.B. 10/30 Ohm, Faktor 40/10*1000 = 4000, 200ms Belastung vor Messung
//#define BAT_SENSOR tmBatteryLoad<A0, 9, 4000, 200>


//---------------------------------------------------------
// Schwellwerte für Batteriespannungsmessung
#define BAT_VOLT_LOW        25  // 2.5V accumulator voltage for low battery signalling measured at pin A2 (see above)
#define BAT_VOLT_CRITICAL   22  // 2.2V accuumulator voltage for switch off (sleep forever)


#endif

// clang-format on
