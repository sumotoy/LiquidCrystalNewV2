static char dummyvar; // dummy declaration for STUPID IDE!!!!

#define LCD_COLS 20
#define LCD_ROWS 2

#define FPS_iter 1 	// number of iterations to repeat each "frame" within the test 
					// (current frame test is 10 full display frames, 1 for each digits 0-9)
					// FPS_iter like 100 will allow the frames to be seen
					// Note: the only reason other than visual to make this larger than 1
					// might be to compensate for Async serial buffering should a serial interface be tested
					// even with 1 iteration, 340 bytes are written for a 16x2 display
					// bytes written = FPS_iter * ((LCD_ROWS * LCD_COLS) + LCD_ROWS) * 10

#define iLCD		// turn on code to calculate speed of "independent" sized display
#define iLCD_ROWS 2 // independent FPS row size
#define iLCD_COLS 20 // independent FPS col size


#define DELAY_TIME 3500 // delay time to see information on lcd

#include <SPI.h>
#include <DigitalIO.h>
#include <LiquidCrystalHSPIF.h>


LiquidCrystalHSPIF lcd(10,0,0x20);

void setup()
{
	// set up the LCD's number of columns and rows: 
	lcd.begin(LCD_COLS, LCD_ROWS);
}

void loop()
{
unsigned long etime;
char buf[8];

	lcd.clear();
	etime = timeFPS(FPS_iter, LCD_COLS, LCD_ROWS);
	showByteXfer(etime);
	sprintf(buf, "%dx%d", LCD_COLS, LCD_ROWS);
	showFPS(etime, buf);
#ifdef iLCD
	etime = etime *iLCD_ROWS * iLCD_COLS / LCD_ROWS / LCD_COLS;
	sprintf(buf, "%dx%d", iLCD_COLS, iLCD_ROWS);
	showFPS(etime, buf);
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
void showFPS(unsigned long etime, const char *type)
{
float fps;
	fps = (10.0 * FPS_iter) * 1000000.0/(etime);
	lcd.clear();
	lcd.print(type);
	lcd.print("FPS: ");
	lcd.print(fps);

	if(LCD_ROWS > 1)
	{
		lcd.setCursor(0,1);
	}
	else
	{
		delay(DELAY_TIME);
		lcd.clear();
	}
	lcd.print("Ftime: ");
	lcd.print((etime)/10.0/FPS_iter/1000);
	lcd.print("ms");

	delay(DELAY_TIME);
}
void showByteXfer(unsigned long etime)
{
	lcd.clear();
	lcd.print("ByteXfer: ");
	lcd.print((int) (etime / (FPS_iter * (10.0 * (LCD_COLS *  LCD_ROWS + LCD_ROWS)))+0.5));
	lcd.print("uS");

	delay(DELAY_TIME);
}