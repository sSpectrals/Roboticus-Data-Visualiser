#include "RoboticusDebugger.h"

bool Sensor::isTriggered() const {
  if (operatorStr == ">=")
    return input >= threshold;
  if (operatorStr == "<=")
    return input <= threshold;
  if (operatorStr == ">")
    return input > threshold;
  if (operatorStr == "<")
    return input < threshold;
  if (operatorStr == "==")
    return input == threshold;
  if (operatorStr == "!=")
    return input != threshold;
  return false;
}

RoboticusDebugger::RoboticusDebugger(Print &output)
    : _output(output), _hasData(false) {}

void RoboticusDebugger::begin() {
  _doc.clear();
  _doc["timestamp"] = millis();
  _sensorsArray = _doc["sensors"].to<JsonArray>();
  _vectorsArray = _doc["vectors"].to<JsonArray>();
  _hasData = true;
}

void RoboticusDebugger::addSensor(const String &name, float input,
                                  const String &operatorStr, float threshold,
                                  const String &layer, double x, double y) {
  if (!_hasData)
    begin();

  Sensor sensor{name, input, operatorStr, threshold, layer, x, y};
  add(sensor);
}

void RoboticusDebugger::addVector(const String &name, float rotation,
                                  const String &color, const String &layer,
                                  int x, int y) {
  if (!_hasData)
    begin();

  Vector vector{name, rotation, color, layer, x, y};
  add(vector);
}

void RoboticusDebugger::add(const Sensor &sensor) {
  if (!_hasData)
    return;

  JsonObject obj = _sensorsArray.add<JsonObject>();
  obj["name"] = sensor.name;
  obj["input"] = sensor.input;
  obj["isTriggered"] = sensor.isTriggered();
  obj["threshold"] = sensor.threshold;
  obj["layer"] = sensor.layer;

  JsonObject loc = obj["location"].to<JsonObject>();
  loc["x"] = sensor.x;
  loc["y"] = sensor.y;
}

void RoboticusDebugger::add(const Vector &vector) {
  if (!_hasData)
    return;

  JsonObject obj = _vectorsArray.add<JsonObject>();
  obj["name"] = vector.name;
  obj["rotation"] = vector.rotation;
  obj["color"] = vector.color;
  obj["layer"] = vector.layer;

  JsonObject loc = obj["location"].to<JsonObject>();
  loc["x"] = vector.x;
  loc["y"] = vector.y;
}
void RoboticusDebugger::write() {
  if (!_hasData)
    return;

  _doc.shrinkToFit(); // This is optional according to ArduinoJson docs,
                      // decreases memory usage by a lot.
  serializeJson(_doc, _output);
  _output.println(); // IMPORTANT!! The app parses the json with a new line as a
                     // delimiter between frames, so this is required.
  _hasData = false;
}

bool RoboticusDebugger::isEmpty() const { return !_hasData; }