#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#if (ARDUINO <  100)
	#include <WProgram.h>
#else
	#include <Arduino.h>
#endif
#include <SPI.h>

#include "LiquidCrystalHSPI.h"

//1/2 chip with SPI GPIO (3 wire)
LiquidCrystalHSPI::LiquidCrystalHSPI(const byte cs,const byte chip,const byte adrs){
	byte e2;
	if (chip == 0){
		e2 = 255;
	} else {
		e2 = (1 << LCDPIN_EN2);
	}
	init(adrs,cs,e2);
}

void LiquidCrystalHSPI::init(const byte adrs,const byte cs,const byte en2){
	//pins
	_cs = cs;
	_en1 = (1 << LCDPIN_EN);
	_en2 = en2;
	_adrs = adrs;
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
	
	_theData = 0b00000000;//  main data of the GPIO DATA PORT, better start with all 0,s
}

void LiquidCrystalHSPI::begin(byte cols, byte lines, uint8_t dotsize) {
	SPI.begin();
#if defined(__MK20DX128__)
	SPI_CLOCK_DIV4;
#elif defined(__arm__)//dunnoyet!
	SPI_CLOCK_DIV4;
#endif
	pinMode(_cs, OUTPUT); //set data pin modes
#if defined(__FASTSPI)
	digitalWriteFast(_cs, HIGH);
#else
	digitalWrite(_cs, HIGH);
#endif
	delay(100);
	if (_adrs != 0){
		writeByte(0x05,0b00101000);//HAEN on (IOCON)
	} else {
		writeByte(0x05,0b00100000);//use dedicated cs
	}
	writeByte(0x00,0x00);//set as out (IODIR)
	_theData = 0b00000000;
	writeByte(0x09,_theData);//write all low to GPIO

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

void LiquidCrystalHSPI::initChip(uint8_t dotsize, byte enPin) {  
	byte	displayfunction = LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS;
	byte i;
	if (_numlines > 1) displayfunction |= LCD_2LINE;
	// for some 1 line displays you can select a 10 pixel high font
	if ((dotsize != 0) && (_numlines == 1)) displayfunction |= LCD_5x10DOTS;

	for (i=0;i<18;i++) {
		delayMicroseconds(LCD_STARTUP_DLY);
	}

	// Now we pull both RS and R/W low to begin commands
	//digitalWrite(_rs_pin, LOW);
	setDataMode(0);//COMMAND MODE
	//digitalWrite(enPin, LOW);
	writeGpio(_theData & ~enPin);  // En LOW----------------------------------

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
	//writeGpio(_theData | enPin);   // En HIGH---------------------------------
}

// write either command or data, with automatic 4/8-bit selection
void LiquidCrystalHSPI::send(uint8_t value, byte mode) {
	byte en = _en1;
	if (_multipleChip && _chip) en = _en2;
	//delayMicroseconds(DELAYPERCHAR);
	setDataMode(mode);					// I2C & SPI
		bitWrite(_theData,LCDPIN_D4,value & 0x10);
		bitWrite(_theData,LCDPIN_D5,value & 0x20);
		bitWrite(_theData,LCDPIN_D6,value & 0x40);
		bitWrite(_theData,LCDPIN_D7,value & 0x80);
		pulseEnable(en);
		bitWrite(_theData,LCDPIN_D4,value & 0x01);
		bitWrite(_theData,LCDPIN_D5,value & 0x02);
		bitWrite(_theData,LCDPIN_D6,value & 0x04);
		bitWrite(_theData,LCDPIN_D7,value & 0x08);

		bitWrite(_theData,LCDPIN_LD,_backLight);//Background led
	pulseEnable(en);
}

void LiquidCrystalHSPI::write4bits(byte value) {  //still used during init
	register byte v = value;
	//byte *pinptr = _data_pins;
	byte en = _en1;
 // 4x40 LCD with 2 controller chips with separate enable lines if we called w 2 enable pins and are on lines 2 or 3 enable chip 2  
	if (_multipleChip && _chip) en = _en2;   
		bitWrite(_theData,LCDPIN_D4,v & 01);
		bitWrite(_theData,LCDPIN_D5,(v >>= 1) & 01);
		bitWrite(_theData,LCDPIN_D6,(v >>= 1) & 01);
		bitWrite(_theData,LCDPIN_D7,(v >>= 1) & 01);
	pulseEnable(en);
} 


//Set data mode, want send data or command?  0:COMMAND -- 1:DATA
void LiquidCrystalHSPI::setDataMode(byte mode) {
	bitWrite(_theData,LCDPIN_RS,mode);
}

void LiquidCrystalHSPI::pulseEnable(byte enPin) {
	writeGpio(_theData | enPin);   // En HIGH
	writeGpio(_theData & ~enPin);  // En LOW
}


void LiquidCrystalHSPI::writeByte(byte cmd,byte value){
  startSend();
	SPI.transfer(cmd);
	SPI.transfer(value);
  endSend();
}

//----------------------GPIO, start
void LiquidCrystalHSPI::startSend(){
#if defined(__FASTSWRITE__)
	digitalWriteFast(_cs, LOW);
#else
	digitalWrite(_cs, LOW);
#endif
	SPI.transfer(_adrs << 1);//in write, in read: SPI.transfer((addr << 1) | 1);
}
//--------------------GPIO, end
void LiquidCrystalHSPI::endSend(){
#if defined(__FASTSWRITE__)
	digitalWriteFast(_cs, HIGH);
#else
	digitalWrite(_cs, HIGH);
#endif
}

void LiquidCrystalHSPI::writeGpio(byte value){
      // Only write HIGH the values of the ports that have been initialised as outputs updating the output shadow of the device
	_theData = (value & ~(0x00));
  startSend();
	SPI.transfer(0x09);//GPIO
	SPI.transfer(_theData);
  endSend();
}


void LiquidCrystalHSPI::backlight(byte val){
#ifdef BACKGND_LGHTINV
	_backLight = !val;
#else
	_backLight = val;
#endif
	bitWrite(_theData,LCDPIN_LD,_backLight);
	writeByte(0x09,_theData);
}
