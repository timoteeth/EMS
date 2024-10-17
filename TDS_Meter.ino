// TDS ppm Meter
// Author: Timothy Strawn (tstrawn@mines.edu)
// Last Edited: 6/11/24

// Libraries
#include <GravityTDS.h>

// Sensor Setup
#define tdsPin A0
GravityTDS TDS;

// LEDs
const int LEDg = 7;
const int LEDy = 9;
const int LEDo = 11;
const int LEDr = 13;

// Definitions
const int dt = 1000;
float temp = 25.0;
float tdsValue = 0;

void setup() {
  Serial.begin(9600);
  while(!Serial)
  Serial.println();

  TDS.setPin(tdsPin);
  TDS.setAref(5.0);
  TDS.setAdcRange(1024);
  TDS.begin();

  pinMode(LEDg, OUTPUT);
  pinMode(LEDy, OUTPUT);
  pinMode(LEDo, OUTPUT);
  pinMode(LEDr, OUTPUT);

}

void loop() {
  // read temp of water 
  temp = 25;

  TDS.setTemperature(temp);
  TDS.update();
  tdsValue = TDS.getTdsValue();
  Serial.print(tdsValue, 0);
  Serial.println(" ppm");
  updateLEDs();
  delay(dt);

}
void updateLEDs(){
// Safe
  if(tdsValue < 200){
    digitalWrite(LEDg, HIGH);
    digitalWrite(LEDy, LOW);
    digitalWrite(LEDo, LOW);
    digitalWrite(LEDr, LOW);
  }
// Moderate
  if(tdsValue > 200 && tdsValue < 300){
    digitalWrite(LEDg, HIGH);
    digitalWrite(LEDy, HIGH);
    digitalWrite(LEDo, LOW);
    digitalWrite(LEDr, LOW);
  }
// Not Good
  if(tdsValue > 300 && tdsValue < 500){
    digitalWrite(LEDg, HIGH);
    digitalWrite(LEDy, HIGH);
    digitalWrite(LEDo, HIGH);
    digitalWrite(LEDr, LOW);
  }
// Dangerous
  if(tdsValue > 500){
    digitalWrite(LEDg, HIGH);
    digitalWrite(LEDy, HIGH);
    digitalWrite(LEDo, HIGH);
    digitalWrite(LEDr, HIGH);
  }
}
