#ifndef _TWI_IO_H_
#define _TWI_IO_H_

#if (ARDUINO <  100)
	#include <WProgram.h>
#else
	#include <Arduino.h>
#endif

#include <inttypes.h>
#include <_utility/cpuident.h>			// identify the MCU used
/*
Created by Max MC Costa for sumotoy,(sumotoy@gmail.com)
this minimal library holds all hardware related commands to drive
microchip MCP23sxxx GPIO series
*/

class TWI_IO {
public:
	TWI_IO();
	void 				initialize(const byte adrs,const byte chipType);
	void				writeByte(byte cmd,byte value);
private:
	
	byte				_adrs;
	//byte				_chipType;
};
#endif