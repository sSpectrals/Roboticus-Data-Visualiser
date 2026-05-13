#pragma once

#include <Arduino.h>
#include <ArduinoJson.h> // @note the syntax used is from ArduinoJson V7

/**
 * @brief Represents a sensor with a name, input value, threshold, and position.
 *        The trigger condition is evaluated by comparing input against
 * threshold using the operatorStr field.
 */
struct Sensor {
  String name;
  float input;
  String operatorStr;
  float threshold;
  String layer;
  float x;
  float y;

  /**
   * @brief Evaluates whether the sensor is triggered by comparing input
   *        against threshold using operatorStr.
   * @return True if the condition is met, false otherwise or if operatorStr is
   * unrecognised.
   */
  bool isTriggered() const;
};

/**
 * @brief Represents a vector with a name, rotation, color, and position.
 */
struct Vector {
  String name;
  float rotation;
  String color;
  String layer;
  float x;
  float y;
};

/**
 * @brief Arduino library for sending sensor and vector data to the
 *        Roboticus Data Visualiser desktop app over serial.
 *
 *        Data is packed into MsgPack frames with a 0xFD header and a
 *        2-byte little-endian payload length, then written to the
 *        configured output stream (defaults to Serial).
 *
 * Basic usage:
 * @code
 *   RoboticusDebugger debugger;
 *
 *   void loop() {
 *       debugger.addSensor("Temperature", 23.5, ">=", 30.0, "Layer 1", 0, 0);
 *       debugger.addVector("Heading", 90.0, "white", "Layer 1", 1, 0);
 *       debugger.write();
 *   }
 * @endcode
 */
class RoboticusDebugger {
public:
  /**
   * @brief Constructs a RoboticusDebugger that writes to the given output
   * stream.
   * @param output Any Print-compatible stream (e.g. Serial, Serial1,
   * SoftwareSerial). Defaults to Serial.
   */
  RoboticusDebugger(Print &output = Serial);

  /**
   * @brief Adds a pre-constructed Sensor to the current frame.
   *        Implicitly calls begin() if the frame has not been started yet.
   * @param sensor The sensor to add.
   */
  void add(const Sensor &sensor);

  /**
   * @brief Adds a pre-constructed Vector to the current frame.
   *        Implicitly calls begin() if the frame has not been started yet.
   * @param vector The vector to add.
   */
  void add(const Vector &vector);

  /**
   * @brief Constructs a Sensor and adds it to the current frame.
   * @param name        Display name shown in the app.
   * @param input       Live sensor reading. Booleans are accepted and cast to
   * 0.0 or 1.0.
   * @param operatorStr Comparison operator for triggering: ">=", "<=", ">",
   * "<", "==", "!="
   * @param threshold   Value to compare input against. Booleans are accepted
   * and cast to 0.0 or 1.0.
   * @param layer       Layer name for grouping sensors in the app.
   * @param x           X coordinate for the sensor's position in the
   * visualiser.
   * @param y           Y coordinate for the sensor's position in the
   * visualiser.
   */
  void addSensor(const String &name, float input, const String &operatorStr,
                 float threshold, const String &layer, float x, float y);

  /**
   * @brief Constructs a Vector and adds it to the current frame.
   * @param name     Display name shown in the app.
   * @param rotation Rotation angle in degrees.
   * @param color    Color string (e.g. "white", "#FF0000").
   * @param layer    Layer name for grouping vectors in the app.
   * @param x        X coordinate for the vector's position in the visualiser.
   * @param y        Y coordinate for the vector's position in the visualiser.
   */
  void addVector(const String &name, float rotation, const String &color,
                 const String &layer, float x, float y);

  /**
   * @brief Serializes the current frame to MsgPack and writes it to the
   *        output stream with a 0xFD frame header, then clears the frame.
   */
  void write();

  /**
   * @brief Returns true if no sensors or vectors have been added to the current
   * frame.
   */
  bool isEmpty() const;

private:
  Print &_output;
  JsonDocument _doc;
  JsonArray _sensorsArray;
  JsonArray _vectorsArray;
  bool _hasData;

  /**
   * @brief Initializes the JsonDocument and sensor/vector arrays for a new
   * frame. Called automatically by add() and addSensor()/addVector() if needed.
   */
  void begin();
};