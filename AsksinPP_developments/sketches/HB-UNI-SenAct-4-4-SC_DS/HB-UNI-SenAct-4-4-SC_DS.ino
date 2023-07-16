//- -----------------------------------------------------------------------------------------------------------------------
// AskSin++
// 2016-10-31 papa Creative Commons - http://creativecommons.org/licenses/by-nc-sa/3.0/de/
// 2018-08-13 jp112sdl Creative Commons - http://creativecommons.org/licenses/by-nc-sa/3.0/de/
// 2023-07-07 FUEL4EP adaptions to PCB HB-UNI-SenAct-4-4-SC_DS_FUEL4EP 
// special thanks to jp112sdl for the base version of HB-UNI-SenAct-4-4
//- -----------------------------------------------------------------------------------------------------------------------
// ci-test=yes board=328p aes=no

// 3-channel power supply switch with hybrid power supply accumulator for start-up / mains operation and automatic charging of accumulators
// adapted to the PCB HB-UNI-SenAct-4-4-SC_DS_FUEL4EP https://github.com/FUEL4EP/HomeAutomation/tree/master/AsksinPP_developments/PCBs/HB-UNI-SenAct-4-4-SC_DS_FUEL4EP
// The fourth switch of HB-UNI-SenAct-4-4_SC is internally used for switching on the mains operationthe first hundred milliseconds after activating a relay
// The battery is only used for supplying the device during sleep mode and for the first few hundred milliseconds after activating a relay output, then the main supply is taking over
// The device can be attached to a main supply with a demand switch (in German: Netzfreischalter), e.g. in a sleeping room
// The software for charging the accumulator batteries is not yet implemented, please charge the accumulator batteries manually for the time being!



// derived from https://github.com/jp112sdl/HB-UNI-SenAct-4-4 (C) jp112sdl
// additional functionality for the hybrid charge mode will be added later on; currently they are not yet implemented in software, but only in hardware
// the class template derived_SwitchChannel is inherited from the class template SwitchChannel of AsksinnPP. Some overloading is done for the required new functionality.


// define this to read the device id, serial and device type from boot-loader section
// #define USE_OTA_BOOTLOADER

#define  USE_WOR

#define  EI_NOTEXTERNAL
#include <EnableInterrupt.h>
#include <AskSinPP.h>
#include <LowPower.h>
#include <Switch.h>
#include <ContactState.h>
#include "tmBattery.h"
#include "PulseReset.h"

#define BAT_VOLT_LOW        46  // 4.6V accumulator voltage for low battery signalling measured at pin A2 (see above); Vbandgap=1.1V (ATMega328P); send a low-voltage message if supply voltage is lower than 4.6V
#define BAT_VOLT_CRITICAL   44  // 4.4V accuumulator voltage for switch off (sleep forever)

// mapping of analog pins as digital I/O
// see definitions at pins_arduino.h
// do not change in case you are using the PCB HB-UNI-SenAct-4-4-SC_DS_FUEL4EP
#define RELAY_PIN_1 A4          // 18  // AC supply // IN4 // brown
#define RELAY_PIN_2 A5          // 19  // Hifiberry // IN3 // white
#define RELAY_PIN_3 A0          // 14  // Monitor   // IN2 // lila
#define RELAY_PIN_4 A1          // 15  // Heating   // IN1 // green

#define SENS_PIN_1  5           // AC supply   // brown
#define SENS_PIN_2  6           // Hifiberry   // white
#define SENS_PIN_3  7           // Monitor     // lila
#define SENS_PIN_4  9           // Heating     // green

#define CHARGE_CONTROL_PIN  A3                 // control pin for charging the accumulators, active low, the charging software is not yet implemented, please charge the accumulator batteries manually for the time being!
#define MAX_ACCUMULATOR_VOLTAGE  5100          // limit the accumulators voltage: charging is disabled if the accumulator voltage is above this threshold

