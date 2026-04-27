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
  _sensorsArray = _doc[0].to<JsonArray>();
  _vectorsArray = _doc[1].to<JsonArray>();
  _hasData = true;
}

void RoboticusDebugger::addSensor(const String &name, float input,
                                  const String &operatorStr, float threshold,
                                  const String &layer, float x, float y) {
  if (!_hasData)
    begin();

  Sensor sensor{name, input, operatorStr, threshold, layer, x, y};
  add(sensor);
}

void RoboticusDebugger::addVector(const String &name, float rotation,
                                  const String &color, const String &layer,
                                  float x, float y) {
  if (!_hasData)
    begin();

  Vector vector{name, rotation, color, layer, x, y};
  add(vector);
}

void RoboticusDebugger::add(const Sensor &sensor) {
  if (!_hasData)
    begin();

  JsonArray obj = _sensorsArray.add<JsonArray>();
  // [0]=name [1]=input [2]=isTriggered [3]=threshold [4]=layer [5]=x [6]=y
  obj.add(sensor.name);
  obj.add(sensor.input);
  obj.add(sensor.isTriggered());
  obj.add(sensor.threshold);
  obj.add(sensor.layer);
  obj.add(sensor.x);
  obj.add(sensor.y);
}

void RoboticusDebugger::add(const Vector &vector) {
  if (!_hasData)
    begin();

  JsonArray obj = _vectorsArray.add<JsonArray>();
  // [0]=name [1]=rotation [2]=color [3]=layer [4]=x [5]=y
  obj.add(vector.name);
  obj.add(vector.rotation);
  obj.add(vector.color);
  obj.add(vector.layer);
  obj.add(vector.x);
  obj.add(vector.y);
}
void RoboticusDebugger::write() {
  if (!_hasData)
    return;

  _doc[2] = millis();
  _doc.shrinkToFit();

  size_t msgSize = measureMsgPack(_doc);

  uint8_t header[3];
  header[0] = 0xFD;
  header[1] = (uint8_t)(msgSize & 0xFF);
  header[2] = (uint8_t)((msgSize >> 8) & 0xFF);

  // unsigned long start = micros(); // Start timer
  _output.write(header, 3);
  serializeMsgPack(_doc, _output);
  // unsigned long end = micros(); // End timer

  // Serial.print("Time (us): ");
  // Serial.println(end - start);
  // Serial.print("Bytes: ");
  // Serial.println(msgSize);

  _hasData = false;
}

bool RoboticusDebugger::isEmpty() const { return !_hasData; }