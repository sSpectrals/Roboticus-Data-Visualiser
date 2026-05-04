#include "SerialParser.h"

#include <QVariant>
#include <QVariantMap>

SerialParser::SerialParser(QObject *parent) : QObject(parent) {}

void SerialParser::setModels(QList<Sensor> sensors, QList<Vector> vectors) {
  for (const Sensor &s : sensors) {
    m_sensorModel->addSensor(s.name, s.inputValue, s.threshold, s.isTriggered,
                             s.layer, s.x, s.y);
  }

  for (const Vector &v : vectors) {
    m_vectorModel->addVector(v.name, v.rotation, v.scale, v.color, v.layer, v.x,
                             v.y);
  }
}

void SerialParser::setModels(SensorModel *sensorModel,
                             VectorModel *vectorModel) {
  m_sensorModel = sensorModel;
  m_vectorModel = vectorModel;
}

void SerialParser::setPortManager(SerialPortManager *pm) {
  if (m_portManager == pm)
    return;

  // Disconnect previous port manager if present
  if (m_portManager) {
    disconnect(m_portManager, &SerialPortManager::rawDataReceived, this,
               &SerialParser::onRawDataReady);
  }

  m_portManager = pm;
  connect(m_portManager, &SerialPortManager::rawDataReceived, this,
          &SerialParser::onRawDataReady, Qt::UniqueConnection);

  emit portManagerChanged();
}

void SerialParser::onRawDataReady(const QByteArray &data) {
  m_frameExtractor.appendData(data);
  const QList<QByteArray> frames = m_frameExtractor.takeCompleteFrames();
  for (const QByteArray &msgpackData : frames) {
    emit dataReceived(msgpackData);
    processMsgPackData(msgpackData);
  }
}

void SerialParser::processMsgPackData(const QByteArray &data) {
  const DecodedFrame frame = m_frameDecoder.decodeMsgPackFrame(data);
  if (!frame.isValid) {
    qDebug() << "Frame contains no sensors or vectors";
    return;
  }

  if (!frame.sensors.isEmpty())
    updateSensorsFromMsgPack(frame.sensors);

  if (!frame.vectors.isEmpty())
    updateVectorsFromMsgPack(frame.vectors);

  FrameSnapshot snapshot;
  snapshot.timestamp = frame.timestamp;
  snapshot.sensors =
      m_sensorModel ? m_sensorModel->getAllSensors() : QList<Sensor>();
  snapshot.vectors =
      m_vectorModel ? m_vectorModel->getAllVectors() : QList<Vector>();

  m_snapshotStore.append(snapshot);
  emit snapshotsChanged();
}

void SerialParser::updateSensorsFromMsgPack(const QVariantList &sensors) {
  if (!m_sensorModel)
    return;

  for (const QVariant &sensorVar : sensors) {
    // [0]=name [1]=input [2]=isTriggered [3]=threshold [4]=layer [5]=x [6]=y
    QVariantList s = sensorVar.toList();
    if (s.size() < 7) { // 7 elements expected for a valid sensor entry
      qDebug() << "Malformed sensor entry, skipping";
      continue;
    }

    QString name = s.at(0).toString();
    double input = roundTo2Decimals(s.at(1).toFloat());
    bool isTriggered = s.at(2).toBool();
    double threshold = roundTo2Decimals(s.at(3).toFloat());
    QString layer = s.at(4).toString();
    double x = roundTo2Decimals(s.at(5).toFloat());
    double y = roundTo2Decimals(s.at(6).toFloat());

    int idx = m_sensorModel->getIndexByName(name);
    if (idx >= 0) {
      QModelIndex modelIdx = m_sensorModel->index(idx);
      m_sensorModel->setData(modelIdx, input, SensorModel::InputRole);
      m_sensorModel->setData(modelIdx, threshold, SensorModel::ThresholdRole);
      m_sensorModel->setData(modelIdx, isTriggered, SensorModel::TriggerRole);
      m_sensorModel->setData(modelIdx, x, SensorModel::XRole);
      m_sensorModel->setData(modelIdx, y, SensorModel::YRole);
      m_sensorModel->setData(modelIdx, layer, SensorModel::LayerRole);
    } else {
      m_sensorModel->addSensor(name, input, threshold, isTriggered, layer, x,
                               y);
    }
  }
}

