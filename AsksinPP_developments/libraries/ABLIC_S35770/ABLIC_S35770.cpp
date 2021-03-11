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
//A datasheet of the ABLIC counter IC is avilable at https://www.ablic.com/en/doc/datasheet/counter_ic/S35770_I_E.pdf

/***************************************************************************************************/

#include <Wire.h>
#include "ABLIC_S35770.h"
#define  DEBUG  //comment out if no serial debug messages are wanted

ABLIC_S35770::ABLIC_S35770(int resetb_pin, int counter_input_pin) {  // resetb_pin is the Arduino pin that is connected to ABLIC_S35770's RSTB pin
                                                                     // counter_input_pin is the Arduino pin that is connected for test purposes to ABLIC_S35770's CLKIN pin 

  pinMode(resetb_pin, OUTPUT);
  _resetb_pin = resetb_pin;
  
  pinMode(counter_input_pin, OUTPUT);
  _counter_input = counter_input_pin;
  
}

/**************************************************************************/
/*
    begin()

    Initializes I2C

*/
/**************************************************************************/

bool ABLIC_S35770::check(uint8_t addr) 
{
  byte error;
  
  _i2c_addr = addr;
  //Serial.println(_i2c_addr,HEX);
  Wire.beginTransmission(_i2c_addr);
  error = Wire.endTransmission();
  Wire.beginTransmission(_i2c_addr+1);
  if (error == 0 && Wire.endTransmission() != 0 )
  {
    Serial.print("SUCCESS: ABLIC S-35770 counter IC device found at I2C address 0x");
    Serial.print(_i2c_addr,HEX);
    Serial.println(" !");
    Serial.println("ABLIC S-35770 counter IC with I2C interface seems to be present !");
    return true;
  }
  else
  {
    Serial.print("ERROR: No I2C device found at address 0x");
    Serial.print(_i2c_addr,HEX);
    Serial.println(" !");
    Serial.println("ERROR: ABLIC S-35770 counter IC with I2C interface seems NOT to be present !");
    return false;
  }
}


// initializes the I2C bus, sets the I2C clock to 100kbaud; enable counting of ABLIC_S35770
void ABLIC_S35770::begin() 
{
  Wire.begin();
  Wire.setClock(100000);
  enable_counting();
}


// do a soft reset of the 24 bit counter register
void ABLIC_S35770::soft_reset_counter(void){
  uint32_t free_reg_val =  read_free_register(); 
  free_reg_val &= 0x00FFFFF8;  // mask bits RST2 = 0, RST1 = 0, RST0 = 0
  free_reg_val |= 0x00000002;  // set RST2 = 0, RST1 = 1, RST0 = 0; this resets the counter
  write_free_register(free_reg_val);
}


// get the count of the counter register, counting is stopped between I2C start and stop condition!
uint32_t ABLIC_S35770::getCount(void){
  uint32_t val = read_24Bit_Reg(_i2c_addr);
  return val;
}


// read the free register
uint32_t ABLIC_S35770::read_free_register(void){
  // write a dummy byte with restart
  //Serial.println(DUMMY_DATA_READ, HEX);
  write_8Bit_Reg(DUMMY_DATA_READ, RESTART_FLAG);
  uint32_t val = read_24Bit_Reg(_i2c_addr);
  return val;
}

// write the free register
byte ABLIC_S35770::write_free_register(uint32_t val){
    
  byte error;
  // write a dummy byte with restart
  Wire.beginTransmission(_i2c_addr);
  Wire.write(DUMMY_DATA_WRITE);
  val = val & 0x00FFFFFF;  // mask lower 24 bits
  uint8_t byte_char;
  byte_char = (val >> 16) & 0xFF;
  Wire.write(byte_char);
  byte_char = (val >> 8) & 0xFF;  
  Wire.write(byte_char);
  byte_char = val & 0xFF;
  Wire.write(byte_char);
  error = Wire.endTransmission(STOP_CONDITION_FLAG);
  if(error > 0)  
  { Serial.print(F("FAIL in ABLIC_S35770 write_free_register! Error code: "));Serial.println(error); }
  return error;
}


// set the (bit_sel)th bit of the F20..F0 part of the free register (upper 21 bits); keep all other bits unchanged
void ABLIC_S35770::set_FXX_bit_of_free_register(uint8_t bit_sel) { // bit_sel := 0..20; F00 .. F20
  if ( bit_sel < 21 ) { // only for valid bit selector
    uint32_t free_reg_val =  read_free_register();
    free_reg_val |= ((uint32_t)1<<(bit_sel+3));
    write_free_register(free_reg_val);
  }
  else {
     Serial.print(F("Illegal bit_sel in ABLIC_S35770 set_FXX_bit_of_free_register! bit_sel: "));Serial.println(bit_sel); 
  }
}


// clear the (bit_sel)th bit of the F20..F0 part of the free register (upper 21 bits); keep all other bits unchanged
void ABLIC_S35770::clear_FXX_bit_of_free_register(uint8_t bit_sel) { // bit_sel := 0..20; F00 .. F20
  if ( bit_sel < 21 ) { // only for valid bit selector
    uint32_t free_reg_val =  read_free_register();
    free_reg_val &= ~((uint32_t)1<<(bit_sel+3));
    write_free_register(free_reg_val);
  }
  else {
     Serial.print(F("Illegal bit_sel in ABLIC_S35770 clear_FXX_bit_of_free_register! bit_sel: "));Serial.println(bit_sel); 
  }
}


