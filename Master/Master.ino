#include <Keypad.h>
#include <SPI.h>

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

void setup(){
  SPI.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV8);
  digitalWrite(SS, HIGH);
}

void loop(){
  char key = keypad.getKey();
  digitalWrite(SS, LOW);
  if(key) SPI.transfer(key);
  digitalWrite(SS, HIGH);
  delay(10);
}
