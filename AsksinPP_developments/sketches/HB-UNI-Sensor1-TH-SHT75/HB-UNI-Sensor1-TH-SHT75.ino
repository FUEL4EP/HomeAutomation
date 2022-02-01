//---------------------------------------------------------
// HB-UNI-Sensor1 Sensirion SHT75 high accuracy Temperature and Humidity Sensor with option for 24 moving average calculation for temperature
// Firmware Version 1.1
// (C) 2018-2020 Tom Major (Creative Commons)
// (C) 2020-2022 FUEL4EP   (Creative Commons)          adaptions to Sensirion SHT75 high accuracy sensor; added moving average filter in firmware 1.1
// https://creativecommons.org/licenses/by-nc-sa/4.0/
// You are free to Share & Adapt under the following terms:
// Give Credit, NonCommercial, ShareAlike
// +++
// AskSin++ 2016-10-31 papa Creative Commons
//---------------------------------------------------------
//please install the addon hb-ep-devices-addon version >= 1.13 before pairing the sensor with the CCU/RaspberryMatic
//---------------------------------------------------------


//---------------------------------------------------------
// remove comment '//' in front of  '#define NDEBUG' if your application is working in debug mode
//
//#define NDEBUG
//#define USE_CC1101_ALT_FREQ_86835  //use alternative frequency to compensate not correct working cc1101 modules


// switch on the option for calculating a 24h moving average for the measured temperature
// note: the available local RAM is getting extremely tight with this option. Therefore, please do NOT cahnge below code in this case
#define CALCULATE_MOVING_AVERAGES

//---------------------------------------------------------
// define this to read the device id, serial and device type from bootloader section
// #define USE_OTA_BOOTLOADER

#define EI_NOTEXTERNAL
#include <EnableInterrupt.h>
#include <AskSinPP.h>
#include <LowPower.h>
#include <MultiChannelDevice.h>
#include <Register.h>
#include "Sensors/tmBattery.h"

// #define EByte_E07_868MS10   // uncomment if an EByte E07-868MS10 module (red color) is used as CC1101 transceiver


#include "Cfg/Device_SHT75.h"


// number of available peers per channel
#define PEERS_PER_CHANNEL 6

// all library classes are placed in the namespace 'as'
using namespace as;

//correction factor of the clock inaccuracy (ceramic resonator is default on Arduino boards), if no quartz RTC is used, wee also HB-UNI-Sen-CURRENT from Jérôme ( jp112sdl )
#define SYSCLOCK_FACTOR    0.944   // adjust to get sampling data exactly every 600 seconds

#ifdef SENSOR_SHT75
#include "Sensors/Sens_SHT75.h"    // HB-UNI-Sensor1 custom sensor class
#endif


#ifdef CLOCK_SYSCLOCK
#define CLOCK sysclock
#define SAVEPWR_MODE Sleep<>
#elif defined CLOCK_RTC
#define CLOCK rtc
#define SAVEPWR_MODE SleepRTC
#undef seconds2ticks
#define seconds2ticks(tm) (tm)
#else
#error INVALID CLOCK OPTION
#endif

// define all device properties
// Bei mehreren Geräten des gleichen Typs (HB-UNI-Sensor1) muss Device ID und Device Serial unterschiedlich sein!
// Device ID und Device Serial werden aus einer .h Datei (hier im Beispiel Cfg/Device_Example.h) geholt um mehrere Geräte ohne weitere Änderungen des
// Sketches flashen zu können.
const struct DeviceInfo PROGMEM devinfo = {
  cDEVICE_ID,        // Device ID           defined in ./Cfg/Device_SHT75.h
  cDEVICE_SERIAL,    // Device Serial       defined in ./Cfg/Device_SHT75.h
#ifdef CALCULATE_MOVING_AVERAGES
  { 0xF6, 0x0A },    // Device Model        needs to fit to Addon XML hb-uni-sensor-TH-SHT75_MA.xml line 6:
  //                     <parameter index="10.0" size="2.0" const_value="0xF60A" /
  0x11,             // Firmware Version 1.1 with 24 h moving average filter for temperature
#else
  { 0xF6, 0x03 },    // Device Model        needs to fit to Addon XML hb-uni-sensor-TH-SHT75.xml line 6:
  //                     <parameter index="10.0" size="2.0" const_value="0xF603" /
  0x10,             // Firmware Version 1.0 without 24 h moving average filter for temperature
#endif
  as::DeviceType::THSensor,    // Device Type
  { 0x01, 0x01 }               // Info Bytes
};

