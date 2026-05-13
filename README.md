# Roboticus Data Visualiser

Roboticus Data Visualiser is a desktop Qt application for visualising live robot telemetry from a serial stream.

It is designed for robotics debugging workflows where you want to:
- stream sensor values in real time,
- draw directional vectors (for heading, tracking, avoidance, etc.),
- group telemetry into layers,
- scrub through captured frames on a timeline,
- save and load sessions as JSON snapshots.

The app expects framed MsgPack packets over serial. Example senders are included for Arduino and Python.

## Features

- Live serial monitoring with selectable COM/TTY port and baud rate.
- 2D graph visualisation of:
  - sensors (value, threshold, triggered state, x/y position),
  - vectors (rotation, color, x/y position).
- Layer-based filtering for sensors and vectors.
- Timeline playback/scrubbing across captured frames.
- Save captured telemetry to JSON.
- Load JSON snapshots and replay them in the UI.
- Example telemetry senders:
  - Arduino library + example sketch,
  - Python library + example script for testing without hardware (virtual comports).


## Project Structure

- `src/`, `include/`: C++ core logic (controllers, models, serial parser, snapshot store)
- `ui/`: QML UI components (graph, monitor panes, timeline, connection bar)
- `examples/Arduino/`: Arduino telemetry sender library + sketch
- `examples/Python/`: Python telemetry sender helper + script
- `libs/qmsgpack/`: bundled MsgPack dependency


## Using the App

1. Launch the app.
2. In the top connection bar:
   - select a serial port,
   - select baud rate 
3. Click **Start Monitor**.
4. Stream telemetry frames from your robot/simulator.
5. Use the left panel to inspect sensors/vectors and switch layers.
6. Hover over a sensor in the graph to see individual values
7. Use the timeline at the bottom to:
   - scrub through frames,
   - step backward/forward,
   - save current session to JSON,
   - load a previously saved JSON session.

When a new serial connection is started, the snapshot timeline is reset.

## Sending Test Data

### Arduino example

How to use:

1. Add the `RoboticusDebugger` files to your Arduino project through the library manager. The RoboticusDebugger is also available on PlatformIO
2. Open the `BasicUsage` example and flash to your board.
3. Ensure board serial baud matches the app setting.
4. In the app, select the board port and click **Start Monitor**.

### Python example

Files:
- `examples/Python/RoboticusDebugger.py`
- `examples/Python/Example.py`

How to use:

1. Install Python deps:

```bash
pip install pyserial msgpack
```

2. In `examples/Python/Example.py`, set a real serial port and baudrate.

3. Run the example script

4. In the app, connect to the same serial port/baud.

## Serial Frame Protocol

The app parses framed MsgPack payloads using this wire format:

- Start byte: `0xFD`
- Payload length: 2 bytes, little-endian (`uint16`)
- Payload: MsgPack-encoded array `[sensors, vectors, timestamp_ms]`

Where:

- `sensors` is an array of sensor entries:
  - `[name, input, isTriggered, threshold, layer, x, y]`
- `vectors` is an array of vector entries:
  - `[name, rotation, color, layer, x, y]`
- `timestamp_ms` is an integer timestamp in milliseconds aka Millis() in Arduino.

Notes:
- Frames with empty sensors and empty vectors are ignored.
- Payload sizes outside expected bounds are rejected by the frame extractor.

## Snapshot JSON Format

Saved sessions are JSON arrays of snapshots. Each snapshot contains:

- `timestamp`
- `sensors[]` with fields:
  - `name`, `input`, `threshold`, `isTriggered`, `layer`, `location.{x,y}`
- `vectors[]` with fields:
  - `name`, `rotation`, `scale`, `color`, `layer`, `location.{x,y}`

This allows offline replay and sharing of captured telemetry sessions.

## Troubleshooting

- No serial ports visible:
  - check cable/device permissions,
  - verify the OS sees the device.
- Cannot connect to port:
  - close other tools using the port (Arduino Serial Monitor, etc.),
  - confirm baud matches sender.
- No data shown:
  - confirm your microcontroller can handle the size of the msgpack in ram (e.g. 40 sensors doesn't work on an Arduino Mega with 8kb sram, the solution is to send a smaller packet multiple times instead of 1 large .write())
- Loaded file fails:
  - ensure the file is valid JSON in the expected snapshot schema.

## Contributing

Issues and pull requests are welcome. If you report a bug, include:
- OS,
- Qt version,
- steps to reproduce,
