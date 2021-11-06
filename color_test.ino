#include <MeMCore.h>
#define LDRWait 10
#define RGBWait 200

int inputArr[] = {A2, A3};

int red = 0;
int green = 0;
int blue = 0;

float colourArray[3] = {0};
float whiteArray[3] = {0};
float blackArray[3] = {0};
float greyDiff[3] = {0};

void setup(){
  char ledArr[4][6] = {"Red", "Green", "Blue"};
  int finalRGB[3] = {0};
  int currentReading;
  char colorResult;
  
  //setup the OUTPUT pins
  for (int i = 0; i <= 1; i++){
    pinMode(inputArr[i], OUTPUT);
  }

  Serial.begin(9600);
  setBalance();

  for (int i = 1; i <= 3; i++){
    Serial.println(ledArr[i - 1]);
    
    set_color(i);
    for (int j = 0; j < 10; j++){
      currentReading = (analogRead(A1) - blackArray[i])/(greyDiff[i])*255;
      printColour(currentReading);
      if (j == 5){
        finalRGB[i - 1] = currentReading;
      }
      delay(RGBWait);
  }
  set_color(0);
  delay(2000);
 }

  colorResult = detectColor(finalRGB);
  Serial.println(colorResult);
  Serial.println("Experiment Completed.");
}

char detectColor(int RGB[]){
  char res = '\0';
  if ((RGB[0] > 300) && (RGB[1] > 215) && (RGB[2] > 110)){
    res = 'O';
  }
  else if((RGB[0] > 300) && (RGB[1] > 150) && (RGB[2] > 120)){
    res = 'R';
  }
  else if ((RGB[0] > 255) && (RGB[1] > 290) && (RGB[2] > 220)){
    res = 'G';
  }
  else if ((RGB[0] > 245) && (RGB[1] > 270) && (RGB[2] > 225)){
    res = 'B';
  }
  else if ((RGB[0] > 260) && (RGB[1] > 180) && (RGB[2] > 170)){
    res = 'P';
  }
  return res; 
}

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

void setBalance(){
//set white balance
  Serial.println("Put White Sample For Calibration ...");
  delay(5000);     //delay for five seconds for getting sample ready
//scan the white sample.
//go through one colour at a time, set the maximum reading for each colour -- red, green and blue to the white array
  for(int i = 1; i<= 3; i++){
     set_color(i);
     delay(RGBWait);
     whiteArray[i - 1] = getAvgReading(5); //scan 5 times to take the average, 
     set_color(0);
     delay(2000);
  }
//done scanning white, time for the black sample.
  set_color(0);
//set black balance
  Serial.println("Put Black Sample For Calibration ...");
  delay(5000);     //delay for five seconds for getting sample ready 
//go through one colour at a time, set the minimum reading for red, green and blue to the black array
  for(int i = 1;i<=3;i++){
     set_color(i);
     delay(RGBWait);
     blackArray[i -1] = getAvgReading(5);
     set_color(0);
     delay(2000);
//the differnce between the maximum and the minimum gives the range
     greyDiff[i -1] = whiteArray[i -1] - blackArray[i - 1];
  }

  //delay another 5 seconds to allow us to prepare the colour objects
  set_color(0);
  Serial.println("Put Colour Sample For Data Collection  ...");
  delay(5000); //get ready the colour paper
}

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

void printColour(int reading){       //prints the colour in the colourArray on the Serial Monitor
  Serial.print(millis());
  Serial.print(",");
  Serial.println(reading);
}

void loop(){
}
