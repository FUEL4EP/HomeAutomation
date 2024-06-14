//--------------------------------------------------------------
// HB-UNI-Sensor1-THPD_MA-AHT20_BMP280 Homebrew Homematic AsksinPP THPD sensor with moving averages stored in a ferromagnetic RAM FRAM
// Aosong AHT20 (Temperature, Humidity, Dewpoint) / Bosch BMP280 (Temperature, Pressure), temperature moving average filters of last day, week, month, and year and WebUI offset settings
// Version 1.0
// (C) 2018-2020 Tom Major (Creative Commons)
// (C) 2024 FUEL4EP        (Creative Commons)          added Aosong AHT20 BMP280 and Bosch BMP280 sensor /  moving average filters (1 day, 1 week, 1 month, and 1 year) for temperature/ offset setting by WebUI
// https://creativecommons.org/licenses/by-nc-sa/4.0/
// You are free to Share & Adapt under the following terms:
// Give Credit, NonCommercial, ShareAlike
// +++
// AskSin++                  2016 papa (Creative Commons)
// HB-UNI-Sensor1            2018 TomMajor (Creative Commons)
// Adafruit BMP280           2020 Adafruit https://github.com/adafruit/Adafruit_BMP280_Library (no license mentioned)
// finitespace/BME280        2016 finitespace https://github.com/finitespace/BME280  (GNU General Public License v3.0) for EnvironmentCalculations
// AHT20                     2020 dvarrel https://github.com/dvarrel/AHT20 (Creative Commons Zero v1.0 Universal)
// Adafruit_FRAM_SPI         2013 Adafruit https://github.com/adafruit/Adafruit_FRAM_SPI (BSD License)
// Adafruit_FRAM_SPI         2024 FUEL4EP fork of Adafruit's Adafruit_FRAM_SPI: added low power mode; https://github.com/FUEL4EP/HomeAutomation/tree/master/AsksinPP_developments/libraries/Adafruit_FRAM_SPI (BSD License)
// HB-LC-SwX-TP              2021 jp112sdl https://github.com/jp112sdl/HB-LC-SwX-TP (no license mentioned)
// Time                      2012 PaulStoffregen https://github.com/PaulStoffregen (no license mentioned)
//-------------------------------------------------------------
// Requires addon hb-ep-devices-addon version >= 1.17  https://github.com/FUEL4EP/HomeAutomation/releases/latest
//-------------------------------------------------------------
// the sensor HB-UNI-Sensor1-THPD-AHT20_BMP280 requires mandatorily an ATMega1284P MCU, recommended is the PCB HB-UNI-SEN-BATT_ATMega1284P_E07-868MS10_FRAM_FUEL4EP,
// see https://github.com/FUEL4EP/HomeAutomation/tree/master/AsksinPP_developments/PCBs/HB-UNI-SEN-BATT_ATMega1284P_E07-868MS10_FRAM_FUEL4EP
//-------------------------------------------------------------
// please set the baud rate in the serial monitor as 38400 baud
//-------------------------------------------------------------


#define NORTC
//-------------------------------------------------------------
// !! NDEBUG sollte aktiviert werden wenn die Sensorentwicklung und die Tests abgeschlossen sind und das Gerät in den 'Produktionsmodus' geht.
//
// #define NDEBUG                     // uncomment if bring up tests have been succeessfully finished
// #define USE_CC1101_ALT_FREQ_86835  // use alternative frequency to compensate not correct working cc1101 modules
//-------------------------------------------------------------
// define this to read the device id, serial and device type from bootloader section
// #define USE_OTA_BOOTLOADER


// this sketch requires an ATMega1284P MCU
// comment '#define  M1284P'
#define  M1284P


//#define DEEP_DISCHARGE_PROTECTION_ENABLED    // IMPORTANT: Comment during FTDI debugging otherwise the sensor will sleep forever if supplied by FTDI debugger w/o an inserted battery
                                               // IMPORTANT: Uncomment only for operational mode w/o FTDI debugger!!!

