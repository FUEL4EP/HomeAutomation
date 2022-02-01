//---------------------------------------------------------
// Sens_SHT75
// 2018-08-12 Tom Major (Creative Commons)
// 2020-11-29 FUEL4EP (Creative Commons) adaption to Sensirion SHT75
// https://creativecommons.org/licenses/by-nc-sa/3.0/
// You are free to Share & Adapt under the following terms:
// Give Credit, NonCommercial, ShareAlike
// +++
// AskSin++ 2016-10-31 papa Creative Commons
//---------------------------------------------------------

// support high accuracy Sensirion SHT75 sensors
// increased accurracy of humidty output to 0.1% rH (uint16_t)

#ifndef _SENS_SHT75_H_
#define _SENS_SHT75_H_

#include <Wire.h>
#include <Sensors.h>
#include <Sensirion.h>    // https://github.com/spease/Sensirion

//calculate 24h moving average
#if defined CALCULATE_MOVING_AVERAGES
#include "Sens_weather_statistics_RAM.h"
#endif

namespace as {

template <uint8_t DATAPIN, uint8_t CLOCKPIN> class Sens_SHT75 : public Sensor {
    Sensirion _sht75;
    bool      _i2cSharedAccess;
    int16_t   _temperature;
    uint16_t  _humidity;
    float     _temperature_correction;
    float     _humidity_correction;
#if defined CALCULATE_MOVING_AVERAGES
    int16_t   _24h_moving_average_temperature100;     // 24 h moving average of temperature,  scaled with 100.0
    weather_statistics<int16_t, int32_t,  360>  _24h_moving_average_temperature_statistics;              // 1 * 24 * 60 / 4 measurement samples int16_t =  720 Bytes RAM memory for circular buffer
#endif

    void i2cDisable()
    {
        if (_i2cSharedAccess && DATAPIN == A4 && CLOCKPIN == A5) {
            Wire.end();
        }
    }

    void i2cEnable()
    {
        if (_i2cSharedAccess && DATAPIN == A4 && CLOCKPIN == A5) {
            Wire.begin();
        }
    }

public:
    Sens_SHT75()
        : _sht75(DATAPIN, CLOCKPIN)
        , _i2cSharedAccess(false)
        , _temperature(0)
        , _humidity(0)
    {
    }

    void i2cEnableSharedAccess()
    {
        _i2cSharedAccess = true;
        DPRINTLN(F("SHT75 enabled I2C shared access"));
    }

    bool init(int16_t temperature_correction, int16_t humidity_correction)
    {
#if defined CALCULATE_MOVING_AVERAGES
        _24h_moving_average_temperature_statistics.clear_buffer();
#endif
        i2cDisable();
        uint8_t stat, i = 10;
        while (i > 0) {
            if (_sht75.readSR(&stat) == 0) {
                _present = true;
                DPRINTLN(F("SHT75 found"));
                i2cEnable();
                _temperature_correction = (float)(temperature_correction / 10);
                _humidity_correction    = (float)(humidity_correction    / 10);
                
                return true;
            }
            delay(100);
            i--;
        }
        DPRINTLN(F("Error: SHT75 not found"));
        i2cEnable();
        return false;
    }

    bool measure()
    {
        _temperature = _humidity = 0;
        bool bRet    = false;
        if (_present == true) {
            i2cDisable();
            uint16_t rawData;
            if (_sht75.measTemp(&rawData) == 0) {
                float t      = _sht75.calcTemp(rawData);
                _temperature = (int16_t)(round((t + _temperature_correction) * 10.0));
                if (_sht75.measHumi(&rawData) == 0) {
                    float h   = _sht75.calcHumi(rawData, t);
                    _humidity = (uint16_t)(round((h + _humidity_correction) * 10.0));
                    bRet      = true;
                }
            }
#if defined CALCULATE_MOVING_AVERAGES
            _24h_moving_average_temperature_statistics.add_measurement(_temperature);
            DDECLN(_24h_moving_average_temperature_statistics.get_moving_average());
            _24h_moving_average_temperature100   = (int16_t)(round(_24h_moving_average_temperature_statistics.get_moving_average() * 10.0 )); // scale by 10.0 in order to get 0.01 K resolution
#endif      
            DPRINT(F("SHT75    Temperature x10  : "));
            DDECLN(_temperature);
            DPRINT(F("SHT75    Humidity    x10  : "));
            DDECLN(_humidity);
#if defined CALCULATE_MOVING_AVERAGES
            DPRINT(F("SHT75    24h MA -T  x100  : ")); DDECLN(_24h_moving_average_temperature100);
#endif
            i2cEnable();
        }
        return bRet;
    }

    int16_t  temperature()                             { return _temperature; }
    uint16_t humidity()                                { return _humidity; }
#if defined CALCULATE_MOVING_AVERAGES
    int16_t  moving_average_24h_temperature100 ()      { return _24h_moving_average_temperature100; }
#endif
};

}

#endif
