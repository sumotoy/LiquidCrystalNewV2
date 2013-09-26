#ifndef _LiquidCrystalNew_T3TWI_h
#define _LiquidCrystalNew_T3TWI_h

#include <inttypes.h>

#include "HD44780.h"
/*
	Higly experimental TWI library ONLY for Teensy3. Using with other processor will use the regular Wire.
	It has all the fetures of LiquidCrystalNew included 2 x HD44780 handling.
	If you are using a ready made piggyBack module that uses this chip but connections with LCD
	are different you can include in the sketch BEFORE includes your personalized configuration file (take a look inside
	_configurations folder)
	Library can directly used with this commercial product:
		http://www.ladyada.net/products/i2cspilcdbackpack/index.html
*/

//#include "_configurations/gpio_config_MCP23X08.h"		// GPIO pin configuration, how the GPIO is connected to LCD		

#define MCP23008 0
#define PCF8574  1
#define PCF8574A 2

#define TWI_PINS_18_19 0
#define TWI_PINS_16_17 1

#define TWI_PULLUP_EXT 0
#define TWI_PULLUP_INT 1

#define TWI_RATE_100 0
#define TWI_RATE_200 1
#define TWI_RATE_300 2
#define TWI_RATE_400 3
#define TWI_RATE_600 4
#define TWI_RATE_800 5
#define TWI_RATE_1000 6
#define TWI_RATE_1200 7
#define TWI_RATE_1500 8
#define TWI_RATE_2000 9
#define TWI_RATE_2400 10

				 
class LiquidCrystalNew_T3TWI : public HD44780
{
public:
	LiquidCrystalNew_T3TWI(const byte adrs=0x20,byte pins=0,byte pullup=0,byte rate=3,const byte chip=0,const byte chip_type=MCP23008);//use twi
	virtual void 		begin(uint8_t cols, uint8_t rows, uint8_t charsize = LCD_5x8DOTS); 
	virtual void 		send(uint8_t value, byte mode);
	virtual void 		backlight(byte value);
	virtual void 		on(void);
	virtual void 		off(void);
private:
	byte				_adrs;				//I2C address
	byte				_theData;			//8 bit of the GPIO chip or SR, not used in direct
	byte				_chipType;
	void 				initChip(uint8_t dotsize, byte witchEnablePin);
	void 				write4bits(byte value);
	void 				setDataMode(byte mode);
	void 				pulseEnable(byte witchEnablePin);
	void 				writeGpio(byte value);
	void				writeByte(byte cmd,byte value);
	uint8_t				_pins;
	uint8_t				_pullup;
	uint8_t				_rate;
	
};

#endif