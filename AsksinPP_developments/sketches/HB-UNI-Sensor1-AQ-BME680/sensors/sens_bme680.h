// based on https://github.com/G6EJD/BME680-Example
// https://github.com/pimoroni/bme680-python/blob/master/examples/indoor-air-quality.py

#ifndef _SENS_BME680_H_
#define _SENS_BME680_H_

#define DEEP_DEBUG // comment out if deep serial monitor debugging is not necessary

#include <Wire.h>
#include <Sensors.h>
#include <ClosedCube_BME680.h>
#include <math.h>
#include <EEPROM.h>


/*
 * Gas is returned as a resistance value in ohms.
 * Once it stabilizes, you can use that as your baseline reading.
 * Higher concentrations of VOC will make the resistance lower.
 */

#define AVG_COUNT                   5
#define IIR_FILTER_COEFFICIENT      0.0001359 // 1.0 -0.9998641 ; Decay to 0.71 in about one week for a 4 min sampling period (in 2520 sampling periods)
#define EPSILON                     0.0001
#define MAX_BATTERY_VOLTAGE         3300
#define EEPROM_START_ADDRESS        100
#define DEVICE_TYPE                 "HB-UNI-Sensor1-AQ-BME680"
#define FINISH_STRING               "/HB-UNI-Sensor1-AQ-BME680"
#define STORE_TO_EEPROM_NO_CYCLES   360                              // store parameters once a day; 360 * 4 min


namespace as {

template <uint8_t ADDRESS=0x76>  // I2C address needs to be be set according to your BME680 sensor breakout in the main sketch HB-UNI-Sen-AQ.ino: 'Sens_Bme680<0x76>   bme680;' not here!
class Sens_Bme680 : public Sensor {
private:
  int16_t   _temperature;
  uint16_t  _pressureNN;
  uint16_t  _humidity;
  uint16_t  _aqLevel;
  uint16_t  _aqState_scaled;
  int32_t   _max_gas_resistance;  // maximum measured gas resistance
  int32_t   _min_gas_resistance;  // minimum measured gas resistance
  int32_t   _gas_lower_limit;     // adaptive lower resistance level for bad air quality 
  int32_t   _gas_upper_limit;     // adaptive upper resistance level for good air quality
  int32_t   _res_lower_limit;     // adaptive lower residual level for bad air quality 
  int32_t   _res_upper_limit;     // adaptive upper residual level for good air quality
  int32_t   _min_res, _max_res;   // adaptive min and max residual of multiple linear regression used for normalization
  int16_t   _gas_resistance_raw_scaled;
  int16_t   _gas_resistance_min_scaled;
  int16_t   _gas_resistance_max_scaled;
  uint16_t  _height;
  int32_t   _gas_upper_limit_min;
  int32_t   _gas_lower_limit_max;
  int32_t   _res_upper_limit_min;
  int32_t   _res_lower_limit_max;
  uint8_t   _max_decay_factor_upper_limit;
  uint8_t   _max_increase_factor_lower_limit;
  double    _mlr_alpha; //multiple linear regression coefficient alpha (temperature)
  double    _mlr_beta;  //multiple linear regression coefficient beta (absolute humidity)
  double    _mlr_delta; //multiple linear regression coefficient delta (offset)
  uint16_t  measurement_index;

  ClosedCube_BME680 _bme680;
public:

  Sens_Bme680 (): _temperature(0),  _pressureNN(0), _humidity(0), _aqLevel(0), _aqState_scaled(0), _gas_resistance_raw_scaled(0), _gas_resistance_min_scaled(0), _gas_resistance_max_scaled(0) {}
    ~Sens_Bme680 () {}

