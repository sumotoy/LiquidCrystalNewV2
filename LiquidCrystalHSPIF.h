#ifndef LiquidCrystal_hspif_h
#define LiquidCrystal_hspif_h

#include <inttypes.h>

#include "HD44780.h"
#include "utility/spi_config.h"
#include <DigitalIO.h>

/** Nop for timing. */
#define nop asm volatile ("nop\n\t")
const uint8_t SPI_MODE = 0;
//------------------------------------------------------------------------------
	const byte			_xMosiPin = 11;
	const byte			_xSckPin =  13;
	const byte			_xCsPin  =  10;

class LiquidCrystalHSPIF : public HD44780
{
public:
	LiquidCrystalHSPIF(const byte cs=10,const byte chip=0,const byte adrs=0x20);//use software SPI
	virtual void 	begin(byte cols, byte rows, uint8_t charsize = LCD_5x8DOTS); 
	virtual void 	send(uint8_t value, byte mode);
	virtual void 	backlight(byte value);
private:

	byte				_cs;		//used only in SPI, the CS pin
	byte				_adrs;				//SPI address or I2C address
	byte				_theData;			//8 bit of the GPIO chip or SR, not used in direct
	void 				init(const byte adrs,const byte cs,const byte en2);
	void 				initChip(uint8_t dotsize, byte enPin);
	void 				write4bits(byte value);
	void 				setDataMode(byte mode);
	void 				pulseEnable(byte enPin);
	void 				writeByte(byte cmd,byte value);
	void 				startSend();
	void 				endSend();
	void 				writeGpio(byte value);
	
    volatile uint8_t 	*csport; 
	uint8_t 			cspinmask; 
  //----------------------------------------------------------------------------
  inline __attribute__((always_inline))
  bool MODE_CPHA(uint8_t mode) {return (mode & 1) != 0;}
  
  inline __attribute__((always_inline))
  bool MODE_CPOL(uint8_t mode) {return (mode & 2) != 0;}
  
  //----------------------------------------------------------------------------
  inline __attribute__((always_inline))
  void transferBit(uint8_t bit, uint8_t* rxData, uint8_t txData) {
    if (MODE_CPHA(SPI_MODE)) fastDigitalWrite(_xSckPin, !MODE_CPOL(SPI_MODE));
    fastDigitalWrite(_xMosiPin, txData & (1 << bit));
    fastDigitalWrite(_xSckPin,MODE_CPHA(SPI_MODE) ? MODE_CPOL(SPI_MODE) : !MODE_CPOL(SPI_MODE));
    if (!MODE_CPHA(SPI_MODE)) fastDigitalWrite(_xSckPin, MODE_CPOL(SPI_MODE));
  }

  inline __attribute__((always_inline))
  uint8_t spiTransfer(uint8_t txData) {
    uint8_t rxData = 0;
    transferBit(7, &rxData, txData);
    transferBit(6, &rxData, txData);
    transferBit(5, &rxData, txData);
    transferBit(4, &rxData, txData);
    transferBit(3, &rxData, txData);
    transferBit(2, &rxData, txData);
    transferBit(1, &rxData, txData);
    transferBit(0, &rxData, txData);
    return rxData;
  }  


};

#endif