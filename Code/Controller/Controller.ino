//import wire library for communication
#include <Wire.h>

//declares variable to hold the led pins
int backLed = 6;
int leftLed = 7;
int frontLed = 8;
int rightLed = 9;

//used for changing mode
int button = 2;
int pressed = 0;

//used for debouncing
boolean buttonState = LOW;
volatile boolean buttonFlag;

//code in setup runs once
void setup() {
  pinMode(leftLed, OUTPUT);
  pinMode(rightLed, OUTPUT);
  pinMode(frontLed, OUTPUT);
  pinMode(backLed, OUTPUT);
  attachInterrupt(0, updateMode, RISING);
  Wire.begin(8);
  Wire.onReceive(receiveObjDirection);
  Wire.begin();
  Serial.begin(9600);
}

//code in loop runs forever
void loop() {

  //used when button is pressed
  if (buttonFlag == true) {
    if (debounceButton(buttonState) == LOW) {
      pressed++;
      buttonState = HIGH;
      buttonFlag = false;
      digitalWrite(frontLed, LOW);
      digitalWrite(rightLed, LOW);
      digitalWrite(leftLed, LOW);
      digitalWrite(backLed, LOW);

      Wire.beginTransmission(5);
      Wire.write(pressed % 2);
      Wire.endTransmission();
    }
  }
}

/*
   this is an interupot function which is called when the arduino receives a signal
   it gets the message and turns on and off leds
   depending on the directing that the master sent
   @param number
          number of bytes recieved
*/
void receiveObjDirection(int number) {
  int val = Wire.read();
  if (val == 2) {
    digitalWrite(frontLed, HIGH);
  } else if (val == 20) {
    digitalWrite(frontLed, LOW);
  } else if (val == 3) {
    digitalWrite(rightLed, HIGH);
  } else if (val == 30) {
    digitalWrite(rightLed, LOW);
  } else if (val == 4) {
    digitalWrite(leftLed, HIGH);
  } else if (val == 40) {
    digitalWrite(leftLed, LOW);
  } else if (val == 5) {
    digitalWrite(backLed, HIGH);
  } else if (val == 50) {
    digitalWrite(backLed, LOW);
  }
}

/*
   this is an interrupt function
   runs when user presses a button
*/
void updateMode() {
  buttonFlag = true;
}

//debouncing function
boolean debounceButton(boolean buttonstate) {
  boolean stateNow = digitalRead(button);
  if (buttonState != stateNow) {
    delay(10);
    stateNow = digitalRead(button);
  }
  return stateNow;
}
