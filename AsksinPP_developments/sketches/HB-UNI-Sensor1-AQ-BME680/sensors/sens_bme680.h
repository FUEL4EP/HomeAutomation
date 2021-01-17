

#ifndef _SENS_BME680_H_
#define _SENS_BME680_H_

#define DEEP_DEBUG               // comment out if deep serial monitor debugging is not necessary

#include <Wire.h>
#include <Sensors.h>
#include <ClosedCube_BME680.h>   // https://github.com/FUEL4EP/ClosedCube_BME680_Arduino/tree/implement_Bosch_datasheet_integer_formulas
#include <math.h>
#include <EEPROM.h>              // for periodically saving important parameters into the Arduino's EEPROM once a day
#include <ErriezCRC32.h>         // https://github.com/Erriez/ErriezCRC32 library for CRC32 checking of EEPROM


/*
 * Gas is returned as a resistance value in ohms.
 * Higher concentrations of VOC will make the resistance lower.
 */

#define AVG_COUNT                   5
#define IIR_FILTER_COEFFICIENT      0.0001359 // 1.0 -0.9998641 ; Decay to 0.71 in about one week for a 4 min sampling period (in 2520 sampling periods)
#define EPSILON                     0.0001
#define MAX_BATTERY_VOLTAGE         3300      // change to 6000 for debugging with FTDI Debugger, default: 3300
#define EEPROM_START_ADDRESS        512       // needs to be above reserved AsksinPP EEPROM area: Address Space: 32 - 110
#define DEVICE_TYPE                 "HB-UNI-Sensor1-AQ-BME680"
#define FINISH_STRING               "/HB-UNI-Sensor1-AQ-BME680"
#define STORE_TO_EEPROM_NO_CYCLES   360       // store parameters once a day; 360 * 4 min
                                              // change to 10 for debugging with FTDI Debugger, default: 360

namespace as {
    

struct AQ_eeprom_data { 
    char        device_type_string[sizeof(DEVICE_TYPE)];
    uint16_t    data_length;
    uint16_t    index;
    uint16_t    operatingVoltage1000;
    uint16_t    first_free_user_eeprom_address;
    uint8_t     max_decay_factor_upper_limit;
    uint8_t     max_increase_factor_lower_limit;
    int32_t     max_gas_resistance;               // maximum measured gas resistance
    int32_t     min_gas_resistance;               // minimum measured gas resistance
    int32_t     gas_upper_limit;                  // adaptive upper resistance level for good air quality
    int32_t     gas_lower_limit;                  // adaptive lower resistance level for bad air quality
    int32_t     gas_upper_limit_min;
    int32_t     gas_lower_limit_max;
    int32_t     min_res;                          // adaptive min residual of multiple linear regression used for normalization
    int32_t     max_res;                          // adaptive max residual of multiple linear regression used for normalization
    int32_t     res_upper_limit;                  // adaptive upper residual level for good air quality
    int32_t     res_lower_limit;                  // adaptive lower residual level for bad air quality
    int32_t     res_upper_limit_min;
    int32_t     res_lower_limit_max;
    double      mlr_alpha;                        // multiple linear regression coefficient alpha (temperature)
    double      mlr_beta;                         // multiple linear regression coefficient beta (absolute humidity)
    double      mlr_delta;                        // multiple linear regression coefficient delta (offset)
    char        finish_string[sizeof(FINISH_STRING)];
    uint32_t    crc32;                            // crc32 of struct AQ_eeprom_data except the crc32 itself
}; 

  

template <uint8_t ADDRESS=0x76>  // I2C address needs to be be set according to your BME680 sensor breakout in the main sketch HB-UNI-Sen-AQ.ino: 'Sens_Bme680<0x76>   bme680;' not here!
class Sens_Bme680 : public Sensor {
private:
  int16_t    _temperature;
  uint16_t   _pressureNN;
  uint16_t   _humidity;
  uint16_t   _aqLevel;
  uint16_t   _aqState_scaled;
  int16_t    _gas_resistance_raw_scaled;
  int16_t    _gas_resistance_min_scaled;
  int16_t    _gas_resistance_max_scaled;
  uint16_t   _height;
  uint16_t   measurement_index;
  uint16_t   _first_free_user_eeprom_address;   // EEPROM starting address for storing essential data
   
