/*
	A minimal setup for drive LCD with this library
	This use a software emulated SPI that allow use any 3 pin
*/

#include <LiquidCrystalNew_SSPI.h>   // import library

/*
	parameters ------------------------------------------------------
	cs_pin,data_pin,clock_pin,use 2xHD44780 chip(1) or not (0), HAEN address (set as 0 or 255 to avoid HAEN)
*/
LiquidCrystalNew_SSPI lcd(10,11,13,0,0x20);	// create the instance

/*
	From this point all instance methods are same and all compatible with 
	classic LiquidCrystal libary.
*/

void setup(){
   lcd.begin(20,2);
   lcd.print("inited");
}


void loop(){

}