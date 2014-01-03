#include <stdio.h>
#include <string.h>
#include <inttypes.h>

#if (ARDUINO <  100)
	#include <WProgram.h>
#else
	#include <Arduino.h>
#endif

#include "HD44780.h"

HD44780::HD44780 () 
{
	_chip = 0;
	_multipleChip = 0;
	_scroll_count = 0;
	_direction = LCD_Right;
	_x = 0;
	_y = 0;
	_setCursFlag = 0;
	_backLight = 0; 
	_scrollOn = 0; 
    _activeDelay = _defaultDelay;
}


void HD44780::clear(){
	if (_multipleChip) {
		setChip(2);
		command(LCD_CLEARDISPLAY); 
		setChip(0);
		command(LCD_CLEARDISPLAY);
		delayForHome();
		setCursor(0,0);
	} else {
		command(LCD_CLEARDISPLAY);  // clear display, set cursor position to zero
		delayForHome();
	}
	_scroll_count = 0;

}


void HD44780::home(){
	commandBoth(LCD_RETURNHOME);  // set cursor position to zero      //both chips.
	delayForHome();
	_scroll_count = 0;
	_scrollOn = 0;
	if (_multipleChip) setCursor(0,0); 
}

void HD44780::noDisplay() {
	_displaycontrol &= ~LCD_DISPLAYON;
	commandBoth(LCD_DISPLAYCONTROL | _displaycontrol);  //both chips
}

void HD44780::display() {
	_displaycontrol |= LCD_DISPLAYON;
	commandBoth(LCD_DISPLAYCONTROL | (_displaycontrol & LCD_CURSORS_MASK));   //both chips on, both cursors off
	command(LCD_DISPLAYCONTROL | _displaycontrol);              //selected chip gets cursor on
}


void HD44780::noCursor() {
	_displaycontrol &= ~LCD_CURSORON;
	command(LCD_DISPLAYCONTROL | _displaycontrol);
}
// enable cursor
void HD44780::cursor() {
	_displaycontrol |= LCD_CURSORON;
	command(LCD_DISPLAYCONTROL | _displaycontrol);
}


// Cursor not blink
void HD44780::noBlink() {
	_displaycontrol &= ~LCD_BLINKON;
	command(LCD_DISPLAYCONTROL | _displaycontrol);
}

void HD44780::blink() {
	_displaycontrol |= LCD_BLINKON;
	command(LCD_DISPLAYCONTROL | _displaycontrol);
}

void HD44780::scrollDisplayLeft(void) {
	commandBoth(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);  //both chips
	_scroll_count++;
	if (_scroll_count >= _lcd_cols) _scroll_count -= _lcd_cols; 
}
//scroll right
void HD44780::scrollDisplayRight(void) {
	commandBoth(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);  //both chips
	_scroll_count--;
	if (_scroll_count <= -_lcd_cols) _scroll_count += _lcd_cols;
}

// This is for text that flows Left to Right
void HD44780::leftToRight(void) {
	_displaymode |= LCD_ENTRYLEFT;
	_direction = LCD_Right;
	commandBoth(LCD_ENTRYMODESET | _displaymode);     //both chips
}

// This is for text that flows Right to Left
void HD44780::rightToLeft(void) {
	_displaymode &= ~LCD_ENTRYLEFT;
	_direction = LCD_Left;
	commandBoth(LCD_ENTRYMODESET | _displaymode);    //both chips
}


void HD44780::autoscroll(void) {           //to count the number of times we scrolled; here we'd need to keep track of microseconds and divide. I'm not going there.
	_displaymode |= LCD_ENTRYSHIFTINCREMENT;
	commandBoth(LCD_ENTRYMODESET | _displaymode);   //both chips
	_scrollOn = 1;
}

// This will 'left justify' text from the cursor
void HD44780::noAutoscroll(void) {
	_displaymode &= ~LCD_ENTRYSHIFTINCREMENT;   //both chips
	commandBoth(LCD_ENTRYMODESET | _displaymode);
	_scrollOn = 0;
}

uint8_t HD44780::getCursorCol(void) {
	return _x;
}

// get cursor position (row)
uint8_t HD44780::getCursorRow(void) {
	return _y;
}

