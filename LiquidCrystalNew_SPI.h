#ifndef _LiquidCrystalNew_SPI_h
#define _LiquidCrystalNew_SPI_h

#include <inttypes.h>


#include "HD44780.h"
/*
Created by Max MC Costa for sumotoy,(sumotoy@gmail.com)
Fast DigitalIO by William Greiman
	This variation of the LiquidCrystalNew allow use the GPIO SPI expander from Microchip
	MCP23S08 that uses HAEN to share the same CS pin (that in SPI should normally tied to one device only)
	with a max of 8 chip of the MCP23Sxx series.
	It has all the fetures of LiquidCrystalNew included 2 x HD44780 handling.
	If you are using a ready made piggyBack module that uses this chip but connections with LCD
	are different you can include in the sketch BEFORE includes your personalized configuration file (take a look inside
	_configurations folder)
	Example:
	for use with this product 	http://www.ladyada.net/products/i2cspilcdbackpack/index.html include at the top of your sketch:
	#include "../LiquidCrystalNew/_configurations/pin_config_ladyada.h"
	This will override the default config file.
*/
	

class LiquidCrystalNew_SPI : public HD44780
{
public:
/*
	LiquidCrystalNew_SPI instantiate
	(parameters)
	cs: dedicated cs pin if HAEN off or shared if ON
	chip: 0=1xHD44780(most lcd's),1=2xHD44780(large 4x40 lcd's)
	adrs: HAEN address,if not using set to 0 or 255
	avoidSPIinit: if true you need to init manually SPI in sketch before begin()! useful if you have complicated sketches
*/
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
 	void inline			sendSPI(byte data){ SPDR = data; while(!(SPSR & _BV(SPIF))); };
	
	volatile uint8_t 	csport;
	uint8_t 			cspin;
	#endif
};	

#endif