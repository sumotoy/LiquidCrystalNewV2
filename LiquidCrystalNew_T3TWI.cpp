#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#if defined(ENERGIA) // LaunchPad, FraunchPad and StellarPad specific
#include "Energia.h"
#else
#if (ARDUINO <  100)
	#include <WProgram.h>
#else
	#include <Arduino.h>
#endif
#endif




#if !defined(_LCDGPIOPINCONFIG_H_)
	#include "_configurations/pin_config_default.h"
	//#error you should include a configuration file!!!
#endif

#include "LiquidCrystalNew_T3TWI.h"

#if defined(__TEENSY3X__)
	#include <../i2c_t3/i2c_t3.h>
#else
	#include <../Wire/Wire.h>
#endif

//1/2 chip with software SPI GPIO (3 wire)
LiquidCrystalNew_T3TWI::LiquidCrystalNew_T3TWI(const byte adrs,byte pins,byte pullup,byte rate,const byte chip,const byte chipType){
	if (pins > 1) pins = 0;
	if (pullup > 1) pullup = 0;
	if (rate > 10) rate = 0;
	_pins = pins;
	_pullup = pullup;
	_rate = rate;
	if (chip == 0 || chip == 255){
		_en2 = 255;
		_multipleChip = 0;
	} else {
		_en2 = (1 << LCDPIN_EN2);
		_multipleChip = 1;
	}
	_en1 = (1 << LCDPIN_EN);
	_adrs = adrs;
	_chipType = chipType;
	if ((chipType == PCF8574A) && (adrs < 0x38)) _adrs = 0x38;
	_scroll_count = 0;      //to fix the bug if we scroll and then setCursor w/o home() or clear()
	_x = 0;
	_y = 0;
	_setCursFlag = 0;
	_direction = LCD_Right;
	setChip(0);
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



void LiquidCrystalNew_T3TWI::begin(uint8_t cols, uint8_t lines, uint8_t dotsize) {
	#if defined(__TEENSY3X__)
		const i2c_pins temp3[2] = {I2C_PINS_18_19,I2C_PINS_16_17};
		const i2c_pullup temp4[2] = {I2C_PULLUP_EXT,I2C_PULLUP_INT};
		const i2c_rate temp5[11] = {I2C_RATE_100,I2C_RATE_200,I2C_RATE_300,I2C_RATE_400,I2C_RATE_600,I2C_RATE_800,I2C_RATE_1000,I2C_RATE_1200,I2C_RATE_1500,I2C_RATE_2000,I2C_RATE_2400};
		Wire.begin(I2C_MASTER,0x00,temp3[_pins],temp4[_pullup],temp5[_rate]);
	#else
		#if !defined(ENERGIA) // LaunchPad, FraunchPad and StellarPad specific
			Wire.begin();
			#if ARDUINO >= 157
				Wire.setClock(400000UL); // Set I2C frequency to 400kHz
			#else
				TWBR = ((F_CPU / 400000UL) - 16) / 2; // Set I2C frequency to 400kHz
			//TWBR = 12;
			#endif
		#else
			Wire.setModule(3);
			Wire.begin();
		#endif
	#endif
	delay(100);
	if (!_chipType) writeByte(0x05,0b00100000);//use dedicated cs //MCP23008
	writeByte(0x00,0x00);//set as out (IODIR)
	writeByte(0x09,0b00000000);//write all low to GPIO

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



void LiquidCrystalNew_T3TWI::initChip(uint8_t dotsize, byte witchEnablePin) {  
	byte	displayfunction = LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS;
	byte i;
	if (_lcd_lines > 1) displayfunction |= LCD_2LINE;
	// for some 1 line displays you can select a 10 pixel high font
	if ((dotsize != 0) && (_lcd_lines == 1)) displayfunction |= LCD_5x10DOTS;

	for (i=0;i<18;i++) {
		delayMicroseconds(LCD_STARTUP_DLY);
	}

	setDataMode(0);//COMMAND MODE
	writeGpio(_theData & ~witchEnablePin);  // En LOW---------------------------------------*/
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


void LiquidCrystalNew_T3TWI::on(void) {
	display();
	backlight(1);
}

void LiquidCrystalNew_T3TWI::off(void) {
	noDisplay();
	backlight(0);
}

// write either command or data, with automatic 4/8-bit selection
void LiquidCrystalNew_T3TWI::send(uint8_t value, byte mode) {
	byte en = _en1;
	if (_multipleChip){
		if (getChip()) en = _en2;
	}	
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

void LiquidCrystalNew_T3TWI::write4bits(byte value) {  //still used during init
	register byte v = value;
	byte en = _en1;
	if (_multipleChip){
		if (getChip()) en = _en2;
	}	
		bitWrite(_theData,LCDPIN_D4,v & 01);
		bitWrite(_theData,LCDPIN_D5,(v >>= 1) & 01);
		bitWrite(_theData,LCDPIN_D6,(v >>= 1) & 01);
		bitWrite(_theData,LCDPIN_D7,(v >>= 1) & 01);
	pulseEnable(en);
} 


//Set data mode, want send data or command?  0:COMMAND -- 1:DATA
void LiquidCrystalNew_T3TWI::setDataMode(byte mode) {
	bitWrite(_theData,LCDPIN_RS,mode);
}

void LiquidCrystalNew_T3TWI::pulseEnable(byte witchEnablePin) {
	writeGpio(_theData | witchEnablePin);   // En HIGH
	writeGpio(_theData & ~witchEnablePin);  // En LOW
}


void LiquidCrystalNew_T3TWI::writeGpio(byte value){
      // Only write HIGH the values of the ports that have been initialised as outputs updating the output shadow of the device
	_theData = (value & ~(0x00));
	writeByte(0x09,_theData);
}




void LiquidCrystalNew_T3TWI::backlight(byte val){
#ifdef BACKGND_LGHTINV
	_backLight = !val;
#else
	_backLight = val;
#endif
	bitWrite(_theData,LCDPIN_LD,_backLight);
	writeByte(0x09,_theData);
}


void LiquidCrystalNew_T3TWI::writeByte(byte cmd,byte value){
	Wire.beginTransmission(_adrs);
#if ARDUINO >= 100
	if (!_chipType) Wire.write(cmd);
	Wire.write(value);
#else
	if (!_chipType) Wire.send(cmd);
	Wire.send(value);
#endif
	Wire.endTransmission();
}