  AQ_eeprom_data ee;                            // EEPROM structure for saving essential parameters
  AQ_eeprom_data ee_get;                        // EEPROM structure for reading essential parameters
  
  uint32_t   crc32_checksum_recreated;

  ClosedCube_BME680 _bme680;
  
public:

  Sens_Bme680 (): _temperature(0),  _pressureNN(0), _humidity(0), _aqLevel(0), _aqState_scaled(0), _gas_resistance_raw_scaled(0), _gas_resistance_min_scaled(0), _gas_resistance_max_scaled(0) {}
    ~Sens_Bme680 () {}
    
  void print_EEPROM_struct(AQ_eeprom_data ee) {
      
    DPRINT(F("ee.first_free_user_eeprom_address      = "));DDECLN(ee.first_free_user_eeprom_address);
    if (ee.index > 0) {
      DPRINT(F("ee.index                               = "));DDECLN(ee.index);
    }
    else {
      DPRINTLN(F("ee.index                               = NOT YET SET"));
    }
    DPRINT(F("ee.data_length                         = "));DDECLN(ee.data_length);
    DPRINT(F("ee.operatingVoltage1000                = "));DDECLN(ee.operatingVoltage1000);
    DPRINT(F("ee.device_type_string                  = "));DPRINTLN(ee.device_type_string);
    DPRINT(F("ee.max_decay_factor_upper_limit        = "));DDECLN(ee.max_decay_factor_upper_limit);
    DPRINT(F("ee.max_increase_factor_lower_limit     = "));DDECLN(ee.max_increase_factor_lower_limit);
    DPRINT(F("ee.max_decay_factor_upper_limit        = "));DDECLN(ee.max_decay_factor_upper_limit);
    DPRINT(F("ee.max_increase_factor_lower_limit     = "));DDECLN(ee.max_increase_factor_lower_limit);
    DPRINT(F("ee.max_gas_resistance                  = "));DDECLN(ee.max_gas_resistance);
    DPRINT(F("ee.min_gas_resistance                  = "));DDECLN(ee.min_gas_resistance);
    DPRINT(F("ee.gas_upper_limit                     = "));DDECLN(ee.gas_upper_limit);
    DPRINT(F("ee.gas_lower_limit                     = "));DDECLN(ee.gas_lower_limit);
    DPRINT(F("ee.gas_upper_limit_min                 = "));DDECLN(ee.gas_upper_limit_min);
    DPRINT(F("ee.gas_lower_limit_max                 = "));DDECLN(ee.gas_lower_limit_max);
    DPRINT(F("ee.min_res                             = "));DDECLN(ee.min_res);
    DPRINT(F("ee.max_res                             = "));DDECLN(ee.max_res);
    DPRINT(F("ee.res_upper_limit                     = "));DDECLN(ee.res_upper_limit);
    DPRINT(F("ee.res_lower_limit                     = "));DDECLN(ee.res_lower_limit);
    DPRINT(F("ee.res_upper_limit_min                 = "));DDECLN(ee.res_upper_limit_min);
    DPRINT(F("ee.res_lower_limit_max                 = "));DDECLN(ee.res_lower_limit_max);
    DPRINT(F("ee.mlr_alpha                           = "));DDECLN(ee.mlr_alpha);
    DPRINT(F("ee.mlr_beta                            = "));DDECLN(ee.mlr_beta);
    DPRINT(F("ee.mlr_delta                           = "));DDECLN(ee.mlr_delta);
    DPRINT(F("ee.crc32                               = "));DHEXLN(ee.crc32);
    DPRINT(F("ee.finish_string                       = "));DPRINTLN(ee.finish_string);
      
  }

