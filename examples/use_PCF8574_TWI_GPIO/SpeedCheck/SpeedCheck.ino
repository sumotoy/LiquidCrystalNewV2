/*
	Classic LCD speed test modified to work with most MCU!!!
*/


#define iLCD		// turn on code to calculate speed of "independent" sized display

#define LCD_COLS 16
#define LCD_ROWS 2

#define iLCD_COLS 20 // independent FPS col size
#define iLCD_ROWS 2 // independent FPS row size

#define FPS_iter 1 	// number of iterations to repeat each "frame" within the test 
// (current frame test is 10 full display frames, 1 for each digits 0-9)
// FPS_iter like 100 will allow the frames to be seen
// Note: the only reason other than visual to make this larger than 1
// might be to compensate for Async serial buffering should a serial interface be tested
// even with 1 iteration, 340 bytes are written for a 16x2 display
// bytes written = FPS_iter * ((LCD_ROWS * LCD_COLS) + LCD_ROWS) * 10

#define DELAY_TIME 3500 // delay time to see information on lcd

#include <Wire.h>
#include <LiquidCrystalNew_TWI.h>


LiquidCrystalNew_TWI lcd(0x38,0,PCF8574A);


#ifdef __AVR__
extern unsigned int __bss_end;
extern unsigned int __heap_start;
extern void *__brkval;
#endif

static int freeMemory(void) 
{
#ifdef __AVR__
  int free_memory;
  if((int)__brkval == 0)
    free_memory = ((int)&free_memory) - ((int)&__bss_end);
  else
    free_memory = ((int)&free_memory) - ((int)__brkval);
  return free_memory;
#else
  return -1;
#endif
}



void setup()
{
  lcd.begin(LCD_COLS, LCD_ROWS);
#ifdef __AVR__
  lcd.print("Free mem: ");
  lcd.print(freeMemory());
  delay(3000);
  lcd.clear();
#endif
}

void loop()
{
  unsigned long etime;
  lcd.clear();
  etime = timeFPS(FPS_iter,LCD_COLS,LCD_ROWS);
  showByteXfer(etime);
  showFPS(etime,0);
#ifdef iLCD
  etime = etime*iLCD_ROWS*iLCD_COLS/LCD_ROWS/LCD_COLS;
  showFPS(etime,1);
#endif

}

unsigned long timeFPS(uint8_t iter, uint8_t cols, uint8_t rows)
{
  char c;
  unsigned long stime, etime;
  stime = micros();
  for(c = '9'; c >= '0'; c--) // do not change this unless you change the FPS/ByteXfer calcuations as well
  {
    for(uint8_t i = 0; i < iter; i++)
    {
      for(uint8_t row = 0; row < rows; row++)
      {
        lcd.setCursor(0, row);
        for(uint8_t col = 0; col< cols;col++)
        {
          lcd.write(c);
        }
      }
    }
  }
  etime = micros();
  return((etime-stime));
}


void showFPS(unsigned long etime,byte mode)
{
  float fps;
  fps = (10.0*FPS_iter)*1000000.0/(etime);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("[");
  if (mode){
    lcd.print(iLCD_ROWS,DEC);
    lcd.print("x");
    lcd.print(iLCD_COLS,DEC);
  } else {
    lcd.print(LCD_ROWS,DEC);
    lcd.print("x");
    lcd.print(LCD_COLS,DEC);
  }
  lcd.print("]:");
  lcd.print(fps);
  lcd.print("fps");
  lcd.setCursor(0,1);
  lcd.print("Ftime:");
  lcd.print((etime)/10.0/FPS_iter/1000);
  lcd.print("ms");
  delay(DELAY_TIME);
}


void showByteXfer(unsigned long etime)
{
  lcd.clear();
  lcd.print("ByteXfer:");
  lcd.print((int)(etime/(FPS_iter*(10.0*(LCD_COLS*LCD_ROWS+LCD_ROWS)))+0.5));
  lcd.print("uS");
  delay(DELAY_TIME);
}