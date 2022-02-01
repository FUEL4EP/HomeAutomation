//---------------------------------------------------------
// Sens_weather_statistics_RAM
// 2021-11-24 FUELEP (Creative Commons
// https://creativecommons.org/licenses/by-nc-sa/4.0/
// You are free to Share & Adapt under the following terms:
// Give Credit, NonCommercial, ShareAlike
// +++
//
// library for calculating moving average, data is stored in RAM, therefore sufficient RAM is required, e.g. ATMega1284P
//
//---------------------------------------------------------
//
// used libraries:
//
// ArduinoSTL.h (https://github.com/FUEL4EP/HomeAutomation/tree/master/AsksinPP_developments/libraries/avr_stl)
//
//---------------------------------------------------------


#ifndef _SENS_WEATHER_STATISTICS_RAM_H_
#define _SENS_WEATHER_STATISTICS_RAM_H_



//#define DEEP_DEBUG // comment out if deep serial monitor debugging is not necessary

// "ArduinoSTL.h": (https://github.com/FUEL4EP/HomeAutomation/tree/master/AsksinPP_developments/libraries/avr_stl)
#include "ArduinoSTL.h"



namespace as {

template<typename T> class cb_params_weather {
public:
    T current_value;
    T tail, head;
    T min, max;
    
};

class cb_params_mav {
public:
    double   current_value;
    double   old_value;
    double   min, max;

};

/*
 * weather_statistics CLASS DEFINITION
 */
template<typename T, typename P, int CIRCULAR_BUFFER_SIZE> class  weather_statistics {
  
  
  private:
    static const size_t BUFSIZE = CIRCULAR_BUFFER_SIZE;
    
    T             circular_buffer_weather_datapoint[BUFSIZE];
    
    uint32_t      count_index;     // number of measured datapoints
    
    uint16_t      circular_index;  // index of tail datapoint
    uint16_t      i;               // general running index
    
    cb_params_weather<T>         weather_params;
    cb_params_mav                mov_av_params;
    
    P             moving_sum;
    

  public:
    weather_statistics() {
      DPRINT(F("initializing circular buffer of size ")); DDECLN(BUFSIZE);
      i                                  = 0;
      count_index                        = 0;
      
      mov_av_params.min                  = 0;
      mov_av_params.max                  = 0;
      weather_params.tail                = 0;
      weather_params.head                = 0;
      
    }

    // clear circular buffer; set circular_index to initial value 0
    void clear_buffer() {
        
        DPRINT(F("clearing circular buffer of size ")); DDECLN(BUFSIZE);
        i                                = 0;
        count_index                      = 0;
        circular_index                   = 0;
        
        mov_av_params.min                = 0;
        mov_av_params.max                = 0;
        weather_params.tail              = 0;
        weather_params.head              = 0;
        
        // clear circular_buffer_weather_datapoint
        for ( i = 0; i < BUFSIZE; i = i + 1 ) {
          circular_buffer_weather_datapoint[i]    = 0;
        }
        moving_sum                       = 0;
    }
    
    uint32_t add_measurement( T new_weather_datapoint ) {
        
      count_index += 1;
        
      weather_params.tail                    = circular_buffer_weather_datapoint[circular_index];                               // save tail value
      weather_params.head                    = new_weather_datapoint;
      weather_params.current_value           = new_weather_datapoint;
      // set head value to count

      circular_buffer_weather_datapoint[circular_index]   = weather_params.head;
      circular_index                         = (circular_index + 1) % BUFSIZE;
      
      //calculate moving average
      mov_av_params.old_value                = mov_av_params.current_value;
      moving_sum                             = moving_sum + (P)weather_params.head - (P)weather_params.tail;     // calculate change of moving_sum by dropped tail value and added head value
      mov_av_params.current_value            = (double)moving_sum / BUFSIZE;                                     // calculate moving_average
      
      // check for new maximum of circular_buffer_weather_datapoint
      if ( mov_av_params.current_value > mov_av_params.max ) {
        mov_av_params.max = mov_av_params.current_value;
      }
      
      // check for new minimum of circular_buffer_weather_datapoint
      if ( mov_av_params.current_value < mov_av_params.min ) {
        mov_av_params.min = mov_av_params.current_value;
      }
      
      determine_min_max(circular_buffer_weather_datapoint, &weather_params );
 
      return count_index;
      
    }
    
    void print_circular_buffer_weather_datapoint ( void ) {
        
        uint16_t i;
        
        for ( i = 0; i < BUFSIZE; i = i + 1 ) {
            DPRINT(F("circular_buffer_weather_datapoint[")); DDEC(i); DPRINT(F("] :")); DDECLN(circular_buffer_weather_datapoint[i]);
        }
    }
    
    T get_weather_params_head ( void ) {
       return weather_params.head;
    }
    
    T get_weather_params_tail ( void ) {
       return weather_params.tail;
    }
    
    T get_min_datapoint ( void ) {
       return weather_params.min;
    }
    
    T get_max_datapoint ( void ) {
       return weather_params.max;
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
    

    // template function for determining min and max value of an array
    template <typename TT, typename PP> 
    void determine_min_max( TT array[], PP* params_ptr ) {
      
      // check if a recalculation of max is needed due to the dropping of the tail value of the circular buffer
      if ( params_ptr->tail == params_ptr->max ) {
        // recalculation of min value is needed
        params_ptr->max = *std::max_element( array, array + BUFSIZE );
      }
      
      // check if a recalculation of min is needed due to the dropping of the tail value of the circular buffer
      if ( params_ptr->tail == params_ptr->min ) {
        // recalculation of max value is needed
        params_ptr->min = *std::min_element(array, array + BUFSIZE );
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
