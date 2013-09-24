/*
	A minimal setup for drive LCD with this library
*/

#include <Wire.h>					// necessary if using LiquidCrystalNew_TWI
#include <LiquidCrystalNew_TWI.h>   // import library

#define GPIO_ADRS 0x20 //PCF8574A starts at 0x38
#define GPIO_TYPE MCP23008 // can be MCP23008,PCF8574,PCF8574A
#define MULTIPLE_HD_CHIPS 0 // 2xHD44780 display? 0=no

#define LCD_COLS 20
#define LCD_ROWS 2

//I2C address (depends of chip), uses 2x44780 chip (set as 1) or standard (set as 0),GPIO chip type (MCP23008,PCF8574,PCF8574A)
LiquidCrystalNew_TWI lcd(GPIO_ADRS,MULTIPLE_HD_CHIPS,GPIO_TYPE);	// create the instance

/*
	From this point all instance methods are same and all compatible with 
	classic LiquidCrystal libary.
*/

void setup(){
   lcd.begin(LCD_COLS,LCD_ROWS); // Wire will be inited here by library so don't need to do manually
   lcd.print("inited");
}


void loop(){

}