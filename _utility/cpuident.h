/*
	Maybe not an example of 'how to do it' but this is very useful to identify MCU's
	It also define some other vars to semplify calls to different methods
*/

#ifndef _CPUIDENT_H_
#define _CPUIDENT_H_

#if defined(__arm__) && defined(CORE_TEENSY)																	// teensy3
	#define __TEENSY3X__
		//#define __FASTPORT__
		#define __FASTSWRITE__
#elif defined(ARDUINO) && defined(__arm__) && !defined(CORE_TEENSY)												// DUE
	#define __ARDUEX__
#elif defined(__AVR_ATmega32U4__)
	#ifdef CORE_TEENSY																							// teensy2
		#define __TEENSY2X__
			#define __FASTSWRITE2__
	#else																										// leonardo
		#define __LEONARDX__
			#define __FASTSWRITE2__
	#endif
#elif defined(__32MX320F128H__) || defined(__32MX795F512L__) 													// chipkit
	#define __CHIPKIT32__
#elif defined(__AVR_ATmega644P__) || defined(__AVR_ATmega644A__) || defined(__AVR_ATmega644PA__) || defined(__AVR_ATmega644__) || defined(__AVR_ATmega1284P__)
																												// 644 or 1284
	#define __SANGUINOX__
		#define __FASTSWRITE2__
#elif defined(__AVR_ATmega8__)																					// 8
		#define __FASTSWRITE2__
#elif defined(__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__) || defined(__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
																												// Tinys
	#define __ATTYNIX__
		#define __FASTSWRITE2__
#elif (defined(__AVR_ATmega1280__) || defined(__AVR_ATmega1281__) || defined(__AVR_ATmega2560__) || defined(__AVR_ATmega2561__))
																												//--- Arduino Mega ---
//#elif defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)												
	//#include "_utility/digitalWriteFast.h"
		//#define __FASTSWRITE__
		#define __FASTSWRITE2__
#elif defined (__AVR_ATmega328P__) || defined (__AVR_ATmega168P__)											// UNO,duemilanove
		#define __FASTSWRITE2__
#else																											// arduino UNO,duemilanove,micro,mini,etc.
	//#include "_utility/digitalWriteFast.h" 
#endif


#define nop asm volatile ("nop\n\t")
#define NANOD asm volatile ("nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t")

#endif