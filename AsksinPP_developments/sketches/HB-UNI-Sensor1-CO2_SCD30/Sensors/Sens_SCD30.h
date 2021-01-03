
//---------------------------------------------------------
// Sens_SCD30
// 2019-07-14 Tom Major (Creative Commons)
// 2020-11-24 FUELEP
// https://creativecommons.org/licenses/by-nc-sa/4.0/
// You are free to Share & Adapt under the following terms:
// Give Credit, NonCommercial, ShareAlike
// +++
// AskSin++ 2016-10-31 papa Creative Commons
//---------------------------------------------------------

#ifndef _SENS_SCD30_H_
#define _SENS_SCD30_H_



#include "my_Sensors_SCD30.h"
#include <Wire.h>
#include "SparkFun_SCD30_Arduino_Library.h" //Click here to get the library: http://librarymanager/All#SparkFun_SCD30

#define COMMAND_STOP_MEASUREMENT 0x0104 // supplemented in order to stop the measurements in case of an empty accumulator battery

//#define DEEP_DEBUG // comment out if deep serial monitor debugging is not necessary

namespace as {

class Sens_SCD30 : public Sensor {

    int16_t  _temperature;
    uint32_t _carbondioxide;
    uint8_t  _humidity;
    int16_t  _humidity_correction;
    
    SCD30    airSensor;

    void measureRaw()
    {
        DPRINTLN("");
        while (!airSensor.dataAvailable())
        {
            delay(1000);
            DPRINT(".");
            if (airSensor.dataAvailable())
            {  
              DPRINTLN("");
              break;
            }
        }
        if (airSensor.dataAvailable())
        {    
             
              _temperature      = (int16_t)(round(airSensor.getTemperature() * 10.0)); 
              DPRINT("Measured temperature x10       : ");
              DPRINT(_temperature);
              DPRINTLN(" deg C");
              _carbondioxide    = (uint32_t)(round(airSensor.getCO2() * 100.0));
#ifdef DEEP_DEBUG
              DPRINT("Sensor's humidity measurement  : ");
              DPRINT(airSensor.getHumidity());
              DPRINTLN(" %");
#endif
              _humidity         = (uint8_t)((int16_t)(round(airSensor.getHumidity())) + _humidity_correction);
              DPRINT("Corrected humidity measurement : ");
              DPRINT(_humidity);
              DPRINTLN(" %");
              DPRINTLN("");
        }
    }

public:
    Sens_SCD30()
        : _temperature(0)
        , _carbondioxide(0)
        , _humidity(0)
    {
    }

    void init(uint16_t update_intervall, uint16_t height, uint16_t ambient_pressure, int16_t temperature_correction, int16_t humidity_correction)
    {
        Wire.begin();
        
        
        delay(200);
        
        if (airSensor.begin() == false)
        {
          DPRINTLN("Error: no Sensirion SCD30 CO2 sensor found");
          DPRINTLN("Please check wiring. Freezing...");
          while (1);
        }
        else
        {
          DPRINTLN(F("SCD30 found"));
          _present = true;
          
          if (update_intervall == 0 ) {
            update_intervall = 240;
          }
        
      
          airSensor.setMeasurementInterval(update_intervall/4);   // Change number of seconds between measurements: 2 to 1800 (30 minutes)
                                                                  // update_intervall = 240 sec; SCD30 measurement intervall: 60 sec to allow SCD30 internal
                                                                  // hardware oversampling

          airSensor.setAltitudeCompensation(height);              // Set altitude of the sensor in m
       
          airSensor.setAmbientPressure(ambient_pressure);         // Current ambient pressure in mBar: 700 to 1200
          
          airSensor.setAutoSelfCalibration(true);                 // enable autocalibration
          
          float offset = airSensor.getTemperatureOffset();
#ifdef DEEP_DEBUG
          DPRINT("Current temperature correction: ");
          DPRINT(offset);
          DPRINTLN(" deg C");
          DPRINT("Changed temperature correction: ");
          DPRINT((float)temperature_correction/10.0);
          DPRINTLN(" deg C");
#endif
          
          if ( temperature_correction < 0) {
             DPRINTLN("The temperature offset must be positive !!!");
             DPRINTLN("The temperature offset has been set to zero !!!");
             temperature_correction = 0;
          }
          airSensor.setTemperatureOffset(((float)(temperature_correction))/10.0);
#ifdef DEEP_DEBUG
          DPRINT("New temperature correction: ");
          DPRINT((float)temperature_correction/10.0);
          DPRINTLN(" deg C");
#endif
          _humidity_correction = humidity_correction/10;
#ifdef DEEP_DEBUG
          DPRINT("Humidity correction: ");
          DPRINT(_humidity_correction);
          DPRINTLN(" %");
          DPRINT("Update intervall: ");
          DPRINT(update_intervall);
          DPRINTLN(" s");
          DPRINT("Altitude above NN : ");
          DPRINT(height);
          DPRINTLN(" m");
          DPRINT("Ambient pressure setting : ");
          DPRINT(ambient_pressure);
          DPRINTLN(" hPa (zero for disabling pressure compensation)");
#endif
          DPRINTLN(" ");
          DPRINTLN("IMPORTANT: Remove accumulator batteries during debugging with a FTDI Debugger in the serial monitor !!");
          DPRINTLN("           This avoids a deep discharge of them by the step-up converter");
          DPRINTLN(" ");
          //DPRINTLN("           Unplug the USB connector of the serial programmer (diamex) during debugging with serial monitor !!");
          //DPRINTLN("           This avoids data corruption");
          //DPRINTLN(" ");

        }
    }
    
    void stop_measurements()
    {
        airSensor.sendCommand(COMMAND_STOP_MEASUREMENT);
        DPRINTLN("Stop continuous measurements of SCD30");
    }

    void measure()
    {
        _temperature = _carbondioxide = _humidity = 0;
        if (_present == true) {
            measureRaw();
#ifdef DEEP_DEBUG
            DPRINT(F("SCD30 Temperature x10        : "));
            DDECLN(_temperature);
            DPRINT(F("SCD30 Carbondioxide x100     : "));
            DDECLN(_carbondioxide);
            DPRINT(F("SCD30 Humidity               : "));
            DDECLN(_humidity);
#endif
        }
    }

    int16_t  temperature()   { return _temperature; }
    uint32_t carbondioxide() { return _carbondioxide; }
    uint8_t  humidity()      { return _humidity; }
};

}

#endif
