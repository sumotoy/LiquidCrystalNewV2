/*
	A minimal setup for drive LCD with this library
*/

#include <Wire.h>					// necessary if using LiquidCrystalNew_TWI
#include <LiquidCrystalNew_TWI.h>   // import library

//I2C address, uses 2x44780 chip (set as 1) or standard 1 chip (set as 0)
LiquidCrystalNew_TWI lcd(0x20,0);	// create the instance

/*
	From this point all instance methods are same and all compatible with 
	classic LiquidCrystal libary.
*/

void setup(){
   lcd.begin(20,2); // Wire will be inited here by library so don't need to do manually
   lcd.print("inited");
}


void loop(){

}