#define EI_NOTEXTERNAL
#include <EnableInterrupt.h>
#include <Adafruit_SPIDevice.h>  // necessary in order to avoid a compiller error, see https://homematic-forum.de/forum/viewtopic.php?f=76&t=64816&start=40#p779162
#include <AskSinPP.h>
#include <LowPower.h>
#include <MultiChannelDevice.h>
#include <Register.h>
#include "Sensors/tmBattery.h"

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

#define EByte_E07_868MS10      // uncomment if an EByte E07-868MS10 module (red color) is used as CC1101 transceiver
//#define EByte_E07_900MM10S   // uncomment if an EByte E07-900MM10S module (10 mm * 10 mm square) is used as CC1101 transceiver
//#define EByte_E07-900M10S    // uncomment if an EByte E07-900M10S module (14 mm * 20 mm) is used as CC1101 transceiver

// please install the addon hb-ep-devices-addon version >= 1.17 before pairing the sensor with the CCU/RaspberryMatic

//---------------------------------------------------------
// Alle Device Parameter werden aus einer .h Datei (hier Cfg/Device_AHT20_BMP280.h) geholt um mehrere Geräte ohne weitere Änderungen des
// Sketches flashen zu können. Für mehrere Geräte einfach mehrere .h Dateien anlegen und dort die Unterschiede zwischen den Geräten definieren. Die
// konfigurierbaren Device Parameter in der .h Datei sind im Einzelnen:
// - Device ID und Device Serial
// - Aktivierung der verwendeten Sensoren
// - Pin Definitionen Allgemein
// - Pin und Address Definitionen der Sensoren
// - Clock Definition
// - Schaltungsvariante und Pins für Batteriespannungsmessung
// - Schwellwerte für Batteriespannungsmessung
#include "Cfg/Device_AHT20_BMP280.h"

// number of available peers per channel
#define PEERS_PER_CHANNEL 6

// all library classes are placed in the namespace 'as'
using namespace as;

//correction factor of the clock inaccuracy (ceramic resonator is default on Arduino boards), if no quartz RTC is used, see also HB-UNI-Sen-CURRENT from Jérôme ( jp112sdl )
#define SYSCLOCK_FACTOR    0.91507106   // adjust to get sampling data exactly every 240 seconds

// both a Aosong AHT20 sensor as well as a Bosch BMP280 sensor are required

