/***************************************************************************************************/
/*
  This is an Arduino basic library for ABLIC_S35770 I2C Counter IC
  
  written by : FUEL4EP
  sourse code: https://github.com/FUEL4EP

  This counter uses I2C bus to communicate, specials pins are required to interface
  Board:                                    SDA                    SCL
  ATmega328..... ...........................A4                     A5
 
*/
/***************************************************************************************************/


#include <ABLIC_S35770.h>

// This example sketch is assuming the following hardware connections between an S-35770 IC and an Pro Mini XL - v2 - ATmega 1284p from Tindie
// If your hardware configuration is different, please modify the example sketch appropriately to fit to your hardware setup. 
//
// The Pro Mini XL - v2 - ATmega 1284p from Tindie requires to install https://github.com/MCUdude/MightyCore
// Enter the following URL in Additional Boards Manager URLs:  https://mcudude.github.io/MightyCore/package_MCUdude_MightyCore_index.json
//
//
//
//
//  _______________________________________________________               _________________________________________
//                                                         |              |
//    Pro Mini XL - v2 - ATmega 1284p from Tindie          |              |   ABLIC S-35770
//                                                         |              |
//                                       IDE pin 15 (PD7)  |--------------| 1 (RSTB)
//                                                         |              |
//                                       IDE pin 19 (PC3)  |--------------| 3 (CLKIN)
//                                                         |              |
// ________________________________________________________|              |________________________________________
//                                                                       

ABLIC_S35770 myABLIC_S35770(15, 19); // example hardware connection 1: attach the RSTB  pin of S-35770 IC to IDE pin 15 (PD7) of Pro Mini XL - v2 - ATmega 1284p (pin 7 on Tindie PCB board)
                                     // example hardware connection 2: attach the CLKIN pin of S-35770 IC to IDE pin 19 (PC3) of Pro Mini XL - v2 - ATmega 1284p (pin 9 on Tindie PCB board)
                                     // see https://www.tindie.com/products/prominimicros/pro-mini-xl-v2-atmega-1284p/
