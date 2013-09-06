#ifndef LiquidCrystal_hspi_h
#define LiquidCrystal_hspi_h

#include <inttypes.h>

#include "HD44780.h"
#include "utility/spi_config.h"

class LiquidCrystalHSPI : public HD44780
{
public:
	LiquidCrystalHSPI(const byte cs,const byte chip=0,const byte adrs=0x20);//use SPI
	virtual void 	begin(byte cols, byte rows, uint8_t charsize = LCD_5x8DOTS); 
	virtual void 	send(uint8_t value, byte mode);
	virtual void 	backlight(byte value);
private:
	byte			_cs;		//used only in SPI, the CS pin
	//byte 			_data_pins[4];
	byte			_adrs;				//SPI address or I2C address
	byte			_theData;			//8 bit of the GPIO chip or SR, not used in direct
	//byte 			_rs_pin;
	void 			init(const byte adrs,const byte cs,const byte en2);
	void 			initChip(uint8_t dotsize, byte enPin);
	void 			write4bits(byte value);
	void 			setDataMode(byte mode);
	void 			pulseEnable(byte enPin);
	void 			writeByte(byte cmd,byte value);
	void 			startSend();
	void 			endSend();
	void 			writeGpio(byte value);
};

#endif