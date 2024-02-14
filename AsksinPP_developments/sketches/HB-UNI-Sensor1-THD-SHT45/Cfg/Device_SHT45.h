//---------------------------------------------------------
// Device_Example.h
// 2019-10-09 Tom Major (Creative Commons)
// 2024-02-09 FUEL4EP (Creative Commons) adaptions to Sensirion SHT45
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
#define cDEVICE_ID      { 0xF6, 0x11, 0x01 }
#define cDEVICE_SERIAL  "SHT45THD01"
//                       1234567890     SERIAL must be exact 10 characters!

//---------------------------------------------------------
// Aktivierung der verwendeten Sensoren
// Über diese defines werden die real angeschlossenen Sensoren aktiviert.
// Andernfalls verwendet der Sketch Dummy-Werte als Messwerte (zum Testen der Anbindung an HomeMatic/RaspberryMatic/FHEM)
//

#define SENSORS_SHT45      // I2C Sensors SHT45

//---------------------------------------------------------
// Pin Definitionen Allgemein
#if defined M1284P
#define CC1101_CS_PIN       4     // SS
#define CC1101_GDO0_PIN     10    // PD2
#define CC1101_SCK_PIN      7     // SCK
#define CC1101_MOSI_PIN     5     // MOSI
#define CC1101_MISO_PIN     6     // MISO
#define LED_PIN 12                // PD4
#define CONFIG_BUTTON_PIN   22
#else
#define CC1101_GDO0_PIN     2     // PD2
#define CC1101_CS_PIN       10    // PB2
#define CC1101_MOSI_PIN     11    // PB3
#define CC1101_MISO_PIN     12    // PB4
#define CC1101_SCK_PIN      13    // PB5
#define LED_PIN 4
#define CONFIG_BUTTON_PIN   8
#endif


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
//#define BAT_SENSOR tmBattery
//------------
// 2) für StepUp/StepDown: tmBatteryResDiv, sense pin A0, activation pin A1, Faktor = Rges/Rlow*1000, z.B. Rlow=INF, Rhigh=INF, Faktor=INF/INF*1000 = 1000
// passend  für die beiden PCBs HB_TH_Sensor_SHT45_AllInOne_FUEL4EP oder HB_Stamp_ATMega1284P_FUEL4EP
#define BAT_SENSOR tmBatteryResDiv<A0, A1, 1000>

// 3) Echte Batteriespannungsmessung unter Last, siehe README und Thema "Babbling Idiot Protection"
// tmBatteryLoad: sense pin A0, activation pin D9, Faktor = Rges/Rlow*1000, z.B. 10/30 Ohm, Faktor 40/10*1000 = 4000, 200ms Belastung vor Messung
//#define BAT_SENSOR tmBatteryLoad<A0, 9, 4000, 200>
//---------------------------------------------------------

//---------------------------------------------------------
// Schwellwerte für Batteriespannungsmessung, Werte für eine Versorgung mit **einer** AA-Batterie und TPS61221 Aufwärtswandler für PCB HB_TH_Sensor_SHT45_AllInOne_FUEL4EP oder HB_Stamp_ATMega1284P_FUEL4EP
#define BAT_VOLT_LOW        11  // 1.1V
#define BAT_VOLT_CRITICAL   10  // 1.0V


#endif

// clang-format on
