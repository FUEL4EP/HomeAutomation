//------------------------------------------------------------------------------------------------------------------------
// HB-UNI-Sensor1-AQ-BME680_KF
// Version 1.0
// normal aH version
// (C) 2020 FUEL4EP (Creative Commons)
// https://creativecommons.org/licenses/by-nc-sa/4.0/
// You are free to Share & Adapt under the following terms:
// Give Credit, NonCommercial, ShareAlike
// +++
// AskSin++                                 2016 papa (Creative Commons) (https://github.com/pa-pa/AskSinPP/tree/master)
// HB-UNI-Sen-IAQ                           2019 jp112sdl Jérôme (https://homematic-forum.de/forum/viewtopic.php?f=76&t=49422) (https://github.com/jp112sdl/HB-UNI-Sen-IAQ)
// HB-UNI-Sensor1                           2018 TomMajor (Creative Commons) (https://github.com/TomMajor/SmartHome/tree/master/HB-UNI-Sensor1)
// ClosedCube_BME680_Arduino                2017 closedcube (MIT License) (https://github.com/FUEL4EP/ClosedCube_BME680_Arduino/tree/implement_Bosch_datasheet_integer_formulas)
// calculation of absolute humidity              Martin Kompf (https://www.kompf.de/weather/vent.html)
// Optimized CRC32 library for Arduino      2018 Erriez (MIT License) (https://github.com/Erriez/ErriezCRC32)
// Kalman Filter Library                    2019 Romain JL. FETICK  (MIT License) (https://github.com/rfetick/Kalman) or (https://www.arduino.cc/reference/en/libraries/kalman-filter-library/)
// BasicLinearAlgebra V3.7                  2016 tomstewart89 (MIT License) (https://github.com/tomstewart89/BasicLinearAlgebra/archive/refs/tags/3.7.zip) Wichtig: Version muss kleiner oder gleich 3.7 sein!
//------------------------------------------------------------------------------------------------------------------------
//
// this code supports only an Atmega1284P MCU, an Atmega328P MCU is NOT supported for memory size reasons 
//
//------------------------------------------------------------------------------------------------------------------------

//#define NDEBUG   // disable all serial debug messages; comment if you want to get debug messages in the serial monitor
//#define USE_CC1101_ALT_FREQ_86835  //use alternative frequency to compensate not correct working cc1101 modules
// 1) Standard: tmBattery, UBatt = Betriebsspannung AVR
#define BAT_SENSOR tmBattery

// the sensor HB-UNI-Sensor1-AQ-BME680_KF requires mandatorily an ATMega1284P MCU (Pro Mini XL - v2 - ATmega 1284p https://www.tindie.com/products/prominimicros/pro-mini-xl-v2-atmega-1284p/)

// please set the baud rate in the serial monitor as 38400 baud 


#define  EI_NOTEXTERNAL
#define  M1284P
#include <EnableInterrupt.h>
#include <AskSinPP.h>
#include <LowPower.h>
#include <Register.h>
#include <MultiChannelDevice.h>
#include "sensors/tmBattery.h"
#include "sensors/sens_bme680_KF.h"


#define PEERS_PER_CHANNEL   6
#define SAMPLINGINTERVALL_IN_SECONDS 240
#define BAT_VOLT_LOW        26  // 2.6V
#define BAT_VOLT_CRITICAL   25  // 2.5V

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

// #define EByte_E07_868MS10   // uncomment if an EByte E07-868MS10 module (red color) is used as CC1101 transceiver

// please install the addon hb-ep-devices-addon version >= 1.10 before pairing the sensor with the CCU/RaspberryMatic


//global offset and device parameter variables
double    temperature_offset, pressure_offset, humidity_offset;
uint8_t   max_decay_factor_upper_limit;
uint8_t   max_increase_factor_lower_limit;

//global start of free eeprom address space
uint16_t first_free_user_eeprom_address;

// all library classes are placed in the namespace 'as'
using namespace as;


// define all device properties
const struct DeviceInfo PROGMEM devinfo = {
  {0xf6, 0x06, 0x01},       // Device ID             // change 0x01 to your sensor's index number, if you use multiple HB-UNI-Sensor1-AQ-BME680_KF sensors, please increase the index for each sensor by 1
  "BME6800601",             // Device Serial         // change NAME to your name abbreviation, if you use multiple HB-UNI-Sensor1-AQ-BME680_KF sensors, please increase the index for each sensor by 1
// 1234567890                                        // device serial must be exactly 10 characters
  {0xf6, 0x06},             // Device Model Indoor   needs to fit to Addon XML hb-uni-sensor-AQ-BME680_KF.xml line 6:
                            //                       <parameter index="10.0" size="2.0" const_value="0xF606" /> 
  
  0x10,                     // Firmware Version
  as::DeviceType::THSensor, // Device Type
  {0x01, 0x00}              // Info Bytes
};