#ifdef SENSORS_AHT20_BMP280
#include "Sensors/Sens_AHT20_BMP280.h"    // HB-UNI-Sensor1 custom sensor class
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
// Device ID und Device Serial werden aus einer .h Datei (hier im Beispiel Cfg/Device_AHT20_BMP280.h) geholt um mehrere Geräte ohne weitere Änderungen des
// Sketches flashen zu können.
const struct DeviceInfo PROGMEM devinfo = {
    cDEVICE_ID,        // Device ID           defined in ./Cfg/Device_AHT20_BMP280.h
    cDEVICE_SERIAL,    // Device Serial       defined in ./Cfg/Device_AHT20_BMP280.h
    { 0xF6, 0x0F },    // Device Model        needs to fit to Addon XML hb-uni-sensor-THPD-AHT20_BMP280.xml line 6:
                       //                     <parameter index="10.0" size="2.0" const_value="0xF60F"
    0x10,
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
    void init(uint8_t msgcnt, int16_t temp, uint16_t my_humidity10, uint16_t airPressure10, uint16_t operatingVoltage1000, int16_t MA_24H_temp100, int16_t MA_7D_temp100, int16_t MA_1M_temp100, int16_t MA_1Y_temp100, bool batLow)
    {

        uint8_t t1 = (temp >> 8) & 0x7f;
        uint8_t t2 = temp & 0xff;
        if (batLow == true) {
            t1 |= 0x80;    // set bat low bit
        }

        DPRINT(F("msg T/H/P/V/MA_24h/MA_7D/MA_1M/MA_1Y (x10, MA x100) = "));
        DDEC(temp);DPRINT("/");
        DDEC(my_humidity10);;DPRINT("/");
        DDEC(airPressure10);;DPRINT("/");
        DDEC(operatingVoltage1000);DPRINT("/");
        DDEC(MA_24H_temp100);DPRINT(F("/"));
        DDEC(MA_7D_temp100);DPRINT(F("/"));
        DDEC(MA_1M_temp100);DPRINT(F("/"));
        DDEC(MA_1Y_temp100);DPRINTLN(F(" "));

        // als Standard wird BCAST gesendet um Energie zu sparen, siehe Beschreibung unten.
        // Bei jeder 40. Nachricht senden wir stattdessen BIDI|WKMEUP, um eventuell anstehende Konfigurationsänderungen auch
        // ohne Betätigung des Anlerntaster übernehmen zu können (mit Verzögerung, worst-case 40x Sendeintervall).
        uint8_t flags = BCAST;
        if ((msgcnt % 40) == 2) {
            flags = BIDI | WKMEUP;
        }
        DPRINT(F("msgcnt    : ")); DDECLN(msgcnt);
        
        
        Message::init(0x19, msgcnt, 0x70, flags, t1, t2); //  length 25 = 0x19 bytes (see also addon hb-ep-devices-addon/CCU_RM/src/addon/firmware/rftypes/hb-uni-sensor-THPD-AHT20_BMP280.xml)
        // Message Length (first byte param.): 11 + payload
        //  1 Byte payload -> length 12
        // 12 Byte payload -> length 23
        // max. payload: 17 Bytes (https://www.youtube.com/watch?v=uAyzimU60jw)

        // BIDI|WKMEUP: erwartet ACK vom Empfänger, ohne ACK wird das Senden wiederholt
        // LazyConfig funktioniert, d.h. eine anstehende Conf.Änderung von der CCU wird nach dem nächsten Senden übernommen. Aber erhöhter
        // Funkverkehr wegen ACK
        //
        // BCAST: ohne ACK zu Erwarten, Standard für HM Sensoren.
        // LazyConfig funktioniert nicht, d.h. eine anstehende Conf.Änderung von der CCU muss durch den Config Button am Sensor übernommen
        // werden!!

        // papa:
        // BIDI - fordert den Empfänger auf ein Ack zu schicken. Das wird auch zwingend für AES-Handling gebraucht. BCAST - signalisiert
        // eine Broadcast-Message. Das wird z.B. verwendet, wenn mehrere Peers vor einen Sensor existieren. Es wird dann an einen Peer
        // gesndet und zusätzlich das BCAST-Flag gesetzt. So dass sich alle die Nachrricht ansehen. Ein Ack macht dann natürlich keinen Sinn
        // - es ist ja nicht klar, wer das senden soll.
        //
        // WKMEUP - wird für LazyConfig verwendet. Ist es in einer Message gesetzt, so weiss
        // die Zentrale, dass das Geräte noch kurz auf weitere Nachrichten wartet. Die Lib setzt diese Flag für die StatusInfo-Message
        // automatisch. Außerdem bleibt nach einer Kommunikation der Empfang grundsätzlich für 500ms angeschalten.

     
        // humidity
        pload[0] =  (my_humidity10 >> 8) & 0xff;
        pload[1] =  (my_humidity10 >> 0) & 0xff;
        
        // pressure
        pload[2] =  (airPressure10 >> 8) & 0xff;
        pload[3] =  (airPressure10 >> 0) & 0xff;

        //operatingVoltage1000
        pload[4] =  (operatingVoltage1000 >> 8) & 0xff;
        pload[5] =  (operatingVoltage1000 >> 0) & 0xff;

        //MA_24H_temp100
        pload[6] =  (MA_24H_temp100 >> 8) & 0xff;
        pload[7] =  (MA_24H_temp100 >> 0) & 0xff;

        //MA_7D_temp100
        pload[8] =  (MA_7D_temp100 >> 8) & 0xff;
        pload[9] =  (MA_7D_temp100 >> 0) & 0xff;

        //MA_1M_temp100
        pload[10] =  (MA_1M_temp100 >> 8) & 0xff;
        pload[11] =  (MA_1M_temp100 >> 0) & 0xff;

        //MA_1Y_temp100
        pload[12] =  (MA_1Y_temp100 >> 8) & 0xff;
        pload[13] =  (MA_1Y_temp100 >> 0) & 0xff;

    }
};