// write the F20..F0 part of the free register (upper 21 bits); keep lower 3 bits unchanged
void ABLIC_S35770::set_F_part_F20_F0_of_free_register(uint32_t val) { // F20 .. F0 (21 upper bits)
  
  uint32_t free_reg_val =  read_free_register();
  free_reg_val &= 0x00000003; // set upper 21 bits to 0
  val &= 0x001FFFFF;          // mask valid bits (lower 21 bits)
  val <<= 3;                  // shift 3 bits to the left
  free_reg_val |= val;      
  write_free_register(free_reg_val);
}


// read the F20..F0 part of the free register (upper 21 bits); ignore the lower 3 bits of the free register
uint32_t ABLIC_S35770::read_F_part_F20_F0_of_free_register(void) { // F20 .. F0 (21 upper bits)
  
  uint32_t free_reg_val =  read_free_register();
  free_reg_val &= 0x00FFFFF8;  // mask upper 21 bits
  free_reg_val >>= 3;          // shift 3 bits to the right
  return free_reg_val;
}


//Low level read and write procedures
//===================================

//write a byte
byte ABLIC_S35770::write_8Bit_Reg(uint8_t val, bool restart_flag) {
  byte error;
  
  Wire.beginTransmission(_i2c_addr);
  // note: S-35770 is a bit special and does not require to write a register address before writing a value!
  // the register is somehow coded in the dummy data
  Wire.write(val);
  error = Wire.endTransmission(restart_flag);
  
  if(error > 0)  
  { Serial.print(F("FAIL in I2C write_8Bit_Reg! Error code: "));Serial.println(error); }
  
  return error;
}


//read a byte
uint8_t ABLIC_S35770::read_8Bit_Reg(uint8_t reg) {
  Wire.beginTransmission(_i2c_addr);
  Wire.write((uint8_t)reg);
  Wire.endTransmission();

  Wire.requestFrom((uint8_t)_i2c_addr, (uint8_t)1);
  return Wire.read();
}




//read 24 bits
uint32_t ABLIC_S35770::read_24Bit_Reg(uint8_t reg) {
  uint32_t val   = 0;
  int8_t   count = 0;
  byte     byte_char;

  byte number_of_received_bytes;
  
  number_of_received_bytes = Wire.requestFrom((uint8_t)_i2c_addr, (uint8_t)3);
  if ( number_of_received_bytes != 3 ) {
     Serial.print("read_24Bit_Reg: Received an incorrect number of bytes : "); 
     Serial.println(number_of_received_bytes,HEX);
  } 
  
  while(Wire.available())       // slave may send less than requested
  {
    byte_char = Wire.read();    // receive a byte as character
    //Serial.println(byte_char,HEX);
    val <<= 8;
    val |= byte_char;           // 
    count = count + 1;
    //Serial.println(count,HEX);
  }
  if ( count != 3 ) {
     Serial.print("read_24Bit_Reg: Read an incorrect number of bytes : "); 
     Serial.println(number_of_received_bytes,HEX);
  } 
  //Serial.println(val,HEX);
  return val;
}




//write 24 bits
byte ABLIC_S35770::write_24Bit_Reg(uint32_t val) {
    
  val = val & 0x00FFFFFF;  // mask lower 24 bits
  
  uint8_t byte_char;
  byte    error;
  
  byte_char = (val >> 16) & 0xFF;
  //Serial.println(byte_char,HEX);
  Wire.write(byte_char);
  byte_char = (val >> 8) & 0xFF;
  //Serial.println(byte_char,HEX);
  Wire.write(byte_char);
  byte_char = val & 0xFF;
  //Serial.println(byte_char,HEX);
  Wire.write(byte_char);
  error = Wire.endTransmission(STOP_CONDITION_FLAG);
  if(error > 0)  
  { Serial.print(F("FAIL in I2C write_24Bit_Reg! Error code: "));Serial.println(error); }
  return error;
}



// enable counting of the ABLIC_S35770 by assigning a 1 to S35770's RSTB pin
void ABLIC_S35770::enable_counting(void)
{
  digitalWrite(_resetb_pin, HIGH);
}

// hardware reset the counter and stop counting of the ABLIC_S35770 by assigning a 0 to S35770's RSTB pin
void ABLIC_S35770::reset_counter_and_stop_counting(void)
{
  digitalWrite(_resetb_pin, LOW);
}

// for test purposes only: set counter input S35770's CLKIN to 1 by asserting the connected _counter_input pin of the Arduino
void ABLIC_S35770::counter_signal_on(void)
{
  digitalWrite(_counter_input, HIGH);
}

// for test purposes only: reset counter input S35770's CLKIN to 0 by asserting the connected _counter_input pin of the Arduino
void ABLIC_S35770::counter_signal_off(void)
{
  digitalWrite(_counter_input, LOW);
}

// create n pulses at the _counter_input pin of the Arduino
 void  ABLIC_S35770::create_fixed_number_of_pulse_at_counter_input_pin(uint32_t n){
     
     int pulse_index = n;
     while (pulse_index > 0) {
       digitalWrite(_counter_input, HIGH);
       delayMicroseconds(20);
       digitalWrite(_counter_input, LOW);
       delayMicroseconds(20);
       pulse_index--;
     }
 }
