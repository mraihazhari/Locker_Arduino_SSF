#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <Servo.h>
#define lockPin 7

Servo myservo;

LiquidCrystal_I2C lcd(0x27,21,4);
volatile boolean receivedInput = false;
boolean loggedIn = false;
const String password = "123";
volatile String buff = "";
volatile char slaveReceive; // key dari master
int startTime, endTime, currTime;

void setup(){
  lcd.begin(21,4);
  lcd.setCursor(0, 0);
  lcd.print("Locker input: ");
  lcd.setCursor(0, 1);
  lcd.print("--> ");
  pinMode(MISO, OUTPUT);
  SPCR |= _BV(SPE);
  SPI.attachInterrupt();
  myservo.attach(9);
  pinMode(lockPin, INPUT);
}

ISR(SPI_STC_vect){
  slaveReceive = SPDR;
  receivedInput = true;
}

void loop(){
  const int lockTime = 3000;
  
  if(receivedInput){
    receivedInput = false;
    lcd.print('*');
    buff += slaveReceive;
    if(buff.length() == password.length()){
      if(buff == password){
        loggedIn = true;
        buff = "";
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("C O R R E C T");
        lcd.setCursor(0, 1);
        lcd.print("P A S S W O R D");
        startTime = millis();
        // Ini ceritanya buka servo krn dari 90 sampe 180
        for(int pos = 90; pos <= 180 ; pos++){
          myservo.write(pos);
          delay(10);
        }
        lcd.clear();
        lcd.print("Welcome, sir");
      }
      else{
        buff = "";
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("I N C O R R E C T");
        lcd.setCursor(0, 1);
        lcd.print("P A S S W O R D");
        delay(300);
        lcd.clear();
        lcd.print("Redirecting...");
        delay(300);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Locker input: ");
        lcd.setCursor(0, 1);
        lcd.print("--> ");
      }
    }
  }

  // Loker akan terkunci apabila
  // 1. Tombol lock di tekan atau
  // 2. Waktu berlalu sudah melebihi limit waktu lock
  
  currTime = endTime - startTime;
  if(loggedIn && (digitalRead(lockPin) == HIGH || currTime >= lockTime)){
    loggedIn = false;
    // Ini ceritanya tutup servo krn dari 180 sampe 90
    for(int pos = 180; pos >= 90; pos--){
      myservo.write(pos);
      delay(10);
    }
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Locker input: ");
    lcd.setCursor(0, 1);
    lcd.print("--> ");
  }
  
  endTime = millis();
}