#define V_RELAIS_BAT_SUPPLIED_PIN  3           // D3
#define PULSE_WIDTH_V_RELAIS_BAT_SUPPLIED_PIN  millis2ticks(1000UL)               // pulse width at V_RELAIS_BAT_SUPPLIED_PIN, needs to be long enough to switch the AC power relay and to power up the main DC power
#define PULSE_WIDTH_CHARGE_CONTROL_PIN         seconds2ticks(60UL * 20 * 0.88)    // pulse width at CHARGE_CONTROL_PIN for an intermediate charging of the accumulator batteris after having switched on a relay output (20 minutes)
                                                                                  // 60 seconds * 15 (= minutes) * corrective factor

#define SABOTAGE_PIN_1    false                // disable SABOTAGE_PIN false=0

#define LED_PIN           4
#define CONFIG_BUTTON_PIN 8

// number of available peers per channel
#define CREATE_INTERNAL_PEERINGS          // needs to be set in order to create the internal peerings
#define PEERS_PER_SwitchChannel  6
#define PEERS_PER_SENSCHANNEL    6

#define DEV_MODEL 0x0E
#define CYCLETIME seconds2ticks(60UL * 60 * 12 * 0.88) // 60 seconds * 60 (= minutes) * 12 (=hours) * corrective factor

//#define  BatterySensor
#define BAT_SENSOR tmBatteryResDiv<A6, A2, 5700>   //see tmbattery.h; measurement of accumulator voltage; sense pin = A6; activationn pin = A2; Rhigh = 470kOhm; Rlow = 100kOhm;  Vinmax=6.27V


// all library classes are placed in the name-space 'as'
using namespace as;

PulseReset PulseReset_V_RELAIS_BAT_SUPPLIED_PIN;         // see PulseReset.h
PulseReset PulseReset_CHARGE_CONTROL_PIN;    // see PulseReset.h

// define all device properties
const struct DeviceInfo PROGMEM devinfo = {
  {0xf6, DEV_MODEL, 0x01},// Device ID
  "SENACTDS01",
  // Device Serial
  {0xf6, DEV_MODEL},      // Device Model
  0x10,                   // Firmware Version
  as::DeviceType::Switch, // Device Type
  {0x01, 0x00}            // Info Bytes
};

/**
   Configure the used hardware
*/
//typedef AvrSPI<10, 11, 12, 13> RadioSPI;
typedef AvrSPI<10, 11, 12, 13, ArduinoPins, true> RadioSPI;  // see https://homematic-forum.de/forum/viewtopic.php?f=76&t=71788&hilit=SCKFloatOnIdle&start=30
typedef AskSin<StatusLed<LED_PIN>, BAT_SENSOR, Radio<RadioSPI, 2> > Hal;
Hal hal;


DEFREGISTER(Reg0, MASTERID_REGS, DREG_INTKEY, DREG_CYCLICINFOMSG, DREG_SABOTAGEMSG)
class SwList0 : public RegList0<Reg0> {
  public:
    SwList0(uint16_t addr) : RegList0<Reg0>(addr) {}
    void defaults() {
      clear();
      intKeyVisible(true);
      sabotageMsg(true);
      cycleInfoMsg(true);
    }
};


DEFREGISTER(Reg1, CREG_AES_ACTIVE, CREG_MSGFORPOS, CREG_EVENTDELAYTIME, CREG_LEDONTIME, CREG_TRANSMITTRYMAX)
class SensList1 : public RegList1<Reg1> {
  public:
    SensList1 (uint16_t addr) : RegList1<Reg1>(addr) {}
    void defaults () {
      clear();
      msgForPosA(1);
      msgForPosB(2);
      aesActive(false);
      eventDelaytime(0);
      ledOntime(100);
      transmitTryMax(6);
    }
};



typedef SwitchChannel<Hal, PEERS_PER_SwitchChannel, SwList0>  SwChannel;
typedef TwoStateChannel<Hal, SwList0, SensList1, DefList4, PEERS_PER_SENSCHANNEL> SensChannel;

