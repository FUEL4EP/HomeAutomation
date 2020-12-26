//---------------------------------------------------------
// HB-UNI-Sensor1-CO2_SCD30
// Version 2.0
// (C) 2020 FUEL4EP (Creative Commons)
// https://creativecommons.org/licenses/by-nc-sa/4.0/
// You are free to Share & Adapt under the following terms:
// Give Credit, NonCommercial, ShareAlike
// +++
// AskSin++                       2016 papa (Creative Commons)
// HB-UNI-Sensor1                 2018-2020 TomMajor (Creative Commons)
// SparkFun_SCD30_Arduino_Library 2018 sparkfun (Creative Commons)
// Adafruit_ADS1X15               2012 Adafruit (BSD license)
//---------------------------------------------------------

//---------------------------------------------------------
// !! NDEBUG sollte aktiviert werden wenn die Sensorentwicklung und die Tests abgeschlossen sind und das Gerät in den 'Produktionsmodus' geht.
// Zum Beispiel bei aktiviertem BME280 und MAX44009 werden damit ca. 2,6 KBytes Flash und 100 Bytes RAM eingespart.
// Insbesondere die RAM-Einsparungen sind wichtig für die Stabilität / dynamische Speicherzuweisungen etc.
// Dies beseitigt dann auch die mögliche Arduino-Warnung 'Low memory available, stability problems may occur'.
//
//#define NDEBUG
#define USE_CC1101_ALT_FREQ_86835  //use alternative frequency to compensate not correct working cc1101 modules

//---------------------------------------------------------

#define ADS1115
#define SOLAR_CHARGE


#define EI_NOTEXTERNAL
#include <EnableInterrupt.h>
#include <AskSinPP.h>
#include <LowPower.h>
#include <MultiChannelDevice.h>
#include <Register.h>
#include "Sensors/tmBattery.h"

//---------------------------------------------------------
// Alle Device Parameter werden aus einer .h Datei (hier im Beispiel Cfg/Device_Example.h) geholt um mehrere Geräte ohne weitere Änderungen des
// Sketches flashen zu können. Für mehrere Geräte einfach mehrere .h Dateien anlegen und dort die Unterschiede zwischen den Geräten definieren. Die
// konfigurierbaren Device Parameter in der .h Datei sind im Einzelnen:
// - Device ID und Device Serial
// - Aktivierung der verwendeten Sensoren
// - Pin Definitionen Allgemein
// - Pin und Address Definitionen der Sensoren
// - Clock Definition
// - Schaltungsvariante und Pins für Batteriespannungsmessung
// - Schwellwerte für Batteriespannungsmessung
#include "Cfg/Device_SCD30.h"

#define PARAMETER_AMBIENT_PRESSURE             0      // ambient air pressure for the SCD30's internal compensation, set to zero for disabling pressure compensation, see 'Interface Description Sensirion SCD30Sensor Module'

// number of available peers per channel
#define PEERS_PER_CHANNEL 6


// all library classes are placed in the namespace 'as'
using namespace as;

#ifdef ADS1115
   #include <Adafruit_ADS1015.h>
   Adafruit_ADS1115 ads;  /* Use this for the 16-bit version */
   const float ADC0_FACTOR = 2 * 0.0625 * 3.509 / 3.486 ; // 2 is the uncorrected volate divider ratio; 0.0625 is the ADS115 ADC resolution for the selected gain of TWO
                                                          // 3.509 / 3.486 is the voltage divider correction factor for ADCO0 based on a multimeter comparison
   const float ADC1_FACTOR = 2 * 0.0625 * 2.582 / 2.560 * 0.9958; // 2 is the uncorrected volate divider ratio; 0.0625 is the ADS115 ADC resolution for the selected gain of TWO
                                                          // 2.582 / 2.560 is the voltage divider correction factor for ADC1 based on a multimeter comparison
   #define LOWBAT_2x_NiMH_BATTERIES  2200                 // mV; low bat threshold for NiMH battery = 1100 mV
#endif
  
