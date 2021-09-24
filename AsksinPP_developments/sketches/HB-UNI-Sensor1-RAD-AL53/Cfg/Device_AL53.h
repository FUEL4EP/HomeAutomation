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
#define cDEVICE_ID      { 0xF6, 0x08, 0x01 }       //change second 0x01 to your sensor's numeration index
#define cDEVICE_SERIAL  "AL53RAD001"               //change 'AL53RAD001' to your name's abbreviation
                    //   1234567890   IMPORTANT: exact 10 characters are required!


//---------------------------------------------------------
// Aktivierung der verwendeten Sensoren
// Über diese defines werden die real angeschlossenen Sensoren aktiviert.
// Andernfalls verwendet der Sketch Dummy-Werte als Messwerte (zum Testen der Anbindung an HomeMatic/RaspberryMatic/FHEM)
//

#define SENSOR_AL53

//Korrekturfaktor der Clock-Ungenauigkeit, wenn keine RTC verwendet wird
//#define SYSCLOCK_FACTOR    0.88

#if defined M1284P
#define CC1101_CS_PIN       4     // SS Tindie Pro Mini XL v2
#define CC1101_GDO0_PIN     10    // PD2 Tindie Pro Mini XL v2
#define CC1101_SCK_PIN      7     // SCK Tindie Pro Mini XL v2
#define CC1101_MOSI_PIN     5     // MOSI Tindie Pro Mini XL v2
#define CC1101_MISO_PIN     6     // MISO Tindie Pro Mini XL v2
#define LED_PIN 12                // PD4 Tindie Pro Mini XL v2
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
// 2) für StepUp/StepDown: tmBatteryResDiv, sense pin A0 (=pin 24), activation pin A1 (=pin 25) , Faktor = Rges/Rlow*1000, z.B. 141k/47k, Faktor (3*47k)/(1*47k)*1000 = 3000 (ATMega1284P)
// start with Faktor = 3000
// check in serial monitor the reported V2='accumulator voltage(MCU ADC) (x1000.0)' and compare with the reported V1='accumulator batteries voltage (x1000.0)'
// calculate voltage correction factor V1/V2
// calculaate total factor = 3000 * V2/V1 = 3000 * 3141 / 3157 = 2985 (example)
#define BAT_SENSOR tmBatteryResDiv<A0, A1, 3000> 
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
