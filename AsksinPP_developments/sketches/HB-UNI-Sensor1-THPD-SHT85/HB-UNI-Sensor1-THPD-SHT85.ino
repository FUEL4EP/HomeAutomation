//---------------------------------------------------------
// HB-UNI-Sensor1-THPD-SHT85    Sensirion SHT85 (Temperature, Humidity, Dewpoint) / Bosch BME280 (Pressure) Temperature, Humidity, Pressure, Dewpoint, Vapor Concentration Sensor with WebUI offset setting
// Version 1.0
// (C) 2018-2020 Tom Major (Creative Commons)
// (C) 2020 FUEL4EP        (Creative Commons)          added SHT85 and adaptions to Bosch BME280 sensor /  offset setting by WebUI / dewpoint and vapor concentration
// https://creativecommons.org/licenses/by-nc-sa/4.0/
// You are free to Share & Adapt under the following terms:
// Give Credit, NonCommercial, ShareAlike
// +++
// AskSin++                  2016 papa (Creative Commons)
// HB-UNI-Sensor1            2018 TomMajor (Creative Commons)
// finitespace/BME280        2016 finitespace https://github.com/finitespace/BME280  (GNU General Public License v3.0) for BME280 sensor
// arduino-sht               2015 Sensirion https://github.com/Sensirion/arduino-sht  (BSD 3-Clause "New" or "Revised" License) for SHT85 sensor
//---------------------------------------------------------

#define SENSOR_ONLY
#define NORTC
#define SIMPLE_CC1101_INIT
//---------------------------------------------------------
// !! NDEBUG sollte aktiviert werden wenn die Sensorentwicklung und die Tests abgeschlossen sind und das Gerät in den 'Produktionsmodus' geht.
// Insbesondere die RAM-Einsparungen sind wichtig für die Stabilität / dynamische Speicherzuweisungen etc.
// Dies beseitigt dann auch die mögliche Arduino-Warnung 'Low memory available, stability problems may occur'.
//
// if an ATMega328P based Arduino Pro Mini is used, this requires to uncomment '// #define NDEBUG'
// #define NDEBUG
// #define USE_CC1101_ALT_FREQ_86835  //use alternative frequency to compensate not correct working cc1101 modules

//---------------------------------------------------------
// define this to read the device id, serial and device type from bootloader section
// #define USE_OTA_BOOTLOADER

// the sensor HB-UNI-Sensor1-THPD-SHT85 requires mandatorily an ATMega1284P MCU (Pro Mini XL - v2 - ATmega 1284p https://www.tindie.com/products/prominimicros/pro-mini-xl-v2-atmega-1284p/)

// please set the baud rate in the serial monitor as 38400 baud 

// comment '#define  M1284P' if an ATMega328P based Arduino Pro Mini is used, this requires to uncomment '// #define NDEBUG' above, a serial monitoring is NOT possible for an ATMega328P based Arduino Pro Mini
#define  M1284P

#ifdef M1284P
// comment '#define CALCULATE_MOVING_AVERAGES' if 24h and 7 days mving averages shall be calculated, this option requires an ATMega1284P
#define CALCULATE_MOVING_AVERAGES
#endif

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

// please install the addon hb-ep-devices-addon version >= 1.10 before pairing the sensor with the CCU/RaspberryMatic


//---------------------------------------------------------
// Alle Device Parameter werden aus einer .h Datei (hier Cfg/Sens_BME280.h) geholt um mehrere Geräte ohne weitere Änderungen des
// Sketches flashen zu können. Für mehrere Geräte einfach mehrere .h Dateien anlegen und dort die Unterschiede zwischen den Geräten definieren. Die
// konfigurierbaren Device Parameter in der .h Datei sind im Einzelnen:
// - Device ID und Device Serial
// - Aktivierung der verwendeten Sensoren
// - Pin Definitionen Allgemein
// - Pin und Address Definitionen der Sensoren
// - Clock Definition
// - Schaltungsvariante und Pins für Batteriespannungsmessung
// - Schwellwerte für Batteriespannungsmessung
#include "Cfg/Device_SHT85_BME280.h"

// number of available peers per channel
#define PEERS_PER_CHANNEL 6