// the class template derived_SwitchChannel is inherited from the class template SwitchChannel of AsksinnPP. Some overloading is done for the required new functionality.
template <class HalType,int PeerCount,class List0Type,class IODriver=ArduinoPins>
class derived_SwitchChannel : public SwitchChannel< HalType,PeerCount, List0Type, IODriver> {

enum { LOWACTIVE=0x08 };

protected:
   typedef ActorChannel<HalType,SwitchList1,SwitchList3,PeerCount,List0Type,SwitchStateMachine> BaseChannel;
   uint8_t pin;

   uint8_t supply_V_relais_by_battery_pin;
   uint8_t relay_pin_1,relay_pin_2,relay_pin_3,relay_pin_4;
   bool    status_relay_1,status_relay_2,status_relay_3,status_relay_4;
   bool    active_external_relay_output_flag;

public:
      static uint8_t state_AC_supply_relay_1;          // static variable that is shared between all instantiations of derived_SwitchChannel

public:

  void init (uint8_t p,bool value=false) {
    //DPRINTLN(F("derived_SwitchChannel init .."));
    pin=p;
    //DPRINT(F("Initializing relay pin : ")); DDEC( pin ); DPRINT(" "); DDECLN( value );
    IODriver::setOutput(pin);
    this->setflag(value,LOWACTIVE);
    typename BaseChannel::List1 l1 = BaseChannel::getList1();
    this->set(l1.powerUpAction() == true ? 200 : 0, 0, 0x0000 );  // 0x0000 defines power-up status of the relays
    this->changed(true);
    //DPRINTLN(F("Initializing of pin done "));
    supply_V_relais_by_battery_pin = V_RELAIS_BAT_SUPPLIED_PIN;
    DPRINT(F("Initializing V_relais control pin : ")); DDECLN( supply_V_relais_by_battery_pin );
    ArduinoPins::setOutput(supply_V_relais_by_battery_pin);  // set D3 as an output pin
    ArduinoPins::setHigh(supply_V_relais_by_battery_pin);    // low-active output, therefore the inactive state is High

    // charge_control_pin
    // put temporarily here until the charging class will be implemented later on
    ArduinoPins::setOutput(CHARGE_CONTROL_PIN);  // set A3 as an output pin
    ArduinoPins::setLow(CHARGE_CONTROL_PIN);     // low-active output, short pulse during init
    delay(400);                                  // short 400 ms pulse of the blue LED during startup
    ArduinoPins::setHigh(CHARGE_CONTROL_PIN);    // low-active output, therefore the inactive state is High
    ArduinoPins::setInput(CHARGE_CONTROL_PIN);   // set A3 as an input pin in order to prevent leakage currents through R9
    
    // define the Arduino control pins for the four relays
    relay_pin_1 = RELAY_PIN_1;
    relay_pin_2 = RELAY_PIN_2;
    relay_pin_3 = RELAY_PIN_3;
    relay_pin_4 = RELAY_PIN_4;

    PulseReset_V_RELAIS_BAT_SUPPLIED_PIN.init(supply_V_relais_by_battery_pin, false);
    PulseReset_CHARGE_CONTROL_PIN.init(CHARGE_CONTROL_PIN, true);                       // temporary solution
  }

  bool check_if_any_external_relay_output_is_active() {
    
     active_external_relay_output_flag = false;

     status_relay_1 = ArduinoPins::getState(relay_pin_1); 
     status_relay_2 = ArduinoPins::getState(relay_pin_2); 
     status_relay_3 = ArduinoPins::getState(relay_pin_3);
     status_relay_4 = ArduinoPins::getState(relay_pin_4);
    
     
     DPRINT(F("Relay 1 status : ")); DDECLN( status_relay_1 );
     DPRINT(F("Relay 2 status : ")); DDECLN( status_relay_2 );
     DPRINT(F("Relay 3 status : ")); DDECLN( status_relay_3 );
     DPRINT(F("Relay 4 status : ")); DDECLN( status_relay_4 );

     active_external_relay_output_flag = !(status_relay_2 && status_relay_3 && status_relay_4); // set true if at least one external relay output is active (active = 0 since low-active outputs)
                                                                                                // relay_pin_1 is not an external relay output ! AC power is an internal power switch only!
     
     DPRINT(F("active_external_relay_output_flag status : ")); DDECLN( active_external_relay_output_flag );
     
     return active_external_relay_output_flag;
  }

