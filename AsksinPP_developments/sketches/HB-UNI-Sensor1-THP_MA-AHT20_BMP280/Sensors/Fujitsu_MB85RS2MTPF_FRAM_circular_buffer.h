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



//#define DEEP_DEBUG // comment out if deep serial monitor debugging is not necessary

#include <SPI.h>
#include <Adafruit_FRAM_SPI.h>

const uint8_t  FRAMS_INITIALIZED   = 0xFF;         //   FRAMS_INITIALIZED = 0xFF       FRAM is initialized
const uint8_t  FRAMS_UNINITIALIZED = 0x00;         //   FRAMS_INITIALIZED = 0x00       FRAM is not (yet) initialized
const uint8_t  FRAM_UNPROTECT      = 0b10000010;   //   FRAM status register setting for unprotecting access: WPEN=1, BP1=0, BP0=0, WEL=1

//#include <AskSinPP.h>

#ifndef FLUSH_INDICATION_LED_PIN                  // Arduino LED pin to indicate an ongoing FRAM flushing by blinking
#define FLUSH_INDICATION_LED_PIN 12
#endif


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
const uint16_t  FUJITSU_MB85RS2MT_prodID    = 0x2803;                            // see datasheet MB85RS2MT, page 10
const uint8_t   FUJITSU_MB85RS2MT_manufID   = 0x04;                              // see datasheet MB85RS2MT, page 10
const uint32_t  FUJITSU_MB85RS2MT_fram_size = 0x40000;   // 2MBit = 262 144 Bytes
const uint32_t  MAX_FRAM_BANK_ADDRESS       = FUJITSU_MB85RS2MT_fram_size * 2;   // 2 x 2MBit FRAMs: 0x00000 .. 0x7FFFF
const uint32_t  SPI_FREQ                    = (uint32_t)4000000;                 // SPI freq is limited to half of CPU frequency

/** Additional Operation Codes **/
typedef enum opcodes_e {
  OPCODE_SLEEP = 0b10111001 /* Speep Mode */
} opcodes_t;


// use hardware SPI mode to access the two FRAMs
// first FRAM
Adafruit_FRAM_SPI fram_1 = Adafruit_FRAM_SPI(FRAM_CS1_PIN, &SPI, SPI_FREQ);  // use hardware SPI
// second FRAM
Adafruit_FRAM_SPI fram_2 = Adafruit_FRAM_SPI(FRAM_CS2_PIN, &SPI, SPI_FREQ);  // use hardware SPI


Adafruit_FRAM_SPI *select_FRAM (uint32_t address) {

  uint8_t  FRAM_select_index   = (uint8_t)(address  / FUJITSU_MB85RS2MT_fram_size);

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
      DPRINT(F("ERROR: FRAM address is out of hardware bounds: 0x"));
      DHEXLN(address);
      DPRINTLN(F(" .. please check your software !"));
      DPRINT(F("ERROR: uninitialized FRAM_select_index is: 0x"));
      DHEXLN(FRAM_select_index);
      DPRINT(F("ERROR: FUJITSU_MB85RS2MT_fram_size is: 0x"));
      DHEXLN(FUJITSU_MB85RS2MT_fram_size);
      return NULL;
      break;
  }
}

