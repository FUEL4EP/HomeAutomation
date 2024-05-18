#include <Adafruit_FRAM_SPI.h>
#include <AskSinPP.h>
#include "Fujitsu_MB85RS2MTPF_FRAM_circular_buffer.h"
#include "Sens_weather_statistics_FRAM.h"


// simple tests of class Sens_weather_statistics_FRAM

// all library classes are placed in the namespace 'as'
using namespace as;

FUJITSU_MB85RS2MTPF_FRAMs frams;

uint8_t cold_start_flag;
bool success;
int16_t  new_temp;
uint32_t index;
double ma, head, tail;
uint32_t moving_sum;
int16_t preset_temp;

weather_statistics<int16_t, int32_t>  statistics_1(&frams, 0x00001, 1, 129, 'Y');
weather_statistics<int16_t, int32_t>  statistics_2(&frams, 0x00312, 1, 39,  'M');



void setup() {
  DINIT(38400, ASKSIN_PLUS_PLUS_IDENTIFIER);
  
  SPI.begin();
  
  DPRINTLN(F("===============> starting test now .. "));

  DPRINTLN(F("===============> warm boot now .. "));
  success=frams.warm_boot();

  cold_start_flag = frams.get_FRAM_start_status_flag();

  preset_temp = 158; //x10 : 15.8 deg C
  success = statistics_1.initialize_circular_buffer(preset_temp);

  statistics_1.dump_flags();

  statistics_1.dump_nvm_weather_statistics_parameters_from_outside();

  statistics_1.dump_flags();

  success=frams.cold_boot();                                   // sets set_FRAM_start_status_flag to FRAMS_UNINITIALIZED !!

  statistics_1.dump_nvm_weather_statistics_parameters_from_outside();

  preset_temp = 0; //x10 : 0 deg C
  success = statistics_1.initialize_circular_buffer(preset_temp);         // sets set_FRAM_start_status_flag to FRAMS_INITIALIZED !!


  statistics_1.dump_nvm_weather_statistics_parameters_from_outside();

  cold_start_flag = frams.get_FRAM_start_status_flag();

  

  success = statistics_1.initialize_circular_buffer(preset_temp);

  success = statistics_1.test_setting_of_circular_buffer();

  statistics_1.dump_nvm_weather_statistics_parameters_from_outside();

  statistics_1.save_nvm_weather_statistics_parameters_to_FRAM();

  statistics_1.dump_flags();

  DPRINTLN(F("===============> test saving and restoring of struct nvm_weather_statistics_parameters .."));

  statistics_1.dump_nvm_weather_statistics_parameters_from_outside();
  statistics_1.save_nvm_weather_statistics_parameters_to_FRAM();
  success = statistics_1.test_setting_of_circular_buffer();

  statistics_1.dump_nvm_weather_statistics_parameters_from_outside();
  statistics_1.restore_nvm_weather_statistics_parameters_from_FRAM();
  statistics_1.dump_nvm_weather_statistics_parameters_from_outside();
  statistics_1.initialize_circular_buffer(preset_temp);
  success = statistics_1.test_setting_of_circular_buffer();
  statistics_1.save_nvm_weather_statistics_parameters_to_FRAM();
  statistics_1.initialize_circular_buffer(preset_temp);
  statistics_1.restore_nvm_weather_statistics_parameters_from_FRAM();
  statistics_1.dump_nvm_weather_statistics_parameters_from_outside();
  statistics_1.dump_circular_buffer_weather_datapoints();
  success = statistics_1.test_setting_of_circular_buffer();
  statistics_1.dump_nvm_weather_statistics_parameters_from_outside();
  statistics_1.dump_circular_buffer_weather_datapoints();

  DPRINTLN(F("===============> cold boot .."));

  success=frams.cold_boot();                                   // sets set_FRAM_start_status_flag to FRAMS_UNINITIALIZED !!

  DPRINTLN(F("===============> initialize_circular_buffer .."));
  DPRINTLN(F("===============> preset circular buffer to 10 deg C.."));
  preset_temp = 100; //x10 : 10.0 deg C
  statistics_1.initialize_circular_buffer(preset_temp);
  statistics_1.dump_nvm_weather_statistics_parameters_from_outside();
  statistics_1.dump_circular_buffer_weather_datapoints();

  DPRINTLN(F("===============> moving average checks: adding data points .."));
  
  new_temp = 20.00;
  index = statistics_1.add_measurement(new_temp);
  statistics_1.dump_nvm_weather_statistics_parameters_from_outside();
  DPRINT(F("index is ")); DDECLN(index);
  
  new_temp = 40.00;
  index = statistics_1.add_measurement(new_temp);
  statistics_1.dump_nvm_weather_statistics_parameters_from_outside();
  DPRINT(F("index is ")); DDECLN(index);
  
  new_temp = 60.00;
  index = statistics_1.add_measurement(new_temp);
  statistics_1.dump_nvm_weather_statistics_parameters_from_outside();
  DPRINT(F("index is ")); DDECLN(index);
  
  new_temp = 80.00;
  index = statistics_1.add_measurement(new_temp);
  statistics_1.dump_nvm_weather_statistics_parameters_from_outside();
  DPRINT(F("index is ")); DDECLN(index);
  
  new_temp = 100.00;
  index = statistics_1.add_measurement(new_temp);
  statistics_1.dump_nvm_weather_statistics_parameters_from_outside();
  DPRINT(F("index is ")); DDECLN(index);

  DPRINTLN(F("===============> dumping data points .."));
  
  statistics_1.dump_circular_buffer_weather_datapoints();
  ma = statistics_1.get_moving_average();
  DPRINT(F("moving average is ")); DDECLN(ma);
  head = statistics_1.get_weather_params_head();
  DPRINT(F("head is ")); DDECLN(head);
  tail = statistics_1.get_weather_params_tail();
  DPRINT(F("tail is ")); DDECLN(tail);
  moving_sum = statistics_1.get_moving_sum();
  DPRINT(F("moving_sum is ")); DDECLN(moving_sum);

  DPRINTLN(F("===============> initialize statistics_2 ring buffer .."));
  
  preset_temp = 150; //x10 : 15.0 deg C
  statistics_2.initialize_circular_buffer(preset_temp);
  statistics_2.dump_nvm_weather_statistics_parameters_from_outside();
  statistics_2.dump_circular_buffer_weather_datapoints();

  DPRINTLN(F("===============> initialize FRAM_start_status_flag as FRAMS_INITIALIZED .."));
  statistics_2.set_FRAM_start_status_flag_from_outside(FRAMS_INITIALIZED);
  statistics_2.dump_nvm_weather_statistics_parameters_from_outside();

  DPRINTLN(F("===============> initialize statistics_1 ring buffer as warm boot.."));

  statistics_1.initialize_circular_buffer(preset_temp);
  statistics_1.dump_nvm_weather_statistics_parameters_from_outside();
  statistics_1.dump_circular_buffer_weather_datapoints();

  DPRINTLN(F("===============> initialize statistics_2 ring buffer as warm boot.."));

  statistics_2.initialize_circular_buffer(preset_temp);
  statistics_2.dump_nvm_weather_statistics_parameters_from_outside();
  statistics_2.dump_circular_buffer_weather_datapoints();




}
  
  


void loop(void) {

}
