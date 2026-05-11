#pragma once

#include <QList>
#include <QVariantList>
#include <QtGlobal>

#include "../models/SensorModel.h"
#include "../models/VectorModel.h"

struct FrameSnapshot {
  qint64 timestamp = 0;
  QList<Sensor> sensors;
  QList<Vector> vectors;
};

struct DecodedFrame {
  QVariantList sensors;
  QVariantList vectors;
  qint64 timestamp = 0;
  bool isValid = false;
};
