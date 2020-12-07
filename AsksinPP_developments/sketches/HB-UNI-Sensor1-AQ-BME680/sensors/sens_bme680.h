// based on https://github.com/G6EJD/BME680-Example
// https://github.com/pimoroni/bme680-python/blob/master/examples/indoor-air-quality.py

#ifndef _SENS_BME680_H_
#define _SENS_BME680_H_

//#define DEEP_DEBUG // comment out if deep serial monitor debugging is not necessary

#include <Wire.h>
#include <Sensors.h>
#include <ClosedCube_BME680.h>
#include <math.h>


/*
 * Gas is returned as a resistance value in ohms.
 * Once it stabilizes, you can use that as your baseline reading.
 * Higher concentrations of VOC will make the resistance lower.
 */

#define AVG_COUNT       5
#define IIR_FILTER_COEFFICIENT 0.0001359 // 1.0 -0.9998641 ; Decay to 0.71 in about one week for a 4 min sampling period (in 2520 sampling periods)
#define EPSILON         0.0001



namespace as {

template <uint8_t ADDRESS=0x76>  // I2C address needs to be be set according to your BME680 sensor breakout in the main sketch HB-UNI-Sen-AQ.ino: 'Sens_Bme680<0x76>   bme680;' not here!
class Sens_Bme680 : public Sensor {
private:
  int16_t   _temperature;
  uint16_t  _pressureNN;
  uint16_t   _humidity;
  uint16_t  _aqLevel;
  uint16_t  _aqState_scaled;
  int32_t   _max_gas_resistance;  // maximum measured gas resistance
  int32_t   _min_gas_resistance;  // minimum measured gas resistance
  int32_t   _gas_lower_limit;     // adaptive lower resistance level for bad air quality 
  int32_t   _gas_upper_limit;     // adaptive upper resistance level for good air quality
  int32_t   _res_lower_limit;     // adaptive lower residual level for bad air quality 
  int32_t   _res_upper_limit;     // adaptive upper residual level for good air quality
  float     _min_res, _max_res;   // adaptive min and max residual of multiple linear regression used for normalization
  int16_t   _gas_resistance_raw_scaled;
  int16_t   _gas_resistance_min_scaled;
  int16_t   _gas_resistance_max_scaled;
  uint16_t  _height;
  int32_t   _gas_upper_limit_min;
  int32_t   _gas_lower_limit_max;
  int32_t   _res_upper_limit_min;
  int32_t   _res_lower_limit_max;
  uint32_t  _gas_decay_factor_upper_limit;
  uint32_t  _gas_increase_factor_lower_limit;
  uint8_t   _max_decay_factor_upper_limit;
  uint8_t   _max_increase_factor_lower_limit;
  double    _mlr_alpha; //multiple linear regression coefficient alpha (temperature)
  double    _mlr_beta;  //multiple linear regression coefficient beta (absolute humidity)
  double    _mlr_delta; //multiple linear regression coefficient delta (offset)

  ClosedCube_BME680 _bme680;
public:

  Sens_Bme680 (): _temperature(0),  _pressureNN(0), _humidity(0), _aqLevel(0), _aqState_scaled(0), _gas_resistance_raw_scaled(0), _gas_resistance_min_scaled(0), _gas_resistance_max_scaled(0) {}
    ~Sens_Bme680 () {}

