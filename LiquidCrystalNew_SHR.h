#ifndef _LiquidCrystalNew_SHR_h
#define _LiquidCrystalNew_SHR_h

#include <inttypes.h>

//#include "___SPI_IO.h"// SPI hardware methods
#include "HD44780.h"
/*
Created by Max MC Costa for sumotoy,(sumotoy@gmail.com)
	This variation of the LiquidCrystalNew allow use a shift register chip (any 74XX595) as GPIO to save pins.
	It has all the fetures of LiquidCrystalNew included 2 x HD44780 handling.
	If you are using a ready made piggyBack module that uses this chip but connections with LCD
	are different you can include in the sketch BEFORE includes your personalized configuration file (take a look inside
	_configurations folder)
*/
//#include "_configurations/gpio_config_74XX595.h"	// GPIO pin configuration, how the GPIO is connected to LCD		

class LiquidCrystalNew_SHR : public HD44780
{
public:
	LiquidCrystalNew_SHR(const byte clk,const byte data,const byte strobe,const byte chip=0);//use shift register
	virtual void 		begin(uint8_t cols, uint8_t rows, uint8_t charsize = LCD_5x8DOTS); 
	virtual void 		send(uint8_t value, byte mode);
	virtual void 		backlight(byte value);
	virtual void 		on(void);
	virtual void 		off(void);
	
private:
	byte				_clk;
	byte				_dta;
	byte				_stb;												
	byte				_theData;											//8 bit of the GPIO chip or SR, not used in direct
	void 				initChip(uint8_t dotsize, byte witchEnablePin);
	void 				write4bits(byte value);
	void 				setDataMode(byte mode);
	void 				pulseEnable(byte witchEnablePin);					//
	void 				writeGpio(byte value);								//
	void 				writeByte(byte value);
	#if defined(__FASTSWRITE2__)
	volatile uint8_t stbport, dtaport, sclkport;
	uint8_t stbpin, dtapin, sclkpin;
	#endif
};	

#endif