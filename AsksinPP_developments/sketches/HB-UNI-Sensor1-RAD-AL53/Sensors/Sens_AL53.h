//---------------------------------------------------------
// Sens_AL53
// 2021-06-03 FUELEP
// https://creativecommons.org/licenses/by-nc-sa/4.0/
// You are free to Share & Adapt under the following terms:
// Give Credit, NonCommercial, ShareAlike
// +++
// AskSin++ 2016-10-31 papa Creative Commons
// 2019-07-14 Tom Major (Creative Commons)
//---------------------------------------------------------

#ifndef _SENS_AL53_H_
#define _SENS_AL53_H_

#include <limits.h>
#include "my_Sensors_AL53.h"
#include <ABLIC_S35770.h>  // download the ABLIC S-35770 libary from https://github.com/FUEL4EP/HomeAutomation, the library for the ABLIC S-35770 I2C counter is at <HomeAutomation_dir>/AsksinPP_developments/libraries/ABLIC_S35770
                           // copy this directory to your Arduino librarry directory

#define DEEP_DEBUG // comment out if deep serial monitor debugging is not necessary
#include "Sens_rad_statistics.h"
#include "Sens_ADS1115_AL53.h"

namespace as {

class Sens_AL53 : public Sensor {

    uint16_t     _counts_per_measurement_interval;
    uint16_t     _moving_average_count;
    uint16_t     _width_confidence_interval;
    uint16_t     _moving_average_count_max;
    uint16_t     _counts_per_measurement_interval_max;
    uint16_t     _accumulator_voltage;
    uint16_t     _AL53_supply_voltage;
    uint16_t     _operating_voltage;
    bool         _alarm_count_per_measurement_interval;
    bool         _alarm_moving_average;
    uint16_t     _alarm_level_counts_per_measurement_interval;
    uint16_t     _alarm_level_moving_average;
    
    
    uint32_t     _count_index;

    
    // This sketch is assuming the following hardware connections between an S-35770 IC and an Pro Mini XL - v2 - ATmega 1284p from Tindie
    // Datasheet S-35570: https://www.ablic.com/en/doc/datasheet/counter_ic/S35770_I_E.pdf
    // Pro Mini XL - v2 - ATmega 1284p, please have a look  at https://www.tindie.com/products/prominimicros/pro-mini-xl-v2-atmega-1284p/
    //
    // If your hardware configuration is different, please modify the example sketch appropriately to fit to your hardware setup. 
    //
    // The Pro Mini XL - v2 - ATmega 1284p from Tindie requires to install https://github.com/MCUdude/MightyCore
    // Enter the following URL in Additional Boards Manager URLs:  https://mcudude.github.io/MightyCore/package_MCUdude_MightyCore_index.json
    //
    //
    //  _______________________________________________________               _________________________________________
    //                                                         |              |
    //    Pro Mini XL - v2 - ATmega 1284p from Tindie          |              |   ABLIC S-35770
    //                                                         |              |
    //                                       IDE pin 15 (PD7)  |--------------| 1 (RSTB)
    //                                                         |              |
    //                                       IDE pin 19 (PC3)  |--------------| 3 (CLKIN)
    //                                                         |              |
    // ________________________________________________________|              |________________________________________
    //                                                                       

    ABLIC_S35770 ABLIC_S35770_counter{15, 19}; // example hardware connection 1: attach the RSTB  pin of S-35770 IC to IDE pin 15 (PD7) of Pro Mini XL - v2 - ATmega 1284p (pin 7 on Tindie PCB board)
                                               // example hardware connection 2: attach the CLKIN pin of S-35770 IC to IDE pin 19 (PC3) of Pro Mini XL - v2 - ATmega 1284p (pin 9 on Tindie PCB board)
                                               // see https://www.tindie.com/products/prominimicros/pro-mini-xl-v2-atmega-1284p

    
    rad_statistics AL53_rad_statistics;
    
    Sens_ADS1115 ADS115_ADC;
    


public:
    Sens_AL53()
        : _counts_per_measurement_interval(0),
          _moving_average_count(0),
          _width_confidence_interval(0),
          _moving_average_count_max(0),
          _counts_per_measurement_interval_max(0),
          _accumulator_voltage(0),
          _AL53_supply_voltage(0),
          _operating_voltage(0),
          _alarm_count_per_measurement_interval(0),
          _alarm_moving_average(0),
          _alarm_level_counts_per_measurement_interval(65535),
          _alarm_level_moving_average(65535),
          _count_index(0)
        
    {
    }

