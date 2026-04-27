#pragma once

#include <Arduino.h>
#include <ArduinoJson.h> // @note the syntax used is from ArduinoJson V7

struct Sensor {
  String name;
  float input;
  String operatorStr;
  float threshold;
  String layer;
  float x;
  float y;

  // compares input and threshold using operatorStr and returns result
  bool isTriggered() const;
};

struct Vector {
  String name;
  float rotation;
  String color;
  String layer;
  float x;
  float y;
};

class RoboticusDebugger {
public:
  /**
   * @brief Construct a new Roboticus Debugger object with optional output
   * @note output defaults to Serial
   *
   * @param Print& - The output stream to write the JSON data to (e.g.,
   * Serial, File, etc.)
   */
  RoboticusDebugger(Print &output = Serial);

  /**
   * @brief Add a sensor/vector object to the current frame
   *
   * @param sensor, vector  - The sensor or vector object to add to the current
   * frame
   */
  void add(const Sensor &sensor);
  void add(const Vector &vector);

  /**
   * @brief Create vector/sensor object and add to current frame
   * @note implicitely calls add(const Sensor&) or add(const Vector&)
   *
   * @param name - name of the sensor/vector, if not given then app will show as
   * "no name given"
   * @param input - value of the sensor input, booleans also work (bool gets
   * automatically parsed as 0.0 or 1.0) // If not given then app will show as
   * "-1"
   * @param operatorStr - operator for comparing input and threshold (">=",
   * "<=", ">", "<", "==", "!=")
   * @param threshold - value to compare the input against, booleans also work
   * (bool gets automatically parsed as 0.0 or 1.0) // If not given then app
   * will show as "-1"
   * @param layer - layer name for organizing sensors/vectors in the app, if not
   * given then app will show as "no layer given"
   * @param x - x coordinate for visualizing the sensor/vector in the app, if
   * not given then app will show as 0
   * @param y - y coordinate for visualizing the sensor/vector in the app, if
   * not given then app will show as 0
   */
  void addSensor(const String &name, float input, const String &operatorStr,
                 float threshold, const String &layer, double x, double y);
  void addVector(const String &name, float rotation, const String &color,
                 const String &layer, int x, int y);

  // Send current frame and clear for next
  void write();

  // Check if data exists
  bool isEmpty() const;

private:
  Print &_output;
  JsonDocument _doc;
  JsonArray _sensorsArray;
  JsonArray _vectorsArray;
  bool _hasData;

  /**
   * @brief initialize the JSON document and arrays if not already initialized
   *
   */
  void begin();
};