// Configure the used hardware

#if defined M1284P
typedef AvrSPI<CC1101_CS_PIN, CC1101_MOSI_PIN, CC1101_MISO_PIN, CC1101_SCK_PIN> SPIType;
typedef Radio<SPIType, CC1101_GDO0_PIN> RadioType;
#else
typedef AvrSPI<10, 11, 12, 13> SPIType;
typedef Radio<SPIType, 2> RadioType;
#endif

typedef StatusLed<LED_PIN> LedType;


typedef AskSin<LedType, BAT_SENSOR, RadioType> BaseHal;
class Hal : public BaseHal {
  public:
    void init (const HMID& id) {
      BaseHal::init(id);
#ifdef USE_CC1101_ALT_FREQ_86835
      //0x2165AA 868.325 MHz
      radio.initReg(CC1101_FREQ2, 0x21);
      radio.initReg(CC1101_FREQ1, 0x65);
      radio.initReg(CC1101_FREQ0, 0xAA);
#endif
      // measure battery every 1h
      battery.init(seconds2ticks(60UL * 60), sysclock);
      battery.low(BAT_VOLT_LOW);
      battery.critical(BAT_VOLT_CRITICAL);

    }

    bool runready () {
      return sysclock.runready() || BaseHal::runready();
    }
} hal;

DEFREGISTER(Reg0, MASTERID_REGS, DREG_LEDMODE, DREG_LOWBATLIMIT, DREG_TRANSMITTRYMAX, 0x20, 0x21, 0x22, 0x23, 0x24)
class SensorList0 : public RegList0<Reg0> {
  public:
    SensorList0(uint16_t addr) : RegList0<Reg0>(addr) {}

    bool updIntervall (uint16_t value) const {
      return this->writeRegister(0x20, (value >> 8) & 0xff) && this->writeRegister(0x21, value & 0xff);
    }
    uint16_t updIntervall () const {
      return (this->readRegister(0x20, 0) << 8) + this->readRegister(0x21, 0);
    }

    bool height (uint16_t value) const {
      return this->writeRegister(0x22, (value >> 8) & 0xff) && this->writeRegister(0x23, value & 0xff);
    }
    uint16_t height () const {
      return (this->readRegister(0x22, 0) << 8) + this->readRegister(0x23, 0);
    }
    
    bool    txPower(uint8_t v) const { return this->writeRegister(0x24,v); }
    uint8_t txPower() const { return this->readRegister(0x24,0); }

    void defaults () {
      clear();
      updIntervall(SAMPLINGINTERVALL_IN_SECONDS);  //sensor measurement period [s]
      ledMode(1);
      lowBatLimit(BAT_VOLT_LOW);
      transmitDevTryMax(6);
      txPower(7);         // maximum tx power as initial value: +10dB
      height(84);         // altitude above NN [m], set to height of your sensor's location
      DPRINTLN(F("Init of device parameters done"));
    }
};

DEFREGISTER(UReg1, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x14)
class SensorList1 : public RegList1<UReg1> {
  public:
    SensorList1 (uint16_t addr) : RegList1<UReg1>(addr) {}

    bool tempOffset10 (int32_t value) const {
      return
          this->writeRegister(0x01, (value >> 24) & 0xff) &&
          this->writeRegister(0x02, (value >> 16) & 0xff) &&
          this->writeRegister(0x03, (value >> 8) & 0xff) &&
          this->writeRegister(0x04, (value) & 0xff)
          ;
    }

    int32_t tempOffset10 () const {
      return
          ((int32_t)(this->readRegister(0x01, 0)) << 24) +
          ((int32_t)(this->readRegister(0x02, 0)) << 16) +
          ((int32_t)(this->readRegister(0x03, 0)) << 8) +
          ((int32_t)(this->readRegister(0x04, 0)))
          ;
    }

    bool humidOffset10 (int32_t value) const {
      return
          this->writeRegister(0x05, (value >> 24) & 0xff) &&
          this->writeRegister(0x06, (value >> 16) & 0xff) &&
          this->writeRegister(0x07, (value >> 8) & 0xff) &&
          this->writeRegister(0x08, (value) & 0xff)
          ;
    }

    int32_t humidOffset10 () const {
      return
          ((int32_t)(this->readRegister(0x05, 0)) << 24) +
          ((int32_t)(this->readRegister(0x06, 0)) << 16) +
          ((int32_t)(this->readRegister(0x07, 0)) << 8) +
          ((int32_t)(this->readRegister(0x08, 0)))
          ;
    }

