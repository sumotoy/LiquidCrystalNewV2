/* Arduino DigitalIO Library
 * Copyright (C) 2013 by William Greiman
 *
 * This file is part of the Arduino DigitalIO Library
 *
 * This Library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This Library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with the Arduino DigitalIO Library.  If not, see
 * <http://www.gnu.org/licenses/>.
 */
/**
 * @file
 * @brief Fast Digital Pin functions
 *
 * @defgroup digitalPin Fast Pin I/O
 * @details  Fast Digital I/O functions and template class.
 * @{
 */
#ifndef DigitalPin_h
#define DigitalPin_h
#include <avr/io.h>
#include <util/atomic.h>
#include "_pindef.h"
/** count of pins */
static const uint8_t digitalPinCount = sizeof(pinMap)/sizeof(pin_map_t);
//==============================================================================
/** generate bad pin number error */
 //void badPinNumber(uint8_t pin)
/*   __attribute__((error("Pin number "))); 
  __attribute__((error2(pin))); */
  //__attribute__((error("is too large or not a constant")));
//------------------------------------------------------------------------------
/** Check for valid pin number
 * @param[in] pin Number of pin to be checked.
 */
static inline __attribute__((always_inline))
void badPinCheck(uint8_t pin) {
  //if (!__builtin_constant_p(pin) || pin >= digitalPinCount) {
  if (pin >= digitalPinCount) {
/*  		Serial.print("\n");
		Serial.print("__builtin_constant_p(");
		Serial.print(pin,DEC);
		Serial.print(") = ");
		Serial.print(__builtin_constant_p(pin),DEC);
		Serial.print(" | digitalPinCount:");
		Serial.print(digitalPinCount,DEC);
		Serial.println("");  */
     //badPinNumber(pin);
  }
}
//------------------------------------------------------------------------------
/** fast write helper
 * @param[in] address I/O register address
 * @param[in] bit bit number to write
 * @param[in] level value for bit
 */
static inline __attribute__((always_inline))
void fastBitWriteSafe(volatile uint8_t* address, uint8_t bit, bool level) {
  uint8_t oldSREG;
  if (address > (uint8_t*)0X5F) {
    oldSREG = SREG;
    cli();
  }
  if (level) {
    *address |= 1 << bit;
  } else {
    *address &= ~(1 << bit);
  }
  if (address > (uint8_t*)0X5F) {
    SREG = oldSREG;
  }
}
//------------------------------------------------------------------------------
/** read pin value
 * @param[in] pin Arduino pin number
 * @return value read
 */
static inline __attribute__((always_inline))
bool fastDigitalRead(uint8_t pin) {
  badPinCheck(pin);
  return (*pinMap[pin].pin >> pinMap[pin].bit) & 1;
}
//------------------------------------------------------------------------------
/** toggle a pin
 * @param[in] pin Arduino pin number
 *
 * If the pin is in output mode toggle the pin level.
 * If the pin is in input mode toggle the state of the 20K pullup.
 */
static inline __attribute__((always_inline))
void fastDigitalToggle(uint8_t pin) {
  badPinCheck(pin);
    if (pinMap[pin].pin > (uint8_t*)0X5F) {
      // must write bit to high address port
      *pinMap[pin].pin = 1 << pinMap[pin].bit;
    } else {
      // will compile to sbi and PIN register will not be read.
      *pinMap[pin].pin |= 1 << pinMap[pin].bit;
    }
}
//------------------------------------------------------------------------------
/** Set pin value
 * @param[in] pin Arduino pin number
 * @param[in] level value to write
 */
static inline __attribute__((always_inline))
void fastDigitalWrite(uint8_t pin, bool level) {
  badPinCheck(pin);
  fastBitWriteSafe(pinMap[pin].port, pinMap[pin].bit, level);
}
//------------------------------------------------------------------------------
/** set pin mode
 * @param[in] pin Arduino pin number
 * @param[in] mode if true set output mode else input mode
 *
 * fastPinMode does not enable or disable the 20K pullup for input mode.
 */
