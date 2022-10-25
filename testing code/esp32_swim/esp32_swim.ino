#include <ESP32Servo.h>

Servo myservo;

const int actuation_range = 60; // [deg] 45 deg is MAX

const int neutral_pos = 86; // [deg]

// Variables
volatile bool swim = false;
int input = 0;

const int servoPin = 21;

// Setup Parameters
void setup() {
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  myservo.setPeriodHertz(50);    // standard 50 hz servo
  myservo.attach(servoPin, 500, 2400); // attaches the servo on pin 18 to the servo object
  
  myservo.write(neutral_pos);

  Serial.begin(9600);
}

// Main Loop Function
void loop() {
  if (swim) {
    swim_cycle(20, 5); // (amplitude [deg], freq [Hz])
  }
  if (Serial.available() > 0) {    // is a character available?
    input = Serial.read(); 
    if (input == 10) {
      switch (swim) {
        case true:
          swim = false;
          break;
        case false:
          swim = true;
          break;
        default:
          Serial.println("STATE ERROR");
      }
    }
    else {
      Serial.println("ERROR");
    }
    Serial.println(input);
  } 
}

/*
 * FUNCTION for one complete swim cycle at given frequency/amplitude
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

/*
 * FUNCTION to flip tail to one side once, then reset to neutral position
 */
void half_flip(int angle, unsigned long MOVING_TIME) {
  move_tail(neutral_pos, neutral_pos+angle, MOVING_TIME);
  delay(5000); // 5 second delay before reset
  move_tail(neutral_pos+angle, neutral_pos, 1000);
}
