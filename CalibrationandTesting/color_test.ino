#include <MeMCore.h>
#define LDRWait 10
#define RGBWait 200

int inputArr[] = {A2, A3};

int red = 0;
int green = 0;
int blue = 0;

float colourArray[3] = {0};
float whiteArray[3] = {719, 675, 693};
float blackArray[3] = {87, 78, 0};
float greyDiff[3] = {632, 597, 693};

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

  for (int i = 1; i <= 3; i++){
    Serial.println(ledArr[i - 1]);
    
    set_color(i);
    delay(RGBWait);
    currentReading = (getAvgReading(5) - blackArray[i - 1])/(greyDiff[i - 1])*255;
    set_color(0);
    printColour(currentReading);
    delay(LDRWait);
 }

  colorResult = detectColor(finalRGB);
  Serial.println(colorResult);
  Serial.println("Experiment Completed.");
}

char detectColor(int RGB[]){
  char res;
  
  if ((RGB[0] >= 310) && (RGB[1] >= 340) && (RGB[2] >= 255)){
    res = 'W'; 
  }
  else if ((RGB[0] >= 315) && (RGB[1] >= 235) && (RGB[2] >= 170)){
    res = 'O';
  }
  else if((RGB[0] >= 305) && (RGB[1] >= 175) && (RGB[2] >= 120)){
    res = 'R';
  }
  else if ((RGB[0] >= 265) && (RGB[1] >= 210) && (RGB[2] >= 185)){
    res = 'P';
  }
   else if ((RGB[0] >= 240) && (RGB[1] >= 280) && (RGB[2] >= 220)){
    res = 'G';
  }
  else if ((RGB[0] >= 238) && (RGB[1] >= 260) && (RGB[2] >= 230)){
    res = 'B';
  }
  else{
    res = 'U';
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