  void init (uint16_t height, uint8_t max_decay_factor_upper_limit, uint8_t max_increase_factor_lower_limit, double mlr_alpha, double mlr_beta, double mlr_delta, uint16_t first_free_user_eeprom_address) {

    Wire.begin();
    DPRINT(F("BME680 "));
    _bme680.init(ADDRESS); // I2C address: 0x76 or 0x77
    if (_bme680.reset() != 0) {
      DPRINTLN(F("ERR"));
      while (1);
    } else DPRINT(F("OK"));
    _present = true;

    DPRINT(", Chip ID=0x"); DHEXLN(_bme680.getChipID());

      // BME680 oversampling: humidity = x2, temperature = x2, pressure = x4
    _bme680.setOversampling(BME680_OVERSAMPLING_X2, BME680_OVERSAMPLING_X2, BME680_OVERSAMPLING_X4);
    _bme680.setIIRFilter(BME680_FILTER_3); // supresses spikes 
    _bme680.setGasOn(310, 300); // 310 degree Celsius and 300 milliseconds; please check in debug mode whether '-> Gas heat_stab_r   = 1' is achieved. If '-> Gas heat_stab_r   = 0' then the heating time is to short or the temp target too high
    _bme680.setForcedMode();
    
    ee.max_gas_resistance              = -2000000;     // initial value
    ee.min_gas_resistance              =  2000000;     // initial value
    ee.max_res                         = ee.max_gas_resistance;   // initial value
    ee.min_res                         = ee.min_gas_resistance;   // initial value
    _height                            = height;
    ee.gas_lower_limit_max             = ee.min_gas_resistance;
    ee.gas_upper_limit_min             = ee.max_gas_resistance;
    ee.res_lower_limit_max             = ee.min_gas_resistance;
    ee.res_upper_limit_min             = ee.max_gas_resistance;
    ee.gas_lower_limit                 = ee.gas_lower_limit_max;
    ee.gas_upper_limit                 = ee.gas_upper_limit_min;
    ee.res_lower_limit                 = ee.res_lower_limit_max;
    ee.res_upper_limit                 = ee.res_upper_limit_min;
    ee.max_decay_factor_upper_limit    = max_decay_factor_upper_limit;
    ee.max_increase_factor_lower_limit = max_increase_factor_lower_limit;
    ee.mlr_alpha                       = mlr_alpha;
    ee.mlr_beta                        = mlr_beta;
    ee.mlr_delta                       = mlr_delta;
    measurement_index                  = 0;
    
    _first_free_user_eeprom_address = max(first_free_user_eeprom_address,(uint16_t)EEPROM_START_ADDRESS); // needs to be equal of above first_free_user_eeprom_address
    
#ifdef DEEP_DEBUG
    strcpy(ee.device_type_string, DEVICE_TYPE);
    strcpy(ee.finish_string, FINISH_STRING);
    ee.index                          = measurement_index;
    ee.operatingVoltage1000           = 0; // init value only !
    ee.first_free_user_eeprom_address = _first_free_user_eeprom_address;
    
    DPRINTLN(F("\n\n====== Parameters after init start ======"));
    
    print_EEPROM_struct(ee);
    
    DPRINTLN(F("====== Parameters after init end ========\n\n"));
    
#endif
    
  }
  
