// Turbidity Sensor
// Author: Timothy Strawn
// Edited Last: 5/30/24

// Pins
#define turbiditySensor A4
const int LEDg = 10;
const int LEDo = 8;
const int LEDy = 6;
const int LEDr = 4;

// Definitions
const int dt = 1000;
int sensorRead = 0;
int n = 25; // number of samples
int sensorValue = 0;
float voltage = 0.0;
float turbidity = 0.0;
float Vclear = 4.73; // Calibrated voltage for clear water

void setup() {
  Serial.begin(9600);
  while(!Serial);
  Serial.println();

  pinMode(turbiditySensor, INPUT);
  pinMode(LEDg, OUTPUT);
  pinMode(LEDy, OUTPUT);
  pinMode(LEDo, OUTPUT);
  pinMode(LEDr, OUTPUT);
}

void loop() {
  getTurbidity();
  Serial.print("Turbidity: ");
  Serial.print(turbidity);
  Serial.println(" %");
  // calibrate();
  // Serial.println(Vclear);
  updateLEDs();
  delay(dt);
}

void getTurbidity(){ // get turbidity reading as percentage
  for(int i = 0; i < n; i++){
    sensorRead += analogRead(turbiditySensor);
    delay(10);
  }
  sensorValue = sensorRead / n; // averaged reading
  voltage = sensorValue*(5.0/1023.0);
  turbidity = 100.0-(voltage/Vclear)*100.0; // turbidity percentage 0% = clear water
  sensorRead = 0; // reset reading
}
void calibrate(){ // calibrate sensor in clear water
  for(int i = 0; i < n; i++){
    sensorRead += analogRead(turbiditySensor);
    delay(10);
  }
  sensorValue = sensorRead / n;
  Vclear = sensorValue*(5.0/1023.0);
  sensorRead = 0;
}
void updateLEDs(){
  // Safe
  if(turbidity < 25.0){
    digitalWrite(LEDg, HIGH);
    digitalWrite(LEDy, LOW);
    digitalWrite(LEDo, LOW);
    digitalWrite(LEDr, LOW);
  }
  // Moderate
  else if(turbidity > 25.0 && turbidity < 50.0){ 
    digitalWrite(LEDg, HIGH);
    digitalWrite(LEDo, HIGH);
    digitalWrite(LEDy, LOW);
    digitalWrite(LEDr, LOW);
  }
  // Not Good
  else if(turbidity > 50.0 && turbidity < 75.0){
    digitalWrite(LEDg, HIGH);
    digitalWrite(LEDo, HIGH);
    digitalWrite(LEDy, HIGH);
    digitalWrite(LEDr, LOW);
  }
  // Dangerous
  else if(turbidity > 75.0){
    digitalWrite(LEDg, HIGH);
    digitalWrite(LEDy, HIGH);
    digitalWrite(LEDo, HIGH);
    digitalWrite(LEDr, HIGH);
  }
}