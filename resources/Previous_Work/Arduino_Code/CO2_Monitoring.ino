// CO2 Monitor
// Author: Timothy Strawn
// Edited Last: 6/18/2024

// Libraries
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Setting LCD address
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Pins
#define CO2pin 3
const int LEDr = 13;
const int LEDo = 11;
const int LEDy = 9;
const int LEDg = 7;

// Variables
int ValorAnterior = LOW;
long tiempoenHIGH, tiempoenLOW, h, l, ppm;
const int dt = 125;

void setup() {
  lcd.init();
  lcd.backlight();
  lcd.noCursor();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("CO2 Monitor");
  delay(1500);

  Serial.begin(9600);
  while (!Serial);
  Serial.println();

  pinMode(CO2pin, INPUT);
  pinMode(LEDr, OUTPUT);
  pinMode(LEDo, OUTPUT);
  pinMode(LEDy, OUTPUT);
  pinMode(LEDg, OUTPUT);

}

void loop() {

  getCO2ppm();
  Serial.print(ppm); Serial.println(" ppm");
  updateLCD();
  // updateLEDs();
  delay(dt);

}

void getCO2ppm(){
  long tt = millis();
  int ValorActual = digitalRead(CO2pin);
  if(ValorActual == HIGH){
    if(ValorActual != ValorAnterior){
      h = tt;
      tiempoenLOW = h- l;
      ValorAnterior = ValorActual;
    }
  }
  else{
    if(ValorActual != ValorAnterior){
      l = tt;
      tiempoenHIGH = l - h;
      ValorAnterior = ValorActual;
      ppm = 5000 * (tiempoenHIGH - 2)/(tiempoenHIGH + tiempoenLOW - 4);
    }
  }
}

void updateLEDs(){
  // Safe
  if(ppm < 1000){
    digitalWrite(LEDg, HIGH);
    digitalWrite(LEDy, LOW);
    digitalWrite(LEDo, LOW);
    digitalWrite(LEDr, LOW);
    lcd.setCursor(12, 0);
    lcd.print("Safe");
  }
  // Moderate
  if(ppm > 1000 && ppm < 2000){
    digitalWrite(LEDg, HIGH);
    digitalWrite(LEDy, HIGH);
    digitalWrite(LEDo, LOW);
    digitalWrite(LEDr, LOW);
    lcd.setCursor(8, 0);
    lcd.print("Moderate");
  }
  // Not Good
  if(ppm > 2000 && ppm < 5000){
    digitalWrite(LEDg, HIGH);
    digitalWrite(LEDy, HIGH);
    digitalWrite(LEDo, HIGH);
    digitalWrite(LEDr, LOW);
    lcd.setCursor(10, 0);
    lcd.print("Unsafe");
  }
  // Dangerous
  if(ppm > 5000){
    digitalWrite(LEDg, HIGH);
    digitalWrite(LEDy, HIGH);
    digitalWrite(LEDo, HIGH);
    digitalWrite(LEDr, HIGH);
    lcd.setCursor(10, 0);
    lcd.print("Danger");
  }
}
void updateLCD(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(ppm);
  lcd.setCursor(4, 0);
  lcd.print("ppm");
  lcd.setCursor(0, 1);
  lcd.write(0xff);
  float j = ppm;
  j = map(j, 0.0, 5000.0, 0.0, 20.0);
  for(float i = 0; i < int(j); i++){
    lcd.setCursor(i, 1);
    lcd.write(0xff);
  } 
}