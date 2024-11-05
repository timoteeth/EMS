// Team Ecosense
// Environmental Monitoring System
// Last updated: 10/24/2024
// 
// Sensor Structure
//
// this code sets up the struct for a sensor and all associated variables

struct Sensor{
  int id;     // unique identifier for each sensor
  int pinCount; // amount of pins being used to read from
  int* pins;    // arduino pins being used by sensor
  struct Entry* buffer;
  int buffi2c;
  int buffCount;
  int buffStride;
  char* (format*)(struct Entry*);

  void (*readFunction)(int* pins, int pinCount);  // function for sensor to convert pin reading to real data
};
