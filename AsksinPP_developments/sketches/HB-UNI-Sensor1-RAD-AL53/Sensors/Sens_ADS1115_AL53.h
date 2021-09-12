//---------------------------------------------------------
// Sens_ADS1115_AL53
// 2021-06-05 FUELEP (Creative Commons
// https://creativecommons.org/licenses/by-nc-sa/4.0/
// You are free to Share & Adapt under the following terms:
// Give Credit, NonCommercial, ShareAlike
// +++
// AskSin++ 2016-10-31 papa Creative Commons
//---------------------------------------------------------

#ifndef _SENS_ADS1115_H_
#define _SENS_ADS1115_H_

#define LOWBAT_2x_NiMH_BATTERIES       2200       // mV; low bat threshold for NiMH battery = 1100 mV
#define ADC_PIN_ADS1115_AL53_SUPPLY    0          // ADS1115 ADC input pin A0 for measuring the AL53 supply voltage
#define ADC_PIN_ADS1115_VCC            1          // ADS1115 ADC input pin A1 for measuring the VCC voltage of the MCU
#define ADC_PIN_ADS1115_ACCUMULATOR    2          // ADS1115 ADC input pin A2 for measuring the accumulator voltage

#include <Adafruit_ADS1X15.h>

//#define DEEP_DEBUG // comment out if deep serial monitor debugging is not necessary


namespace as {

class Sens_ADS1115 {

  uint16_t   _AL53_supply_voltage;
  uint16_t   _VCC_operating_voltage;
  uint16_t   _accumulator_voltage;

    
private:
    
  Adafruit_ADS1115 ads;  /* Use this for the 16-bit version */
  
  // ADC adjustment factors per channel
  const float       ADC0_FACTOR = 2220.0 / 220.0 * 0.0625 * 9.10 / 8.859 ;  // 2200.0 / 220.0 is the uncorrected voltage divider ratio; 0.0625 is the ADS115 ADC resolution for the selected gain of TWO
                                                                            // 8.98 / 8.859 is the voltage divider correction factor for ADCO0 based on a multimeter comparison
  const float       ADC1_FACTOR = 2.0 * 0.0625 * 3.314 / 3.292 ;            // 2 .0is the uncorrected volate divider ratio; 0.0625 is the ADS115 ADC resolution for the selected gain of TWO
                                                                            // 3.314 / 3.292 is the voltage divider correction factor for ADC1 based on a multimeter comparison
  const float       ADC2_FACTOR = 2.0 * 0.0625 * 3.227 / 3.172 ;            // 2.0 is the uncorrected volate divider ratio; 0.0625 is the ADS115 ADC resolution for the selected gain of TWO
                                                                            // 3.227 / 3.172 is the voltage divider correction factor for ADC2 based on a multimeter comparison
                                                              
  int16_t adc0_AL53,  adc1_VCC,   adc2_ACCU;
  float adc0_AL53_f,  adc1_VCC_f, adc2_ACCU_f;

public:
    //constructor
    Sens_ADS1115()
        :
          _AL53_supply_voltage(0),
          _VCC_operating_voltage(0),
          _accumulator_voltage(0)

    {
    }

    void init(void)
    {
        Serial.println("ADS1115 init");
        ads.begin();
        ads.setGain(GAIN_TWO);        // 2x gain   +/- 2.048V  1 bit = 1mV      0.0625mV
        ads.begin();
    }

    void measure()
    {
       adc0_AL53  = ads.readADC_SingleEnded(ADC_PIN_ADS1115_AL53_SUPPLY);
       adc1_VCC   = ads.readADC_SingleEnded(ADC_PIN_ADS1115_VCC);
       adc2_ACCU  = ads.readADC_SingleEnded(ADC_PIN_ADS1115_ACCUMULATOR);

       adc0_AL53_f  = (float)adc0_AL53 *  ADC0_FACTOR;
       adc1_VCC_f   = (float)adc1_VCC  *  ADC1_FACTOR;
       adc2_ACCU_f  = (float)adc2_ACCU *  ADC2_FACTOR;
       
       _AL53_supply_voltage       = (uint16_t)adc0_AL53_f;   
       _VCC_operating_voltage     = (uint16_t)adc1_VCC_f;    
       _accumulator_voltage       = (uint16_t)adc2_ACCU_f;  

    }
    
    uint16_t  AL53_supply_voltage()        { return _AL53_supply_voltage;     }
    uint16_t  VCC_operating_voltage()      { return _VCC_operating_voltage; }
    uint16_t  accumulator_voltage()        { return _accumulator_voltage;   }
    
};

}

#endif
 
