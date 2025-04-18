//---------------------------------------------------------
// HB-UNI-Sensor1-THPD-SHT45_BME280    high-accuracy (accuracies ΔRH = ±1.0 %RH, ΔT = ±0.1 °C) Sensirion SHT45 (Temperature, Humidity, Dewpoint) / Bosch BME280 (Pressure) Temperature, Humidity, Pressure, Dewpoint, Vapor Concentration Sensor
//                              with WebUI offset setting and 24h and 7 days moving average filters for the measured temperature
//                              fitting sketch for PCBs HB_Stamp_ATMega1284P_FUEL4EP with I2C sensor adapter PCB I2C_Module_SHT45_BME280_small_FUEL4EP
//                              or PCB HB-UNI-SEN-BATT_ATMega1284P_E07-900MM10S_FUEL4EP with I2C sensor adapter I2C_Module_SHT45_BME280_FUEL4EP
// Version 1.0
// (C) 2018-2020 Tom Major      (Creative Commons)
// (C) 2020-2022 FUEL4EP        (Creative Commons)          added SHT45 and adaptions to Bosch BME280 sensor /  offset setting by WebUI / dewpoint and vapor concentration
// https://creativecommons.org/licenses/by-nc-sa/4.0/
// You are free to Share & Adapt under the following terms:
// Give Credit, NonCommercial, ShareAlike
// +++
// AskSin++                     2016 papa (Creative Commons)
// HB-UNI-Sensor1               2018 TomMajor (Creative Commons)
// Sensirion_I2C_SHT4x          2023 Sensirion https://github.com/Sensirion/arduino-i2c-sht4x  (BSD 3-Clause "New" or "Revised" License)
// finitespace/BME280           2016 finitespace https://github.com/finitespace/BME280  (GNU General Public License v3.0) for BME280 sensor
// arduino-sht                  2015 Sensirion https://github.com/Sensirion/arduino-sht  (BSD 3-Clause "New" or "Revised" License) for SHT45 sensor

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//please install the addon hb-ep-devices-addon version >= 1.16 before pairing the sensor with the CCU/RaspberryMatic
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------

#define SENSOR_ONLY
#define NORTC

//---------------------------------------------------------
// #define NDEBUG  // disable all serial debug messages; comment if you want to get debug messages in the serial monitor, uncomment if sketch has been proven to be fully functional
// #define USE_CC1101_ALT_FREQ_86835  //use alternative frequency to compensate not correct working cc1101 modules

//---------------------------------------------------------
// define this to read the device id, serial and device type from bootloader section
// #define USE_OTA_BOOTLOADER

// the sensor HB-UNI-Sensor1-THPD-SHT45_BME280 requires mandatorily an ATMega1284P MCU
// this sketch is best fitting to PCBs HB_Stamp_ATMega1284P_FUEL4EP with I2C sensor adapter PCB I2C_Module_SHT45_BME280_small_FUEL4EP
//                             or PCB HB-UNI-SEN-BATT_ATMega1284P_E07-900MM10S_FUEL4EP with I2C sensor adapter I2C_Module_SHT45_BME280_FUEL4EP
// more suitable PCBs are available at  https://github.com/FUEL4EP/HomeAutomation/tree/master/AsksinPP_developments/PCBs

// please set the baud rate in the serial monitor as 38400 baud

// comment '#define  M1284P' for ATMega1284P MCU, note; ATMega328P is NOT supported since RAM size is too small
#define  M1284P

//#define DEEP_DISCHARGE_PROTECTION_ENABLED    // IMPORTANT: Comment during FTDI debugging otherwise the sensor will sleep forever if supplied by FTDI debugger w/o an inserted battery
                                               // IMPORTANT: Uncomment only for operational mode w/o FTDI debugger!!!

#define EI_NOTEXTERNAL
#include <EnableInterrupt.h>
#include <AskSinPP.h>
#include <LowPower.h>
#include <MultiChannelDevice.h>
#include <Register.h>
#include "Sensors/tmBattery.h"

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
#define EByte_E07_900MM10S     // uncomment if an EByte E07-900MM10S module (10 mm * 10 mm square) is used as CC1101 transceiver
//#define EByte_E07-900M10S    // uncomment if an EByte E07-900M10S module (14 mm * 20 mm) is used as CC1101 transceiver

#include "Cfg/Device_SHT45_BME280.h"

// number of available peers per channel
#define PEERS_PER_CHANNEL 6

// all library classes are placed in the namespace 'as'
using namespace as;

//correction factor of the clock inaccuracy (ceramic resonator is default on Arduino boards), if no quartz RTC is used, see also HB-UNI-Sen-CURRENT from Jérôme ( jp112sdl )
#define SYSCLOCK_FACTOR    0.91762   // adjust to get sampling data every 240 seconds

