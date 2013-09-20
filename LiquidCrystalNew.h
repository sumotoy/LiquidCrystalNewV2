#ifndef LiquidCrystalNew_direct_h
#define LiquidCrystalNew_direct_h

#include <inttypes.h>

#include "HD44780.h"
#include "_utility/cpuident.h"				// identify the MCU used
// #include "pins_arduino.h"
// #include "wiring_private.h"

/*
	This is the direct connect methods exact as the canonical liquidLibrary from arduino.
	Main differences is that it can run only in 4bit mode but can drive 2 x HD44780 LCD's
	and uses faster digitalWrite routine so it's faster! It also have some fix that allow
	correct handle of big screens and better cursor management.
*/
//#define nop asm volatile ("nop\n\t")
//#define NANOD asm volatile ("nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t")
//#define NANOD __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t"); //500nS

class LiquidCrystalNew : public HD44780
{
public:
	LiquidCrystalNew(const byte rs,const byte en1,const byte en2,const byte d0,const byte d1,const byte d2,const byte d3,const byte bk);
	LiquidCrystalNew(const byte rs,const byte en1,const byte d0,const byte d1,const byte d2,const byte d3,const byte bk);
	virtual void 	begin(byte cols, byte rows, uint8_t charsize = LCD_5x8DOTS); 
	virtual void 	send(uint8_t value, byte mode);
	virtual void 	backlight(byte value);
private:
	byte 			_rs_pin;	// LOW: command.  HIGH: character.
	byte 			_data_pins[4];
	byte			_backlightPin;
	void 			init(const byte rs,const byte en1,const byte en2,const byte d0,const byte d1,const byte d2,const byte d3,const byte bk);
	void 			initChip(uint8_t dotsize, byte enPin);
	void 			write4bits(byte value);
	void 			setDataMode(byte mode);
	void 			pulseEnable(byte enPin);

};

#endif