  void init (uint16_t height, uint8_t max_decay_factor_upper_limit, uint8_t max_increase_factor_lower_limit, int32_t mlr_alpha, int32_t mlr_beta, int32_t mlr_delta) {

    Wire.begin();
    DPRINT(F("BME680 "));
    _bme680.init(ADDRESS); // I2C address: 0x76 or 0x77
    if (_bme680.reset() != 0) {
      DPRINTLN(F("ERR"));
      while (1);
    } else DPRINT(F("OK"));
    _present = true;


    DPRINT(", Chip ID=0x");
    DHEXLN(_bme680.getChipID());

      // oversampling: humidity = x2, temperature = x2, pressure = x4
    _bme680.setOversampling(BME680_OVERSAMPLING_X2, BME680_OVERSAMPLING_X2, BME680_OVERSAMPLING_X4);
    _bme680.setIIRFilter(BME680_FILTER_3); // supresses spikes 
    _bme680.setGasOn(310, 300); // 310 degree Celsius and 300 milliseconds; please check in debug mode whether '-> Gas heat_stab_r   = 1' is achieved. If '-> Gas heat_stab_r   = 0' then the heating time is to short or the temp target too high
    _bme680.setForcedMode();
    
    _max_gas_resistance = -2000000;     // initial value
    _min_gas_resistance = 2000000;      // initial value
    _max_res = _max_gas_resistance;     // initial value
    _min_res = _min_gas_resistance;     // initial value
    _height  =  height;
    _gas_lower_limit_max = _min_gas_resistance;
    _gas_upper_limit_min = _max_gas_resistance;
    _res_lower_limit_max = _min_gas_resistance;
    _res_upper_limit_min = _max_gas_resistance;
    _gas_lower_limit = _gas_lower_limit_max;
    _gas_upper_limit = _gas_upper_limit_min;
    _res_lower_limit = _res_lower_limit_max;
    _res_upper_limit = _res_upper_limit_min;
    _max_decay_factor_upper_limit = max_decay_factor_upper_limit;
    _max_increase_factor_lower_limit = max_increase_factor_lower_limit;
    _mlr_alpha = (double)mlr_alpha / 1000.0;
    _mlr_beta  = (double)mlr_beta / 1000.0;
    _mlr_delta = (double)mlr_delta / 1000.0;
    
#ifdef DEEP_DEBUG
    DPRINT(F("Gas UPPER LIMIT                    = "));DDECLN(_gas_upper_limit);
    DPRINT(F("Gas LOWER LIMIT                    = "));DDECLN(_gas_lower_limit);
    DPRINT(F("_gas_lower_limit_max               = "));DDECLN(_gas_lower_limit_max);
    DPRINT(F("_gas_upper_limit_min               = "));DDECLN(_gas_upper_limit_min);
    DPRINT(F("_max_decay_factor_upper_limit      = "));DDECLN(_max_decay_factor_upper_limit);
    DPRINT(F("_max_increase_factor_lower_limit   = "));DDECLN(_max_increase_factor_lower_limit);
    DPRINT(F("mlr_alpha                         = "));DDECLN(mlr_alpha);
    DPRINT(F("mlr_beta                          = "));DDECLN(mlr_beta);
    DPRINT(F("mlr_delta                         = "));DDECLN(mlr_delta);
    DPRINT(F("_mlr_alpha                         = "));DDECLN(_mlr_alpha);
    DPRINT(F("_mlr_beta                          = "));DDECLN(_mlr_beta);
    DPRINT(F("_mlr_delta                         = "));DDECLN(_mlr_delta);
#endif
    
  }

  double EquivalentSeaLevelPressure(float altitude, float temp, double pres) {
      double seaPress = NAN;
      if(!isnan(altitude) && !isnan(temp) && !isnan(pres))
          seaPress = (pres / pow(1 - ((0.0065 *(double)altitude) / ((double)temp + (0.0065 *(double)altitude) + 273.15)), 5.257));
      return seaPress;
  }


