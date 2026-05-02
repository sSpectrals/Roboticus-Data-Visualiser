#include "RoboticusDebugger.h"


RoboticusDebugger debugger(Serial);

// Create arrays of sensors  ||  name | input | operatorStr | threshold | x | y
Sensor sensors[] = {
    // 20 points on a circle (radius 10) - every 18 degrees
    {"IR_1", 23.5, ">=", 30.0, "Circle Layer", 10.0, 0.0},
    {"IR_2", 18.2, "<=", 20.0, "Circle Layer", 9.51, 3.09},
    {"IR_3", 31.7, ">", 28.0, "Circle Layer", 8.09, 5.88},
    {"IR_4", 22.1, "<", 25.0, "Circle Layer", 5.88, 8.09},
    {"IR_5", true, "==", true, "Circle Layer", 3.09, 9.51},
    {"IR_6", 23.5, ">=", 30.0, "Circle Layer", 0.0, 10.0},
    {"IR_7", 18.2, "<=", 20.0, "Circle Layer", -3.09, 9.51},
    {"IR_8", 31.7, ">", 28.0, "Circle Layer", -5.88, 8.09},
    {"IR_9", 22.1, "<", 25.0, "Circle Layer", -8.09, 5.88},
    {"IR_10", true, "==", true, "Circle Layer", -9.51, 3.09},
};

// Create arrays of vectors
Vector vectors[] = {
    {"Line avoidance", 45.0, "#FF5733", "Circle Layer", -15, -15},
    {"Line tracker", 90.0, "#33FF57", "Right arm", -18, -15},
    {"Ball", 135.0, "#3357FF", "whatever is left", -10, -15},
};

// Calculate array sizes
const int numSensors = sizeof(sensors) / sizeof(sensors[0]);
const int numVectors = sizeof(vectors) / sizeof(vectors[0]);

void setup() { Serial.begin(115200); }

void loop() {

  for (int i = 0; i < numSensors; i++) {
    // Add every sensor object to the debugger
    debugger.add(sensors[i]);
  }

  for (int i = 0; i < numVectors; i++) {
    // Add every vector object to the debugger
    debugger.add(vectors[i]);
  }

  // emulating changes in sensor inputs and vector rotations for demonstration
  // purposes
  for (int i = 0; i < numSensors; i++) {
    sensors[i].input += 1.0;
  }
  vectors[0].rotation += 1.0;
  vectors[1].rotation += 3.0;
  vectors[2].rotation += 9.0;

  debugger.write(); // Send the frame to the app and clear for next frame
}
