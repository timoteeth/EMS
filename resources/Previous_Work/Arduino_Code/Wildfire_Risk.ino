// Wildfire Risk Assessment Monitor
// Author: Timothy Strawn (tstrawn@mines.edu)
// Last Edited: 7/2/2024

// Libraries
#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// DHT Sendsor Setup
#define DHTPIN 4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// LCD Setup
LiquidCrystal_I2C lcd(0x27, 20, 4);

// Wind Sensor Setup
#define windSensor A0

// Definitions
const int dt = 200;
int windRead;
float windVoltage;
float windSpeed;
float humid;
float temp;
float heatidx;

void setup() {
  lcd.init();
  lcd.backlight();
  lcd.noCursor();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Wildfire Risk");
  lcd.setCursor(0, 1);
  lcd.print("Assessment");
  delay(3000);

  Serial.begin(9600);
  while (!Serial);
  Serial.println();
  dht.begin();

  pinMode(windSensor, INPUT);

}

void loop() {
  delay(dt); // delay for DHT sensor takes about 250 ms for a reading
  getWindSpeed();
  getTemperature(); // temp in farenhit
  getHumidity();

  if(isnan(humid) || isnan(temp)){
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  getHeatIndex();
  updateLCD();

}
void getWindSpeed(){
  windRead = analogRead(windSensor);
  windSpeed = windRead*(1/34.91)*(0.371);
  // windVoltage = windRead*(5.0/1023.0);
  // windSpeed = windVoltage*14.0;
}
void getHumidity(){humid = dht.readHumidity();}
void getTemperature(){temp = dht.readTemperature(true);}
void getHeatIndex(){heatidx = dht.computeHeatIndex(temp, humid, true);}
void updateLCD(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("T");
  lcd.setCursor(2, 0);
  lcd.print(temp);
  lcd.setCursor(10, 0);
  lcd.print("H");
  lcd.setCursor(12, 0);
  lcd.print(humid);
  lcd.setCursor(10, 1);
  lcd.print("I");
  lcd.setCursor(12, 1);
  lcd.print(heatidx);
  lcd.setCursor(0, 1);
  lcd.print("W");
  lcd.setCursor(2, 1);
  lcd.print(windSpeed);
}