// all library classes are placed in the namespace 'as'
using namespace as;

// both a Sensirion SHT85 as well as a Bosch BME280 are required

#ifdef SENSORS_SHT85_BME280
#include "Sensors/Sens_SHT85_BME280.h"    // HB-UNI-Sensor1 custom sensor class
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
    cDEVICE_ID,        // Device ID           defined in ./Cfg/Device_SHT85_BME280.h
    cDEVICE_SERIAL,    // Device Serial       defined in ./Cfg/Device_SHT85_BME280.h
#ifdef CALCULATE_MOVING_AVERAGES
    { 0xF6, 0x09 },    // Device Model        needs to fit to Addon XML hb-uni-sensor-THPD-SHT85.xml line 6:
                       //                     <parameter index="10.0" size="2.0" const_value="0xF609" 
#else
    { 0xF6, 0x04 },    // Device Model        needs to fit to Addon XML hb-uni-sensor-THPD-BME280.xml line 6:
                       //                     <parameter index="10.0" size="2.0" const_value="0xF604"
#endif
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
#ifdef CALCULATE_MOVING_AVERAGES
    void init(uint8_t msgcnt, int16_t temp, uint16_t my_humidity10, uint16_t airPressure10, int16_t dewpoint10, uint16_t vaporConcentration100, uint16_t operatingVoltage1000, bool batLow, int16_t MA_24H_temp10, int16_t MA_7D_temp10)
#else
    void init(uint8_t msgcnt, int16_t temp, uint16_t my_humidity10, uint16_t airPressure10, int16_t dewpoint10, uint16_t vaporConcentration100, uint16_t operatingVoltage1000, bool batLow)
