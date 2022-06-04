#include <Keypad.h>
#include <SPI.h>
#include <LiquidCrystal_I2C.h>
#include <avr/sleep.h>
#define interruptPin 2
#define fpPin 0 // Pin input Finger Print

// Setup Keypad
const byte ROWS = 4, COLS = 4;
const char keyPad[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {9,8,7,6}; 
byte colPins[COLS] = {5,4,3,0}; 
Keypad keypad = Keypad(makeKeymap(keyPad), rowPins, colPins, ROWS, COLS); 

const int idleTime = 2000;
int startTime, endTime, currTime;
char key, masterSend;
  
void setup(){
  Serial.begin(9600);
  pinMode(interruptPin,INPUT_PULLUP);
  SPI.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV8);
  digitalWrite(SS, HIGH);
}

void loop(){
  startTime = millis();
  endTime = startTime;
  
  digitalWrite(SS, LOW);

  while(true){
    currTime = endTime - startTime;
    key = keypad.getKey();
    if(key){
      masterSend = key;
      SPI.transfer(masterSend);
      break;
    }
    else{
      if(currTime >= idleTime){
        gonna_sleep();
        break;
      }
    }
    endTime = millis();
  }
  
  digitalWrite(SS, HIGH);
  delay(10);
}

void gonna_sleep(){
   sleep_enable();
   Serial.println("Sleep Mode");
   Serial.println("Zzz");
   attachInterrupt(0, wakeUp, LOW);
   set_sleep_mode(SLEEP_MODE_PWR_DOWN);
   delay(1000);
   sleep_cpu();
   Serial.println("Hi! I'm Awake");
}

void wakeUp(){
   Serial.println("Wake Up Mode");
   sleep_disable();
   detachInterrupt(0);
}
