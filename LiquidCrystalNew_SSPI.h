#ifndef _LiquidCrystalNew_SSPI_h
#define _LiquidCrystalNew_SSPI_h

#include <inttypes.h>

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
#include "_configurations/gpio_config_MCP23X08.h"	// GPIO pin configuration, how the GPIO is connected to LCD		

class LiquidCrystalNew_SSPI : public HD44780
{
public:
	LiquidCrystalNew_SSPI(const byte mosiPin,const byte clockPin,const byte csPin,const byte chip=0,const byte adrs=0x20);//use software SPI
	virtual void 		begin(byte cols, byte rows, uint8_t charsize = LCD_5x8DOTS); 
	virtual void 		send(uint8_t value, byte mode);
	virtual void 		backlight(byte value);
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
    volatile uint8_t *mosiport, *clkport, *csport;
    uint8_t mosipinmask, clkpinmask, cspinmask;
	#endif
};	

#endif