#include "arduino_secrets.h"
/* 
  Sketch generated by the Arduino IoT Cloud Thing "Untitled"
  https://create.arduino.cc/cloud/things/86e4ac2b-4066-4b8c-bdc2-4c6e88748a00 

  Arduino IoT Cloud Variables description

  The following variables are automatically generated and updated when changes are made to the Thing

  bool swim;

  Variables which are marked as READ/WRITE in the Cloud Thing will also have functions
  which are called when their values are changed from the Dashboard.
  These functions are generated with the Thing and added at the end of this sketch.
*/

// Import Libraries
#include <ESP32Servo.h>
#include "thingProperties.h"

// Create Servo Object
Servo myservo;

// Important Physical System Constants
const int neutral_pos = 86; // [deg] empirically-determined neutral position of the servomotor
const int servoPin = 21;

void setup() {
  // Initialize serial and wait for port to open:
  Serial.begin(9600);
  // This delay gives the chance to wait for a Serial Monitor without blocking if none is found
  delay(1500); 

  // Defined in thingProperties.h
  initProperties();

  // Connect to Arduino IoT Cloud
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);
  
  /*
     The following function allows you to obtain more information
     related to the state of network and IoT Cloud connection and errors
     the higher number the more granular information you’ll get.
     The default is 0 (only errors).
     Maximum is 4
 */
  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();
  
  // Attach and connect to servo
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  myservo.setPeriodHertz(50);    // standard 50 hz servo
  myservo.attach(servoPin, 500, 2400); // attaches the servo on pin 18 to the servo object
  myservo.write(neutral_pos);
}

void loop() {
  ArduinoCloud.update();
  // Your code here 
  if (swim) {
    swim_cycle(20, 2); // (amplitude [deg], freq [Hz])
  }
}



/*
  Since Swim is READ_WRITE variable, onSwimChange() is
  executed every time a new value is received from IoT Cloud.
*/
void onSwimChange()  {
  // Add your code here to act upon Swim change
}

/*
   FUNCTION for one complete swim cycle at given frequency/amplitude
 */
 void swim_cycle(int amplitude, unsigned long freq) {
  unsigned long period = 1000.0 / freq; // [ms]
  Serial.println(period);
  move_tail(neutral_pos, neutral_pos+amplitude, period/4);
  move_tail(neutral_pos+amplitude, neutral_pos-amplitude, period/2);
  move_tail(neutral_pos-amplitude, neutral_pos, period/4);
 }

 /* 
 * FUNCTION to move from position A to B in given time 
 */
void move_tail(int A, int B, unsigned long MOVING_TIME) {
  unsigned long moveStartTime = millis(); // time when start moving
  unsigned long progress = 0;
  long angle;
  
  while (progress <= MOVING_TIME) {
    progress = millis() - moveStartTime;
    angle = map(progress, 0, MOVING_TIME, A, B);
    //Serial.println(angle);
    myservo.write(angle); 
  }
}