Adafruit_FRAM_SPI *select_FRAM_no_deep_debug (uint32_t address) {

  uint8_t  FRAM_select_index   = (uint8_t)(address  / FUJITSU_MB85RS2MT_fram_size);

  switch (FRAM_select_index) {
    case 0:
      return &fram_1;
      break;
    case 1:
      return &fram_2;
      break;
    default:
      DPRINT(F("ERROR: FRAM address is out of hardware bounds: 0x"));
      DHEXLN(address);
      DPRINTLN(F(" .. please check your software !"));
      DPRINT(F("ERROR: uninitialized FRAM_select_index is: 0x"));
      DHEXLN(FRAM_select_index);
      DPRINT(F("ERROR: FUJITSU_MB85RS2MT_fram_size is: 0x"));
      DHEXLN(FUJITSU_MB85RS2MT_fram_size);
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

void FRAM_read_status_register(Adafruit_FRAM_SPI &fram){
  DPRINT(F("Status register of FRAM is 0x"));
  uint8_t statuts_register_FRAM = fram.getStatusRegister();
  DHEXLN(statuts_register_FRAM);
}

bool write_status_register(Adafruit_FRAM_SPI &fram, uint8_t new_status_register_value){
   bool status;
   fram.writeEnable(true);
   status = fram.setStatusRegister(new_status_register_value);
   return status;
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
    DPRINTLN(F(""));
    DHEX(fram.read8(0x1));
    DPRINTLN(F(""));
    DHEX(fram.read8(0x2));
    DPRINTLN(F(""));

    FRAM_read_status_register(fram);
    DPRINTLN(F("Removing write protection is FRAM status register now .."));
    (void)write_status_register(fram,FRAM_UNPROTECT);
    FRAM_read_status_register(fram);

    return true;
  } else {
    DPRINT(F("No "));
    DPRINT(index_str);
    DPRINTLN(F(" SPI FRAM found ... check your connections"));
    return false;;
  }

}

public:

bool init_FRAMs () {

  bool FRAMs_found_flag=true;

  DPRINTLN(F("Initializing and checking FRAMs .."));

  DPRINTLN(F("Setting FRAM flush indicator LED pin 'FLUSH_INDICATION_LED_PIN' as OUTPUT .."));
  pinMode(FLUSH_INDICATION_LED_PIN, OUTPUT);
  DPRINTLN(F("WARNING: Do not interrupt the flushing of the FRAMs indicated by a blinking LED at pin 'FLUSH_INDICATION_LED_PIN' .."));

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
    DPRINT(F("selected_FRAM_ptr is "));
    DHEX((uint32_t)selected_FRAM_ptr);
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
    DPRINTLN(F("ERROR: reading dummy value!"));
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
        DPRINTLN(F("ERROR: reading dummy value!"));
        return 0xFFFF; // dummy value
      }
      #ifdef DEEP_DEBUG
        DHEX(read_value);
        DPRINTLN(F(""));
      #endif
      return read_value;
    }
    else {
      DPRINTLN(F("ERROR: reading dummy value!"));
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
      DPRINT(F("address 0x"));
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
    DPRINT(F("ERROR: FRAM block access would be out of hardware bounds: 0x"));
    DHEXLN(addr);
    print_FRAM_sizes();
    DHEXLN(count);
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
      DPRINTLN(F(""));
      DPRINT(F("selected_FRAM_ptr is "));
      DHEX((uint32_t)selected_FRAM_ptr);
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
        DPRINTLN(F("ERROR occured when reading block!"));
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
      DPRINT(F("selected_FRAM_ptr is "));
      DHEX((uint32_t)selected_FRAM_ptr);
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
          DPRINTLN(F("ERROR occured when reading block!"));
        }
      #endif
      return success1;
    }
    else {
      fill_block_with_dummy_values(values, count, 0xFF);
      DPRINTLN(F("ERROR occured when reading block!"));
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
    DPRINTLN(F("ERROR writing a byte to FRAM"));
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
      DPRINTLN(F("ERROR writing two bytes to FRAM!"));
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
        DPRINTLN(F("ERROR occured when writing a splitted block!"));
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
          DPRINTLN(F("ERROR occured when writing a block!"));
        }
      #endif
      return success1;
    }
    else {
      DPRINTLN(F("ERROR occured when writing block!"));
      return false;
    }
  }
}

void indicate_flushing_by_toggling_LED() {
    digitalWrite(FLUSH_INDICATION_LED_PIN, !digitalRead(FLUSH_INDICATION_LED_PIN));
}


