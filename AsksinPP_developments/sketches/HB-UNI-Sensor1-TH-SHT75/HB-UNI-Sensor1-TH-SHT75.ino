//---------------------------------------------------------
// HB-UNI-Sensor1-TH-SHT75    Sensirion SHT75 high accuracy Temperature and Humidity Sensor
// Version 1.18a
// (C) 2018-2020 Tom Major (Creative Commons)
// (C) 2020 FUEL4EP i      (Creative Commons)          adaptions to Sensirion SHT75 high accuracy sensor
// https://creativecommons.org/licenses/by-nc-sa/4.0/
// You are free to Share & Adapt under the following terms:
// Give Credit, NonCommercial, ShareAlike
// +++
// AskSin++                  2016 papa Creative Commons
// HB-UNI-Sensor1            2018 TomMajor (Creative Commons)
// Sensirion                 2014 spease https://github.com/spease/Sensirion         
//---------------------------------------------------------

//---------------------------------------------------------
// !! NDEBUG sollte aktiviert werden wenn die Sensorentwicklung und die Tests abgeschlossen sind und das Gerät in den 'Produktionsmodus' geht.
// Insbesondere die RAM-Einsparungen sind wichtig für die Stabilität / dynamische Speicherzuweisungen etc.
// Dies beseitigt dann auch die mögliche Arduino-Warnung 'Low memory available, stability problems may occur'.
//
//#define NDEBUG
//#define USE_CC1101_ALT_FREQ_86835  //use alternative frequency to compensate not correct working cc1101 modules

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

//please install the addon hb-ep-devices-addon before pairing the sensor with the CCU/RaspberryMatic

//---------------------------------------------------------
// Alle Device Parameter werden aus einer .h Datei (hier Cfg/Sens_SHT75.h) geholt um mehrere Geräte ohne weitere Änderungen des
// Sketches flashen zu können. Für mehrere Geräte einfach mehrere .h Dateien anlegen und dort die Unterschiede zwischen den Geräten definieren. Die
// konfigurierbaren Device Parameter in der .h Datei sind im Einzelnen:
// - Device ID und Device Serial
// - Aktivierung der verwendeten Sensoren
// - Pin Definitionen Allgemein
// - Pin und Address Definitionen der Sensoren
// - Clock Definition
// - Schaltungsvariante und Pins für Batteriespannungsmessung
// - Schwellwerte für Batteriespannungsmessung
#include "Cfg/Device_SHT75.h"


// number of available peers per channel
#define PEERS_PER_CHANNEL 6

// all library classes are placed in the namespace 'as'
using namespace as;


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
    { 0xF6, 0x03 },    // Device Model        needs to fit to Addon XML hb-uni-sensor-TH-SHT75.xml line 6:
                       //                     <parameter index="10.0" size="2.0" const_value="0xF603" 
    0x10,
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

    bool runready() { return CLOCK.runready() || BaseHal::runready(); }
} hal;

class WeatherEventMsg : public Message {
public:
    void init(uint8_t msgcnt, int16_t temp, uint16_t my_humidity10, uint16_t operatingVoltage1000, bool batLow)
    {

        uint8_t t1 = (temp >> 8) & 0x7f;
        uint8_t t2 = temp & 0xff;
        if (batLow == true) {
            t1 |= 0x80;    // set bat low bit
        }

        //DPRINT("check battery voltage1000 = ");         DDECLN(operatingVoltage1000);
        //DPRINT("check my humidity10 =       ");         DDECLN(my_humidity10);
        //DPRINT("check temperature10 =       ");         DDECLN(temp);

        // als Standard wird BCAST gesendet um Energie zu sparen, siehe Beschreibung unten.
        // Bei jeder 40. Nachricht senden wir stattdessen BIDI|WKMEUP, um eventuell anstehende Konfigurationsänderungen auch
        // ohne Betätigung des Anlerntaster übernehmen zu können (mit Verzögerung, worst-case 40x Sendeintervall).
        uint8_t flags = BCAST;
        if ((msgcnt % 40) == 2) {
            flags = BIDI | WKMEUP;
        }
        Message::init(0x0F, msgcnt, 0x70, flags, t1, t2); //  length 15 = 0x0F bytes (see also addon hb-ep-devices-addon/CCU_RM/src/addon/firmware/rftypes/hb-uni-sensor-TH-SHT75.xml)
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

        //operatingVoltage1000
        pload[2] = (operatingVoltage1000 >> 8) & 0xff;
        pload[3] = (operatingVoltage1000 >> 0) & 0xff;
    }
};

