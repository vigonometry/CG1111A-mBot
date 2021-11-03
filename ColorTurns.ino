/**
 * @author: Vigneshwar Hariharan (Lab: B01)
 * @file: ColorTurns.ino
 */
#include <MeMCore.h>
#define LDRWait 10
#define RGBWait 100

//Motor Ports
MeDCMotor left_motor(M1);
MeDCMotor right_motor(M2);

//Signal Pins to modulate voltage at 2-to-4 bit decoder inputs
int inputArr[] = {A2, A3}; 

//Speed of the motors
uint8_t motorSpeed = 255;

//Values were found by calibrating the sensor using White and Black Paper
float whiteArray[3] = {653,854,720}; 
float blackArray[3] = {517, 330, 222};
float greyDiff[3] = {136, 524, 498};

/*
 * This function is used to move the bot forward.
 */
void move_forward(){
  delay(5000);
    left_motor.run(-motorSpeed);
    right_motor.run(motorSpeed);
    delay(500);
    left_motor.stop();
    right_motor.stop();
    delay(2000);
}
/*
 * This function is used to turn the bot 90 degrees to the left
 */
void turn_left(){
  delay(5000);
  left_motor.run(motorSpeed / 2); 
  right_motor.run(motorSpeed / 2);
  delay(500);
  left_motor.stop();
  right_motor.stop();
  delay(2000);
}
/*
 * This function is used to turn the bot 90 degrees to the right 
 */
void turn_right(){
  delay(5000);
  left_motor.run(-motorSpeed / 2); 
  right_motor.run(-motorSpeed / 2);
  delay(500);
  left_motor.stop();
  right_motor.stop();
  delay(2000);
}

/*
 * This function is used to turn the bot 180 degrees on the spot (towards the right)
 */
void u_turn(){
  delay(5000);
  left_motor.run(-motorSpeed / 2);
  right_motor.run(-motorSpeed / 2);
  delay(1000);
  left_motor.stop();
  right_motor.stop();
  delay(2000);
}

/*
 * This function is used to perform to consecutive 90 degree left turns on the bot
 * The bot turns left, moves forward one tile and turns left again
 */
void two_left_turns(){
  delay(5000);
  left_motor.run(motorSpeed / 2); 
  right_motor.run(motorSpeed / 2);
  delay(500);
  left_motor.stop();
  right_motor.stop();
  delay(2000);
  left_motor.run(-motorSpeed);
  right_motor.run(motorSpeed);
  delay(500);
  left_motor.run(motorSpeed / 2); 
  right_motor.run(motorSpeed / 2);
  delay(520);
  left_motor.stop();
  right_motor.stop();
  delay(2000);
}

/*
 * This function is used to perform to consecutive 90 degree right turns on the bot
 * The bot turns right, moves forward one tile and turns right again
 */
void two_right_turns(){
  delay(5000);
  left_motor.run(-motorSpeed / 2); /* value: between -255 and 255. */
  right_motor.run(-motorSpeed / 2);
  delay(542);
  left_motor.stop();
  right_motor.stop();
  delay(2000);
  left_motor.run(-motorSpeed);
  right_motor.run(motorSpeed);
  delay(600);
  left_motor.run(-motorSpeed / 2); /* value: between -255 and 255. */
  right_motor.run(-motorSpeed / 2);
  delay(650);
  left_motor.stop();
  right_motor.stop();
  delay(2000);
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
    for (int j = 0; j < 10; j++){
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
  char res;
  if ((RGB[0] >= 290) && (RGB[1] >= 290) && (RGB[2] >= 290)){
    res = 'W';
  }
  else if ((RGB[0] >= 290) && (RGB[1] >= 185) && (RGB[2] >= 160)){
    res = 'O';
  }
  else if((RGB[0] >= 280) && (RGB[1] >= 110) && (RGB[2] >= 145)){
    res = 'R';
  }
  else if ((RGB[0] >= 240) && (RGB[1] >= 160) && (RGB[2] >= 205)){
    res = 'P';
  }
   else if ((RGB[0] >= 225) && (RGB[1] >= 235) && (RGB[2] >= 260)){
    res = 'G';
  }
  else if ((RGB[0] >= 215) && (RGB[1] >= 220) && (RGB[2] >= 255)){
    res = 'B';
  }
  else{
    res = 'U';
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
  else {
    move_forward();
  }
}


void setup(){
  //setup the OUTPUT pins
  for (int i = 0; i <= 1; i++){
    pinMode(inputArr[i], OUTPUT);
  }
  //Start serial monitor for communication between the LDR and the Arduino Hub of the mBot
  Serial.begin(9600);
}

void loop(){
  char colorResult = readColor();
  Serial.println(colorResult);
  lightWaypoint(colorResult);
  delay(1000);
}