// flush the whole FRAM bank with a value
bool FRAM_flush(uint8_t value)
{

  const uint8_t BUF_SIZE=32;
  uint8_t       buf[BUF_SIZE];
  bool          success = true;
  uint32_t      address;

  Adafruit_FRAM_SPI* selected_FRAM_ptr;
  
  #ifdef DEEP_DEBUG
    DPRINTLN(F(""));
    DPRINTLN(F(".. flushing the FRAMs .."));
    DPRINTLN(F(""));
    DPRINT(F(".. flushing values is 0x"));
    DHEXLN(value);
  #endif
  
  for (uint8_t i = 0; i < BUF_SIZE; i++) {
    buf[i] = value;
    //DHEXLN(buf[i]);
  }


  for (address = 0; address < MAX_FRAM_BANK_ADDRESS; address += BUF_SIZE)
  {
    if ((address % 8192) == 0) {
      DPRINTLN(F(""));
      indicate_flushing_by_toggling_LED();
    }
    if ((address % 256) == 0) {
      DPRINT(F("."));  
    }

    selected_FRAM_ptr = select_FRAM_no_deep_debug(address);
    selected_FRAM_ptr->writeEnable(true);
    uint32_t address_of_selected_FRAM = FRAM_address(address);
    success &= selected_FRAM_ptr->write(address_of_selected_FRAM,buf,BUF_SIZE);  // flush one block of selected FRAM
  }
  DPRINTLN(F(""));
  DPRINT(F("final flushed address + 1 : 0x"));
  DHEXLN(address);
  DPRINTLN(F(""));
  DPRINTLN(F("both FRAMs were flushed"));
  DPRINTLN(F(""));
  digitalWrite(FLUSH_INDICATION_LED_PIN, LOW);
  #ifdef DEEP_DEBUG
    if (!success ){
      DPRINTLN(F("ERROR occured when flushing FRAMs!"));
    }
    else
    {
      DPRINTLN(F("FRAMs were flushed successfully"));
    }
  #endif
  
  return success;
}

void dump_begin_and_end_of_FRAMs() {

  const uint8_t BUF_SIZE = 32;
  uint8_t       buffer[BUF_SIZE];
  bool          status = true;

  DPRINTLN(F(".. dumping data block os FRAMs now .."));
  DPRINTLN(F("first BUF_SIZE bytes of first FRAM"));
  status &= FRAM_read(0x00000,                                  buffer, BUF_SIZE);   //first BUF_SIZE bytes of first FRAM
  DPRINTLN(F("last BUF_SIZE bytes of first FRAM"));
  status &= FRAM_read(FUJITSU_MB85RS2MT_fram_size   - BUF_SIZE, buffer, BUF_SIZE);   //last  BUF_SIZE bytes of first FRAM
  DPRINTLN(F("first BUF_SIZE bytes of second FRAM"));
  status &= FRAM_read(FUJITSU_MB85RS2MT_fram_size,              buffer, BUF_SIZE);   //first BUF_SIZE bytes of second FRAM
  DPRINTLN(F("last BUF_SIZE bytes of second FRAM"));
  status &= FRAM_read(2*FUJITSU_MB85RS2MT_fram_size - BUF_SIZE, buffer, BUF_SIZE);   //last  BUF_SIZE bytes of second FRAM
  #ifdef DEEP_DEBUG
  if (!status ){
    DPRINTLN(F("ERROR occured during dump_begin_and_end_of_FRAMs !"));
  }
  #endif
}


// flush a region of the FRAM bank with an uint8_t value
bool FRAM_region_flush8(uint8_t value, uint32_t start_address,uint32_t end_address)
{

  const uint32_t blockSize = 256;
  uint32_t current_address = start_address;

  #ifdef DEEP_DEBUG
    DPRINTLN(F(""));
    DPRINTLN(F(".. flushing a region of the FRAMs .."));
    DPRINTLN(F(""));
    DPRINT(F("start address of region : 0x"));
    DHEX(start_address);
    DPRINTLN(F(""));
    DPRINT(F("end address of region   : 0x"));
    DHEX(end_address);
    DPRINTLN(F(""));
    DPRINT(F("flushing with value     : 0x"));
    DHEX(value);
    DPRINTLN(F(""));
  #endif

  bool success = true;

  // check address range
  if (( end_address < start_address ) || ( start_address > MAX_FRAM_BANK_ADDRESS - 1 ) || ( end_address > MAX_FRAM_BANK_ADDRESS - 1 )){
    DPRINTLN(F(""));
    DPRINTLN(F("ERROR: wrong address range of region flush :"));
    DPRINTLN(F(""));
    DPRINT(F("start address of region : 0x"));
    DHEX(start_address);
    DPRINTLN(F(""));
    DPRINT(F("end address of region : 0x"));
    DHEX(end_address);
    DPRINTLN(F(""));
    success = false;
  }

  // fill buffer with value
  uint8_t buf[blockSize];
  for (uint16_t i = 0; i < blockSize; i++) buf[i] = value;

  while (success && (current_address < end_address)) {
    // Calculate the remaining bytes to erase
    uint32_t remainingBytes = end_address - current_address;

    // Determine the size of the block to erase
    size_t blockSizeToErase = min(blockSize, remainingBytes);

     #ifdef DEEP_DEBUG
      DPRINTLN(F(""));
      DPRINT(F("flushing block size : 0x"));
      DHEX(blockSizeToErase);
      DPRINTLN(F(""));
      DPRINT(F("current address     : 0x"));
      DHEX(current_address);
      DPRINTLN(F(""));
      DPRINT(F("remaining bytes     : 0x"));
      DHEX(remainingBytes);
    #endif

    success =  FRAM_write(current_address,  buf, blockSizeToErase);

    // Move to the next block
    current_address += (uint32_t)blockSizeToErase;

  }
  DPRINTLN(F(""));
  DPRINTLN(F("region of FRAM bank has been flushed"));
  DPRINTLN(F(""));
  #ifdef DEEP_DEBUG
    if (!success ){
      DPRINTLN(F("ERROR occured when flushing region of FRAMs!"));
    }
    else
    {
      DPRINTLN(F("region of FRAM bank was flushed successfully"));
    }
  #endif

  return success;
}



