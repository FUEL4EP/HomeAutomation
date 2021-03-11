/***************************************************************************************************/

// ABLIC_S35770    This is an Arduino basic library for ABLIC_S35770 COUNTER IC WITH 2-WIRE (I2C-bus) INTERFACE
// Version 1.0
// (C) 2021 FUEL4EP        (Creative Commons License)          
// https://creativecommons.org/licenses/by-nc-sa/4.0/
// You are free to Share & Adapt under the following terms:
// Give Credit, NonCommercial, ShareAlike
// +++
//
//
//A datasheet of the counter IC is avilable at https://www.ablic.com/en/doc/datasheet/counter_ic/S35770_I_E.pdf

/***************************************************************************************************/

#ifndef ABLIC_S35770_h
#define ABLIC_S35770_h

#include <Arduino.h>
#include <Wire.h>

/* misc */
#define ABLIC_S35770_I2CADDR                        0x32   //i2c address: The slave address of the S-35770 is specified to "0110010".
#define RESTART_FLAG                                false  //flag for I2C write operation with restart
#define STOP_CONDITION_FLAG                         true   //flag for I2C write operation without restart
#define DUMMY_DATA_READ                             0x01   //dummy data for reading free register, B7=0, B0=1, B6..B1 are arbitrary
#define DUMMY_DATA_WRITE                            0xFF   //dummy data for writing free register, B7=1, B0=1, B6..B1 are arbitrary
#define NACK_RECEIVED_AFTER_ADDRESS_TRANSMISSION    0x04   //received NACK on transmit of address
#define NACK_RECEIVED_AFTER_DATA_TRANSMISSION       0x05   //received NACK on transmit of data 


class ABLIC_S35770
{
 public:
 
  ABLIC_S35770(int resetb_pin, int counter_input_pin);

  void     begin(void);
  boolean  check(uint8_t addr = ABLIC_S35770_I2CADDR);
 
  
  uint32_t getCount(void);
  uint32_t read_free_register(void);
  byte     write_free_register(uint32_t val);
  void     soft_reset_counter(void);
  
  void     set_FXX_bit_of_free_register(uint8_t bit_sel);
  void     clear_FXX_bit_of_free_register(uint8_t bit_sel);
  
  void     set_F_part_F20_F0_of_free_register(uint32_t val);
  uint32_t read_F_part_F20_F0_of_free_register(void);
  
  void     enable_counting(void);                    // controlled by ABLIC_S35770 reset pin: RSTB(ABLIC_S35770) = 1
  void     reset_counter_and_stop_counting(void);    // controlled by ABLIC_S35770 reset pin: RSTB(ABLIC_S35770) = 0
  void     counter_signal_on(void);
  void     counter_signal_off(void);
  void     create_fixed_number_of_pulse_at_counter_input_pin(uint32_t n);

 private:
  uint8_t  _i2c_addr;


  byte     write_8Bit_Reg(uint8_t val, bool restart_flag);
  byte     write_24Bit_Reg(uint32_t val);

  uint8_t  read_8Bit_Reg(uint8_t reg);
  uint32_t read_24Bit_Reg(uint8_t reg);
  int      _resetb_pin;
  int      _counter_input;

};


#endif //#ifndef ABLIC_S35770_h
