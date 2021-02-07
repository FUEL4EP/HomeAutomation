

#ifndef _SENS_BME680_H_
#define _SENS_BME680_H_

#define DEEP_DEBUG               // comment out if deep serial monitor debugging is not necessary

#define KALMAN_FILTER_SETTLING_INDICATION_PIN PA0  // PA0 / physical pin 37 of ATmega1284P of Tindie Pro Mini XL V2.0 (A0 pin on board); pease adapt if you use a different MCU

#include <limits.h>
#include <Wire.h>
#include <Sensors.h>
#include <ClosedCube_BME680.h>   // https://github.com/FUEL4EP/ClosedCube_BME680_Arduino/tree/implement_Bosch_datasheet_integer_formulas
#include <math.h>
#include <EEPROM.h>              // for periodically saving important parameters into the Arduino's EEPROM once a day
#include <ErriezCRC32.h>         // https://github.com/Erriez/ErriezCRC32 library for CRC32 checking of EEPROM
#include <Streaming.h>           // only needed for formatting stream outputs '<<' below

#include <Kalman.h>              // Kalman Filter Library: https://github.com/rfetick/Kalman or https://www.arduino.cc/reference/en/libraries/kalman-filter-library/ 
// A Kalman filter with a zero covariance matrix for the process noise is well known a the recursive minimum least-square error (LMMSE) filter for a linear system
// with some assumptions on auto- and cross-correlations of process and measurement noise and initial state.

/* extract from https://github.com/rfetick/Kalman/blob/master/Kalman.h
 * 
 * Big thanks to R.JL. Fétick for publishing his easy to use Kalman filter for Arduino at Github !
 * 
 * Implement the Kalman filter corresponding to the linear problem
 *    x_k = F*x_{k-1} + B*u_k + q_k   (evolution model)
 *    y_k = H*x_k + r_k               (measure)
 *
 * with the matrices and vectors
 *    x [output] [size=Nstate]          Estimated state vector
 *    F [input]  [size=(Nstate,Nstate)] Free evolution of the state vector
 *    B [input]  [size=(Nstate,Ncom)]   [optional] Command vector acting on state
 *    Q [input]  [size=(Nstate,Nstate)] Model covariance acting as (1/inertia)
 *    y [input]  [size=Nobs]            Observed (measured) data from sensors
 *    H [input]  [size=(Nobs,Nstate)]   Observation matrix
 *    R [input]  [size=(Nobs,Nobs)]     Measurement noise covariance matrix
 *
 * Many attributes are public, so you might modify them as you wish.
 * However be careful since modification of attributes (especially 'P' and 'x')
 * might lead to unconsistant results.
 * Use the 'getxcopy' method to get a copy of the 'x' state vector.
 *
 * Requires:
 *  BasicLinearAlgebra  https://github.com/tomstewart89/BasicLinearAlgebra
 *
 * License:
 *  See the LICENSE file
 *
 * Author:
 *  R.JL. Fétick
 *
 */

using namespace BLA;

/*
 * Gas is returned as a resistance value in ohms.
 * Higher concentrations of VOC will make the resistance lower.
 */

#define AVG_COUNT                                        5
#define IIR_FILTER_COEFFICIENT_KF_SETTLED                0.0001359 // 1.0 - 0.9998641 ; Decay to 0.71 in about one week for a 4 min sampling period (in 2520 sampling periods); settled status of Kalman filter
#define IIR_FILTER_COEFFICIENT_KF_UNSETTLED              0.0376494 // 1.0 - 0.9623506 ; Decay to 0.1 in about 4 hours for a 4 min sampling period (in 60 sampling periods); unsettled status of Kalman filter
#define EPSILON                                          0.0001
#define MAX_BATTERY_VOLTAGE                              3300      // change to 6000 for debugging with FTDI Debugger, default: 3300
#define EEPROM_START_ADDRESS                             512       // needs to be above reserved AsksinPP EEPROM area: Address Space: 32 - 110
#define DEVICE_TYPE                                      "HB-UNI-Sensor1-AQ-BME680_KF"
#define FINISH_STRING                                    "/HB-UNI-Sensor1-AQ-BME680_KF"
#define STORE_TO_EEPROM_NO_CYCLES                        60        // store parameters once a day; 360 * 4 min
                                                                   // change to 10 for debugging with FTDI Debugger, default: 360
#define START_RESISTANCE                                 4000000   // 4 Meg Ohm
                                              
//------------------------------------
/****  KALMAN MODEL PARAMETERS  ****/
//------------------------------------

#define Nstate                                          4          // VOC_resistance, alpha_temperature, beta_ah, delta_intercept
#define Nobs                                            1          // raw_gas_resistance; note: 'temperature' and 'aH' are NOT part of the observation vector! 

// measurement std

