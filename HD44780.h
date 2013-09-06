#ifndef _HD44780_H_
#define _HD44780_H_

#if (ARDUINO <  100)
	#include <WProgram.h>
#else
	#include <Arduino.h>
#endif

#include <inttypes.h>
#include <Print.h>

// -------- DETECT micros -----------------------------------------
#if defined(__arm__) && defined(CORE_TEENSY)//teensy3
	#define TEENSY3X
	#define __FASTPORT__
	#define __FASTSWRITE__
#elif defined(ARDUINO) && defined(__arm__) && !defined(CORE_TEENSY)//due
	#define ARDUE
#elif defined(__AVR_ATmega32U4__)
	#ifdef CORE_TEENSY// teensy2
		#define TEENSY2
	#else//leonardo
		#define LEONARDX
	#endif
#elif defined(__AVR_ATmega644P__) || defined(__AVR_ATmega644A__) || defined(__AVR_ATmega644PA__) || defined(__AVR_ATmega644__) || defined(__AVR_ATmega1284P__)//not supported 
#elif defined(__AVR_ATmega8__)//not supported
#elif defined(__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__) || defined(__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)//not supported
#else//arduino UNO,duemilanove,micro,mini,etc.
	#include "utility/digitalWriteFast.h"
	#define __FASTSWRITE__
#endif
//-------------------------------------------------------------------

#include "utility/lcdConfig.h"

class HD44780 : public Print 
{
public:
	HD44780();
#if (ARDUINO <  100)
	virtual void 	begin(uint8_t cols, uint8_t rows, uint8_t charsize = LCD_5x8DOTS) { };
	virtual void 	backlight(byte value) { };
#else
	virtual void 	begin(uint8_t cols, uint8_t rows, uint8_t charsize = LCD_5x8DOTS) = 0;
	virtual void 	backlight(byte value)  = 0;
#endif

	void 			clear();
	void 			home();
	void 			noDisplay();
	void 			display();
	void 			noBlink();
	void 			blink();
	void 			noCursor();
	void 			cursor();
	void 			scrollDisplayLeft();
	void 			scrollDisplayRight();
	void 			leftToRight();
	void 			rightToLeft();
	void 			autoscroll();
	void 			noAutoscroll();
	byte			getCursorCol(void);
	byte			getCursorRow(void);
	void 			setCursor(byte col, byte row);
	void 			createChar(uint8_t, uint8_t[]);
#if (ARDUINO <  100)
   virtual void write(uint8_t value);
#else
   virtual size_t write(uint8_t value);
#endif
	using Print::write;
	//inline void 	command(byte value)  {send(value, LOW);}
	//inline void 	commandBoth(byte value)  {if (!_multipleChip) {send(value,LOW);} else {byte chipSave = _chip;_chip = 0;send(value,LOW);_chip = 2;send(value,LOW);_chip = chipSave;}}
protected:
	inline void 	command(byte value)  {send(value, LOW);}
	inline void 	commandBoth(byte value)  {if (!_multipleChip) {send(value,LOW);} else {byte chipSave = _chip;_chip = 0;send(value,LOW);_chip = 2;send(value,LOW);_chip = chipSave;}}
	byte 			_en1; 		// activated by a HIGH pulse.
	byte 			_en2; 		// activated by a HIGH pulse.
	//byte 			_data_pins[4]; 
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
	byte 			_displaycontrol;   //display on/off, cursor on/off, blink on/off
	byte 			_displaymode;      //text direction	
	byte			_backLight;			//0:off/1:ON
private:
#if (ARDUINO <  100)
	virtual void send(uint8_t value, byte mode) { };
#else
	virtual void send(uint8_t value, byte mode) = 0;
#endif
};
#endif