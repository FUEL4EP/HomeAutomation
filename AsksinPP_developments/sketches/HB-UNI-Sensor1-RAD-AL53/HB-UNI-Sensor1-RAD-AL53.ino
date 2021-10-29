//---------------------------------------------------------
// HB-UNI-Sensor1-RAD-AL53
// Radiation sensor for alpha, beta, and gamma radiation based on Teviso AL53 PIN diode sensor https://www.teviso.com/file/pdf/al53-data-specification.pdf
// Version 1.0
// (C) 2021 FUEL4EP (Creative Commons)
// HB-UNI-Sensor1 (C) 2018-2021 Tom Major (Creative Commons)
// https://creativecommons.org/licenses/by-nc-sa/4.0/
// You are free to Share & Adapt under the following terms:
// Give Credit, NonCommercial, ShareAlike
// +++
// AskSin++ 2016-10-31 papa Creative Commons
//---------------------------------------------------------

//------------------------------------------------------------------------------------------------------------------------
//
// this code supports only an Atmega1284P MCU, an Atmega328P MCU is NOT supported for memory size reasons !!!!!!!!!!!!!!!!
//
//------------------------------------------------------------------------------------------------------------------------


// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//
// !! NDEBUG muss UNBEDINGT aktiviert werden wenn die Sensorentwicklung und die Tests abgeschlossen sind und das Gerät in den 'Produktionsmodus' geht !!!
//
//
//#define NDEBUG
//#define USE_CC1101_ALT_FREQ_86835  // use alternative frequency to compensate not correct working cc1101 modules

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

// the sensor HB-UNI-Sensor1-RAD-AL53 requires mandatorily an ATMega1284P MCU (Pro Mini XL - v2 - ATmega 1284p https://www.tindie.com/products/prominimicros/pro-mini-xl-v2-atmega-1284p/)

// please set the baud rate in the serial monitor as 38400 baud 

#define SENSOR_ONLY

#define EI_NOTEXTERNAL
#define M1284P
#include <EnableInterrupt.h>
#include <AskSinPP.h>
#include <LowPower.h>
#include <Register.h>
#include <MultiChannelDevice.h>
#include "Sensors/tmBattery.h"
#include "Sensors/Sens_AL53.h"
 

//---------------------------------------------------------
// Alle wesentlichen Device Parameter werden aus einer Sens_AL53.h Datei  geholt um mehrere Geräte ohne weitere Änderungen des
// Sketches flashen zu können. Für mehrere Geräte einfach mehrere .h Dateien anlegen und dort die Unterschiede zwischen den Geräten definieren. Die
// konfigurierbaren Device Parameter in der Sens_AL53.h Datei sind im Einzelnen:
// - Device ID und Device Serial
// - Pin Definitionen Allgemein
// - Clock Definition
// - Schaltungsvariante und Pins für Batteriespannungsmessung
// - Schwellwerte für Batteriespannungsmessung (bitte nicht ändern, sonst droht eine Tiefentladung der Akkumulatoren
#include "Cfg/Device_AL53.h"

// number of available peers per channel
#define PEERS_PER_CHANNEL 6               // see also https://homematic-forum.de/forum/viewtopic.php?f=76&t=68499&sid=801b5e55eb90f287a53c512cace14235
                                          // currently no direct links are supported
#define SAMPLINGINTERVALL_IN_SECONDS 600  // read the radiation event counter every 10 minutes = 600 seconds

// #define EByte_E07_868MS10   // uncomment if an EByte E07-868MS10 module (red color) is used as CC1101 transceiver

// please install the addon hb-ep-devices-addon version >= 1.10 before pairing the sensor with the CCU/RaspberryMatic


// all library classes are placed in the namespace 'as'
using namespace as;

//correction factor of the clock inaccuracy (ceramic resonator is default on Arduino boards), if no quartz RTC is used, wee also HB-UNI-Sen-CURRENT from Jérôme ( jp112sdl )
#define SYSCLOCK_FACTOR    0.89    // adjust to get sampling data every 600 seconds

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

const struct DeviceInfo PROGMEM devinfo = {
    cDEVICE_ID,        // Device ID                 defined in ./Cfg/Device_AL53.h
    cDEVICE_SERIAL,    // Device Serial             defined in ./Cfg/Device_AL53.h
    { 0xf6, 0x08 },    // Device Model              needs to fit to Addon XML hb-uni-sensor-RAD-AL53.xml line 6:
                       //                           <parameter index="10.0" size="2.0" const_value="0xF608" /
                       // Firmware Version
    0x10,
    0x53,              // Device type, see also HB-UNI-Sen-DUST.ino
    { 0x01, 0x01 }     // Info Bytes
};

// Configure the used hardware

