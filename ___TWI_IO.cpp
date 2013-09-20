#include <stdio.h>
#include <string.h>
#include <inttypes.h>

#if (ARDUINO <  100)
	#include <WProgram.h>
#else
	#include <Arduino.h>
#endif

#include <../Wire/Wire.h>
#include "___TWI_IO.h"


TWI_IO::TWI_IO(){
}

void TWI_IO::initialize(const byte adrs){
	_adrs = adrs;

	Wire.begin();
	
	TWBR = 12;
	delay(100);
/* 	if (_adrs != 0){
		writeByte(0x05,0b00101000);//HAEN on (IOCON)
	} else {
		writeByte(0x05,0b00100000);//use dedicated cs
	} */
	writeByte(0x05,0b00100000);//use dedicated cs
	writeByte(0x00,0x00);//set as out (IODIR)
	writeByte(0x09,0b00000000);//write all low to GPIO
}



void TWI_IO::writeByte(byte cmd,byte value){
	Wire.beginTransmission(_adrs);
#if ARDUINO >= 100
	Wire.write(cmd);
	Wire.write(value);
#else
	Wire.send(cmd);
	Wire.send(value);
#endif
	Wire.endTransmission();
}