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

#ifndef LiquidCrystalNew_direct_h
#define LiquidCrystalNew_direct_h

#include <inttypes.h>

#include "HD44780.h"
#include "_utility/cpuident.h"				// identify the MCU used


class LiquidCrystalNew : public HD44780
{
public:
	LiquidCrystalNew(const byte rs,const byte en1,const byte en2,const byte d0,const byte d1,const byte d2,const byte d3,const byte bk);
	LiquidCrystalNew(const byte rs,const byte en1,const byte d0, const byte d1,const byte d2,const byte d3,const byte bk);
	virtual void 	begin(uint8_t cols, uint8_t rows, uint8_t charsize = LCD_5x8DOTS); 
	virtual void 	send(uint8_t value, byte mode);
	virtual void 	backlight(byte value);
	virtual void 	on(void);
	virtual void 	off(void);

private:
	byte 				_rs_pin;	// LOW: command.  HIGH: character.
	byte 				_data_pins[4];
	byte				_backlightPin;
	void 				init(const byte rs,const byte en1,const byte en2,const byte d0,const byte d1,const byte d2,const byte d3,const byte bk);
	void 				initChip(uint8_t dotsize, byte enPin);
	void 				write4bits(byte value);
	void 				setDataMode(byte mode);
	void 				pulseEnable(byte enPin);
	#if defined(__FASTSWRITE2__)
	volatile uint8_t 	d0port, d1port, d2port, d3port, rsport, en1port, en2port;
	uint8_t 		 	d0pin,  d1pin,  d2pin,  d3pin,  rspin,  en1pin,  en2pin;
	#endif
};

#endif