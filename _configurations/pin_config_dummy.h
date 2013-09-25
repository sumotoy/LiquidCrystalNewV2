#ifndef _LCDGPIOPINCONFIG_H_
#define _LCDGPIOPINCONFIG_H_

#if (ARDUINO <  100)
	#include <WProgram.h>
#else
	#ifndef Arduino_h
		#include <Arduino.h>
	#endif
#endif
/*
	in direct connection mode it's no needed a specific configuration file since pins are defined directly by the instance.
	this dummy file contains the only parameter you can configure....
*/
#define BACKGND_LGHTINV	//depends of what you using for drive led background. In my case I use an p-channel mosfet called IRFD9120

#endif