  void pulse_charging_of_accumulators () {
    ArduinoPins::setOutput(CHARGE_CONTROL_PIN);  // set A3 as an output pin
    ArduinoPins::setLow(CHARGE_CONTROL_PIN);     // low-active output, short pulse during init
    DPRINT(F("Charging accumulators for ")); DDEC( PULSE_WIDTH_CHARGE_CONTROL_PIN ); DPRINTLN(F(" milliseconds"));
    PulseReset_CHARGE_CONTROL_PIN.reset_pins(PULSE_WIDTH_CHARGE_CONTROL_PIN); // stop charging after PULSE_WIDTH_CHARGE_CONTROL_PIN milliseconds
  }

  virtual void switchState(__attribute__((unused)) uint8_t oldstate,uint8_t newstate,__attribute__((unused)) uint32_t tdelay) {

    DPRINT(F("\nWorking on relay pin : ")); DDECLN( pin );
    DPRINT(F("Old State : ")); DDECLN( oldstate );
    DPRINT(F("New State : ")); DDECLN( newstate );
    
    if ( ( newstate == AS_CM_JT_ON ) || ( newstate == AS_CM_JT_OFF ) ) {
      // set V_RELAIS_BAT_SUPPLIED_PIN to the active low state for a temporary supply of V_relais by the batteries
      DPRINTLN(F("Set V_RELAIS_BAT_SUPPLIED_PIN to active .."));
      ArduinoPins::setLow(supply_V_relais_by_battery_pin);  // active low output!
    }
   
    DPRINT(F("Switching relay pin : ")); DDEC( pin ); DPRINT(F(" ")); DDECLN( newstate );
    if( newstate == AS_CM_JT_ON ) {
      
      if( this->hasflag(LOWACTIVE) == true ) IODriver::setLow(pin);
      else IODriver::setHigh(pin);

      // switch on the AC supply relay for supplying the relays from now on
      DPRINTLN(F("Switching on AC supply relay .."));
      ArduinoPins::setLow(RELAY_PIN_1);

      // charge the accumulators for a pulse of PULSE_WIDTH_CHARGE_CONTROL_PIN milliseconds
      pulse_charging_of_accumulators();
    }
    else if ( newstate == AS_CM_JT_OFF ) {
      if( this->hasflag(LOWACTIVE) == true ) IODriver::setHigh(pin);
      else IODriver::setLow(pin);
    }

    // save the current status of the AC power relay at pin RELAY_PIN_1 in the static variable RELAY_PIN_1; this variable caan be read by the other derived_SwitchChannel instantiations
    if ( pin == relay_pin_1 ) {
      DPRINT(F("Updating  AC power state 'state_AC_supply_relay_1' : ")); DDECLN( newstate );
      state_AC_supply_relay_1 = newstate;   // store the new state of AC supply relay at pin RELAY_PIN_1 in static variable state_AC_supply_relay_1
    }

    if ( ( newstate == AS_CM_JT_ON ) || ( newstate == AS_CM_JT_OFF ) ) {
      DPRINT(F("delay for ")); DDEC( PULSE_WIDTH_V_RELAIS_BAT_SUPPLIED_PIN ); DPRINTLN(F(" ms at V_RELAIS_BAT_SUPPLIED_PIN"));
      PulseReset_V_RELAIS_BAT_SUPPLIED_PIN.reset_pins(PULSE_WIDTH_V_RELAIS_BAT_SUPPLIED_PIN); // reset V_RELAIS_BAT_SUPPLIED_PIN to inactive state after a PULSE_WIDTH_V_RELAIS_BAT_SUPPLIED_PIN miliseconds delay in order to get main DC supply switched on and stabilized
    }
    
    // check status of all external relays
    active_external_relay_output_flag = check_if_any_external_relay_output_is_active();
    
    if ( ! active_external_relay_output_flag) {
      // switch off the AC suppy relay if no external relay output is active and ( state_AC_supply_relay_1 == AS_CM_JT_OFF )
      if (state_AC_supply_relay_1 == AS_CM_JT_OFF) {
        DPRINTLN(F("Switching off AC supply relay .."));
        ArduinoPins::setHigh(RELAY_PIN_1); // reset state of AC supply to the 'off'-state: active low-output! only if ( state_AC_supply_relay_1 == AS_CM_JT_OFF )
      }
      else {
        DPRINTLN(F("Keep state oof  AC supply relay unchanged"));
      }
      
    }
 
    this->changed(true);
  }
};