// Configure the used hardware
typedef AvrSPI<10, 11, 12, 13>                 SPIType;
typedef Radio<SPIType, 2>                      RadioType;
typedef StatusLed<LED_PIN>                     LedType;
typedef AskSin<LedType, BAT_SENSOR, RadioType> BaseHal;

class Hal : public BaseHal {
  public:
    void init(const HMID& id)
    {
      BaseHal::init(id);
#ifdef USE_CC1101_ALT_FREQ_86835
      radio.initReg(CC1101_FREQ2, 0x21);
      radio.initReg(CC1101_FREQ1, 0x65);
      radio.initReg(CC1101_FREQ0, 0x4A);
      //0x2165FA 868.357 MHz
      //0x2165F2 868.354 MHz
      //0x21654A 868.287 MHz
#endif
#ifdef CLOCK_RTC
      rtc.init();    // init real time clock - 1 tick per second
#endif
      // measure battery every 12h
      battery.init(seconds2ticks(12UL * 60 * 60), CLOCK);
      battery.low(BAT_VOLT_LOW);
      battery.critical(BAT_VOLT_CRITICAL);
    }

    bool runready() {
      return CLOCK.runready() || BaseHal::runready();
    }
} hal;

class WeatherEventMsg : public Message {
  public:
#ifdef CALCULATE_MOVING_AVERAGES
    void init(uint8_t msgcnt, int16_t temp, uint16_t my_humidity10, uint16_t operatingVoltage1000, bool batLow, int16_t MA_24H_temp100)
#else
    void init(uint8_t msgcnt, int16_t temp, uint16_t my_humidity10, uint16_t operatingVoltage1000, bool batLow)
#endif
    {

      uint8_t t1 = (temp >> 8) & 0x7f;
      uint8_t t2 = temp & 0xff;
      if (batLow == true) {
        t1 |= 0x80;    // set bat low bit
      }

      uint8_t flags = BCAST;
      if ((msgcnt % 40) == 2) {
        flags = BIDI | WKMEUP;
      }
#ifdef CALCULATE_MOVING_AVERAGES
      Message::init(0x11, msgcnt, 0x70, flags, t1, t2); //  length 17 = 0x11 bytes (see also addon hb-ep-devices-addon/CCU_RM/src/addon/firmware/rftypes/hb-uni-sensor-TH-SHT75_MA.xml) + 2 additional Bytes for 24h moving average
#else
      Message::init(0x0F, msgcnt, 0x70, flags, t1, t2); //  length 15 = 0x0F bytes (see also addon hb-ep-devices-addon/CCU_RM/src/addon/firmware/rftypes/hb-uni-sensor-TH-SHT75.xml)
#endif
      // humidity
      pload[0] =  (my_humidity10 >> 8) & 0xff;
      pload[1] =  (my_humidity10 >> 0) & 0xff;

      //operatingVoltage1000
      pload[2] = (operatingVoltage1000 >> 8) & 0xff;
      pload[3] = (operatingVoltage1000 >> 0) & 0xff;

#ifdef CALCULATE_MOVING_AVERAGES
      //MA_24H_temp100
      pload[4] =  (MA_24H_temp100 >> 8) & 0xff;
      pload[5] =  (MA_24H_temp100 >> 0) & 0xff;
#endif
    }
};


DEFREGISTER(Reg0, MASTERID_REGS, DREG_LEDMODE, DREG_LOWBATLIMIT, DREG_TRANSMITTRYMAX, 0x20, 0x21, 0x24)
class SensorList0 : public RegList0<Reg0> {
  public:
    SensorList0(uint16_t addr)
      : RegList0<Reg0>(addr)
    {
    }

    bool     updIntervall(uint16_t value) const {
      return this->writeRegister(0x20, (value >> 8) & 0xff) && this->writeRegister(0x21, value & 0xff);
    }
    uint16_t updIntervall() const {
      return (this->readRegister(0x20, 0) << 8) + this->readRegister(0x21, 0);
    }

    bool     txPower(uint8_t v) const {
      return this->writeRegister(0x24, v);
    }
    uint8_t  txPower() const {
      return this->readRegister(0x24, 0);
    }


