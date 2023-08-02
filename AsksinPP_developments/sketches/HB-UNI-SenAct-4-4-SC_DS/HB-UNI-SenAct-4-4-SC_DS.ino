//- -----------------------------------------------------------------------------------------------------------------------
// AskSin++
// 2016-10-31 papa Creative Commons - http://creativecommons.org/licenses/by-nc-sa/3.0/de/
// 2018-08-13 jp112sdl Creative Commons - http://creativecommons.org/licenses/by-nc-sa/3.0/de/
// 2023-07-30 FUEL4EP adaptions to PCB HB-UNI-SenAct-4-4-SC_DS_FUEL4EP 
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

//---------------------------------------------------------
// IMPORTANT NOTE: switch on NDEBUG in production phase after havin finished all bring-up tests.
//
//#define NDEBUG
//#define USE_CC1101_ALT_FREQ_86835  //use alternative frequency to compensate not correct working cc1101 modules

//---------------------------------------------------------


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

#define CHARGE_CONTROL_PIN         A3          // control pin for charging the accumulators, active low, the charging software is not yet implemented, please charge the accumulator batteries manually for the time being!
#define MAX_ACCUMULATOR_VOLTAGE    5100        // limit the accumulators voltage: charging is disabled if the accumulator voltage is above this threshold

#define V_RELAIS_BAT_SUPPLIED_PIN  3           // D3
#define PULSE_WIDTH_V_RELAIS_BAT_SUPPLIED_PIN  millis2ticks(900UL)                // pulse width at V_RELAIS_BAT_SUPPLIED_PIN, needs to be long enough to switch the AC power relay and to power up the main DC power
#define PULSE_WIDTH_CHARGE_CONTROL_PIN         seconds2ticks(60UL * 20 * 0.88)    // pulse width at CHARGE_CONTROL_PIN for an intermediate charging of the accumulator batteris after having switched on a relay output (20 minutes)
                                                                                  // 60 seconds * 20 (= minutes) * corrective factor
#define LONG_PULSE_WIDTH_CHARGE_CONTROL_PIN    seconds2ticks(60UL * 240 * 0.88)   // long pulse width at CHARGE_CONTROL_PIN for an intermediate charging of the accumulator batteris after having switched on a relay output (240 minutes = 4 hours), total charge of accumulators is about 1/4 * C
                                                                                  // 60 seconds * 240 (= minutes) * corrective factor

                                                                                  
#define SABOTAGE_PIN_1    false                // disable SABOTAGE_PIN false=0

#define LED_PIN                  4
#define CONFIG_BUTTON_PIN        8

// number of available peers per channel
#define CREATE_INTERNAL_PEERINGS  // needs to be set in order to create the internal peerings
#define PEERS_PER_SwitchChannel  6
#define PEERS_PER_SENSCHANNEL    6

#define DEV_MODEL 0x0E
#define CYCLETIME seconds2ticks(60UL * 60 * 12 * 0.88) // 60 seconds * 60 (= minutes) * 12 (=hours) * corrective factor

//#define  BatterySensor
#define BAT_SENSOR tmBatteryResDiv<A6, A2, 5700>   //see tmbattery.h; measurement of accumulator voltage; sense pin = A6; activationn pin = A2; Rhigh = 470kOhm; Rlow = 100kOhm;  Vinmax=6.27V


// all library classes are placed in the name-space 'as'
using namespace as;

PulseReset PulseReset_V_RELAIS_BAT_SUPPLIED_PIN;         // see PulseReset.h
PulseReset PulseReset_CHARGE_CONTROL_PIN;                // see PulseReset.h

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

template <uint8_t CS,uint8_t MOSI,uint8_t MISO,uint8_t SCLK, class PINTYPE=ArduinoPins,bool SCKFloatOnIdle=false>
class derived_AvrSPI : public AvrSPI< CS, MOSI, MISO, SCLK, PINTYPE> {

public:

  void select () {
    if( SCKFloatOnIdle==true) {
      PINTYPE::setOutput(SCLK);
    }
    PINTYPE::setLow(CS);
  }

  void deselect () {
    PINTYPE::setHigh(CS);
    if( SCKFloatOnIdle==true) {
      PINTYPE::setInput(SCLK);
    }
  }

};


/**
   Configure the used hardware
*/
//typedef AvrSPI<10, 11, 12, 13> RadioSPI;
typedef derived_AvrSPI<10, 11, 12, 13, ArduinoPins, true> RadioSPI;  // see https://homematic-forum.de/forum/viewtopic.php?f=76&t=71788&hilit=SCKFloatOnIdle&start=30
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

   bool    active_external_relay_output_flag;

