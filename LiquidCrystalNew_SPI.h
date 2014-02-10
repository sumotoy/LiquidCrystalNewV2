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


#ifndef _LiquidCrystalNew_SPI_h
#define _LiquidCrystalNew_SPI_h

#include <inttypes.h>

#include "HD44780.h"


class LiquidCrystalNew_SPI : public HD44780
{
public:

	LiquidCrystalNew_SPI(const byte cs,const byte chip=0,const byte adrs=0x20,const byte avoidSPIinit=0);//use software SPI
	// classical LCD begin
	virtual void 		begin(uint8_t cols, uint8_t rows, uint8_t charsize = LCD_5x8DOTS); 
	virtual void 		backlight(byte value);
	//following it's used by library and should never used in sketch.
	virtual void 		send(byte value, byte mode);
	virtual void 		on(void);
	virtual void 		off(void);
	
private:
	byte				_cs;												//used only in SPI, the CS pin
	byte				_adrs;												//SPI address or I2C address
	byte				_theData;											//8 bit of the GPIO chip or SR, not used in direct
	byte				_avoidInit;
	void 				initChip(uint8_t dotsize, byte witchEnablePin);
	void 				write4bits(byte value);
	void				writeByte(byte cmd,byte value);
	void 				_setDataMode(byte mode);
	void 				pulseEnable(byte witchEnablePin);					//
	void 				writeGpio(byte value);								//
	#if defined(__FASTSWRITE2__)
 	void inline			sendSPI(byte data){ BLOCK_IRQS(); SPDR = data; while(!(SPSR & _BV(SPIF))); BLOCK_IRQS(); };
	
	volatile uint8_t 	csport;
	uint8_t 			cspin;
	#endif
};	

#endif