// die "freien" Register 0x20/21 werden hier als 16bit memory für das Update
// Intervall in Sek. benutzt siehe auch hb-uni-sensor1.xml, <parameter
// id="Sendeintervall"> .. ausserdem werden die Register 0x22/0x23 für den
// konf. Parameter Höhe benutzt
// das Register 0x24 wird für die Einstellung der CC1101 Tx Sendeleistung verwendet
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


    void defaults()
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

DEFREGISTER(UReg1, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07)
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

    bool bootType (int8_t value) const {
      return
          this->writeRegister(0x04, (value) & 0xff)
          ;
    }

      int8_t bootType () const {
      return
          (int8_t)(this->readRegister(0x04, 0))
          ;
    }

    bool presetHistoricTemperatures (int8_t value) const {
      return
          this->writeRegister(0x05, (value) & 0xff)
          ;
    }

    int8_t presetHistoricTemperatures () const {
      return
          (int8_t)(this->readRegister(0x05, 0))
          ;
    }

    bool presetTemp10(int16_t value) const {
      return
        this->writeRegister(0x06, (value >> 8) & 0xff) && this->writeRegister(0x07, value & 0xff)
        ;
    }

    int16_t presetTemp10() const {
        return
            (this->readRegister(0x06, 0) << 8) + this->readRegister(0x07, 0)
            ;
    }


    void defaults () {
      clear();
      tempOffset10(0);                // temperature measurement offset, multiplied by 10 [K], must be positive, will be subtracted, calibrate your sensor's characteristics, enter in WebUI as device parameter for dynamic adjustment
      humidOffset10(0);               // humidity measurement offset, multiplied by 10 [%], calibrate your sensor's characteristics, enter in WebUI as device parameter for dynamic adjustment
      pressOffset10(0);               // pressure measurement offset, multiplied by 10 [hPa], calibrate your sensor's characteristics, enter in WebUI as device parameter for dynamic adjustment
      bootType(0);                    // boot type: 0 = cold boot; 1 = warm boot, enter in WebUI as device parameter for dynamic adjustment
      presetHistoricTemperatures(1);  // preset temperature mode: 0 = fixed preset temperature, 1 = interpolated historic temperatures; enter in WebUI as device parameter for dynamic adjustment
      presetTemp10(106);              // preset mean average temperature of a year (x10); used if presetHistoricTemperatures = 0; enter in WebUI as device parameter for dynamic adjustment
    }
    
};

class WeatherChannel : public Channel<Hal, SensorList1, EmptyList, List4, PEERS_PER_CHANNEL, SensorList0>, public Alarm {

    WeatherEventMsg msg;

    int16_t   temperature10;
    uint16_t  humidity10;
    uint16_t  airPressure10;
    uint16_t  operatingVoltage1000;
    int16_t   MA_24H_temp100;
    int16_t   MA_7D_temp100;
    int16_t   MA_1M_temp100;
    int16_t   MA_1Y_temp100;
    bool      regularWakeUp;

#ifdef SENSORS_AHT20_BMP280
    Sens_AHT20_BMP280   AHT20_BMP280;
#endif

public:
    WeatherChannel()
        : Channel()
        , Alarm(seconds2ticks(60))
        , temperature10(0)
        , humidity10(0)
        , airPressure10(0)
        , operatingVoltage1000(0)
        , MA_24H_temp100(0)
        , MA_7D_temp100(0)
        , MA_1M_temp100(0)
        , MA_1Y_temp100(0)
        , regularWakeUp(true)
    {
    }
    virtual ~WeatherChannel() {}

