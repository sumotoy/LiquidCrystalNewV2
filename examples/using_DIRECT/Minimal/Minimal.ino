/*
	A minimal setup for drive LCD with this library
*/

#include <LiquidCrystalNew.h>   // import library

/* -------------- parameters -------------------------
This is the instance for 2 x HD44780 chip
rs_pin,enable1_pin,enable2_pin,d4_pin,d5_pin,d6_pin,d7_pin,backlight_pin
*/
LiquidCrystalNew lcd(3,4,5,6,7,8,9,10);	// create the instance

/* -------------- parameters -------------------------
This is the instance for 1 x HD44780 chip
rs_pin,enable1_pin,d4_pin,d5_pin,d6_pin,d7_pin,backlight_pin
*/
//LiquidCrystalNew lcd(3,4,6,7,8,9,10);	// create the instance

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