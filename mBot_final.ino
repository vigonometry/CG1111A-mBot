/**
 * @author: Vigneshwar Hariharan (Lab: B01)
 * @file: mBot_Final.ino
 */
#include "MeMCore.h"
#include <math.h>
#include <stdbool.h>

//Constants for ColorSensor
#define LDRWait 10
#define PREREAD 20
#define RGBWait 200

//Constants for IR and Ultrasonic Sensor
#define IR_MIN 50

#define IR_MAX 600
#define ULTRA 12

//Constants for Music
#define NOTE_C3  131
#define NOTE_D3  147
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_G3  196
#define NOTE_A3  220
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_D4  294
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_G4  392
#define NOTE_A4  440
#define NOTE_B4  494

//Constants for movement
#define TURNDELAY 200

//Motor Ports
MeDCMotor left_motor(M1);
MeDCMotor right_motor(M2);

//Ultrasonic and IR Sensor Port and Pin
MeUltrasonicSensor ultraSensor(PORT_1);
int IRPin = A0;

//Line Sensor Ports
MeLineFollower lineFinder(PORT_2);

//Buzzer
MeBuzzer buzzer;

//Signal Pins to modulate voltage at 2-to-4 bit decoder inputs
int inputArr[] = {A2, A3}; 

//Speed of motors and parameters for movement
uint8_t motorSpeed = 180;
uint8_t turnSpeed = 255;
uint8_t modulator = 90;
int turnTime_left = 530;
int turnTime_right = 480;
float turnMultiplier = 1.85;

//Values were found by calibrating the sensor using White and Black Paper
float whiteArray[3] = {719, 675, 693}; 
float blackArray[3] = {87, 78, 0};
float greyDiff[3] = {632, 597, 693};

//Music note array (for Melody) - Mortal Kombat Theme
int melody[] = {
NOTE_A3, 
NOTE_A3,
NOTE_C4,
NOTE_A3,
NOTE_D4,
NOTE_A3,
NOTE_E4,
NOTE_D4,
NOTE_C4,
NOTE_C4,
NOTE_E4,
NOTE_C4,
NOTE_G4,
NOTE_C4,
NOTE_E4,
NOTE_C4,
NOTE_G3,
NOTE_G3,
NOTE_B3,
NOTE_G3,
NOTE_C4,
NOTE_G3,
NOTE_D4,
NOTE_C4,
NOTE_F3,
NOTE_F3,
NOTE_A3,
NOTE_F3,
NOTE_C4,
NOTE_F3,
NOTE_C4,
NOTE_B3,
NOTE_A3,
NOTE_A3,
NOTE_C4,
NOTE_A3,
NOTE_D4,
NOTE_A3,
NOTE_E4,
NOTE_D4,
NOTE_C4,
NOTE_C4,
NOTE_E4,
NOTE_C4,
NOTE_G4,
NOTE_C4,
NOTE_E4,
NOTE_C4,
NOTE_G3,
NOTE_G3,
NOTE_B3,
NOTE_G3,
NOTE_C4,
NOTE_G3,
NOTE_D4,
NOTE_C4,
NOTE_F3,
NOTE_F3,
NOTE_A3,
NOTE_F3,
NOTE_C4,
NOTE_F3,
NOTE_C4,
NOTE_B3,
NOTE_A3,
NOTE_A3,
NOTE_A3,
NOTE_A3,
NOTE_G3,
NOTE_C4,
NOTE_A3,
NOTE_A3,
NOTE_A3,
NOTE_A3,
NOTE_G3,
NOTE_E3,
NOTE_A3,
NOTE_A3,
NOTE_A3,
NOTE_A3,
NOTE_G3,
NOTE_C4,
NOTE_A3,
NOTE_A3,
NOTE_A3,
NOTE_A3,
NOTE_G3,
NOTE_E3,
NOTE_A3,
NOTE_A3,
NOTE_A3,
NOTE_A3,
NOTE_A3
};

//Duration of each note in melody
int noteDurations[] = {
  8,8,8,8,8,8,8,8,
  8,8,8,8,8,8,8,8,
  8,8,8,8,8,8,8,8,
  8,8,8,8,8,8,8,8,
  8,8,8,8,8,8,8,8,
  8,8,8,8,8,8,8,8,
  8,8,8,8,8,8,8,8,
  8,8,8,8,8,8,8,8,
  6,6,6,6,6,6,6,6,
  6,6,6,6,6,6,6,6,
  6,6,6,6,6,6,4,8,
  4,6,6,6,6
};

