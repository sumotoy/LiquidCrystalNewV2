#ifndef LiquidCrystalNew_direct_h
#define LiquidCrystalNew_direct_h

#include <inttypes.h>

#include "HD44780.h"


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