#define n_obs 0.01
#define NO_MEAS_CYCLES_TO_CHECK_KALMAN_FILTER_SETTLING  60         // measurement index for resetting the residual boundaries if Kalman online regression parameters are not yet stable enough (check every 4 hours); default 60
#define AVG_GAS_RESISTANCE                              200000.0   // raw average gas resistance of BME680 sensor; adjust to your sensor if convergence of the Kalman filter takes > 2 days
#define REGRESSION_SETLLED_THRESHOLD                    0.15       // relative changes of Kalman online regression parameters alpha and beta need to be below this threshold for a settled regression
#define REGRESSION_ABSOLUTE_ALPHA_CHANGE                1800       // allow absolute changes up to REGRESSION_ABSOLUTE_ALPHA_CHANGE for a settled 'alpha' temperature coefficient

namespace as {
                                            
    
// All variables of the following struct 'AQ_eeprom_data' will be periodically saved to the ATmega1284P's EEPROM once a day
// After a battery change, these saved parameters will be restored

struct AQ_eeprom_data { 
    char                        device_type_string[sizeof(DEVICE_TYPE)];
    uint16_t                    data_length;
    uint16_t                    index;
    uint16_t                    operatingVoltage1000;
    uint16_t                    first_free_user_eeprom_address;
    uint8_t                     max_decay_factor_upper_limit;
    uint8_t                     max_increase_factor_lower_limit;
    int32_t                     max_gas_resistance;               // maximum measured gas resistance
    int32_t                     min_gas_resistance;               // minimum measured gas resistance
    int32_t                     gas_upper_limit;                  // adaptive upper resistance level for good air quality
    int32_t                     gas_lower_limit;                  // adaptive lower resistance level for bad air quality
    int32_t                     gas_upper_limit_min;
    int32_t                     gas_lower_limit_max;
    int32_t                     min_res;                          // adaptive min residual of multiple linear regression used for normalization
    int32_t                     max_res;                          // adaptive max residual of multiple linear regression used for normalization
    int32_t                     res_upper_limit;                  // adaptive upper residual level for good air quality
    int32_t                     res_lower_limit;                  // adaptive lower residual level for bad air quality
    int32_t                     res_upper_limit_min;
    int32_t                     res_lower_limit_max;
    double                      kalman_alpha;                     // Kalman regression coefficient alpha (temperature)
    double                      kalman_beta;                      // Kalman linear regression coefficient beta (absolute humidity)
    double                      kalman_delta;                     // Kalman linear regression coefficient delta (intercept)
    double                      previous_alpha;                   // regression parameter alpha at RESET_INDEX_KALMAN measurement cycles in the past; used for checking whether regression parameters are stable
    double                      previous_beta;                    // regression parameter beta at RESET_INDEX_KALMAN measurement cycles in the past; used for checking whether regression parameters are stable
    KALMAN<Nstate,Nobs>         K;                                // Kalman filter
    BLA::Matrix<Nobs>           obs;                              // Kalman observation vector
    double                      iir_filter_coefficient;           // IIR filter coefficient for decay/increase of normalization boundaries of gas resistances / residual gas resistances
    double                      non_convergence_factor;           // factor that indicates the non-convergence of the Kalman filter 0.0 .. 100.0 (poor convergence)
    bool                        settled_flag;                     // indicates whether Kalman filter regression coefficients are settled
    char                        finish_string[sizeof(FINISH_STRING)];
    uint32_t                    crc32;                            // crc32 of struct AQ_eeprom_data except the crc32 itself
}; 
  

template <uint8_t ADDRESS=0x76>  // I2C address needs to be be set according to your BME680 sensor breakout in the main sketch HB-UNI-Sen-AQ.ino: 'Sens_Bme680<0x76>   bme680;' not here!
class Sens_Bme680 : public Sensor {
private:
  int16_t    _temperature;
  uint16_t   _pressureNN;
  uint16_t   _humidity;
  uint16_t   _aqLevel;
  uint16_t   _aqState_scaled;
  uint16_t   _gas_resistance_raw_scaled;
  uint16_t   _gas_resistance_min_scaled;
  uint16_t   _gas_resistance_max_scaled;
  int16_t    _aq_compensated_gas_res_raw_scaled;
  int16_t    _aq_compensated_gas_res_min_scaled;
  int16_t    _aq_compensated_gas_res_max_scaled;
  int16_t    _aq_alpha_scaled;
  int16_t    _aq_beta_scaled;
  int16_t    _aq_delta_scaled;
  uint16_t   _height;
  uint16_t   measurement_index;
  uint16_t   _first_free_user_eeprom_address;   // EEPROM starting address for storing essential data
   
  AQ_eeprom_data ee;                            // EEPROM structure for saving essential parameters
  AQ_eeprom_data ee_get;                        // EEPROM structure for reading essential parameters
  
