#ifndef _SPI_IO_H_
#define _SPI_IO_H_

#if (ARDUINO <  100)
	#include <WProgram.h>
#else
	#include <Arduino.h>
#endif

#include <inttypes.h>
#include <_utility/cpuident.h>		// identify the MCU used
/*
this minimal library holds all hardware related commands to drive
microchip MCP23sxxx GPIO series
*/

class SPI_IO {
public:
	SPI_IO();
	void 				initialize(const byte adrs,const byte cs);
	void				writeByte(byte cmd,byte value);
private:
	
	byte				_cs;
	byte				_adrs;
#if defined(__FASTSWRITE2__)
	volatile uint8_t 	*csport;
	uint8_t 			cspinmask;
#endif
};
#endif