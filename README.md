LiquidCrystalNew V2
===================

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
additional processor pin (in GPIO's version of course).

Goals:

 - FAST ! ! !
 - totally compatible with standard LiquidCrystal (apart the bads, sorry Tom!)
 - small memory impact.
 - compact and small code.
 - large 2 HD44780 chip display capable.
 - large selection of common GPIO or Shift Register to save MC pins.
 - cursor position from memory.
 - backlight driving included.

This is the first release, alpha but SPI and Software SPI working. Wait next release for an usable version

Description of the INSTANCES:---------------------------------------------------------------------------------

[<b>LiquidCrystalHSPI</b>]

This uses 3 wires SPI to drive an MCP23s08 connected to the LCD display as 4 bit. It uses HAEN so the 3 wires can be
shared with other 7 additional MCP23Sxx chips.
It's still not completely optimized but actually it's the 'fastest' of all libraries I tested!
Using Francisco Malpartida LCDiSpeed and performanceLCD as reference here's the results on a Arduino Uno 16Mhz

 - Francisco Malpartida LiquidCrystal_SR (that uses FASTIO and a shift register):  <b>73.34us</b>
 - Francisco Malpartida LiquidCrystal_SPI (extension module that I wrote): <b>143us</b>
 - LiquidTWI2 (working in I2C): <b>over 500us</b>
 - LiquidCrystalHSPI (this library, just standard SPI from any MC): <b>101us</b> (!)

I will optimize to get better results but please consider that I'm not using any strange port manipulation (oops, just one
for CS pin but in the final release will be compatible with any MC!!!)

[<b>LiquidCrystalSSPI</b>]

Same as above but uses ANY 3 pin to drive an MCP23s08. It also has HAEN enabled so pins can be shared with other 7 MCP23sXX
chips if you use my incoming software SPI library. It's slower than hardware one (around 700us) but I will optimize to
get faster

[<b>LiquidCrystalSR</b>]

Not Yet

[<b>LiquidCrystalTWI</b>]

Not yet

[<b>LiquidCrystalNew</b>]

This is the classic 4bit mode hardwired method, exact like standard LiquidCrystal. However it can drive 2 HD44780 chip displays
and has all new features enabled.


Last Changes:-------------------------------------------------------------------------------------------------

 - HardwareSPI version working
 - SoftwareSPI version working
 
 Library seem useable even if in alpha state but not for production jet (bugs still present and methods will change)

Not Tested yet:-----------------------------------------------------------------------------------------------

 - LiquidCrystalNew has not been tested

Pin Connection:-----------------------------------------------------------------------------------------------

Soon will be posted but if you are in a hurry to try this library consult the old library in my GitHub repository,
file LiquidCrystalNew.h

some words about <b>HAEN</b> ----------------------------------------------------------------------------------

HAEN it's a feature of some Microchip MCP23Sxx GPIO chips that adds an addressing method (similar to I2C) on SPI bus.
It works by add a fixed address above every command so only the addressed chip will react, for this reason using 
non HAEN chip on the same bus it's not raccomanded and easily cause malfunctions. Of course 2 of the SPI pin can be
always shared (sck and data) as normal happen, the real advantage of HAEN it's that you can use CS as well!
HAEN not cause any performance degradation on those chips so I try to leave it always on since can help in busses
where noise it's present byt my libraries can also disable it if you use 255 as address.

