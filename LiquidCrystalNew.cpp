#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#if (ARDUINO <  100)
	#include <WProgram.h>
#else
	#include <Arduino.h>
#endif


#include "LiquidCrystalNew.h"

//2 chip
LiquidCrystalNew::LiquidCrystalNew(const byte rs,const byte en1,const byte en2,const byte d0,const byte d1,const byte d2,const byte d3,const byte bk){
	init(rs,en1,en2,d0,d1,d2,d3,bk);
}

//1 chip
LiquidCrystalNew::LiquidCrystalNew(const byte rs,const byte en1,const byte d0,const byte d1,const byte d2,const byte d3,const byte bk){
	init(rs,en1,255,d0,d1,d2,d3,bk);
}



void LiquidCrystalNew::init(const byte rs,const byte en1,const byte en2,const byte d0,const byte d1,const byte d2,const byte d3,const byte bk){
	//pins
	_data_pins[0] = d0;
	_data_pins[1] = d1;
	_data_pins[2] = d2;
	_data_pins[3] = d3; 
	_en1 = en1;
	_en2 = en2;
	_rs_pin = rs;
	if (bk != d0 && bk != d1 && bk != d2 && bk != d3 && bk != en1 && bk != rs && bk != 255) {
		_backlightPin = bk;
	} else {
		_backlightPin = 255;
	}
	_scroll_count = 0;      //to fix the bug if we scroll and then setCursor w/o home() or clear()
	_x = 0;
	_y = 0;
	_setCursFlag = 0;
	_direction = LCD_Right;
	_chip = 0;
	if (_en2 != 255) {
		_multipleChip = 1;
	} else {
		_multipleChip = 0;
	}
#ifdef BACKGND_LGHTINV
	_backLight = 0;
#else
	_backLight = 1;
#endif
	//settings the offsets
	_row_offsets[0] = 00;   // DDRAM addresses inside the HD44780 are strange: 0-nColumns-1 on line 0
	_row_offsets[1] = 0x40; // 64-(63+nColumns) for line 1
	_row_offsets[2] = 0x14; // 20- (19+nColumns) for line 2 --- NOTHING FROM 40-63 !
	_row_offsets[3] = 0x54; // 84 - (83+nColumns) for line 3  -- so 80 characters tops out at #103 !
}

void LiquidCrystalNew::begin(byte cols, byte lines, uint8_t dotsize) {
	uint8_t i;
	for (i=0;i<4;i++){
		pinMode(_data_pins[i],OUTPUT);
	}
	pinMode(_rs_pin,OUTPUT);
	pinMode(_en1,OUTPUT);
	digitalWrite(_en1, LOW);
	if (_multipleChip) {
		pinMode(_en2,OUTPUT);  //4X40 LCD
		digitalWrite(_en2, LOW);
	}
	if (_backlightPin != 255){
		pinMode(_backlightPin,OUTPUT);
		digitalWrite(_backlightPin,_backLight);
	}
	for (i=0;i<4;i++){
		digitalWrite(_data_pins[i],LOW);
	}
	_numcols = cols;    //there is an implied lack of trust; the private version can't be munged up by the user.
	_numlines = lines;
	_row_offsets[2] = cols + _row_offsets[0];  //should autoadjust for 16/20 or whatever columns now
	_row_offsets[3] = cols + _row_offsets[1];
	initChip(dotsize,_en1);
	//manage second chip if exists
	if (_multipleChip) {
		_row_offsets[2] = 0;
		_row_offsets[3] = 0x40; //each line gets its own little 40 char section of DDRAM--would be fine if there were a 4x32, I suppose
		_chip = 2;
		initChip(dotsize,_en2);//initialize the second HD44780 chip
	}
}

