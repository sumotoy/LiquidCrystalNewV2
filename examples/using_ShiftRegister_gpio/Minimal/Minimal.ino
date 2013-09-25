/*
	A minimal setup for drive LCD with this library
        This if you want to connect your LCD by using a GPIO chip using a shift register
        
        --- Parameters description ---
        Clock pin: 
        Data pin:
		Latch pin:
        Using multiple HD44780:some display uses 2 x HD44780 chip. If so, set this to 1
*/

#include <LiquidCrystalNew_SHR.h>

/* This is the instance for 2 x HD44780 chip
-------------- parameters -------------------------
CLOCK pin / DATA pin / LATCH pin /  Multiple HD44780 chip
*/
//LiquidCrystalNew_SHR lcd(6,7,8,1);

/* This is the instance for 1 x HD44780 chip
-------------- parameters -------------------------
CLOCK pin / DATA pin / LATCH pin /  Multiple HD44780 chip
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