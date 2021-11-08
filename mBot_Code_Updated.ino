/**
 * @author: Vigneshwar Hariharan (Lab: B01)
 * @file: mBot_Code.ino
 */
#include "MeMCore.h"
#include <math.h>

//Constants for ColorSensor
#define LDRWait 100
#define RGBWait 100

//Constants for IR and Ultrasonic Sensor
#define IR_MIN 47
#define IR_MAX 320
#define ULTRA 11

//Constants for Music
#define NOTE_D4  294
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_G4  392
#define NOTE_A4  440
#define NOTE_B4  494

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

//Speed of the motors
uint8_t motorSpeed = 200;
uint8_t turnSpeed = 255;
uint8_t modulator = 100;

//Values were found by calibrating the sensor using White and Black Paper
float whiteArray[3] = {929,906, 143}; 
float blackArray[3] = {693, 636, 0};
float greyDiff[3] = {236, 270, 143};

//Music note array (for Melody)
// notes in the melody:
int melody[] = {
NOTE_D4,//5  
0,//5
NOTE_A4,//6
0,//5
NOTE_A4,//1.
NOTE_G4,
NOTE_A4,
0,
NOTE_G4,
0,
NOTE_F4,//5
NOTE_G4,//5
0,//6
NOTE_G4,//5
NOTE_F4,//2.
NOTE_D4,//1.
NOTE_F4,
NOTE_D4,
0,
NOTE_A4,//6
0,//5
NOTE_A4,//1.
NOTE_G4,
NOTE_A4,
NOTE_A4,
0,
NOTE_G4,
NOTE_F4,
NOTE_G4,
0,
NOTE_G4,
NOTE_F4,
NOTE_D4,
NOTE_F4,
0,
NOTE_D4,
0,
NOTE_A4,
0,
};

//Duration of each note in melody
int noteDurations[] = {
  4,8,4,8,4,4,
  4,
  8,4,4,4,8,4,
  4,
  4,4,8,4,8,4,4,
  4,
  8,4,4,4,8,4,
  4,4,4,4,4,
  12,4,8,4,8
};

float readIR(){
  float IR_reading = analogRead(IRPin);
  float reading = (IR_reading - IR_MIN) / (IR_MAX - IR_MIN);
  if (fabs(reading - 0) < 0.4){
    return 0; //object is too close to the wall
  }
  if (fabs(reading - 0) > 1.0){
    return 1; //object is too far from the wall;
  }
  return reading;
}

float readUltra(){
  float reading = ultraSensor.distanceCm();
  reading = reading / ULTRA;

  if (fabs(reading - 0) < 0.7){
    return 0; //object is close to the wall
  }
  if (fabs(reading - 0) > 1.0){
    return 1; //object is too far from the wall;
  }
  return reading;
}

bool is_black_line(){
  int sensorState = lineFinder.readSensors();
  return sensorState != S1_OUT_S2_OUT;
}

void play()
{
    for (int thisNote = 0; thisNote < 37; thisNote++) {
    int noteDuration = 625 / noteDurations[thisNote];
    buzzer.tone(8, melody[thisNote],noteDuration);
    int pauseBetweenNotes = noteDuration * 1.20;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    buzzer.noTone(8);
  }
}

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
 * This function is used to turn the bot 90 degrees to the left
 */
void turn_left(){
  delay(RGBWait);
  left_motor.run(turnSpeed / 2); 
  right_motor.run(turnSpeed / 2);
  delay(575);
  left_motor.stop();
  right_motor.stop();
}

/*
 * This function is used to turn the bot 90 degrees to the right 
 */
void turn_right(){
  delay(RGBWait);
  left_motor.run(-turnSpeed / 2); 
  right_motor.run(-turnSpeed / 2);
  delay(600);
  left_motor.stop();
  right_motor.stop();
}

/*
 * This function is used to turn the bot 180 degrees on the spot (towards the right)
 */
void u_turn(){
  delay(RGBWait);
  left_motor.run(-turnSpeed/2);
  right_motor.run(-turnSpeed/2);
  delay(1200);
  left_motor.stop();
  right_motor.stop();
}

/*
 * This function is used to perform to consecutive 90 degree left turns on the bot
 * The bot turns left, moves forward one tile and turns left again
 */
void two_left_turns(){
  delay(RGBWait);
  left_motor.run(turnSpeed / 2); 
  right_motor.run(turnSpeed / 2);
  delay(600);
  left_motor.stop();
  right_motor.stop();
  delay(RGBWait);
  left_motor.run(-turnSpeed / 2);
  right_motor.run(turnSpeed / 2);
  delay(1000);
  left_motor.run(turnSpeed / 2); 
  right_motor.run(turnSpeed / 2);
  delay(600);
  left_motor.stop();
  right_motor.stop();
}

/*
 * This function is used to perform to consecutive 90 degree right turns on the bot
 * The bot turns right, moves forward one tile and turns right again
 */
void two_right_turns(){
  delay(RGBWait);
  left_motor.run(-turnSpeed / 2); /* value: between -255 and 255. */
  right_motor.run(-turnSpeed / 2);
  delay(542);
  left_motor.stop();
  right_motor.stop();
  delay(1000);
  left_motor.run(-turnSpeed / 2);
  right_motor.run(turnSpeed / 2);
  delay(1250);
  left_motor.run(-turnSpeed / 2); /* value: between -255 and 255. */
  right_motor.run(-turnSpeed / 2);
  delay(550);
  left_motor.stop();
  right_motor.stop();
}

void move_backward(){
  delay(RGBWait);
  left_motor.run(motorSpeed);
  right_motor.run(-motorSpeed);
  delay(100);
  left_motor.stop();
  right_motor.stop();
}

/*
 * This function is used to read the color of the paper underneath the bot and return the associated character that can be obtained using the detectColor function.
 * 
 * @returns returns character of colored paper underneath the bot
 */
char readColor(){
  int finalRGB[3] = {0};
  int currentReading;
  for (int i = 1; i <= 3; i++){
    set_color(i);
    for (int j = 0; j < 7; j++){
      currentReading = (analogRead(A1) - blackArray[i])/(greyDiff[i])*255;
      if (j == 5){
        finalRGB[i - 1] = currentReading;
      }
      delay(RGBWait);
  }
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
  char res = 'W';
  if (RGB[0] >= 310){
    res = 'W';
  }
  else if ((RGB[0] >= 295 && RGB[0] <= 310) && (RGB[1] >= 1500) && (RGB[2] >= 290)){
    res = 'O';
  }
  else if((RGB[0] >= 280) && (RGB[1] >= 1300) && (RGB[2] >= 250)){
    res = 'R';
  }
  else if ((RGB[0] >= 240) && (RGB[1] >= 1550) && (RGB[2] >= 310)){
    res = 'G';
  }
  else if ((RGB[0] >= 230) && (RGB[1] >= 1500) && (RGB[2] >= 310)){
    res = 'B';
  }
  else if ((RGB[0] >= 220) && (RGB[1] >= 1300) && (RGB[2] >= 250)){
    res = 'P';
  }
  else{
    res = 'W';
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
  } else if (color == 'W'){
    play();
  }
}


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
  delay(20);
  left_motor.stop();
  right_motor.stop();
  delay(20);
  move_backward();
  char color = readColor();
  lightWaypoint(color);
}