  void init (uint16_t height, uint8_t max_decay_factor_upper_limit, uint8_t max_increase_factor_lower_limit, double mlr_alpha, double mlr_beta, double mlr_delta) {

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
    _min_gas_resistance =  2000000;     // initial value
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
    _max_decay_factor_upper_limit    = max_decay_factor_upper_limit;
    _max_increase_factor_lower_limit = max_increase_factor_lower_limit;
    _mlr_alpha = mlr_alpha;
    _mlr_beta  = mlr_beta;
    _mlr_delta = mlr_delta;
    measurement_index = 0;
    
#ifdef DEEP_DEBUG
    DPRINT(F("Gas UPPER LIMIT                    = "));DDECLN(_gas_upper_limit);
    DPRINT(F("Gas LOWER LIMIT                    = "));DDECLN(_gas_lower_limit);
    DPRINT(F("_gas_lower_limit_max               = "));DDECLN(_gas_lower_limit_max);
    DPRINT(F("_gas_upper_limit_min               = "));DDECLN(_gas_upper_limit_min);
    DPRINT(F("_max_decay_factor_upper_limit      = "));DDECLN(_max_decay_factor_upper_limit);
    DPRINT(F("_max_increase_factor_lower_limit   = "));DDECLN(_max_increase_factor_lower_limit);
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
  
  // check whether batteries have been replaced
  // if yes, restore important device parameters from EEPROM
  // if no, store important device parameters to EEPROM every 360th measurement cycle, typically once a day
  bool check_for_battery_change(uint16_t index, uint16_t operatingVoltage1000) {
    
    bool battery_change_flag = false;
    
    int      address            = EEPROM_START_ADDRESS; // EEPROM starting address
    uint16_t EEPROM_data_length = 0;                    // length of EEPROM data
    uint16_t EEPROM_index;
    uint16_t EEPROM_operatingVoltage1000;
    uint8_t  EEPROM_max_decay_factor_upper_limit;
    uint8_t  EEPROM_max_increase_factor_lower_limit;
    int32_t  EEPROM_max_gas_resistance;
    int32_t  EEPROM_min_gas_resistance;
    int32_t  EEPROM_gas_upper_limit_min;
    int32_t  EEPROM_gas_lower_limit_max;
    int32_t  EEPROM_min_res;
    int32_t  EEPROM_max_res;
    int32_t  EEPROM_res_upper_limit_min;
    int32_t  EEPROM_res_lower_limit_max;
    double   EEPROM_mlr_alpha;
    double   EEPROM_mlr_beta;
    double   EEPROM_mlr_delta;
    
    //check for previous reset ( index is then 0)
    if ( index == 0 ) {
      // check if device is battery supplied, i.e. not supplied by ISP Programmer or FTDI Debugger
        
      if ( operatingVoltage1000 <= MAX_BATTERY_VOLTAGE ) {
        //battery has been changed
        battery_change_flag = true;
        //restore device parameters from EEPROM
        
        char buffer1[100]        = {0}; // EEPROM read buffer; start string
        char buffer2[100]        = {0}; // EEPROM read buffer; stop string
#ifdef DEEP_DEBUG
        DPRINT(F("Restored identification string     = "));DPRINTLN(buffer1);
#endif
        // read EEPROM data
        EEPROM.get(address, buffer1);
        DPRINT(F("Restored identification string     = "));DPRINTLN(buffer1);
        
        if (strcmp(buffer1, DEVICE_TYPE) == 0) {
          DPRINT(F("Matching identification string     = "));DPRINTLN(buffer1);
          address += sizeof(DEVICE_TYPE);
          
          EEPROM.get(address, EEPROM_data_length);
          address += sizeof(EEPROM_data_length);
          
          EEPROM.get(address, EEPROM_index);
          address += sizeof(EEPROM_index);
          
          EEPROM.get(address, EEPROM_operatingVoltage1000);
          address += sizeof(EEPROM_operatingVoltage1000);
          
          EEPROM.get(address, EEPROM_max_decay_factor_upper_limit);
          address += sizeof(EEPROM_max_decay_factor_upper_limit);
          
          EEPROM.get(address, EEPROM_max_increase_factor_lower_limit);
          address += sizeof(EEPROM_max_increase_factor_lower_limit);
          
          EEPROM.get(address, EEPROM_max_gas_resistance);
          address += sizeof(EEPROM_max_gas_resistance);
          
          EEPROM.get(address, EEPROM_min_gas_resistance);
          address += sizeof(EEPROM_min_gas_resistance);
          
          EEPROM.get(address, EEPROM_gas_upper_limit_min);
          address += sizeof(EEPROM_gas_upper_limit_min);
          
          EEPROM.get(address, EEPROM_gas_lower_limit_max);
          address += sizeof(EEPROM_gas_lower_limit_max);
          
          EEPROM.get(address, EEPROM_min_res);
          address += sizeof(EEPROM_min_res);
          
          EEPROM.get(address, EEPROM_max_res);
          address += sizeof(EEPROM_max_res);
          
          EEPROM.get(address, EEPROM_res_upper_limit_min);
          address += sizeof(EEPROM_res_upper_limit_min);
          
          EEPROM.get(address, EEPROM_res_lower_limit_max);
          address += sizeof(EEPROM_res_lower_limit_max);
          
          EEPROM.get(address, EEPROM_mlr_alpha);
          address += sizeof(EEPROM_mlr_alpha);
          
          EEPROM.get(address, EEPROM_mlr_beta);
          address += sizeof(EEPROM_mlr_beta);
          
          EEPROM.get(address, EEPROM_mlr_delta);
          address += sizeof(EEPROM_mlr_delta);
          
          EEPROM.get(address, buffer2);
          DPRINT(F("Restored finish string             = "));DPRINTLN(buffer2);
         
          if (strcmp(buffer2, FINISH_STRING) == 0) {
            DPRINT(F("Matching finish string             = "));DPRINTLN(buffer2);
            
            // restore actual parameters from read EEPROM data
#ifdef DEEP_DEBUG
            DPRINT(F("EEPROM_data_length                      = "));DDECLN(EEPROM_data_length);
            DPRINT(F("EEPROM_index                            = "));DDECLN(EEPROM_index);
            DPRINT(F("EEPROM_operatingVoltage1000             = "));DDECLN(EEPROM_operatingVoltage1000);
            DPRINT(F("EEPROM_max_decay_factor_upper_limit     = "));DDECLN(EEPROM_max_decay_factor_upper_limit);
            DPRINT(F("EEPROM_max_increase_factor_lower_limit  = "));DDECLN(EEPROM_max_increase_factor_lower_limit);
            DPRINT(F("EEPROM_max_gas_resistance               = "));DDECLN(EEPROM_max_gas_resistance);
            DPRINT(F("EEPROM_min_gas_resistance               = "));DDECLN(EEPROM_min_gas_resistance);
            DPRINT(F("EEPROM_gas_upper_limit_min              = "));DDECLN(EEPROM_gas_upper_limit_min);
            DPRINT(F("EEPROM_gas_lower_limit_max              = "));DDECLN(EEPROM_gas_lower_limit_max);
            DPRINT(F("EEPROM_min_res                          = "));DDECLN(EEPROM_min_res);
            DPRINT(F("EEPROM_max_res                          = "));DDECLN(EEPROM_max_res);
            DPRINT(F("EEPROM_res_upper_limit_min              = "));DDECLN(EEPROM_res_upper_limit_min);
            DPRINT(F("EEPROM_res_lower_limit_max              = "));DDECLN(EEPROM_res_lower_limit_max);
            DPRINT(F("EEPROM_data_length                      = "));DDECLN(EEPROM_data_length);
            DPRINT(F("EEPROM_data_length                      = "));DDECLN(EEPROM_data_length);
#endif 
            // restore parameters from EEPROM data
            _max_decay_factor_upper_limit    = EEPROM_max_decay_factor_upper_limit;
            _max_increase_factor_lower_limit = EEPROM_max_increase_factor_lower_limit;
            _max_gas_resistance              = EEPROM_max_gas_resistance;
            _min_gas_resistance              = EEPROM_min_gas_resistance;
            _gas_upper_limit_min             = EEPROM_gas_upper_limit_min;
            _gas_lower_limit_max             = EEPROM_gas_lower_limit_max;
            _min_res                         = EEPROM_min_res;
            _max_res                         = EEPROM_max_res;
            _res_upper_limit_min             = EEPROM_res_upper_limit_min;
            _res_lower_limit_max             = EEPROM_res_lower_limit_max;
            _mlr_alpha                       = EEPROM_mlr_alpha;
            _mlr_beta                        = EEPROM_mlr_beta;
            _mlr_delta                       = EEPROM_mlr_delta;
          }
          else
          {
            DPRINTLN("Finish string does not match!");
            DPRINTLN("Restore of EEPROM data is aborted!");
          }
        }
        else
        {
          DPRINTLN("Start string does not match!");
          DPRINTLN("Restore of EEPROM data is aborted!");
        }
      }
      else {
        // device is powered by ISP Programmer or FTDI Debugger (VCC > MAX_BATTERY_VOLTAGE)
        DPRINT(F("Device is not battery supplied     = "));DDECLN(operatingVoltage1000);
        DPRINTLN("No restore of EEPROM data is done!");
      }
    }
    else
    { 
        // store device parameters in EEPROM every (STORE_TO_EEPROM_NO_CYCLES)th measurement (typically once a day)
        if ((measurement_index % STORE_TO_EEPROM_NO_CYCLES) == 0 ) {
          DPRINTLN("Saving important device parameters into EEPROM that should be persistent");
          DPRINT(F("measurement_index                  = "));DDECLN(index);
          
          char Device_type[] = DEVICE_TYPE;

          EEPROM.put(address, Device_type );                       // writing Device_type string
          address += sizeof(Device_type);                          // add size of string Device_type
          
          address = address + 2;                                   // reserve two bytes for length of EEPROM data
          
          EEPROM.put(address, index);                              // writing measurement index value
          address += sizeof(uint16_t);                             // add size of uint16_t value
          
          EEPROM.put(address, operatingVoltage1000);               // writing operatingVoltage1000 value
          address += sizeof(uint16_t);                             // add size of uint16_t value
           
          EEPROM.put(address, _max_decay_factor_upper_limit);      // writing _max_decay_factor_upper_limit value
          address += sizeof(uint8_t);                              // add size of uint8_t value
          
          EEPROM.put(address, _max_increase_factor_lower_limit);   // writing _max_increase_factor_lower_limit value
          address += sizeof(uint8_t);                              // add size of uint8_t value
          
          EEPROM.put(address, _max_gas_resistance);                // writing _max_gas_resistance value
          address += sizeof(int32_t);                              // add size of int32_t value
          
          EEPROM.put(address, _min_gas_resistance);                // writing _min_increase_factor_lower_limit value
          address += sizeof(int32_t);                              // add size of int32_t value
          
          EEPROM.put(address, _gas_upper_limit_min);               // writing _gas_upper_limit_min value
          address += sizeof(int32_t);                              // add size of int32_t value         
          
          EEPROM.put(address, _gas_lower_limit_max);               // writing _gas_lower_limit_max value
          address += sizeof(int32_t);                              // add size of int32_t value  
          
          EEPROM.put(address, _min_res);                           // writing _min_res value
          address += sizeof(int32_t);                              // add size of int32_t value
          
          EEPROM.put(address, _max_res);                           // writing _max_res value
          address += sizeof(int32_t);                              // add size of uint32_t value
        
          EEPROM.put(address, _res_upper_limit_min);               // writing _res_upper_limit_min value
          address += sizeof(int32_t);                              // add size of int32_t value         
          
          EEPROM.put(address, _res_lower_limit_max);               // writing _res_lower_limit_max value
          address += sizeof(int32_t);                              // add size of int32_t value
          
          EEPROM.put(address, _mlr_alpha);                         // writing _mlr_alpha value
          address += sizeof(double);                               // add size of double value
          
          EEPROM.put(address, _mlr_beta);                          // writing _mlr_beta value
          address += sizeof(double);                               // add size of double value
          
          EEPROM.put(address, _mlr_delta);                         // writing _mlr_delta value
          address += sizeof(double);                               // add size of double value
          
          EEPROM.put(address, FINISH_STRING);                      // writing FINISH_STRING value
          address += sizeof(FINISH_STRING);                        // add size of sting FINISH_STRING
          
          address = EEPROM_START_ADDRESS + sizeof(Device_type);    // write length of EEPROM data value
          EEPROM.put(address, (uint16_t)(address - EEPROM_START_ADDRESS));
          DPRINT(F("Number of Bytes written to EEPROM  = "));DDECLN((uint16_t)(address - EEPROM_START_ADDRESS));
        }
    }
          
      
    return battery_change_flag;
  }


  void measure (double tempOffset, double pressOffset, double humidOffset, uint8_t max_decay_factor_upper_limit, uint8_t max_increase_factor_lower_limit, double mlr_alpha, double mlr_beta, double mlr_delta, uint16_t operatingVoltage1000) {
    if (_present == true) {
      double  temp(NAN), hum(NAN), pres(NAN);                                 // use type double in order to match the return type of closed cubes's library function readPressure
      double  ah,tt,ttt,vp,svp;                                               // variables for calculating the absolute humidity
      double  log_normalized_residual,normalized_residual,mlr_pred;           // double variables for calculating the air quality level
      int32_t residual;                                                       // int32_t variables for calculating the air quality level
      int32_t gas = 0;
      double  gas_raw;
      bool    batteries_were_changed;
      
      _max_decay_factor_upper_limit    = max_decay_factor_upper_limit;
      _max_increase_factor_lower_limit = max_increase_factor_lower_limit;
      _mlr_alpha = mlr_alpha;
      _mlr_beta  = mlr_beta;
      _mlr_delta = mlr_delta;
    
        
      batteries_were_changed = check_for_battery_change(measurement_index, operatingVoltage1000);
      DPRINT(F("Batteries change detected :        = "));DDECLN(batteries_were_changed);

      measurement_index = measurement_index + 1; // increase measurement index by 1

    
      // Ensure that _max_decay_factor_upper_limit is bigger than _max_increase_factor_lower_limit
      if (_max_decay_factor_upper_limit <= _max_increase_factor_lower_limit) {
          _max_decay_factor_upper_limit = _max_increase_factor_lower_limit + 1;
      }
      
#ifdef DEEP_DEBUG
      DPRINT(F("\n\nmeasurement_index                  = "));DDECLN(measurement_index);
      DPRINT(F("operatingVoltage1000               = "));DDECLN(operatingVoltage1000);
      DPRINT(F("_max_decay_factor_upper_limit      = "));DDECLN(_max_decay_factor_upper_limit);
      DPRINT(F("_max_increase_factor_lower_limit   = "));DDECLN(_max_increase_factor_lower_limit);
      DPRINT(F("mlr_alpha                          = "));DDECLN(mlr_alpha);
      DPRINT(F("mlr_beta                           = "));DDECLN(mlr_beta);
      DPRINT(F("mlr_delta                          = "));DDECLN(mlr_delta);
      DPRINT(F("_mlr_alpha                         = "));DDECLN(_mlr_alpha);
      DPRINT(F("_mlr_beta                          = "));DDECLN(_mlr_beta);
      DPRINT(F("_mlr_delta                         = "));DDECLN(_mlr_delta);
#endif
  

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
      
      gas      /= AVG_COUNT;
      gas_raw   = (double)gas;  // without any limiting
      DPRINT("avg gas: ");DDECLN(gas);
      
      
      // peak detectors for min/max ever measured gas resistances since last reset
      
      if ( gas > _max_gas_resistance) {   // capture maximum of ever measured gas resistances since last reset
        _max_gas_resistance = gas;
        if ( _max_gas_resistance >= _min_gas_resistance ) {
            
            // set lower limit for decay of _gas_upper_limit; _max_decay_factor_upper_limit is typically set to 70 as WebUI device parameter
            _gas_upper_limit_min = _min_gas_resistance + (_max_gas_resistance - _min_gas_resistance) * (int32_t)_max_decay_factor_upper_limit / 100;
            
            // set upper limit for increase of _gas_lower_limit; _max_increase_factor_lower_limit is typically set to 30 as WebUI device parameter
            _gas_lower_limit_max = _min_gas_resistance + (_max_gas_resistance - _min_gas_resistance) * (int32_t)_max_increase_factor_lower_limit / 100;
#ifdef DEEP_DEBUG
            DPRINT(F("_min_gas_resistance    = "));DDECLN(_min_gas_resistance);
            DPRINT(F("_max_gas_resistance    = "));DDECLN(_max_gas_resistance);
            DPRINT(F("_gas_upper_limit_min   = "));DDECLN(_gas_upper_limit_min);
            DPRINT(F("_gas_lower_limit_max   = "));DDECLN(_gas_lower_limit_max);
#endif            
        }
      }
      
      if ( gas < _min_gas_resistance) {   // capture minimum of ever measured gas resistances since last reset
        _min_gas_resistance = gas;
         if ( _max_gas_resistance >= _min_gas_resistance ) {
             
            // set lower limit for decay of _gas_upper_limit; _max_decay_factor_upper_limit is typically set to 70 as WebUI device parameter
            _gas_upper_limit_min = _min_gas_resistance + (_max_gas_resistance - _min_gas_resistance) * (int32_t)_max_decay_factor_upper_limit / 100;
            
            // set upper limit for increase of _gas_lower_limit; _max_increase_factor_lower_limit is typically set to 30 as WebUI device parameter
            _gas_lower_limit_max = _min_gas_resistance + (_max_gas_resistance - _min_gas_resistance) * (int32_t)_max_increase_factor_lower_limit / 100;
#ifdef DEEP_DEBUG
            DPRINT(F("_min_gas_resistance    = "));DDECLN(_min_gas_resistance);
            DPRINT(F("_max_gas_resistance    = "));DDECLN(_max_gas_resistance);
            DPRINT(F("_gas_upper_limit_min   = "));DDECLN(_gas_upper_limit_min);
            DPRINT(F("_gas_lower_limit_max   = "));DDECLN(_gas_lower_limit_max);
#endif 
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

      DPRINT(F("T                 = "));DDECLN(_temperature);
#ifdef DEEP_DEBUG
      DPRINT(F("P                 = "));DDECLN(pres); 
#endif
      DPRINT(F("PNN               = "));DDECLN(_pressureNN);
      DPRINT(F("Hum               = "));DDECLN(_humidity);
      DPRINT(F("Gas               = "));DDECLN(gas);

     
      float _gas_score = 0.0;

      // calculate the normalized air quality level = AQ_LEVEL datapoint parameter in CCU Historian
      
      _gas_score = (float)(gas - _gas_lower_limit)/(float)(_gas_upper_limit - _gas_lower_limit) * 100.0;
      
      if ( _gas_score > 100.0 )
        _gas_score = 100.0;

      _aqLevel = (uint8_t)(_gas_score);
      //DPRINT(F("AQ% = "));DDECLN(_aqLevel);
      DPRINT(F("AQ                = "));DDEC((uint8_t)_gas_score);DPRINTLN(F("%"));
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
      DPRINT(F("rLF               = "));  DDEC(hum);  DPRINTLN(F(" %"));
      DPRINT(F("Temp              = ")); DDEC(temp); DPRINTLN(F(" deg C"));
      DPRINT(F("aLF               = "));  DDEC(ah);   DPRINTLN(F(" g/m^3"));
#endif
      
      //calculate the multi linear regression prediction in double precision
      mlr_pred = _mlr_delta + _mlr_alpha * temp + _mlr_beta * ah;
      
      //calculate residual and convert to int32_t
      residual = (int32_t)(gas_raw - mlr_pred);
      
#ifdef DEEP_DEBUG
      DPRINT(F("previous minimum residual  = "));DDEC(_min_res);DPRINTLN(F(" "));
      DPRINT(F("previous maximum residual  = "));DDEC(_max_res);DPRINTLN(F(" "));
#endif
      
      
      // peak detectors for min/max ever calculated residual since last reset
      
      
      if ( residual > _max_res) {   // capture maximum of ever calculated residual gas resistances since last reset
        _max_res = residual;
         if ( _max_res > _min_res ) {
             
             // set lower limit for decay of _res_upper_limit_min; _max_decay_factor_upper_limit is typically set to 70 as WebUI device parameter
            _res_upper_limit_min = _min_res + (_max_res - _min_res) * (int32_t)_max_decay_factor_upper_limit / 100;
            
            // set upper limit for increase of _res_lower_limit_max; _max_increase_factor_lower_limit is typically set to 30 as WebUI device parameter
            _res_lower_limit_max = _min_res + (_max_res - _min_res) * (int32_t)_max_increase_factor_lower_limit / 100;
#ifdef DEEP_DEBUG
            DPRINT(F("_min_res               = "));DDECLN(_min_res);
            DPRINT(F("_max_res               = "));DDECLN(_max_res);
            DPRINT(F("_res_upper_limit_min   = "));DDECLN(_res_upper_limit_min);
            DPRINT(F("_res_lower_limit_max   = "));DDECLN(_res_lower_limit_max);
#endif
        }
      }
      if ( residual < _min_res) {   // capture minimum of ever calculated residual gas resistances since last reset
        _min_res = residual;
        if ( _max_res > _min_res ) {
            
            // set lower limit for decay of _res_upper_limit_min; _max_decay_factor_upper_limit is typically set to 70 as WebUI device parameter
            _res_upper_limit_min = _min_res + (_max_res - _min_res) * (int32_t)_max_decay_factor_upper_limit / 100;
            
            // set upper limit for increase of _res_lower_limit_max; _max_increase_factor_lower_limit is typically set to 30 as WebUI device parameter
            _res_lower_limit_max = _min_res + (_max_res - _min_res) * (int32_t)_max_increase_factor_lower_limit / 100;
#ifdef DEEP_DEBUG
            DPRINT(F("_min_res               = "));DDECLN(_min_res);
            DPRINT(F("_max_res               = "));DDECLN(_max_res);
            DPRINT(F("_res_upper_limit_min   = "));DDECLN(_res_upper_limit_min);
            DPRINT(F("_res_lower_limit_max   = "));DDECLN(_res_lower_limit_max);
#endif
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
      
      normalized_residual=((double)(residual - _min_res)/(double)(_max_res - _min_res));
      
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
      DPRINT(F("raw gas resistance                     = "));DDEC(gas_raw);DPRINTLN(F(" "));
      DPRINT(F("multiple linear regression prediction  = "));DDEC(mlr_pred);DPRINTLN(F(" "));
      DPRINT(F("residual                               = "));DDEC(residual);DPRINTLN(F(" "));
      DPRINT(F("minimum residual                       = "));DDEC(_min_res);DPRINTLN(F(" "));
      DPRINT(F("maximum residual                       = "));DDEC(_max_res);DPRINTLN(F(" "));
      DPRINT(F("normalized residual                    = "));DDEC(normalized_residual);DPRINTLN(F(" "));
      DPRINT(F("air quality level                      = "));DDEC(log_normalized_residual);DPRINTLN(F(" "));
      DPRINT(F("Gas UPPER LIMIT                        = "));DDECLN(_gas_upper_limit);
      DPRINT(F("Gas LOWER LIMIT                        = "));DDECLN(_gas_lower_limit);
      DPRINT(F("air quality level scaled               = "));DDEC(_aqState_scaled);DPRINTLN(F(" "));
      DPRINT(F("_gas_resistance_raw_scaled             = "));DDEC(_gas_resistance_raw_scaled);DPRINTLN(F(" "));
      DPRINT(F("_gas_resistance_min_scaled             = "));DDEC(_gas_resistance_min_scaled);DPRINTLN(F(" ")); 
      DPRINT(F("_gas_resistance_max_scaled             = "));DDEC(_gas_resistance_max_scaled);DPRINTLN(F(" "));
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