#ifdef SOLAR_CHARGE
  #define UPPER_THRESHOLD_NiMH_VOLTAGE                  2950  // mV; charge up to this upper threshold by solar
  #define VCC_THRESHOLD_VOLTAGE_INDICATING_SOLAR_POWER  3840  // mV; minimum VCC to activate solar charging
  #define SOLAR_CHARGE_ACTIVATION_PIN 6                       // controls external PNP transistor to charge the accumulator battery by solar power
  #define PNP_SOLAR_CHARGER_OFF  HIGH                         // SOLAR_CHARGE_ACTIVATION_PIN high level
  #define PNP_SOLAR_CHARGER_ON   LOW                          // SOLAR_CHARGE_ACTIVATION_PIN low level
#endif

#ifdef SENSOR_SCD30
#include "Sensors/Sens_SCD30.h"    // HB-UNI-Sensor1 custom sensor class
#endif

#ifdef  CLOCK_SYSCLOCK
#define CLOCK sysclock
#define SAVEPWR_MODE Sleep<>
#elif   defined CLOCK_RTC
#define CLOCK rtc
#define SAVEPWR_MODE SleepRTC
#undef  seconds2ticks
#define seconds2ticks(tm) (tm)
#else
#error  INVALID CLOCK OPTION
#endif

// define all device properties
// Bei mehreren Geräten des gleichen Typs (HB-UNI-Sensor1) muss Device ID und Device Serial unterschiedlich sein!
// Device ID und Device Serial werden aus einer .h Datei (hier im Beispiel Cfg/Device_Example.h) geholt um mehrere Geräte ohne weitere Änderungen des
// Sketches flashen zu können.
const struct DeviceInfo PROGMEM devinfo = {
    cDEVICE_ID,        // Device ID                 defined in ./Cfg/Device_SCD30.h
    cDEVICE_SERIAL,    // Device Serial             defined in ./Cfg/Device_SCD30.h
    { 0xf6, 0x01 },    // Device Model              needs to fit to Addon XML hb-uni-sensor-CO2-SCD30.xml line 6:
                       //                           <parameter index="10.0" size="2.0" const_value="0xF601" /
    0x10,              // Firmware Version
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
      radio.initReg(CC1101_FREQ0, 0x72);
      //0x216572 868.303 MHz
#endif
#ifdef CLOCK_RTC
        rtc.init();    // init real time clock - 1 tick per second
#endif
        // measure battery every 12h
        battery.init(seconds2ticks(12UL * 60 * 60), CLOCK);
        battery.low(BAT_VOLT_LOW);
        battery.critical(BAT_VOLT_CRITICAL);
#ifdef ADS1115
        ads.begin();
        ads.setGain(GAIN_TWO);        // 2x gain   +/- 2.048V  1 bit = 1mV      0.0625mV
        ads.begin();
#endif

#ifdef SOLAR_CHARGE
  pinMode(SOLAR_CHARGE_ACTIVATION_PIN, OUTPUT);
  digitalWrite(SOLAR_CHARGE_ACTIVATION_PIN, LOW);
#endif

    }

    bool runready() { return CLOCK.runready() || BaseHal::runready(); }
} hal;

#ifdef SENSOR_SCD30
    Sens_SCD30 scd30;
#endif

class WeatherEventMsg : public Message {
public:
#ifdef ADS1115
    void init(uint8_t msgcnt, int16_t temp, uint32_t carbondioxide, uint8_t humidity,
              uint16_t operatingVoltage1000, bool batLow, uint16_t batteryVoltage1000, uint16_t VCCVoltage1000)
#else
    void init(uint8_t msgcnt, int16_t temp, uint32_t carbondioxide, uint8_t humidity,
              uint16_t operatingVoltage1000, bool batLow)
#endif
    {

        uint8_t t1 = (temp >> 8) & 0x7f;
        uint8_t t2 = temp & 0xff;
        if (batLow == true) {
            t1 |= 0x80;    // set bat low bit
        }

 #ifdef ADS1115        
        if ( batteryVoltage1000 < LOWBAT_2x_NiMH_BATTERIES ) {
            t1 |= 0x80;    // set bat low bit       
        }
 #endif
  
        // als Standard wird BCAST gesendet um Energie zu sparen, siehe Beschreibung unten.
        // Bei jeder 40. Nachricht senden wir stattdessen BIDI|WKMEUP, um eventuell anstehende Konfigurationsänderungen auch
        // ohne Betätigung des Anlerntaster übernehmen zu können (mit Verzögerung, worst-case 40x Sendeintervall).
        uint8_t flags = BCAST;
        if ((msgcnt % 40) == 2) {
            flags = BIDI | WKMEUP;
        }
        Message::init(23, msgcnt, 0x70, flags, t1, t2);

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


        // carbondioxide
        pload[0] = (carbondioxide >> 24) & 0xff;
        pload[1] = (carbondioxide >> 16) & 0xff;
        pload[2] = (carbondioxide >> 8) & 0xff;
        pload[3] = (carbondioxide >> 0) & 0xff;

        // humidity
        pload[4] = humidity;

        // operatingVoltage1000
        pload[5] = (operatingVoltage1000 >> 8) & 0xff;
        pload[6] = (operatingVoltage1000 >> 0) & 0xff;
        
#ifdef ADS1115

        // batteryVoltage1000
        pload[7] = (batteryVoltage1000 >> 8) & 0xff;
        pload[8] = (batteryVoltage1000 >> 0) & 0xff;

        // VCCVoltage1000
        pload[9] =  (VCCVoltage1000 >> 8) & 0xff;
        pload[10] = (VCCVoltage1000 >> 0) & 0xff;
#endif

    }
};