static inline __attribute__((always_inline))
void fastPinMode(uint8_t pin, bool mode) {
  badPinCheck(pin);
  fastBitWriteSafe(pinMap[pin].ddr, pinMap[pin].bit, mode);
}
//------------------------------------------------------------------------------
/** set pin configuration
 * @param[in] pin Arduino pin number
 * @param[in] mode If true set output mode else input mode
 * @param[in] level If mode is output, set level high/low.
 *                  If mode is input, enable or disable the pin's 20K pullup.
 */
 static inline __attribute__((always_inline))
void fastPinConfig(uint8_t pin, bool mode, bool level) {
  fastPinMode(pin, mode);
  fastDigitalWrite(pin, level);
} 
//==============================================================================
/**
 * @class DigitalPin
 * @brief Fast AVR digital port I/O
 */
template<uint8_t PinNumber>
class DigitalPin {
 public:
  //----------------------------------------------------------------------------
  /** Constructor */
  DigitalPin() {}
  //----------------------------------------------------------------------------
  /** Constructor
   * @param[in] pinMode if true set output mode else input mode.
   */
  explicit DigitalPin(bool pinMode) {
    mode(pinMode);
  }
  //----------------------------------------------------------------------------
  /** Constructor
   * @param[in] mode If true set output mode else input mode
   * @param[in] level If mode is output, set level high/low.
   *                  If mode is input, enable or disable the pin's 20K pullup.
   */
  
  DigitalPin(bool mode, bool level) {
    config(mode, level);
  }

  //----------------------------------------------------------------------------
  /** Asignment operator
   * @param[in] value If true set the pin's level high else set the
   *  pin's level low.
   *
   * @return This DigitalPin instance.
   */
  inline DigitalPin & operator = (bool value) __attribute__((always_inline)) {
    write(value);
    return *this;
  }
  //----------------------------------------------------------------------------
  /** Parenthesis operator
   * @return Pin's level
   */
	inline operator bool () const __attribute__((always_inline)) {
    return read();
  }
  //----------------------------------------------------------------------------
  /** set pin configuration
   * @param[in] mode If true set output mode else input mode
   * @param[in] level If mode is output, set level high/low.
   *                  If mode is input, enable or disable the pin's 20K pullup.
   */
  
  inline __attribute__((always_inline))
  void config(bool mode, bool level) {
    fastPinConfig(PinNumber, mode, level);
  }

  //----------------------------------------------------------------------------
  /**
   * Set pin level high if output mode or enable 20K pullup if input mode.
   */
  inline __attribute__((always_inline))
  void high() {write(true);}
  //----------------------------------------------------------------------------
  /**
   * Set pin level low if output mode or disable 20K pullup if input mode.
   */
  inline __attribute__((always_inline))
  void low() {write(false);}
  //----------------------------------------------------------------------------
  /**
   * Set pin mode
   * @param[in] pinMode if true set output mode else input mode.
   *
   * mode() does not enable or disable the 20K pullup for input mode.
   */
  inline __attribute__((always_inline))
  void mode(bool pinMode) {
    fastPinMode(PinNumber, pinMode);
  }
  //----------------------------------------------------------------------------
  /** @return Pin's level */
  inline __attribute__((always_inline))
  bool read() const {
    return fastDigitalRead(PinNumber);
  }
  //----------------------------------------------------------------------------
  /** toggle a pin
   *
   * If the pin is in output mode toggle the pin's level.
   * If the pin is in input mode toggle the state of the 20K pullup.
   */
  inline __attribute__((always_inline))
  void toggle() {
    fastDigitalToggle(PinNumber);
  }
  //----------------------------------------------------------------------------
  /** Write the pin's level.
   * @param[in] value If true set the pin's level high else set the
   *  pin's level low.
   */
  inline __attribute__((always_inline))
  void write(bool value) {
    fastDigitalWrite(PinNumber, value);
  }
};
#endif  // DigitalPin_h
/** @} */