void SerialParser::updateVectorsFromMsgPack(const QVariantList &vectors) {
  if (!m_vectorModel)
    return;

  for (const QVariant &vectorVar : vectors) {
    // [0]=name [1]=rotation [2]=color [3]=layer [4]=x [5]=y
    QVariantList v = vectorVar.toList();
    if (v.size() < 6) { // 6 elements expected for a valid vector entry
      qDebug() << "Malformed vector entry, skipping";
      continue;
    }

    QString name = v.at(0).toString();
    double rotation = roundTo2Decimals(v.at(1).toFloat());
    QString color = v.at(2).toString();
    QString layer = v.at(3).toString();
    double x = roundTo2Decimals(v.at(4).toFloat());
    double y = roundTo2Decimals(v.at(5).toFloat());

    int idx = m_vectorModel->getIndexByName(name);
    if (idx >= 0) {
      // Update existing vector in the model
      QModelIndex modelIdx = m_vectorModel->index(idx);
      m_vectorModel->setData(modelIdx, rotation, VectorModel::RotationRole);
      m_vectorModel->setData(modelIdx, color, VectorModel::ColorRole);
      m_vectorModel->setData(modelIdx, x, VectorModel::XRole);
      m_vectorModel->setData(modelIdx, y, VectorModel::YRole);
      m_vectorModel->setData(modelIdx, layer, VectorModel::LayerRole);
    } else {
      // Add new vector if it doesn't exist yet
      // Assuming magnitude is 1.0 for visualization
      m_vectorModel->addVector(name, rotation, 1.0, QColor(color), layer, x, y);
    }
  }
}

double SerialParser::roundTo2Decimals(float value) {
  return qRound(value * 100) / 100.0;
}

Q_INVOKABLE qint64 SerialParser::timestampAt(int index) const {
  return m_snapshotStore.timestampAt(index);
}

Q_INVOKABLE bool SerialParser::saveToFile(QUrl filePath) {
  QString errorMessage;
  const bool success =
      m_snapshotLoader.save(filePath, m_snapshotStore.all(), &errorMessage);
  if (!success && !errorMessage.isEmpty()) {
    emit errorOccurred(errorMessage);
  }
  return success;
}

Q_INVOKABLE bool SerialParser::loadFromFile(QUrl filePath) {
  QList<FrameSnapshot> loadedSnapshots;
  QString errorMessage;
  const bool success =
      m_snapshotLoader.load(filePath, &loadedSnapshots, &errorMessage);
  if (!success) {
    if (!errorMessage.isEmpty()) {
      emit errorOccurred(errorMessage);
    }
    return false;
  }

  m_snapshotStore.clear();
  for (const auto &snapshot : loadedSnapshots) {
    m_snapshotStore.append(snapshot);
  }
  emit snapshotsChanged();
  return true;
}

// Returns a list of all available timestamps
QList<qint64> SerialParser::availableTimestamps() const {
  return m_snapshotStore.availableTimestamps();
}

// Restores the model state to the snapshot at the given index
bool SerialParser::restoreToIndex(int index) {
  if (!m_snapshotStore.isValidIndex(index))
    return false;

  const FrameSnapshot snapshot = m_snapshotStore.at(index);
  if (m_sensorModel)
    m_sensorModel->clear();
  if (m_vectorModel)
    m_vectorModel->clear();

  if (m_portManager)
    m_portManager->disconnectPort();
  setModels(snapshot.sensors, snapshot.vectors);
  return true;
}
