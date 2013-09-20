#ifndef _LCDGPIOCONFIGA_H_
#define _LCDGPIOCONFIGA_H_

#include <inttypes.h>

/*
I2C ic to lcd pinmap are compatible with
( http://www.ladyada.net/products/i2cspilcdbackpack/index.html )
*/

// MCP23x08 display connections
static const byte LCDPIN_EN 		=	2;// enable pin
static const byte LCDPIN_EN2		=	0;// Not used if you have 1 chip LCD's (most ones), leave unconnected
static const byte LCDPIN_RS			=	1;// rs pin
static const byte LCDPIN_D4			=	3;// data
static const byte LCDPIN_D5			=	4;// data
static const byte LCDPIN_D6			=	5;// data
static const byte LCDPIN_D7			=	6;// data
static const byte LCDPIN_LD			=	7;// Background Led (use a transistor or mosfet!!! see notes)
/*
MCP23S08 reference
        sck (13)  -> [|--U--|] <- +5v
                     [|     |] <- 7
        miso (11) -> [|     |] <- 6
        adrs* A1  -> [|     |] <- 5
        adrs* A0  -> [|     |] <- 4
        +5V-(10k)--> [|     |] <- 3
        CS (10)   -> [|     |] <- 2
                     [|     |] <- 1
        gnd       -> [|_____|] <- 0
		
MCP23008 reference
              scl -> [|--U--|] <- +5v
              sda -> [|     |] <- 7
         adrs* A2 -> [|     |] <- 6
         adrs* A1 -> [|     |] <- 5
         adrs* A0 -> [|     |] <- 4
        +5V-(10k)--> [|     |] <- 3
                     [|     |] <- 2
                     [|     |] <- 1
        gnd       -> [|_____|] <- 0	

*Addressing-------------------------------------
Both chip SHOULD be addressed! Use a 10K or 4K7 resistor for 1 and GND for 0.
MCP23S08 base address is: 001000A1A0 - R/W (R=0,W=1) - tied on GND A1 & A0 result in 0x20
MCP23008 base address is: 00100A2A1A0 - R/W (R=0,W=1) - tied on GND A2,A1,A0 result in 0x20
*/
#endif