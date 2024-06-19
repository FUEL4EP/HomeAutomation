#ifndef _SENS_AHT20_BMP280_H_
#define _SENS_AHT20_BMP280_H_


#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BMP280.h>
#include <Sensors.h>
#include <AHT20.h>
#include <EnvironmentCalculations.h>
#include <Adafruit_FRAM_SPI.h>


#include "Fujitsu_MB85RS2MTPF_FRAM_circular_buffer.h"


#include "Sens_weather_statistics_FRAM.h"



namespace as {


// default I2C addresses of the required sensors are
//
// Aosong AHT20    0x38     # fixed, see AHT20 data sheet
const uint8_t  _I2C_addr_AHT20  = 0x38;
// Bosch  BMP280   0x77     # default
const uint8_t  _I2C_addr_BMP280 = 0x77;


class Sens_AHT20_BMP280 : public Sensor {

  int16_t   _altitude;
  int16_t   _temperature10;
  uint16_t  _pressure10;
  uint16_t  _pressureNN10;
  uint16_t  _humidity10;
  int8_t    _bootType;
  int8_t    _presetHistoricTemperatures;
  int16_t   _presetTemp10;
  int16_t   _24h_moving_average_temperature100;     // 24 h moving average of temperature   , scaled with 100.0
  int16_t   _7D_moving_average_temperature100;      // 7 days moving average of temperature , scaled with 100.0
  int16_t   _1M_moving_average_temperature100;      // 1 month moving average of temperature, scaled with 100.0
  int16_t   _1Y_moving_average_temperature100;      // 1 year moving average of temperature , scaled with 100.0
  bool       success;


  AHT20                     aht20;                  // set I2C adddress
  Adafruit_BMP280           bmp;                    // I2C mode
  FUJITSU_MB85RS2MTPF_FRAMs frams;

  // instantiate circular buffers in FRAMs, note the curly braces for the initialization parameters!

  weather_statistics<int16_t, int32_t>  _1Y_moving_average_temperature_statistics{&frams,  0x00001, 1, 131400L, 'Y'}; // 365 * 24 * 60 / 4 measurement samples int16_t =  262800 Bytes RAM memory for circular buffer

  weather_statistics<int16_t, int32_t>  _1M_moving_average_temperature_statistics{&frams,  0x402C2, 2,  11160L, 'M'}; // 31 * 24 * 60 / 4 measurement samples int16_t =    22320 Bytes RAM memory for circular buffer

  weather_statistics<int16_t, int32_t>  _7D_moving_average_temperature_statistics{&frams,  0x45AEB, 3,   2520L, 'W'}; // 7 * 24 * 60 / 4 measurement samples int16_t  =     5040 Bytes RAM memory for circular buffer

  weather_statistics<int16_t, int32_t>  _24h_moving_average_temperature_statistics{&frams, 0x46ECC, 4,    360L, 'D'}; // 1 * 24 * 60 / 4 measurement samples int16_t  =      720 Bytes RAM memory for circular buffer

public:

  Sens_AHT20_BMP280 () : _temperature10(0), _pressure10(0), _pressureNN10(0), _humidity10(0) {}