    void defaults() // for factory reset (press config button for > 6 seconds)
    {
      clear();
      ledMode(1);
      lowBatLimit(BAT_VOLT_LOW);
      transmitDevTryMax(6);
      txPower(7);               // maximum tx power as initial value: +10dB
      updIntervall(240);        // default measurement interval after factory reset is 4 minutes
    }
};

DEFREGISTER(UReg1, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08)
class SensorList1 : public RegList1<UReg1> {
  public:
    SensorList1 (uint16_t addr) : RegList1<UReg1>(addr) {}

    bool tempOffset10 (int32_t value) const {  //32 Bits are mndatory: https://homematic-forum.de/forum/viewtopic.php?f=76&t=62578&sid=70470ba9692246570b42a7e083402adf
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

    void defaults () {
      clear();
      tempOffset10(0);    // temperature measurement offset, multiplied by 10 [K], must be positive, will be subtracted, calibrate your sensor's characteristics, enter in WebUI as device parameter for dynamic adjustment
      humidOffset10(0);   // humidity measurement offset,    multiplied by 10 [%], calibrate your sensor's characteristics, enter in WebUI as device parameter for dynamic adjustment
    }

};

class WeatherChannel : public Channel<Hal, SensorList1, EmptyList, List4, PEERS_PER_CHANNEL, SensorList0>, public Alarm {

    WeatherEventMsg msg;

    int16_t   temperature10;
    uint16_t  humidity10;
    uint16_t  operatingVoltage1000;
    bool      regularWakeUp;
#ifdef CALCULATE_MOVING_AVERAGES
    int16_t   MA_24H_temp100;
#endif


#ifdef SENSOR_SHT75
    Sens_SHT75<SHT75_DATAPIN, SHT75_CLKPIN> sht75;
#endif

  public:
    WeatherChannel()
      : Channel()
      , Alarm(seconds2ticks(60))
      , temperature10(0)
      , humidity10(0)
      , operatingVoltage1000(0)
      , regularWakeUp(true)
#ifdef CALCULATE_MOVING_AVERAGES
      , MA_24H_temp100(0)
#endif
    {
    }
    virtual ~WeatherChannel() {}

    virtual void trigger(AlarmClock& clock)
    {
      measure();

      device().battery().update();                            // get current battery voltage; measure every sampling cycle
      operatingVoltage1000 = device().battery().current();    // BatteryTM class, mV resolution
      DPRINT(F("battery voltage x1000 = "));       DDECLN(operatingVoltage1000);

      uint8_t msgcnt = device().nextcount();
#ifdef CALCULATE_MOVING_AVERAGES
      msg.init(msgcnt, temperature10, humidity10, operatingVoltage1000, device().battery().low(), MA_24H_temp100);
#else
      msg.init(msgcnt, temperature10, humidity10, operatingVoltage1000, device().battery().low());
#endif
      if (msg.flags() & Message::BCAST) {
        device().broadcastEvent(msg, *this);
      } else {
        device().sendPeerEvent(msg, *this);
      }
      // reactivate for next measure
      uint16_t updCycle = this->device().getList0().updIntervall();
      if ( updCycle < 240 ) {
        set(seconds2ticks(240 * SYSCLOCK_FACTOR ));
      }
      else {
        set((seconds2ticks(updCycle) * SYSCLOCK_FACTOR ));
      }
      CLOCK.add(*this);

      regularWakeUp = true;
    }

    void measure()
    {
      sht75.measure();
      temperature10 = sht75.temperature();
      humidity10    = sht75.humidity();
#ifdef CALCULATE_MOVING_AVERAGES
      MA_24H_temp100        = sht75.moving_average_24h_temperature100();
      DPRINT(F("MA T (x100) = "));
      DDEC(MA_24H_temp100); DPRINTLN(F(" "));
#endif
    }

    void initSensors()
    {
      sht75.init((int16_t)this->getList1().tempOffset10(), (int16_t)this->getList1().humidOffset10());

      DPRINTLN(F("Sensor setup done"));
      DPRINT(F("Serial: "));
      DPRINTLN(cDEVICE_SERIAL);
#ifdef CLOCK_SYSCLOCK
      DPRINTLN(F("Clock is SYSCLOCK"));
      DPRINT(F("SYSCLOCK correction factor                                                               : "));
      DDECLN(SYSCLOCK_FACTOR);
#elif defined CLOCK_RTC
      DPRINTLN(F("Clock is RTC"));
#endif
    }