// die "freien" Register 0x20/21 werden hier als 16bit memory für das Update
// Intervall in Sek. benutzt siehe auch hb-uni-sensor1.xml, <parameter
// id="Sendeintervall"> .. ausserdem werden die Register 0x22/0x23 für den
// konf. Parameter Höhe benutzt
DEFREGISTER(Reg0, MASTERID_REGS, DREG_LEDMODE, DREG_LOWBATLIMIT, DREG_TRANSMITTRYMAX, 0x20, 0x21, 0x22, 0x23)
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

    void defaults()
    {
        clear();
        ledMode(1);
        lowBatLimit(BAT_VOLT_LOW);
        transmitDevTryMax(6);
        updIntervall(240);        // measurement intervall 240 seconds          ; adjust to your needs in WebUI device / device/channel parameter page
        altitude(84);             // height of sensor's location above sea level; adjust to your sensor's location in WebUI device / device/channel parameter page
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
      tempOffset10(0);       // temperature measurement offset, multiplied by 10 [K], must be positive, will be subtracted, calibrate your sensor's characteristics, enter in WebUI as device parameter for dynamic adjustment
      humidOffset10(30);     // humidity measurement offset, multiplied by 10 [%], calibrate your sensor's characteristics, enter in WebUI as device parameter for dynamic adjustment
    }
    
};


class WeatherChannel : public Channel<Hal, SensorList1, EmptyList, List4, PEERS_PER_CHANNEL, SensorList0>, public Alarm {

    WeatherEventMsg msg;

    int16_t  temperature10;
    uint32_t carbondioxide100;
    uint8_t  humidity;
    uint16_t operatingVoltage1000;
    bool     regularWakeUp;

#ifdef ADS1115
    int16_t adc0_VCC, adc1_VBat, adc2, adc3;
    float adc0_VCC_f, adc1_VBat_f, adc2_f, adc3_f;
    uint16_t batteryVoltage1000;
    uint16_t VCCVoltage1000;
#endif


//#ifdef SENSOR_SCD30
//    Sens_SCD30 scd30;
//#endif


public:
    WeatherChannel()
        : Channel()
        , Alarm(seconds2ticks(60))
        , temperature10(0)
        , carbondioxide100(0)
        , humidity(0)
        , operatingVoltage1000(0)
        , regularWakeUp(true)
    {
    }
    virtual ~WeatherChannel() {}

    virtual void trigger(AlarmClock& clock)
    {
        measure();
        uint8_t msgcnt = device().nextcount();
#ifdef ADS1115
        msg.init(msgcnt, temperature10, carbondioxide100, humidity, operatingVoltage1000, device().battery().low(), batteryVoltage1000, VCCVoltage1000);
#else
        msg.init(msgcnt, temperature10, carbondioxide100, humidity, operatingVoltage1000, device().battery().low());
#endif  
        if (msg.flags() & Message::BCAST) {
            device().broadcastEvent(msg, *this);
        } else {
            device().sendPeerEvent(msg, *this);
        }
        DPRINT(F("temperature10        : "));
        DDECLN(temperature10);
        DPRINT(F("carbondioxide100     : "));
        DDECLN(carbondioxide100);
        DPRINT(F("humidity             : "));
        DDECLN(humidity);
        DPRINT(F("operatingVoltage1000 : "));
        DDECLN(operatingVoltage1000);
#ifdef ADS1115
        DPRINT(F("batteryVoltage1000   : "));
        DDECLN(batteryVoltage1000);
        DPRINT(F("VCCVoltage1000       : "));
        DDECLN(VCCVoltage1000);
#endif
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
#ifdef SENSOR_DIGINPUT
        digitalInput.enableINT();
#endif
    }

