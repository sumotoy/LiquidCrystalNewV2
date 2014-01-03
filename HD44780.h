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


#ifndef _HD44780_H_
#define _HD44780_H_

#if (ARDUINO <  100)
	#include <WProgram.h>
#else
	#include <Arduino.h>
#endif

#include <inttypes.h>
#include <Print.h>

#include "_utility/cpuident.h"				// identify the MCU used
#include "_configurations/HD44780_constants.h"		// lcd controller constants

//set timing things
#define nop asm volatile ("nop\n\t")

#if defined(__AVR__)
	#define DelayNanoseconds(__ns) __builtin_avr_delay_cycles( (double)(F_CPU)*((double)__ns)/1.0e9 + 0.5 ) 
#else
	#define DelayNanoseconds(void) {nop;nop;nop;nop;nop;nop;nop;nop;} 
#endif


class HD44780 : public Print 
{
public:
	HD44780( );

#if (ARDUINO <  100)
	virtual void 	begin(uint8_t cols, uint8_t rows, uint8_t charsize = LCD_5x8DOTS) { };
#else
	virtual void 	begin(uint8_t cols, uint8_t rows, uint8_t charsize = LCD_5x8DOTS) = 0;
#endif

	void 			clear();								//
	void 			home();									//
	void 			noDisplay();							//
	void 			display();								//
	void 			noBlink();								//
	void 			blink();								//
	void 			noCursor();								//
	void 			cursor();								//
	void 			scrollDisplayLeft();					//
	void 			scrollDisplayRight();					//
	void 			leftToRight();							//
	void 			rightToLeft();							//
	void 			autoscroll();							//
	void 			noAutoscroll();							//
	uint8_t			getCursorCol(void);						// NEW
	uint8_t			getCursorRow(void);						// NEW
	void 			setCursor(uint8_t col, uint8_t row);	//
	void 			createChar(uint8_t, uint8_t[]);			//
	void 			vfdBrightness(uint8_t val);				// NEW 2
	void			tuneLcdDelay(int val);			// added. change the default delay for slower displays, add microseconds to the configured pulse delay
	// in you see garbage on screen, try modify this. If 0=reset to default, otherwise it will add (or subcract if negative) to the default delay
#if (ARDUINO <  100)
   virtual void 	write(uint8_t value);
   virtual void 	backlight(byte value) { };				// NEW
   virtual void 	on() { };								// NEW
   virtual void 	off() { };								// NEW
#else
   virtual size_t 	write(uint8_t value);
   virtual void 	backlight(byte value) = 0;				// NEW		
   virtual void 	on(void) = 0;							// NEW
   virtual void 	off(void) = 0;							// NEW
#endif
	using Print::write;
	
protected:

	byte 			_en1; 				
	byte 			_en2; 				
	byte 			_chip;				// witch chip actually active?
	byte			_multipleChip;		// 0:one chip / 1:2xchip
	int8_t 			_scroll_count;
	int8_t			_x;
	int8_t			_y;
	byte 			_lcd_cols;			// LCD width
	byte 			_lcd_lines;			// LCD height
	byte 			_setCursFlag;
	byte 			_direction;
	byte 			_row_offsets[4];
	byte 			_displaycontrol;   	// display on/off, cursor on/off, blink on/off
	byte 			_displaymode;      	// text direction	
	byte			_backLight;			// 0:off/1:ON
	byte			_scrollOn;			// 0:off/1:ON
	
	inline void 	command(byte value)  {send(value, LOW);}
	inline void 	commandBoth(byte value)  {if (!_multipleChip) {command(value);}else{byte chipSave = getChip();setChip(0);command(value);setChip(2);command(value);setChip(chipSave);}}
	inline void		setChip(byte chip){ _chip = chip; }
	inline byte		getChip(){ return _chip; }
	uint8_t			_activeDelay;//ADDED 2014

	//again timing things...
#if defined(__FASTSWRITE__)
	static const uint8_t _defaultDelay = 27;
	inline void		HD44780DLY_OUT() { delayMicroseconds(_activeDelay+_customDelay); }
#elif defined(__FASTSWRITE2__)
	static const uint8_t _defaultDelay = 5;
	inline void		HD44780DLY_OUT() { delayMicroseconds(_activeDelay); }
#else
	#if defined(__TEENSY3X__)
		const static uint8_t _defaultDelay = 27;
		inline void		HD44780DLY_OUT() { delayMicroseconds(_activeDelay); }
	#else
		static const uint8_t _defaultDelay = 1;
		inline void		HD44780DLY_OUT() { if (_activeDelay == 0){nop;} else {delayMicroseconds(_activeDelay);} }
	#endif
#endif
	
private:
#if (ARDUINO <  100)
	virtual void 	send(byte value, byte mode) { };
#else
	virtual void 	send(byte value, byte mode) = 0;
#endif
	inline void		delayForHome() {delayMicroseconds(LCD_HOME_DLY);}
};
#endif