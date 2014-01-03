/*
	LiquidCrystalNew - An attemp to create a fast and universal library for drive HD44780 LCD's
	
	Features:
	- Can use a GPIO chip (I2C,SR or SPI) to save pins on your microcontroller.
	- Can drive double HD44780 displays (large ones).
	- It's faster than canonical one.
	- Better management of the cursor.
	
	Library works only in 4bit mode but can drive 2 x HD44780 LCD's
	and uses faster digitalWrite routine. It also have some fix that allow
	correct handle of big screens and better cursor management.

    Copyright (c) 2013, sumotoy, coded by Max MC Costa.    

    LiquidCrystalNew Library is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    LiquidCrystalNew Library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Foobar.  If not, see <http://www.gnu.org/licenses/>.

    This file contains high level functions based on ;

    Fast DigitalIO by William Greiman:
 
    liquicrystal440 by John Rain:
    http://code.google.com/p/liquidcrystal440/
	
	+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	Version:0.99b2
	+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/

#ifndef _LiquidCrystalNew_TWI_h
#define _LiquidCrystalNew_TWI_h

#include <inttypes.h>

#include "HD44780.h"
/*
	Library can directly used with this commercial product:
		http://www.ladyada.net/products/i2cspilcdbackpack/index.html
*/

//#include "_configurations/gpio_config_MCP23X08.h"		// GPIO pin configuration, how the GPIO is connected to LCD		

#define MCP23008 0
#define PCF8574  1
#define PCF8574A 2


class LiquidCrystalNew_TWI : public HD44780
{
public:
	LiquidCrystalNew_TWI(const byte adrs=0x20,const byte chip = 0,const byte chip_type = MCP23008);//use twi
	virtual void 		begin(uint8_t cols, uint8_t rows, uint8_t charsize = LCD_5x8DOTS); 
	virtual void 		send(uint8_t value, byte mode);
	virtual void 		backlight(byte value);
	virtual void 		on(void);
	virtual void 		off(void);
private:
	byte				_adrs;				//I2C address
	byte				_theData;			//8 bit of the GPIO chip or SR, not used in direct
	byte				_chipType;
	void 				initChip(uint8_t dotsize, byte witchEnablePin);
	void 				write4bits(byte value);
	void 				setDataMode(byte mode);
	void 				pulseEnable(byte witchEnablePin);
	void 				writeGpio(byte value);
	void				writeByte(byte cmd,byte value);
};

#endif