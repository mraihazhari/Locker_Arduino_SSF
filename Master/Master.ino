#include <Keypad.h>
#include <SPI.h>
#include <LiquidCrystal_I2C.h>
#include <avr/sleep.h>
#define interruptPin 2

//pinInput untuk fingerprint

LiquidCrystal_I2C lcd(0x27,21,4);
const int fp = 0;
int i;
int j;
const byte ROWS = 4, COLS = 4;
const char keyPad[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {9,8,7,6}; 
byte colPins[COLS] = {5,4,3,2}; 
Keypad keypad = Keypad(makeKeymap(keyPad), rowPins, colPins, ROWS, COLS); 
boolean sleep;



void setup(){
  Serial.begin(9600);
   lcd.begin(21,4);
  pinMode(interruptPin,INPUT_PULLUP);
  SPI.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV8);
  digitalWrite(SS, HIGH);
}

void loop(){
  Serial.println("tes");
  char key = keypad.getKey();
  lcd.clear();
  lcd.setCursor(0, 0);
    
  digitalWrite(SS, LOW);

  for(i = 0; i < 1000; i++){
    Serial.println(i);
    char key = keypad.getKey();
    Serial.println(key);
    if(key){
      SPI.transfer(key);
      break;
    }
    else{
      if(i == 999){
        sleep = true;
        SPI.transfer(sleep);
        gonna_sleep();
        
      }
    }
  }
  
  digitalWrite(SS, HIGH);
  delay(10);
}



void gonna_sleep(){
  //melakukan enable ke mode sleep
   sleep_enable();
   Serial.println("Sleep Mode");
   Serial.println("Zzz");
   //menset interrupt
   /*
    * Jika interruptPin bernilai 0
    * maka akan dilakukan interrupt
    */
   attachInterrupt(0, wakeUp, LOW);
   set_sleep_mode(SLEEP_MODE_PWR_DOWN);
   digitalWrite(LED_BUILTIN,LOW);
   delay(1000);
   sleep_cpu();
   Serial.println("Hi! I'm Awake");
   digitalWrite(LED_BUILTIN,HIGH);
}

void wakeUp(){
   Serial.println("Wake Up Mode");
   /*
    * Membangunkan sleep
    */
   sleep_disable();
   detachInterrupt(0);
   /*
    * Mengakhiri interrupt
    */
}