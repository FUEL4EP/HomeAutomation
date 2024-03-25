//---------------------------------------------------------
// Fujitsu_MB85RS2MTPF_FRAM_circular_buffer
// 2024-03-19 (C) FUEL4EP (Creative Commons by-nc-sa/)
// https://creativecommons.org/licenses/by-nc-sa/4.0/
// You are free to Share & Adapt under the following terms:
// Give Credit, NonCommercial, ShareAlike
// +++
//
// Arduino library for a circular buffer in 2x 2MBit SPI Fujitsu FRAM MB85RS2MTPF
//
// fitting to and required by Sens_weather_statistics_FRAM.h
//
//---------------------------------------------------------
//
// used libraries:
//
// Adafruit_FRAM_SPI         2013 Adafruit https://github.com/adafruit/Adafruit_FRAM_SPI (BSD License)
//
//---------------------------------------------------------
//
// This sketch is fitting to the PCB HB-UNI-SEN-BATT_ATMega1284P_E07-868MS10_FRAM_FUEL4EP
// see https://github.com/FUEL4EP/HomeAutomation/tree/master/AsksinPP_developments/PCBs/HB-UNI-SEN-BATT_ATMega1284P_E07-868MS10_FRAM_FUEL4EP
//
// Two 2Mbit Fujitsu SPI FRAMs MB85RS2MTPF are required
//
// The Arduino chip select pins for selecting these FRAMs should be (see also
// https://github.com/FUEL4EP/HomeAutomation/tree/master/AsksinPP_developments/PCBs/HB-UNI-SEN-BATT_ATMega1284P_E07-868MS10_FRAM_FUEL4EP/Schematics/HB-UNI-SEN-BATT_ATMega1284P_E07-868MS10_FRAM_FUEL4EP.pdf)
//
// FRAM_CS1_PIN = 26 // PA2
// FRAM_CS2_PIN = 27 // PA3
//
// see MightyCore pinout mapping https://github.com/MCUdude/MightyCore?tab=readme-ov-file
//
//---------------------------------------------------------

#ifndef _FUJITSU_MB85RS2MTPF_FRAM_CIRCULAR_BUFFER_
#define _FUJITSU_MB85RS2MTPF_FRAM_CIRCULAR_BUFFER_



#define DEEP_DEBUG // comment out if deep serial monitor debugging is not necessary

#include <SPI.h>
#include <Adafruit_FRAM_SPI.h>

//#include <AskSinPP.h>