  uint32_t   crc32_checksum_recreated;
  bool       lowbat_save_to_eeprom_flag;        // indcates that parameters need to be saved to eeprom due to lowbat

 
  
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
    DPRINT(F("ee.kalman_alpha                        = "));DDECLN(ee.kalman_alpha);
    DPRINT(F("ee.kalman_beta                         = "));DDECLN(ee.kalman_beta);
    DPRINT(F("ee.kalman_delta                        = "));DDECLN(ee.kalman_delta);
    DPRINT(F("ee.previous_alpha                      = "));DDECLN(ee.previous_alpha);
    DPRINT(F("ee.previous_beta                       = "));DDECLN(ee.previous_beta);
    DPRINT(F("ee.iir_filter_coefficient              = "));DDECLN(ee.iir_filter_coefficient);
    if (ee.settled_flag) {
      DPRINTLN(F("ee.settled_flag                        = true"));
    }
    else {
      DPRINTLN(F("ee.settled_flag                        = false"));
    }
    DPRINT(F("ee.crc32                               = "));DHEXLN(ee.crc32);
    DPRINT(F("ee.finish_string                       = "));DPRINTLN(ee.finish_string);
      
  }

  void init (uint16_t height, uint8_t max_decay_factor_upper_limit, uint8_t max_increase_factor_lower_limit, uint16_t first_free_user_eeprom_address) {

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
    
    ee.max_gas_resistance              = -START_RESISTANCE;    // initial value
    ee.min_gas_resistance              =  START_RESISTANCE;    // initial value
    ee.max_res                         = -START_RESISTANCE;    // initial value
    ee.min_res                         =  START_RESISTANCE;    // initial value
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
    ee.kalman_alpha                    = 0.0;
    ee.kalman_beta                     = 0.0;
    ee.kalman_delta                    = 0.0;
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
    DPRINTLN(F("====== Initialize Kalman Filter ========\n\n"));
    kalman_filter_init();
    
    lowbat_save_to_eeprom_flag = false;
    ee.previous_alpha          = 0.0;                               // initial value
    ee.previous_beta           = 0.0;                               // initial value
    
    ee.iir_filter_coefficient  = IIR_FILTER_COEFFICIENT_KF_SETTLED; // initial value
    ee.settled_flag            = false;                             // initial value
    ee.non_convergence_factor  = 100.0;                             // initial.value
    
    pinMode(KALMAN_FILTER_SETTLING_INDICATION_PIN, OUTPUT);
    
  }
  
  double EquivalentSeaLevelPressure(float altitude, float temp, double pres) {
      double seaPress = NAN;
      if(!isnan(altitude) && !isnan(temp) && !isnan(pres))
          seaPress = (pres / pow(1 - ((0.0065 *(double)altitude) / ((double)temp + (0.0065 *(double)altitude) + 273.15)), 5.257));
      return seaPress;
      
  }
  
