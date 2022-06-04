#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <Servo.h>

Servo myservo;


int pos = 0;
int lock = 7;
int startime;
int endtime;
boolean lock_status;

LiquidCrystal_I2C lcd(0x27,21,4);
volatile boolean process;
volatile boolean masterMode;
volatile boolean canInput;
volatile char key;
const String password;
volatile String buff;

void setup(){
  lcd.begin(21,4);
  lcd.setCursor(0, 0);
  lcd.print("Slave input: ");
  lcd.setCursor(0, 1);
  lcd.print("--> ");
  pinMode(MISO, OUTPUT);
  SPCR |= _BV(SPE);
  process = false;
  canInput = true;
  password = "123";
  buff = "";
  masterMode = false;
  SPI.attachInterrupt();
  myservo.attach(9);
  pinMode(lock, INPUT);
}

ISR(SPI_STC_vect){
  key = SPDR;
  process = true;
}

void loop(){
  // Authentication Mode
  
  if(process && !masterMode && canInput){
    process = false;
    lcd.print(key);
    buff += key;
    if(buff.length() == password.length()){
      // Correct Password
      if(buff == password){
        masterMode = true;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("C O R R E C T");
        lcd.setCursor(0, 1);
        lcd.print("P A S S W O R D");
        delay(300);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Welcome, Master");
        delay(150);
        lcd.setCursor(0, 1);
        lcd.print("Should I execute");
        lcd.setCursor(0, 2);
        lcd.print("Operation Overdrive?");
        lcd.setCursor(0, 3);
        lcd.print("--> ");
        buff = "";
        startime = millis();
        lock_status = false;
        for(pos = 90; pos <= 180 ; pos ++){
          myservo.write(pos);
          delay(10);
        }
      }
      // Incorrect Password
      else{
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("I N C O R R E C T");
        lcd.setCursor(0, 1);
        lcd.print("P A S S W O R D");
        delay(300);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Intruder detected");
        delay(150);
        lcd.setCursor(0, 1);
        lcd.print("Initiating");
        lcd.setCursor(0, 2);
        lcd.print("Operation Bloodshed");
        delay(300);
        lcd.setCursor(0, 3);
        lcd.print("See you in hell");
        buff = "";
      }
    }
  }
  // Master Mode
  if(process && masterMode && canInput){
    process = false;
    lcd.print(key);
    
    // Set of instructions ...
  }

  if((digitalRead(lock) == HIGH || (endtime - startime) >= 360000) && lock_status == false){
     for(pos = 180; pos >= 90 ; pos --){
          myservo.write(pos);
          lock_status = true;
          delay(10);
      }
  }

  endtime = millis();
}
