#ifndef _LCDSPICONFIG_H_
#define _LCDSPICONFIG_H_

#include <inttypes.h>



// MCP23S08 display connections
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
        adrs*     -> [|     |] <- 5
        adrs*     -> [|     |] <- 4
        +5V-(10k)--> [|     |] <- 3
        CS (10)   -> [|     |] <- 2
                     [|     |] <- 1
        gnd       -> [|_____|] <- 0
*/
#endif