// flush a region of the FRAM bank with an uint16_t value
bool FRAM_region_flush16(uint16_t value, uint32_t start_address,uint32_t end_address)
{

  const uint32_t blockSize = 256;
  uint32_t current_address = start_address;

  #ifdef DEEP_DEBUG
    DPRINTLN(F(""));
    DPRINTLN(F(".. flushing a region of the FRAMs .."));
    DPRINTLN(F(""));
    DPRINT(F("start address of region : 0x"));
    DHEX(start_address);
    DPRINTLN(F(""));
    DPRINT(F("end address of region   : 0x"));
    DHEX(end_address);
    DPRINTLN(F(""));
    DPRINT(F("flushing with value     : 0x"));
    DHEX(value);
    DPRINTLN(F(""));
  #endif

  bool success = true;

  // check address range
  if (( end_address < start_address ) || ( start_address > MAX_FRAM_BANK_ADDRESS - 1 ) || ( end_address > MAX_FRAM_BANK_ADDRESS - 1 )){
    DPRINTLN(F(""));
    DPRINTLN(F("ERROR: wrong address range of region flush :"));
    DPRINTLN(F(""));
    DPRINT(F("start address of region : 0x"));
    DHEX(start_address);
    DPRINTLN(F(""));
    DPRINT(F("end address of region : 0x"));
    DHEX(end_address);
    DPRINTLN(F(""));
    success = false;
  }

  // fill buffer with value
  uint16_t buf[blockSize];
  for (uint16_t i = 0; i < blockSize; i++) buf[i] = value;

  while (success && (current_address < end_address)) {
    // Calculate the remaining bytes to erase
    uint32_t remainingBytes = end_address - current_address;

    // Determine the size of the block to erase
    size_t blockSizeToErase = min(blockSize*2, remainingBytes);

     #ifdef DEEP_DEBUG
      DPRINTLN(F(""));
      DPRINT(F("flushing block size : 0x"));
      DHEX(blockSizeToErase);
      DPRINTLN(F(""));
      DPRINT(F("current address     : 0x"));
      DHEX(current_address);
      DPRINTLN(F(""));
      DPRINT(F("remaining bytes     : 0x"));
      DHEX(remainingBytes);
    #endif

    success =  FRAM_write(current_address,  (uint8_t*)(&buf), blockSizeToErase);

    // Move to the next block
    current_address += (uint32_t)blockSizeToErase;

  }
  DPRINTLN(F(""));
  DPRINTLN(F("region of FRAM bank has been flushed"));
  DPRINTLN(F(""));
  #ifdef DEEP_DEBUG
    if (!success ){
      DPRINTLN(F("ERROR occured when flushing region of FRAMs!"));
    }
    else
    {
      DPRINTLN(F("region of FRAM bank was flushed successfully"));
    }
  #endif

  return success;
}





