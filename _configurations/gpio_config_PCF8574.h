#ifndef _LCDGPIOCONFIGPCFA_H_
#define _LCDGPIOCONFIGPCFA_H_

#include <inttypes.h>



// LCD PINS                          PCF8574x - comment
static const byte LCDPIN_EN 		=	2;// EN pin
static const byte LCDPIN_EN2		=	0;// EN2 (unused if 1 chip LCD's(most popular)
static const byte LCDPIN_RS			=	1;// RS pin
static const byte LCDPIN_D4			=	3;// D4 data
static const byte LCDPIN_D5			=	4;// D5 data
static const byte LCDPIN_D6			=	5;// D6 data
static const byte LCDPIN_D7			=	6;// D7 data
static const byte LCDPIN_LD			=	7;// Background Led (use a transistor or mosfet!!! see readme)
// LCDPIN_RW ---> connect to ground!

/*
PCF8574 & PCF8574A reference
         adrs* A0 -> [|--U--|] <+ +5v
         adrs* A1 -> [|     |] <= sda
         adrs* A2 -> [|     |] <= scl
                0 -> [|     |] 
                1 -> [|     |] <- 7
                2 -> [|     |] <- 6
                3 -> [|     |] <- 5
              gnd -> [|_____|] <- 4

*Addressing-------------------------------------
Both chip SHOULD be addressed! Use a 10K or 4K7 resistor for 1 and GND for 0.
PCF8574 base address is: 00100A2A1A0 - R/W (R=0,W=1) - tied on GND A1 & A0 result in 0x20
PCF8574A base address is: 00111A2A1A0 - R/W (R=0,W=1) - tied on GND A2,A1,A0 result in 0x38
*/
#endif