  void measure (double tempOffset, double pressOffset, double humidOffset) {
    if (_present == true) {
      double temp(NAN), hum(NAN), pres(NAN);                                 // use type double in order to match the return type of closed cubes's library function readPressure
      double ah,tt,ttt,vp,svp;                                               //variables for calculating the absolute humidity
      double log_normalized_residual,normalized_residual,residual,mlr_pred;  //variables for calculating the air quality level
      int32_t gas = 0;
      int32_t gas_raw;
  

      ClosedCube_BME680_Status status = _bme680.readStatus();
      while (! (status.newDataFlag == 1)) {
        _bme680.setForcedMode();
        DPRINT(".");
        _delay_ms(200);
        status = _bme680.readStatus();
      }

      temp = _bme680.readTemperature();
      pres = _bme680.readPressure();
      hum =  _bme680.readHumidity();
      
      //initial trigger of measurement at the beginning of the averaging loop
      _bme680.setForcedMode();
      _delay_ms(200);
      status = _bme680.readStatus();
     
      gas = 0;
      for (uint8_t c = 0; c < AVG_COUNT; c++) {
        while (! (status.newDataFlag == 1)) {
          _bme680.setForcedMode();
          DPRINT(".");
          _delay_ms(100);
          status = _bme680.readStatus();
        }
        // need to check whether first reading belongs to previous quintuple reading (last reading). Measurement results are indicating this. 
        ClosedCube_BME680_gas_r_lsb gas_status = _bme680.read_gas_r_lsb();
        uint32_t _g = _bme680.readGasResistance();
        
#ifdef DEEP_DEBUG
        DPRINT(F("Gas heat_stab_r   = "));DDECLN(gas_status.heat_stab_r);
        DPRINT(F("Gas gas_valid_r   = "));DDECLN(gas_status.gas_valid_r);

        DPRINT("index: ");DDECLN(c);
        DPRINT("gas: ");DDECLN(_g);
#endif
        
        gas  += _g;
        status = _bme680.readStatus();
      }
      
      gas /= AVG_COUNT;
      gas_raw = gas;  // without any limiting
      DPRINT("avg gas: ");DDECLN(gas);
      
      
      // peak detectors for min/max ever measured gas resistances since last reset
      
      if ( gas > _max_gas_resistance) {   // capture maximum of ever measured gas resistances since last reset
        _max_gas_resistance = gas;
        if ( _max_gas_resistance >= _min_gas_resistance ) {
            
            // set lower limit for decay of _gas_upper_limit; _max_decay_factor_upper_limit is typically set to 70 as WebUI device parameter
            _gas_upper_limit_min = _min_gas_resistance + (_max_gas_resistance - _min_gas_resistance) * (int32_t)_max_decay_factor_upper_limit / 100;
            
            // set upper limit for increase of _gas_lower_limit; _max_increase_factor_lower_limit is typically set to 30 as WebUI device parameter
            _gas_lower_limit_max = _min_gas_resistance + (_max_gas_resistance - _min_gas_resistance) * (int32_t)_max_increase_factor_lower_limit / 100;
            
        }
      }
      
      if ( gas < _min_gas_resistance) {   // capture minimum of ever measured gas resistances since last reset
        _min_gas_resistance = gas;
         if ( _max_gas_resistance >= _min_gas_resistance ) {
             
            // set lower limit for decay of _gas_upper_limit; _max_decay_factor_upper_limit is typically set to 70 as WebUI device parameter
            _gas_upper_limit_min = _min_gas_resistance + (_max_gas_resistance - _min_gas_resistance) * (int32_t)_max_decay_factor_upper_limit / 100;
            
            // set upper limit for increase of _gas_lower_limit; _max_increase_factor_lower_limit is typically set to 30 as WebUI device parameter
            _gas_lower_limit_max = _min_gas_resistance + (_max_gas_resistance - _min_gas_resistance) * (int32_t)_max_increase_factor_lower_limit / 100;
            
        }
      }
      
      //peak detector for _gas_upper_limit    (CCU Historian datapoint parameter AQ_GAS_RESISTANCE_MAX)
      
      if ( gas > _gas_upper_limit )
      {
        _gas_upper_limit = gas;
      }
      else
      {
        _gas_upper_limit = _gas_upper_limit - (_gas_upper_limit - _gas_lower_limit) * IIR_FILTER_COEFFICIENT; // decay each sample by IIR_FILTER_COEFFICIENT * (max-min)
        if ( _gas_upper_limit < _gas_upper_limit_min ) {
          // limit decay of _gas_upper_limit to _gas_upper_limit_min
          _gas_upper_limit = _gas_upper_limit_min; // lower limit for _gas_upper_limit
        }
      }
      
      //peak detector for _gas_lower_limit  (CCU Historian datapoint parameter AQ_GAS_RESISTANCE_MIN)
      
      if ( gas < _gas_lower_limit )
      {
        _gas_lower_limit = gas;
      }
      else
      {
        _gas_lower_limit = _gas_lower_limit + (_gas_upper_limit - _gas_lower_limit) * IIR_FILTER_COEFFICIENT; // increase each sample by IIR_FILTER_COEFFICIENT * (max-min)
        if ( _gas_lower_limit > _gas_lower_limit_max ) {
          // limit increase of _gas_lower_limit to _gas_lower_limit_max
          _gas_lower_limit = _gas_lower_limit_max; // upper limit for _gas_lower_limit
        }
      }
      
      //Offset compensation
      temp = temp + tempOffset;
      hum  = hum + humidOffset;
      pres = pres + pressOffset;
      
      //integer representations
      _temperature = (int16_t)(temp * 10.0);
      _pressureNN  = (uint16_t)(EquivalentSeaLevelPressure(float(_height), temp, pres)*10.0);
      _humidity    = (uint16_t)(hum * 10.0);
      
      DPRINT(F("Gas UPPER LIMIT   = "));DDECLN(_gas_upper_limit);
      DPRINT(F("Gas LOWER LIMIT   = "));DDECLN(_gas_lower_limit);

      DPRINT(F("T   = "));DDECLN(_temperature);
#ifdef DEEP_DEBUG
      DPRINT(F("P   = "));DDECLN(pres); 
#endif
      DPRINT(F("PNN = "));DDECLN(_pressureNN);
      DPRINT(F("Hum = "));DDECLN(_humidity);
      DPRINT(F("Gas = "));DDECLN(gas);

      /*
       This software, the ideas and concepts is Copyright (c) David Bird 2018. All rights to this software are reserved.

       Any redistribution or reproduction of any part or all of the contents in any form is prohibited other than the following:
       1. You may print or download to a local hard disk extracts for your personal and non-commercial use only.
       2. You may copy the content to individual third parties for their personal use, but only if you acknowledge the author David Bird as the source of the material.
       3. You may not, except with my express written permission, distribute or commercially exploit the content.
       4. You may not transmit it or store it in any other website or other form of electronic retrieval system for commercial purposes.

       The above copyright ('as annotated') notice and this permission notice shall be included in all copies or substantial portions of the Software and where the
       software use is visible to an end-user.

       THE SOFTWARE IS PROVIDED "AS IS" FOR PRIVATE USE ONLY, IT IS NOT FOR COMMERCIAL USE IN WHOLE OR PART OR CONCEPT. FOR PERSONAL USE IT IS SUPPLIED WITHOUT WARRANTY
       OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
       IN NO EVENT SHALL THE AUTHOR OR COPYRIGHT HOLDER BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
       FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
       See more at http://www.dsbird.org.uk
      */
     
      float _gas_score = 0.0;

      // calculate the normalized air quality level = AQ_LEVEL datapoint parameter in CCU Historian
      
      _gas_score = (float)(gas - _gas_lower_limit)/(float)(_gas_upper_limit - _gas_lower_limit) * 100.0;
      
      if ( _gas_score > 100.0 )
        _gas_score = 100.0;

      _aqLevel = (uint8_t)(_gas_score);
      //DPRINT(F("AQ% = "));DDECLN(_aqLevel);
      DPRINT(F("AQ  = "));DDEC((uint8_t)_gas_score);DPRINTLN(F("%"));
      _bme680.setForcedMode();
      
      // calculate the absolute humidity
      // see for detils https://www.kompf.de/weather/vent.html or  https://rechneronline.de/barometer/luftfeuchtigkeit.php for x-checking the calculated result
      tt = (17.67*temp)/(243.5+temp);
      ttt = exp(tt);
      svp = 6.112 * ttt;
      vp = ((1.0 * hum)/100.0)*svp;
      // 18.16 kg/mol Molekulargewicht des Wasserdampfs
      // 8314.3 J/(kmol*K) universelle Gaskonstante
      ah = (100000.0*(18.016/8314.3)*vp)/(temp + 273.15);
#ifdef DEEP_DEBUG
      DPRINT(F("rLF  = "));  DDEC(hum);  DPRINTLN(F(" %"));
      DPRINT(F("Temp  = ")); DDEC(temp); DPRINTLN(F(" deg C"));
      DPRINT(F("aLF  = "));  DDEC(ah);   DPRINTLN(F(" g/m^3"));
#endif
      
      //calculate the multi linear regression prediction
      
      mlr_pred = _mlr_delta + _mlr_alpha * temp + _mlr_beta * ah;
      
      residual = gas_raw - mlr_pred;
      
#ifdef DEEP_DEBUG
      DPRINT(F("previous minimum residual  = "));DDEC(_min_res);DPRINTLN(F(" "));
      DPRINT(F("previous maximum residual  = "));DDEC(_max_res);DPRINTLN(F(" "));
#endif
      
      
      // peak detectors for min/max ever calculated residual since last reset
      
      
      if ( residual > _max_res) {   // capture maximum of ever calculated residual gas resistances since last reset
        _max_res = residual;
         if ( _max_res > _min_res ) {
             
             // set lower limit for decay of _res_upper_limit; _max_decay_factor_upper_limit is typically set to 70 as WebUI device parameter
            _res_upper_limit_min = _min_res + (_max_res - _min_res) * (int32_t)_max_decay_factor_upper_limit / 100;
            
            // set upper limit for increase of _res_lower_limit; _max_increase_factor_lower_limit is typically set to 30 as WebUI device parameter
            _res_lower_limit_max = _min_res + (_max_res - _min_res) * (int32_t)_max_increase_factor_lower_limit / 100;
            
        }
      }
      if ( residual < _min_res) {   // capture minimum of ever calculated residual gas resistances since last reset
        _min_res = residual;
        if ( _max_res > _min_res ) {
            
            // set lower limit for decay of _res_upper_limit; _max_decay_factor_upper_limit is typically set to 70 as WebUI device parameter
            _res_upper_limit_min = _min_res + (_max_res - _min_res) * (int32_t)_max_decay_factor_upper_limit / 100;
            
            // set upper limit for increase of _res_lower_limit; _max_increase_factor_lower_limit is typically set to 30 as WebUI device parameter
            _res_lower_limit_max = _min_res + (_max_res - _min_res) * (int32_t)_max_increase_factor_lower_limit / 100;
            
        }
      }
      
      //peak detector for _res_upper_limit 
      if ( residual > _res_upper_limit )
      {
        _res_upper_limit = residual;
       
      }
      else
      {
        _res_upper_limit = _res_upper_limit - (_res_upper_limit - _res_lower_limit) * IIR_FILTER_COEFFICIENT; // decay each sample by IIR_FILTER_COEFFICIENT * (max-min)
        // limit decay of _res_upper_limit to _res_upper_limit_min
        if ( _res_upper_limit < _res_upper_limit_min ) {
          _res_upper_limit = _res_upper_limit_min; // lower limit for _res_upper_limit
        }
      }
      
      //peak detector for _res_lower_limit 
      if ( residual < _res_lower_limit )
      {
        _res_lower_limit = residual;
      }
      else
      {
        _res_lower_limit = _res_lower_limit + (_res_upper_limit - _res_lower_limit) * IIR_FILTER_COEFFICIENT; // increase each sample by IIR_FILTER_COEFFICIENT * (max-min)
        // limit increase of _res_lower_limit to _res_lower_limit_max
        if ( _res_lower_limit > _res_lower_limit_max ) {
          _res_lower_limit = _res_lower_limit_max; // upper limit for _res_lower_limit
        }
      }
      
      normalized_residual=((residual - _min_res)/(_max_res - _min_res));
      
      // limit minimum of normalized_residual to EPSILON
      if ( normalized_residual < EPSILON)
      { normalized_residual = EPSILON;
      }
      
      normalized_residual = normalized_residual * 100.0;
      
      log_normalized_residual = -1.0 * (log10(normalized_residual) - 2.0);
      
      _aqState_scaled = (uint16_t)(log_normalized_residual*10000.0); //0.0..40000.0
      _gas_resistance_raw_scaled = (uint16_t) (gas_raw / 20);
      _gas_resistance_min_scaled = (uint16_t) (_gas_lower_limit / 20);
      _gas_resistance_max_scaled = (uint16_t) (_gas_upper_limit / 20);
      
#ifdef DEEP_DEBUG
      DPRINT(F("raw gas resistance  = "));DDEC(gas_raw);DPRINTLN(F(" "));
      DPRINT(F("multiple linear regression prediction  = "));DDEC(mlr_pred);DPRINTLN(F(" "));
      DPRINT(F("residual  = "));DDEC(residual);DPRINTLN(F(" "));
      DPRINT(F("minimum residual  = "));DDEC(_min_res);DPRINTLN(F(" "));
      DPRINT(F("maximum residual  = "));DDEC(_max_res);DPRINTLN(F(" "));
      DPRINT(F("normalized residual  = "));DDEC(normalized_residual);DPRINTLN(F(" "));
      DPRINT(F("air quality level  = "));DDEC(log_normalized_residual);DPRINTLN(F(" "));
      DPRINT(F("Gas UPPER LIMIT   = "));DDECLN(_gas_upper_limit);
      DPRINT(F("Gas LOWER LIMIT   = "));DDECLN(_gas_lower_limit);
      DPRINT(F("air quality level scaled= "));DDEC(_aqState_scaled);DPRINTLN(F(" "));
      DPRINT(F("_gas_resistance_raw_scaled= "));DDEC(_gas_resistance_raw_scaled);DPRINTLN(F(" "));
      DPRINT(F("_gas_resistance_min_scaled= "));DDEC(_gas_resistance_min_scaled);DPRINTLN(F(" ")); 
      DPRINT(F("_gas_resistance_max_scaled= "));DDEC(_gas_resistance_max_scaled);DPRINTLN(F(" "));
#endif
    }


  }
  
  int16_t   temperature ()                  { return _temperature; }
  uint16_t  pressureNN ()                   { return _pressureNN; }
  uint16_t  humidity ()                     { return _humidity; }                     // 0..100%
  uint8_t   aq_level ()                     { return _aqLevel; }                      // 0..100%
  uint16_t  aq_state_scaled ()              { return _aqState_scaled; }               // 0..40000, mul 10000!
  uint16_t  gas_resistance_raw_scaled ()    { return _gas_resistance_raw_scaled; }    // 0..65365; div 20!      CCU Historian datapoint parameter AQ_GAS_RESISTANCE_RAW
  uint16_t  gas_resistance_min_scaled ()    { return _gas_resistance_min_scaled; }    // 0..65365; div 20!    	CCU Historian datapoint parameter AQ_GAS_RESISTANCE_MIN
  uint16_t  gas_resistance_max_scaled ()    { return _gas_resistance_max_scaled; }    // 0..65365; div 20!      CCU Historian datapoint parameter AQ_GAS_RESISTANCE_MAX
};

}

#endif