  void init (uint16_t altitude, float tempOffset, float humidOffset, float pressOffset) {


    DPRINTLN(F("init Sens_AHT20_BMP280 .. "));

    DPRINTLN(F("init I2C .. "));
    Wire.begin(); // join I2C bus

    // check whether AHT20 sensor can be accessed @ IC2 address _I2C_addr_AHT20
    bool _present_AHT20;
    DPRINT(F("AHT20 "));
    if (aht20.begin() == false)
    {
      DPRINT(F("not detected at I2C address 0x"));
      DHEXLN(_I2C_addr_AHT20);

      _present_AHT20 = false;
    }
    else {
      DPRINT(F("sensor detected at I2C address 0x"));
      DHEXLN(_I2C_addr_AHT20);
      _present_AHT20 = true;

      DPRINTLN(F("AHT20 offsets:"));
      DPRINT(F("-T    : ")); DDECLN(tempOffset);
      DPRINT(F("-H    : ")); DDECLN(humidOffset);
    }


    // check whether BMP280 sensor can be accessed @ IC2 address _I2C_addr_BMP280
    bool _present_BMP280;
    unsigned status;
    status = bmp.begin(_I2C_addr_BMP280);
    DPRINT(F("BMP280 "));
    if (!status) {
      DPRINT(F("not detected at I2C address 0x"));
      DHEXLN(_I2C_addr_BMP280);

      _present_BMP280 = false;
    }
    else {
      DPRINT(F("sensor detected at I2C address 0x"));
      DHEXLN(_I2C_addr_BMP280);
      const uint8_t chipid = BMP280_CHIPID;
      switch(bmp.sensorID()) {
        case chipid:
          DPRINTLN(F("BMP280 sensor OK"));
          _present_BMP280 = true;

          /* Default settings from datasheet. */
          bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
            Adafruit_BMP280::SAMPLING_X2,                   /* Temp. oversampling */
            Adafruit_BMP280::SAMPLING_X16,                  /* Pressure oversampling */
            Adafruit_BMP280::FILTER_X16,                    /* Filtering. */
            Adafruit_BMP280::STANDBY_MS_500);               /* Standby time. */
          break;
       default:
         DPRINTLN(F("BMP280 sensor NOT found"));
        _present_BMP280 = false;
       }

      _altitude                    = altitude;
      DPRINTLN(F("BMP280 offset :"));
      DPRINT(F("-P    : ")); DDECLN(pressOffset);


    }
    frams.print_FRAM_sizes();
    _present = _present_BMP280 && _present_AHT20;
  }


  bool init_circular_buffers_in_FRAMs(int8_t bootType, int8_t presetHistoricTemperatures, int32_t presetTemp10) {
    bool success = true;

    DPRINTLN(F("inititialize circular buffers in FRAM bank .. "));
    DPRINT(F("bootType                   : "));DDECLN(bootType);
    DPRINT(F("presetHistoricTemperatures : "));DDECLN(presetHistoricTemperatures);


    _presetTemp10                = (int16_t)(presetTemp10); // convert to int16_t

    DPRINT(F("presetTemp10               : "));DDECLN(_presetTemp10);

    SPI.end();    // end SPI bus
    DPRINTLN(F("init SPI .. "));
    SPI.begin();  // initialize SPI bus


    if (bootType == 0) {
      // cold boot
      success=frams.cold_boot();
    }
    else {
      // warm boot
      success=frams.warm_boot();
    }

    if ( success ) {

      // initialize the circular buffers:
      // a) restore from FRAM in case of a warm boot
      // b) preset the circular buffers with fixed preset temperature or interpolated historical temperatures

      DPRINTLN(F("initialize yearly circular buffer .."));
      success &= _1Y_moving_average_temperature_statistics.initialize_circular_buffer(_presetTemp10, presetHistoricTemperatures);
      DPRINTLN(F("initialize monthly circular buffer .."));
      success &= _1M_moving_average_temperature_statistics.initialize_circular_buffer(_presetTemp10, presetHistoricTemperatures);
      DPRINTLN(F("initialize weekly circular buffer .."));
      success &= _7D_moving_average_temperature_statistics.initialize_circular_buffer(_presetTemp10, presetHistoricTemperatures);
      DPRINTLN(F("initialize daily circular buffer .."));
      success &= _24h_moving_average_temperature_statistics.initialize_circular_buffer(_presetTemp10, presetHistoricTemperatures);

      if ( success ) {
        // set FRAM start status flag as INITIALIZED
        success &= frams.set_FRAM_start_status_flag(FRAMS_INITIALIZED);
      }

      frams.print_FRAM_sizes();
    }

    return success;
  }
  

