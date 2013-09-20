#include <stdio.h>
#include <string.h>
#include <inttypes.h>

#if (ARDUINO <  100)
	#include <WProgram.h>
#else
	#include <Arduino.h>
#endif

#include <../SPI/SPI.h>
#include "___SPI_IO.h"


SPI_IO::SPI_IO(){

}


/*
MCP23S017 initialization
begin SPI, set HAEN if necessary, set as out, zeroing all ports
*/
void SPI_IO::initialize(const byte adrs,const byte cs){
	_adrs = adrs;
	_cs = cs;
// ---- start SPI initializations HERE
	SPI.begin();
#if defined(__TEENSY3X__)
/*
SPI_CLOCK_DIV2    //24.0 MHz ----> ???
SPI_CLOCK_DIV4    //12.0 MHz ----> ok
SPI_CLOCK_DIV6    //08.0 MHz ----> ok
SPI_CLOCK_DIV8    //05.3 MHz ----> ok
SPI_CLOCK_DIV16   //03.0 MHz ----> ok
SPI_CLOCK_DIV32   //01.5 MHz ----> ok
SPI_CLOCK_DIV64   //750 KHz
SPI_CLOCK_DIV128  //375 Khz
*/
    SPI.setClockDivider(SPI_CLOCK_DIV4); // 4 MHz (half speed)
    SPI.setBitOrder(MSBFIRST);
    SPI.setDataMode(SPI_MODE0);
#elif defined(__ARDUEX__)//dunnoyet!
    SPI.setClockDivider(SPI_CLOCK_DIV4); // 4 MHz (half speed)
    SPI.setBitOrder(MSBFIRST);
    SPI.setDataMode(SPI_MODE0);
#else
    SPI.setClockDivider(SPI_CLOCK_DIV4); // 4 MHz (half speed)
    SPI.setBitOrder(MSBFIRST);
    SPI.setDataMode(SPI_MODE0);
#endif
	pinMode(_cs, OUTPUT); //set data pin modes
#if defined(__FASTSWRITE2__)
	csport      = portOutputRegister(digitalPinToPort(_cs));
	cspinmask   = digitalPinToBitMask(_cs);
	*csport |= cspinmask;//cs HI
#elif defined(__FASTSWRITE__)
	digitalWriteFast(_cs, HIGH);
#else
	digitalWrite(_cs, HIGH);
#endif
	delay(100);
// ---- now prepare GPIO chip and initialize it
	if (_adrs > 0 && _adrs < 255){
		writeByte(0x05,0b00101000);//HAEN -> ON (IOCON)
	} else {
		writeByte(0x05,0b00100000);//use dedicated CS pin (not shared)
		_adrs = 0x20;//default chip adrs when not using HAEN
	}
	writeByte(0x00,0x00);//first, set all GPIO's pin as outs (IODIR)
	writeByte(0x09,0b00000000);//finally, set all GPIO's out as LOW
 }


void SPI_IO::writeByte(byte cmd,byte value){
//start send
#if defined(__FASTSWRITE2__)
	*csport &= ~cspinmask;//cs LOW
#elif defined(__FASTSWRITE__)
	digitalWriteFast(_cs, LOW);
#else
	digitalWrite(_cs, LOW);
#endif

	SPI.transfer(_adrs << 1);//in write, in read: SPI.transfer((addr << 1) | 1);
	
	//this 2 byte blocks instruct the chip
	SPI.transfer(cmd);
	SPI.transfer(value);

	// now closing communication...
#if defined(__FASTSWRITE2__)
	*csport |= cspinmask;//cs HI
#elif defined(__FASTSWRITE__)
	digitalWriteFast(_cs, HIGH);
#else
	digitalWrite(_cs, HIGH);
#endif
}