//Sensor Functions
/*
 * This function reads in values from the IR detector and returns the ratio of the current reading against the max reading.
 * 
 * @returns 0 if object is too far away from the wall, 1 if the object is too close to the wall and the reading itself if neither of the above conditions are true.
 */
float readIR(){
  float IR_reading = analogRead(IRPin);
  float reading =  (IR_reading - IR_MIN) / (IR_MAX - IR_MIN);

  if (fabs(reading - 0) < 0.4){
    return 1; //object is too close to the wall
  }
  if (fabs(reading - 0) > 1.0){
    return 0; //object is too far from the wall;
  }
  return reading;
}

/*
 * This function reads in values from the Ultrasonic sensor and returns the ratio of the current reading against the max reading.
 * 
 * @returns 0 if object is too far away from the wall, 1 if the object is too close to the wall and the reading itself if neither of the above conditions are true.
 */
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

/*
 * This function is used to check if any of the sensors present in the line detector are within a black line
 * 
 * @returns true if any of the given sensors are within the black strip, false otherwise.
 */
bool is_black_line(){
  int sensorState = lineFinder.readSensors();
  return sensorState != S1_OUT_S2_OUT;
}

//End of Sensor Functions

//Movement Functions

/*
 * This function is used to move the bot forward.
 */
void moveForward(){
  int left_speed = -(motorSpeed - (modulator * (1 - readIR())));
  int right_speed = motorSpeed - (modulator * (1 - readUltra()));
  left_motor.run(left_speed);
  right_motor.run(right_speed);
}
/*
 * This function is used to move the bot forward during successive turns
 */
void forwardForTurns(){
  left_motor.run(-motorSpeed);
  right_motor.run(motorSpeed);
  delay(600);
  stopMotor();
}
/*
 * This function is used to stop motors
 */
void stopMotor(){
  left_motor.stop();
  right_motor.stop();
}
/*
 * This function is used to turn the bot 90 degrees to the left
 */
void turn_left(){
  delay(TURNDELAY);
  left_motor.run(motorSpeed); 
  right_motor.run(motorSpeed);
  delay(turnTime_left);
  stopMotor();
}

/*
 * This function is used to turn the bot 90 degrees to the right 
 */
void turn_right(){
  delay(TURNDELAY);
  left_motor.run(-motorSpeed); 
  right_motor.run(-motorSpeed);
  delay(turnTime_right);
  stopMotor();
}

/*
 * This function is used to turn the bot 180 degrees on the spot (towards the right)
 */
void u_turn(){
  delay(TURNDELAY);
  left_motor.run(-motorSpeed);
  right_motor.run(-motorSpeed);
  delay(turnTime_right * turnMultiplier);
  stopMotor();
}

/*
 * This function is used to perform to consecutive 90 degree left turns on the bot
 * The bot turns left, moves forward one tile and turns left again
 */
void two_left_turns(){
  delay(TURNDELAY);
  turn_left();
  forwardForTurns();
  delay(TURNDELAY);
  left_motor.run(motorSpeed / 2);
  right_motor.run(motorSpeed/2);
  delay(turnTime_left);
  stopMotor();
}

/*
 * This function is used to perform to consecutive 90 degree right turns on the bot
 * The bot turns right, moves forward one tile and turns right again
 */
void two_right_turns(){
  delay(TURNDELAY);
  turn_right();
  forwardForTurns();
  delay(TURNDELAY);
  left_motor.run(-motorSpeed / 2);
  right_motor.run(-motorSpeed/2);
  delay(turnTime_right);
  stopMotor();
}

/*
 * This function is used to move the bot backwards to get a better color reading.
 */
void move_backward(){
  delay(RGBWait);
  left_motor.run(motorSpeed);
  right_motor.run(-motorSpeed);
  delay(100);
  stopMotor();
}

//End of Movement functions

//Color Detection Functions
/*
 * This function is used to read the color of the paper underneath the bot and return the associated character that can be obtained using the detectColor function.
 * 
 * @returns returns character of colored paper underneath the bot
 */