    void setup(Device<Hal, SensorList0>* dev, uint8_t number, uint16_t addr)
    {
      Channel::setup(dev, number, addr);
      initSensors();
      set(seconds2ticks(5));    // first message in 5 sec.
      CLOCK.add(*this);
    }

    void configChanged() {
      DPRINTLN(F("* Config Changed                                 : List1"));
      DPRINT(F("* Temperature Offset x10                         : ")); DDECLN(this->getList1().tempOffset10());
      DPRINT(F("* Humidity Offset x10                            : ")); DDECLN(this->getList1().humidOffset10());
      sht75.init((int16_t)this->getList1().tempOffset10(), (int16_t)this->getList1().humidOffset10());
    }

    uint8_t status() const {
      return 0;
    }

    uint8_t flags() const {
      return 0;
    }
};

class SensChannelDevice : public MultiChannelDevice<Hal, WeatherChannel, 1, SensorList0> {
  public:
    typedef MultiChannelDevice<Hal, WeatherChannel, 1, SensorList0> TSDevice;
    SensChannelDevice(const DeviceInfo& info, uint16_t addr)
      : TSDevice(info, addr)
    {
    }
    virtual ~SensChannelDevice() {}

    virtual void configChanged()
    {
      TSDevice::configChanged();
      DPRINTLN(F("Config Changed: List0"));
      
      DPRINT(F("ledMode: "));
      DDECLN(this->getList0().ledMode());

      DPRINT(F("lowBatLimit: "));
      DDECLN(this->getList0().lowBatLimit());
      battery().low(this->getList0().lowBatLimit());

      DPRINT(F("transmitDevTryMax: "));
      DDECLN(this->getList0().transmitDevTryMax());

      DPRINT(F("updCycle: "));
      DDECLN(this->getList0().updIntervall());

      // setting of the Tx transmission power of the CC1101

      // setting of CC1101's PATABLE Tx output power settings according tables 37 (wire-wound inductors, e.g. EByte E07-868MS10)  and 39 (multi-layer inductors, e.g. no name green module) of TI CC1101 data sheet https://www.ti.com/lit/ds/symlink/cc1101.pdf
#ifdef EByte_E07_868MS10
      const byte powerMode[8] PROGMEM = { 0x03, 0x17, 0x1D, 0x26, 0x50, 0x81, 0xCD, 0xC0 };  // wire-wound inductor Tx output levels (e.g. red color PCB EByte E07-868MS10), see table 37 of CC1101 data sheet
      // 0xC0 allows +11/dBm+12dBm Tx output power, change 0xC0 to 0xC5 for max. 10 dBm Tx output power
#else
      const byte powerMode[8] PROGMEM = { 0x03, 0x0F, 0x1E, 0x27, 0x50, 0x81, 0xCB, 0xC2 };  // multi-layer inductor Tx output levels (e.g. no name green color PCB CC1101 module), see table 39 of CC1101 data sheet
#endif
      uint8_t txPower;
      if ( this->getList0().txPower() > 7 ) {
        txPower = 7;
      }
      else {
        txPower = this->getList0().txPower();
      }
      radio().initReg(CC1101_PATABLE, powerMode[txPower]);
      DPRINT(F("txPower: ")); DDECLN(txPower);
    }
};

SensChannelDevice               sdev(devinfo, 0x20);
ConfigButton<SensChannelDevice> cfgBtn(sdev);

void setup()
{
  DINIT(57600, ASKSIN_PLUS_PLUS_IDENTIFIER);
  sdev.init(hal);
  buttonISR(cfgBtn, CONFIG_BUTTON_PIN);
  sdev.initDone();
}

void loop()
{
  bool worked = hal.runready();
  bool poll   = sdev.pollRadio();
  if (worked == false && poll == false) {
    // deep discharge protection
    // if we drop below critical battery level - switch off all and sleep forever
    if (hal.battery.critical()) {
      // this call will never return
      hal.activity.sleepForever(hal);
    }
    // if nothing to do - go sleep
    hal.activity.savePower<SAVEPWR_MODE>(hal);
    //hal.activity.savePower<Idle<>>(hal);
  }
}