#if defined M1284P
typedef AvrSPI<CC1101_CS_PIN, CC1101_MOSI_PIN, CC1101_MISO_PIN, CC1101_SCK_PIN> SPIType;
typedef Radio<SPIType, CC1101_GDO0_PIN> RadioType;
#else
typedef AvrSPI<10, 11, 12, 13> SPIType;
typedef Radio<SPIType, 2> RadioType;
#endif

#if defined(LED_PIN1) && defined(LED_PIN2)
typedef DualStatusLed<LED_PIN1, LED_PIN2> LedType;
#else
typedef StatusLed<LED_PIN> LedType;
#endif


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
#ifdef CLOCK_RTC
     rtc.init();    // init real time clock - 1 tick per second
#endif
      // measure battery every 1h
      battery.init(seconds2ticks(1UL * 60 * 60), CLOCK);
      battery.low(BAT_VOLT_LOW);
      battery.critical(BAT_VOLT_CRITICAL);

    }

    bool runready () {
      return CLOCK.runready() || BaseHal::runready();
    }
} hal;

#ifdef SENSOR_AL53                               // defined in ./Cfg/Device_AL53.h
    Sens_AL53 al53;
#endif

class MeasureEventMsg : public Message {
  public:
    void init(uint8_t msgcnt, bool alarm_count_per_measurement_interval, bool alarm_moving_average, uint16_t counts_per_measurement_interval, uint16_t moving_average_count, uint16_t width_confidence_interval, uint16_t moving_average_count_max, uint16_t counts_per_measurement_interval_max, uint16_t accumulator_voltage, uint16_t AL53_supply_voltage, uint16_t operating_voltage, bool batlow) {
        
      uint8_t sb1, sb2;   // sb1:  status bits, sb2 is used as lower byte of counts_per_measurement_interval
      
      sb1 = 0x00;
      
      if ( alarm_count_per_measurement_interval == true ) {
        sb1 |= 0x01; // set alarm_count_per_measurement_interval bit
      }
      
      if ( alarm_moving_average == true ) {
        sb1 |= 0x02; // set alarm_moving_average bit
      }
      
      if ( batlow == true ) {
        sb1 |= 0x80; // set bat low bit
      }
      
      sb2 = (counts_per_measurement_interval >> 8) & 0xff;
      
      Message::init(0x1A, msgcnt, 0x53, (msgcnt % 18 == 2) ? (BIDI | WKMEUP) : BCAST, sb1, sb2);  // request every 3 hours = 18 * 10 minutes a BIDI message
      

      pload[0] = counts_per_measurement_interval & 0xff;
      
      pload[1] = (moving_average_count >> 8) & 0xff ;
      pload[2] = moving_average_count & 0xff;

      pload[3] = (width_confidence_interval >> 8) & 0xff;
      pload[4] = width_confidence_interval & 0xff;
      
      pload[5] = (moving_average_count_max >> 8) & 0xff ;
      pload[6] = moving_average_count_max & 0xff;

      pload[7] = (counts_per_measurement_interval_max >> 8) & 0xff;
      pload[8] = counts_per_measurement_interval_max & 0xff;
      
      pload[9] = (accumulator_voltage >> 8) & 0xff ;
      pload[10] = accumulator_voltage & 0xff;
      
      pload[11] = (AL53_supply_voltage >> 8) & 0xff ;
      pload[12] = AL53_supply_voltage & 0xff;
      
      pload[13] = (operating_voltage >> 8) & 0xff ;
      pload[14] = operating_voltage & 0xff;
      
      // max allowed pload index is 14

    }
};




DEFREGISTER(Reg0, MASTERID_REGS, DREG_LEDMODE, DREG_LOWBATLIMIT, DREG_TRANSMITTRYMAX, 0x20, 0x21, 0x24)
class SensorList0 : public RegList0<Reg0> {
public:
    SensorList0(uint16_t addr)
        : RegList0<Reg0>(addr)
    {
    }

    bool     updInterval(uint16_t value) const { return this->writeRegister(0x20, (value >> 8) & 0xff) && this->writeRegister(0x21, value & 0xff); }
    uint16_t updInterval() const { return (this->readRegister(0x20, 0) << 8) + this->readRegister(0x21, 0); }
    
    bool    txPower(uint8_t v) const { return this->writeRegister(0x24,v); }
    uint8_t txPower() const { return this->readRegister(0x24,0); }


    void defaults()
    {
        clear();
        ledMode(1);
        lowBatLimit(BAT_VOLT_LOW);
        transmitDevTryMax(6);
        txPower(7);                                       // maximum tx power as initial value: +10dB
        updInterval(SAMPLINGINTERVALL_IN_SECONDS);        // sampling intervall 600 seconds          ; adjust to your needs in WebUI device / device/channel parameter page
        powerSupply(true);                                // true = battery mode
    }
};

