//- -----------------------------------------------------------------------------------------------------------------------
// Range_test_beacon for evaluating the RF range of a RF transceiver
// is used a a distant beacon device that periodically is transmitting dummy sensor data
// an attached sensor is not needed
//
// Please use this sketch in parallel with the receiver sketch 'FreqTest_1284P_range_test_with_distant_beacon'
// The beacon 'Range_test_beacon' and the receiver 'FreqTest_1284P_range_test_with_distant_beacon' form test RF transmission path.
// Recommendation is to use a metal for additional attenuation. Put your 'Range_test_beacon' hardware into the metal can for that purpose.
//- -----------------------------------------------------------------------------------------------------------------------
// AskSin++
// 2016-10-31 papa Creative Commons - http://creativecommons.org/licenses/by-nc-sa/3.0/de/
// derived from HM-WDS40-TH-I-SHT10
// 2019-10-02 jp112sdl Creative Commons - http://creativecommons.org/licenses/by-nc-sa/3.0/de/
//- -----------------------------------------------------------------------------------------------------------------------
// stripped down to s sensorless device that is periodically transmitting dummy data as a dummy distant sensor
// to be used in conjunction with sketch 'FreqTest_1284P_distant_sensor'
//
// ci-test=yes board=328p aes=no

// define this to read the device id, serial and device type from bootloader section
// #define USE_OTA_BOOTLOADER

// #define USE_LCD             //bei Verwendung des LCD Displays https://www.aliexpress.com/item/1435066364.html

#define EI_NOTEXTERNAL
#include <EnableInterrupt.h>
#include <AskSinPP.h>
#include <LowPower.h>

#include <MultiChannelDevice.h>

// we use a Pro Mini
// Arduino pin for the LED
// D4 == PIN 4 on Pro Mini
#define LED_PIN 4
// Arduino pin for the config button
// B0 == PIN 8 on Pro Mini
#define CONFIG_BUTTON_PIN 8

// number of available peers per channel
#define PEERS_PER_CHANNEL 6

//seconds between sending dummy messages
#define MSG_INTERVAL 10  // adapt to your needs, but ensure that you follow the local RF regulations of your country

// all library classes are placed in the namespace 'as'
using namespace as;

// define all device properties
const struct DeviceInfo PROGMEM devinfo = {
  {0x77, 0x77, 0x77},     // Device ID
  "BEACON0001",           // Device Serial
  {0x00, 0x3f},           // Device Model
  0x10,                   // Firmware Version
  as::DeviceType::THSensor, // Device Type
  {0x01, 0x00}            // Info Bytes
};

/**
   Configure the used hardware
*/
typedef AvrSPI<10, 11, 12, 13> SPIType;
typedef Radio<SPIType, 2> RadioType;
typedef StatusLed<LED_PIN> LedType;
typedef AskSin<LedType, IrqInternalBatt, RadioType> Hal;
Hal hal;


class WeatherEventMsg : public Message {
  public:
    void init(uint8_t msgcnt, int16_t temp, uint8_t humidity, bool batlow) {
      uint8_t t1 = (temp >> 8) & 0x7f;
      uint8_t t2 = temp & 0xff;
      if ( batlow == true ) {
        t1 |= 0x80; // set bat low bit
      }
      Message::init(0xc, msgcnt, 0x70, BIDI | WKMEUP, t1, t2);
      pload[0] = humidity;
    }
};

class WeatherChannel : public Channel<Hal, List1, EmptyList, List4, PEERS_PER_CHANNEL, List0>, public Alarm {

    WeatherEventMsg msg;
    int16_t         temp;
    uint8_t         humidity;
    uint16_t        millis;

  public:
    WeatherChannel () : Channel(), Alarm(2), temp(0), humidity(0), millis(0) {}
    virtual ~WeatherChannel () {}


    // here we the dummy measurement data
    void measure () {
      DPRINT("Create dummy measurement values ...\n");
      temp=999;
      humidity=88;
      DPRINT("T/H = " + String(temp) + "/" + String(humidity) + "\n");
    }

    virtual void trigger (__attribute__ ((unused)) AlarmClock& clock) {
      uint8_t msgcnt = device().nextcount();
      // reactivate for next measure
      tick = delay();
      clock.add(*this);
      measure();
      msg.init(msgcnt, temp, humidity, device().battery().low());
      device().broadcastEvent(msg, *this);
    }

    uint32_t delay () {
      return seconds2ticks(MSG_INTERVAL);
    }
    void setup(Device<Hal, List0>* dev, uint8_t number, uint16_t addr) {
      Channel::setup(dev, number, addr);
      sysclock.add(*this);
    }

    uint8_t status () const {
      return 0;
    }

    uint8_t flags () const {
      return 0;
    }
};

typedef MultiChannelDevice<Hal, WeatherChannel, 1> WeatherType;
WeatherType sdev(devinfo, 0x20);
ConfigButton<WeatherType> cfgBtn(sdev);

void setup () {
  DINIT(57600, ASKSIN_PLUS_PLUS_IDENTIFIER);
  sdev.init(hal);
  hal.initBattery(60UL * 60, 22, 19);
  buttonISR(cfgBtn, CONFIG_BUTTON_PIN);
  sdev.initDone();
}

void loop() {
  bool worked = hal.runready();
  bool poll = sdev.pollRadio();
  if ( worked == false && poll == false ) {
    hal.activity.savePower<Sleep<>>(hal);
  }
}