// both a Sensirion SHT45 as well as a Bosch BME280 are required

#ifdef SENSORS_SHT45_BME280
#include "Sensors/Sens_SHT45_BME280.h"    // HB-UNI-Sensor1 custom sensor class
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


const struct DeviceInfo PROGMEM devinfo = {
    cDEVICE_ID,        // Device ID           defined in ./Cfg/Device_SHT45_BME280.h
    cDEVICE_SERIAL,    // Device Serial       defined in ./Cfg/Device_SHT45_BME280.h

    { 0xF6, 0x12 },    // Device Model        needs to fit to Addon XML hb-uni-sensor-THPD-SHT45_BME280.xml line 6:
                       //                     <parameter index="10.0" size="2.0" const_value="0xF612"
    0x10,              // Firmware version V1.0
    as::DeviceType::THSensor,    // Device Type
    { 0x01, 0x01 }               // Info Bytes
};

// Configure the used hardware

#if defined M1284P
typedef AvrSPI<CC1101_CS_PIN, CC1101_MOSI_PIN, CC1101_MISO_PIN, CC1101_SCK_PIN> SPIType;
typedef Radio<SPIType, CC1101_GDO0_PIN> RadioType;
#else
typedef AvrSPI<10, 11, 12, 13> SPIType;
typedef Radio<SPIType, 2> RadioType;
#endif

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

    bool runready() { return CLOCK.runready() || BaseHal::runready(); }
} hal;

class WeatherEventMsg : public Message {
public:
    void init(uint8_t msgcnt, int16_t temp, uint16_t my_humidity10, uint16_t airPressure10, int16_t dewpoint10, uint16_t vaporConcentration100, uint16_t batteryVoltage1000, bool batLow, int16_t MA_24H_temp100, int16_t MA_7D_temp100)

    {

        uint8_t t1 = (temp >> 8) & 0x7f;
        uint8_t t2 = temp & 0xff;
        if (batLow == true) {
            t1 |= 0x80;    // set bat low bit
        }

        DPRINT(F("msg T/H/D/P/V/MA_24h/MA_7D (x10, ah x100, MA x100) = "));
        DDEC(temp);DPRINT("/");
        DDEC(my_humidity10);DPRINT(F("/"));
        DDEC(airPressure10);DPRINT(F("/"));
        DDEC(dewpoint10);DPRINT(F("/"));
        DDEC(vaporConcentration100);DPRINT(F("/"));
        DDEC(batteryVoltage1000);DPRINT(F("/"));
        DDEC(MA_24H_temp100);DPRINT(F("/"));
        DDEC(MA_7D_temp100);DPRINTLN(F(" "));

        uint8_t flags = BCAST;
        if ((msgcnt % 40) == 2) {
            flags = BIDI | WKMEUP;
        }
        DPRINT(F("msgcnt    : ")); DDECLN(msgcnt);
        

        Message::init(0x19, msgcnt, 0x70, flags, t1, t2); //  length 25 = 0x19 bytes (see also addon hb-ep-devices-addon/CCU_RM/src/addon/firmware/rftypes/hb-uni-sensor-THPD-SHT45_BME280.xml); + 4 additional Bytes for moving averages

        // humidity
        pload[0] =  (my_humidity10 >> 8) & 0xff;
        pload[1] =  (my_humidity10 >> 0) & 0xff;
        
        // pressure
        pload[2] =  (airPressure10 >> 8) & 0xff;
        pload[3] =  (airPressure10 >> 0) & 0xff;

        // dewpoint
        pload[4] =  (dewpoint10 >> 8) & 0xff;
        pload[5] =  (dewpoint10 >> 0) & 0xff;
        
        // vapor concentration
        pload[6] =  (vaporConcentration100 >> 8) & 0xff;
        pload[7] =  (vaporConcentration100 >> 0) & 0xff;

        //batteryVoltage1000
        pload[8] =  (batteryVoltage1000 >> 8) & 0xff;
        pload[9] =  (batteryVoltage1000 >> 0) & 0xff;

        //MA_24H_temp100
        pload[10] =  (MA_24H_temp100 >> 8) & 0xff;
        pload[11] =  (MA_24H_temp100 >> 0) & 0xff;
        
        //MA_7D_temp100
        pload[12] =  (MA_7D_temp100 >> 8) & 0xff;
        pload[13] =  (MA_7D_temp100 >> 0) & 0xff;
    }
};


