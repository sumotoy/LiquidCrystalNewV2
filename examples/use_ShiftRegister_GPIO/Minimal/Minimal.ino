/*
	A minimal setup for drive LCD with this library
*/

#include <LiquidCrystalNew_SHR.h>
/*
clock_pin,data_pin,strobe_pin,using a 2xHD44780 chip(1) or not (0)
*/
LiquidCrystalNew_SHR lcd(6,7,8,0);


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