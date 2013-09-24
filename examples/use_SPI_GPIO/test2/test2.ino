/*
	Speed 2 - a experimental test
 */

#define iLCD		// turn on code to calculate speed of "independent" sized display

#define MULTIPLE_HD_CHIPS 0
#define GPIO_ADRS  0x20
#define GPIO_CS  10
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

const byte startC[4] = {
  48,65,97,48};
const byte lenghC[4] = {
  10,26,26,28};
const byte xxx[4] = {
  0,LCD_COLS-1,0,LCD_COLS-1};
const byte yyy[4] = {
  0,0,LCD_ROWS-1,LCD_COLS-1};
  
#include <SPI.h>
#include <LiquidCrystalNew_SPI.h>

LiquidCrystalNew_SPI lcd(GPIO_CS,MULTIPLE_HD_CHIPS,GPIO_ADRS);

void setup()
{
  //Serial.begin(38400);
  lcd.begin(LCD_COLS, LCD_ROWS);

}


void loop()
{

  lcd.clear();
  for (byte i=0;i<LCD_ROWS;i++){
    fillChars(startC[i],lenghC[i],0,i,false,0);
    delay(1000);
  }
  for (byte i=0;i<2;i++){
    lcd.off();
    delay(1000);
    lcd.on();
    delay(1000);
  }
  delay(1000);

  fourCorners();
  delay(4000);

  lcd.clear();
  lcd.autoscroll();
  for (byte i=0;i<LCD_ROWS;i++){
    fillChars(startC[i],lenghC[i],LCD_COLS,i,false,100);
    delay(1000);
  }
  lcd.noAutoscroll();
  delay(1000);


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
  } 
  else {
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

void fillChars(byte stCh,byte leng,byte scol,byte row,boolean efx,int dly){
  //Numbers:           48,10
  //Uppercase letters: 65,26
  //Lowercase letters: 97,26
  if (leng > LCD_COLS) leng = LCD_COLS;
  lcd.setCursor(scol,row);
  uint8_t count = 0;
  for (uint8_t col=0;col<LCD_COLS;col++){
    if (efx){
      dly = 0;
      uint8_t oldCol = lcd.getCursorCol();
      for (uint8_t i=22;i<=57;i++){  
        lcd.setCursor(oldCol,row);
        lcd.write(i);
        delay(5);
      }
      lcd.setCursor(oldCol,row);
      lcd.write(stCh+count);
    } 
    else {
      lcd.write(stCh+count);
    }
    if (count < (leng-1)){
      count++;
    } 
    else {
      count = 0;
    }
    if (dly > 0) delay(dly);
  }
}

void fourCorners(){
  lcd.clear();
  for (byte i=0;i<4;i++){
    lcd.setCursor(xxx[i],yyy[i]);
    lcd.print(i+1,DEC);
  }
}

