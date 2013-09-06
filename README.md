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

 - fast.
 - totally compatible with standard LiquidCrystal (apart the bads)
 - small memory impact.
 - compact and small code.
 - large 2 HD44780 chip display capable.
 - large selection of GPIO or Shift Register to save pins.
 - cursor position from memory.
 - backlight driving.

This is the first release, alpha but SPI and Software SPI working. Wait next release for an usable version

Description of the INSTANCES:---------------------------------------------------------------------------------

[LiquidCrystalHSPI]
This uses 3 wires SPI to drive an MCP23s08 connected to the LCD display as 4 bit. It uses HAEN so the 3 wires can be
shared with other 7 additional MCP23Sxx chips.
It's still not completely optimized but actually it's the 'fastest' of all libraries I tested!
Using Francisco Malpartida LCDiSpeed and performanceLCD as reference here's the results on a Arduino Uno 16Mhz

 - Francisco Malpartida LiquidCrystal_SR (that uses FASTIO and a shift register):  73.34us
 - Francisco Malpartida LiquidCrystal_SPI (extension module that I wrote): 143us
 - LiquidTWI2: over 500us
 - LiquidCrystalHSPI (this library, just standard SPI from any MC): 101us (!)
 - 
I will optimize to get better results but please consider that I'm not using any strange port manipulation (oops, just one
for CS pin but in the final release will be compatible with any MC!!!)

[LiquidCrystalSSPI]
Same as above but uses ANY 3 pin to drive an MCP23s08. It also has HAEN enabled so pins can be shared with other 7 MCP23sXX
chips if you use my incoming software SPI library. It's slower than hardware one (around 700us) but I will optimize to
get faster

[LiquidCrystalNew]
This is the classic 4bit mode hardwired method, exact like standard LiquidCrystal. However it can drive 2 HD44780 chip displays
and has all new features enabled.


Last Changes:-------------------------------------------------------------------------------------------------

 - HardwareSPI version working
 - SoftwareSPI version working

Not Tested yet:-----------------------------------------------------------------------------------------------

 - LiquidCrystalNew has not been tested