// alarm levels (not yet used, only prepared here)
DEFREGISTER(UReg1, 0x01, 0x02, 0x03, 0x04)
class SensorList1 : public RegList1<UReg1> {
  public:
    SensorList1 (uint16_t addr) : RegList1<UReg1>(addr) {}
    
    bool alarm_level_counts_per_measurement_interval (uint16_t value) const {
      return
          this->writeRegister(0x01, (value >> 8) & 0xff) &&
          this->writeRegister(0x02, (value) & 0xff)
          ;
    }

    uint16_t alarm_level_counts_per_measurement_interval () const {
      return
          ((uint16_t)(this->readRegister(0x01, 0)) << 8) +
          ((uint16_t)(this->readRegister(0x02, 0)))
          ;
    }

    bool alarm_level_moving_average (uint16_t value) const {
      return
          this->writeRegister(0x03, (value >> 8) & 0xff) &&
          this->writeRegister(0x04, (value) & 0xff)
          ;
    }

    uint16_t alarm_level_moving_average () const {
      return
          ((uint16_t)(this->readRegister(0x03, 0)) << 8) +
          ((uint16_t)(this->readRegister(0x04, 0)))
          ;
    }

    void defaults () {
      clear();
      alarm_level_counts_per_measurement_interval(65535);     // set default to max
      alarm_level_moving_average(65535);                      // set default to max    
    }
};



class MeasureChannel : public Channel<Hal, SensorList1, EmptyList, List4, PEERS_PER_CHANNEL, SensorList0>, public Alarm {
    MeasureEventMsg msg;
    bool            alarm_count_per_measurement_interval;   // alarm bit 
    bool            alarm_moving_average;                   // alarm bit
    uint16_t        counts_per_measurement_interval;
    uint16_t        moving_average_count;
    uint16_t        width_confidence_interval;
    uint16_t        moving_average_count_max;
    uint16_t        counts_per_measurement_interval_max;
    uint16_t        accumulator_voltage;
    uint16_t        AL53_supply_voltage;
    uint16_t        operating_voltage;

  public:
    MeasureChannel () : Channel(), Alarm(seconds2ticks(60)), alarm_count_per_measurement_interval(false), alarm_moving_average(false), counts_per_measurement_interval(0), moving_average_count(0), width_confidence_interval(0), moving_average_count_max(0), counts_per_measurement_interval_max(0), accumulator_voltage(0), AL53_supply_voltage(0), operating_voltage(0)  {}
    virtual ~MeasureChannel () {}

    void measure() {
        
      DPRINTLN(F("\n\nreadout of the last radiation measurement interval's results ... "));

      al53.measure();
      
      alarm_count_per_measurement_interval = al53.alarm_count_per_measurement_interval();
      alarm_moving_average                 = al53.alarm_moving_average();
      counts_per_measurement_interval      = al53.counts_per_measurement_interval();
      moving_average_count                 = al53.moving_average_count();
      width_confidence_interval            = al53.width_confidence_interval();
      moving_average_count_max             = al53.moving_average_count_max();
      counts_per_measurement_interval_max  = al53.counts_per_measurement_interval_max();
      accumulator_voltage                  = al53.accumulator_voltage();
      AL53_supply_voltage                  = al53.AL53_supply_voltage();
      operating_voltage                    = al53.operating_voltage();
      
      DPRINT(F("accumulator voltage(MCU ADC) (x1000.0)                                                   : "));
      device().battery().update();
      DDECLN(device().battery().current());    // BatteryTM class, mV resolution
    }  

    virtual void trigger (__attribute__ ((unused)) AlarmClock& clock) {
      

      uint8_t msgcnt = device().nextcount();
      measure();
          
      msg.init(msgcnt, alarm_count_per_measurement_interval, alarm_moving_average, counts_per_measurement_interval, moving_average_count, width_confidence_interval, moving_average_count_max, counts_per_measurement_interval_max, accumulator_voltage, AL53_supply_voltage, operating_voltage,  device().battery().low() );
      
      if (msg.flags() & Message::BCAST) {
            device().broadcastEvent(msg, *this);
        } else {
            device().sendPeerEvent(msg, *this);
      }
      
      // reactivate for next measure
      uint16_t updCycle = this->device().getList0().updInterval();
      set((seconds2ticks(updCycle) * SYSCLOCK_FACTOR));
      clock.add(*this);
    }
    