int count_index, max_count_index;
int SDA_pin=17;

 
void setup()
{
  Serial.begin(57600);

  Serial.println(F(" "));
  Serial.println(F("Demo for usage of the Arduino library for ABLIC S-35770 counter IC"));
  Serial.println(F("(C) FUEL4EP 2021-03-09"));
  Serial.println(F(" "));
  
  myABLIC_S35770.begin();

  while (myABLIC_S35770.check(0x32) != true)  // S-35770 IC's I2C slave address is hard wired as 0x32
  {
    Serial.println(F("ABLIC S-35770 counter IC is not connected !"));
    delay(5000);
  }
  
  Serial.println(F("ABLIC S-35770 counter IC is accessible"));

  Serial.println(F(" "));
  Serial.println(F("Read free register of ABLIC S-35770 counter IC"));
  Serial.print(F("Read free register....: ")); Serial.print(myABLIC_S35770.read_free_register(),HEX); Serial.println(F(" "));
  Serial.println(F(" "));
  Serial.println(F(" "));

  
  Serial.println(F(" "));
  Serial.println(F("Soft resetting 24 Bit binary counter of ABLIC S-35770 counter IC"));
  myABLIC_S35770.soft_reset_counter();
  Serial.print(F("Read free register....: ")); Serial.print(myABLIC_S35770.read_free_register(),HEX); Serial.println(F(" "));
  Serial.println(F(" "));
  
  Serial.println(F(" "));
  uint8_t bit_sel = 17;
  Serial.print(F("Set bit F")); Serial.print(bit_sel,DEC); Serial.println(F(" of free register of ABLIC S-35770 counter IC"));
  myABLIC_S35770.set_FXX_bit_of_free_register(bit_sel);
  Serial.print(F("Read free register....: ")); Serial.print(myABLIC_S35770.read_free_register(),HEX); Serial.println(F(" "));
  Serial.print(F("Clear bit F")); Serial.print(bit_sel,DEC); Serial.println(F(" of free register of ABLIC S-35770 counter IC"));
  myABLIC_S35770.clear_FXX_bit_of_free_register(bit_sel);
  Serial.print(F("Read free register....: ")); Serial.print(myABLIC_S35770.read_free_register(),HEX); Serial.println(F(" "));
  Serial.println(F(" "));

  Serial.println(F(" "));
  uint32_t F_part = 0x12345;
  Serial.print(F("Read free register....: ")); Serial.print(myABLIC_S35770.read_free_register(),HEX); Serial.println(F(" "));
  Serial.print(F("Set F part ot free register of ABLIC S-35770 counter IC to ")); Serial.println(F_part,HEX);
  myABLIC_S35770.set_F_part_F20_F0_of_free_register(F_part);
  Serial.print(F("Read F part ot free register of ABLIC S-35770 counter IC   ")); Serial.println(F_part,HEX);
  Serial.print(F("Read free register....: ")); Serial.print(myABLIC_S35770.read_free_register(),HEX); Serial.println(F(" "));
  Serial.println(F(" "));
  Serial.println(F(" "));
  
  Serial.println(F(" "));
  F_part = 0x1FFFFF;
  Serial.print(F("Read free register....: ")); Serial.print(myABLIC_S35770.read_free_register(),HEX); Serial.println(F(" "));
  Serial.print(F("Set F part ot free register of ABLIC S-35770 counter IC to ")); Serial.println(F_part,HEX);
  myABLIC_S35770.set_F_part_F20_F0_of_free_register(F_part);
  Serial.print(F("Read F part ot free register of ABLIC S-35770 counter IC   ")); Serial.println(F_part,HEX);
  Serial.print(F("Read free register....: ")); Serial.print(myABLIC_S35770.read_free_register(),HEX); Serial.println(F(" "));
  Serial.println(F(" "));
  Serial.println(F(" "));

  Serial.println(F(" "));
  F_part = 0x0;
  Serial.print(F("Read free register....: ")); Serial.print(myABLIC_S35770.read_free_register(),HEX); Serial.println(F(" "));
  Serial.print(F("Set F part ot free register of ABLIC S-35770 counter IC to ")); Serial.println(F_part,HEX);
  myABLIC_S35770.set_F_part_F20_F0_of_free_register(F_part);
  Serial.print(F("Read F part ot free register of ABLIC S-35770 counter IC   ")); Serial.println(F_part,HEX);
  Serial.print(F("Read free register....: ")); Serial.print(myABLIC_S35770.read_free_register(),HEX); Serial.println(F(" "));
  Serial.println(F(" "));
  Serial.println(F(" "));

  uint32_t free_reg_data;
  Serial.print(F("Count register........: ")); Serial.print(myABLIC_S35770.getCount());               Serial.println(F(" "));
  Serial.print(F("Read free register....: ")); Serial.print(myABLIC_S35770.read_free_register(),HEX); Serial.println(F(" "));
  free_reg_data = 111;
  Serial.print(F("Write free register...: ")); Serial.print(free_reg_data,HEX);                       Serial.println(F(" ")); myABLIC_S35770.write_free_register(free_reg_data);
  Serial.print(F("Read free register....: ")); Serial.print(myABLIC_S35770.read_free_register(),HEX); Serial.println(F(" "));


  Serial.println(F(" "));
  Serial.println(F(" "));
  //set counter input signal to 0
  myABLIC_S35770.counter_signal_off();                  // PC3, IDE pin 19 set to 0
  Serial.println(F("Enable counting of ABLIC S-35770 counter IC")); 
  myABLIC_S35770.enable_counting();                     // PD7, IDE pin 15 set to 1
  int count = 16384;
  Serial.print(F("Creating fixed number of pulses..: ")); Serial.print(count);                      Serial.println(F(" "));
  myABLIC_S35770.create_fixed_number_of_pulse_at_counter_input_pin(count);
  Serial.println(F("Wire.end"));
  Wire.end();
  pinMode(SDA_pin, OUTPUT);      //SDA
  Serial.println(F("SDA High"));
  digitalWrite(SDA_pin, HIGH);    //SDA
  delay(3000);
  pinMode(SDA_pin, OUTPUT);      //SDA
  Serial.println(F("SDA Low"));
  digitalWrite(SDA_pin, LOW);   //SDA
  delay(3000);
  count = 16384;
  Serial.print(F("Creating fixed number of pulses..: ")); Serial.print(count);                      Serial.println(F(" "));
  myABLIC_S35770.create_fixed_number_of_pulse_at_counter_input_pin(count);
  pinMode(SDA_pin, OUTPUT);     //SDA
  Serial.println(F("SDA High"));
  digitalWrite(SDA_pin, HIGH);   //SDA
  Serial.println(F("myABLIC_S35770.begin"));
  myABLIC_S35770.begin();
  Serial.print(F("Read count register..............: ")); Serial.print(myABLIC_S35770.getCount());  Serial.println(F(" "));
  //
  // IMPORTANT: The ABLIC_S35770 binary counter stops counting if SDA is low (as described in the data sheet)!  Therefore accessing another I2C slave may influence the counting result!!!!
  //
  Serial.print(F("IMPORTANT: Second pulse sequence has not been counted since SDA was low!!!"));
  //
  //reset counter and disable counting by setting RSTB=0
  Serial.println(F("Disable counting and reset counter of ABLIC S-35770 counter IC")); 
  myABLIC_S35770.reset_counter_and_stop_counting();     // PD7, IDE pin 15 set to 0
  Serial.print(F("Read count register..............: ")); Serial.print(myABLIC_S35770.getCount());  Serial.println(F(" "));
  Serial.println(F(" "));

  
  max_count_index   = 32;  // stop  index for loop below; set max_count_index < count_index to disable the loop below
  count_index       = 14;  // start index for loop below

  if (count_index < max_count_index) {
    Serial.println(F(" "));
    Serial.println(F("Starting loop now .. "));
    Serial.println(F(" "));
  }
  else {
    Serial.println(F(" "));
    Serial.println(F("Loop is not started since max_count_index < count_index !"));
    Serial.println(F(" "));
  }
 
}
  
