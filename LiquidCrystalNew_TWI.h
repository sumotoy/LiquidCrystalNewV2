#ifndef _LiquidCrystalNew_TWI_h
#define _LiquidCrystalNew_TWI_h

#include <inttypes.h>

#include "___TWI_IO.h"					// hardware methods for TWI
#include "HD44780.h"
/*
	This variation of the LiquidCrystalNew allow use the GPIO I2C expander from Microchip
	MCP23008.
	If you want to use the PCF8574 GPIO from NTX please use the alternative version of this library
	called LiquidCrystalNew_TWI2.
	It has all the fetures of LiquidCrystalNew included 2 x HD44780 handling.
	If you are using a ready made piggyBack module that uses this chip but connections with LCD
	are different you can change pin configuration below (change to gpio_config_b.h and
	modify or just rename it and modify)
	Library can directly used with this commercial product:
		http://www.ladyada.net/products/i2cspilcdbackpack/index.html
*/

#include "_configurations/gpio_config_MCP23X08.h"		// GPIO pin configuration, how the GPIO is connected to LCD		

typedef enum chip_type{
    MCP23008 	= 0,
	PCF8574 	= 1,
    PCF8574A 	= 3
};

class LiquidCrystalNew_TWI : public HD44780
{
public:
	LiquidCrystalNew_TWI(const byte adrs=0x20,const byte chip=0,const byte chip_type=MCP23008);//use twi
	virtual void 		begin(byte cols, byte rows, uint8_t charsize = LCD_5x8DOTS); 
	virtual void 		send(uint8_t value, byte mode);
	virtual void 		backlight(byte value);
private:
	byte				_adrs;				//I2C address
	byte				_theData;			//8 bit of the GPIO chip or SR, not used in direct
	byte				_chipType;
	void 				initChip(uint8_t dotsize, byte witchEnablePin);
	void 				write4bits(byte value);
	void 				setDataMode(byte mode);
	void 				pulseEnable(byte witchEnablePin);
	void 				writeGpio(byte value);
	TWI_IO   			_twiobj;
};

#endif