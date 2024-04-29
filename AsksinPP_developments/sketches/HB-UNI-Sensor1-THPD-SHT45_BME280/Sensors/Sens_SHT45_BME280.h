
#ifndef _SENS_SHT45_BME280_H_
#define _SENS_SHT45_BME280_H_


#include <Sensors.h>
#include <BME280I2C.h>
#include <EnvironmentCalculations.h>
#include <SensirionI2cSht4x.h>
#include <Wire.h>
//calculate 24h and 7 days moving averages, note: This option requires an ATMega1284P for RAM memory size reasons
#include "Sens_weather_statistics_RAM.h"



// default I2C addresses of the required sensors are
//
// Sensirion       0x44     # (fixed, see Sensirion SHT45 data sheet)
// Bosch BME280    0x76
//


BME280I2C::Settings settings(
   BME280::OSR_X1,      // temperature
   BME280::OSR_X1,      // pressure
   BME280::OSR_X1,      // humidity
   BME280::Mode_Forced,
   BME280::StandbyTime_1000ms,
   BME280::Filter_Off,
   BME280::SpiEnable_False,
   BME280I2C::I2CAddr_0x76 // I2C address. I2C specific.
);

BME280I2C bme(settings);

namespace as {

class Sens_SHT45_Bme280 : public Sensor {

  int16_t   _altitude;
  float     _tempOffset;
  float     _humidOffset;
  float     _pressOffset;
  int16_t   _temperature10;
  uint16_t  _pressure10;
  uint16_t  _pressureNN10;
  uint16_t  _humidity10;
  int16_t   _dewPoint10;                           // dewpoint x10
  uint16_t  _vaporConcentration100;                // absolute humidity concentration x100
  int16_t   _24h_moving_average_temperature100;     // 24 h moving average of temperature,  scaled with 10.0
  int16_t   _7days_moving_average_temperature100;   // 7 days moving average of temperature, scaled with 10.0

  BME280I2C _bme280;        // Default : forced mode, standby time = 1000 ms, Oversampling = pressure ×1, temperature ×1, humidity ×1, filter off

  SensirionI2cSht4x _sht45;

  char     errorMessage[256];
  

  weather_statistics<int16_t, int32_t,  360>  _24h_moving_average_temperature_statistics;              // 1 * 24 * 60 / 4 measurement samples int16_t =  720 Bytes RAM memory for circular buffer
  weather_statistics<int16_t, int32_t, 2520>  _7days_moving_average_temperature_statistics;            // 7 * 24 * 60 / 4 measurement samples int16_t = 5040 Bytes RAM memory for circular buffer

public:

  Sens_SHT45_Bme280 () : _temperature10(0), _pressure10(0), _pressureNN10(0), _humidity10(0), _dewPoint10(0), _vaporConcentration100(0) {}

  void init (uint16_t altitude, int8_t tempOffset10, int8_t humidOffset10, int8_t pressOffset10) {

    Wire.begin();

    uint8_t i = 10;
    while( (!_bme280.begin()) && (i > 0) ) {
      delay(100);
      i--;
    }
    
// check whether BME280 sensor can be accessed
    bool _present_BME280;
  
    switch(_bme280.chipModel()) {
     case BME280::ChipModel_BME280:
       _present_BME280 = true;
       _bme280.setSettings(settings);
       DPRINTLN(F("BME280 sensor OK"));       
       _altitude     = altitude;
       _pressOffset  = (float)( pressOffset10 ) / 10.0;
       DPRINTLN(F("BME280 offsets (all x10):"));
       DPRINT(F("-P    : ")); DDECLN(pressOffset10);
       break;
     default:
       DPRINTLN(F("BME280 sensor NOT found"));
       _present_BME280 = false;
  }
  
   // check whether SHT45 sensor can be accessed
   bool _present_SHT45;
   uint16_t error;

  _sht45.begin(Wire, 0x44);

  uint32_t serialNumber;

  DPRINTLN(F("SHT45: Trying to get serial number.."));
  error = _sht45.serialNumber(serialNumber);
  DPRINTLN(F("SHT45: Getting serial number finished .."));
  if (error) {
    DPRINTLN(F("SHT45 sensor NOT found"));
    DPRINTLN(F("SHT45: Error trying to execute serialNumber(): "));
    errorToString(error, errorMessage, 256);
    DPRINTLN(errorMessage);
    _present_SHT45 = false;
  } else {
    DPRINTLN(F("SHT45 sensor OK"));
    DPRINT(F("Serial Number: "));
    DPRINTLN(serialNumber);
    _present_SHT45 = true;
    _tempOffset   = (float)( tempOffset10 )  / 10.0;
    _humidOffset  = (float)( humidOffset10 ) / 10.0;
    DPRINTLN(F("SHT45 offsets (all x10):"));
    DPRINT(F("-T    : ")); DDECLN(tempOffset10);
    DPRINT(F("-H    : ")); DDECLN(humidOffset10);
  }
  
  _24h_moving_average_temperature_statistics.clear_buffer();
  _7days_moving_average_temperature_statistics.clear_buffer();

  
  _present = _present_BME280 && _present_SHT45;
  }
  