typedef derived_SwitchChannel<Hal, PEERS_PER_SwitchChannel, SwList0>  derived_SwChannel;

class MixDevice : public ChannelDevice<Hal, VirtBaseChannel<Hal, SwList0>, 8, SwList0> {  // derived class
    class CycleInfoAlarm : public Alarm {
        MixDevice& dev;
      public:
        CycleInfoAlarm (MixDevice& d) : Alarm (CYCLETIME), dev(d) {}
        virtual ~CycleInfoAlarm () {}

        void trigger (AlarmClock& clock)  {
          set(CYCLETIME);
          clock.add(*this);
          dev.derived_switchChannel(1).changed(true);
          DPRINT(F("accumulator voltage(MCU ADC) (x1000.0) [mV] : "));
          dev.battery().update();
          DDECLN(dev.battery().current());    // BatteryTM class, mV resolution
        }
    } cycle;

    
  public:
    VirtChannel<Hal, derived_SwChannel, SwList0>   swChannel1,   swChannel2,   swChannel3,   swChannel4;
    VirtChannel<Hal, SensChannel, SwList0> sensChannel5, sensChannel6, sensChannel7, sensChannel8;
  public:
    typedef ChannelDevice<Hal, VirtBaseChannel<Hal, SwList0>, 8, SwList0> DeviceType;
    // constructor
    MixDevice (const DeviceInfo& info, uint16_t addr) : DeviceType(info, addr), cycle(*this) {
      DeviceType::registerChannel(swChannel1, 1);
      DeviceType::registerChannel(swChannel2, 2);
      DeviceType::registerChannel(swChannel3, 3);
      DeviceType::registerChannel(swChannel4, 4);

      DeviceType::registerChannel(sensChannel5, 5);
      DeviceType::registerChannel(sensChannel6, 6);
      DeviceType::registerChannel(sensChannel7, 7);
      DeviceType::registerChannel(sensChannel8, 8);
    }
    // destructor
    virtual ~MixDevice () {}


    derived_SwChannel& derived_switchChannel (uint8_t num)  {
      switch (num) {
        case 1:
          return swChannel1;
          break;
        case 2:
          return swChannel2;
          break;
        case 3:
          return swChannel3;
          break;
        case 4:
          return swChannel4;
          break;
        default:
          return swChannel1;
          break;
      }
    }

    SensChannel& sensorChannel (uint8_t num)  {
      switch (num) {
        case 5:
          return sensChannel5;
          break;
        case 6:
          return sensChannel6;
          break;
        case 7:
          return sensChannel7;
          break;
        case 8:
          return sensChannel8;
          break;
        default:
          return sensChannel5;
          break;
      }
    }

    virtual void configChanged () {
      if ( this->getList0().cycleInfoMsg() == true ) {
        DPRINTLN(F("Activate Cycle Msg"));
        sysclock.cancel(cycle);
        cycle.set(CYCLETIME);
        sysclock.add(cycle);
      }
      else {
        DPRINTLN(F("Deactivate Cycle Msg"));
        sysclock.cancel(cycle);
      }
    }
};

MixDevice sdev(devinfo, 0x20);
ConfigButton<MixDevice> cfgBtn(sdev);