  void check_if_Kalman_filter_regression_is_settled () {
  // check id Kalman filter has already settled; check is done every NO_MEAS_CYCLES_TO_CHECK_KALMAN_FILTER_SETTLING th measurement cycle
      
    // ee.settled_flag is true if Kalman filter is settled
    double ratio = 0.0;
    double delta;
    
    // check settling of the Kalman filter's online regression coefficients every NO_MEAS_CYCLES_TO_CHECK_KALMAN_FILTER_SETTLING th measurement cycle
    if (( measurement_index % NO_MEAS_CYCLES_TO_CHECK_KALMAN_FILTER_SETTLING) == 1 ){

#ifdef DEEP_DEBUG
      DPRINTLN(F("\n\n=== Checking convergence of Kalman filter ====\n\n"));                                  DPRINTLN(F(" ")); 
#endif

      ee.settled_flag = true;
      
      // check online regression coefficient alpha
      if (ee.kalman_alpha != 0.0 ) {
        // the temperature regression coefficient alpha can be quite small, therefore we also check for the absolute change 'delta'
        delta = fabs((ee.kalman_alpha - ee.previous_alpha));
        ratio = delta / ee.kalman_alpha;
#ifdef DEEP_DEBUG
        DPRINT(F("ee.kalman_alpha                        = "));DDEC(ee.kalman_alpha);                       DPRINTLN(F(" "));
        DPRINT(F("ee.previous_alpha                      = "));DDEC(ee.previous_alpha);                     DPRINTLN(F(" "));
        DPRINT(F("Convergence ratio of alpha coefficient = "));DDEC(ratio);                                 DPRINTLN(F(" "));
        DPRINT(F("Absolute change of alpha coefficient   = "));DDEC(delta);                                 DPRINTLN(F(" "));
#endif
        // check if relative change of regression coefficient alpha is above REGRESSION_SETLLED_THRESHOLD
        if ( ratio >= REGRESSION_SETLLED_THRESHOLD) {
          if ( delta > REGRESSION_ABSOLUTE_ALPHA_CHANGE ) {
            ee.settled_flag = false;
          }
        }
      }
      else {
        ee.settled_flag = false;
      }
      
      
      ee.non_convergence_factor = ratio;
      
      // check online regression coefficient beta
      if (ee.kalman_beta != 0.0 ) { 
        ratio = fabs((ee.kalman_beta - ee.previous_beta) / ee.kalman_beta) ;
#ifdef DEEP_DEBUG
        DPRINT(F("ee.kalman_beta                         = "));DDEC(ee.kalman_beta);                        DPRINTLN(F(" "));
        DPRINT(F("ee.previous_beta                       = "));DDEC(ee.previous_beta);                      DPRINTLN(F(" "));
        DPRINT(F("Convergence ratio of beta coefficient  = "));DDEC(ratio);                                 DPRINTLN(F(" "));
#endif
        // check if relative change of regression coefficient beta is above REGRESSION_SETLLED_THRESHOLD
        if ( ratio >= REGRESSION_SETLLED_THRESHOLD) {
          ee.settled_flag = false;
        }
      }
      else {
        ee.settled_flag = false;
      }
      
      if ( ratio > ee.non_convergence_factor ) {
        ee.non_convergence_factor = ratio;
      }
      
      ee.non_convergence_factor = constrain( ee.non_convergence_factor * 100.0, 0.0, 100.0 );
      
      // update previous regression coefficients
      ee.previous_alpha = ee.kalman_alpha;
      ee.previous_beta  = ee.kalman_beta;
    
      if ( (! ee.settled_flag)  && (measurement_index != 1) ){  // not for startup: measurement_index == 1
        // Kalman filter online regression did not yet settle
        // reset upper and lower ever measured/calulated gas resistances/residual gas resistances
        // increase decay factor to about 10% in about 4h
        ee.max_res                         = -START_RESISTANCE;                      // initial value
        ee.min_res                         =  START_RESISTANCE;                      // initial value
        ee.max_gas_resistance              = -START_RESISTANCE;                      // initial value
        ee.min_gas_resistance              =  START_RESISTANCE;                      // initial value
        ee.iir_filter_coefficient          =  IIR_FILTER_COEFFICIENT_KF_UNSETTLED;   // increase decay factor to about 10% in about 4h
#ifdef DEEP_DEBUG
        DPRINTLN(F("\n\n\n\n==================================================="));
        DPRINT(F("reset boundaries at measurement index  = "));DDECLN(measurement_index);
        DPRINTLN(F("==================================================="));
#endif
      }
      else {
        // Kalman filter online regression did settle
        // set decay factor to about 71% in about 7 days
        ee.iir_filter_coefficient         =  IIR_FILTER_COEFFICIENT_KF_SETTLED;     // increase decay factor to about 10% in about 4h
      }
      
    }
    
    // indicate the Kalman filter's settling state to pin A0 = pin 37 of ATMega1284P of Tindie Pro Mini XL V2, active high for status 'settled'
    // you may connect a yellow LED with resistor 220 Ohms in series between A0 PCB PIN and PCB GND pin
    digitalWrite(KALMAN_FILTER_SETTLING_INDICATION_PIN, ee.settled_flag);
    
  }
  
  bool check_for_parameter_save_due_to_lowbat( bool lowbat) {
      
    if (lowbat) {
      if (!lowbat_save_to_eeprom_flag) {
        // lowbat has been set newly
        lowbat_save_to_eeprom_flag = true;
      }
      else // lowbat_save_to_eeprom_flag ist set
      {
        // reset lowbat_save_to_eeprom_flag due to not newly set lowbat
        lowbat_save_to_eeprom_flag = false;
      }
    }
    else {
      // lowbat not set
      lowbat_save_to_eeprom_flag = false;
    }
    
    return lowbat_save_to_eeprom_flag;
  }
  
  
  // check whether batteries have been replaced, this is detected by checking whether VCC voltage is smaller than MAX_BATTERY_VOLTAGE (#define MAX_BATTERY_VOLTAGE)
  // if yes, restore important device parameters from EEPROM
  // if no, store important device parameters to EEPROM every (STORE_TO_EEPROM_NO_CYCLES)th measurement cycle, typically once a day (#define STORE_TO_EEPROM_NO_CYCLES 360)
  bool check_for_battery_change(uint16_t index, uint16_t operatingVoltage1000, uint16 first_free_user_eeprom_address, bool batlow) {
    
    bool battery_change_flag                = false;
    
    //check for previous reset ( index is then 0 )
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
        DPRINT(F("Recalculated CRC32 from read data  = "));DHEXLN(crc32_checksum_recreated);
        
        if (ee_get.crc32 == crc32_checksum_recreated) {
	            
            // stored and recreated crc32 checksum are matching
            DPRINTLN(F("CRC32 of stored and recalculated data are matching"));
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
        DPRINT(F("Device is not battery supplied         = "));DDECLN(operatingVoltage1000);
        DPRINTLN(F("No restore of EEPROM data is done!"));
      }
    }
    else
    { 
        // store device parameters in EEPROM every (STORE_TO_EEPROM_NO_CYCLES)th measurement (typically once a day) or if low bat has been detected
        if ( ((measurement_index % STORE_TO_EEPROM_NO_CYCLES) == 0 ) || check_for_parameter_save_due_to_lowbat(batlow) ){
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

#ifdef DEEP_DEBUG          
          unsigned long EEPROMStorageStartTime  = millis();
#endif
          
          EEPROM.put(_first_free_user_eeprom_address, ee);   //write whole structure 'ee' to EEPROM
          
#ifdef DEEP_DEBUG 
          unsigned long EEPROMStorageFinishTime  = millis();
         
          DPRINT(F("Number of Bytes written to EEPROM    = "));DDECLN((uint16_t)(ee.data_length));
          
          unsigned long EEPROMStorageTime = EEPROMStorageFinishTime - EEPROMStorageStartTime;
          Serial << "EEPROM storage time   = " << EEPROMStorageTime << " msec" << '\n';
#endif          
          
#ifdef DEEP_DEBUG
          DPRINTLN(F("\n\n====== Data stored to EEPROM start ======"));
          
          print_EEPROM_struct(ee);
         
          DPRINTLN(F("====== Data stored to EEPROM end ========\n\n"));
#endif
        }
    }
          
