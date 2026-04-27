import sys
import time
import struct
import msgpack # pip install msgpack
 
_START_TIME = time.monotonic()
 
class Sensor:
    def __init__(self, name: str, input_val: float, operator_str: str,
                 threshold: float, layer: str, x: float, y: float):
        self.name = name
        self.input = float(input_val)
        self.operator_str = operator_str
        self.threshold = float(threshold)
        self.layer = layer
        self.x = float(x)
        self.y = float(y)
 
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
                 layer: str, x: float, y: float):
        self.name = name
        self.rotation = float(rotation)
        self.color = color
        self.layer = layer
        self.x = float(x)
        self.y = float(y)
 
 
class RoboticusDebugger:
    def __init__(self, output=None):
        self._output = output
        self._sensors: list = []
        self._vectors: list = []
        self._has_data = False
 
    def add(self, item):
        if isinstance(item, Sensor):
            self._add_sensor_to_pack(item)
        elif isinstance(item, Vector):
            self._add_vector_to_pack(item)
        else:
            raise TypeError(f"Expected Sensor or Vector, got {type(item)}")
 
    def add_sensor(self, name, input_val, operator_str, threshold, layer, x, y):
        self.add(Sensor(name, input_val, operator_str, threshold, layer, x, y))
 
    def add_vector(self, name, rotation, color, layer, x, y):
        self.add(Vector(name, rotation, color, layer, x, y))
 
    def write(self):
        if not self._has_data:
            return
 
        frame = [self._sensors, self._vectors, self._millis()]
 
        payload = msgpack.packb(frame, use_single_float=True, use_bin_type=True)
        size = len(payload)
        header = bytes([0xFD, size & 0xFF, (size >> 8) & 0xFF])
        
        out = header + payload
 
        if self._output is None:
            sys.stdout.buffer.write(out)
            sys.stdout.buffer.flush()
        else:
            self._output.write(out)
            
 
        self._reset()
 
    def is_empty(self) -> bool:
        return not self._has_data
 
    def _reset(self):
        self._sensors = []
        self._vectors = []
        self._has_data = False
 
    def _add_sensor_to_pack(self, s: Sensor):
        # [0]=name [1]=input [2]=isTriggered [3]=threshold [4]=layer [5]=x [6]=y
        self._sensors.append([
            s.name,
            s.input,
            s.is_triggered(),
            s.threshold,
            s.layer,
            s.x,
            s.y,
        ])
        self._has_data = True
 
    def _add_vector_to_pack(self, v: Vector):
        # [0]=name [1]=rotation [2]=color [3]=layer [4]=x [5]=y
        self._vectors.append([
            v.name,
            v.rotation,
            v.color,
            v.layer,
            v.x,
            v.y,
        ])
        self._has_data = True
 
    @staticmethod
    def _millis() -> int:
        return int((time.monotonic() - _START_TIME) * 1000)
 