    bool pressOffset10 (int32_t value) const {
      return
          this->writeRegister(0x09, (value >> 24) & 0xff) &&
          this->writeRegister(0x0A, (value >> 16) & 0xff) &&
          this->writeRegister(0x0B, (value >> 8) & 0xff) &&
          this->writeRegister(0x0C, (value) & 0xff)
          ;
    }

    int32_t pressOffset10 () const {
      return
          ((int32_t)(this->readRegister(0x09, 0)) << 24) +
          ((int32_t)(this->readRegister(0x0A, 0)) << 16) +
          ((int32_t)(this->readRegister(0x0B, 0)) << 8) +
          ((int32_t)(this->readRegister(0x0C, 0)))
          ;
    }

    bool max_decay_factor_upper_limit (int32_t value) const {
      return
          this->writeRegister(0x0D, (value >> 24) & 0xff) &&
          this->writeRegister(0x0E, (value >> 16) & 0xff) &&
          this->writeRegister(0x0F, (value >> 8) & 0xff) &&
          this->writeRegister(0x10, (value) & 0xff)
          ;
    }

    int32_t max_decay_factor_upper_limit () const {
      return
          ((int32_t)(this->readRegister(0x0D, 0)) << 24) +
          ((int32_t)(this->readRegister(0x0E, 0)) << 16) +
          ((int32_t)(this->readRegister(0x0F, 0)) << 8) +
          ((int32_t)(this->readRegister(0x10, 0)))
          ;
    }

    bool max_increase_factor_lower_limit (int32_t value) const {
      return
          this->writeRegister(0x11, (value >> 24) & 0xff) &&
          this->writeRegister(0x12, (value >> 16) & 0xff) &&
          this->writeRegister(0x13, (value >> 8) & 0xff) &&
          this->writeRegister(0x14, (value) & 0xff)
          ;
    }

    int32_t max_increase_factor_lower_limit () const {
      return
          ((int32_t)(this->readRegister(0x11, 0)) << 24) +
          ((int32_t)(this->readRegister(0x12, 0)) << 16) +
          ((int32_t)(this->readRegister(0x13, 0)) << 8) +
          ((int32_t)(this->readRegister(0x14, 0)))
          ;
    }

 


    void defaults () {                      // these defaults are read and used if you press the config button for >6 seconds
      clear();
      tempOffset10(-15);                    // temperature measurement offset, multiplied by 10 [K], calibrate your sensor's characteristics, enter in WebUI as device parameter for dynamic adjustment
      humidOffset10(2);                     // humidity measurement offset, multiplied by 10 [%], calibrate your sensor's characteristics, enter in WebUI as device parameter for dynamic adjustment
      pressOffset10(-4);                    // pressure measurement offset, multiplied by 10 [hPa], calibrate your sensor's characteristics, enter in WebUI as device parameter for dynamic adjustment
      max_decay_factor_upper_limit(60);     // IIR's filter max decay value of gas resistor upper limit
      max_increase_factor_lower_limit(40);  // IIR's filter max increase value of gas resistor lower limit 
      DPRINTLN(F("Init of channel parameters done"));
    }
    
};