  double EquivalentSeaLevelPressure(float altitude, float temp, double pres) {
      double seaPress = NAN;
      if(!isnan(altitude) && !isnan(temp) && !isnan(pres))
          seaPress = (pres / pow(1 - ((0.0065 *(double)altitude) / ((double)temp + (0.0065 *(double)altitude) + 273.15)), 5.257));
      return seaPress;
      
  }
  
  
  // check whether batteries have been replaced, this is detected by checking whether VCC voltage is smaller than MAX_BATTERY_VOLTAGE (#define MAX_BATTERY_VOLTAGE)
  // if yes, restore important device parameters from EEPROM
  // if no, store important device parameters to EEPROM every (STORE_TO_EEPROM_NO_CYCLES)th measurement cycle, typically once a day (#define STORE_TO_EEPROM_NO_CYCLES 360)
  bool check_for_battery_change(uint16_t index, uint16_t operatingVoltage1000, uint16 first_free_user_eeprom_address) {
    
    bool battery_change_flag = false;
    
    //check for previous reset ( index is then 0)
    if ( index == 0 ) {
      // check if device is battery supplied, i.e. not supplied by ISP Programmer or FTDI Debugger
        
      if ( operatingVoltage1000 <= MAX_BATTERY_VOLTAGE ) {
        //battery has been changed; no ISP programmer or FTDI debugger are suppying the ATmega1284P Pro Mini XL
        battery_change_flag = true;
        
        DPRINTLN(F("\n\n====== Reading now data back from EEPROM =====\n\n"));
        
        //restore device parameters from EEPROM
        
        // first step: read back EEPROM data into struct 'ee_get', i.e. an intermediate storage structure
        
        EEPROM.get(_first_free_user_eeprom_address, ee_get);
        
        // check for correct crc32
        
        crc32_checksum_recreated = crc32Buffer(&ee_get, sizeof(ee_get)-sizeof(ee_get.crc32));   // exclude scrc32 from creating the crc32 checksum
        DPRINT(F("Recalculated CRC32 fron read data  = "));DHEXLN(crc32_checksum_recreated);
        
        if (ee_get.crc32 == crc32_checksum_recreated) {
	            
            // stored and recreated crc32 checksum are matching
	        
            // check for correct start string
	        
            if (strncmp(ee_get.device_type_string, DEVICE_TYPE, sizeof(DEVICE_TYPE)) == 0) {
                DPRINTLN(F("Identification start string is matching"));
	          
                // check for correct finish string
	          
                if (strncmp(ee_get.finish_string, FINISH_STRING,sizeof(FINISH_STRING)) == 0) {
                    DPRINTLN(F("Identification finish string is matching"));
	            
                    // restore actual parameters from read EEPROM data
#ifdef DEEP_DEBUG
                    DPRINTLN(F("\n\n====== Data read back from EEPROM start ======"));
	            
                    print_EEPROM_struct(ee_get);
	            
                    DPRINTLN(F("====== Data read back from EEPROM end ========\n\n"));
	            
                    DPRINTLN(F("Read back of EEPROM data after Reset has been successfully finished!\n\n"));
	            
#endif 
                    // restore parameters from EEPROM 'ee_get' data
	            
                    //memcpy( &ee, &ee_get, sizeof(ee_get) );
                    ee = ee_get;
	            
#ifdef DEEP_DEBUG
                    DPRINTLN(F("\n\n====== Data restored from EEPROM start ======"));
	            
                    print_EEPROM_struct(ee);
	            
                    DPRINTLN(F("====== Data restored from EEPROM end ========\n\n"));
#endif
                    DPRINTLN(F("Restore of EEPROM data after Reset has been successfully finished!\n\n"));
                }
                else
                {
                    DPRINTLN(F("Finish string does not match!"));
                    DPRINTLN(F("Restore of EEPROM data is aborted!"));
                }
            }  
            else
            {
                DPRINTLN(F("Start string does not match!"));
                DPRINTLN(F("Restore of EEPROM data is aborted!"));
            }
        }
        else {
            // stored and recreated crc32 checksum are NOT matching!
                
            DPRINTLN(F("CRC32 checksum does not match!"));
            DPRINTLN(F("Restore of EEPROM data is aborted!"));
        }
      }
      else  {
        // device is powered by ISP Programmer or FTDI Debugger (VCC > MAX_BATTERY_VOLTAGE)
        DPRINT(F("Device is not battery supplied     = "));DDECLN(operatingVoltage1000);
        DPRINTLN(F("No restore of EEPROM data is done!"));
      }
    }
    else
    { 
        // store device parameters in EEPROM every (STORE_TO_EEPROM_NO_CYCLES)th measurement (typically once a day)
        if ((measurement_index % STORE_TO_EEPROM_NO_CYCLES) == 0 ) {
          DPRINTLN(F("\n\nSaving important device parameters that should be persistent into EEPROM"));
          DPRINT(F("\nmeasurement_index when saving        = "));DDECLN(index);
          
          // set some additional parameters
          strcpy(ee.device_type_string, DEVICE_TYPE);
          strcpy(ee.finish_string, FINISH_STRING);
          ee.index                          = index;
          ee.operatingVoltage1000           = operatingVoltage1000;
          ee.first_free_user_eeprom_address = _first_free_user_eeprom_address;
          ee.data_length                    = (uint16_t)sizeof(struct AQ_eeprom_data);
          ee.crc32                          = crc32Buffer(&ee, sizeof(ee)-sizeof(ee.crc32));   // exclude scrc32 from creating the crc32 checksum
          
          EEPROM.put(_first_free_user_eeprom_address, ee);   //write whole structure 'ee' to EEPROM
          
          DPRINT(F("Number of Bytes written to EEPROM    = "));DDECLN((uint16_t)(ee.data_length));
          
          
#ifdef DEEP_DEBUG
          DPRINTLN(F("\n\n====== Data stored to EEPROM start ======"));
          
          print_EEPROM_struct(ee);
         
          DPRINTLN(F("====== Data stored to EEPROM end ========\n\n"));
#endif
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
      
      ee.max_decay_factor_upper_limit    = max_decay_factor_upper_limit;
      ee.max_increase_factor_lower_limit = max_increase_factor_lower_limit;
      ee.mlr_alpha = mlr_alpha;
      ee.mlr_beta  = mlr_beta;
      ee.mlr_delta = mlr_delta;
    
        
      batteries_were_changed = check_for_battery_change(measurement_index, operatingVoltage1000,_first_free_user_eeprom_address);
      if (batteries_were_changed )  {
        DPRINT(F("Change of batteries was detected       = yes"));
      }
      else {
        DPRINT(F("Change of batteries was detected       = no"));
      }

      measurement_index = measurement_index + 1; // increase measurement index by 1
#ifdef DEEP_DEBUG
      DPRINTLN(F("\n\n\n\n==================================================="));
      DPRINT(F("measurement_index                      = "));DDECLN(measurement_index);
      DPRINTLN(F("==================================================="));
#endif
    
      // Ensure that ee.max_decay_factor_upper_limit is bigger than ee.max_increase_factor_lower_limit
      if (ee.max_decay_factor_upper_limit <= ee.max_increase_factor_lower_limit) {
          ee.max_decay_factor_upper_limit = ee.max_increase_factor_lower_limit + 1;
      }
      
#ifdef DEEP_DEBUG

      DPRINTLN(F("\n\n====== Actual parameters start ======"));
      
      strcpy(ee.device_type_string, DEVICE_TYPE);
      strcpy(ee.finish_string, FINISH_STRING);
      ee.index                          = measurement_index;
      ee.data_length                    = (uint16_t)sizeof(struct AQ_eeprom_data);
      ee.operatingVoltage1000           = operatingVoltage1000;
      ee.first_free_user_eeprom_address = _first_free_user_eeprom_address;
      ee.crc32                          = 0; // not relevant here

      print_EEPROM_struct(ee);
      
      DPRINTLN(F("====== Actual parameters end ========\n\n"));
     
#endif
  

      ClosedCube_BME680_Status status = _bme680.readStatus();
      while (! (status.newDataFlag == 1)) {
        _bme680.setForcedMode();
        DPRINT(F("."));
        _delay_ms(200);
        status = _bme680.readStatus();
      }
      DPRINT(F("\n"));

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
          DPRINT(F("."));
          _delay_ms(100);
          status = _bme680.readStatus();
        }
        DPRINT(F("\n"));
        // need to check whether first reading belongs to previous quintuple reading (last reading). Measurement results are indicating this. 
        ClosedCube_BME680_gas_r_lsb gas_status = _bme680.read_gas_r_lsb();
        uint32_t _g = _bme680.readGasResistance();
        
#ifdef DEEP_DEBUG
        DPRINT(F("Gas heat_stab_r     = "));DDECLN(gas_status.heat_stab_r);
        DPRINT(F("Gas gas_valid_r     = "));DDECLN(gas_status.gas_valid_r);

        DPRINT(F("gas sampling index  = "));DDECLN(c);
        DPRINT(F("gas resistance      = "));DDECLN(_g);
#endif
        
        gas  += _g;
        status = _bme680.readStatus();
      }
      
      gas      /= AVG_COUNT;
      gas_raw   = (double)gas;  // without any limiting
      DPRINT("avg gas: ");DDECLN(gas);
      DPRINT(F("avg. gas resistance = "));DDECLN(gas);DPRINT(F("\n\n"));
      
      // peak detectors for min/max ever measured gas resistances since last reset
      
      if ( gas > ee.max_gas_resistance) {   // capture maximum of ever measured gas resistances since last reset
        ee.max_gas_resistance = gas;
        if ( ee.max_gas_resistance >= ee.min_gas_resistance ) {
            
            // set lower limit for decay of ee.gas_upper_limit; ee.max_decay_factor_upper_limit is typically set to 70 as WebUI device parameter
            ee.gas_upper_limit_min = ee.min_gas_resistance + (ee.max_gas_resistance - ee.min_gas_resistance) * (int32_t)ee.max_decay_factor_upper_limit / 100;
            
            // set upper limit for increase of ee.gas_lower_limit; ee.max_increase_factor_lower_limit is typically set to 30 as WebUI device parameter
            ee.gas_lower_limit_max = ee.min_gas_resistance + (ee.max_gas_resistance - ee.min_gas_resistance) * (int32_t)ee.max_increase_factor_lower_limit / 100;
#ifdef DEEP_DEBUG
            DPRINT(F("ee.min_gas_resistance                  = "));DDECLN(ee.min_gas_resistance);
            DPRINT(F("ee.max_gas_resistance                  = "));DDECLN(ee.max_gas_resistance);
            DPRINT(F("ee.gas_upper_limit_min                 = "));DDECLN(ee.gas_upper_limit_min);
            DPRINT(F("ee.gas_lower_limit_max                 = "));DDECLN(ee.gas_lower_limit_max);
#endif            
        }
      }
      
      if ( gas < ee.min_gas_resistance) {   // capture minimum of ever measured gas resistances since last reset
        ee.min_gas_resistance = gas;
         if ( ee.max_gas_resistance >= ee.min_gas_resistance ) {
             
            // set lower limit for decay of ee.gas_upper_limit; ee.max_decay_factor_upper_limit is typically set to 70 as WebUI device parameter
            ee.gas_upper_limit_min = ee.min_gas_resistance + (ee.max_gas_resistance - ee.min_gas_resistance) * (int32_t)ee.max_decay_factor_upper_limit / 100;
            
            // set upper limit for increase of ee.gas_lower_limit; ee.max_increase_factor_lower_limit is typically set to 30 as WebUI device parameter
            ee.gas_lower_limit_max = ee.min_gas_resistance + (ee.max_gas_resistance - ee.min_gas_resistance) * (int32_t)ee.max_increase_factor_lower_limit / 100;
#ifdef DEEP_DEBUG
            DPRINT(F("ee.min_gas_resistance                  = "));DDECLN(ee.min_gas_resistance);
            DPRINT(F("ee.max_gas_resistance                  = "));DDECLN(ee.max_gas_resistance);
            DPRINT(F("ee.gas_upper_limit_min                 = "));DDECLN(ee.gas_upper_limit_min);
            DPRINT(F("ee.gas_lower_limit_max                 = "));DDECLN(ee.gas_lower_limit_max);
#endif 
        }
      }
      
      //peak detector for ee.gas_upper_limit    (CCU Historian datapoint parameter AQ_GAS_RESISTANCE_MAX)
      
      if ( gas > ee.gas_upper_limit )
      {
        ee.gas_upper_limit = gas;
      }
      else
      {
        ee.gas_upper_limit = ee.gas_upper_limit - (ee.gas_upper_limit - ee.gas_lower_limit) * IIR_FILTER_COEFFICIENT; // decay each sample by IIR_FILTER_COEFFICIENT * (max-min)
        if ( ee.gas_upper_limit < ee.gas_upper_limit_min ) {
          // limit decay of ee.gas_upper_limit to ee.gas_upper_limit_min
          ee.gas_upper_limit = ee.gas_upper_limit_min; // lower limit for ee.gas_upper_limit
        }
      }
      
      //peak detector for ee.gas_lower_limit  (CCU Historian datapoint parameter AQ_GAS_RESISTANCE_MIN)
      
      if ( gas < ee.gas_lower_limit )
      {
        ee.gas_lower_limit = gas;
      }
      else
      {
        ee.gas_lower_limit = ee.gas_lower_limit + (ee.gas_upper_limit - ee.gas_lower_limit) * IIR_FILTER_COEFFICIENT; // increase each sample by IIR_FILTER_COEFFICIENT * (max-min)
        if ( ee.gas_lower_limit > ee.gas_lower_limit_max ) {
          // limit increase of ee.gas_lower_limit to ee.gas_lower_limit_max
          ee.gas_lower_limit = ee.gas_lower_limit_max; // upper limit for ee.gas_lower_limit
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

      DPRINT(F("Gas UPPER LIMIT                        = "));DDECLN(ee.gas_upper_limit);
      DPRINT(F("Gas LOWER LIMIT                        = "));DDECLN(ee.gas_lower_limit);

      DPRINT(F("T                                      = "));DDECLN(_temperature);
#ifdef DEEP_DEBUG
      DPRINT(F("P                                      = "));DDECLN(pres); 
#endif
      DPRINT(F("PNN                                    = "));DDECLN(_pressureNN);
      DPRINT(F("Hum                                    = "));DDECLN(_humidity);
      DPRINT(F("Gas                                    = "));DDECLN(gas);

     
      float _gas_score = 0.0;

      // calculate the normalized air quality level = AQ_LEVEL datapoint parameter in CCU Historian
      
      _gas_score = (float)(gas - ee.gas_lower_limit)/(float)(ee.gas_upper_limit - ee.gas_lower_limit) * 100.0;
      
      if ( _gas_score > 100.0 )
        _gas_score = 100.0;

      _aqLevel = (uint8_t)(_gas_score);
      //DPRINT(F("AQ% = "));DDECLN(_aqLevel);
      DPRINT(F("AQ Level                               = "));DDEC((uint8_t)_gas_score);DPRINTLN(F("%"));
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
      DPRINT(F("rLF                                    = "));  DDEC(hum);  DPRINTLN(F(" %"));
      DPRINT(F("Temp                                   = ")); DDEC(temp); DPRINTLN(F(" deg C"));
      DPRINT(F("aLF                                    = "));  DDEC(ah);   DPRINTLN(F(" g/m^3"));
#endif
      
      //calculate the multi linear regression prediction in double precision
      mlr_pred = ee.mlr_delta + ee.mlr_alpha * temp + ee.mlr_beta * ah;
      
      //calculate residual and convert to int32_t
      residual = (int32_t)(gas_raw - mlr_pred);
      
#ifdef DEEP_DEBUG
      DPRINT(F("previous minimum residual              = "));DDEC(ee.min_res);DPRINTLN(F(" "));
      DPRINT(F("previous maximum residual              = "));DDEC(ee.max_res);DPRINTLN(F(" "));
#endif
      
      
      // peak detectors for min/max ever calculated residual since last reset
      
      
      if ( residual > ee.max_res) {   // capture maximum of ever calculated residual gas resistances since last reset
        ee.max_res = residual;
         if ( ee.max_res > ee.min_res ) {
             
             // set lower limit for decay of ee.res_upper_limit_min; ee.max_decay_factor_upper_limit is typically set to 70 as WebUI device parameter
            ee.res_upper_limit_min = ee.min_res + (ee.max_res - ee.min_res) * (int32_t)ee.max_decay_factor_upper_limit / 100;
            
            // set upper limit for increase of ee.res_lower_limit_max; ee.max_increase_factor_lower_limit is typically set to 30 as WebUI device parameter
            ee.res_lower_limit_max = ee.min_res + (ee.max_res - ee.min_res) * (int32_t)ee.max_increase_factor_lower_limit / 100;
#ifdef DEEP_DEBUG
            DPRINT(F("ee.min_res                             = "));DDECLN(ee.min_res);
            DPRINT(F("ee.max_res                             = "));DDECLN(ee.max_res);
            DPRINT(F("ee.res_upper_limit_min                 = "));DDECLN(ee.res_upper_limit_min);
            DPRINT(F("ee.res_lower_limit_max                 = "));DDECLN(ee.res_lower_limit_max);
#endif
        }
      }
      if ( residual < ee.min_res) {   // capture minimum of ever calculated residual gas resistances since last reset
        ee.min_res = residual;
        if ( ee.max_res > ee.min_res ) {
            
            // set lower limit for decay of ee.res_upper_limit_min; ee.max_decay_factor_upper_limit is typically set to 70 as WebUI device parameter
            ee.res_upper_limit_min = ee.min_res + (ee.max_res - ee.min_res) * (int32_t)ee.max_decay_factor_upper_limit / 100;
            
            // set upper limit for increase of ee.res_lower_limit_max; ee.max_increase_factor_lower_limit is typically set to 30 as WebUI device parameter
            ee.res_lower_limit_max = ee.min_res + (ee.max_res - ee.min_res) * (int32_t)ee.max_increase_factor_lower_limit / 100;
#ifdef DEEP_DEBUG
            DPRINT(F("ee.min_res                             = "));DDECLN(ee.min_res);
            DPRINT(F("ee.max_res                             = "));DDECLN(ee.max_res);
            DPRINT(F("ee.res_upper_limit_min                 = "));DDECLN(ee.res_upper_limit_min);
            DPRINT(F("ee.res_lower_limit_max                 = "));DDECLN(ee.res_lower_limit_max);
#endif
        }
      }
      
      //peak detector for ee.res_upper_limit 
      if ( residual > ee.res_upper_limit )
      {
        ee.res_upper_limit = residual;
       
      }
      else
      {
        ee.res_upper_limit = ee.res_upper_limit - (ee.res_upper_limit - ee.res_lower_limit) * IIR_FILTER_COEFFICIENT; // decay each sample by IIR_FILTER_COEFFICIENT * (max-min)
        // limit decay of ee.res_upper_limit to ee.res_upper_limit_min
        if ( ee.res_upper_limit < ee.res_upper_limit_min ) {
          ee.res_upper_limit = ee.res_upper_limit_min; // lower limit for ee.res_upper_limit
        }
      }
      
      //peak detector for ee.res_lower_limit 
      if ( residual < ee.res_lower_limit )
      {
        ee.res_lower_limit = residual;
      }
      else
      {
        ee.res_lower_limit = ee.res_lower_limit + (ee.res_upper_limit - ee.res_lower_limit) * IIR_FILTER_COEFFICIENT; // increase each sample by IIR_FILTER_COEFFICIENT * (max-min)
        // limit increase of ee.res_lower_limit to ee.res_lower_limit_max
        if ( ee.res_lower_limit > ee.res_lower_limit_max ) {
          ee.res_lower_limit = ee.res_lower_limit_max; // upper limit for ee.res_lower_limit
        }
      }
      
      normalized_residual=((double)(residual - ee.min_res)/(double)(ee.max_res - ee.min_res));
      
      // limit minimum of normalized_residual to EPSILON
      if ( normalized_residual < EPSILON)
      { normalized_residual = EPSILON;
      }
      
      normalized_residual = normalized_residual * 100.0;
      
      log_normalized_residual = -1.0 * (log10(normalized_residual) - 2.0);
      
      _aqState_scaled = (uint16_t)(log_normalized_residual*10000.0); //0.0..40000.0
      _gas_resistance_raw_scaled = (uint16_t) (gas_raw / 20);
      _gas_resistance_min_scaled = (uint16_t) (ee.gas_lower_limit / 20);
      _gas_resistance_max_scaled = (uint16_t) (ee.gas_upper_limit / 20);
      
#ifdef DEEP_DEBUG
      DPRINT(F("raw gas resistance                     = "));DDEC(gas_raw);DPRINTLN(F(" "));
      DPRINT(F("multiple linear regression prediction  = "));DDEC(mlr_pred);DPRINTLN(F(" "));
      DPRINT(F("residual                               = "));DDEC(residual);DPRINTLN(F(" "));
      DPRINT(F("minimum residual                       = "));DDEC(ee.min_res);DPRINTLN(F(" "));
      DPRINT(F("maximum residual                       = "));DDEC(ee.max_res);DPRINTLN(F(" "));
      DPRINT(F("normalized residual                    = "));DDEC(normalized_residual);DPRINTLN(F(" "));
      DPRINT(F("air quality level                      = "));DDEC(log_normalized_residual);DPRINTLN(F(" "));
      DPRINT(F("Gas UPPER LIMIT                        = "));DDECLN(ee.gas_upper_limit);
      DPRINT(F("Gas LOWER LIMIT                        = "));DDECLN(ee.gas_lower_limit);
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
