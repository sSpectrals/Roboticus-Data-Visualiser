import serial
from RoboticusDebugger import RoboticusDebugger, Sensor, Vector

#    port = None for simple print
#    port = serial.Serial("COM3", 115200)        # Windows
#    port = serial.Serial("/dev/ttyUSB0", 115200) # Linux / macOS
port = None

debugger = RoboticusDebugger(output=port)

sensors = [
    # Circle layer – 20 points on a circle (radius 10), every 18°
    Sensor("IR_1",  23.5,  ">=", 30.0, "Circle Layer",  10.0,   0.0),
    Sensor("IR_2",  18.2,  "<=", 20.0, "Circle Layer",   9.51,  3.09),
    Sensor("IR_3",  31.7,  ">",  28.0, "Circle Layer",   8.09,  5.88),
    Sensor("IR_4",  22.1,  "<",  25.0, "Circle Layer",   5.88,  8.09),
    Sensor("IR_5",  True,  "==", True,  "Circle Layer",   3.09,  9.51),
    Sensor("IR_6",  23.5,  ">=", 30.0, "Circle Layer",   0.0,  10.0),
    Sensor("IR_7",  18.2,  "<=", 20.0, "Circle Layer",  -3.09,  9.51),
    Sensor("IR_8",  31.7,  ">",  28.0, "Circle Layer",  -5.88,  8.09),
    Sensor("IR_9",  22.1,  "<",  25.0, "Circle Layer",  -8.09,  5.88),
    Sensor("IR_10", True,  "==", True,  "Circle Layer",  -9.51,  3.09),
    Sensor("IR_11", 23.5,  ">=", 30.0, "Circle Layer", -10.0,   0.0),
    Sensor("IR_12", 18.2,  "<=", 20.0, "Circle Layer",  -9.51, -3.09),
    Sensor("IR_13", 31.7,  ">",  28.0, "Circle Layer",  -8.09, -5.88),
    Sensor("IR_14", 22.1,  "<",  25.0, "Circle Layer",  -5.88, -8.09),
    Sensor("IR_15", True,  "==", True,  "Circle Layer",  -3.09, -9.51),
    Sensor("IR_16", 23.5,  ">=", 30.0, "Circle Layer",   0.0, -10.0),
    Sensor("IR_17", 18.2,  "<=", 20.0, "Circle Layer",   3.09, -9.51),
    Sensor("IR_18", 31.7,  ">",  28.0, "Circle Layer",   5.88, -8.09),
    Sensor("IR_19", 22.1,  "<",  25.0, "Circle Layer",   8.09, -5.88),
    Sensor("IR_20", True,  "==", True,  "Circle Layer",   9.51, -3.09),
    # Right arm
    Sensor("IR_21", 23.5,  ">=", 30.0, "Right arm",  13.0,  0.0),
    Sensor("IR_22", 18.2,  "<=", 20.0, "Right arm",  16.0,  0.0),
    Sensor("IR_23", 31.7,  ">",  28.0, "Right arm",  18.5,  0.0),
    Sensor("IR_24", 22.1,  "<",  25.0, "Right arm",  20.0,  0.0),
    # Left arm
    Sensor("IR_25", True,  "==", True,  "Left arm", -13.0,  0.0),
    Sensor("IR_26", 23.5,  ">=", 30.0, "Left arm", -16.0,  0.0),
    Sensor("IR_27", 18.2,  "<=", 20.0, "Left arm", -18.5,  0.0),
    Sensor("IR_28", 31.7,  ">",  28.0, "Left arm", -20.0,  0.0),
    # Top arm
    Sensor("IR_29", 22.1,  "<",  25.0, "Top arm",  0.0,  13.0),
    Sensor("IR_30", True,  "==", True,  "Top arm",  0.0,  16.0),
    Sensor("IR_31", 23.5,  ">=", 30.0, "Top arm",  0.0,  18.5),
    Sensor("IR_32", 18.2,  "<=", 20.0, "Top arm",  0.0,  20.0),
    # Bottom arm
    Sensor("IR_33", 31.7,  ">",  28.0, "Bottom arm",  0.0, -13.0),
    Sensor("IR_34", 22.1,  "<",  25.0, "Bottom arm",  0.0, -16.0),
    Sensor("IR_35", True,  "==", True,  "Bottom arm",  0.0, -18.5),
    Sensor("IR_36", 23.5,  ">=", 30.0, "Bottom arm",  0.0, -20.0),
]

vectors = [
    Vector("Line avoidance",  45.0, "#FF5733", "Circle Layer",    -15, -15),
    Vector("Line tracker",    90.0, "#33FF57", "Right arm",        -18, -15),
    Vector("Ball",           135.0, "#3357FF", "whatever is left", -10, -15),
]


while True:
    for s in sensors:
        debugger.add(s)

    for v in vectors:
        debugger.add(v)

    # Emulate sensors changing over time.
    for s in sensors:
        s.input += 1.0

    vectors[0].rotation += 1.0
    vectors[1].rotation += 3.0
    vectors[2].rotation += 9.0

    debugger.write()

