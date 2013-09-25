/*
	A minimal setup for drive LCD with this library
        This if you want to connect your LCD by using a GPIO chip in SPI
        
        --- Parameters description ---
        CS pin: This is the chip select pin
        if you set an address this can be shared with other gpio chip from MCP23Sxx series
        Using multiple HD44780:some display uses 2 x HD44780 chip. If so, set this to 1
        HAEN address: if not using HAEN, set as 255 otherwise you should refere to datasheet
        Please note that you always have to tie MCP address pin to GND or + (depend of the address)
        even if you are not using HAEN. If you are unsure, set to GND.
        avoidSPIinit: see notes beloiw.
        There's an additional parameter that let you avoid SPI initialization from library.
        This can be useful if you have already inited your library in your sketch.
        If you set this parameter to 1, you MUST init SPI in setup loop BEFORE begin command!!!
        
*/

#include <SPI.h> // necessary if using LiquidCrystalNew_SPI
#include <LiquidCrystalNew_SPI.h>   // import library

/* This is the instance for 2 x HD44780 chip, use HAEN
-------------- parameters -------------------------
CS pin / Multiple HD44780 chip / HAEN address / (optional) avoidSPIinit
*/
//LiquidCrystalNew_SPI lcd(10,1,0x20);

/* This is the instance for 1 x HD44780 chip, use HAEN
-------------- parameters -------------------------
CS pin / Multiple HD44780 chip / HAEN address / (optional) avoidSPIinit
*/
LiquidCrystalNew_SPI lcd(10,0,0x20);

/* This is the instance for 1 x HD44780 chip, no HAEN
-------------- parameters -------------------------
CS pin / Multiple HD44780 chip / HAEN address / (optional) avoidSPIinit
*/
//LiquidCrystalNew_SPI lcd(10,0,255);


/*
	From this point all functions are compatible with 
	classic LiquidCrystal libary, you can open an example,
        change the instances as above and will work.
*/

void setup(){
   lcd.begin(20,2); // SPI will be inited here by library so don't need to do manually
   lcd.print("inited");
}


void loop(){

}