DEFREGISTER(Reg0, MASTERID_REGS, DREG_LEDMODE, DREG_LOWBATLIMIT, DREG_TRANSMITTRYMAX, 0x20, 0x21, 0x22, 0x23, 0x24)
class SensorList0 : public RegList0<Reg0> {
public:
    SensorList0(uint16_t addr)
        : RegList0<Reg0>(addr)
   {
   }

    bool     updIntervall(uint16_t value) const { return this->writeRegister(0x20, (value >> 8) & 0xff) && this->writeRegister(0x21, value & 0xff); }
    uint16_t updIntervall() const { return (this->readRegister(0x20, 0) << 8) + this->readRegister(0x21, 0); }

    bool     altitude(uint16_t value) const { return this->writeRegister(0x22, (value >> 8) & 0xff) && this->writeRegister(0x23, value & 0xff); }
    uint16_t altitude() const { return (this->readRegister(0x22, 0) << 8) + this->readRegister(0x23, 0); }
    
    bool    txPower(uint8_t v) const { return this->writeRegister(0x24,v); }
    uint8_t txPower() const { return this->readRegister(0x24,0); }


    void defaults()  // default values for factory reset (press config button for > 6 seconds)
    {
        clear();
        ledMode(1);
        lowBatLimit(BAT_VOLT_LOW);
        transmitDevTryMax(6);
        updIntervall(240);
        txPower(7);               // maximum tx power as initial value: +10dB
        altitude(84);             // height of sensor's location above sea level; adjust to your sensor's location in WebUI device / device/channel parameter page
    }
};

DEFREGISTER(UReg1, 0x01, 0x02, 0x03)
class SensorList1 : public RegList1<UReg1> {
  public:
    SensorList1 (uint16_t addr) : RegList1<UReg1>(addr) {}

    bool tempOffset10 (int8_t value) const {  
      return
          this->writeRegister(0x01, (value) & 0xff)
          ;
    }

    int8_t tempOffset10 () const {
      return
          (int8_t)(this->readRegister(0x01, 0))
          ;
    }

    bool humidOffset10 (int8_t value) const {
      return
          this->writeRegister(0x02, (value) & 0xff)
          ;
    }

    int8_t humidOffset10 () const {
      return
          (int8_t)(this->readRegister(0x02, 0))
          ;
    }
    
    bool pressOffset10 (int8_t value) const {
      return
          this->writeRegister(0x03, (value) & 0xff)
          ;
    }

      int8_t pressOffset10 () const {
      return
          (int8_t)(this->readRegister(0x03, 0))
          ;
    }
    
    void defaults () {
      clear();
      tempOffset10(0);   // temperature measurement offset, multiplied by 10 [K], must be positive, will be subtracted, calibrate your sensor's characteristics, enter in WebUI as device parameter for dynamic adjustment
      humidOffset10(0);  // humidity measurement offset, multiplied by 10 [%], calibrate your sensor's characteristics, enter in WebUI as device parameter for dynamic adjustment
      pressOffset10(0);  // pressure measurement offset, multiplied by 10 [hPa], calibrate your sensor's characteristics, enter in WebUI as device parameter for dynamic adjustment
    }
    
};

class WeatherChannel : public Channel<Hal, SensorList1, EmptyList, List4, PEERS_PER_CHANNEL, SensorList0>, public Alarm {

    WeatherEventMsg msg;

    int16_t   temperature10;
    uint16_t  humidity10;
    uint16_t  airPressure10;
    int16_t   dewpoint10;
    uint16_t  vaporConcentration100;
    uint16_t  batteryVoltage1000;
    bool      regularWakeUp;
    int16_t   MA_24H_temp100;
    int16_t   MA_7D_temp100;

#ifdef SENSORS_SHT45_BME280
    Sens_SHT45_Bme280   SHT45_BME280;
#endif

public:
    WeatherChannel()
        : Channel()
        , Alarm(seconds2ticks(60))
        , temperature10(0)
        , humidity10(0)
        , airPressure10(0)
        , dewpoint10(0)
        , vaporConcentration100(0)
        , batteryVoltage1000(0)
        , regularWakeUp(true)
        , MA_24H_temp100(0)
        , MA_7D_temp100(0)
    {
    }
    virtual ~WeatherChannel() {}

