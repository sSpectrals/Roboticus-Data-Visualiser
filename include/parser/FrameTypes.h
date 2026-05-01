#ifndef FRAMETYPES_H
#define FRAMETYPES_H

#include <QList>
#include <QVariantList>
#include <QtGlobal>

#include "SensorModel.h"
#include "VectorModel.h"

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

#endif // FRAMETYPES_H