template <class HalType,int PeerCount,class List0Type,class IODriver>                                     // <-------- ALLOCATE MEMORY FOR THE STATIC VARIABLE - see also https://stackoverflow.com/questions/17071130/default-argument-for-template-parameter-for-class-enclosing
uint8_t derived_SwitchChannel< HalType, PeerCount, List0Type, IODriver>::state_AC_supply_relay_1;         // <-------- ALLOCATE MEMORY FOR THE STATIC VARIABLE -


void initPeerings (bool first) {
  // create internal peerings - CCU2 needs this
  if ( first == true ) {
#ifdef CREATE_INTERNAL_PEERINGS    
    HMID devid;
    sdev.getDeviceID(devid);
    DPRINTLN(F("Internal Peering .."));
    for ( uint8_t i = 1; i <= 4; ++i ) {
      Peer ipeer(devid, i + 4);
      sdev.derived_switchChannel(i).peer(ipeer);
    }
    for ( uint8_t i = 1; i <= 4; ++i ) {
      Peer ipeer(devid, i);
      sdev.sensorChannel(i + 4).peer(ipeer);
    }
#endif    
  }
}

void setup () {
  
  DINIT(57600, ASKSIN_PLUS_PLUS_IDENTIFIER);  // set baud rate of serial monitor
  
  bool first = sdev.init(hal);
  //first=true;    // comment out for only one (!) programming if the peering is not working as expected, e.g. if after you have changed the pin assignment
  
  DPRINTLN(F("Init HAL .."));
  
  sdev.derived_switchChannel(1).init(RELAY_PIN_1, true);   // inverse polarity see https://homematic-forum.de/forum/viewtopic.php?t=53687
  sdev.derived_switchChannel(2).init(RELAY_PIN_2, true);
  sdev.derived_switchChannel(3).init(RELAY_PIN_3, true);
  sdev.derived_switchChannel(4).init(RELAY_PIN_4, true);
  
  sdev.sensorChannel(5).init(SENS_PIN_1, SABOTAGE_PIN_1);  // sabotage pin are disabled; see above
  sdev.sensorChannel(6).init(SENS_PIN_2, SABOTAGE_PIN_1);
  sdev.sensorChannel(7).init(SENS_PIN_3, SABOTAGE_PIN_1);
  sdev.sensorChannel(8).init(SENS_PIN_4, SABOTAGE_PIN_1);

  DPRINTLN(F("Init HAL done"));
  
  buttonISR(cfgBtn, CONFIG_BUTTON_PIN);

 
  sdev.derived_switchChannel(RELAY_PIN_1).switchState(AS_CM_JT_ON, AS_CM_JT_OFF, 0);  // switch off the relay as initial state
  sdev.derived_switchChannel(RELAY_PIN_2).switchState(AS_CM_JT_ON, AS_CM_JT_OFF, 0);  // switch off the relay as initial state
  sdev.derived_switchChannel(RELAY_PIN_3).switchState(AS_CM_JT_ON, AS_CM_JT_OFF, 0);  // switch off the relay as initial state
  sdev.derived_switchChannel(RELAY_PIN_4).switchState(AS_CM_JT_ON, AS_CM_JT_OFF, 0);  // switch off the relay as initial state
 

  sdev.radio().initReg(CC1101_PATABLE, PA_Normal);                                    // normal RF transmission power of CC1101, PA_Normal = 0x50, can be set to PA_LowPower or PA_MaxPower; see Radio.h

  initPeerings(first);
  // stay on for 15 seconds after start
  hal.activity.stayAwake(seconds2ticks(15));
  hal.battery.low(BAT_VOLT_LOW);
  hal.battery.critical(BAT_VOLT_CRITICAL);
  // measure battery every 12 hours
  hal.battery.init(seconds2ticks(CYCLETIME), sysclock);

  DPRINT(F("accumulator voltage(MCU ADC) (x1000.0) [mV] : "));
  sdev.battery().update();
  DDECLN(sdev.battery().current());    // BatteryTM class, mV resolution
  
  sdev.initDone();
}

void loop() {
  bool worked = hal.runready();
  bool poll   = sdev.pollRadio();
  if ( worked == false && poll == false ) {
    hal.activity.savePower<Sleep<> >(hal);

  }
}
