#ifndef LiquidCrystal_sspi_h
#define LiquidCrystal_sspi_h

#include <inttypes.h>

#include "HD44780.h"
#include "utility/spi_config.h"

class LiquidCrystalSSPI : public HD44780
{
public:
	LiquidCrystalSSPI(const byte mosi,const byte sck,const byte cs,const byte chip=0,const byte adrs=0x20);//use software SPI
	virtual void 	begin(byte cols, byte rows, uint8_t charsize = LCD_5x8DOTS); 
	virtual void 	send(uint8_t value, byte mode);
	virtual void 	backlight(byte value);
private:
	byte				_cs;		//used only in SPI, the CS pin
	byte				_mosi;		//used only in SPI, the CS pin
	byte				_sck;		//used only in SPI, the CS pin
	byte				_adrs;				//SPI address or I2C address
	byte				_theData;			//8 bit of the GPIO chip or SR, not used in direct
	void 				init(const byte adrs,const byte mosi,const byte sck,const byte cs,const byte en2);
	void 				initChip(uint8_t dotsize, byte enPin);
	void 				write4bits(byte value);
	void 				setDataMode(byte mode);
	void 				pulseEnable(byte enPin);
	void 				writeByte(byte cmd,byte value);
	void 				startSend();
	void 				endSend();
	void 				writeGpio(byte value);
	
	//void 				altSPIwrite(uint8_t data);
	inline void 		altSPIwrite(uint8_t data){for(uint8_t bit=0x80;bit;bit>>=1){*clkport&=~clkpinmask;if(data&bit){*mosiport|=mosipinmask;}else{*mosiport&=~mosipinmask;}*clkport|=clkpinmask;}};
    volatile uint8_t 	*mosiport, *clkport, *csport; 
	uint8_t 			mosipinmask, clkpinmask, cspinmask;
};

#endif