#ifndef _LCDCONFIG_H_
#define _LCDCONFIG_H_



#define BACKGND_LGHTINV	//depends of what you using for drive led background. In my case I use an p-channel mosfet called IRFD9120

static const unsigned int DELAYPERCHAR    	=	320;
static const unsigned int LCD_STARTUP_DLY 	=	7500;// max 65535
static const unsigned int LCD_HOME_DLY 	   =	2900;// max 65535
/*I don't think I can adequately test this number; it will depend a little on which Arduino or clone you have and probably
 could also vary with the power source applied to that board. The way to test is really to load your program, remove power
 and then reapply power so that the program starts up as power is applied. If anyone finds they need a larger number please
*/

// commands
static const byte LCD_CLEARDISPLAY 			=	0x01;
static const byte LCD_RETURNHOME			   =	0x02;
static const byte LCD_ENTRYMODESET			 =	0x04;
static const byte LCD_DISPLAYCONTROL		=	0x08;
static const byte LCD_CURSORSHIFT			  =	0x10;
static const byte LCD_FUNCTIONSET			  =	0x20;
static const byte LCD_SETCGRAMADDR			 =	0x40;
static const byte LCD_SETDDRAMADDR			 =	0x80;

// flags for display entry mode
static const byte LCD_ENTRYRIGHT 			       =	0x00;
static const byte LCD_ENTRYLEFT 			        =	0x02;
static const byte LCD_ENTRYSHIFTINCREMENT 	=	0x01;
static const byte LCD_ENTRYSHIFTDECREMENT 	=	0x00;

// flags for display on/off control
static const byte LCD_DISPLAYON 			 =	0x04;
static const byte LCD_DISPLAYOFF 			=	0x00;
static const byte LCD_CURSORON 				 =	0x02;
static const byte LCD_CURSOROFF 			 =	0x00;
static const byte LCD_BLINKON 				  =	0x01;
static const byte LCD_BLINKOFF 				 =	0x00;
static const byte LCD_CURSORS_MASK 	=	0xFC;

// flags for display/cursor shift
static const byte LCD_DISPLAYMOVE 			=	0x08;
static const byte LCD_CURSORMOVE 			 =	0x00;
static const byte LCD_MOVERIGHT 			  =	0x04;
static const byte LCD_MOVELEFT 				  =	0x00;

// flags for function set
static const byte LCD_4BITMODE 				=	0x00;
static const byte LCD_2LINE 				   =	0x08;
static const byte LCD_1LINE 				   =	0x00;
static const byte LCD_5x10DOTS 				=	0x04;
static const byte LCD_5x8DOTS 			 	=	0x00;
static const byte LCD_Right 				   =	0;
static const byte LCD_Left 					   =	1;

#endif