public:
   static uint8_t state_AC_supply_relay_1;          // static variable that is shared between all instantiations of derived_SwitchChannel
   static bool    current_relay_states[4];          // static array that iss conserving the relay states and sharing the relays' status between all instantiations of derived_SwitchChannel
      
public:

  void init (uint8_t p,bool value=false) {

    PulseReset_V_RELAIS_BAT_SUPPLIED_PIN.init(V_RELAIS_BAT_SUPPLIED_PIN, false);
    PulseReset_CHARGE_CONTROL_PIN.init(CHARGE_CONTROL_PIN, true);                       // temporary solution as long as the charge control software is not yet implemented
    
    pin=p;
    IODriver::setOutput(pin);
    this->setflag(value,LOWACTIVE);
    typename BaseChannel::List1 l1 = BaseChannel::getList1();
    this->set(l1.powerUpAction() == true ? 200 : 0, 0, 0x0000 );  // 0x0000 defines power-up status of the relays
    this->changed(true);
    IODriver::setOutput(V_RELAIS_BAT_SUPPLIED_PIN);  // set D3 as an output pin
    IODriver::setHigh(V_RELAIS_BAT_SUPPLIED_PIN);    // low-active output, therefore the inactive state is High

    // charge_control_pin
    // put temporarily here until the charging class will be implemented later on
    IODriver::setOutput(CHARGE_CONTROL_PIN);  // set A3 as an output pin
    IODriver::setLow(CHARGE_CONTROL_PIN);     // low-active output, short pulse during init
    delay(200);                               // short 200 ms pulse of the blue LED during startup
    IODriver::setHigh(CHARGE_CONTROL_PIN);    // low-active output, therefore the inactive state is High
    IODriver::setInput(CHARGE_CONTROL_PIN);   // set A3 as an input pin in order to prevent leakage currents through R9

  }


  void store_relay_state_in_static_array ( uint8_t p, uint8_t newstate ) {
    switch(p) {
      case RELAY_PIN_1:
        current_relay_states[0] =  newstate == AS_CM_JT_ON ? true : false ;  // set to true if newstate is equal to AS_CM_JT_ON
        break;
      case RELAY_PIN_2:
        current_relay_states[1]  = newstate == AS_CM_JT_ON ? true : false ;
        break;
      case RELAY_PIN_3:
        current_relay_states[2]  = newstate == AS_CM_JT_ON ? true : false ;
        break;
      case RELAY_PIN_4:
        current_relay_states[3]  = newstate == AS_CM_JT_ON ? true : false ;
        break;
      default:
        // wrong pin number; issue error message in serial monitor
        //DPRINT(F("Wrong pin in store_relay_state_in_static_array : ")); DDEC( pin ); DPRINTLN(F(" !"));
        break;
    }
  }
  

  bool check_if_any_external_relay_output_is_active() {
    
    active_external_relay_output_flag = false;
    active_external_relay_output_flag = (current_relay_states[1] || current_relay_states[2] || current_relay_states[3]); // set true if at least one external relay output is active (state == AS_CM_JT_ON)                                                                                                                         // current_relay_states[0] is not an external relay output of the hardware ! AC power is an internal power switch only
    return active_external_relay_output_flag;
  }

  void pulse_charging_of_accumulators () {
    IODriver::setOutput(CHARGE_CONTROL_PIN);  // set A3 as an output pin
    IODriver::setLow(CHARGE_CONTROL_PIN);     // low-active output, short pulse during init
    PulseReset_CHARGE_CONTROL_PIN.reset_pins(PULSE_WIDTH_CHARGE_CONTROL_PIN); // stop charging after PULSE_WIDTH_CHARGE_CONTROL_PIN ticks
  }

  virtual void switchState(__attribute__((unused)) uint8_t oldstate,uint8_t newstate,__attribute__((unused)) uint32_t tdelay) {

    /*DPRINT(F("\nWorking on relay pin : ")); DDECLN( pin );
    DPRINT(F("Old State : ")); DDECLN( oldstate );
    DPRINT(F("New State : ")); DDECLN( newstate );
    if ( newstate == AS_CM_JT_ON ) {
       DPRINTLN(F("New State : ON")); 
    } else
    if ( newstate == AS_CM_JT_OFF ) {
       DPRINTLN(F("New State : OFF")); 
    }
    else {
       DPRINTLN(F("New State : OTHER")); 
    }
    
    */
    if ( ( newstate == AS_CM_JT_ON ) || ( newstate == AS_CM_JT_OFF ) ) {

      store_relay_state_in_static_array(pin, newstate);  // store new state of channel in the shared static array 'current_relay_states'
      
      // set V_RELAIS_BAT_SUPPLIED_PIN to the active low state for a temporary supply of V_relais by the batteries
      IODriver::setOutput(V_RELAIS_BAT_SUPPLIED_PIN);  // set D3 as an output pin
      IODriver::setLow(V_RELAIS_BAT_SUPPLIED_PIN);     // active low output!
    }
   
    if( newstate == AS_CM_JT_ON ) {
      IODriver::setOutput(pin); // define pin as output (again)
      if( this->hasflag(LOWACTIVE) == true ) IODriver::setLow(pin);
      else IODriver::setHigh(pin);

      // switch on the AC supply relay for supplying the relays from now on
      IODriver::setOutput(RELAY_PIN_1);
      IODriver::setLow(RELAY_PIN_1);

      // charge the accumulators for a pulse of PULSE_WIDTH_CHARGE_CONTROL_PIN ticks
      pulse_charging_of_accumulators();
    }
    else if ( newstate == AS_CM_JT_OFF ) {
      IODriver::setOutput(pin); // define pin as output (again)
      if( this->hasflag(LOWACTIVE) == true ) {
        IODriver::setHigh(pin);
        IODriver::setInput(pin);  // for avoiding any leakage
      }
      else IODriver::setLow(pin);
    }

    // save the current status of the AC power relay at pin RELAY_PIN_1 in the static variable RELAY_PIN_1; this variable caan be read by the other derived_SwitchChannel instantiations
    if ( pin == RELAY_PIN_1 ) {
      state_AC_supply_relay_1 = newstate;   // store the new state of AC supply relay at pin RELAY_PIN_1 in static variable state_AC_supply_relay_1
    }

    if ( ( newstate == AS_CM_JT_ON ) || ( newstate == AS_CM_JT_OFF ) ) {
      PulseReset_V_RELAIS_BAT_SUPPLIED_PIN.reset_pins(PULSE_WIDTH_V_RELAIS_BAT_SUPPLIED_PIN); // reset V_RELAIS_BAT_SUPPLIED_PIN to inactive state after a PULSE_WIDTH_V_RELAIS_BAT_SUPPLIED_PIN miliseconds delay in order to get main DC supply switched on and stabilized
    }
    
    // check status of all external relays
    active_external_relay_output_flag = check_if_any_external_relay_output_is_active();
    
    if ( ! active_external_relay_output_flag) {
      // switch off the AC suppy relay if no external relay output is active and ( state_AC_supply_relay_1 == AS_CM_JT_OFF )
      if (state_AC_supply_relay_1 == AS_CM_JT_OFF) {
        IODriver::setOutput(RELAY_PIN_1);
        IODriver::setHigh(RELAY_PIN_1); // reset state of AC supply to the 'off'-state: active low-output! only if ( state_AC_supply_relay_1 == AS_CM_JT_OFF )
        IODriver::setInput(RELAY_PIN_1); 
      }
      else {
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
          uint16_t        accumulator_voltage;
          set(CYCLETIME);
          clock.add(*this);
          dev.derived_switchChannel(1).changed(true);
          DPRINT(F("accumulator voltage(MCU ADC) (x1000.0) [mV] : "));
          accumulator_voltage = dev.battery().current();
          DDECLN(accumulator_voltage);    // BatteryTM class, mV resolution
          if ( accumulator_voltage <= BAT_VOLT_LOW * 100 ) {
             dev.derived_switchChannel(RELAY_PIN_1).switchState(AS_CM_JT_OFF, AS_CM_JT_ON, 0);  // switch on AC power supply
             ArduinoPins::setOutput(CHARGE_CONTROL_PIN);  // set A3 as an output pin
             ArduinoPins::setLow(CHARGE_CONTROL_PIN);     // low-active output, short pulse during init
             PulseReset_CHARGE_CONTROL_PIN.reset_pins(LONG_PULSE_WIDTH_CHARGE_CONTROL_PIN); // stop charging after PLONG_ULSE_WIDTH_CHARGE_CONTROL_PIN ticks
          }
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
template <class HalType,int PeerCount,class List0Type,class IODriver>                                                                  // <-------- ALLOCATE MEMORY FOR THE STATIC VARIABLE - see also https://stackoverflow.com/questions/17071130/default-argument-for-template-parameter-for-class-enclosing
bool derived_SwitchChannel< HalType, PeerCount, List0Type, IODriver>::current_relay_states[4] = {false, false, false, false};          // <-------- ALLOCATE MEMORY FOR THE STATIC VARIABLE -



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
