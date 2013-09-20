#ifndef _LCDGPIOCONFIGPSHRA_H_
#define _LCDGPIOCONFIGPSHRA_H_

#include <inttypes.h>



// 595 shift register to display connections
static const byte LCDPIN_EN 		=	2;// enable pin
static const byte LCDPIN_EN2		=	0;// Not used if you have 1 chip LCD's (most ones), leave unconnected
static const byte LCDPIN_RS			=	1;// rs pin
static const byte LCDPIN_D4			=	3;// data
static const byte LCDPIN_D5			=	4;// data
static const byte LCDPIN_D6			=	5;// data
static const byte LCDPIN_D7			=	6;// data
static const byte LCDPIN_LD			=	7;// Background Led (use a transistor or mosfet!!! see notes)
/*
74hc585 reference
			    1 -> [|--U--|] <= +5v
                2 -> [|     |] <- 0
                3 -> [|     |] <- dta_pin
				4 -> [|     |] <= gnd 
				5 -> [|     |] <- strobe_pin
				6 -> [|     |] <- clk_pin
				7 -> [|     |] <= +5v
             gnd  => [|_____|] nc
               
*/
#endif