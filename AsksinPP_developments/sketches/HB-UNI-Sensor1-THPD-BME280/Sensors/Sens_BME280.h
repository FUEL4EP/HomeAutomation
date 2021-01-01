
#ifndef _SENS_BME280_H_
#define _SENS_BME280_H_

#include <Wire.h>
#include <Sensors.h>
#include <BME280I2C.h>
#include <EnvironmentCalculations.h>


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

class Sens_Bme280 : public Sensor {

  int16_t   _altitude;
  float     _tempOffset;
  float     _humidOffset;
  float     _pressOffset;
  int16_t   _temperature10;
  uint16_t  _pressure10;
  uint16_t  _pressureNN10;
  uint16_t  _humidity10;
  int16_t   _dewPoint10;                // dewpoint x10
  uint16_t  _vaporConcentration100;     // absolute humidity concentration x100

  BME280I2C _bme280;        // Default : forced mode, standby time = 1000 ms, Oversampling = pressure ×1, temperature ×1, humidity ×1, filter off

public:

  Sens_Bme280 () : _temperature10(0), _pressure10(0), _pressureNN10(0), _humidity10(0), _dewPoint10(0), _vaporConcentration100(0) {}

  void init (uint16_t altitude, int8_t tempOffset10, int8_t humidOffset10, int8_t pressOffset10) {

    Wire.begin(); //ToDo sync with further I2C sensor classes

    uint8_t i = 10;
    while( (!_bme280.begin()) && (i > 0) ) {
      delay(100);
      i--;
    }
  
    switch(_bme280.chipModel()) {
     case BME280::ChipModel_BME280:
       _present = true;
       DPRINTLN(F("BME280 sensor OK"));
       
       _altitude     = altitude;
       _tempOffset   = (float)( tempOffset10  / 10.0 );
       _humidOffset  = (float)( humidOffset10 / 10.0 );
       _pressOffset  = (float)( pressOffset10 / 10.0 );
       
       DPRINTLN(F("BME280 offsets (all x10):"));
       DPRINT(F("-T    : ")); DDECLN(tempOffset10);
       DPRINT(F("-P    : ")); DDECLN(pressOffset10);
       DPRINT(F("-H    : ")); DDECLN(humidOffset10);
       
       break;
     default:
       DPRINTLN(F("BME280 sensor NOT found"));
    }
  }

  void measure () {
    if (_present == true) {
      float temp(NAN), hum(NAN), pres(NAN);
      _bme280.read(pres, temp, hum, BME280::TempUnit_Celsius, BME280::PresUnit_hPa);
      
      temp = temp + _tempOffset;
      hum  = hum  + _humidOffset;
      pres = pres + _pressOffset;
      
      _temperature10           = (int16_t)(round(temp * 10.0));
      _pressure10              = (uint16_t)(round(pres * 10.0));
      _pressureNN10            = (uint16_t)(round(EnvironmentCalculations::EquivalentSeaLevelPressure(float(_altitude), temp, pres) * 10.0));
      _humidity10              = (uint16_t)(round(hum * 10.0));
      _dewPoint10              = (int16_t)(EnvironmentCalculations::DewPoint(temp, hum, EnvironmentCalculations::TempUnit_Celsius) * 10.0);
      _vaporConcentration100   = (uint16_t)(EnvironmentCalculations::AbsoluteHumidity(temp, hum, EnvironmentCalculations::TempUnit_Celsius) * 100.0);
      
      DPRINTLN(F("BME280 (all x10):"));
      DPRINT(F("-T    : ")); DDECLN(_temperature10);
      DPRINT(F("-P    : ")); DDECLN(_pressure10);
      DPRINT(F("-P(NN): ")); DDECLN(_pressureNN10);
      DPRINT(F("-H    : ")); DDECLN(_humidity10);
      DPRINT(F("-DP   : ")); DDECLN(_dewPoint10);
      DPRINT(F("-aH   : ")); DDECLN(_vaporConcentration100);
    }
  }
  
  int16_t  temperature ()           { return _temperature10; }
  uint16_t humidity ()              { return _humidity10; }
  uint16_t pressureNN ()            { return _pressureNN10; }
  uint16_t pressure ()              { return _pressure10; }
  int16_t  dewPoint ()              { return _dewPoint10; }
  uint16_t vaporConcentration ()    { return _vaporConcentration100; }
};

}

#endif