void LiquidCrystalNew::initChip(uint8_t dotsize, byte enPin) {  
	byte	displayfunction = LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS;
	byte i;
	if (_numlines > 1) displayfunction |= LCD_2LINE;
	// for some 1 line displays you can select a 10 pixel high font
	if ((dotsize != 0) && (_numlines == 1)) displayfunction |= LCD_5x10DOTS;

	for (i=0;i<18;i++) {
		delayMicroseconds(LCD_STARTUP_DLY);
	}
	// Now we pull both RS and R/W low to begin commands
	setDataMode(0);//COMMAND MODE
	write4bits(0x03);
	delayMicroseconds(5000); // I have one LCD for which 4500 here was not long enough.
	// second try
	write4bits(0x03);      
	delayMicroseconds(150); // wait 
	// third go!
	write4bits(0x03); 
	delayMicroseconds(150);
	// finally, set to 4-bit interface
	write4bits(0x02); 
	delayMicroseconds(150);
	// finally, set # lines, font size, etc.
	command(LCD_FUNCTIONSET | displayfunction);  
	// turn the display on with no cursor or blinking default
	_displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;  
	display();
	// clear it off
	clear();
	// Initialize to default text direction (for romance languages)
	_displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
	// set the entry mode
	command(LCD_ENTRYMODESET | _displaymode);	
}

// write either command or data, with automatic 4/8-bit selection
void LiquidCrystalNew::send(uint8_t value, byte mode) {
	byte en = _en1;
	if (_multipleChip && _chip) en = _en2;
	delayMicroseconds(DELAYPERCHAR);
	setDataMode(mode);					
	#if defined(__FASTSPI)		
		digitalWriteFast(_data_pins[0],value & 0x10);
		digitalWriteFast(_data_pins[1],value & 0x20);
		digitalWriteFast(_data_pins[2],value & 0x40);
		digitalWriteFast(_data_pins[3],value & 0x80);
		pulseEnable(en);
		
		digitalWriteFast(_data_pins[0],value & 0x01);
		digitalWriteFast(_data_pins[1],value & 0x02);
		digitalWriteFast(_data_pins[2],value & 0x04);
		digitalWriteFast(_data_pins[3],value & 0x08);
	#else
		digitalWrite(_data_pins[0],value & 0x10);
		digitalWrite(_data_pins[1],value & 0x20);
		digitalWrite(_data_pins[2],value & 0x40);
		digitalWrite(_data_pins[3],value & 0x80);

		pulseEnable(en);
		digitalWrite(_data_pins[0],value & 0x01);
		digitalWrite(_data_pins[1],value & 0x02);
		digitalWrite(_data_pins[2],value & 0x04);
		digitalWrite(_data_pins[3],value & 0x08);
	#endif
	pulseEnable(en);
	}

void LiquidCrystalNew::write4bits(byte value) {  //still used during init
	register byte v = value;
	byte *pinptr = _data_pins;
	byte en = _en1;
 // 4x40 LCD with 2 controller chips with separate enable lines if we called w 2 enable pins and are on lines 2 or 3 enable chip 2  
	if (_multipleChip && _chip) en = _en2;   
#if defined(__FASTSPI)
		digitalWriteFast(*pinptr++,v & 01);
		digitalWriteFast(*pinptr++,(v >>= 1) & 01);
		digitalWriteFast(*pinptr++,(v >>= 1) & 01);
		digitalWriteFast(*pinptr++,(v >>= 1) & 01);
#else
		digitalWrite(*pinptr++,v & 01);
		digitalWrite(*pinptr++,(v >>= 1) & 01);
		digitalWrite(*pinptr++,(v >>= 1) & 01);
		digitalWrite(*pinptr++,(v >>= 1) & 01);
#endif
	pulseEnable(en);
} 


//Set data mode, want send data or command?  0:COMMAND -- 1:DATA
void LiquidCrystalNew::setDataMode(byte mode) {
#if defined(__FASTSPI)
	digitalWriteFast(_rs_pin,mode);
#else
	digitalWrite(_rs_pin,mode);
#endif
}

void LiquidCrystalNew::pulseEnable(byte enPin) {
#if defined(__FASTSPI)
	digitalWriteFast(enPin,HIGH);   // enable pulse must be >450ns
	digitalWriteFast(enPin,LOW);
#else
	digitalWrite(enPin,HIGH);   // enable pulse must be >450ns
	digitalWrite(enPin,LOW);
#endif
}



void LiquidCrystalNew::backlight(byte val){
#ifdef BACKGND_LGHTINV
	_backLight = !val;
#else
	_backLight = val;
#endif
	if (_backlightPin != 255){
#if defined(__FASTSPI)
		digitalWriteFast(_backlightPin,_backLight);   // enable pulse must be >450ns
#else
		digitalWrite(_backlightPin,_backLight);   // enable pulse must be >450ns
#endif
	}
}