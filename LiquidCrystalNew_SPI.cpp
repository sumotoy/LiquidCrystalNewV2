#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#if (ARDUINO <  100)
	#include <WProgram.h>
#else
	#include <Arduino.h>
#endif

#include "LiquidCrystalNew_SPI.h"
#if defined(__FASTSWRITE2__)	
#include "_utility/DigitalIO/DigitalPin.h"
#endif
#include <../SPI/SPI.h>

//1/2 chip with software SPI GPIO (3 wire)
LiquidCrystalNew_SPI::LiquidCrystalNew_SPI(const byte cs,const byte chip,const byte adrs,const byte avoidSPIinit){
	if (chip == 0 || chip == 255){
		_en2 = 255;
		_multipleChip = 0;
	} else {
		_en2 = (1 << LCDPIN_EN2);
		_multipleChip = 1;
	}
	_avoidInit = avoidSPIinit;
	_cs = cs;
	_en1 = (1 << LCDPIN_EN);
	_adrs = adrs;
	setChip(0);
	_scroll_count = 0;      //to fix the bug if we scroll and then setCursor w/o home() or clear()
	_x = 0;
	_y = 0;
	_setCursFlag = 0;
	_direction = LCD_Right;
	setChip(0);
#if defined(BACKGND_LGHTINV)
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


void LiquidCrystalNew_SPI::begin(uint8_t cols, uint8_t lines, uint8_t dotsize) {
// ---- start SPI initializations HERE
if (!_avoidInit){
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
	SPI.setClockDivider(SPI_CLOCK_DIV4);
	SPI.setBitOrder(MSBFIRST);
	SPI.setDataMode(SPI_MODE0);
	#else
/*
SPI_CLOCK_DIV2    //8.0 MHz ----> ???
SPI_CLOCK_DIV4    //4.0 MHz ----> ok
SPI_CLOCK_DIV8    
*/
	SPI.setClockDivider(SPI_CLOCK_DIV4); 
	SPI.setBitOrder(MSBFIRST);
	SPI.setDataMode(SPI_MODE0);
	#endif
}	
#if defined(__FASTSWRITE2__)
	fastPinMode(_cs, OUTPUT);
	fastDigitalWrite(_cs, HIGH);
#else
	pinMode(_cs, OUTPUT); //set data pin modes
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
	_lcd_cols = cols;    //there is an implied lack of trust; the private version can't be munged up by the user.
	_lcd_lines = lines;
	_row_offsets[2] = _lcd_cols + _row_offsets[0];  //should auto-adjust for 16/20 or whatever columns now
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



void LiquidCrystalNew_SPI::initChip(uint8_t dotsize, byte witchEnablePin) {  
	byte	displayfunction = LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS;
	byte i;
	if (_lcd_lines > 1) displayfunction |= LCD_2LINE;
	// for some 1 line displays you can select a 10 pixel high font
	if ((dotsize != 0) && (_lcd_lines == 1)) displayfunction |= LCD_5x10DOTS;

	for (i=0;i<18;i++) {
		delayMicroseconds(LCD_STARTUP_DLY);
	}
	// Now we pull both RS and R/W low to begin commands
	//digitalWrite(_rs_pin, LOW);
	_setDataMode(0);//COMMAND MODE
	//digitalWrite(witchEnablePin, LOW);
	//writeGpio(_theData & ~witchEnablePin);  // En LOW---------------------------------------*/
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
	//writeGpio(_theData | witchEnablePin);   // En HIGH ---------------------------------
	noAutoscroll();
}


// write either command or data, with automatic 4/8-bit selection
void LiquidCrystalNew_SPI::send(uint8_t value, byte mode) {
	byte en = _en1;
	byte testChip = getChip();
	if (_multipleChip && testChip) en = _en2;
	_setDataMode(mode);					// I2C & SPI
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

void LiquidCrystalNew_SPI::write4bits(byte value) {  //still used during init
	register byte v = value;
	byte en = _en1;
	byte testChip = getChip();
 // 4x40 LCD with 2 controller chips with separate enable lines if we called w 2 enable pins and are on lines 2 or 3 enable chip 2  
	if (_multipleChip && testChip) en = _en2;   
		bitWrite(_theData,LCDPIN_D4,v & 01);
		bitWrite(_theData,LCDPIN_D5,(v >>= 1) & 01);
		bitWrite(_theData,LCDPIN_D6,(v >>= 1) & 01);
		bitWrite(_theData,LCDPIN_D7,(v >>= 1) & 01);
	pulseEnable(en);
} 


void LiquidCrystalNew_SPI::on(void) {
	display();
	backlight(1);
}


void LiquidCrystalNew_SPI::off(void) {
	noDisplay();
	backlight(0);
}

//Set data mode, want send data or command?  0:COMMAND -- 1:DATA
void LiquidCrystalNew_SPI::_setDataMode(byte mode) {
	bitWrite(_theData,LCDPIN_RS,mode);
}

void LiquidCrystalNew_SPI::pulseEnable(byte witchEnablePin) {
	writeGpio(_theData | witchEnablePin);   // En HIGH
	//NANOD;
	writeGpio(_theData & ~witchEnablePin);  // En LOW
	//NANOD;
}


void LiquidCrystalNew_SPI::writeGpio(byte value){
      // Only write HIGH the values of the ports that have been initialised as outputs updating the output shadow of the device
	_theData = (value & ~(0x00));
	writeByte(0x09,_theData);
}




void LiquidCrystalNew_SPI::backlight(byte val){
#if defined(BACKGND_LGHTINV)
	_backLight = !val;
#else
	_backLight = val;
#endif
	bitWrite(_theData,LCDPIN_LD,_backLight);
	writeByte(0x09,_theData);
}


void LiquidCrystalNew_SPI::writeByte(byte cmd,byte value){
//start send
#if defined(__FASTSWRITE2__)
	fastDigitalWrite(_cs, LOW);
	sendSPI(_adrs << 1);
	sendSPI(cmd);
	sendSPI(value); 
	fastDigitalWrite(_cs, HIGH);
#elif defined(__FASTSWRITE__)
	digitalWriteFast(_cs, LOW);
	SPI.transfer(_adrs << 1);
	SPI.transfer(cmd);
	SPI.transfer(value);
	digitalWriteFast(_cs, HIGH);
#else
	digitalWrite(_cs, LOW);
	SPI.transfer(_adrs << 1);
	SPI.transfer(cmd);
	SPI.transfer(value);
	digitalWrite(_cs, HIGH);
#endif
}
