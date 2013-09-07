LiquidCrystalNew V2   
===================
<div>Version <b>alpha02</b> </div>

An optimized version of my old library LiquidCrystalNew.
Purpose to this library is drive any HD44780 drived (or compatible) LCD easily, LiquidCrystal compatible commands
fast and use less pin as possible. Actually this is the only library that can drive 2 HD44780 chip LCD (usually large
40X4) lcd's.
Respect old library this has been improved, uses much less memory and space since every method has a separate file.
I have fixed a lot of stuff that never included in the standard LiquidCrystal as correct cursor position (now read from memory),
corrected scrolling and after scrolling problems (like position of the cursor, etc)
This library works ONLY in 4 bit mode since 8 bit has no apparent advantages (to me has only disadvantages) and WR has
stuck to ground in SPI,SR,I2C versions to save GPIO pins for drive a second HD44780 if needed (even this has not
apparently disadvantages). In addition you can get cursor row/column position and drive the backlight without use any
additional processor pin (in GPIO's version of course). New library design let user to choose from many different method
to save microcontroller pins, TWI (different chips), SPI (only MCP23Sxx series), Shift Register or connect directly display
without use any GPIO's.
GPIO connections to display can be easily modified by change the file utility/spi_config.h so it's easy to adapt existings
 shield or piggyBack modules.

Goals:

 - F A S T ! ! !
 - compatible with standard LiquidCrystal (apart the bads, sorry Tom!),
 - small memory impact.
 - compact and small code.
 - can drive large 2 HD44780 chip displays.
 - large selection of popular GPIO or Shift Register chip to save MC pins.
 - cursor position from memory.
 - autowrap.
 - backlight driving included.

<div><b>ATTENTION!!!</b></div>
<b>This is the first EXPERIMENTAL release. Wait a beta release for an usable version!!!</b>

Description of the INSTANCES:---------------------------------------------------------------------------------

[<b>LiquidCrystalHSPI</b>]

This uses 3 wires SPI to drive an MCP23s08 connected to the LCD display as 4 bit. It uses HAEN so the 3 wires can be
shared with other 7 additional MCP23Sxx chips.
It's still not optimized and all in experiment mode but actually it's the 'fastest' of all libraries I tested!
Using <i>Francisco Malpartida</b> <b>LCDiSpeed</b> and <b>performanceLCD</b> as reference here's the results on a Arduino Uno 16Mhz

 - Francisco Malpartida <b>LiquidCrystal_SR</b> (that uses FASTIO and a shift register):  <b>73.34us</b>
 - Francisco Malpartida <b>LiquidCrystal_SPI</b> (extension module that I wrote): <b>143us</b>
 - <b>LiquidTWI2</b> (working in I2C at 400Khz): <b>over 500us</b> (ouch...)
 - <b>LiquidCrystalHSPI</b> (this library, standard SPI): <b>101us</b>



MCP23s08/MCP23008 connections

        computer side              LCD side
                           
        sck (13)  -> [|--U--|] <- +5v
                     [|     |] <- background led driver out
        miso (11) -> [|     |] <- D7
        adrs*     -> [|     |] <- D6
        adrs*     -> [|     |] <- D5
        +5V-(10k)--> [|     |] <- D4
        CS (10)   -> [|     |] <- E
                     [|     |] <- RS
        gnd       -> [|_____|] <- EN2 (if needed)
        
[<b>LiquidCrystalHSPIF</b>]

This is an experiment, Hardware SPI fast.
Same as above but uses the really fast <b>digitalIO</b> library by <b>William Greiman</b>.
Same functionalities of the library above and CS pin it's freely assegnable. Performance really improved!

 - Byte transfer: <b>80us</b>
 - FPS : <b>297</b>
 - Write time: <b>3.3us</b>

Numbers near Francisco Malpartida LiquidCrystal_SR but here I'm using an SPI and HAEN it's enabled so I can share more
GPIO's on the same line and use sck and miso for other devices!
I can get a better performance by removing CS assignement (convert as constant), around <b>73us</b>,
 don't think it's possible squeeze more speed than that from MCP23Sxx whithout sacrifice some features of the library.

[<b>LiquidCrystalSSPI</b>]

Same as above but uses ANY 3 pin to drive an MCP23s08. It also has HAEN enabled so pins can be shared with other 7 MCP23sXX
chips if you use my incoming software SPI library. It's slower than hardware one (around <b>378us</b>) but I will optimize to
get faster.

MCP23s08/MCP23008 connections

        computer side              LCD side
                           
        soft SCK  -> [|--U--|] <- +5v
                     [|     |] <- background led driver out
       soft MISO  -> [|     |] <- D7
        adrs*     -> [|     |] <- D6
        adrs*     -> [|     |] <- D5
        +5V-(10k)--> [|     |] <- D4
         soft CS  -> [|     |] <- E
                     [|     |] <- RS
        gnd       -> [|_____|] <- EN2 (if needed)
        
[<b>LiquidCrystalSSPIF</b>]

Experimental.
Same as above but uses <b>digitalIO</b> library by <b>William Greiman</b> to greatly improve performance (around <b>75us!!!</b>).

[<b>LiquidCrystalSR</b>]

Not Yet

[<b>LiquidCrystalTWI</b>]

Not yet

[<b>LiquidCrystalNew</b>]

This is the classic 4bit mode hardwired method, exact like standard LiquidCrystal. However it can drive 2 HD44780 chip displays
and has all new features enabled.


Last Changes:-------------------------------------------------------------------------------------------------

 - HardwareSPI version working (v a01)
 - SoftwareSPI version working (v a01)
 - Increased speed (v a02)

Not Tested yet:-----------------------------------------------------------------------------------------------

 - LiquidCrystalNew has not been tested


some words about <b>HAEN</b> ----------------------------------------------------------------------------------

HAEN it's a feature of some Microchip MCP23Sxx GPIO chips that adds an addressing method (similar to I2C) on SPI bus.
It works by add a fixed address above every command so only the addressed chip will react, for this reason using 
non HAEN chip on the same bus it's not raccomanded and easily cause malfunctions. Of course 2 of the SPI pin can be
always shared (sck and data) as normal happen, the real advantage of HAEN it's that you can use CS as well!
HAEN not cause any performance degradation on those chips so I try to leave it always on since can help in busses
where noise it's present byt my libraries can also disable it if you use 255 as address.

