/*
 A benchmark test from Francisco Malpartida used only for comparing performances
 between libraries.
*/

#include <LiquidCrystalNew_SHR.h>
/*
clock_pin,data_pin,strobe_pin,using a 2xHD44780 chip(1) or not (0)
*/
LiquidCrystalNew_SHR lcd(6,7,8,0);



#define NUM_BENCHMARKS 4
#define ITERATIONS    10
#define LCD_ROWS        2
#define LCD_COLUMNS    20


const byte charBitmap[][8] = {
  {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}
  ,
  {0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x0}
  ,
  {0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x0}
  ,
  {0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x0}
  ,
  {0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x0}
  ,
  {0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x0}
};


typedef long (*t_benchmarkOp)(uint8_t);


typedef struct {
  t_benchmarkOp   benchmark; /**< Function pointer associated to the benchmark */
  long            benchTime; /**< execution time for benchmark 1 in useconds */
  uint16_t        numWrites; /**< Number of write cycles of the benchmark    */
} 
t_benchMarks;


// benchMarks definitions
// ----------------------
extern long benchmark1(uint8_t);
extern long benchmark2(uint8_t);
extern long benchmark3(uint8_t);
extern long benchmark4(uint8_t);

//! @brief benchmark structure that will be initialised and 
static t_benchMarks myBenchMarks[NUM_BENCHMARKS] =
{
  {benchmark1,0,(LCD_ROWS * LCD_COLUMNS) + 2}
  ,
  {benchmark2,0,LCD_ROWS * LCD_COLUMNS *6*2}
  ,
  {benchmark3,0,40 + 2}
  ,
  {benchmark4,0,40 + 2}
};


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

static void LCDLoadCharacters(int numChars){
  for (int i = 0; i < numChars; i++){
    lcd.createChar(i,(uint8_t *)charBitmap[i]);
  }
}


long benchmark1(uint8_t iterations){
  unsigned long time, totalTime = 0;
  int i, j;
  while(iterations > 0){
    lcd.clear();
    time = micros();
    for (i = 0; i < LCD_ROWS; i++){
      lcd.setCursor(0,i);
      for (j = 0; j < LCD_COLUMNS; j++){
        lcd.print(char(5));
      }
    }
    totalTime += (micros() - time);
    delay (200); // it doesn't keep up with the LCD refresh rate.
    iterations--;
  }
  return (totalTime);
}

long benchmark2(uint8_t iterations){
  unsigned long time, totalTime = 0;
  int i, j, k;
  while(iterations > 0){
    lcd.clear();
    time = micros();
    for (i = 0; i < LCD_ROWS; i++){
      for (j = 0;j < LCD_COLUMNS;j++){
        for (k = 0; k <= 5; k++){
          lcd.setCursor(j,i);
          lcd.print(char(k));
        }
      }
    }
    totalTime += (micros() - time);
    iterations--;
  }
  return (totalTime);
}


long benchmark3(uint8_t iterations){
  unsigned long time, totalTime = 0;
  int i;
  while (iterations > 0){
    lcd.clear();
    time = micros();
    for (i = 0; i < LCD_ROWS; i++){
      lcd.setCursor(0,i);
      lcd.print("####################");
    }
    totalTime += (micros() - time);
    delay(200); // it doesn't keep up with the LCD refresh rate.
    iterations--;
  }
  return (totalTime);
}


long benchmark4(uint8_t iterations)
{
  unsigned long time, totalTime = 0;
  int i;
  while(iterations > 0){
    lcd.clear();
    time = micros();
    for (i = 0; i < LCD_ROWS; i++){
      lcd.setCursor(0,i);
      lcd.print("####################");
    }
    totalTime += (micros() - time);
    delay(200); // it doesn't keep up with the LCD refresh rate.
    iterations--;
  }
  return(totalTime);
}

void setup(){
  Serial.begin(38400);
#ifdef __AVR__
  Serial.print("Free mem: ");
  Serial.println(freeMemory());
#endif
  lcd.begin(LCD_COLUMNS,LCD_ROWS);
  lcd.clear();
  LCDLoadCharacters((sizeof(charBitmap)/sizeof(charBitmap[0])));
}


void loop(){
  int i;
  lcd.setCursor ( 0, 0 );
  lcd.clear ( );
  for (i = 0; i < NUM_BENCHMARKS; i++){
    myBenchMarks[i].benchTime = (myBenchMarks[i].benchmark(ITERATIONS))/ITERATIONS;
    Serial.println(i);
  }
  float fAllWrites=0.0;
  for(i = 0; i < NUM_BENCHMARKS; i++){   
    Serial.print("benchmark(");
    Serial.print(i);
    Serial.print("): ");
    Serial.print(myBenchMarks[i].benchTime);
    Serial.print(" us - ");
    Serial.print(" write: ");
    Serial.print(myBenchMarks[i].benchTime/(float)myBenchMarks[i].numWrites);
    Serial.println(" us");
    fAllWrites += myBenchMarks[i].benchTime/(float)myBenchMarks[i].numWrites;
  }
  Serial.print("avg. write: ");
  Serial.println(fAllWrites/(float)NUM_BENCHMARKS);
}