  void measure () {
    if (_present == true) {
      float   temp(NAN), hum(NAN), pres(NAN), temp_bme280(NAN), hum_bme280(NAN);
      uint16_t error;

      // read BME280 sensor, temp_bme280 and hum_bme280 are discarded here
      _bme280.read(pres, temp_bme280, hum_bme280, BME280::TempUnit_Celsius, BME280::PresUnit_hPa);

      DPRINTLN(F("SHT45 measurement .."));

      // read SHT45 measurement values
      error = _sht45.measureHighPrecision(temp, hum);

     // check for errors
      if (error) {
        DPRINTLN(F("SHT45: Error trying to execute measureHighPrecision(): "));
        errorToString(error, errorMessage, 256);
        DPRINTLN(errorMessage);
      } else {

        temp = temp + _tempOffset;
        hum  = hum  + _humidOffset;
        pres = pres + _pressOffset;

        _temperature10           = (int16_t)(round(temp * 10.0));
         _24h_moving_average_temperature_statistics.add_measurement(_temperature10);
        _7days_moving_average_temperature_statistics.add_measurement(_temperature10);
        _pressure10              = (uint16_t)(round(pres * 10.0));
        _pressureNN10            = (uint16_t)(round(EnvironmentCalculations::EquivalentSeaLevelPressure(float(_altitude), temp, pres) * 10.0));
        _humidity10              = (uint16_t)(round(hum * 10.0));
        _dewPoint10              = (int16_t)(round(EnvironmentCalculations::DewPoint(temp, hum, EnvironmentCalculations::TempUnit_Celsius) * 10.0));
        _vaporConcentration100   = (uint16_t)(EnvironmentCalculations::AbsoluteHumidity(temp, hum, EnvironmentCalculations::TempUnit_Celsius) * 100.0);
        _24h_moving_average_temperature100   = (int16_t)(round(_24h_moving_average_temperature_statistics.get_moving_average()*10.0));      // temperature scaled by 100.0: 10.0 by _temperature10 and 10.0 here
        _7days_moving_average_temperature100 = (int16_t)(round(_7days_moving_average_temperature_statistics.get_moving_average()*10.0));    // temperature scaled by 100.0: 10.0 by _temperature10 and 10.0 here

      
        DPRINTLN(F("SHT45 and BME280 (all x10 except vapour x100):"));
        DPRINT(F("SHT45  -T    : ")); DDECLN(_temperature10);
        DPRINT(F("BME280 -P    : ")); DDECLN(_pressure10);
        DPRINT(F("BME280 -P(NN): ")); DDECLN(_pressureNN10);
        DPRINT(F("SHT45  -H    : ")); DDECLN(_humidity10);
        DPRINT(F("SHT45  -DP   : ")); DDECLN(_dewPoint10);
        DPRINT(F("SHT45  -aH   : ")); DDECLN(_vaporConcentration100);
        DPRINT(F("24h MA -T    : ")); DDECLN(_24h_moving_average_temperature100);
        DPRINT(F("7D MA  -T    : ")); DDECLN(_7days_moving_average_temperature100);
      }
    }
  }
  
  int16_t  temperature ()                           { return _temperature10; }
  uint16_t humidity ()                              { return _humidity10; }
  uint16_t pressureNN ()                            { return _pressureNN10; }
  uint16_t pressure ()                              { return _pressure10; }
  int16_t  dewPoint ()                              { return _dewPoint10; }
  uint16_t vaporConcentration ()                    { return _vaporConcentration100; }
  int16_t  moving_average_24h_temperature100 ()      { return _24h_moving_average_temperature100; }
  int16_t  moving_average_7D_temperature100 ()       { return _7days_moving_average_temperature100; }
};

}

#endif
