// Noise Pollution Monitor V1.1
// Author: Timothy Strawn (tstrawn@mines.edu)
// Edited Last: 6/5/24

// Libraries
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Setting LCD address
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Pins
#define soundSensorPin A0 // Analog pin for sound sensor
#define LEDg 7 // green LED pin
#define LEDy 9 // yellow LED pin
#define LEDo 11 // orange LED pin
#define LEDr 13 // red LED pin

// Variables
const int sampleTime = 50; // time of each sample in milliseconds
unsigned int sample; // current time in milliseconds
const int dt = 125;
float peakToPeak = 0;
float  dbValue = 0;

int dBref = 45; // refereance dB value from phone app
int peakRef = 38; // peaktopeak analog value from sensor at referance dB



void setup() {
  lcd.init();
  lcd.backlight();
  lcd.noCursor();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Noise Pollution");
  lcd.setCursor(0, 1);
  lcd.print("Monitor");
  delay(1500);
  Serial.begin(9600); 

  // Pins
  pinMode(soundSensorPin, INPUT);
  pinMode(LEDg, OUTPUT);
  pinMode(LEDy, OUTPUT);
  pinMode(LEDo, OUTPUT);
  pinMode(LEDr, OUTPUT);

}

void loop() {
  getPeakToPeak();
  getdB();
  updateLCD();
  updateLEDs();
  delay(dt);
}
void getPeakToPeak(){
  unsigned long startMillis = millis(); // start of sample
  peakToPeak = 0;
  unsigned int signalMax = 0;
  unsigned int signalMin = 1024;

  while(millis() - startMillis < sampleTime){ // collect data for 50 ms
    sample = analogRead(soundSensorPin); // sensor reading
    if(sample < 1024){ // throw out spurious readings
      if(sample > signalMax){signalMax = sample;} // save max level
      else if(sample < signalMin){signalMin = sample;} // save min level
    }
  }
  peakToPeak = signalMax - signalMin; // amplitude
  // Serial.print(peakToPeak); Serial.println("ptp");
}
void getdB(){
  dbValue = dBref + 20*log10((float)abs(peakToPeak)/peakRef); // convert to dB
  if(isinf(dbValue)==0){dbValue = dbValue;}
  else{dbValue = 49.9;}
  Serial.print(dbValue); Serial.println(" dB");
}
void updateLCD(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(dbValue);
  lcd.setCursor(4, 0);
  lcd.print(" dB");
  lcd.setCursor(0, 1);
  lcd.write(0xff);
  float j = dbValue;
  j = map(j, 0.0, 110.0, 0.0, 20.0);
  for(float i = 0; i < int(j); i++){
    lcd.setCursor(i, 1);
    lcd.write(0xff);
  } 
}
void updateLEDs(){
  // dB thresholds
  if(dbValue >= 110){ // Dangerous
    digitalWrite(LEDr, HIGH);
    digitalWrite(LEDo, HIGH);
    digitalWrite(LEDy, HIGH);
    digitalWrite(LEDg, HIGH);
    lcd.setCursor(10, 0);
    lcd.print("Unsafe");
    }
  if(dbValue >= 85 && dbValue < 110){ // Loud
    digitalWrite(LEDr, LOW);
    digitalWrite(LEDo, HIGH);
    digitalWrite(LEDy, HIGH);
    digitalWrite(LEDg, HIGH);
    lcd.setCursor(12, 0);
    lcd.print("Loud");
    }
  if(dbValue >= 70 && dbValue < 85){ // Moderate
    digitalWrite(LEDr, LOW);
    digitalWrite(LEDo, LOW);
    digitalWrite(LEDy, HIGH);
    digitalWrite(LEDg, HIGH);
    lcd.setCursor(8, 0);
    lcd.print("Moderate");
    }
  if(dbValue >= 0 && dbValue < 70){ // Safe
    digitalWrite(LEDr, LOW);
    digitalWrite(LEDo, LOW);
    digitalWrite(LEDy, LOW);
    digitalWrite(LEDg, HIGH);
    lcd.setCursor(12, 0);
    lcd.print("Safe");
    }
}