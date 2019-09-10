/************************************************************************
* Adapted from: https://github.com/sparkfun/SparkFun_VCNL4040_Arduino_Library/blob/master/src/SparkFun_VCNL4040_Arduino_Library.cpp
************************************************************************/
#include <atmel_start.h>
#include <i2c_simple_master.h>
#include "proximity.h"

const uint8_t VCNL4040_ADDR = 0x60; //7-bit unshifted I2C address of VCNL4040

//Used to select between upper and lower byte of command register
#define LOWER true
#define UPPER false

//VCNL4040 Command Codes
#define VCNL4040_ALS_CONF 0x00
#define VCNL4040_ALS_THDH 0x01
#define VCNL4040_ALS_THDL 0x02
#define VCNL4040_PS_CONF1 0x03 //Lower
#define VCNL4040_PS_CONF2 0x03 //Upper
#define VCNL4040_PS_CONF3 0x04 //Lower
#define VCNL4040_PS_MS 0x04 //Upper
#define VCNL4040_PS_CANC 0x05
#define VCNL4040_PS_THDL 0x06
#define VCNL4040_PS_THDH 0x07
#define VCNL4040_PS_DATA 0x08
#define VCNL4040_ALS_DATA 0x09
#define VCNL4040_WHITE_DATA 0x0A
#define VCNL4040_INT_FLAG 0x0B //Upper
#define VCNL4040_ID 0x0C

static void __bit_mask(uint8_t reg, bool command_height,  uint8_t mask, uint8_t value);

void proximity_init() {
	uint16_t conf1_conf2 = 0x0000;
	uint16_t conf3_psms = 0x0000;
	I2C_0_write2ByteRegister(VCNL4040_ADDR, VCNL4040_PS_CONF1, conf1_conf2);
	I2C_0_write2ByteRegister(VCNL4040_ADDR, VCNL4040_PS_CONF3, conf3_psms);
	
}

//Read the Proximity value
uint16_t proximity_get()
{
	return I2C_0_read2ByteRegister(VCNL4040_ADDR, VCNL4040_PS_DATA);
}
//
//static uint8_t __read_command_lower(uint8_t cmd) {
	//return (I2C_0_read2ByteRegister(VCNL4040_ADDR, cmd) & 0xFF);
//}
//static uint8_t __read_command_upper(uint8_t cmd) {
	//return (I2C_0_read2ByteRegister(VCNL4040_ADDR, cmd) >> 8);
//}
//
//void __write_command_lower(uint8_t cmd, uint8_t new_value)
//{
	//uint16_t cmd_value =I2C_0_read2ByteRegister(VCNL4040_ADDR, cmd);
	//cmd_value &= 0xFF00; //Remove lower 8 bits
	//cmd_value |= (uint16_t)new_value; //Mask in
	//I2C_0_write2ByteRegister(VCNL4040_ADDR, cmd, cmd_value);
//}
//void __write_command_upper(uint8_t cmd, uint8_t new_value)
//{
	//uint16_t cmd_value =I2C_0_read2ByteRegister(VCNL4040_ADDR, cmd);
	//cmd_value &= 0x00FF; //Remove upper 8 bits
	//cmd_value |= (uint16_t)new_value << 8; //Mask in
	//I2C_0_write2ByteRegister(VCNL4040_ADDR, cmd, cmd_value);
//}
//
//static void __bit_mask(uint8_t cmd, bool command_height,  uint8_t mask, uint8_t value) {
	  //// Grab current register context
	  //uint8_t registerContents;
	  //if (command_height == LOWER) registerContents = __read_command_lower(cmd);
	  //else registerContents = __read_command_upper(cmd);
//
	  //// Zero-out the portions of the register we're interested in
	  //registerContents &= mask;
//
	  //// Mask in new thing
	  //registerContents |= value;
//
	  //// Change contents
	  //if (command_height == LOWER) __write_command_lower(cmd, registerContents);
	  //else __write_command_upper(cmd, registerContents);
//}