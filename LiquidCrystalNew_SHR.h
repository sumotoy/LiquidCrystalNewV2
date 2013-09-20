#ifndef _LiquidCrystalNew_SHR_h
#define _LiquidCrystalNew_SHR_h

#include <inttypes.h>

//#include "___SPI_IO.h"// SPI hardware methods
#include "HD44780.h"
/*
	This variation of the LiquidCrystalNew allow use the GPIO SPI expander from Microchip
	MCP23S08 that uses HAEN to share the same CS pin (that in SPI should normally tied to one device only)
	with a max of 8 chip of the MCP23Sxx series.
	It has all the fetures of LiquidCrystalNew included 2 x HD44780 handling.
	If you are using a ready made piggyBack module that uses this chip but connections with LCD
	are different you can change pin configuration below (change to gpio_config_b.h and
	modify or just rename it and modify)
*/
#include "_configurations/gpio_config_74XX595.h"	// GPIO pin configuration, how the GPIO is connected to LCD		

class LiquidCrystalNew_SHR : public HD44780
{
public:
	LiquidCrystalNew_SHR(const byte clk,const byte data,const byte strobe,const byte chip=0);//use shift register
	virtual void 		begin(byte cols, byte rows, uint8_t charsize = LCD_5x8DOTS); 
	virtual void 		send(uint8_t value, byte mode);
	virtual void 		backlight(byte value);
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
	//SPI_IO   			_spiobj;											// the spi object from SPI_IO library
	#if defined(__FASTSWRITE2__)
	volatile uint8_t 	*clkport, *dtaport, *stbport;
	uint8_t 			clkpinmask, dtapinmask, stbpinmask;
	#endif
	void				_fastShiftOut(byte val);
};	

#endif