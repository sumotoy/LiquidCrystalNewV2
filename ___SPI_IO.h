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
Created by Max MC Costa for sumotoy,(sumotoy@gmail.com)
this minimal library holds all hardware related commands to drive fast
microchip MCP23S08 GPIO chip.
*/

class SPI_IO {
public:
	SPI_IO();
	void 				initialize(const byte adrs,const byte cs,byte avoidInit);
	void				writeByte(byte cmd,byte value);
private:
	byte				_cs;
	byte				_adrs;
#if defined(__FASTSWRITE2__)
	void inline			sendSPI(byte data){ SPDR = data; while(!(SPSR & _BV(SPIF))); };
	volatile uint8_t 	*csport;
	uint8_t 			cspinmask;
#endif
};
#endif