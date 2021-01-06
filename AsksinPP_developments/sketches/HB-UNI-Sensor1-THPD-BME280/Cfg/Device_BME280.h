//---------------------------------------------------------
// Device_Example.h
// 2019-10-09 Tom Major (Creative Commons)
// 2020-11-29 FUEL4EP (Creative Commons) adaptions to Bosch BME280
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
#define cDEVICE_ID      { 0xF6, 0x04, 0x01 }
#define cDEVICE_SERIAL  "BME280THP1"
//                       1234567890     SERIAL must be exact 10 characters!

//---------------------------------------------------------
// Aktivierung der verwendeten Sensoren
// Über diese defines werden die real angeschlossenen Sensoren aktiviert.
// Andernfalls verwendet der Sketch Dummy-Werte als Messwerte (zum Testen der Anbindung an HomeMatic/RaspberryMatic/FHEM)
//

#define SENSOR_BME280      // I2C Sensor

//---------------------------------------------------------
// Pin Definitionen Allgemein
#define CONFIG_BUTTON_PIN   8
#define LED_PIN             4


//---------------------------------------------------------
// Pin und Address Definitionen Sensoren

//#define DIGINPUT_PIN      A1


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
#define BAT_SENSOR tmBattery
//------------
// 2) für StepUp/StepDown: tmBatteryResDiv, sense pin A0, activation pin D9, Faktor = Rges/Rlow*1000, z.B. 470k/100k, Faktor 570k/100k*1000 = 5700
//#define BAT_SENSOR tmBatteryResDiv<A0, 9, 5700>
//------------
// 3) Echte Batteriespannungsmessung unter Last, siehe README und Thema "Babbling Idiot Protection"
// tmBatteryLoad: sense pin A0, activation pin D9, Faktor = Rges/Rlow*1000, z.B. 10/30 Ohm, Faktor 40/10*1000 = 4000, 200ms Belastung vor Messung
//#define BAT_SENSOR tmBatteryLoad<A0, 9, 4000, 200>


//---------------------------------------------------------
// Schwellwerte für Batteriespannungsmessung
#define BAT_VOLT_LOW        26  // 2.6V  increased after some tests
#define BAT_VOLT_CRITICAL   23  // 2.3V


#endif

// clang-format on
