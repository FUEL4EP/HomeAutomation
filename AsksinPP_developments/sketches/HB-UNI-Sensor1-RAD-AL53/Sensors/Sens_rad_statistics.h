//---------------------------------------------------------
// Sens_rad_statistics
// 2021-06-19 FUELEP (Creative Commons
// https://creativecommons.org/licenses/by-nc-sa/4.0/
// You are free to Share & Adapt under the following terms:
// Give Credit, NonCommercial, ShareAlike
// +++
//---------------------------------------------------------
//
// used libraries:
//
// none
//
//---------------------------------------------------------
//
// Required by:
//
//
//---------------------------------------------------------
//
// This libarary provides a resource optimized calculation of the statistical parameters based on a circular buffer.
// In case of a measurement interval of 10 minutes, the circular ring buffer of size 1008 is storing the measurement
// values of the past week.
// The following statistical parameters are calulated:
//
// 1. unint16_t         count, min count, max count
// 2. double            moving average, min moving averag, max moving averag
// 3. double            variance, min variance, max variance
// 4. double            standard deviation, min standard deviation, max standard deviation
// 5. double            width of 95% confidence interval, min width of 95% confidence interval, max width of 95% confidence interval
//
//---------------------------------------------------------


#ifndef _SENS_RAD_STATISTICS_H_
#define _SENS_RAD_STATISTICS_H_


#define CIRCULAR_BUFFER_SIZE                              1008    // 24 * 60 * 7 / 10  => stores samples of 7 days, one sample every 10 minutes
//#define CIRCULAR_BUFFER_SIZE                                20    // for test purposes
#define VARIANCE_RECALC_INDEX                             1000    // variance is recalculated from scratch for every VARIANCE_RECALC_INDEX

#define PERCENT_95_CONFIDENCE_INTERVALL_FACTOR            1.96    // factor to get 95% confidence intervall from standard deviation

//#define DEEP_DEBUG // comment out if deep serial monitor debugging is not necessary
#include "ArduinoSTL.h"


namespace as {

class cb_params_cnt {
public:
    uint16_t current_value;
    uint16_t tail, head;
    double   min, max;
    
};

class cb_params_var {
public:
    double   current_value;
    double   tail, head;
    double   min, max;

};

class cb_params_mav {
public:
    double   current_value;
    double   old_value;
    double   min, max;

};

class mima_params {
public:
    double   value;
    double   min, max;
};


/*
 * rad_statistics CLASS DEFINITION
 */
class  rad_statistics {
  
  
  private:
    uint16_t      circular_count_buffer[CIRCULAR_BUFFER_SIZE];
    double        circular_variance_buffer[CIRCULAR_BUFFER_SIZE];
    
    uint32_t      count_index;
    
    uint16_t      circular_index;
    uint16_t      i;
    
    cb_params_cnt cnt_params;
    
    cb_params_mav mov_av_params;
    uint32_t      moving_sum;
    
    cb_params_var var_params;
  
    mima_params   std_dev_params;
    
    mima_params   confid_interval_params;
    

  public:
    rad_statistics() {
      i           = 0;
      count_index = 0;
    }

    // clear circular buffer; set circular_index to initial value 0
    void clear_buffer() {
        
        count_index = 0;
        
        // clear circular_count_buffer
        for ( i = 0; i < CIRCULAR_BUFFER_SIZE; i = i + 1 ) {
          circular_count_buffer[i]    = 0;
        }
        
        // clear circular_variance_buffer
        for ( i = 0; i < CIRCULAR_BUFFER_SIZE; i = i + 1 ) {
          circular_variance_buffer[i] = 0;
        }
        
        circular_index                   = 0;
        
        cnt_params.current_value         = 0;
        cnt_params.head                  = 0;
        cnt_params.tail                  = 0;
        cnt_params.min                   = 0;
        cnt_params.max                   = 0;
        
        moving_sum                       = 0;
        
        mov_av_params.current_value      = 0.0;
        mov_av_params.old_value          = 0.0;
        mov_av_params.min                = 0.0;
        mov_av_params.max                = 0.0;
        
        var_params.current_value         = 0.0;
        var_params.head                  = 0.0;
        var_params.tail                  = 0.0;
        var_params.min                   = 0.0;
        var_params.max                   = 0.0;
       
        std_dev_params.value             = 0.0;
        std_dev_params.min               = 0.0;
        std_dev_params.max               = 0.0;

    }
    