    void init( uint16_t alarm_level_counts_per_measurement_interval, uint16_t alarm_level_moving_average )
    {
        
        DPRINTLN(F("\n\nWelcome to HB-UNI-Sensor1-RAD-AL53 (C) 2021 FUEL4EP (Creative Commons)\n\n"));
        
        DPRINTLN(F("\n\nIMPORTANT: Remove accumulator batteries during debugging with a FTDI Debugger in the serial monitor or a programming with an ISP Programmer"));
        DPRINTLN(F("           This avoids a deep discharge of them by the boost converter\n\n"));
        
        
        
        DPRINTLN(F("initializing ABLIC S35770 counter"));
        ABLIC_S35770_counter.begin();
        
        DPRINTLN(F("initializing AL53 radiation statistics data base"));
        AL53_rad_statistics.clear_buffer();
        
        DPRINTLN(F("initializing ADS1115 ADC"));
        ADS115_ADC.init();
        
        while (ABLIC_S35770_counter.check(0x32) != true)  // S-35770 IC's I2C slave address is hard wired as 0x32
        {
          DPRINTLN(F("ABLIC S-35770 counter IC is not connected !"));
          DPRINTLN(F("Please check your wiring ...\n"));
          delay(1000);
        }
  
        DPRINTLN(F("ABLIC S-35770 counter IC is accessible"));
        
        _present = true;
        
        ABLIC_S35770_counter.counter_signal_off(); // PC3, IDE pin 19 set to 0
        ABLIC_S35770_counter.soft_reset_counter();
        // tie reset signal of ABLIC_S35770, this is stopping the counter
        ABLIC_S35770_counter.reset_counter_and_stop_counting();
        
        _counts_per_measurement_interval          = 0;
        _moving_average_count                     = 0;
        _width_confidence_interval                = 0;
        _moving_average_count_max                 = 0;
        _counts_per_measurement_interval_max      = 0;
        _accumulator_voltage                      = 0;
        
        // initialize alarm levels 
        _alarm_level_counts_per_measurement_interval = alarm_level_counts_per_measurement_interval;
        _alarm_level_moving_average                  = alarm_level_moving_average;
        
        DPRINTLN(F("ADS1115 measurement .."));
        ADS115_ADC.measure();
        
        _AL53_supply_voltage                      = ADS115_ADC.AL53_supply_voltage();
        _operating_voltage                        = ADS115_ADC.VCC_operating_voltage();
        _accumulator_voltage                      = ADS115_ADC.accumulator_voltage();
        
        DPRINT(F("AL53 supply voltage (x1000.0)                                                            : "));
        DDECLN(_AL53_supply_voltage); 
        DPRINT(F("VCC supply voltage (x1000.0)                                                             : "));
        DDECLN(_operating_voltage);
        DPRINT(F("accumulator batteries voltage (x1000.0)                                                  : "));
        DDECLN(_accumulator_voltage);
        DPRINT(F("alarm level counts per measurement interval                                              : "));
        DDECLN(_alarm_level_counts_per_measurement_interval); 
        DPRINT(F("alarm level moving average  (x1000.0)                                                    : "));
        DDECLN(_alarm_level_moving_average); 

        
        // wait 1 second for settling
        delay(1000);
        
        ABLIC_S35770_counter.counter_signal_off(); // PC3, IDE pin 19 set to 0
        ABLIC_S35770_counter.reset_counter_and_stop_counting();
        
        // execute a test count on ABLIC S35770 counter
        
        // enable the counting of the ABLIC S35770 counter
        ABLIC_S35770_counter.enable_counting();
        
        int test_count = 16384;
        DPRINT(F("creating fixed number of test pulses at pin PC3                                          : ")); 
        DDECLN(test_count);
        // create test pulses at pin PC3 (pin 19)
        ABLIC_S35770_counter.create_fixed_number_of_pulse_at_counter_input_pin(test_count);
        // read the ABLIC S35770 count register
        DPRINT(F("read count register                                                                      : "));
        uint16_t test_read_count = ABLIC_S35770_counter.getCount();
        ABLIC_S35770_counter.reset_counter_and_stop_counting();
        DDECLN(test_read_count);
        DPRINT(F("check if the count register result was                                                   : "));
        DDECLN(test_count);    
        if ( test_read_count != (uint16_t)test_count ) {
          DPRINTLN(F("\n\n\nERROR: Test read count is wrong!!! Please check your hardware!\n\n\n")); 
        }
        
        ABLIC_S35770_counter.counter_signal_off(); // PC3, IDE pin 19 set to 0
        ABLIC_S35770_counter.enable_counting();
        
    }

