// Air/Sound pollution monitor
// Author: Timothy Strawn
// Edited Last: 5/28/24

// Libraries
#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// DHT Sensor setup
#define DHTPIN 4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// LCD Setup
LiquidCrystal_I2C lcd(0x27, 20, 4);

// Pins
#define soundSensor A2
#define airSensor A0
const int LEDg = 12;
const int LEDy = 10;
const int LEDo = 8;
const int LEDr = 6;

// Definitions
const int sampleTime = 50; // time of each sample in milliseconds
unsigned int sample; // current time in milliseconds
int airRead = 0;
int dt = 300; // delay time ms

void setup() {
  lcd.init();
  lcd.backlight();
  lcd.noCursor();

  Serial.begin(9600);
  while (!Serial);
  Serial.println();
  dht.begin();

  pinMode(soundSensor, INPUT);
  pinMode(airSensor, INPUT);
  pinMode(LEDg, OUTPUT);
  pinMode(LEDy, OUTPUT);
  pinMode(LEDo, OUTPUT);
  pinMode(LEDr, OUTPUT);

}

void loop() {
  delay(dt); // delay for temp/humid sensor, takes about 250 ms for a reading 
  airRead = analogRead(airSensor);
  float hum = dht.readHumidity();
  float temp = dht.readTemperature(); // temp in celsius
  
  if(isnan(hum) || isnan(temp)){
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  float heatidx = dht.computeHeatIndex(temp, hum, false);

  // Print weather data to screen
  Serial.print("Humidity: ");Serial.print(hum);Serial.print(" | ");
  Serial.print("Temp C: ");Serial.print(temp);Serial.print(" | ");
  Serial.print("Heat Index: ");Serial.println(heatidx);

  unsigned long startMillis = millis(); // start of sound sample
  float peakToPeak = 0;
  unsigned int signalMax = 0;
  unsigned int signalMin = 1024;
  while(millis() - startMillis < sampleTime){ // collect data for 50 ms
    sample = analogRead(soundSensor); // sensor reading
    if(sample < 1024){ // throw out spurious readings
      if(sample > signalMax){signalMax = sample;} // save max level
      else if(sample < signalMin){signalMin = sample;} // save min level
    }
  }
  peakToPeak = signalMax - signalMin; // amplitude
  float dbValue = 20*log(peakToPeak); // convert to dB

  // Print to LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(dbValue);
  lcd.setCursor(5, 0);
  lcd.print(" dB");
  lcd.setCursor(0, 1);
  lcd.print(airRead);
  lcd.setCursor(5, 1);
  lcd.print(" Air");

// Safe
  if((airRead < 181) || (dbValue >= 0 && dbValue < 70)){
    digitalWrite(LEDg, HIGH);
    digitalWrite(LEDy, LOW);
    digitalWrite(LEDo, LOW);
    digitalWrite(LEDr, LOW);
  }
// Moderate
  else if((airRead > 181 && airRead < 225) || (dbValue >= 70 && dbValue < 85)){
    digitalWrite(LEDg, HIGH);
    digitalWrite(LEDy, HIGH);
    digitalWrite(LEDo, LOW);
    digitalWrite(LEDr, LOW);
  }
// Not Good
  else if((airRead > 225 && airRead < 300) || (dbValue >= 85 && dbValue < 110)){
    digitalWrite(LEDg, HIGH);
    digitalWrite(LEDy, HIGH);
    digitalWrite(LEDo, HIGH);
    digitalWrite(LEDr, LOW);
  }
// Dangerous
  else if(airRead > 300 || dbValue >= 110){
    digitalWrite(LEDg, HIGH);
    digitalWrite(LEDy, HIGH);
    digitalWrite(LEDo, HIGH);
    digitalWrite(LEDr, HIGH);
  }
}