// die "freien" Register 0x20/21 werden hier als 16bit memory für das Update
// Intervall in Sek. benutzt siehe auch hb-uni-sensor1.xml, <parameter
// id="Sendeintervall"> .. ausserdem werden die Register 0x22/0x23 für den
// konf. Parameter Höhe benutzt
DEFREGISTER(Reg0, MASTERID_REGS, DREG_LEDMODE, DREG_LOWBATLIMIT, DREG_TRANSMITTRYMAX, 0x20, 0x21)
class SensorList0 : public RegList0<Reg0> {
public:
    SensorList0(uint16_t addr)
        : RegList0<Reg0>(addr)
    {
    }

    bool     updIntervall(uint16_t value) const { return this->writeRegister(0x20, (value >> 8) & 0xff) && this->writeRegister(0x21, value & 0xff); }
    uint16_t updIntervall() const { return (this->readRegister(0x20, 0) << 8) + this->readRegister(0x21, 0); }

    void defaults()
    {
        clear();
        ledMode(1);
        lowBatLimit(BAT_VOLT_LOW);
        transmitDevTryMax(6);
        updIntervall(240);
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
      humidOffset10(30);  // humidity measurement offset, multiplied by 10 [%], calibrate your sensor's characteristics, enter in WebUI as device parameter for dynamic adjustment
    }
    
};

class WeatherChannel : public Channel<Hal, SensorList1, EmptyList, List4, PEERS_PER_CHANNEL, SensorList0>, public Alarm {

    WeatherEventMsg msg;

    int16_t   temperature10;
    uint16_t  humidity10;
    uint16_t  operatingVoltage1000;
    bool      regularWakeUp;


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
    {
    }
    virtual ~WeatherChannel() {}

    virtual void trigger(AlarmClock& clock)
    {
#ifdef SENSOR_DIGINPUT
        digitalInput.disableINT();    // digitalInput Interrupt abschalten, dieser könnte beim Senden ausgelöst werden (bei PIR aufgetreten)
#endif
        measure();
        
        device().battery().update();                            // get current battery voltage; measure every sampling cycle
        operatingVoltage1000 = device().battery().current();    // BatteryTM class, mV resolution
        DPRINT("battery voltage x1000 = ");       DDECLN(operatingVoltage1000);
        
        uint8_t msgcnt = device().nextcount();
        msg.init(msgcnt, temperature10, humidity10, operatingVoltage1000, device().battery().low());
        if (msg.flags() & Message::BCAST) {
            device().broadcastEvent(msg, *this);
        } else {
            device().sendPeerEvent(msg, *this);
        }
        // reactivate for next measure
        uint16_t updCycle = this->device().getList0().updIntervall();
        set(seconds2ticks(max(updCycle,240)));
        CLOCK.add(*this);
        
        regularWakeUp = true;
    }

    void forceSend()
    {
        CLOCK.cancel(*this);
        regularWakeUp = false;    // Verhindert enableINT in trigger()
        trigger(CLOCK);           // Messen/Senden
        delay(250);               // Verzögerung für wiederholtes Senden bzw. digitalInput Entprellen
#ifdef SENSOR_DIGINPUT
        digitalInput.enableINT();
#endif
    }

    void measure()
    {


#ifdef SENSOR_SHT75
        sht75.measure();
        temperature10 = sht75.temperature();
        humidity10    = sht75.humidity();
#endif
    }

    void initSensors()
    {

#ifdef SENSOR_SHT75
        sht75.init((int16_t)this->getList1().tempOffset10(), (int16_t)this->getList1().humidOffset10());
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
      DPRINTLN("* Config Changed                                 : List1");
      DPRINT(F("* Temperature Offset x10                         : ")); DDECLN(this->getList1().tempOffset10());
      DPRINT(F("* Humidity Offset x10                            : ")); DDECLN(this->getList1().humidOffset10());
      sht75.init((int16_t)this->getList1().tempOffset10(), (int16_t)this->getList1().humidOffset10());
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
#ifdef SENSOR_DIGINPUT
        if (digitalInput.notifyEvent()) {
            digitalInput.resetEvent();
            DPRINTLN(F("DIGINPUT change"));
            sdev.channel(1).forceSend();
        }
#endif
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