    void measure()
    {
        if (_present == true) {
            
#ifdef DEEP_DEBUG    
            unsigned long MeasureStartTime = millis();
#endif
          
            _counts_per_measurement_interval = ABLIC_S35770_counter.getCount();
            
            ABLIC_S35770_counter.reset_counter_and_stop_counting();
            
            // add current measurement count to the circular buffer and update the statistics
            _count_index = AL53_rad_statistics.add_count( _counts_per_measurement_interval );
            
            DPRINT(F("\nindex of measurement interval                                                            : "));
            DDECLN(_count_index);
            
            DPRINT(F("number of radiation event counts per measurement interval                                : "));
            DDEC(_counts_per_measurement_interval);
            DPRINTLN(F("  <==="));
            
            _moving_average_count                = (uint16_t) constrain(( AL53_rad_statistics.get_moving_average() *100.0 ),0,USHRT_MAX);  // see scaling in hb-uni-sensor-RAD-AL53.xml
            
            DPRINT(F("moving average of radiation event counts per measurement interval (x100.0)               : "));
            DDECLN(_moving_average_count);
            
            _width_confidence_interval           = (uint16_t) constrain(( AL53_rad_statistics.get_width_of_confidence_interval() *100.0 ),0,USHRT_MAX); // see scaling in hb-uni-sensor-RAD-AL53.xml
            
            DPRINT(F("width of confidence interval of radiation event counts per measurement interval (x100.0) : "));
            DDECLN(_width_confidence_interval);
            
            _moving_average_count_max            = (uint16_t) constrain(( AL53_rad_statistics.get_moving_average_max() *100.0 ),0,USHRT_MAX); // see scaling in hb-uni-sensor-RAD-AL53.xml
            
            DPRINT(F("maximum moving average of radiation event counts per measurement interval                : "));
            DDECLN(_moving_average_count_max);
            
            _counts_per_measurement_interval_max = AL53_rad_statistics.get_max_count();
            
            DPRINT(F("maximum of number of radiation event counts per measurement interval                     : "));
            DDECLN(_counts_per_measurement_interval_max);
            
            ADS115_ADC.measure();
        
           _AL53_supply_voltage                      = ADS115_ADC.AL53_supply_voltage();
           
            DPRINT(F("AL53 supply voltage (x1000.0)                                                            : "));
            DDECLN(_AL53_supply_voltage);
            
           _operating_voltage                        = ADS115_ADC.VCC_operating_voltage();
           
            DPRINT(F("VCC supply voltage (x1000.0)                                                             : "));
            DDECLN(_operating_voltage);           
            
           _accumulator_voltage                      = ADS115_ADC.accumulator_voltage();
           
            DPRINT(F("accumulator batteries voltage (x1000.0)                                                  : "));
            DDECLN(_accumulator_voltage); 
            
            // check if alarm level of counts_per_measurement_interval is exceeded
            
            if ( _counts_per_measurement_interval > _alarm_level_counts_per_measurement_interval ) {
              _alarm_count_per_measurement_interval = 1;
            }
            else
            { _alarm_count_per_measurement_interval = 0;
            }
            
            // check if alarm level of moving_average is exceeded
             
            if ( _moving_average_count > _alarm_level_moving_average ) {
              _alarm_moving_average = 1;
            }
            else
            { _alarm_moving_average = 0;
            }
            
            DPRINT(F("alarm count per measurement interval                                                     : "));
            DDECLN(_alarm_count_per_measurement_interval);
            DPRINT(F("alarm moving average                                                                     : "));
            DDECLN(_alarm_moving_average); 
            
#ifdef DEEP_DEBUG
            unsigned long MeasureFinishTime  = millis();
            unsigned long MeasureElapsedTime = MeasureFinishTime - MeasureStartTime;
            DPRINT(F("executiom time of measurement method                                                     : "));
            DDEC(MeasureElapsedTime); 
            DPRINTLN(F(" msec"));
#endif
            
            ABLIC_S35770_counter.counter_signal_off(); // PC3, IDE pin 19 set to 0
            // enable again the counting of the ABLIC S35770 counter
            ABLIC_S35770_counter.enable_counting();
        }
        
    }
    
    
    bool      alarm_count_per_measurement_interval()       { return _alarm_count_per_measurement_interval;}
    bool      alarm_moving_average()                       { return _alarm_moving_average;}
    uint16_t  moving_average_count()                       { return _moving_average_count; }
    uint16_t  counts_per_measurement_interval()            { return _counts_per_measurement_interval; }
    uint16_t  width_confidence_interval()                  { return _width_confidence_interval; }
    uint16_t  moving_average_count_max()                   { return _moving_average_count_max; }
    uint16_t  counts_per_measurement_interval_max()        { return _counts_per_measurement_interval_max; }
    uint16_t  AL53_supply_voltage()                        { return _AL53_supply_voltage; }
    uint16_t  operating_voltage()                          { return _operating_voltage; }
    uint16_t  accumulator_voltage()                        { return _accumulator_voltage; }
    
};


}

#endif
