/*
	A minimal setup for drive LCD with this library
        This if you want to connect your LCD by using a GPIO chip in I2C
        
        --- Parameters description ---
        I2C address: take a look to chip datasheet. Normally 0x20 or 0x38 (for PCF8574A)
        Using multiple HD44780:some display uses 2 x HD44780 chip. If so set tyhis to 1
        GPIO type: you can use MCP23008,PCF8574 or PCF8574A
*/

#include <Wire.h> // necessary if using LiquidCrystalNew_TWI
#include <LiquidCrystalNew_TWI.h>   // import library

/* This is the instance for 2 x HD44780 chip using MCP23008
-------------- parameters -------------------------
I2C address / Multiple HD44780 chip / GPIO type
*/
//LiquidCrystalNew_TWI lcd(0x20,1,MCP23008);

/* This is the instance for 1 x HD44780 chip using MCP23008
-------------- parameters -------------------------
I2C address / Multiple HD44780 chip / GPIO type
*/
LiquidCrystalNew_TWI lcd(0x20,0,MCP23008);

/* This is the instance for 2 x HD44780 chip using PCF8574
-------------- parameters -------------------------
I2C address / Multiple HD44780 chip / GPIO type
*/
//LiquidCrystalNew_TWI lcd(0x20,1,PCF8574);

/* This is the instance for 1 x HD44780 chip using PCF8574
-------------- parameters -------------------------
I2C address / Multiple HD44780 chip / GPIO type
*/
//LiquidCrystalNew_TWI lcd(0x20,0,PCF8574);

/* This is the instance for 2 x HD44780 chip using PCF8574A
-------------- parameters -------------------------
I2C address / Multiple HD44780 chip / GPIO type
*/
//LiquidCrystalNew_TWI lcd(0x38,1,PCF8574A);

/* This is the instance for 1 x HD44780 chip using PCF8574A
-------------- parameters -------------------------
I2C address / Multiple HD44780 chip / GPIO type
*/
//LiquidCrystalNew_TWI lcd(0x38,1,PCF8574A);

/*
	From this point all functions are compatible with 
	classic LiquidCrystal libary, you can open an example,
        change the instances as above and will work.
*/

void setup(){
   //remember to set the size of your display here!
   lcd.begin(20,2); // Wire will be inited here by library so don't need to do manually
   lcd.print("inited");
}


void loop(){

}