LiquidCrystalNew V2   
===================
<div>Version <b>beta 0.83</b> </div>. Finally a beta stage!

An optimized version of my old library LiquidCrystalNew.
Purpose to this library is drive any HD44780 drived (or compatible) LCD easily, LiquidCrystal compatible commands
fast and use less pin as possible. Actually this is the only library that can drive 2 HD44780 chip LCD (usually large
40X4) lcd's.
Respect old library this has been improved, uses much less memory and space since every method has a separate file.
I have fixed a lot of stuff that never included in the standard LiquidCrystal as correct cursor position (now read from memory),
corrected scrolling and after scrolling problems (like position of the cursor, etc)
This library works ONLY in <b>4 bit mode</b> since 8 bit has no apparent advantages (to me has only disadvantages) and WR has
stuck to ground in SPI,SR,I2C versions to save GPIO pins for drive a second HD44780 if needed (even this has not
apparently disadvantages). In addition you can get cursor row/column position and drive the backlight without use any
additional processor pin (in GPIO's version of course). New library design let user to choose from many different method
to save microcontroller pins, TWI (different chips), SPI (only MCP23Sxx series), Shift Register or connect directly display
without use any GPIO's.
GPIO connections to display can be easily modified by change the file utility/spi_config.h so it's easy to adapt existings
 shield or piggyBack modules.

Goals:

 - F A S T ! ! !
 - compatible with standard LiquidCrystal (apart the bads, sorry Tom!).
 - compatiple with most MCU's.
 - pin configurable to adapt other existing library.
 - small memory impact.
 - compact and small code.
 - can drive large 2xHD44780 chip displays.
 - large selection of popular GPIO or Shift Register chip to save MC pins.
 - cursor position from memory.
 - autowrap.
 - backlight driving included.


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


Last Changes:-------------------------------------------------------------------------------------------------

 - HardwareSPI version working (v a01)
 - SoftwareSPI version working (v a01)
 - Increased speed (v a02)
 - First beta version with major changes and speed improvements (v b0.8)
 - Fixed Software SPI (slow method) (v b0.81)
 - TWI handle more chips, some fix. (v b0.83) <----

Not Tested yet:-----------------------------------------------------------------------------------------------

 - LiquidCrystal_SHR still not working (next release will...)

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
 - LiquidCrystalNew (this library)     : <b>355fps</b> best result 
 
 +++ Byte XFer +++++++++++++++++++++++++++++++++++++++++++++++++++

 - LiquidCrystal (francesco malpartida): 142us 
 - LiquidCrystalNew (this library)     : <b>63us</b> best result 
 
 +++ FTime +++++++++++++++++++++++++++++++++++++++++++++++++++++++

 - LiquidCrystal (francesco malpartida): 5,98ms 
 - LiquidCrystalNew (this library)     : <b>2,24ms</b> best result 

 
= <b>TWI GPIO chip helper test</b> ==========================================

 +++ FPS TEST ++++++++++++++++++++++++++++++++++++++++++++++++++++
 
 - LiquidCrystal (classic)             : 
 - LiquidCrystal (francesco malpartida): 
 - LiquidCrystalNew (this library)     : <b>48,57fps</b> 
 - LiquidTWI2                          : 49,57 best result 
 
 +++ Byte XFer +++++++++++++++++++++++++++++++++++++++++++++++++++

 - LiquidCrystal (classic)             : 
 - LiquidCrystal (francesco malpartida):  
 - LiquidCrystalNew (this library)     : <b>484us</b> 
 - LiquidTWI2                          : 475us best result 
 - 
 +++ FTime +++++++++++++++++++++++++++++++++++++++++++++++++++++++

 - LiquidCrystal (classic)             : 
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