    uint32_t add_count( uint16_t count ) {
        
      count_index += 1;
        
      cnt_params.tail                         = circular_count_buffer[circular_index];                     // save tail value
      cnt_params.head                         = count;
      cnt_params.current_value                = count;
      // set head value to count

      circular_count_buffer[circular_index]   = cnt_params.head;
      circular_index                          = (circular_index + 1) % CIRCULAR_BUFFER_SIZE;
      
      //calculate moving average
      mov_av_params.old_value                 = mov_av_params.current_value;
      moving_sum                              = moving_sum + (uint32_t)cnt_params.head - (uint32_t)cnt_params.tail;  // calculate change of moving_sum by dropped tail value and added head value
      mov_av_params.current_value             = (double)moving_sum / CIRCULAR_BUFFER_SIZE;                 // calculate moving_average
      
      // check for new maximum of mov_av_params
      if ( mov_av_params.current_value > mov_av_params.max ) {
        mov_av_params.max = mov_av_params.current_value;
      }
      
      //calculate variance of circular buffer according https://jonisalonen.com/2014/efficient-and-accurate-rolling-standard-deviation/
      var_params.current_value = var_params.current_value + (double) ( (int16_t)cnt_params.head - (int16_t)cnt_params.tail ) * ( (double)( cnt_params.head + cnt_params.tail ) - mov_av_params.current_value - mov_av_params.old_value ) / (double)( CIRCULAR_BUFFER_SIZE - 1 );

      
      var_params.tail = circular_variance_buffer[circular_index];
      var_params.head = var_params.current_value;

      circular_variance_buffer[circular_index] = var_params.current_value;
      
      std_dev_params.value = sqrt( var_params.current_value );
      
      if ( count_index == VARIANCE_RECALC_INDEX ) {
          full_variance_calculation();
      }
      
      determine_min_max(circular_count_buffer, &cnt_params );

      determine_min_max(circular_variance_buffer, &var_params );

      std_dev_params.min = sqrt( var_params.min );
      std_dev_params.max = sqrt( var_params.max );
      
      confid_interval_params.value = PERCENT_95_CONFIDENCE_INTERVALL_FACTOR * std_dev_params.value;
      confid_interval_params.min   = PERCENT_95_CONFIDENCE_INTERVALL_FACTOR * std_dev_params.min;
      confid_interval_params.max   = PERCENT_95_CONFIDENCE_INTERVALL_FACTOR * std_dev_params.max;
      
      return count_index;
      
    }
    
    void print_circular_count_buffer ( void ) {
        
        uint16_t i;
        
        for ( i = 0; i < CIRCULAR_BUFFER_SIZE; i = i + 1 ) {
            DPRINT(F("circular_count_buffer[")); DDEC(i); DPRINT(F("] :")); DDECLN(circular_count_buffer[i]);
        }
    }
    
    void print_circular_variance_buffer ( void ) {
        
        uint16_t i;
        
        for ( i = 0; i < CIRCULAR_BUFFER_SIZE; i = i + 1 ) {
            DPRINT(F("circular_variance_buffer[")); DDEC(i); DPRINT(F("] :")); DDECLN(circular_variance_buffer[i]);
        }
    }
    
    // do full calculation of variance from scratch
    void full_variance_calculation ( void ) {
        
        double  full_variance;
        uint16_t i;
        
        full_variance = 0.0;
        for ( i = 0; i < CIRCULAR_BUFFER_SIZE; i = i + 1 ) {
            full_variance = full_variance + ( ( (double)circular_count_buffer[i] - mov_av_params.current_value ) * ( (double)circular_count_buffer[i] - mov_av_params.current_value ) / (double) ( CIRCULAR_BUFFER_SIZE - 1 ) );
        }
        DPRINT(F("offset of variance : ")); DDECLN(full_variance - var_params.current_value);
        
        // correct incrementally calculated variance to full calculated variance
        var_params.current_value = full_variance;
    }
    
    uint16_t get_head_count ( void ) {
       return cnt_params.head;
    }
    
    uint16_t get_tail_count ( void ) {
       return cnt_params.tail;
    }
    
    uint16_t get_min_count ( void ) {
       return cnt_params.min;
    }
    
    uint16_t get_max_count ( void ) {
       return cnt_params.max;
    }
    
    double get_moving_average ( void ) {
       return mov_av_params.current_value;
    }
    
    double get_moving_average_min ( void ) {
       return mov_av_params.min;
    }
    
    double get_moving_average_max ( void ) {
       return mov_av_params.max;
    }
    
    double get_variance ( void ) {
       return var_params.current_value;
    }
    
    double get_variance_min ( void ) {
       return var_params.min;
    }
    
    double get_variance_mx ( void ) {
       return var_params.max;
    }
    
    double get_standard_deviation ( void ) {
       return std_dev_params.value;
    }
    
    double get_standard_deviation_min ( void ) {
       return std_dev_params.min;
    }
    
    double get_standard_deviation_max ( void ) {
       return std_dev_params.max;
    }
    
    double get_width_of_confidence_interval ( void ) {
       return confid_interval_params.value * 2.0;
    }
    
    double get_width_of_confidence_interval_min ( void ) {
       return confid_interval_params.min * 2.0;
    }
    
    double get_width_of_confidence_interval_max ( void ) {
       return confid_interval_params.max * 2.0;
    }

    // template function for determining min and max value of an array
    template <typename T, typename P> 
    void determine_min_max( T array[], P* params_ptr ) {
      
      // check if a recalculation of max is needed due to the dropping of the tail value of the circular buffer
      if ( params_ptr->tail == params_ptr->max ) {
        // recalculation of min value is needed
        params_ptr->max = *std::max_element( array, array + CIRCULAR_BUFFER_SIZE );
      }
      
      // check if a recalculation of min is needed due to the dropping of the tail value of the circular buffer
      if ( params_ptr->tail == params_ptr->min ) {
        // recalculation of max value is needed
        params_ptr->min = *std::min_element(array, array + CIRCULAR_BUFFER_SIZE );
      }
      
      if ( params_ptr->current_value > params_ptr->max ) {
         params_ptr->max = params_ptr->current_value;

      } else {
        if ( params_ptr->current_value < params_ptr->min ) {
            params_ptr->min = params_ptr->current_value;
        }
      }
    }

};
  
}

#endif