class WeatherEventMsg : public Message {
  public:
    void init(uint8_t msgcnt, int16_t temp, uint16_t airPressure, uint16_t my_humidity, uint8_t aq_level, uint16_t aq_state_scaled, uint16_t gas_resistance_raw_scaled, uint16_t gas_resistance_min_scaled, uint16_t gas_resistance_max_scaled, bool batlow, uint16_t operatingVoltage1000) {
        
      uint8_t t1 = (temp >> 8) & 0x7f;
      uint8_t t2 = temp & 0xff;
      
      if ( batlow == true ) {
        t1 |= 0x80; // set bat low bit
      }
      
      // als Standard wird BCAST gesendet um Energie zu sparen, siehe Beschreibung unten.
      // Bei jeder 40. Nachricht senden wir stattdessen BIDI|WKMEUP, um eventuell anstehende Konfigurationsänderungen auch
      // ohne Betätigung des Anlerntaster übernehmen zu können (mit Verzögerung, worst-case x Sendeintervall).
    
      // als Standard wird BCAST gesendet um Energie zu sparen, siehe Beschreibung unten.
      // Bei jeder 40. Nachricht senden wir stattdessen BIDI|WKMEUP, um eventuell anstehende Konfigurationsänderungen auch
      // ohne Betätigung des Anlerntaster übernehmen zu können (mit Verzögerung, worst-case x Sendeintervall).
      uint8_t flags = BCAST;
      if ((msgcnt % 40) == 2) {
        flags = BIDI | WKMEUP;
      }
      Message::init(0x1A, msgcnt, 0x70, flags, t1, t2);  //0x1A = 26 Bytes to be sent per message, see tail of addon's XML hb-uni-sensor-AQ-BME680.xml
      pload[0] =   (airPressure >> 8) & 0xff;
      pload[1] =   (airPressure >> 0) & 0xff;

      pload[2] =   (my_humidity >> 8) & 0xff;
      pload[3] =   (my_humidity >> 0) & 0xff;  
      
      pload[4] =   aq_level & 0xff;
      
      pload[5] =   (aq_state_scaled >> 8) & 0xff;
      pload[6] =   (aq_state_scaled >> 0) & 0xff;

      pload[7] =   (gas_resistance_raw_scaled >> 8) & 0xff;
      pload[8] =   (gas_resistance_raw_scaled >> 0) & 0xff;
      
      pload[9] =   (gas_resistance_min_scaled >> 8) & 0xff;
      pload[10] =  (gas_resistance_min_scaled >> 0) & 0xff;
      
      pload[11] =  (gas_resistance_max_scaled >> 8) & 0xff;
      pload[12] =  (gas_resistance_max_scaled >> 0) & 0xff;

      pload[13] =  (operatingVoltage1000 >> 8) & 0xff;
      pload[14] =  (operatingVoltage1000 >> 0) & 0xff;
      
      // max payload index is 14!! Total payload ist max 17 bytes including temperature!
    }
};

class WeatherChannel : public Channel<Hal, SensorList1, EmptyList, List4, PEERS_PER_CHANNEL, SensorList0>, public Alarm {
    WeatherEventMsg msg;
    Sens_Bme680<0x76>   bme680; // please insert your sensor's I2C address between the brackets, here 0x76
    uint16_t            millis;
    uint16_t            operatingVoltage1000;

  public:
    WeatherChannel () : Channel(), Alarm(10), millis(0) {}
    virtual ~WeatherChannel () {}

    virtual void trigger (__attribute__ ((unused)) AlarmClock& clock) {
      uint8_t msgcnt = device().nextcount();
      // reactivate for next measure
      tick = delay();
      clock.add(*this);

      device().battery().update();                            // get current battery voltage; measure every sampling cycle
      operatingVoltage1000 = device().battery().current();    // BatteryTM class, mV resolution

      bme680.measure(temperature_offset, pressure_offset, humidity_offset, max_decay_factor_upper_limit, max_increase_factor_lower_limit, operatingVoltage1000, device().battery().low());
       
      DPRINT(F("corrected T/H                          = "));  DDEC(bme680.temperature()); DPRINT("/"); DDECLN(bme680.humidity());
      DPRINT(F("Pressure NN                            = "));  DDECLN(bme680.pressureNN());
      DPRINT(F("scaled aq state                        = "));  DDECLN(bme680.aq_state_scaled());
      DPRINT(F("scaled gas resistance                  = "));  DDECLN(bme680.gas_resistance_raw_scaled());
      DPRINT(F("battery voltage                        = "));  DDECLN(operatingVoltage1000);

      msg.init( msgcnt, bme680.temperature(), bme680.pressureNN(), bme680.humidity(), bme680.aq_level(), bme680.aq_state_scaled(), bme680.gas_resistance_raw_scaled(), bme680.gas_resistance_min_scaled(), bme680.gas_resistance_max_scaled(), device().battery().low(), operatingVoltage1000);
      if (msg.flags() & Message::BCAST) {
        device().broadcastEvent(msg, *this);
      } else {
        device().sendPeerEvent(msg, *this);
      }
    }

    uint32_t delay () {
      return seconds2ticks(device().getList0().updIntervall());
    }
    void setup(Device<Hal, SensorList0>* dev, uint8_t number, uint16_t addr) {
      Channel::setup(dev, number, addr);
      
      bme680.init(this->device().getList0().height(), max_decay_factor_upper_limit, max_increase_factor_lower_limit, first_free_user_eeprom_address);

      sysclock.add(*this);
  
    }

