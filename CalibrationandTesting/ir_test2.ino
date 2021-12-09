#include <math.h>
#include "MeMCore.h"

#define IR_MIN 47
#define IR_MAX 320
#define ULTRA 5

MeUltrasonicSensor ultraSensor(PORT_1);
MeDCMotor left_motor(M1);
MeDCMotor right_motor(M2);

uint8_t motorspeed = 250;
uint8_t modulator = 100;

int IRPin = A0;

float readIR(){
  float IR_reading = analogRead(IRPin);
  float reading =  ((float)IR_reading - IR_MIN) / (IR_MAX - IR_MIN);

  if (fabs(reading - 0) < 0.5){
    return 1; //object is too close to the wall
  }
  if (fabs(reading - 0) > 1.0){
    return 0; //object is too far from the wall;
  }
  return reading;
}

float readUltra(){
  float reading = ultraSensor.distanceCm();
  reading = (reading - ULTRA) / ULTRA;

  if (fabs(reading - 0) < 0.5){
    return 1;
  }
  if (fabs(reading - 0) > 1.0){
    return 0;
  }
  return reading;
}


void moveForward(){
  int left_speed = -(motorspeed - (modulator * (1 - readUltra())));
  int right_speed = motorspeed - (modulator * (1 - readIR()));
  left_motor.run(left_speed);
  right_motor.run(right_speed);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  moveForward();
}