    return battery_change_flag;
  }
  
  
  // initializethe Kalman filter
  
  void kalman_filter_init() {
      
   // for details see 'Optimum Linear Estimation' at https://www.sciencedirect.com/topics/social-sciences/kalman-filter
      
   // system evolution matrix (unity diagonal matrix)
  
   ee.K.F = {1.0, 0.0, 0.0, 0.0,
          0.0, 1.0, 0.0, 0.0,
          0.0, 0.0, 1.0, 0.0,
          0.0, 0.0, 0.0, 1.0};
          
   // measurement matrix

   ee.K.H = {1.0, 1.0, 1.0, 1.0};
   
   // system model covariance matrix (zero matrix)
   
   ee.K.Q = {0.0, 0.0, 0.0, 0.0,
          0.0, 0.0, 0.0, 0.0,
          0.0, 0.0, 0.0, 0.0,
          0.0, 0.0, 0.0, 0.0};
          
   // initial setting of posteriori estimation covariance matrix P (unity diagonal matrix)
  
   ee.K.P = {1.0, 0.0, 0.0, 0.0,
          0.0, 1.0, 0.0, 0.0,
          0.0, 0.0, 1.0, 0.0,
          0.0, 0.0, 0.0, 1.0};

   // initialize system state vector

  // get BME680 gas resistance (only rough value is needed) for initialization of the Kalman filter's state vector, this will fasten the convergence of the Kalman filter
   uint32_t _g0 = _bme680.readGasResistance();
   _delay_ms(400);
   uint32_t _g1 = _bme680.readGasResistance();
   _delay_ms(400);
   uint32_t _g2 = _bme680.readGasResistance();
   uint32_t _g_avg0 = (_g0 + _g1 + _g2)/3;
   
   DPRINT(F("start raw gas resistance               = "));  DDEC(_g_avg0);  DPRINTLN(F(" Ohm"));
   
   ee.K.x = {_g_avg0/2.0, 0.0, 0.0, 0.0};
  

   // measurement covariance matrix
    
   ee.K.R = {n_obs*n_obs};  // 0.0001 (very small value is needed for Optimum Linear Estimation)
   
#ifdef DEEP_DEBUG  
   Serial << "init K.R : " << ee.K.R << '\n';
#endif         
 
}