    void measure()
    {
 
#ifdef  SENSOR_SCD30
        scd30.measure();
        temperature10    = scd30.temperature();
        carbondioxide100 = scd30.carbondioxide();
        humidity         = scd30.humidity();
#endif
#ifdef ADS1115
  #ifdef SOLAR_CHARGE
        digitalWrite(SOLAR_CHARGE_ACTIVATION_PIN, PNP_SOLAR_CHARGER_OFF); // deactivate solar charging for voltage measurement
        delayMicroseconds(30000);                                         // wait for 30 ms until VCC voltage has settled, 47 uF need to be charged by the solar step-up charger
  #endif
       adc0_VCC  = ads.readADC_SingleEnded(0);
       //delayMicroseconds(100);
       adc1_VBat = ads.readADC_SingleEnded(1);
       //adc2 = ads.readADC_SingleEnded(2);
       //adc3 = ads.readADC_SingleEnded(3);
       adc0_VCC_f  = (float)adc0_VCC*ADC0_FACTOR;
       adc1_VBat_f = (float)adc1_VBat*ADC1_FACTOR;
       DPRINT(F("VCC(ADC0)   : "));
       DDECLN(adc0_VCC_f);
       DPRINT(F("VBat(ADC1)  : "));
       DDECLN(adc1_VBat_f);
       //DDECLN(adc2);
       //DDECLN(adc3);
       batteryVoltage1000 = (uint16_t)adc1_VBat_f;   //captured by HW ADC ADS1115
       VCCVoltage1000     = (uint16_t)adc0_VCC_f;    //captured by HW ADC ADS1115    
#endif
       DPRINT(F("VCC(MCU ADC): "));
       device().battery().update();
       operatingVoltage1000 = device().battery().current();    // BatteryTM class, mV resolution

#ifdef SOLAR_CHARGE
  if ( VCCVoltage1000 > VCC_THRESHOLD_VOLTAGE_INDICATING_SOLAR_POWER ) {
    if ( batteryVoltage1000 < UPPER_THRESHOLD_NiMH_VOLTAGE ) {
      digitalWrite(SOLAR_CHARGE_ACTIVATION_PIN, PNP_SOLAR_CHARGER_ON); // ACTIVATE SOLAR CHARGING
    }
    else
    {
      digitalWrite(SOLAR_CHARGE_ACTIVATION_PIN, PNP_SOLAR_CHARGER_OFF); // DEACTIVATE SOLAR CHARGING
    }
  }
  else
  {
    digitalWrite(SOLAR_CHARGE_ACTIVATION_PIN, PNP_SOLAR_CHARGER_OFF); // DEACTIVATE SOLAR CHARGING
  }
#endif

    }

    void initSensors()
    {

#ifdef SENSOR_SCD30
        scd30.init(this->device().getList0().updIntervall(), this->device().getList0().altitude(), PARAMETER_AMBIENT_PRESSURE, (int16_t)this->getList1().tempOffset10(), (int16_t)this->getList1().humidOffset10());
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
      scd30.init(this->device().getList0().updIntervall(), this->device().getList0().altitude(), PARAMETER_AMBIENT_PRESSURE, (int16_t)this->getList1().tempOffset10(), (int16_t)this->getList1().humidOffset10());
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
            // stop continuous measurements of SCD30 before falling to sleep
            scd30.stop_measurements();
            // this call will never return
            hal.activity.sleepForever(hal);
        }
        // if nothing to do - go sleep
        hal.activity.savePower<SAVEPWR_MODE>(hal);
        //hal.activity.savePower<Idle<>>(hal);
    }
}