  void measure (float tempOffset, float humidOffset, float pressOffset) {

    bool success = true;

    if (_present == true) {
      float   temp(NAN), hum(NAN), pres(NAN);

      DPRINTLN(F("measuring now .. "));

      // wake-up FRAMs from low power mode
      success = frams.FRAM_normal_mode();
      if ( !success ) {
        DPRINTLN(F("ERROR: Cannot wake-up FRAMs!"));
      } else {
        frams.print_FRAM_sizes();
      }

      // read AHT20 sensor
      uint8_t counter = 0;
      while (aht20.available() == false) {
            delay(1);  // wait 1 ms
            if (counter++ > 100)
                break; //Give up after 100ms
      }
      if (counter < 100) {
          //DPRINT(F("counter AHT20 : "));DDECLN(counter);
          temp = aht20.getTemperature();;
          hum  = aht20.getHumidity();
      } else {
        temp=999;
        hum=999;
        DPRINTLN(F("error in reading AHT20 !"));
      }

      // read pressure from BMP280 sensor as hPa
      pres=bmp.readPressure() / 100.0;   // read pressure is scaled with x 100.0 since Pa, not hPa

      // do offset compensations
      temp = temp + tempOffset;
      hum  = hum  + humidOffset;
      pres = pres + pressOffset;

      _temperature10           = (int16_t)(round(temp * 10.0));

      // add current temperature to circular buffers
      success &= _1Y_moving_average_temperature_statistics.add_measurement(_temperature10);
      success &= _1M_moving_average_temperature_statistics.add_measurement(_temperature10);
      success &= _7D_moving_average_temperature_statistics.add_measurement(_temperature10);
      success &= _24h_moving_average_temperature_statistics.add_measurement(_temperature10);

      _pressure10              = (uint16_t)(round(pres * 10.0));
      _pressureNN10            = (uint16_t)(round(EnvironmentCalculations::EquivalentSeaLevelPressure(float(_altitude), temp, pres) * 10.0));
      _humidity10              = (uint16_t)(round(hum * 10.0));
      
      // get moving averages from circular buffers
      _1Y_moving_average_temperature100 =  (int16_t)(round(_1Y_moving_average_temperature_statistics.get_moving_average() * 10.0));
      _1M_moving_average_temperature100 =  (int16_t)(round(_1M_moving_average_temperature_statistics.get_moving_average() * 10.0));
      _7D_moving_average_temperature100 =  (int16_t)(round(_7D_moving_average_temperature_statistics.get_moving_average() * 10.0));
      _24h_moving_average_temperature100 = (int16_t)(round(_24h_moving_average_temperature_statistics.get_moving_average() * 10.0));

      // save non-volatile parameters of circular buffers to FRAM
      success &=  _1Y_moving_average_temperature_statistics.save_nvm_weather_statistics_parameters_to_FRAM();
      success &=  _1M_moving_average_temperature_statistics.save_nvm_weather_statistics_parameters_to_FRAM();
      success &=  _7D_moving_average_temperature_statistics.save_nvm_weather_statistics_parameters_to_FRAM();
      success &= _24h_moving_average_temperature_statistics.save_nvm_weather_statistics_parameters_to_FRAM();

      DPRINTLN(F("AHT20 and BMP280 (all x10):"));
      DPRINT(F("AHT20  -T    : ")); DDECLN(_temperature10);
      DPRINT(F("BMP280 -P    : ")); DDECLN(_pressure10);
      DPRINT(F("BMP280 -P(NN): ")); DDECLN(_pressureNN10);
      DPRINT(F("AHT20  -H    : ")); DDECLN(_humidity10);
      DPRINTLN(F("moving average tempertures (all x100):"));
      DPRINT(F("24h MA -T    : ")); DDECLN(_24h_moving_average_temperature100);
      DPRINT(F("7D MA  -T    : ")); DDECLN(_7D_moving_average_temperature100);
      DPRINT(F("1M MA  -T    : ")); DDECLN(_1M_moving_average_temperature100);
      DPRINT(F("1Y MA  -T    : ")); DDECLN(_1Y_moving_average_temperature100);

      // enter low power mode of FRAMs
      success = frams.FRAM_sleep_mode();
      if ( !success ) {
        DPRINTLN(F("ERROR: Cannot enter low power mode of FRAMs!"));
      }

    }
    else
    {
      DPRINTLN(F("ERROR: Sensor initalization failed!"));
    }
  }
  
  int16_t  temperature ()                            { return _temperature10; }
  uint16_t humidity ()                               { return _humidity10; }
  uint16_t pressureNN ()                             { return _pressureNN10; }
  uint16_t pressure ()                               { return _pressure10; }

  int16_t  moving_average_24h_temperature100 ()      { return _24h_moving_average_temperature100; }
  int16_t  moving_average_7D_temperature100 ()       { return _7D_moving_average_temperature100; }
  int16_t  moving_average_1M_temperature100 ()       { return _1M_moving_average_temperature100; }
  int16_t  moving_average_1Y_temperature100 ()       { return _1Y_moving_average_temperature100; }

};

}

#endif
