//imports library to control servo
#include <Servo.h>
//imports library to start master slave communication
#include <Wire.h>

volatile int mode = 0;

int numOfServos = 4;
Servo servos[4];
int infaredSensors[4] = {A0, A1, A2, A3};
int servoPins[4] = {2, 3, 4, 5};

//code in setup runs once
void setup() {

  //attaches each servo to a pin
  for (int i = 0; i < numOfServos ; i++) {
    servos[i].attach(servoPins[i]);
  }

  //used to send data about the direction of an object
  Wire.begin();
  //used to recieve data about the current mode the user has selected
  Wire.begin(5);
  //interupt to run when it recieves data from the master
  Wire.onReceive(receiveMode);
}

//code in loop runs forever
void loop() {
  /*
     for loop loops through all the sensors,
     measures the distance of an object
     sends signal to controller or move servos depending on the mode
  */
  for (int i = 0; i < numOfServos; i++) {
    infaredSensor(infaredSensors[i], servos[i], servoPins[i]);
    delay(100);
  }
}

/*
   method calculates the distace of an object from infrared sensor
   either changes position of servo
   or sends a signal to the controll arduino depending on the mode

   @param irSensor
          pin of the infrared sensor
   @param servo
          the servo to be moved
   @param servoPin
          the pin where the servo is connected
*/
void infaredSensor(int irSensor, Servo servo, int servoPin) {
  //gets the analog voltage from the pin
  int irval = analogRead(irSensor);
  delayMicroseconds(10);
  int irval2 = analogRead(irSensor);

  //converts the analog value to distance in centimetres
  int distance = 26 * 1024 / (5 * irval) ;
  int distance2 = 26 * 1024 / (5 * irval2) ;

  int meanDistance = (int)((distance + distance2) / 2);

  if (mode == 1) {
    if (meanDistance < 30 && (distance - distance2) < 20) {
      servo.write(90);
      delay(50);
    } else {
      servo.write(10);
      delay(50);
    }
  } else {
    /* sends data to the controller.
         if there is nothing in front of sensor it sends the pinnumber * 10
         if there is something in front of sensor it sends the pinnumber
    */
    if (meanDistance < 30 && (distance - distance2) < 20) {
      Wire.beginTransmission(8);
      Wire.write(servoPin);
      Wire.endTransmission();
    } else {
      Wire.beginTransmission(8);
      Wire.write(servoPin * (10));
      Wire.endTransmission();
    }
  }
}


/*
   this is an interupot function which is called when the arduino receives a signal
   it gets the message recieved and sets the mode variable to the mode beign sent
   @param number
          number of bytes recieved

*/
void receiveMode(int number) {
  //gets integer sent
  int val = Wire.read();

  if (val == 0) {
    mode = 0;
    for (int i = 0; i < numOfServos ; i++) {
      servos[i].write(10);
    }
  } else {
    mode = 1;
  }
}