char readColor(){
  move_backward();
  int finalRGB[3] = {0};
  for (int i = 1; i <= 3; i++){
    set_color(i);
    delay(RGBWait);
    finalRGB[i - 1] = (getAvgReading(5) - blackArray[i - 1])/(greyDiff[i - 1])*255;
    set_color(0);
    delay(LDRWait);
 }
 return detectColor(finalRGB);  
}

/*
 * This function is used to identify the color of the paper present underneath the bot and return the character matching said color.
 * 
 * @param[in] RGB array of colors read in from the color sensor mounted to the bottom of the mBot.
 * @returns character of the color if color is within given list of colors for the Waypoint challenge else 'U' for undefined.
 *
 */
 
char detectColor(int RGB[3]){
  char res;
  if ((RGB[0] >= 230) && (RGB[1] >= 230) && (RGB[2] >= 230)){
    res = 'W';
  }
  else if ((RGB[0] >= 270) && (RGB[1] >= 130) && (RGB[2] >= 105)){
    res = 'O';
  }
  else if((RGB[0] >= 245) && (RGB[1] >= 35) && (RGB[2] >= 45)){
    res = 'R';
  }
  else if ((RGB[0] >= 140) && (RGB[1] >= 205) && (RGB[2] >= 140)){
    res = 'G';
  }
  else if ((RGB[0] >= 145) && (RGB[1] >= 100) && (RGB[2] >= 135)){
    res = 'P';
  }
  else if ((RGB[0] >= 120) && (RGB[1] >= 200) && (RGB[2] >= 205)){
    res = 'B';
  }
  return res; 
}

/*
 * This function is used to set the color of the LED by modulating input signals at pin A2 and A3 (2A and 2B on decoder).
 * 
 * @param[in] i Integer used within loop to alternate color from blank to red to green to blue
 */
void set_color(int i){
    if (i == 0){
      digitalWrite(A2, LOW);
      digitalWrite(A3, LOW);
    }
    if (i == 1){
      digitalWrite(A2, HIGH);
      digitalWrite(A3, HIGH);
    }
    if (i == 2){
      digitalWrite(A2, HIGH);
      digitalWrite(A3, LOW);
    }
    if(i == 3){
      digitalWrite(A2, LOW);
      digitalWrite(A3, HIGH);
    }
}

/*
 * This function is used to perform the movement corresponding to the color of the paper underneath it should a black line be detected.
 * 
 * @param[in] color The character of color identified by the color sensor mounted to the bottom of the mBot.
 */
void lightWaypoint(char color){
  if (color == 'R'){
    turn_left();
  }
  else if (color == 'G'){
    turn_right();
  }
  else if (color == 'O'){
    u_turn();
  }
  else if (color == 'P'){
    two_left_turns();
  }
  else if (color == 'B'){
    two_right_turns();
  } 
  else if (color == 'W'){
    play();
  }
}

/*
 * This function reads in values analog pin A1 (the LDR) and returns the average RGB readings depending on the number of times specified by the user.
 * 
 * @returns average reading of the LDR for each RGB component.
 * @pre times >= 1
 */
int getAvgReading(int times){      
//find the average reading for the requested number of times of scanning LDR
  int reading;
  int total =0;
//take the reading as many times as requested and add them up
  for(int i = 0;i < times;i++){
     reading = analogRead(A1);
     total = reading + total;
     delay(LDRWait);
  }
//calculate the average and return it
  return total/times;
}
// End of Color Detection functions 

//Music functions

/*
 * This function is used to play the music or melody specified in the global melody array by varying the frequency of the mBot Buzzer and timing at which it is played.
 */
void play()
{
    for (int thisNote = 0; thisNote < 93; thisNote++) {
    int noteDuration = 625 / noteDurations[thisNote];
    buzzer.tone(8, melody[thisNote],noteDuration);
    int pauseBetweenNotes = noteDuration * 1.20;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    buzzer.noTone(8);
  }
}

//End of music functions

void setup() {
  //setup the OUTPUT pins
  for (int i = 0; i <= 1; i++){
    pinMode(inputArr[i], OUTPUT);
  }
  //Start serial monitor for communication between the LDR and the Arduino Hub of the mBot
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(3000);
  while(!is_black_line()){
    moveForward();
  }
  delay(PREREAD);
  stopMotor();
  char color = readColor();
  Serial.println(color);
  lightWaypoint(color);
}