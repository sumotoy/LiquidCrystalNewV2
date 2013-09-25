LiquidCrystalNew V2   
===================
<div>Version <b>beta 0.94</b> </div>. Finally a beta stage, but still buggy. 
DO NOT USE UNTIL 1.0 RELEASED (and tested)!! 

First, have to thank a lot <b>William Greiman</b> and specially <b>John Rain</b> http://code.google.com/p/liquidcrystal440/ , they truly help me a lot to discover a lot of unseen things about coding.
I also thanks Tom and Limor and Paul, without their help this will never come to life.

I wrote this library mainly for my needs... I have a lot of LCD laying around my place, mostly recovered by whatever, so I had to modify continuosly existings library because don't work with all LCD's.
Existings libraries has 2 main missed features:
 - do not handle LCD that uses 2xHD44780 chip (believe me, are NOT rare).
 - do not uses SPI GPIO chip like MCP23S08 or not use properly.

In addition, I never see a fix about setCursor that use HD ram and not tracked correctly, this until I see <b>John Rain</b> library that really inspired me alot.
Purpose to this library is drive any HD44780 (or compatible) LCD easily but should also remain compatible with the original liquidCrystal
library. In addition this one let you use large displays and it's the only one that can drive 2 x HD44780 chip displays (normally large 4x40, very
easy to find used in ebay).It's faster than original and better suited since had several bugfix and support for SPI and I2C gpio's.
hardware memory. Library it's able to drive LCD by using several GPIO's chips (that use just 2,or 3 wires) in fast way, for example using SPI MCP23S08 you get almost the same speed of direct connected LCD!
I know there's other libraries around but I wrote this for my needs some years ago and at the time I'm wroting this, none of them has all the features of this oneso if you need a pretty universal library for HD44780 displays that it's fast and pin saver
you can try this one :-)

