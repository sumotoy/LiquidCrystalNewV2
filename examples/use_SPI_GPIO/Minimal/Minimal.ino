/*
	A minimal setup for drive LCD with this library
*/

#include <SPI.h>					// necessary if using LiquidCrystalNew_SPI
#include <LiquidCrystalNew_SPI.h>   // import library

//CS pin, uses 2 x HD44780 chip (1) or not (0), HAEN address (set as 0 or 255 to avoid HAEN)
LiquidCrystalNew_SPI lcd(10,0,0x20);	// create the instance

/*
	From this point all instance methods are same and all compatible with 
	classic LiquidCrystal libary.
*/

void setup(){
   lcd.begin(20,2); // SPI will be inited here by library so don't need to do manually
   lcd.print("inited");
}


void loop(){

}