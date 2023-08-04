#ifndef __PULSERESET_H__
#define __PULSERESET_H__

#include <AlarmClock.h>

// derived from https://homematic-forum.de/forum/viewtopic.php?t=71702
// thanks to OliR for posting his nice solution

namespace as
{

  class PulseReset : public Alarm
  {
  private:
    uint8_t control_pin;
    bool    high_ohmic_flag;

  public:
    PulseReset () : Alarm(0)
    {
      async(true);
    }
    virtual ~PulseReset () {}

    virtual void trigger (__attribute__ ((unused)) AlarmClock& clock)
    {
      DPRINT(F("Switching pin delayed back to inactive state: ")); DDECLN( control_pin );
      digitalWrite(control_pin,HIGH);
      if ( high_ohmic_flag ) {
        pinMode(control_pin,INPUT);
      }
    }

    void init (uint8_t c_in, bool set_to_input_flag)
    {
      control_pin     = c_in;
      high_ohmic_flag = set_to_input_flag;   // if high_ohmic_flag is set, the pin is configured as input pin at the end of the pulse
    }

    void reset_pins(uint32_t t)
    {
      sysclock.cancel(*this);
      set(t);
      sysclock.add(*this);
    }
    
  };
}

#endif

