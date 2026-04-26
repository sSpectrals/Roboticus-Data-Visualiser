import json
import sys
import time
 
 
_START_TIME = time.monotonic()
 
class Sensor:
    def __init__(self, name: str, input_val: float, operator_str: str,
                 threshold: float, layer: str, x: float, y: float):
        self.name = name
        self.input = float(input_val)
        self.operator_str = operator_str
        self.threshold = float(threshold)
        self.layer = layer
        self.x = x
        self.y = y
 
    def is_triggered(self) -> bool:
        ops = {
            ">=": self.input >= self.threshold,
            "<=": self.input <= self.threshold,
            ">":  self.input >  self.threshold,
            "<":  self.input <  self.threshold,
            "==": self.input == self.threshold,
            "!=": self.input != self.threshold,
        }
        return ops.get(self.operator_str, False)
 
 
class Vector:
    def __init__(self, name: str, rotation: float, color: str,
                 layer: str, x: int, y: int):
        self.name = name
        self.rotation = rotation
        self.color = color
        self.layer = layer
        self.x = x
        self.y = y
 
 
class RoboticusDebugger:
    def __init__(self, output=None):
        self._output = output
        self._sensors: list[dict] = []
        self._vectors: list[dict] = []
        self._has_data = False
 
    def add(self, item):
        if isinstance(item, Sensor):
            self._add_sensor_to_json(item)
        elif isinstance(item, Vector):
            self._add_vector_to_json(item)
        else:
            raise TypeError(f"Expected Sensor or Vector, got {type(item)}")
 
    def add_sensor(self, name, input_val, operator_str, threshold, layer, x, y):
        self.add(Sensor(name, input_val, operator_str, threshold, layer, x, y))
 
    def add_vector(self, name, rotation, color, layer, x, y):
        self.add(Vector(name, rotation, color, layer, x, y))
 
    def write(self):
        if not self._has_data:
            return
 
        frame = {
            "timestamp": self._millis(),
            "sensors":   self._sensors,
            "vectors":   self._vectors,
        }
 
        line = json.dumps(frame, separators=(",", ":")) + "\n"
 
        if self._output is None:
            sys.stdout.write(line)
            sys.stdout.flush()
        else:
            try:
                self._output.write(line.encode())
            except TypeError:
                self._output.write(line)
 
        self._reset()
 
    def is_empty(self) -> bool:
        return not self._has_data
 
    def _reset(self):
        self._sensors = []
        self._vectors = []
        self._has_data = False
 
    def _add_sensor_to_json(self, s: Sensor):
        self._sensors.append({
            "name":        s.name,
            "input":       s.input,
            "isTriggered": s.is_triggered(),
            "threshold":   s.threshold,
            "layer":       s.layer,
            "location":    {"x": s.x, "y": s.y},
        })
        self._has_data = True
 
    def _add_vector_to_json(self, v: Vector):
        self._vectors.append({
            "name":     v.name,
            "rotation": v.rotation,
            "color":    v.color,
            "layer":    v.layer,
            "location": {"x": v.x, "y": v.y},
        })
        self._has_data = True
 
    @staticmethod
    def _millis() -> int:
        return int((time.monotonic() - _START_TIME) * 1000)
 