void HD44780::setCursor(uint8_t col, uint8_t row) { // this can be called by the user but is also called before writing some characters.
	if (row > _lcd_lines) row = _lcd_lines-1;    // we count rows starting w/0
	_y = row;
	_x = col;
	_setCursFlag = 0;                                                 //user did a setCursor--clear the flag that may have been set in write()
   
	int8_t high_bit = _row_offsets[row] & 0x40;                        // this keeps coordinates pegged to a spot on the LCD screen even if the user scrolls right or
	int8_t offset = col + (_row_offsets[row] & 0x3F) + _scroll_count; //left under program control. Previously setCursor was pegged to a location in DDRAM
	//the 3 quantities we add are each <_lcd_cols
	if (_multipleChip){ //da testare
		if (offset > (_lcd_cols-1)) offset -= _lcd_cols; //_lcd_cols   // if the display is autoscrolled this method does not work (2 chip only) ??, however.
		if (offset < 0) offset += _lcd_cols;
	}	
	offset |= high_bit;
	byte testChip = getChip();
	if (testChip != (row & 0b10)) 	{
		command(LCD_DISPLAYCONTROL | (_displaycontrol & LCD_CURSORS_MASK));  //turn off cursors on chip we are leaving
		setChip(row & 0b10);
																		//if it is row 0 or 1 this is 0; if it is row 2 or 3 this is 2
		command(LCD_DISPLAYCONTROL | _displaycontrol);									//turn on cursor on chip we moved to
	}
	command(LCD_SETDDRAMADDR | (byte)offset);
}

void HD44780::createChar(uint8_t location, uint8_t charmap[]) {
	byte i,t;
	location &= 0x7;            // we only have 8 locations 0-7
	if (!_multipleChip) {
		command(LCD_SETCGRAMADDR | (location << 3));
		delayMicroseconds(30);
		for (i=0; i<8; i++) {
			send(charmap[i],HIGH);
			delayMicroseconds(40);
		}
	} else {
		byte chipSave = getChip();
		for (t=0;t<3;t=t+2){
			setChip(t);
			command(LCD_SETCGRAMADDR | (location << 3));
			delayMicroseconds(30);
			for (i=0; i<8; i++) {
				send(charmap[i],HIGH);
				delayMicroseconds(40);
			}
		}
		setChip(chipSave);
	}
}

void HD44780::vfdBrightness(uint8_t val) {
	if ((val >= 0) && (val < 4)) {
		commandBoth(LCD_NORITAKEBRIGHT);
		send(val,HIGH);
	}
}

void HD44780::tuneLcdDelay(int val) {
	if (val == 0){
		_activeDelay = _defaultDelay;
	} else {
		_activeDelay = _defaultDelay + val;
		if (_activeDelay < 1) _activeDelay = _defaultDelay;
	}
}


// --------------------------- SPECIAL write belongs to stream.h, need special attention -----------------------------------
#if (ARDUINO <  100)
void   HD44780::write(uint8_t value) {
#else
size_t HD44780::write(uint8_t value) {
#endif
	if (!_scrollOn && ((_scroll_count != 0) || (_setCursFlag != 0))) setCursor(_x,_y);   //first we call setCursor and send the character
	if ((value != '\r') && (value != '\n')) send(value,HIGH);
	_setCursFlag = 0;

	if (_direction == LCD_Right) {                    // then we update the x & y location for the NEXT character
		_x++;
		if ((value == '\r') || (_x >= _lcd_cols)) {      //romance languages go left to right
			_x = 0;
			_y++;
			_setCursFlag = 1;          //we'll need a setCursor() before the next char to move to begin of next line
		}
	}
	else {
		_x--;
		if ((value == '\n') || (_x < 0)) {   //emulate right to left text mode which is built in but would be defeated by my code above
			_x = _lcd_cols-1;
			_y++;
			_setCursFlag = 1;
		}
	}
	if ((_scrollOn == 0) && (_y >= _lcd_lines)) _y = 0;   //wrap last line up to line 0

#if (ARDUINO <  100)
#else
	return 1; //assume success  added for Arduino 1
#endif
}