    virtual void trigger(AlarmClock& clock)
    {
        measure();
        
        device().battery().update();                            // get current battery voltage; measure every sampling cycle
        operatingVoltage1000 = device().battery().current();    // BatteryTM class, mV resolution
        DPRINT("battery voltage x1000 = ");       DDECLN(operatingVoltage1000);
        
        uint8_t msgcnt = device().nextcount();
        msg.init(msgcnt, temperature10, humidity10, airPressure10, operatingVoltage1000, MA_24H_temp100, MA_7D_temp100, MA_1M_temp100, MA_1Y_temp100, device().battery().low());
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

    void forceSend()
    {
        CLOCK.cancel(*this);
        regularWakeUp = false;    // Verhindert enableINT in trigger()
        trigger(CLOCK);           // Messen/Senden
        delay(250);               // Verzögerung für wiederholtes Senden bzw. digitalInput Entprellen
    }

    void measure()
    {


#ifdef SENSORS_AHT20_BMP280
        AHT20_BMP280.measure();
        temperature10         = AHT20_BMP280.temperature();
        humidity10            = AHT20_BMP280.humidity();
        airPressure10         = AHT20_BMP280.pressureNN();
        MA_24H_temp100        = AHT20_BMP280.moving_average_24h_temperature100();
        MA_7D_temp100         = AHT20_BMP280.moving_average_7D_temperature100();
        MA_1M_temp100         = AHT20_BMP280.moving_average_1M_temperature100();
        MA_1Y_temp100         = AHT20_BMP280.moving_average_1Y_temperature100();
        
        DPRINT(F("T/H/P/V/MA_24h/MA_7D/MA_1M/MA_1Y (x10, MA x100) = "));
        DDEC(temperature10);DPRINT("/");
        DDEC(humidity10);;DPRINT("/");
        DDEC(airPressure10);;DPRINT("/");
        DDEC(operatingVoltage1000);DPRINT("/");
        DDEC(MA_24H_temp100);DPRINT(F("/"));
        DDEC(MA_7D_temp100);DPRINT(F("/"));
        DDEC(MA_1M_temp100);DPRINT(F("/"));
        DDEC(MA_1Y_temp100);DPRINTLN(F(" "));
#endif
    }

    void initSensors()
    {

#ifdef SENSORS_AHT20_BMP280
        AHT20_BMP280.init((uint16_t)this->device().getList0().altitude(), (int8_t)this->getList1().tempOffset10(), (int8_t)this->getList1().humidOffset10(), (int8_t)this->getList1().pressOffset10(), (int8_t)this->getList1().bootType(), (int8_t)this->getList1().presetHistoricTemperatures(), (int16_t)this->getList1().presetTemp10());
#endif

        DPRINTLN(F("Sensor setup done"));
        DPRINT(F("Serial: "));
        DPRINTLN(cDEVICE_SERIAL);
#ifdef CLOCK_SYSCLOCK
        DPRINTLN(F("Clock SYSCLOCK"));
#elif defined CLOCK_RTC
        DPRINTLN(F("Clock RTC"));
#endif
        DPRINTLN(F("setting bootType to 'warm_boot'"));
        this->getList1().bootType(1);
    }

    void setup(Device<Hal, SensorList0>* dev, uint8_t number, uint16_t addr)
    {
        Channel::setup(dev, number, addr);
        initSensors();
        set(seconds2ticks(5));    // first message in 5 sec.
        CLOCK.add(*this);
    }

    void configChanged() {
      DPRINTLN("* Config Changed                 : List1");
      DPRINT(F("* Temperature Offset x10         : ")); DDECLN(this->getList1().tempOffset10());
      DPRINT(F("* Humidity Offset x10            : ")); DDECLN(this->getList1().humidOffset10());
      DPRINT(F("* Pressure Offset x10            : ")); DDECLN(this->getList1().pressOffset10());
      DPRINT(F("* Boot type                      : ")); DDECLN(this->getList1().bootType());
      DPRINT(F("* Preset historic temperatures   : ")); DDECLN(this->getList1().presetHistoricTemperatures());
      DPRINT(F("* Preset temperatures x10        : ")); DDECLN(this->getList1().presetTemp10());
      AHT20_BMP280.init((uint16_t)this->device().getList0().altitude(), (int8_t)this->getList1().tempOffset10(), (int8_t)this->getList1().humidOffset10(), (int8_t)this->getList1().pressOffset10(), (int8_t)this->getList1().bootType(), (int8_t)this->getList1().presetHistoricTemperatures(), (int16_t)this->getList1().presetTemp10());
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
        
        // Einstellung der CC1101 Tx Sendeleistung

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
