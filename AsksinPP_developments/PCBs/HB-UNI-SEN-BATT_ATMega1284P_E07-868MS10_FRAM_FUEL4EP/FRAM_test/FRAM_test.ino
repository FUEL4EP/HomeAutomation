#include <Adafruit_FRAM_SPI.h>
#include <AskSinPP.h>
#include "Fujitsu_MB85RS2MTPF_FRAM_circular_buffer.h"


// simple tests of FRAM class FUJITSU_MB85RS2MTPF_FRAMs

// all library classes are placed in the namespace 'as'
using namespace as;

FUJITSU_MB85RS2MTPF_FRAMs frams;

void setup() {
  DINIT(38400, ASKSIN_PLUS_PLUS_IDENTIFIER);
  frams.init_FRAMs();

  uint8_t buf;
  buf = frams.FRAM_read8(0x40000);
  DPRINTLN(F(" "));
  DPRINT(F("single byte read data is "));
  DHEX(buf);
  DPRINTLN(F(" "));

  uint16_t buf16;
  buf16 = frams.FRAM_read16(0x00000);
  DPRINTLN(F(" "));
  DPRINTLN(F("two bytes read data is "));
  DHEX(buf16);
  DPRINTLN(F(" "));

  buf16 = frams.FRAM_read16(0x40000);
  DPRINTLN(F("two bytes read data is "));
  DHEX(buf16);
  DPRINTLN(F(" "));

  uint8_t  block[128];
  size_t   no_of_bytes_read = 64;
  uint32_t start_address = 0x9FFFE;
  bool success;
  
  success = frams.FRAM_read(start_address, block, no_of_bytes_read);

  if (success) {
     DPRINTLN(F(" "));
     DPRINTLN(F("block read data is "));
     frams.dump_array (start_address, block, no_of_bytes_read);
  }
  else
  {
     DPRINTLN(F("an error occured when reading a block data from FRAM "));
     frams.dump_array (start_address, block, no_of_bytes_read);
  }


  no_of_bytes_read = 32;
  start_address    = 0x9FFFD;
  
  success = frams.FRAM_read(start_address, block, no_of_bytes_read);

  if (success) {
     DPRINTLN(F(" "));
     DPRINTLN(F("block read data is "));
     frams.dump_array (start_address, block, no_of_bytes_read);
  }
  else
  {
     DPRINTLN(F("an error occured when reading a block data from FRAM "));
     frams.dump_array (start_address, block, no_of_bytes_read);
  }
  
  // write a byte
  uint32_t write_addr = 0x0000D;
  uint8_t  write_value = 0x77;
  success = frams.FRAM_write8(write_addr, write_value);
  if (!success) {
    DPRINTLN(F("write error "));
  }

  // write two bytes
  write_addr = 0x0000A;
  uint16_t  write16_value = 0xABCD;
  success = frams.FRAM_write16(write_addr, write16_value);
  if (!success) {
    DPRINTLN(F("write16 error "));
  }

  // write two bytes
  write_addr = 0x3FFFF;
  write16_value = 0x1234;
  success = frams.FRAM_write16(write_addr, write16_value);
  if (!success) {
    DPRINTLN(F("write16 error "));
  }


  //write five bytes block

  uint8_t write_array[5];
  write_array[0]=0x57;
  write_array[1]=0x67;
  write_array[2]=0x78;
  write_array[3]=0x89;
  write_array[4]=0x98;

  write_addr = 0x3FFFE;  // crossing border
  success = frams.FRAM_write(write_addr, write_array, 5);
  if (!success) {
    DPRINTLN(F("block write error "));
  }

  
  no_of_bytes_read = 32;
  start_address    = 0x03FFF0;
  
  success = frams.FRAM_read(start_address, block, no_of_bytes_read);

  if (success) {
     DPRINTLN(F(" "));
     DPRINTLN(F("block read data is "));
     frams.dump_array (start_address, block, no_of_bytes_read);
  }
  else
  {
     DPRINTLN(F("an error occured when reading a block data from FRAM "));
     frams.dump_array (start_address, block, no_of_bytes_read);
  }

  //write five bytes block

  write_array[0]=0x55;
  write_array[1]=0x67;
  write_array[2]=0x78;
  write_array[3]=0x89;
  write_array[4]=0x90;

  write_addr = 0x00011;
  success = frams.FRAM_write(write_addr, write_array, 5);
  if (!success) {
    DPRINTLN(F("block write error "));
  }

  
  no_of_bytes_read = 32;
  start_address    = 0x000000;
  
  success = frams.FRAM_read(start_address, block, no_of_bytes_read);

  if (success) {
     DPRINTLN(F(" "));
     DPRINTLN(F("block read data is "));
     frams.dump_array (start_address, block, no_of_bytes_read);
  }
  else
  {
     DPRINTLN(F("an error occured when reading a block data from FRAM "));
     frams.dump_array (start_address, block, no_of_bytes_read);
  }

  no_of_bytes_read = 32;
  start_address    = 0x003FFF0;

  success = frams.FRAM_read(start_address, block, no_of_bytes_read);

  if (success) {
     DPRINTLN(F(" "));
     DPRINTLN(F("block read data is "));
     frams.dump_array (start_address, block, no_of_bytes_read);
  }
  else
  {
     DPRINTLN(F("an error occured when reading a block data from FRAM "));
     frams.dump_array (start_address, block, no_of_bytes_read);
  }

 

  success = frams.FRAM_flush(0x00);
  if (!success) {
    DPRINTLN(F("flushing error "));
  }

 
  no_of_bytes_read = 128;
  start_address    = 0x000000;

  success = frams.FRAM_read(start_address, block, no_of_bytes_read);

  if (success) {
     DPRINTLN(F(" "));
     DPRINTLN(F("block read data is "));
     frams.dump_array (start_address, block, no_of_bytes_read);
  }
  else
  {
     DPRINTLN(F("an error occured when reading a block data from FRAM "));
     frams.dump_array (start_address, block, no_of_bytes_read);
  }


  no_of_bytes_read = 128;
  start_address    = 0x040000;

  success = frams.FRAM_read(start_address, block, no_of_bytes_read);

  if (success) {
     DPRINTLN(F(" "));
     DPRINTLN(F("block read data is "));
     frams.dump_array (start_address, block, no_of_bytes_read);
  }
  else
  {
     DPRINTLN(F("an error occured when reading a block data from FRAM "));
     frams.dump_array (start_address, block, no_of_bytes_read);
  }


 
  no_of_bytes_read = 128;
  start_address    = 0x03FF80;

  success = frams.FRAM_read(start_address, block, no_of_bytes_read);

  if (success) {
     DPRINTLN(F(" "));
     DPRINTLN(F("block read data is "));
     frams.dump_array (start_address, block, no_of_bytes_read);
  }
  else
  {
     DPRINTLN(F("an error occured when reading a block data from FRAM "));
     frams.dump_array (start_address, block, no_of_bytes_read);
  }


  no_of_bytes_read = 128;
  start_address    = 0x07FF80;

  success = frams.FRAM_read(start_address, block, no_of_bytes_read);

  if (success) {
     DPRINTLN(F(" "));
     DPRINTLN(F("block read data is "));
     frams.dump_array (start_address, block, no_of_bytes_read);
  }
  else
  {
     DPRINTLN(F("an error occured when reading a block data from FRAM "));
     frams.dump_array (start_address, block, no_of_bytes_read);
  }

}


void loop(void) {

}