#endif
    {

        uint8_t t1 = (temp >> 8) & 0x7f;
        uint8_t t2 = temp & 0xff;
        if (batLow == true) {
            t1 |= 0x80;    // set bat low bit
        }

        DPRINT("msg T/H/P/D/V/O (x10, ah x100) = ");
        DDEC(temp);DPRINT("/");
        DDEC(my_humidity10);;DPRINT("/");
        DDEC(airPressure10);;DPRINT("/");
        DDEC(dewpoint10);DPRINT("/");
        DDEC(vaporConcentration100);DPRINT("/");
#ifdef CALCULATE_MOVING_AVERAGES
        DDEC(operatingVoltage1000);DPRINT("/");
        DDEC(MA_24H_temp10);DPRINT("/");
        DDEC(MA_7D_temp10);DPRINTLN(" ");
#else
        DDEC(operatingVoltage1000);DPRINTLN(" ");
#endif

        // als Standard wird BCAST gesendet um Energie zu sparen, siehe Beschreibung unten.
        // Bei jeder 40. Nachricht senden wir stattdessen BIDI|WKMEUP, um eventuell anstehende Konfigurationsänderungen auch
        // ohne Betätigung des Anlerntaster übernehmen zu können (mit Verzögerung, worst-case 40x Sendeintervall).
        uint8_t flags = BCAST;
        if ((msgcnt % 40) == 2) {
            flags = BIDI | WKMEUP;
        }
        DPRINT(F("msgcnt    : ")); DDECLN(msgcnt);
        
 #ifdef CALCULATE_MOVING_AVERAGES       
        Message::init(0x19, msgcnt, 0x70, flags, t1, t2); //  length 25 = 0x19 bytes (see also addon hb-ep-devices-addon/CCU_RM/src/addon/firmware/rftypes/hb-uni-sensor-THPD-SHT85.xml); + 4 additional Bytes for moving averages
#else
        Message::init(0x15, msgcnt, 0x70, flags, t1, t2); //  length 21 = 0x15 bytes (see also addon hb-ep-devices-addon/CCU_RM/src/addon/firmware/rftypes/hb-uni-sensor-THPD-BME280.xml)
#endif
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
        
        // humidity
        pload[2] =  (airPressure10 >> 8) & 0xff;
        pload[3] =  (airPressure10 >> 0) & 0xff;

        // dewpoint
        pload[4] =  (dewpoint10 >> 8) & 0xff;
        pload[5] =  (dewpoint10 >> 0) & 0xff;
        
        // vapor concentration
        pload[6] =  (vaporConcentration100 >> 8) & 0xff;
        pload[7] =  (vaporConcentration100 >> 0) & 0xff;

        //operatingVoltage1000
        pload[8] =  (operatingVoltage1000 >> 8) & 0xff;
        pload[9] =  (operatingVoltage1000 >> 0) & 0xff;
        
#ifdef CALCULATE_MOVING_AVERAGES        
        //MA_24H_temp10
        pload[10] =  (MA_24H_temp10 >> 8) & 0xff;
        pload[11] =  (MA_24H_temp10 >> 0) & 0xff;
        
        //MA_7D_temp10
        pload[12] =  (MA_7D_temp10 >> 8) & 0xff;
        pload[13] =  (MA_7D_temp10 >> 0) & 0xff;
#endif
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
    uint16_t  operatingVoltage1000;
    bool      regularWakeUp;
#ifdef CALCULATE_MOVING_AVERAGES
    int16_t   MA_24H_temp10;
    int16_t   MA_7D_temp10;
#endif

#ifdef SENSORS_SHT85_BME280
    Sens_SHT85_Bme280   SHT85_BME280;
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
        , operatingVoltage1000(0)
        , regularWakeUp(true)
#ifdef CALCULATE_MOVING_AVERAGES
        , MA_24H_temp10(0)
        , MA_7D_temp10(0)
#endif
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
#ifdef CALCULATE_MOVING_AVERAGES
        msg.init(msgcnt, temperature10, humidity10, airPressure10, dewpoint10, vaporConcentration100, operatingVoltage1000, device().battery().low(), MA_24H_temp10, MA_7D_temp10);
#else
        msg.init(msgcnt, temperature10, humidity10, airPressure10, dewpoint10, vaporConcentration100, operatingVoltage1000, device().battery().low());
#endif
        if (msg.flags() & Message::BCAST) {
            device().broadcastEvent(msg, *this);
        } else {
            device().sendPeerEvent(msg, *this);
        }
        // reactivate for next measure
        uint16_t updCycle = this->device().getList0().updIntervall();
        set(seconds2ticks(updCycle));
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


#ifdef SENSORS_SHT85_BME280
        SHT85_BME280.measure();
        temperature10         = SHT85_BME280.temperature();
        humidity10            = SHT85_BME280.humidity();
        airPressure10         = SHT85_BME280.pressureNN();
        dewpoint10            = SHT85_BME280.dewPoint();
        vaporConcentration100 = SHT85_BME280.vaporConcentration();
        
        
        DPRINT("T/H/P/D/V (x10, aH x100) = ");
        DDEC(temperature10);DPRINT("/");
        DDEC(humidity10);;DPRINT("/");
        DDEC(airPressure10);;DPRINT("/");
        DDEC(dewpoint10);DPRINT("/");
        DDEC(vaporConcentration100);DPRINTLN(" ");
#endif
#ifdef CALCULATE_MOVING_AVERAGES
        MA_24H_temp10        = SHT85_BME280.moving_average_24h_temperature10();
        MA_7D_temp10         = SHT85_BME280.moving_average_7D_temperature10();
        
        DPRINT("MA T (x10) = ");
        DDEC(MA_24H_temp10);DPRINT("/");
        DDEC(MA_7D_temp10);DPRINTLN(" ");
#endif 
    }

    void initSensors()
    {

#ifdef SENSORS_SHT85_BME280
        SHT85_BME280.init((uint16_t)this->device().getList0().altitude(), (int8_t)this->getList1().tempOffset10(), (int8_t)this->getList1().humidOffset10(), (int8_t)this->getList1().pressOffset10());
#endif

        DPRINTLN(F("Sensor setup done"));
        DPRINT(F("Serial: "));
        DPRINTLN(cDEVICE_SERIAL);
#ifdef CLOCK_SYSCLOCK
        DPRINTLN(F("Clock SYSCLOCK"));
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
      SHT85_BME280.init((uint16_t)this->device().getList0().altitude(), (int8_t)this->getList1().tempOffset10(), (int8_t)this->getList1().humidOffset10(), (int8_t)this->getList1().pressOffset10());
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
