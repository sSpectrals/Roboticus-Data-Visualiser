#include "parser/SnapshotLoader.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

SnapshotLoader::SnapshotLoader(QObject *parent) : QObject(parent) {}

bool SnapshotLoader::save(const QUrl &filePath,
                          const QList<FrameSnapshot> &snapshots,
                          QString *errorMessage) const {
  if (filePath.isEmpty()) {
    if (errorMessage) {
      *errorMessage = "No file path provided for saving.";
    }
    return false;
  }

  QFile file(filePath.toLocalFile());
  if (!file.open(QIODevice::WriteOnly)) {
    if (errorMessage) {
      *errorMessage =
          "Failed to open file for writing. Please check the file path.";
    }
    return false;
  }

  QJsonArray snapshotsArray;
  for (const auto &snapshot : snapshots) {
    QJsonObject snapshotObject;
    snapshotObject["timestamp"] = snapshot.timestamp;

    QJsonArray sensorsArray;
    for (const auto &sensor : snapshot.sensors) {
      QJsonObject sensorObject;
      sensorObject["name"] = sensor.name;
      sensorObject["input"] = sensor.inputValue;
      sensorObject["threshold"] = sensor.threshold;
      sensorObject["isTriggered"] = sensor.isTriggered;
      sensorObject["layer"] = sensor.layer;

      QJsonObject locationObject;
      locationObject["x"] = sensor.x;
      locationObject["y"] = sensor.y;
      sensorObject["location"] = locationObject;

      sensorsArray.append(sensorObject);
    }
    snapshotObject["sensors"] = sensorsArray;

    QJsonArray vectorsArray;
    for (const auto &vector : snapshot.vectors) {
      QJsonObject vectorObject;
      vectorObject["name"] = vector.name;
      vectorObject["rotation"] = vector.rotation;
      vectorObject["scale"] = vector.scale;
      vectorObject["color"] = vector.color.name();
      vectorObject["layer"] = vector.layer;

      QJsonObject locationObject;
      locationObject["x"] = vector.x;
      locationObject["y"] = vector.y;
      vectorObject["location"] = locationObject;

      vectorsArray.append(vectorObject);
    }
    snapshotObject["vectors"] = vectorsArray;
    snapshotsArray.append(snapshotObject);
  }

  const qint64 bytesWritten =
      file.write(QJsonDocument(snapshotsArray).toJson(QJsonDocument::Compact));
  file.close();
  if (bytesWritten == -1) {
    if (errorMessage) {
      *errorMessage = "Failed to write snapshots to file.";
    }
    return false;
  }
  return true;
}

bool SnapshotLoader::load(const QUrl &filePath, QList<FrameSnapshot> *snapshots,
                          QString *errorMessage) const {
  if (!snapshots) {
    if (errorMessage) {
      *errorMessage = "Missing output snapshots container.";
    }
    return false;
  }

  if (filePath.isEmpty()) {
    if (errorMessage) {
      *errorMessage = "No file path provided for loading.";
    }
    return false;
  }

  QFile file(filePath.toLocalFile());
  if (!file.open(QIODevice::ReadOnly)) {
    if (errorMessage) {
      *errorMessage =
          "Failed to open file for reading. Please check the file path.";
    }
    return false;
  }

  const QByteArray jsonData = file.readAll();
  file.close();

  QJsonParseError parseError;
  const QJsonDocument document = QJsonDocument::fromJson(jsonData, &parseError);
  if (parseError.error != QJsonParseError::NoError || !document.isArray()) {
    if (errorMessage) {
      *errorMessage = "Invalid file format. Please check the file.";
    }
    return false;
  }

  QList<FrameSnapshot> loaded;
  const QJsonArray snapshotsArray = document.array();
  for (const QJsonValue &snapshotValue : snapshotsArray) {
    if (!snapshotValue.isObject()) {
      continue;
    }

    const QJsonObject snapshotObject = snapshotValue.toObject();
    FrameSnapshot snapshot;
    snapshot.timestamp = snapshotObject["timestamp"].toInteger();

    const QJsonArray sensorsArray = snapshotObject["sensors"].toArray();
    for (const QJsonValue &sensorValue : sensorsArray) {
      if (!sensorValue.isObject()) {
        continue;
      }
      const QJsonObject sensorObject = sensorValue.toObject();
      Sensor sensor;
      sensor.name = sensorObject["name"].toString();
      sensor.inputValue = sensorObject["input"].toDouble();
      sensor.threshold = sensorObject["threshold"].toDouble();
      sensor.isTriggered = sensorObject["isTriggered"].toBool();
      sensor.layer = sensorObject["layer"].toString();
      const QJsonObject location = sensorObject["location"].toObject();
      sensor.x = location["x"].toDouble(0.0);
      sensor.y = location["y"].toDouble(0.0);
      snapshot.sensors.append(sensor);
    }

    const QJsonArray vectorsArray = snapshotObject["vectors"].toArray();
    for (const QJsonValue &vectorValue : vectorsArray) {
      if (!vectorValue.isObject()) {
        continue;
      }
      const QJsonObject vectorObject = vectorValue.toObject();
      Vector vector;
      vector.name = vectorObject["name"].toString();
      vector.rotation = vectorObject["rotation"].toDouble();
      vector.scale = vectorObject["scale"].toDouble(1.0);
      vector.color = QColor(vectorObject["color"].toString("white"));
      vector.layer = vectorObject["layer"].toString();
      const QJsonObject location = vectorObject["location"].toObject();
      vector.x = location["x"].toDouble(0.0);
      vector.y = location["y"].toDouble(0.0);
      snapshot.vectors.append(vector);
    }

    loaded.append(snapshot);
  }

  *snapshots = loaded;
  return true;
}
