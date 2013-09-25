#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#if (ARDUINO <  100)
	#include <WProgram.h>
#else
	#include <Arduino.h>
#endif

#include "LiquidCrystalNew.h"

#if defined(__FASTSWRITE2__)	
#include "_utility/DigitalIO/DigitalPin.h"
#endif

#ifndef _LCDGPIOPINCONFIG_H_
	#include "_configurations/pin_config_dummy.h"
	//#error you should include a configuration file!!!
#endif

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
	setChip(0);
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

	
void LiquidCrystalNew::begin(uint8_t cols, uint8_t lines, uint8_t dotsize) {
	
	#if defined(__FASTSWRITE2__)	
		fastPinMode(_data_pins[0], OUTPUT);
		fastPinMode(_data_pins[1], OUTPUT);
		fastPinMode(_data_pins[2], OUTPUT);
		fastPinMode(_data_pins[3], OUTPUT);
		fastPinMode(_data_pins[0], OUTPUT);
		fastPinMode(_en1, OUTPUT);
		fastPinMode(_rs_pin, OUTPUT);
		fastDigitalWrite(_en1, LOW);
	#else	
		byte i;
		for (i=0;i<4;i++){
			pinMode(_data_pins[i],OUTPUT);
		}
		pinMode(_rs_pin,OUTPUT);
		pinMode(_en1,OUTPUT);
		digitalWrite(_en1, LOW);
	#endif
	
	if (_multipleChip) {
		#if defined(__FASTSWRITE2__)
			fastPinMode(_en2, OUTPUT);	
			fastDigitalWrite(_en2, LOW);			
		#else
			pinMode(_en2,OUTPUT);  //4X40 LCD
			digitalWrite(_en2, LOW);
		#endif
	}
	
	if (_backlightPin != 255){
		#if defined(__FASTSWRITE2__)	
			fastPinMode(_backlightPin, OUTPUT);
			fastDigitalWrite(_backlightPin, _backLight);
		#else
			pinMode(_backlightPin,OUTPUT);
			digitalWrite(_backlightPin,_backLight);
		#endif
	}
	#if defined(__FASTSWRITE2__)
		fastDigitalWrite(_data_pins[0], LOW);
		fastDigitalWrite(_data_pins[1], LOW);
		fastDigitalWrite(_data_pins[2], LOW);
		fastDigitalWrite(_data_pins[3], LOW);
	#else
		for (i=0;i<4;i++){
			digitalWrite(_data_pins[i],LOW);
		}
	#endif
	_lcd_cols = cols;    //there is an implied lack of trust; the private version can't be munged up by the user.
	_lcd_lines = lines;
	_row_offsets[2] = _lcd_cols + _row_offsets[0];  //should autoadjust for 16/20 or whatever columns now
	_row_offsets[3] = _lcd_cols + _row_offsets[1];
	initChip(dotsize,_en1);
	//manage second chip if exists
	if (_multipleChip) {
		_row_offsets[2] = 0;
		_row_offsets[3] = 0x40; //each line gets its own little 40 char section of DDRAM--would be fine if there were a 4x32, I suppose
		setChip(2);
		initChip(dotsize,_en2);//initialize the second HD44780 chip
	}
}

void LiquidCrystalNew::initChip(uint8_t dotsize, byte enPin) {  
	byte	displayfunction = LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS;
	byte i;
	if (_lcd_lines > 1) displayfunction |= LCD_2LINE;
	// for some 1 line displays you can select a 10 pixel high font
	if ((dotsize != 0) && (_lcd_lines == 1)) displayfunction |= LCD_5x10DOTS;

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
	noAutoscroll();
}

// write either command or data, with automatic 4/8-bit selection
void LiquidCrystalNew::send(uint8_t value, byte mode) {
	byte en = _en1;
	byte testChip = getChip();
	if (_multipleChip && testChip) en = _en2;
	//delayMicroseconds(DELAYPERCHAR);
	//nop;
	setDataMode(mode);	
	#if defined(__FASTSWRITE2__)
		fastDigitalWrite(_data_pins[0],value & 0x10);
		fastDigitalWrite(_data_pins[1],value & 0x20);
		fastDigitalWrite(_data_pins[2],value & 0x40);
		fastDigitalWrite(_data_pins[3],value & 0x80);
		
		pulseEnable(en);
		
		fastDigitalWrite(_data_pins[0],value & 0x01);
		fastDigitalWrite(_data_pins[1],value & 0x02);
		fastDigitalWrite(_data_pins[2],value & 0x04);
		fastDigitalWrite(_data_pins[3],value & 0x08);
	#elif defined(__FASTSWRITE__)	
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
    byte testChip = getChip();
	byte en = _en1;
 // 4x40 LCD with 2 controller chips with separate enable lines if we called w 2 enable pins and are on lines 2 or 3 enable chip 2  
	if (_multipleChip && testChip) en = _en2; 
	#if defined(__FASTSWRITE2__)
		fastDigitalWrite(*pinptr++,v & 01);
		fastDigitalWrite(*pinptr++,(v >>= 1) & 01);
		fastDigitalWrite(*pinptr++,(v >>= 1) & 01);
		fastDigitalWrite(*pinptr++,(v >>= 1) & 01);
	#elif defined(__FASTSWRITE__)	
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
	#if defined(__FASTSWRITE2__)
		fastDigitalWrite(_rs_pin,mode);
	#elif defined (__FASTSWRITE__)
		digitalWriteFast(_rs_pin,mode);
	#else
		digitalWrite(_rs_pin,mode);
	#endif
}

void LiquidCrystalNew::pulseEnable(byte enPin) {
	#if defined(__FASTSWRITE2__)
		fastDigitalWrite(enPin,HIGH);   // enable pulse must be >450ns
		nop;nop;nop;nop;nop;nop;nop;nop; //>450ns in theory?!?!
		fastDigitalWrite(enPin,LOW);
	#elif defined (__FASTSWRITE__)
		digitalWriteFast(enPin,HIGH);   // enable pulse must be >450ns
		nop;nop;nop;nop;nop;nop;nop;nop; //>450ns in theory?!?!
		digitalWriteFast(enPin,LOW);
	#else
		digitalWrite(enPin,HIGH);   // enable pulse must be >450ns
		digitalWrite(enPin,LOW);
	#endif
	HD44780DLY_OUT();
}

void LiquidCrystalNew::on(void) {
	display();
	backlight(1);
}

void LiquidCrystalNew::off(void) {
	noDisplay();
	backlight(0);
}

void LiquidCrystalNew::backlight(byte val){
#ifdef BACKGND_LGHTINV
	_backLight = !val;
#else
	_backLight = val;
#endif
	if (_backlightPin != 255){
	#if defined(__FASTSWRITE2__)
		fastDigitalWrite(_backlightPin,_backLight);  
	#elif defined(__FASTSWRITE__)
		digitalWriteFast(_backlightPin,_backLight);   
	#else
		digitalWrite(_backlightPin,_backLight);   
	#endif
	}
}