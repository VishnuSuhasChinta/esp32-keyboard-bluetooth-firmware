#include "pins.h"
#include <BleKeyboard.h>

BleKeyboard bleKeyboard("SS_V1", "VSC", 100);



char keymap[5][3] = {
  {'q','a','z'},
  {'w','s','x'},
  {'e','d','c'},
  {'r','f','v'},
  {'t','g','b'}
};

bool keyState[5][3] = {false};
bool m1State = false;
bool m2State = false;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  bleKeyboard.begin();

  pinMode(r1, INPUT_PULLUP);
  pinMode(r2, INPUT_PULLUP);
  pinMode(r3, INPUT_PULLUP);
  pinMode(m1, INPUT_PULLUP);
  pinMode(m2, INPUT_PULLUP);


  pinMode(c1, OUTPUT);
  pinMode(c2, OUTPUT);
  pinMode(c3, OUTPUT);
  pinMode(c4, OUTPUT);
  pinMode(c5, OUTPUT);




  digitalWrite(c1, HIGH);
  digitalWrite(c2, HIGH);
  digitalWrite(c3, HIGH);
  digitalWrite(c4, HIGH);
  digitalWrite(c5, HIGH);

  
}



void loop() {
  // put your main code here, to run repeatedly:
  if(!bleKeyboard.isConnected()){ //but i need it to retry not just DIE
    delay(50);
    return;
  }


  for(int col = 0; col<5; col++){
    //write the column low
    digitalWrite(columnPins[col],LOW);
    //small delay
    delayMicroseconds(50); //why this number specifically
    
    //scan the rows
    for(int row = 0; row<3; row++){
      bool isPressed = (digitalRead(rowPins[row]) == LOW);

      if(isPressed && !keyState[col][row]){//if somethings pressed and nothings been pressed aready then send a stroke
        bleKeyboard.press(keymap[col][row]);
        Serial.write(keymap[col][row]);
        delay(1);
      }else if(!isPressed && keyState[col][row]){
        bleKeyboard.release(keymap[col][row]); //does the press itself handle it or something? how do i get multiple keystrokes like a real keyboard?
        delay(1);
      }

      keyState[col][row] = isPressed;
    }
    //write the column back high
    digitalWrite(columnPins[col], HIGH);



    
    bool m1Pressed = (digitalRead(m1) == LOW);
    if (m1Pressed && !m1State) {
      bleKeyboard.press(KEY_LEFT_CTRL); // swap for whatever m1 actually should be
      Serial.println('1');
    } else if (!m1Pressed && m1State) {
      bleKeyboard.release(KEY_LEFT_CTRL);
    }
    m1State = m1Pressed;


    bool m2Pressed = (digitalRead(m2) == LOW);
    if (m2Pressed && !m2State) {
      bleKeyboard.press(KEY_LEFT_SHIFT); // same, placeholder
      Serial.println('2');
    } else if (!m2Pressed && m2State) {
      bleKeyboard.release(KEY_LEFT_SHIFT);
    }
    m2State = m2Pressed;


    //add a delay
    delay(5);
  }
}