void loop()
{

  if (count_index < max_count_index) {
  
    myABLIC_S35770.enable_counting();
  
    Serial.print(F("Creating fixed number of pulses..: ")); Serial.print(count_index);                Serial.println(F(" "));
    myABLIC_S35770.create_fixed_number_of_pulse_at_counter_input_pin(count_index);
    Serial.print(F("Read count register..............: ")); Serial.print(myABLIC_S35770.getCount());  Serial.println(F(" "));
    Serial.println(F("Software resetting 24 Bit binary counter of ABLIC S-35770 counter IC"));
    myABLIC_S35770.soft_reset_counter();
    Serial.print(F("Read count register..............: ")); Serial.print(myABLIC_S35770.getCount());  Serial.println(F(" "));
    Serial.print(F("Creating fixed number of pulses..: ")); Serial.print(2*count_index);              Serial.println(F(" "));
    myABLIC_S35770.create_fixed_number_of_pulse_at_counter_input_pin(2*count_index);
    Serial.print(F("Read count register..............: ")); Serial.print(myABLIC_S35770.getCount());  Serial.println(F(" "));
    Serial.println(F("Hardware resetting 24 Bit binary counter of ABLIC S-35770 counter IC")); 
    myABLIC_S35770.reset_counter_and_stop_counting();
    Serial.print(F("Read count register..............: ")); Serial.print(myABLIC_S35770.getCount());  Serial.println(F(" "));
    Serial.println(F(" "));

    Serial.println(F("Starts over again in 1 sec."));
    Serial.println(F(" "));
  }
  else {
    if (count_index == max_count_index){
      Serial.println(F("Stop incrementing loop index! Upper boarder has been reached!"));
    }
  }
  count_index++;
  delay(1000);
}