// requires extension of Adafruit's Adafruit_FRAM_SPI class, need to create a fork of Adafruit's repository
bool FRAM_sleep_mode() {

  bool success1, success2;

  // set low power mode of FRAMs
  #ifdef DEEP_DEBUG
    DPRINTLN(F(".. entering low power mode of FRAMs .."));
  #endif
  success1 = fram_1.enter_low_power_mode();
  success2 = fram_2.enter_low_power_mode();


  return success1 && success2;
}

// requires extension of Adafruit's Adafruit_FRAM_SPI class, need to create a fork of Adafruit's repository
bool FRAM_normal_mode() {
  bool success1, success2;

  // set low power mode of FRAMs
  #ifdef DEEP_DEBUG
    DPRINTLN(F(".. exiting low power mode of FRAMs .."));
  #endif

  success1 = fram_1.exit_low_power_mode();
  success2 = fram_2.exit_low_power_mode();

  return success1 && success2;
}


void initialize_FRAMs_chip_selects(void){

  bool status;

  pinMode(FRAM_CS1_PIN, OUTPUT);
  pinMode(FRAM_CS2_PIN, OUTPUT);
  digitalWrite(FRAM_CS1_PIN, LOW);
  digitalWrite(FRAM_CS2_PIN, LOW);
  delay(400);
  status = FRAM_writeEnable(true);
  if (!status) {
    DPRINTLN(F("ERROR: executing FRAM write enable failed"));
  }

}


// cold_boot shall be executed at a factory reset or if set by WebUI channel parameters

bool cold_boot(void) {
  bool status = false;

  DPRINTLN(F(""));
  DPRINTLN(F("executing now a cold start of FRAM bank .."));
  DPRINTLN(F(""));
  DPRINTLN(F("this will take quite a while .."));
  DPRINTLN(F(""));
  DPRINTLN(F(""));

#ifdef DEEP_DEBUG
  print_FRAM_sizes();
#endif

  initialize_FRAMs_chip_selects();


  status = init_FRAMs();

  if ( status ) {
    status = FRAM_writeEnable(true);status = FRAM_writeEnable(true);

    if ( status ) {
      status = FRAM_flush(0x00);
    }

    // set start_status_flag of FRAM bank
    DPRINTLN(F("Setting now the cold start flag of FRAM bank .."));
    if ( status ) {
      status = set_FRAM_start_status_flag(FRAMS_UNINITIALIZED);
    }

    if ( status ) {
      // save compilation date time string to FRAM @ address 0x00001
      status = save_compilation_date_time_string_to_FRAM(0x00001);
      if ( status ) {
        status = FRAM_writeEnable(false);
      }
    }
  }


  if (!status) {
    DPRINTLN(F("ERROR: init_FRAMs failed !"));
  }
  else {
    DPRINTLN(F("cold start of FRAM bank is done"));
    DPRINTLN(F(""));
  }

  return status;
}


// warm_boot shall be executed at a battery change or other reasons for a power supply interruption

bool warm_boot(void) {
  bool status = false;

  DPRINTLN(F(""));
  DPRINTLN(F("executing now a warm start of FRAM bank .."));
  DPRINTLN(F(""));
  DPRINTLN(F(""));

#ifdef DEEP_DEBUG
  print_FRAM_sizes();
#endif

  uint8_t start_status_flag;

  status = init_FRAMs();

  if ( status ) {
    start_status_flag = get_FRAM_start_status_flag();

    DPRINT(F("the start_status_flag of the FRAM bank is : "));
    DHEX(start_status_flag);
    DPRINTLN(F(""));

    if ( start_status_flag != FRAMS_INITIALIZED ) {
      DPRINTLN(F("ERROR: The FRAM bank is not yet initialized !"));
      DPRINTLN(F(""));
      status = false;
    }
    else
    {
      DPRINTLN(F("INFO: The FRAM bank is initialized !"));
      DPRINTLN(F(""));
    }
    // read compilation date time string from FRAM at address 0x00001
    status = read_compilation_date_time_string_from_FRAM(0x00001);

  }

  return status;
}

public:

FUJITSU_MB85RS2MTPF_FRAMs() {
  // default constructor
   DPRINTLN(F("executing default constructor of class FUJITSU_MB85RS2MTPF_FRAMs"));
}



