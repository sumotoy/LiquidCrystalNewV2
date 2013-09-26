#ifndef LiquidCrystalNew_direct_h
#define LiquidCrystalNew_direct_h

#include <inttypes.h>

#include "HD44780.h"
#include "_utility/cpuident.h"				// identify the MCU used



/*
Created by Max MC Costa for sumotoy,(sumotoy@gmail.com)
Fast DigitalIO by William Greiman
	This is the direct connect methods exact as the canonical liquidLibrary from arduino.
	Main differences is that it can run only in 4bit mode but can drive 2 x HD44780 LCD's
	and uses faster digitalWrite routine so it's faster! It also have some fix that allow
	correct handle of big screens and better cursor management.
*/

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