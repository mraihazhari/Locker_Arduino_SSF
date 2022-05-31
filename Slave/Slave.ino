#include <LiquidCrystal_I2C.h>
#include <SPI.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
volatile boolean process;
volatile char key;

void setup(){
  lcd.begin(16,2);
  lcd.setCursor(0, 0);
  lcd.print("Slave input: ");
  lcd.setCursor(0, 1);
  pinMode(MISO, OUTPUT);
  SPCR |= _BV(SPE);
  process = false;
  SPI.attachInterrupt();
}

ISR(SPI_STC_vect){
  key = SPDR;
  process = true;
}

void loop(){
  if(process){
    process = false;
    lcd.print(key);
  }
}
