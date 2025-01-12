#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"

const int RIGHT = 14;
const int FRONT = 15;
const int LEFT = 16;
const int POWER_BUTTON = 5; // push button for on/off switch

bool isPoweredOn = false; 
bool counterClockwise = true;

Adafruit_MotorShield AFMS = Adafruit_MotorShield();
//initializing motor pins (DO NOT need to initialize motor pins as outpuds using pinMode if using the adafruit libraries)
Adafruit_DCMotor *leftMotor = AFMS.getMotor(3); //  left motor is on M3
Adafruit_DCMotor *rightMotor = AFMS.getMotor(4); //  right motor is on M4

void setup () {
	Serial.begin(9600); // if we use serial monitor for testing
	
  AFMS.begin();
  leftMotor->setSpeed(150);
  rightMotor ->setSpeed(150);
  
	pinMode (RIGHT, INPUT);
	pinMode (FRONT, INPUT);
  pinMode (LEFT, INPUT);
	pinMode (POWER_BUTTON, INPUT_PULLUP);
}

void loop () {

  // leftMotor->run(FORWARD); // test motor
  // rightMotor->run(FORWARD); // test motor

  //serial.println(digitalRead(FRONT)); // test the IR sensors (1 on table, 0 off table)

	int powerSignal = digitalRead(POWER_BUTTON);
	
	if (powerSignal == LOW) {
		isPoweredOn = true;
		delay(500); //debounce delay
	}
	
	if (isPoweredOn) {

    int l = digitalRead(LEFT);
    int f = digitalRead(FRONT);
    int r = digitalRead(RIGHT);

    if (l == HIGH && f == HIGH && r == LOW) { //position: bttm right corner
      leftMotor->run(FORWARD);
      rightMotor->run(FORWARD);
    }
    else if (l == HIGH && f == LOW && r == LOW) { //position: top right corner
      if (counterClockwise) {
        LFL();
      }
      else {
        RFR();
      }
      counterClockwise = !counterClockwise; // toggle direction
    }
    else if (l == HIGH && f == HIGH && r == HIGH) { //position: finished LFL or RFR turn. Not on final pass
      leftMotor->run(FORWARD);
      rightMotor->run(FORWARD);
    }
    else if(l == HIGH && f == LOW && r == HIGH) { //position: at edge, must turn CW or CCW, will be opposite of direction turned last time
      if (counterClockwise) {
        LFL();
      }
      else {
        RFR();
      }
      counterClockwise = !counterClockwise; // toggle direction
    }
    else if (l == HIGH && f == HIGH && r == LOW) { //position: on its final pass to the far corner
      leftMotor->run(FORWARD);
      rightMotor->run(FORWARD);
    }
    else if (l == HIGH && f == LOW && r == LOW) { //position: at far corner, STOP
      leftMotor->run(RELEASE);
      rightMotor->run(RELEASE);
    }
	}
}

//COUNTER CLOCKWISE TURN
void LFL () {
  leftMotor->run(RELEASE);
  rightMotor->run(RELEASE);
  delay(500); //debounce delay for stability

  leftMotor->run(FORWARD);
  rightMotor->run(BACKWARD);
  delay(3000); //adjust delay to achieve a 180 degree CCW turn
}

//CLOCKWISE TURN
void RFR () {
  leftMotor->run(RELEASE);
  rightMotor->run(RELEASE);
  delay(500); //debounce delay for stability

  leftMotor->run(BACKWARD);
  rightMotor->run(FORWARD);
  delay(3000); //adjust delay to achieve a 180 degree CW turn
}