    void initSensor()
    {

#ifdef SENSOR_AL53
        al53.init((int16_t)this->getList1().alarm_level_counts_per_measurement_interval(), (int16_t)this->getList1().alarm_level_moving_average());  // set threshold levels for alarm signals as parameters
#endif

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
    
    void setup(Device<Hal, SensorList0>* dev, uint8_t number, uint16_t addr) {
      Channel::setup(dev, number, addr);
      initSensor();
      set(seconds2ticks(5));    // first message in 5 sec.
      CLOCK.add(*this);
    }

    void configChanged() {
      DPRINT(F("alarm level counts per measurement interval : ")); DDECLN(this->getList1().alarm_level_counts_per_measurement_interval());
      DPRINT(F("alarm level moving average                  : ")); DDECLN(this->getList1().alarm_level_moving_average());
      al53.init((int16_t)this->getList1().alarm_level_counts_per_measurement_interval(), (int16_t)this->getList1().alarm_level_moving_average()); // set threshold levels for alarm signals as parameters
    }
    
    uint8_t status() const { return 0; }

    uint8_t flags() const { return 0; }
};





class RadChannelDevice : public MultiChannelDevice<Hal, MeasureChannel, 1, SensorList0> {
public:
    typedef MultiChannelDevice<Hal, MeasureChannel, 1, SensorList0> TSDevice;
    RadChannelDevice(const DeviceInfo& info, uint16_t addr)
        : TSDevice(info, addr)
    {
    }
    virtual ~RadChannelDevice() {}

    virtual void configChanged()
    {
      TSDevice::configChanged();
      DPRINTLN(F("Config Changed: List0"));

      uint8_t ledMode = this->getList0().ledMode();
      DPRINT(F("ledMode: "));
      DDECLN(ledMode);

      uint8_t lowBatLimit = this->getList0().lowBatLimit();
      DPRINT(F("lowBatLimit: "));
      DDECLN(lowBatLimit);
      battery().low(lowBatLimit);

      uint8_t txDevTryMax = this->getList0().transmitDevTryMax();
      DPRINT(F("transmitDevTryMax: "));
      DDECLN(txDevTryMax);

      uint16_t updCycle = this->getList0().updInterval();
      DPRINT(F("updCycle: "));
      DDECLN(updCycle);

      // Einstellung der CC1101 Tx Sendeleistung

      // setting of CC1101's PATABLE Tx output power settings according tables 37 (wire-wound inductors, e.g. EByte E07-868MS10)  and 39 (multi-layer inductors, e.g. no name green module) of TI CC1101 data sheet https://www.ti.com/lit/ds/symlink/cc1101.pdf
#ifdef EByte_E07_868MS10
      const byte powerMode[8] = { 0x03, 0x17, 0x1D, 0x26, 0x50, 0x81, 0xCD, 0xC0 };  // wire-wound inductor Tx output levels (e.g. red color PCB EByte E07-868MS10), see table 37 of CC1101 data sheet
                                                                                      // 0xC0 allows +11/dBm+12dBm Tx output power, change 0xC0 to 0xC5 for max. 10 dBm Tx output power
#else
      const byte powerMode[8] = { 0x03, 0x0F, 0x1E, 0x27, 0x50, 0x81, 0xCB, 0xC2 };  // multi-layer inductor Tx output levels (e.g. no name green color PCB CC1101 module), see table 39 of CC1101 data sheet
#endif
      uint8_t txPower = this->getList0().txPower();
      // the used libary ArduinoSTL seems to disable the min function !? Therefore here a standard if is used instead
      if ( txPower > 7 ) {
        txPower = 7;
      }
      radio().initReg(CC1101_PATABLE, powerMode[txPower]);
      DPRINT(F("txPower: "));DDECLN(txPower);
      
    }
};

RadChannelDevice               sdev(devinfo, 0x20);
ConfigButton<RadChannelDevice> cfgBtn(sdev);


void setup()
{
    DINIT(38400, ASKSIN_PLUS_PLUS_IDENTIFIER);   // The baud rate of 38400 baud is required for the ATMega1284P 
    sdev.init(hal);
    buttonISR(cfgBtn, CONFIG_BUTTON_PIN);
    sdev.initDone();
}

void loop()
{
    bool worked = hal.runready();
    bool poll   = sdev.pollRadio();
    if (worked == false && poll == false) {
#ifdef NDEBUG
        // deep discharge protection // only active when Debugging is switched !! Otherwise the deep discharge protection would get active if an accumulator battery is removed
        //
        // IMPORTANT: During Debugging or programming at least one accumulator battery needs to be removed in order to avoid a short current loop:
        // accumulator => boost converter => external supply by the ISP programmer or FTDI debugger
        // This would cause a deep discharge of the accumulator batteries!!!
        //
        // if we drop below critical battery level - switch off all and sleep forever
        if (hal.battery.critical()) {
          // this call will never return
          hal.activity.sleepForever(hal);
        }
#endif
        // if nothing to do - go sleep
        hal.activity.savePower<SAVEPWR_MODE>(hal);
        //hal.activity.savePower<Idle<>>(hal);
    }
}