    virtual void trigger(AlarmClock& clock)
    {
        measure();
        
        device().battery().update();                          // get current battery voltage; measure every sampling cycle
        batteryVoltage1000 = device().battery().current();    // BatteryTM class, mV resolution
        DPRINT(F("battery voltage x1000 = "));       DDECLN(batteryVoltage1000);
        
        uint8_t msgcnt = device().nextcount();
        msg.init(msgcnt, temperature10, humidity10, airPressure10, dewpoint10, vaporConcentration100, batteryVoltage1000, device().battery().low(), MA_24H_temp100, MA_7D_temp100);
        if (msg.flags() & Message::BCAST) {
            device().broadcastEvent(msg, *this);
        } else {
            device().sendPeerEvent(msg, *this);
        }
        // reactivate for next measure
        uint16_t updCycle = this->device().getList0().updIntervall();
        set((seconds2ticks(updCycle) * SYSCLOCK_FACTOR));
        CLOCK.add(*this);
        
        regularWakeUp = true;
    }

    void measure()
    {


#ifdef SENSORS_SHT45_BME280

        SHT45_BME280.measure();
        temperature10         = SHT45_BME280.temperature();
        humidity10            = SHT45_BME280.humidity();
        airPressure10         = SHT45_BME280.pressureNN();
        dewpoint10            = SHT45_BME280.dewPoint();
        vaporConcentration100 = SHT45_BME280.vaporConcentration();
        
        
        DPRINT(F("T/H/P/D/V (x10, aH x100) = "));
        DDEC(temperature10);DPRINT(F("/"));
        DDEC(humidity10);DPRINT(F("/"));
        DDEC(airPressure10);DPRINT(F("/"));
        DDEC(dewpoint10);DPRINT(F("/"));
        DDEC(vaporConcentration100);DPRINTLN(F(" "));
        MA_24H_temp100        = SHT45_BME280.moving_average_24h_temperature100();
        MA_7D_temp100         = SHT45_BME280.moving_average_7D_temperature100();
        DPRINT(F("MA T (x100) = "));
        DDEC(MA_24H_temp100);DPRINT(F("/"));
        DDEC(MA_7D_temp100);DPRINTLN(F(" "));
#endif 
    }

    void initSensors()
    {

#ifdef SENSORS_SHT45_BME280
        SHT45_BME280.init((uint16_t)this->device().getList0().altitude(), (int8_t)this->getList1().tempOffset10(), (int8_t)this->getList1().humidOffset10(), (int8_t)this->getList1().pressOffset10());
#endif

        DPRINTLN(F("Sensor setup done"));
        DPRINT(F("Serial: "));
        DPRINTLN(cDEVICE_SERIAL);
#ifdef CLOCK_SYSCLOCK
        DPRINTLN(F("Clock SYSCLOCK"));
        DPRINT(F("SYSCLOCK correction factor                                                               : "));
        DDECLN(SYSCLOCK_FACTOR);
#elif defined CLOCK_RTC
        DPRINTLN(F("Clock RTC"));
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
      DPRINTLN("* Config Changed         : List1");
      DPRINT(F("* Temperature Offset x10 : ")); DDECLN(this->getList1().tempOffset10());
      DPRINT(F("* Humidity Offset x10    : ")); DDECLN(this->getList1().humidOffset10());
      DPRINT(F("* Pressure Offset x10    : ")); DDECLN(this->getList1().pressOffset10());
      SHT45_BME280.init((uint16_t)this->device().getList0().altitude(), (int8_t)this->getList1().tempOffset10(), (int8_t)this->getList1().humidOffset10(), (int8_t)this->getList1().pressOffset10());
    }

    uint8_t status() const { return 0; }

    uint8_t flags() const { return 0; }
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

        uint16_t updCycle = this->getList0().updIntervall();
        DPRINT(F("updCycle: "));
        DDECLN(updCycle);
   
        uint16_t altitude = this->getList0().altitude();
        DPRINT(F("altitude: "));
        DDECLN(altitude);
        
        // setting of the Tx transmission power of the CC1101 tranceiver module

        // setting of CC1101's PATABLE Tx output power settings according tables 37 (wire-wound inductors, e.g. EByte E07-868MS10)  and 39 (multi-layer inductors, e.g. no name green module) of TI CC1101 data sheet https://www.ti.com/lit/ds/symlink/cc1101.pdf
#if defined (EByte_E07_868MS10) || defined (EByte_E07_900MM10S) || defined (EByte_E07_900M10S)
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

SensChannelDevice               sdev(devinfo, 0x20);
ConfigButton<SensChannelDevice> cfgBtn(sdev);

void setup()
{
    DINIT(38400, ASKSIN_PLUS_PLUS_IDENTIFIER);
    sdev.init(hal);
    buttonISR(cfgBtn, CONFIG_BUTTON_PIN);
    sdev.initDone();
}

void loop()
{
    bool worked = hal.runready();
    bool poll   = sdev.pollRadio();
    if (worked == false && poll == false) {
#ifdef DEEP_DISCHARGE_PROTECTION_ENABLED
        // deep discharge protection
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
