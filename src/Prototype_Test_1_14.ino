#include <Arduino.h>

typedef int32_t EMS_Int;
typedef float EMS_Float;

typedef uint32_t EMS_Time;

typedef uint8_t EMS_Option;
typedef uint8_t EMS_OptionCount;

typedef uint8_t EMS_Pin;
typedef uint8_t EMS_PinCount;

EMS_Time clock = 0;

enum EMS_DATA_TYPE {
  EMS_DATA_TYPE_INT,
  EMS_DATA_TYPE_FLOAT
};

struct EMS_DataPoint {
  EMS_Time recordedTime;
  union {
    EMS_Int dataInt;
    EMS_Float dataFloat;
  };
};

struct EMS_DataSeries {
  const char* name;
  EMS_DATA_TYPE type;
  EMS_DataPoint array[10]; // Fixed-size array for simplicity
  size_t arrayLen;
};

enum EMS_SENSOR_TYPE {
  EMS_SENSOR_TYPE_PH,
  EMS_SENSOR_TYPE_WATER
};

struct EMS_Sensor {
  EMS_SENSOR_TYPE type;
  EMS_Option settings[2]; // Fixed size for simplicity
  EMS_Pin pins[2];
};

EMS_DataSeries seriesArray[4]; // Predefined series storage
size_t seriesCount = 0;

bool registerDataSeries(const char* name, EMS_DATA_TYPE type) {
  if (seriesCount >= 4) return false; // Limit on series
  seriesArray[seriesCount++] = { name, type, {}, 0 };
  return true;
}

bool addDataToSeries(const char* series, EMS_DataPoint data) {
  for (size_t i = 0; i < seriesCount; ++i) {
    if (strcmp(seriesArray[i].name, series) == 0) {
      if (seriesArray[i].arrayLen < 10) { // Ensure within bounds
        seriesArray[i].array[seriesArray[i].arrayLen++] = data;
        return true;
      }
    }
  }
  return false;
}

void printAllData() {
  for (size_t i = 0; i < seriesCount; ++i) {
    Serial.println(seriesArray[i].name);
    if (seriesArray[i].type == EMS_DATA_TYPE_INT) {
      for (size_t j = 0; j < seriesArray[i].arrayLen; ++j) {
        Serial.print(seriesArray[i].array[j].dataInt);
        Serial.print(" @ ");
        Serial.println(seriesArray[i].array[j].recordedTime);
      }
    } else if (seriesArray[i].type == EMS_DATA_TYPE_FLOAT) {
      for (size_t j = 0; j < seriesArray[i].arrayLen; ++j) {
        Serial.print(seriesArray[i].array[j].dataFloat);
        Serial.print(" @ ");
        Serial.println(seriesArray[i].array[j].recordedTime);
      }
    }
  }
}

void setup() {
  Serial.begin(9600);

  // Increment clock and assign values manually
  EMS_DataPoint data1;
  data1.recordedTime = ++clock;
  data1.dataInt = 42;

  EMS_DataPoint data2;
  data2.recordedTime = ++clock;
  data2.dataFloat = 7.5;

  // Print values for verification
  Serial.print("Data1: Time = ");
  Serial.print(data1.recordedTime);
  Serial.print(", Int = ");
  Serial.println(data1.dataInt);

  Serial.print("Data2: Time = ");
  Serial.print(data2.recordedTime);
  Serial.print(", Float = ");
  Serial.println(data2.dataFloat);
}


void loop() {
  // No continuous operations for now
}
