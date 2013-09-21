#ifndef _HD44780_H_
#define _HD44780_H_

#if (ARDUINO <  100)
	#include <WProgram.h>
#else
	#include <Arduino.h>
#endif

#include <inttypes.h>
#include <Print.h>

#include "_utility/cpuident.h"				// identify the MCU used
#include "_configurations/lcd_config.h"		// lcd controller constants
/*
Created by Max MC Costa for sumotoy,(sumotoy@gmail.com)
	This is the main library that holds all HD44780 methods common to all other child libraries
	that uses specific hardware
*/
class HD44780 : public Print 
{
public:
	HD44780( );

#if (ARDUINO <  100)
	virtual void 	begin(uint8_t cols, uint8_t rows, uint8_t charsize = LCD_5x8DOTS) { };
#else
	virtual void 	begin(uint8_t cols, uint8_t rows, uint8_t charsize = LCD_5x8DOTS) = 0;
#endif

	void 			clear();								//
	void 			home();									//
	void 			noDisplay();							//
	void 			display();								//
	void 			noBlink();								//
	void 			blink();								//
	void 			noCursor();								//
	void 			cursor();								//
	void 			scrollDisplayLeft();					//
	void 			scrollDisplayRight();					//
	void 			leftToRight();							//
	void 			rightToLeft();							//
	void 			autoscroll();							//
	void 			noAutoscroll();							//
	byte			getCursorCol(void);						// NEW
	byte			getCursorRow(void);						// NEW
	void 			setCursor(byte col, byte row);			//
	void 			createChar(uint8_t, uint8_t[]);			//
#if (ARDUINO <  100)
   virtual void 	write(uint8_t value);
   virtual void 	backlight(byte value) { };				// NEW
#else
   virtual size_t 	write(uint8_t value);
   virtual void 	backlight(byte value)  = 0;	
#endif
	using Print::write;
	
protected:

	byte 			_en1; 				// activated by a HIGH pulse.
	byte 			_en2; 				// activated by a HIGH pulse.
	byte 			_chip;
	byte			_multipleChip;
	int8_t 			_scroll_count;
	int8_t			_x;
	int8_t			_y;
	byte 			_numcols;
	byte 			_numlines;
	byte 			_setCursFlag;
	byte 			_direction;
	byte 			_row_offsets[4];
	byte 			_displaycontrol;   	// display on/off, cursor on/off, blink on/off
	byte 			_displaymode;      	// text direction	
	byte			_backLight;			// 0:off/1:ON
	
	inline void 	command(byte value)  {send(value, LOW);}
	inline void 	commandBoth(byte value)  {if (!_multipleChip) {send(value,LOW);} else {byte chipSave = _chip;_chip = 0;send(value,LOW);_chip = 2;send(value,LOW);_chip = chipSave;}}
private:
#if (ARDUINO <  100)
	virtual void send(uint8_t value, byte mode) { };
#else
	virtual void send(uint8_t value, byte mode) = 0;
#endif
	void inline		delayForHome() {delayMicroseconds(LCD_HOME_DLY);}
};
#endif