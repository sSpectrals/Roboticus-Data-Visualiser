#include "RoboticusDebugger.h"

RoboticusDebugger debugger(Serial);

// Create arrays of sensors  ||  name | input | operatorStr | threshold | x | y
Sensor sensors[] = {
    // Circle
    {"IR_1", 23.5, ">=", 30.0, "Circle Layer", 10.00, 0.00},
    {"IR_2", 18.2, "<=", 20.0, "Circle Layer", 9.66, 2.59},
    {"IR_3", 31.7, ">", 28.0, "Circle Layer", 8.66, 5.00},
    {"IR_4", 22.1, "<", 25.0, "Circle Layer", 7.07, 7.07},
    {"IR_5", 23.5, "==", 30.0, "Circle Layer", 5.00, 8.66},
    {"IR_6", 23.5, ">=", 30.0, "Circle Layer", 2.59, 9.66},
    {"IR_7", 18.2, "<=", 20.0, "Circle Layer", 0.00, 10.00},
    {"IR_8", 31.7, ">", 28.0, "Circle Layer", -2.59, 9.66},
    {"IR_9", 22.1, "<", 25.0, "Circle Layer", -5.00, 8.66},
    {"IR_10", 23.5, ">=", 30.0, "Circle Layer", -7.07, 7.07},
    {"IR_11", 23.5, ">=", 30.0, "Circle Layer", -8.66, 5.00},
    {"IR_12", 18.2, "<=", 20.0, "Circle Layer", -9.66, 2.59},
    {"IR_13", 31.7, ">", 28.0, "Circle Layer", -10.00, 0.00},
    {"IR_14", 22.1, "<", 25.0, "Circle Layer", -9.66, -2.59},
    {"IR_15", 23.5, ">=", 30.0, "Circle Layer", -8.66, -5.00},
    {"IR_16", 23.5, ">=", 30.0, "Circle Layer", -7.07, -7.07},
    {"IR_17", 18.2, "<=", 20.0, "Circle Layer", -5.00, -8.66},
    {"IR_18", 31.7, ">", 28.0, "Circle Layer", -2.59, -9.66},
    {"IR_19", 22.1, "<", 25.0, "Circle Layer", 0.00, -10.00},
    {"IR_20", 23.5, ">=", 30.0, "Circle Layer", 2.59, -9.66},
    {"IR_21", 23.5, ">=", 30.0, "Circle Layer", 5.00, -8.66},
    {"IR_22", 18.2, "<=", 20.0, "Circle Layer", 7.07, -7.07},
    {"IR_23", 31.7, ">", 28.0, "Circle Layer", 8.66, -5.00},
    {"IR_24", 22.1, "<", 25.0, "Circle Layer", 9.66, -2.59},

    // Right arm
    {"IR_25", 23.5, ">=", 30.0, "Right Arm", 13.0, 0.0},
    {"IR_26", 18.2, "<=", 20.0, "Right Arm", 15.5, 0.0},
    {"IR_27", 31.7, ">", 28.0, "Right Arm", 18.0, 0.0},
    {"IR_28", 22.1, "<", 25.0, "Right Arm", 20.5, 0.0},

    // Left arm
    {"IR_29", 23.5, ">=", 30.0, "Left Arm", -13.0, 0.0},
    {"IR_30", 18.2, "<=", 20.0, "Left Arm", -15.5, 0.0},
    {"IR_31", 31.7, ">", 28.0, "Left Arm", -18.0, 0.0},
    {"IR_32", 22.1, "<", 25.0, "Left Arm", -20.5, 0.0},

    // Top arm
    {"IR_33", 23.5, ">=", 30.0, "Top Arm", 0.0, 13.0},
    {"IR_34", 18.2, "<=", 20.0, "Top Arm", 0.0, 15.5},
    {"IR_35", 31.7, ">", 28.0, "Top Arm", 0.0, 18.0},
    {"IR_36", 22.1, "<", 25.0, "Top Arm", 0.0, 20.5},

    // Bottom arm
    {"IR_37", 23.5, ">=", 30.0, "Bottom Arm", 0.0, -13.0},
    {"IR_38", 18.2, "<=", 20.0, "Bottom Arm", 0.0, -15.5},
    {"IR_39", 31.7, ">", 28.0, "Bottom Arm", 0.0, -18.0},
    {"IR_40", 22.1, "<", 25.0, "Bottom Arm", 0.0, -20.5}};

// Create arrays of vectors
Vector vectors[] = {
    {"Line avoidance", 45.0, "#FF5733", "Circle Layer", -15, -15},
    {"Line tracker", 90.0, "#33FF57", "Right arm", -18, -15},
    {"Ball", 135.0, "#3357FF", "whatever is left", -10, -15}};

// Calculate array sizes
const int numSensors = sizeof(sensors) / sizeof(sensors[0]);
const int numVectors = sizeof(vectors) / sizeof(vectors[0]);

void setup() { Serial.begin(115200); }


int frame = 0;
void loop() {
  // emulating changes in sensor inputs and vector rotations for demonstration
  // purposes
  frame++;
  for (int i = 0; i < numSensors; i++) {
    if (frame % 2 == 0) {
      sensors[i].input = sensors[i].threshold + 1.0;   // trigger
    } else {
      sensors[i].input = sensors[i].threshold - 1.0;   // not trigger
    }
  }
  vectors[0].rotation += 1.0;
  vectors[1].rotation += 3.0;
  vectors[2].rotation += 9.0;

  for (int i = 0; i < numSensors; i++) {
    // Add all sensor objects to the debugger
    debugger.add(sensors[i]);
  }
  for (int i = 0; i < numVectors; i++) {
    // Add every vector object to the debugger
    debugger.add(vectors[i]);
  }

  debugger.write(); // Send the frame to the app and clear for next frame
  delay(100);
}