This library works ONLY in <b>4 bit mode</b> since 8 bit has no apparent advantages (to me has only disadvantages) and WR has
stuck to ground in SPI,SR,I2C versions to save GPIO pins for drive a second HD44780 chip if needed. In addition you can get cursor row/column position and drive the backlight without use any
additional processor pin (using GPIO's version of course).New library design let user to choose from several different methods
to save microcontroller pins, for example: TWI (MCP23S008,PCF8574,PCF8574A), SPI (only MCP23Sxx series), Shift Register (74xx595) or connect directly display
without use any GPIO's.
To use existing commercial hardware shields or Piggy Back, you can load a personalized configuration file at the top of the sketch.

Goals:

 - F A S T ! ! ! (trying to avoid digitalWrite)
 - compatible with standard LiquidCrystal.
 - compatible with most MCU's.
 - pin configurable to adapt other existing hardware.
 - possibly small memory impact.
 - possibly compact and small code.
 - can drive large 2xHD44780 chip displays.
 - large selection of popular GPIO or Shift Register chip to save MC pins.
 - cursor position from memory.
 - autowrap.
 - backlight driving support included.
 - EXPANDABLE whithout growing up the main library.


Description of the INSTANCES:---------------------------------------------------------------------------------

[<b>LiquidCrystalNew</b>]

This is the basic and classic configuration where you connect your LCD directly to MCU without using any helper GPIO chip.
Main difference from classic LiquidCrystal? It's much faster!!! Actually it's the fastest I have checked thanks to <b>William Greiman</b>
DigitalPin library that works with many MCU's. I added support for Teensy3 to get faster response even on this microcontroller.


[<b>LiquidCrystal_SPI</b>]

This uses 3 wires SPI to drive an MCP23s08 connected to the LCD display as 4 bit. It uses HAEN so the 3 wires can be
shared with other 7 additional MCP23Sxx chips.
It's still not fully optimized but actually it's the 'fastest' of all libraries I tested!


<b>MCP23S08</b> connections

        computer side              LCD side
                           
        sck (13)  -> [|--U--|] <- +5v
                     [|     |] <- background led driver out
        miso (11) -> [|     |] <- D7
        adrs*     -> [|     |] <- D6
        adrs*     -> [|     |] <- D5
        +5V-(4k7)--> [|     |] <- D4
        CS (10)   -> [|     |] <- E
                     [|     |] <- RS
        gnd       -> [|_____|] <- EN2 (if needed)
        
LCD's R/W to ground
(remember that you can change gpio's connection with lcd in a file inside _configurations folder!!!)
        
[<b>LiquidCrystal_SSPI</b>]

Same as above but uses ANY 3 pin to drive an MCP23s08. It also has HAEN enabled so pins can be shared with other 7 MCP23sXX
chips if you use my incoming software SPI library.

<b>MCP23S08</b> connections

        computer side              LCD side
                           
        soft SCK  -> [|--U--|] <- +5v
                     [|     |] <- background led driver out
       soft MISO  -> [|     |] <- D7
        adrs*     -> [|     |] <- D6
        adrs*     -> [|     |] <- D5
        +5V-(4k7)--> [|     |] <- D4
         soft CS  -> [|     |] <- E
                     [|     |] <- RS
        gnd       -> [|_____|] <- EN2 (if needed)

LCD's R/W to ground
(remember that you can change gpio's connection with lcd in a file inside _configurations folder!!!)
        
[<b>LiquidCrystal_TWI</b>]

This version uses 2 wire MCP23008,PCF8574 & PCF8574A chip as GPIO and it's directly compatible with ladyada LCD piggyback. GPIO pins are freely configurable
though a dedicated file so it can be easily adapted to other existing hardware design.

<b>MCP23008</b> connections

        computer side              LCD side
                           
             scl  -> [|--U--|] <- +5v
             sda  -> [|     |] <- background led driver out
        adrs*     -> [|     |] <- D7
        adrs*     -> [|     |] <- D6
        adrs*     -> [|     |] <- D5
        +5V-(4k7)--> [|     |] <- D4
                     [|     |] <- E
                     [|     |] <- RS
        gnd       -> [|_____|] <- EN2 (if needed)
        
        
<b>PCF8574/PCF8574A</b> connections
 
         adrs* A0 -> [|--U--|] <+ +5v
         adrs* A1 -> [|     |] <= sda
         adrs* A2 -> [|     |] <= scl
              EN2 -> [|     |] 
               RS -> [|     |] <- backlight (need driver*)
               EN -> [|     |] <- D7
               D4 -> [|     |] <- D6
              gnd -> [|_____|] <- D5

LCD's R/W to ground
(remember that you can change gpio's connection with lcd in a file inside _configurations folder!!!)


[<b>LiquidCrystal_SHR</b>]

This version uses 3 wire shift register chip 74XX595 as GPIO. GPIO pins are freely configurable
though a dedicated file so it can be easily adapted to other existing hardware design.

<b>74xx595</b> connections

               RS -> [|--U--|] <= +5v
               EN -> [|     |] <- EN2
               D4 -> [|     |] <- dta_pin
               D5 -> [|     |] <= gnd 
               D6 -> [|     |] <- strobe_pin
               D7 -> [|     |] <- clk_pin
        backlight -> [|     |] <= +5v
             gnd  => [|_____|] 

LCD's R/W to ground
(remember that you can change gpio's connection with lcd in a file inside _configurations folder!!!)

About Backlight:----------------------------------------------------------------------------------------------

All GPIO's supported have one pin for drive led backlight but beware that you cannot use it directly!!!
The current needed can destroy GPIO's or MCU so you need a simple circuit that can be done with transistors, 
mosfet,etc. I haved used an IRF9120 with a couple of resistors but you can use your own one since the library
provide a way to invert or not the way backlight it's managed. Simply open <b>_configurations/lcd_config.h</b> file,
it contains a line called <b>#define BACKGND_LGHTINV</b> that invert the backlight pin, comment out this line will
go back to normal. I will publish a couple of tested easy example circuits when release it's out.

<b>Last Changes:</b>-------------------------------------------------------------------------------------------------

 - HardwareSPI version working (v a01)
 - SoftwareSPI version working (v a01)
 - Increased speed (v a02)
 - First beta version with major changes and speed improvements (v b0.8)
 - Fixed Software SPI (slow method) (v b0.81)
 - TWI handle more chips, some fix. (v b0.83)
 - Dropped hardware methods (useless!!!), various fix. (v b0.85) <----
 - Lot of fix! 2xHD chip large display works, fixed garbage char issue, fixed scrolling, fixed strange issue
 after autoscroll, slow down a little SPI thoughput that was too fast for GPIO chips (v b0.9)
 - Beta version of shift register method. (v b0.91) 
 - Cleaned methods, added external config file (usable from sketch). (v b0.94) <----

<b>Not Tested or Working out:</b>-------------------------------------------------------------------------------------------

 - Software SPI method dropped in beta but will be present in final release.

<b>Know Bugs:</b>-----------------------------------------------------------------------------------------------------

- A well know one still present only with 2 x HD44780 chips large displays and only related to autoscroll.

<b>Speed Test and comparison with other libraries:</b>---------------------------------------------------------------

Speed tests where maded with the same MCU (arduino UNO/16Mhz), same sketch and an LCD of 2x20.

= <b>Direct Connection Test</b> ===========================================

 +++ FPS TEST +++++++++++++++++++++++++++++++++++++++++++++++++++++
 
 - LiquidCrystal (classic)             : 72,37fps
 - LiquidCrystal (francesco malpartida): 287,85fps 
 - LiquidCrystalNew (this library)     : <b>383fps</b> best result 
 
 +++ Byte XFer ++++++++++++++++++++++++++++++++++++++++++++++++++++

 - LiquidCrystal (classic)             : 329us
 - LiquidCrystal (francesco malpartida): 83us 
 - LiquidCrystalNew (this library)     : <b>62us</b> best result 
 
 +++ FTime ++++++++++++++++++++++++++++++++++++++++++++++++++++++++

 - LiquidCrystal (classic)             : 13,82ms
 - LiquidCrystal (francesco malpartida): 3,47ms 
 - LiquidCrystalNew (this library)     : <b>2,61ms</b> best result 

= <b>SPI GPIO chip helper test</b> ========================================

  Francesco Malpartida library has not SPI support so I wrote a small addon.

 +++ FPS TEST ++++++++++++++++++++++++++++++++++++++++++++++++++++
 
 - LiquidCrystal (francesco malpartida): 167,3fps 
 - LiquidCrystalNew (this library)     : <b>351fps</b> best result 
 
 +++ Byte XFer +++++++++++++++++++++++++++++++++++++++++++++++++++

 - LiquidCrystal (francesco malpartida): 142us 
 - LiquidCrystalNew (this library)     : <b>63us</b> best result 
 
 +++ FTime +++++++++++++++++++++++++++++++++++++++++++++++++++++++

 - LiquidCrystal (francesco malpartida): 5,98ms 
 - LiquidCrystalNew (this library)     : <b>2,24ms</b> best result 

 
= <b>TWI GPIO chip helper test</b> ==========================================

 +++ FPS TEST ++++++++++++++++++++++++++++++++++++++++++++++++++++
 
 - LiquidCrystal (francesco malpartida): 
 - LiquidCrystalNew (this library)     : <b>48,57fps</b> 
 - LiquidTWI2                          : 49,57 best result 
 
 +++ Byte XFer +++++++++++++++++++++++++++++++++++++++++++++++++++

 - LiquidCrystal (francesco malpartida):  
 - LiquidCrystalNew (this library)     : <b>484us</b> 
 - LiquidTWI2                          : 475us best result 
 - 
 +++ FTime +++++++++++++++++++++++++++++++++++++++++++++++++++++++

 - LiquidCrystal (francesco malpartida):  
 - LiquidCrystalNew (this library)     : <b>20,18ms</b> 
 - LiquidTWI2                          : 16,47ms best result 

As you see this library outperforms any others apart TWI that still need some work. The main problem of TWI is
that cannot use very fast or exotic solutions since can break other TWI devices connected on same bus.

some words about <b>HAEN</b> ----------------------------------------------------------------------------------

HAEN it's a feature of some Microchip MCP23Sxx GPIO chips that adds an addressing method (similar to I2C) on SPI bus.
It works by add a fixed address above every command so only the addressed chip will react, for this reason using 
non HAEN chip on the same bus it's not raccomanded and easily cause malfunctions. Of course 2 of the SPI pin can be
always shared (sck and data) as normal happen, the real advantage of HAEN it's that you can use CS as well!
HAEN not cause any performance degradation on those chips so I try to leave it always on since can help in busses
where noise it's present byt my libraries can also disable it if you use 255 as address.

