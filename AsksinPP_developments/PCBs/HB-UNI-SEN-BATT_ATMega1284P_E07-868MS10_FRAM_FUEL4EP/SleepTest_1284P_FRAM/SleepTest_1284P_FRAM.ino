// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// SleepTest ATMega1284P, CC1101, FRAMs 2x Fujitsu MB85RS2MTPF (AVR Power-down Mode)
// fitting to PCB HB-UNI-SEN-BATT_ATMega1284P_E07-868MS10_FRAM_FUEL4EP (https://github.com/FUEL4EP/HomeAutomation/tree/master/AsksinPP_developments/PCBs/HB-UNI-SEN-BATT_ATMega1284P_E07-868MS10_FRAM_FUEL4EP)
// 2019-01-22 Tom Major (Creative Commons)
// 2024-06-14 FUEL4EP (Creative Commons)
// https://creativecommons.org/licenses/by-nc-sa/3.0/
// Clock Fuses: RC-Oszillator 8MHz, BOD off
// alternativ 8MHz Quarz an XTAL
//
// Der AVR wechselt mit diesem Sketch zwischen 8sec Aktiv Mode und 8sec power-down Mode mit Watchdog wakeup
// !! Falls mit der HW alles stimmt benötigt die Schaltung im power-down Mode nur ca. 12uA !!
//
// Auf die uA-Messung sollte man nur dann kurz umschalten wenn der AVR im power-down Mode ist (LED aus) und vor Ablauf der 8sec
// wieder zurück auf den mA-Messbereich. Andernfalls wird der AVR im aktivem Zustand eventuell nicht wieder anlaufen, da der Spannungsabfall
// dann über den uA-Messbereich des Multimeters zu hoch ist.
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#include <Adafruit_FRAM_SPI.h>
#include <SPI.h>
#include "LowPower.h"


#define ATMega1284P

// -------------------------------------------------------------------------
// CC1101 power down

#if defined ATMega1284P
  #define CC1101_GDO0        10           // PD2
  #define CC1101_CS_PIN       4           // PB4
#else // ATMega328P
  #define CC1101_GDO0         2           // GDO0 input interrupt pin
  #define CC1101_CS_PIN      10           // PB2
#endif

#define CC1101_SRES         0x30        // Reset CC1101 chip
#define CC1101_SIDLE        0x36        // Exit RX / TX, turn off frequency synthesizer and exit Wake-On-Radio mode if applicable
#define CC1101_SPWD         0x39        // Enter power down mode when CSn goes high

#define cc1101_Select()     digitalWrite(CC1101_CS_PIN, LOW)
#define cc1101_Deselect()   digitalWrite(CC1101_CS_PIN, HIGH)
#define wait_Miso()         while(digitalRead(MISO)>0)



const int ledPin = 12;  // adapt to your PCB as needed

// please adapt these pin assignments to your hardware if you do not use the PCB HB-UNI-SEN-BATT_ATMega1284P_E07-868MS10_FRAM_FUEL4EP
const uint8_t FRAM_CS1_PIN = 26;
const uint8_t FRAM_CS2_PIN = 27;
const uint8_t FRAM_SCK = 7;
const uint8_t FRAM_MISO = 6;
const uint8_t FRAM_MOSI = 5;

const uint32_t  SPI_FREQ                    = (uint32_t)4000000;                 // SPI freq is limited to half of CPU frequency

// use hardware SPI mode to access the two FRAMs
// first FRAM
Adafruit_FRAM_SPI fram_1 = Adafruit_FRAM_SPI(FRAM_CS1_PIN, &SPI, SPI_FREQ);  // use hardware SPI
// second FRAM
Adafruit_FRAM_SPI fram_2 = Adafruit_FRAM_SPI(FRAM_CS2_PIN, &SPI, SPI_FREQ);  // use hardware SPI


void setup() 
{

    // cc1101_powerdown nur aktivieren falls wirklich ein CC1101 angeschlossen ist!
    cc1101_powerdown();
    // FRAMs_powerdown nur aktivieren falls wirklich zwei Fujitsu MB85RS2MTPF FRAMs angeschlossen sind!
    FRAMs_powerdown();
    pinMode(ledPin, OUTPUT);
}

void loop() 
{
    digitalWrite(ledPin, HIGH);
    delay(8000); 
    digitalWrite(ledPin, LOW);
    LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
    
}



void cc1101_powerdown()
{
  SPI.begin();                          // Initialize SPI interface

  (void)fram_1.enter_low_power_mode();
  (void)fram_2.enter_low_power_mode();
  
  pinMode(CC1101_GDO0, INPUT);          // Config GDO0 as input

  //reset();                            // Reset CC1101
  cc1101_Deselect();                    // Deselect CC1101
  delayMicroseconds(5);
  cc1101_Select();                      // Select CC1101
  delayMicroseconds(10);
  cc1101_Deselect();                    // Deselect CC1101
  delayMicroseconds(41);
  cc1101_Select();                      // Select CC1101

  wait_Miso();                          // Wait until MISO goes low
  SPI.transfer(CC1101_SRES);            // Send reset command strobe
  wait_Miso();                          // Wait until MISO goes low

  cc1101_Deselect();                    // Deselect CC1101
  // reset() end

  delay(100);
  
  //cmdStrobe(byte cmd) 
  cc1101_Select();                      // Select CC1101
  wait_Miso();                          // Wait until MISO goes low
  SPI.transfer(CC1101_SIDLE);           // Send strobe command
  cc1101_Deselect();                    // Deselect CC1101
  
  //cmdStrobe(byte cmd) 
  cc1101_Select();                      // Select CC1101
  wait_Miso();                          // Wait until MISO goes low
  SPI.transfer(CC1101_SPWD);            // Send strobe command
  cc1101_Deselect();                    // Deselect CC1101

  SPI.end();
  delay(10);
}


void FRAMs_powerdown()
{
  SPI.begin();                          // Initialize SPI interface
  // initialize the FRAMs
  (void)fram_1.begin();
  (void)fram_2.begin();
  // enter low power mode of FRAMs
  (void)fram_1.enter_low_power_mode();
  (void)fram_2.enter_low_power_mode();

  SPI.end();
  delay(10);
}
