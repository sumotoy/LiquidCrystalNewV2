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
	uint8_t			getCursorCol(void);						// NEW
	uint8_t			getCursorRow(void);						// NEW
	void 			setCursor(uint8_t col, uint8_t row);			//
	void 			createChar(uint8_t, uint8_t[]);			//
#if (ARDUINO <  100)
   virtual void 	write(uint8_t value);
   virtual void 	backlight(byte value) { };				// NEW
   virtual void 	on() { };
   virtual void 	off() { };
#else
   virtual size_t 	write(uint8_t value);
   virtual void 	backlight(byte value) = 0;	
   virtual void 	on(void) = 0;
   virtual void 	off(void) = 0;
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
	byte 			_lcd_cols;
	byte 			_lcd_lines;
	byte 			_setCursFlag;
	byte 			_direction;
	byte 			_row_offsets[4];
	byte 			_displaycontrol;   	// display on/off, cursor on/off, blink on/off
	byte 			_displaymode;      	// text direction	
	byte			_backLight;			// 0:off/1:ON
	byte			_scrollOn;
	bool			_backlightFlag;
	inline void 	command(byte value)  {send(value, LOW);}
	inline void 	commandBoth(byte value)  {if (!_multipleChip) {command(value);}else{byte chipSave = getChip();setChip(0);command(value);setChip(2);command(value);setChip(chipSave);}}
	inline void		setChip(byte chip){ _chip = chip; }
	inline byte		getChip(){ return _chip; }
private:
#if (ARDUINO <  100)
	virtual void send(byte value, byte mode) { };
#else
	virtual void send(byte value, byte mode) = 0;
#endif
	void inline		delayForHome() {delayMicroseconds(LCD_HOME_DLY);}
};
#endif