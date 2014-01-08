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
	Version:0.99b7
	+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/

#ifndef _LiquidCrystalNew_SSPI_h
#define _LiquidCrystalNew_SSPI_h

#include <inttypes.h>

#include "HD44780.h"

//#include "_configurations/gpio_config_MCP23X08.h"	// GPIO pin configuration, how the GPIO is connected to LCD		

class LiquidCrystalNew_SSPI : public HD44780
{
public:
	LiquidCrystalNew_SSPI(const byte mosiPin,const byte clockPin,const byte csPin,const byte chip=0,const byte adrs=0x20);//use software SPI
	virtual void 		begin(uint8_t cols, uint8_t rows, uint8_t charsize = LCD_5x8DOTS); 
	virtual void 		send(uint8_t value, byte mode);
	virtual void 		backlight(byte value);
	virtual void 		on(void);
	virtual void 		off(void);
	
private:
	byte				_mosi;
	byte				_clk;
	byte				_cs;												
	byte				_adrs;												//SPI address or I2C address
	byte				_theData;											//8 bit of the GPIO chip or SR, not used in direct
	void 				initChip(uint8_t dotsize, byte witchEnablePin);
	void 				write4bits(byte value);
	void 				setDataMode(byte mode);
	void 				pulseEnable(byte witchEnablePin);					//
	void 				writeGpio(byte value);								//
	void				writeByte(byte cmd,byte value);
	inline void 		altSPIwrite(uint8_t c);
	#if defined(__FASTSWRITE2__)
	volatile uint8_t csport, mosiport, sclkport;
	uint8_t cspin, mosipin, sclkpin;
	#endif
};	

#endif