namespace as {


class FUJITSU_MB85RS2MTPF_FRAMs {
// two 2Mbit Fujitsu SPI FRAMs MB85RS2MTPF are required, see above

private:

// please adapt these pin assignments to your hardware if you do not use the PCB HB-UNI-SEN-BATT_ATMega1284P_E07-868MS10_FRAM_FUEL4EP
const uint8_t FRAM_CS1_PIN = 26;
const uint8_t FRAM_CS2_PIN = 27;
const uint8_t FRAM_SCK = 7;
const uint8_t FRAM_MISO = 6;
const uint8_t FRAM_MOSI = 5;

// please adapt these FRAM IDs assignments to your hardware if you do not use the PCB HB-UNI-SEN-BATT_ATMega1284P_E07-868MS10_FRAM_FUEL4EP
const uint16_t  FUJITSU_MB85RS2MT_prodID    = 0x2803; // see datasheet MB85RS2MT, page 10
const uint8_t   FUJITSU_MB85RS2MT_manufID   = 0x04;   // see datasheet MB85RS2MT, page 10
const uint32_t  FUJITSU_MB85RS2MT_fram_size = 0x40000;   // 2MBit = 262 144 Bytes
const uint32_t  MAX_FRAM_BANK_ADDRESS       = FUJITSU_MB85RS2MT_fram_size * 2;   // 2 x 2MBit FRAMs: 0x7FFFF

/** Additional Operation Codes **/
typedef enum opcodes_e {
  OPCODE_SLEEP = 0b10111001 /* Speep Mode */
} opcodes_t;


// use hardware SPI mode to access the two FRAMs
// first FRAM
Adafruit_FRAM_SPI fram_1 = Adafruit_FRAM_SPI(FRAM_CS1_PIN);  // use hardware SPI
// second FRAM
Adafruit_FRAM_SPI fram_2 = Adafruit_FRAM_SPI(FRAM_CS2_PIN);  // use hardware SPI


Adafruit_FRAM_SPI *select_FRAM (uint32_t address) {

  uint8_t  FRAM_select_index   = address  / FUJITSU_MB85RS2MT_fram_size;

  switch (FRAM_select_index) {
    case 0:
      #ifdef DEEP_DEBUG
        DPRINTLN(F(".. selecting first FRAM"));
      #endif
      return &fram_1;
      break;
    case 1:
      #ifdef DEEP_DEBUG
        DPRINTLN(F(".. selecting second FRAM"));
      #endif
      return &fram_2;
      break;
    default:
      DPRINT(F("error: FRAM address is out of hardware bounds: 0x"));
      DHEX(address);
      DPRINTLN(F(" .. please check your software !"));
      return NULL;
      break;
  }
}

uint32_t FRAM_address (uint32_t address) {

   return address % FUJITSU_MB85RS2MT_fram_size;
}


bool check_FRAM_IDs(uint8_t manufID, uint16_t prodID) {

  bool correct_ID_flag = true;

  if ( manufID != FUJITSU_MB85RS2MT_manufID)
  {
    correct_ID_flag = false;
    DPRINTLN(F("FRAM has wrong manufacturer ID !"));
  }
  if ( prodID != FUJITSU_MB85RS2MT_prodID)
  {
    correct_ID_flag = false;
    DPRINTLN(F("FRAM has wrong product ID !"));
  }
  if ( correct_ID_flag ) {
    DPRINTLN(F("FRAM has correct IDs"));
    DPRINT(F("FRAM size is 0x"));
    DHEX(FUJITSU_MB85RS2MT_fram_size);
    DPRINTLN(F(" Bytes"));
  }

  return correct_ID_flag;
}

bool check_FRAM(Adafruit_FRAM_SPI &fram, const String& index_str){

  uint8_t  manufID;
  uint16_t prodID;

  DPRINT(F("Checking now "));
  DPRINT(index_str);
  DPRINTLN(F(" SPI FRAM"));
  if (fram.begin()) {
    DPRINT(F("Found "));
    DPRINT(index_str);
    DPRINTLN(F(" SPI FRAM"));
    fram.getDeviceID(&manufID, &prodID);
    DPRINT(F("-> FRAM Product ID 0x"));
    DHEX(prodID);
    DPRINTLN(F(""));
    DPRINT(F("-> FRAM Manufacturer ID 0x"));
    DHEX(manufID);
    DPRINTLN(F(""));

    if ( check_FRAM_IDs(manufID,prodID) == false ) {
      return false;
    }
    
    DPRINTLN(F("The first three data bytes of the FRAM are : "));

    DHEX(fram.read8(0x0));
    DPRINTLN(F(" "));
    DHEX(fram.read8(0x1));
    DPRINTLN(F(" "));
    DHEX(fram.read8(0x2));
    DPRINTLN(F(" "));

    return true;
  } else {
    DPRINT(F("No "));
    DPRINT(index_str);
    DPRINTLN(F(" SPI FRAM found ... check your connections"));
    return false;;
  }

}

public:

// constructor
FUJITSU_MB85RS2MTPF_FRAMs() {}

bool init_FRAMs () {

  bool FRAMs_found_flag=true;

  DPRINTLN(F("Initializing and checking FRAMs .."));

  // check first FRAM selected by FRAM_CS1_PIN
  if (check_FRAM(fram_1,"first") == false){
    FRAMs_found_flag = false;
  }

   // check second  FRAM selected by FRAM_CS2_PIN
  if (check_FRAM(fram_2,"second") == false){
    FRAMs_found_flag = false;
  }

  return FRAMs_found_flag;

}


uint8_t FRAM_read8(uint32_t addr) {

  Adafruit_FRAM_SPI* selected_FRAM_ptr;

  #ifdef DEEP_DEBUG
    DPRINTLN(F(""));
    DPRINTLN(F(".. reading 1 byte from FRAM bank .."));
    DPRINTLN(F(""));
  #endif
  
  uint32_t address_of_selected_FRAM = FRAM_address(addr);

  selected_FRAM_ptr = select_FRAM(addr);
  #ifdef DEEP_DEBUG
    DPRINT(F("FRAM address is "));
    DHEX(address_of_selected_FRAM);
    DPRINTLN(F(""));
  #endif

  if (selected_FRAM_ptr != NULL) {
    #ifdef DEEP_DEBUG
      DPRINT(F("read8 data is "));
      DHEX(selected_FRAM_ptr->read8(address_of_selected_FRAM));
      DPRINTLN(F(""));
    #endif
    return selected_FRAM_ptr->read8(address_of_selected_FRAM);
  }
  else {
    DPRINTLN(F("Error: reading dummy value!"));
    return 0xFF; // dummy value
  }  
}

uint16_t FRAM_read16(uint32_t addr) {

  uint8_t  lower_byte, upper_byte;
  uint16_t read_value;

  #ifdef DEEP_DEBUG
    DPRINTLN(F(""));
    DPRINTLN(F(".. reading 2 bytes from FRAM bank .."));
    DPRINTLN(F(""));
  #endif

  uint32_t address_of_selected_FRAM = FRAM_address(addr);

  // check if the read is crossing the address border between two FRAMs
  if (address_of_selected_FRAM > FUJITSU_MB85RS2MT_fram_size - sizeof(read_value)) {

    // split into two read8 if read16 crosses the address range of the FRAMs
    #ifdef DEEP_DEBUG
      DPRINTLN(F("splitted access to two FRAMs .."));
    #endif

    lower_byte = FRAM_read8(addr);
    upper_byte = FRAM_read8(addr+1);

    read_value = ((uint16_t)upper_byte << 8) + ((uint16_t)lower_byte);

    return read_value;
  }
  else
  // read16 data is stored in a single FRAM
  {
    Adafruit_FRAM_SPI* selected_FRAM_ptr;

    #ifdef DEEP_DEBUG
      DPRINTLN(F("single FRAM access .."));
    #endif

    selected_FRAM_ptr = select_FRAM(addr);
    #ifdef DEEP_DEBUG
      DPRINT(F("FRAM address is "));
      DHEX(address_of_selected_FRAM);
      DPRINTLN(F(""));
    #endif

    if (selected_FRAM_ptr != NULL) {
      #ifdef DEEP_DEBUG
        DPRINTLN(F("read16 data is "));
      #endif
      bool success = selected_FRAM_ptr->read(address_of_selected_FRAM,(uint8_t*)(&read_value),2);
      if ( !success ) {
        DPRINTLN(F("Error: reading dummy value!"));
        return 0xFFFF; // dummy value
      }
      #ifdef DEEP_DEBUG
        DHEX(read_value);
        DPRINTLN(F(""));
      #endif
      return read_value;
    }
    else {
      DPRINTLN(F("Error: reading dummy value!"));
      return 0xFFFF; // dummy value
    }
  }
}


void dump_array (uint32_t addr, uint8_t *values, size_t count) {

  size_t i;
  uint32_t laddr = addr;
  
  DPRINTLN(F(""));
  DPRINTLN(F(".. dumping an array .."));
  DPRINTLN(F(""));

  for (i = 0; i < count; i++) {
    if ((i % 16) == 0) {
      DPRINTLN(F(""));
      DPRINT(F("index 0x"));
      DHEX(laddr);
      DPRINT(F(": "));
    }
    DPRINT(F(" 0x"));
    DHEX(*(values+i));
    DPRINT(F(" "));
    laddr++;
  }
  DPRINTLN(F(""));
}

void fill_block_with_dummy_values(uint8_t *values, size_t count, uint8_t dummy_value) {
  size_t i = 0;
  while ( i < count) {
    *(values+i) = dummy_value;
    i++;
  }
}


bool FRAM_read(uint32_t addr,  uint8_t *values, size_t count) {

  bool success1 = false;
  bool success2 = false;

  // check for count == 1
  if ( count == 1 ) {
     #ifdef DEEP_DEBUG
      DPRINTLN(F(""));
      DPRINTLN(F(".. unusual single byte block read from FRAM bank .."));
      DPRINTLN(F(""));
    #endif
    *values = FRAM_read8(addr);
    return true;
  }

  Adafruit_FRAM_SPI* selected_FRAM_ptr;
  size_t count_lower_FRAM, count_upper_FRAM;

  #ifdef DEEP_DEBUG
    DPRINTLN(F(""));
    DPRINTLN(F(".. reading a block from FRAM bank .."));
    DPRINTLN(F(""));
  #endif

  // check if a block read adress would be beyond the maximum hardware address of the FRAM bank
  if (addr > MAX_FRAM_BANK_ADDRESS - count) {
    DPRINT(F("error: FRAM block access would be out of hardware bounds: 0x"));
    DHEX(addr);
    DPRINTLN(F(" .. please check your software !"));
    fill_block_with_dummy_values(values, count, 0xFF);
    return false;
  }

  uint32_t address_of_selected_FRAM = FRAM_address(addr);

  // check if the block read is crossing the address border between the two FRAMs
  if (address_of_selected_FRAM > FUJITSU_MB85RS2MT_fram_size - count) {


    // split into two block reads that are crossing the address range of the two FRAMs
    #ifdef DEEP_DEBUG
      DPRINTLN(F("splitted block access to two FRAMs .."));
    #endif

    count_lower_FRAM =  FUJITSU_MB85RS2MT_fram_size - address_of_selected_FRAM;  // number of bytes read from lower FRAM
    count_upper_FRAM =  count - count_lower_FRAM;                                // number of bytes read from upper FRAM

    selected_FRAM_ptr = select_FRAM(addr);
    #ifdef DEEP_DEBUG
      DPRINT(F("FRAM address is "));
      DHEX(address_of_selected_FRAM);
      DPRINTLN(F(""));
    #endif

    if (selected_FRAM_ptr != NULL) {
      // read count_lower_FRAM bytes from lower FRAM
      success1 = selected_FRAM_ptr->read(address_of_selected_FRAM, values, count_lower_FRAM);
    }

    selected_FRAM_ptr = select_FRAM(FUJITSU_MB85RS2MT_fram_size);
    #ifdef DEEP_DEBUG
      DPRINT(F("FRAM address is "));
      DHEX(FUJITSU_MB85RS2MT_fram_size);
      DPRINTLN(F(""));
    #endif
    if (success1) {
      // read remaining count_upper_FRAM bytes from upper FRAM
      success2 = selected_FRAM_ptr->read(FUJITSU_MB85RS2MT_fram_size, values+count_lower_FRAM, count_upper_FRAM);
    }
    #ifdef DEEP_DEBUG
      if (success1 && success2 ){
        dump_array (addr, values, count);
      }
      else {
        fill_block_with_dummy_values(values, count, 0xFF);
        DPRINTLN(F("Error occured when reading block!"));
      }
    #endif
    return success1 && success2;
  }

  else
  // read block data is stored in a single FRAM
  {
    Adafruit_FRAM_SPI* selected_FRAM_ptr;

    #ifdef DEEP_DEBUG
      DPRINTLN(F("single FRAM block access .."));
    #endif

    selected_FRAM_ptr = select_FRAM(addr);
    #ifdef DEEP_DEBUG
      DPRINT(F("FRAM address is "));
      DHEX(address_of_selected_FRAM);
      DPRINTLN(F(""));
    #endif

    if (selected_FRAM_ptr != NULL) {
      bool success1 = selected_FRAM_ptr->read(address_of_selected_FRAM,values,count);
      #ifdef DEEP_DEBUG
        if (success1 ){
          dump_array (addr, values, count);
        }
        else {
          fill_block_with_dummy_values(values, count, 0xFF);
          DPRINTLN(F("Error occured when reading block!"));
        }
      #endif
      return success1;
    }
    else {
      fill_block_with_dummy_values(values, count, 0xFF);
      DPRINTLN(F("Error occured when reading block!"));
      return false;
    }
  }
}

bool FRAM_writeEnable(bool enable) {

// IMPORTANT: Please note that according the data sheet the FRAM write enable latch WEL is reset after
// Write Enable Latch
// This indicates FRAM Array and status register are writable. The WREN
// command is for setting, and the WRDI command is for resetting. With the
// RDSR command, reading is possible but writing is not possible with the
// WRSR command. WEL is reset after the following operations.
// After power ON.
// After WRDI command recognition.
// The rising edge of CS after WRSR command recognition.
// The rising edge of CS after WRITE command recognition.


  bool success1, success2;
  if (enable) {
     // enable the FRAMs for write operations
    success1 = fram_1.writeEnable(true);
    success2 = fram_2.writeEnable(true);
    #ifdef DEEP_DEBUG
      DPRINTLN(F(".. enabled FRAMs for write operations"));
    #endif
  }
  else {
    // disable the FRAMs for write operations
    success1 = fram_1.writeEnable(false);
    success2 = fram_2.writeEnable(false);
    #ifdef DEEP_DEBUG
      DPRINTLN(F(".. disabled FRAMs for write operations"));
    #endif
  }
  return success1 && success2;
}


bool FRAM_write8(uint32_t addr, uint8_t value) {

  // IMPORTANT: Please note that according the data sheet the FRAM write enable latch WEL is reset after
// Write Enable Latch
// This indicates FRAM Array and status register are writable. The WREN
// command is for setting, and the WRDI command is for resetting. With the
// RDSR command, reading is possible but writing is not possible with the
// WRSR command. WEL is reset after the following operations.
// After power ON.
// After WRDI command recognition.
// The rising edge of CS after WRSR command recognition.
// The rising edge of CS after WRITE command recognition.



  Adafruit_FRAM_SPI* selected_FRAM_ptr;

  #ifdef DEEP_DEBUG
    DPRINTLN(F(""));
    DPRINTLN(F(".. writing 1 byte to FRAM bank .."));
    DPRINTLN(F(""));
  #endif

  uint32_t address_of_selected_FRAM = FRAM_address(addr);

  selected_FRAM_ptr = select_FRAM(addr);
  #ifdef DEEP_DEBUG
    DPRINT(F("FRAM address is "));
    DHEX(address_of_selected_FRAM);
    DPRINTLN(F(""));
  #endif

  if (selected_FRAM_ptr != NULL) {
    #ifdef DEEP_DEBUG
      DPRINT(F("write8 data is "));
      DHEX(value);
      DPRINTLN(F(""));
    #endif
    selected_FRAM_ptr->writeEnable(true);  // enable a write operation, i.e. set RAM write enable latch WEL
    return selected_FRAM_ptr->write8(address_of_selected_FRAM, value);
  }
  else {
    DPRINTLN(F("Error writing a byte to FRAM"));
    return false;
  }
}



bool FRAM_write16(uint32_t addr, uint16_t value) {

// IMPORTANT: Please note that according the data sheet the FRAM write enable latch WEL is reset after
// Write Enable Latch
// This indicates FRAM Array and status register are writable. The WREN
// command is for setting, and the WRDI command is for resetting. With the
// RDSR command, reading is possible but writing is not possible with the
// WRSR command. WEL is reset after the following operations.
// After power ON.
// After WRDI command recognition.
// The rising edge of CS after WRSR command recognition.
// The rising edge of CS after WRITE command recognition.

  uint8_t  lower_byte, upper_byte;
  bool success1, success2;

  #ifdef DEEP_DEBUG
    DPRINTLN(F(""));
    DPRINTLN(F(".. writing 2 bytes to FRAM bank .."));
    DPRINTLN(F(""));
  #endif

  #ifdef DEEP_DEBUG
      DPRINT(F("write16 data is "));
      DHEX(value);
      DPRINTLN(F(""));
  #endif

  uint32_t address_of_selected_FRAM = FRAM_address(addr);

  // check if the write is crossing the address border between two FRAMs
  if (address_of_selected_FRAM > FUJITSU_MB85RS2MT_fram_size - sizeof(value)) {

    // split into two write8 if write16 crosses the address range of the FRAMs
    #ifdef DEEP_DEBUG
      DPRINTLN(F("splitted access to two FRAMs .."));
    #endif

    lower_byte = value & 0xFF;
    upper_byte = value >> 8;

    success1 = FRAM_write8(addr,lower_byte);
    success2 = FRAM_write8(addr+1,upper_byte);

    return success1 && success2;
  }
  else
  // write16 data is stored into a single FRAM
  {
    Adafruit_FRAM_SPI* selected_FRAM_ptr;

    #ifdef DEEP_DEBUG
      DPRINTLN(F("single FRAM access .."));
    #endif

    selected_FRAM_ptr = select_FRAM(addr);
    #ifdef DEEP_DEBUG
      DPRINT(F("FRAM address is "));
      DHEX(address_of_selected_FRAM);
      DPRINTLN(F(""));
    #endif

    if (selected_FRAM_ptr != NULL) {
      #ifdef DEEP_DEBUG
        DPRINTLN(F("write16 data is "));
        DHEX(value);
        DPRINTLN(F(""));
      #endif
      selected_FRAM_ptr->writeEnable(true);  // enable a write operation, i.e. set RAM write enable latch WEL
      bool success = selected_FRAM_ptr->write(address_of_selected_FRAM,(uint8_t*)(&value),2);
      return success;
    }
    else {
      DPRINTLN(F("Error writing two bytes to FRAM!"));
      return false;
    }
  }
}

bool FRAM_write(uint32_t addr,  uint8_t *values, size_t count) {


// IMPORTANT: Please note that according the data sheet the FRAM write enable latch WEL is reset after
// Write Enable Latch
// This indicates FRAM Array and status register are writable. The WREN
// command is for setting, and the WRDI command is for resetting. With the
// RDSR command, reading is possible but writing is not possible with the
// WRSR command. WEL is reset after the following operations.
// After power ON.
// After WRDI command recognition.
// The rising edge of CS after WRSR command recognition.
// The rising edge of CS after WRITE command recognition.


  bool success1 = false;
  bool success2 = false;

  // check for count == 1
  if ( count == 1 ) {
     #ifdef DEEP_DEBUG
      DPRINTLN(F(""));
      DPRINTLN(F(".. unusual single byte block write to FRAM bank .."));
      DPRINTLN(F(""));
    #endif
    success1 = FRAM_write8(addr,*values);
    return success1;
  }

  Adafruit_FRAM_SPI* selected_FRAM_ptr;
  size_t count_lower_FRAM, count_upper_FRAM;

  #ifdef DEEP_DEBUG
    DPRINTLN(F(""));
    DPRINTLN(F(".. writing a block to FRAM bank .."));
    DPRINTLN(F(""));
  #endif

  // check if a block write adress would be beyond the maximum hardware address of the FRAM bank
  if (addr > MAX_FRAM_BANK_ADDRESS - count) {
    DPRINT(F("error: FRAM block access would be out of hardware bounds: 0x"));
    DHEX(addr);
    DPRINTLN(F(" .. please check your software !"));
    return false;
  }

  uint32_t address_of_selected_FRAM = FRAM_address(addr);

  // check if the block write is crossing the address border between the two FRAMs
  if (address_of_selected_FRAM > FUJITSU_MB85RS2MT_fram_size - count) {


    // split into two block writes that are crossing the address range of the two FRAMs
    #ifdef DEEP_DEBUG
      DPRINTLN(F("splitted block access to two FRAMs .."));
    #endif

    count_lower_FRAM =  FUJITSU_MB85RS2MT_fram_size - address_of_selected_FRAM;  // number of bytes to be written to lower FRAM
    count_upper_FRAM =  count - count_lower_FRAM;                                // number of bytes to be written to upper FRAM
    selected_FRAM_ptr = select_FRAM(addr);
    #ifdef DEEP_DEBUG
      DPRINT(F("FRAM address is "));
      DHEX(address_of_selected_FRAM);
      DPRINTLN(F(""));
    #endif

    if (selected_FRAM_ptr != NULL) {
      selected_FRAM_ptr->writeEnable(true);  // enable a write operation, i.e. set RAM write enable latch WEL
      // write count_lower_FRAM bytes to lower FRAM
      success1 = selected_FRAM_ptr->write(address_of_selected_FRAM, values, count_lower_FRAM);
    }

    selected_FRAM_ptr = select_FRAM(FUJITSU_MB85RS2MT_fram_size);
    #ifdef DEEP_DEBUG
      DPRINT(F("FRAM address is "));
      DHEX(FUJITSU_MB85RS2MT_fram_size);
      DPRINTLN(F(""));
    #endif
    if (success1) {
      selected_FRAM_ptr->writeEnable(true);  // enable a write operation, i.e. set RAM write enable latch WEL
      // read remaining count_upper_FRAM bytes from upper FRAM
      success2 = selected_FRAM_ptr->write(FUJITSU_MB85RS2MT_fram_size, values+count_lower_FRAM, count_upper_FRAM);
    }
    #ifdef DEEP_DEBUG
      if (!success1 || !success2 ) {
        DPRINTLN(F("Error occured when writing a splitted block!"));
      }
    #endif
    return success1 && success2;
  }

  else
  // write block data to be written to a single FRAM
  {
    Adafruit_FRAM_SPI* selected_FRAM_ptr;

    #ifdef DEEP_DEBUG
      DPRINTLN(F("single FRAM block access .."));
    #endif

    selected_FRAM_ptr = select_FRAM(addr);
    #ifdef DEEP_DEBUG
      DPRINT(F("FRAM address is "));
      DHEX(address_of_selected_FRAM);
      DPRINTLN(F(""));
    #endif

    if (selected_FRAM_ptr != NULL) {
      selected_FRAM_ptr->writeEnable(true);  // enable a write operation, i.e. set RAM write enable latch WEL
      bool success1 = selected_FRAM_ptr->write(address_of_selected_FRAM,values,count);
      #ifdef DEEP_DEBUG
        if (!success1 ){
          DPRINTLN(F("Error occured when writing a block!"));
        }
      #endif
      return success1;
    }
    else {
      DPRINTLN(F("Error occured when writing block!"));
      return false;
    }
  }
}



bool FRAM_flush(uint8_t value)
{

  #ifdef DEEP_DEBUG
    DPRINTLN(F(""));
    DPRINTLN(F(".. flushing the FRAMs .."));
    DPRINTLN(F(""));
  #endif
  bool success = true;

  uint8_t buf[32];
  for (uint8_t i = 0; i < 32; i++) buf[i] = value;
  
  for (uint32_t address = 0; address < FUJITSU_MB85RS2MT_fram_size; address += 32)
  {
    if ((address % 8192) == 0) {
      DPRINTLN(F(""));
    }
    if ((address % 128) == 0) {
      DPRINT(F("."));
    }

    
    fram_1.writeEnable(true);
    success &= fram_1.write(address,buf,32);  // flush lower FRAM block
    fram_2.writeEnable(true);
    success &= fram_2.write(address,buf,32);  // flush upper FRAM block
  }
  DPRINTLN(F(""));
  DPRINTLN(F("Both FRAMs were flushed"));
  DPRINTLN(F(""));
  #ifdef DEEP_DEBUG
    if (!success ){
      DPRINTLN(F("Error occured when flushing FRAMs!"));
    }
    else
    {
      DPRINTLN(F("FRAMs were flushed successfully"));
    }
  #endif
  
  return success;
}

//!!!!!!!!!!!!!!!!!!!!!!
// to be done
//!!!!!!!!!!!!!!!!!!!!!!

// requires extension of Adafruit's Adafruit_FRAM_SPI class, need to create a fork of Adafruit's repository
bool FRAM_sleep_mode() {
  return false;
}



void cold_boot() {
}


void warm_boot() {
}

};


  
}

#endif
