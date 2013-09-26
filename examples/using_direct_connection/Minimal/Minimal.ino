/*
	A minimal setup for drive LCD with this library
        This if you want to connect your LCD by directly connecting on your MC
        
        --- Parameters description (from left)---
        RS  pin:
		EN  pin: 
		EN2 pin: Only few LCD has 2 HD chip onboard that need this pin. If your not use, ignore it
		D4: data pin4
		D5: data pin5
		D6: data pin6
		D7: data pin7
		Backlight pin: If you want drive separately LCD backlight put 255
		On LCD side, connect WR to ground
----------- Some setup for commercial LCD	
		
//LiquidCrystalNew lcd(8,9,4,5,6,7,255);//DFRobot shield v1
//http://www.dfrobot.com/index.php?route=product/product&product_id=51#.UkSTJj_4LZx
*/
#include <LiquidCrystalNew.h>   // import library

/* -------------- parameters -------------------------
This is the instance for 2 x HD44780 chip, backlight on 10
rs_pin,enable1_pin,enable2_pin,d4_pin,d5_pin,d6_pin,d7_pin,backlight_pin
*/
LiquidCrystalNew lcd(3,4,5,6,7,8,9,10);	


/* -------------- parameters -------------------------
This is the instance for 1 x HD44780 chip, backlight not drived
rs_pin,enable1_pin,d4_pin,d5_pin,d6_pin,d7_pin,backlight_pin
*/
//LiquidCrystalNew lcd(3,4,6,7,8,9,255);	

/*
	From this point all functions are compatible with 
	classic LiquidCrystal libary, you can open an example,
        change the instances as above and will work.
*/

void setup(){
   lcd.begin(20,2); 
   lcd.print("inited");
}


void loop(){

}