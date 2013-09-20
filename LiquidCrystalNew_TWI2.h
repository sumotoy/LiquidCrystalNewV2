#ifndef _LiquidCrystalNew_TWI2_h
#define _LiquidCrystalNew_TWI2_h

#include <inttypes.h>

#include "___PCF8574_IO.h"					// hardware methods for TWI
#include "HD44780.h"
/*
	This variation of the LiquidCrystalNew allow use the GPIO I2C expander from Microchip
	PCF8574.
	If you want to use the MCP23S08 GPIO from NTX please use the alternative version of this library
	called LiquidCrystalNew_TWI.
	It has all the fetures of LiquidCrystalNew included 2 x HD44780 handling.
	If you are using a ready made piggyBack module that uses this chip but connections with LCD
	are different you can change pin configuration below (change to gpio_config_pcfb.h and/or
	modify it)
*/

#include "_configurations/gpio_config_PCF8574.h"		// GPIO pin configuration, how the GPIO is connected to LCD		

class LiquidCrystalNew_TWI2 : public HD44780
{
public:
	LiquidCrystalNew_TWI2(const byte adrs=0x20,const byte chip=0);//use twi
	virtual void 		begin(byte cols, byte rows, uint8_t charsize = LCD_5x8DOTS); 
	virtual void 		send(uint8_t value, byte mode);
	virtual void 		backlight(byte value);
private:
	byte				_adrs;				//I2C address
	byte				_theData;			//8 bit of the GPIO chip or SR, not used in direct

	void 				initChip(uint8_t dotsize, byte witchEnablePin);
	void 				write4bits(byte value);
	void 				setDataMode(byte mode);
	void 				pulseEnable(byte witchEnablePin);
	void 				writeGpio(byte value);
	PCF8574_IO   		_twiobj;
};

#endif