// get first byte of FRAM bank which is indicating the cold_start status
uint8_t get_FRAM_start_status_flag(void) {
  uint8_t start_status_flag;

  DPRINTLN(F("reading start status flag of FRAM bank @ addr 0x00000 .."));

  // set start_status_flag of FRAM bank
  start_status_flag = FRAM_read8(0x00000);  // read first byte of FRAM bank which is indicating the cold_start status

  DPRINTLN(F(""));
  DPRINT(F("the start_status_flag of the FRAM bank is : "));
  DHEXLN(start_status_flag);
  DPRINTLN(F(""));

  return start_status_flag;
}

// set first byte of FRAM bank which is indicating the cold_start status
bool set_FRAM_start_status_flag(uint8_t FRAM_status_flag) {
  bool    status = false;
  uint8_t start_status_flag;

  DPRINT(F("setting start status flag of FRAM bank @ addr 0x00000 to : "));
  DHEXLN(FRAM_status_flag);

  status = FRAM_writeEnable(false);

  if(status) {
    // set start_status_flag of FRAM bank
    status = FRAM_write8(0x00000, FRAM_status_flag);  // write first byte of FRAM bank which is indicating the start status status
  }
  if(status) {
    // confirm start_status_flag of FRAM bank by a read back
    start_status_flag = FRAM_read8(0x00000);  // read back first byte of FRAM bank which is indicating the start status status
    DPRINT(F("the read back start_status_flag of the FRAM bank is : "));
    DHEX(start_status_flag);
    DPRINTLN(F(""));
  }
  return status;
}




// save compilation date time string to FRAM
bool save_compilation_date_time_string_to_FRAM(uint32_t address) {
  bool status = false;


  DPRINT(F("saving compilation date time string to FRAM bank @ address 0x"));
  DHEXLN(address);

  char compilation_date_time[21];

  strcpy(compilation_date_time, __DATE__ " " __TIME__);  // example of compilation_date_time string: 'May 20 2024 13:08:22'
  DPRINT(F("compilation was done at ")); DPRINTLN(compilation_date_time);

  status = FRAM_writeEnable(false);

  if(status) {
    // save compilation date time string to FRAM at address
    status = FRAM_write(address,  (uint8_t*)compilation_date_time, sizeof(compilation_date_time));  // save compilation date time string to FRAM at address
  }
  if (!status) {
    DPRINT(F("ERROR: saving compilation date time string to FRAM bank failed !"));
  }

  return status;
}

// read compilation date time string from FRAM
bool read_compilation_date_time_string_from_FRAM(uint32_t address) {
  bool status;
  char compilation_date_time[21]; // example of compilation_date_time string: 'May 20 2024 13:08:22'

  DPRINT(F("reading compilation date time string from FRAM bank @ address 0x"));
  DHEXLN(address);

  // read compilation date time string to FRAM at address 0x00001
  status = FRAM_read(address,  (uint8_t*)compilation_date_time, sizeof(compilation_date_time));  // read compilation date time string to FRAM at address

  if (!status) {
    DPRINT(F("ERROR: reading compilation date time string to FRAM bank failed !"));
  }
  else {
    DPRINT(F("FRAM compilation date string ")); DPRINTLN(compilation_date_time);
  }

  return status;
}

void print_FRAM_sizes(void){
    DPRINT(F(""));
    DPRINT(F("FRAM size is 0x"));
    DHEX(FUJITSU_MB85RS2MT_fram_size);
    DPRINTLN(F(" Bytes"));
    DPRINT(F("total FRAM bank size is 0x"));
    DHEX(MAX_FRAM_BANK_ADDRESS);
    DPRINTLN(F(" Bytes"));
    DPRINT(F(""));
    DPRINT(F("FRAM 1 address is 0x"));
    DHEXLN((uint32_t)&fram_1);
    DPRINT(F(""));
    DPRINT(F("FRAM 2 address is 0x"));
    DHEXLN((uint32_t)&fram_2);
    DPRINT(F("CS1 pin number is "));
    DDECLN(FRAM_CS1_PIN);
    DPRINT(F("CS2 pin number is "));
    DDECLN(FRAM_CS2_PIN);
    DPRINT(F("CS1 pin address is 0x"));
    DHEXLN((uint32_t)&FRAM_CS1_PIN);
    DPRINT(F(""));
}


};


  
}

#endif
