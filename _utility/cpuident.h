/*
	---------- super silly mcu identifier -------------------------------------------
	Maybe not an example of 'how to do it' but this is very useful to identify MCU's
	It also define some other vars to simplify calls to different methods
*/

#ifndef _CPUIDENT_H_
#define _CPUIDENT_H_

#if defined(ENERGIA) // LaunchPad, FraunchPad and StellarPad specific
	#define __ENERGIAX__
#elif defined(__MK20DX128__)//3.0													
	#define __TEENSY3X__
	#define __FASTSWRITE__
#elif defined(__MK20DX256__)//3.1
	#define __TEENSY3X__
	#define __FASTSWRITE__
#elif defined(__MKL26Z64__)//LC
	#define __TEENSY3X__
	#define __FASTSWRITE__
#elif defined(ARDUINO) && defined(__arm__) && !defined(CORE_TEENSY)	//sometime ide makes confusion...											
	#define __ARDUEX__
#elif defined(__AVR_ATmega32U4__)
	#ifdef CORE_TEENSY																							
		#define __TEENSY2X__
		#define __FASTSWRITE2__
	#else																										
		#define __LEONARDX__
		#define __FASTSWRITE2__
	#endif
#elif defined(__32MX320F128H__) || defined(__32MX795F512L__) //uno and max													
	#define __CHIPKIT32X__
#elif defined(__AVR_ATmega644P__) || defined(__AVR_ATmega644A__) || defined(__AVR_ATmega644PA__) || defined(__AVR_ATmega644__) || defined(__AVR_ATmega1284P__)
	#define __SANGUINOX__
	#define __FASTSWRITE2__
#elif defined(__AVR_ATmega8__) //oldboy...
	//#define __FASTSWRITE2__
#elif defined(__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__) || defined(__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
	#define __ATTYNIX__
#elif (defined(__AVR_ATmega1280__) || defined(__AVR_ATmega1281__) || defined(__AVR_ATmega2560__) || defined(__AVR_ATmega2561__))
	#define __ATMEGAX__
	#define __FASTSWRITE2__
#elif defined (__AVR_ATmega328P__) || defined (__AVR_ATmega168P__)											
	#define __ATUNOX__
	#define __FASTSWRITE2__
#else			
	#define __UNDEFINEX__																								
#endif



//this should'n stays here since it depends of processor and speed so it will be moved soon...


#endif