     void configChanged() {
      DPRINTLN("* Config Changed                                         : List1");
      DPRINT(F("* Temperature Offset x10                                 : ")); DDECLN(this->getList1().tempOffset10());
      DPRINT(F("* Humidity Offset x10                                    : ")); DDECLN(this->getList1().humidOffset10());
      DPRINT(F("* Pressure Offset x10                                    : ")); DDECLN(this->getList1().pressOffset10());
      max_decay_factor_upper_limit    = (uint8_t)this->getList1().max_decay_factor_upper_limit();
      max_increase_factor_lower_limit = (uint8_t)this->getList1().max_increase_factor_lower_limit();
      DPRINT(F("* max IIR filter decay factor of upper limit             : ")); DDECLN(this->getList1().max_decay_factor_upper_limit());
      DPRINT(F("* max IIR filter increase factor of lower limit          : ")); DDECLN(this->getList1().max_increase_factor_lower_limit());
      temperature_offset = (double)this->getList1().tempOffset10()  / 10.0;
      pressure_offset    = (double)this->getList1().pressOffset10() / 10.0;
      humidity_offset    = (double)this->getList1().humidOffset10() / 10.0;
      DPRINT(F("* Temperature Offset                                     : ")); DDECLN(temperature_offset);
      DPRINT(F("* Humidity Offset                                        : ")); DDECLN(humidity_offset);
      DPRINT(F("* Pressure Offset                                        : ")); DDECLN(pressure_offset);
      DPRINT(F("* max IIR filter decay factor of upper limit             : ")); DDECLN(max_decay_factor_upper_limit);
      DPRINT(F("* max IIR filter increase factor of lower limit          : ")); DDECLN(max_increase_factor_lower_limit);
    }

    uint8_t status () const {
      return 0;
    }

    uint8_t flags () const {
      return 0;
    }
};

class AQDevice : public MultiChannelDevice<Hal, WeatherChannel, 1, SensorList0> {
  public:
    typedef MultiChannelDevice<Hal, WeatherChannel, 1, SensorList0> TSDevice;
    AQDevice(const DeviceInfo& info, uint16_t addr) : TSDevice(info, addr) {}
    virtual ~AQDevice () {}

    virtual void configChanged () {
      TSDevice::configChanged();
      DPRINTLN("* Config Changed                                         : List0");
      DPRINT(F("* LED Mode                                               : ")); DDECLN(this->getList0().ledMode());    
      DPRINT(F("* Low Bat Limit                                          : ")); DDECLN(this->getList0().lowBatLimit()); 
      DPRINT(F("* Sendeversuche                                          : ")); DDECLN(this->getList0().transmitDevTryMax());                   
      DPRINT(F("* Sendeintervall                                         : ")); DDECLN(this->getList0().updIntervall());
      DPRINT(F("* Hoehe ueber NN                                         : ")); DDECLN(this->getList0().height());
      
      // Einstellung der CC1101 Tx Sendeleistung

      // setting of CC1101's PATABLE Tx output power settings according tables 37 (wire-wound inductors, e.g. EByte E07-868MS10)  and 39 (multi-layer inductors, e.g. no name green module) of TI CC1101 data sheet https://www.ti.com/lit/ds/symlink/cc1101.pdf
#ifdef EByte_E07_868MS10
      const byte powerMode[8] = { 0x03, 0x17, 0x1D, 0x26, 0x50, 0x81, 0xCD, 0xC0 };  // wire-wound inductor Tx output levels (e.g. red color PCB EByte E07-868MS10), see table 37 of CC1101 data sheet
                                                                                      // 0xC0 allows +11/dBm+12dBm Tx output power, change 0xC0 to 0xC5 for max. 10 dBm Tx output power
#else
      const byte powerMode[8] = { 0x03, 0x0F, 0x1E, 0x27, 0x50, 0x81, 0xCB, 0xC2 };  // multi-layer inductor Tx output levels (e.g. no name green color PCB CC1101 module), see table 39 of CC1101 data sheet
#endif
      uint8_t txPower = min(this->getList0().txPower(), 7);
      radio().initReg(CC1101_PATABLE, powerMode[txPower]);
      DPRINT(F("txPower: "));DDECLN(txPower);
           
    }
};

AQDevice sdev(devinfo, 0x20);
ConfigButton<AQDevice> cfgBtn(sdev);

void setup () {
  DINIT(38400, ASKSIN_PLUS_PLUS_IDENTIFIER);
  sdev.init(hal);
  buttonISR(cfgBtn, CONFIG_BUTTON_PIN);
  sdev.initDone();
  DPRINT("List0 dump: "); sdev.getList0().dump();
  DDEVINFO(sdev)
  first_free_user_eeprom_address = sdev.getUserStorage().getAddress();  //first_free_user_eeprom_address iss a global variable
  DPRINT(F("first_free_user_eeprom_address  : ")); DDECLN(first_free_user_eeprom_address);
}

void loop() {
  bool worked = hal.runready();
  bool poll = sdev.pollRadio();
  if ( worked == false && poll == false ) {
    hal.activity.savePower<Sleep<>>(hal);
  }
}