void kalman_filter(double raw_gas_resistance, double temperature, double absolute_humidity) {

#ifdef DEEP_DEBUG    
    unsigned long StartTime = millis();
#endif
      
    // make observation model matrix state dependant
    ee.K.H = {1.0, temperature, absolute_humidity, 1.0};
    
    DPRINT(F("temperature                            = "));  DDEC(temperature);         DPRINTLN(F(" deg C"));
    DPRINT(F("aH                                     = "));  DDEC(absolute_humidity);   DPRINTLN(F(" g/m^3"));
    DPRINT(F("raw gas resistance                     = "));  DDEC(raw_gas_resistance);  DPRINTLN(F(" Ohm"));
    
#ifdef DEEP_DEBUG     
    Serial << "K.H : " << ee.K.H << '\n';
    
    Serial << "K.F : " << ee.K.F << '\n';
#endif

    ee.obs = raw_gas_resistance;
    
    // APPLY KALMAN FILTER
    ee.K.update(ee.obs);


#ifdef DEEP_DEBUG 
    unsigned long CurrentTime = millis();
    unsigned long ElapsedTime = CurrentTime - StartTime;
    Serial << "Executiom time of Kalman filter update  = " << ElapsedTime << " msec" << '\n';
    
    // PRINT RESULTS: measures and estimated state
    Serial << "obs : " << ee.obs << '\n' << "K.x : "  << ee.K.x << '\n';
#endif

}



  void measure (double tempOffset, double pressOffset, double humidOffset, uint8_t max_decay_factor_upper_limit, uint8_t max_increase_factor_lower_limit, uint16_t operatingVoltage1000, bool batlow) {
    if (_present == true) {
      double  temp(NAN), hum(NAN), pres(NAN);                                 // use type double in order to match the return type of closed cubes's library function readPressure
      double  ah,tt,ttt,vp,svp;                                               // variables for calculating the absolute humidity
      double  log_normalized_residual,normalized_residual;                    // double variables for calculating the air quality level
      int32_t residual;                                                       // int32_t variables for calculating the air quality level
      int32_t gas = 0;
      double  gas_raw;
      bool    batteries_were_changed;
      
#ifdef DEEP_DEBUG
      unsigned long MeasureStartTime = millis();
#endif
      
      // check if the Kalman filter online regression is settled, min/max ever boundaries and ee.iir_filter_coefficient are adapted in case of a non yet settled Kalman filter
      check_if_Kalman_filter_regression_is_settled();
#ifdef DEEP_DEBUG
      if (ee.settled_flag) {
        DPRINTLN(F("\nConvergence of Kalman filter           = GOOD\n"));
      }
      else {
        DPRINTLN(F("\nConvergence of Kalman filter           = POOR\n"));
      }
      DPRINT(F("ee.iir_filter_coefficient              = "));DDEC(ee.iir_filter_coefficient);                         DPRINTLN(F(" \n"));
#endif
      
      ee.max_decay_factor_upper_limit    = max_decay_factor_upper_limit;
      ee.max_increase_factor_lower_limit = max_increase_factor_lower_limit;

      batteries_were_changed = check_for_battery_change(measurement_index, operatingVoltage1000,_first_free_user_eeprom_address, batlow);
#ifdef DEEP_DEBUG
      if (batteries_were_changed )  {
        DPRINT(F("Change of batteries was detected       = yes"));
      }
      else {
        DPRINT(F("Change of batteries was detected       = no"));
      }
#endif

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
      
      if ( gas >= ee.max_gas_resistance) {   // capture maximum of ever measured gas resistances since last reset
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
      
      if ( gas <= ee.min_gas_resistance) {   // capture minimum of ever measured gas resistances since last reset
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
      
      if ( gas >= ee.gas_upper_limit )
      {
        ee.gas_upper_limit = gas;
      }
      else
      {
        ee.gas_upper_limit = ee.gas_upper_limit - (ee.gas_upper_limit - ee.gas_lower_limit) * ee.iir_filter_coefficient; // decay each sample by ee.iir_filter_coefficient * (max-min)
        if ( ee.gas_upper_limit < ee.gas_upper_limit_min ) {
          // limit decay of ee.gas_upper_limit to ee.gas_upper_limit_min
          ee.gas_upper_limit = ee.gas_upper_limit_min; // lower limit for ee.gas_upper_limit
        }
      }
      
      //peak detector for ee.gas_lower_limit  (CCU Historian datapoint parameter AQ_GAS_RESISTANCE_MIN)
      
      if ( gas <= ee.gas_lower_limit )
      {
        ee.gas_lower_limit = gas;
      }
      else
      {
        ee.gas_lower_limit = ee.gas_lower_limit + (ee.gas_upper_limit - ee.gas_lower_limit) * ee.iir_filter_coefficient; // increase each sample by ee.iir_filter_coefficient * (max-min)
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
      DPRINT(F("AQ Level                               = "));DDEC((uint8_t)_gas_score);  DPRINTLN(F("%"));
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
      DPRINT(F("rLF                                    = "));  DDEC(hum);   DPRINTLN(F(" %"));
      DPRINT(F("Temp                                   = "));  DDEC(temp);  DPRINTLN(F(" deg C"));
      DPRINT(F("aLF                                    = "));  DDEC(ah);    DPRINTLN(F(" g/m^3"));
      DPRINT(F("gas                                    = "));  DDEC(gas);   DPRINTLN(F(" Ohm"));
#endif
      
      // run kalman filter
      DPRINTLN(F("\n\n====== Kalman Filter Update ===========\n\n"));
      kalman_filter(gas, temp, ah);
      
      // save Kalman regression coefficients to eeprom variables
      ee.kalman_alpha                       = ee.K.x(1);
      ee.kalman_beta                        = ee.K.x(2);
      ee.kalman_delta                       = ee.K.x(0)+ee.K.x(3);
      
      //calculate the compensated gas resistance in double precision, use the regression coefficients calculated by the Kalman filter for compensating the interference of temperature and absolute humidity
      residual = (int32_t)(gas_raw - ee.kalman_alpha*temp - ee.kalman_beta*ah); 
      
      
#ifdef DEEP_DEBUG
      DPRINT(F("residual Kalman Filter                 = "));DDEC(residual);   DPRINTLN(F(" "));
      DPRINT(F("previous minimum residual              = "));DDEC(ee.min_res); DPRINTLN(F(" "));
      DPRINT(F("previous maximum residual              = "));DDEC(ee.max_res); DPRINTLN(F(" "));
#endif
      
      
      // peak detectors for min/max ever calculated residual since last reset
      
      
      if ( residual >= ee.max_res) {   // capture maximum of ever calculated residual gas resistances since last reset
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
      if ( residual <= ee.min_res) {   // capture minimum of ever calculated residual gas resistances since last reset
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
      if ( residual >= ee.res_upper_limit )
      {
        ee.res_upper_limit = residual;
       
      }
      else
      {
        ee.res_upper_limit = ee.res_upper_limit - (ee.res_upper_limit - ee.res_lower_limit) * ee.iir_filter_coefficient; // decay each sample by ee.iir_filter_coefficient * (max-min)
        // limit decay of ee.res_upper_limit to ee.res_upper_limit_min
        if ( ee.res_upper_limit < ee.res_upper_limit_min ) {
          ee.res_upper_limit = ee.res_upper_limit_min; // lower limit for ee.res_upper_limit
        }
      }
      
      //peak detector for ee.res_lower_limit 
      if ( residual <= ee.res_lower_limit )
      {
        ee.res_lower_limit = residual;
      }
      else
      {
        ee.res_lower_limit = ee.res_lower_limit + (ee.res_upper_limit - ee.res_lower_limit) * ee.iir_filter_coefficient; // increase each sample by ee.iir_filter_coefficient * (max-min)
        // limit increase of ee.res_lower_limit to ee.res_lower_limit_max
        if ( ee.res_lower_limit > ee.res_lower_limit_max ) {
          ee.res_lower_limit = ee.res_lower_limit_max; // upper limit for ee.res_lower_limit
        }
      }
      
      normalized_residual=constrain( ((double)(residual - ee.res_lower_limit)/(double)(ee.res_upper_limit - ee.res_lower_limit)), 0.0, 1.0 ); // constraining is necessary to avoid underflow effects later on
      
      
      // limit minimum of normalized_residual to EPSILON
      if ( normalized_residual < EPSILON)
      { normalized_residual = EPSILON;
      }
      
      normalized_residual = normalized_residual * 100.0;
      
      log_normalized_residual = -1.0 * (log10(normalized_residual) - 2.0);
      
      _aqState_scaled                    = (uint16_t)(log_normalized_residual*10000.0); //0.0..40000.0
      _gas_resistance_raw_scaled         = (uint16_t) constrain(( gas_raw / 20.0 ),0,USHRT_MAX);
      _gas_resistance_min_scaled         = (uint16_t) constrain(( ee.gas_lower_limit / 20 ),0,USHRT_MAX);
      _gas_resistance_max_scaled         = (uint16_t) constrain(( ee.gas_upper_limit / 20 ),0,USHRT_MAX);
      
      _aq_compensated_gas_res_raw_scaled = (int16_t) constrain(( residual / 80 ),SHRT_MIN,SHRT_MAX);
      _aq_compensated_gas_res_min_scaled = (int16_t) constrain(( ee.res_lower_limit / 80 ),SHRT_MIN,SHRT_MAX);
      _aq_compensated_gas_res_max_scaled = (int16_t) constrain(( ee.res_upper_limit / 80 ),SHRT_MIN,SHRT_MAX);
      DPRINT(F("ee.kalman_alpha                        = "));DDEC(ee.kalman_alpha);                          DPRINTLN(F(" "));
      _aq_alpha_scaled                   = (int16_t) constrain(( ee.kalman_alpha / 4.0 ),SHRT_MIN,SHRT_MAX);
      DPRINT(F("_aq_alpha_scaled                       = "));DDEC(_aq_alpha_scaled);                         DPRINTLN(F(" "));
      _aq_beta_scaled                    = (int16_t) constrain(( ee.kalman_beta / 16.0 ),SHRT_MIN,SHRT_MAX);
      _aq_delta_scaled                   = (int16_t) constrain(( ee.kalman_delta / 40.0 ),SHRT_MIN,SHRT_MAX);
      
#ifdef DEEP_DEBUG
      DPRINT(F("raw gas resistance                     = "));DDEC(gas_raw);                                  DPRINTLN(F(" "));
      DPRINT(F("residual                               = "));DDEC(residual);                                 DPRINTLN(F(" "));
      DPRINT(F("minimum residual                       = "));DDEC(ee.min_res);                               DPRINTLN(F(" "));
      DPRINT(F("maximum residual                       = "));DDEC(ee.max_res);                               DPRINTLN(F(" "));
      DPRINT(F("normalized residual                    = "));DDEC(normalized_residual);                      DPRINTLN(F(" "));
      DPRINT(F("air quality level                      = "));DDEC(log_normalized_residual);                  DPRINTLN(F(" "));
      DPRINT(F("Gas UPPER LIMIT                        = "));DDEC(ee.gas_upper_limit);                       DPRINTLN(F(" "));
      DPRINT(F("Gas LOWER LIMIT                        = "));DDEC(ee.gas_lower_limit);                       DPRINTLN(F(" "));
      DPRINT(F("air quality level scaled               = "));DDEC(_aqState_scaled);                          DPRINTLN(F(" "));
      DPRINT(F("_gas_resistance_raw_scaled             = "));DDEC(_gas_resistance_raw_scaled);               DPRINTLN(F(" "));
      DPRINT(F("_gas_resistance_min_scaled             = "));DDEC(_gas_resistance_min_scaled);               DPRINTLN(F(" ")); 
      DPRINT(F("_gas_resistance_max_scaled             = "));DDEC(_gas_resistance_max_scaled);               DPRINTLN(F(" "));
      DPRINT(F("_aq_compensated_gas_res_raw_scaled     = "));DDEC(_aq_compensated_gas_res_raw_scaled);       DPRINTLN(F(" "));
      DPRINT(F("_aq_compensated_gas_res_min_scaled     = "));DDEC(_aq_compensated_gas_res_min_scaled);       DPRINTLN(F(" ")); 
      DPRINT(F("_aq_compensated_gas_res_max_scaled     = "));DDEC(_aq_compensated_gas_res_max_scaled);       DPRINTLN(F(" "));
      DPRINT(F("_aq_alpha_scaled                       = "));DDEC(_aq_alpha_scaled);                         DPRINTLN(F(" "));
      DPRINT(F("_aq_beta_scaled                        = "));DDEC(_aq_beta_scaled);                          DPRINTLN(F(" ")); 
      DPRINT(F("_aq_delta_scaled                       = "));DDEC(_aq_delta_scaled);                         DPRINTLN(F(" "));
#endif
      
#ifdef DEEP_DEBUG
      unsigned long MeasureFinishTime  = millis();
      unsigned long MeasureElapsedTime = MeasureFinishTime - MeasureStartTime;
      Serial << "Executiom time of measurement method   = " << MeasureElapsedTime << " msec" << '\n';
#endif

    }
    
    // reset indication of the Kalman filter's settling state to pin A0 = pin 37 of ATMega1284P of Tindie Pro Mini XL V2, active high for status 'settled', reset here to 'LOW'
    // you may connect a yellow LED with resistor 220 Ohms in series between A0 PCB PIN and PCB GND pin
    digitalWrite(KALMAN_FILTER_SETTLING_INDICATION_PIN, LOW);
    
    // indicate the non-settled status to the AQ_LOG10 datapoint and the settling convergence factor to AQ_LEVEL datapoint; during the initial settling these datapoints would anyway be somehow chaotic
    // such the non-setlled state can easily be observed without an additinal LED
    if (ee.settled_flag) {
        _aqState_scaled = 33333;                              // during settling AQ_LOG10 is set to 3.3333
        _aqLevel        = (uint8_t)ee.non_convergence_factor;  // indicates the non convergence ( ee.non_convergence_factor higher than REGRESSION_SETLLED_THRESHOLD * 100.0; > 15 .. 100 poor convergence )
    }

  }
  
  // list of return variables, please notice the limitation of payload of a event message to max. 17 Bytes!
  
  int16_t   temperature ()                  { return _temperature; }
  uint16_t  pressureNN ()                   { return _pressureNN; }
  uint16_t  humidity ()                     { return _humidity; }                     // 0..100%
  uint8_t   aq_level ()                     { return _aqLevel; }                      // 0..100%                         CCU Historian datapoint parameter AQ_LEVEL
  uint16_t  aq_state_scaled ()              { return _aqState_scaled; }               // 0..40000, mul 10000!            CCU Historian datapoint parameter AQ_LOG10
  uint16_t  gas_resistance_raw_scaled ()    { return _gas_resistance_raw_scaled; }    // 0..65365; div 20!               CCU Historian datapoint parameter AQ_GAS_RESISTANCE_RAW
  uint16_t  gas_resistance_min_scaled ()    { return _gas_resistance_min_scaled; }    // 0..65365; div 20!               CCU Historian datapoint parameter AQ_GAS_RESISTANCE_MIN
  uint16_t  gas_resistance_max_scaled ()    { return _gas_resistance_max_scaled; }    // 0..65365; div 20!               CCU Historian datapoint parameter AQ_GAS_RESISTANCE_MAX
};

}

#endif
