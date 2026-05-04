#include "SerialParser.h"

#include <QVariant>
#include <QVariantMap>

SerialParser::SerialParser(QObject *parent) : QObject(parent) {
  refreshPorts();

  // Setup timer to poll for port changes every 1 second
  connect(&m_portRefreshTimer, &QTimer::timeout, this,
          &SerialParser::refreshPorts);
  m_portRefreshTimer.start(1000);

  connect(&m_serial, &QSerialPort::errorOccurred, this,
          &SerialParser::handleSerialError);
}

Q_INVOKABLE bool SerialParser::connectToPort() {
  if (m_serial.isOpen()) {
    m_serial.close();
  }

  if (m_serial.portName().isEmpty()) {
    qDebug() << "No COM port selected";
    emit errorOccurred(
        "No COM port selected. Please choose a port to connect.");
    return false;
  }

  if (m_serial.baudRate() <= 0) {
    qDebug() << "Failed to set baudrate";
    emit errorOccurred("Failed to set baud rate. Please check your settings.");
    return false;
  }

  configureDefaultSettings();

  bool success = m_serial.open(QIODevice::ReadOnly);

  if (success) {

    m_serial.setDataTerminalReady(true);

    // Clear any startup garbage (null bytes from Arduino reset)
    m_frameExtractor = SerialFrameExtractor();
    m_serial.clear();

    connect(&m_serial, &QSerialPort::readyRead, this, &SerialParser::readData,
            Qt::UniqueConnection);

    qDebug() << "successfully connected";
    qDebug() << "Port:" << m_serial.portName()
             << "Baud:" << m_serial.baudRate();
    emit connectionChanged();
    emit portChanged();
  } else {
    qDebug() << "Error:" << m_serial.error() << m_serial.errorString()
             << "\nCheck if you have a serial monitor open somewhere else";
    qDebug() << "Port:" << m_serial.portName()
             << "Baud:" << m_serial.baudRate();
    emit errorOccurred("Failed to connect to port. Please check if the port is "
                       "correct and not in use by another application.");
  }

  return success;
}

void SerialParser::configureDefaultSettings() {
  m_serial.setParity(QSerialPort::NoParity);
  m_serial.setDataBits(QSerialPort::Data8);
  m_serial.setStopBits(QSerialPort::OneStop);
  m_serial.setFlowControl(QSerialPort::NoFlowControl);
}

bool SerialParser::setBaudRate(int baudRate) {
  bool wasOpen = m_serial.isOpen();

  if (wasOpen) {
    QString currentPort = m_serial.portName();
    m_serial.close();

    m_serial.setBaudRate(baudRate);
    bool success = connectToPort();

    if (success) {
      emit connectionChanged();
    }

    return success;
  } else {
    return m_serial.setBaudRate(baudRate);
  }
}

bool SerialParser::setComPort(QString port) {
  bool wasOpen = m_serial.isOpen();

  if (wasOpen) {
    m_serial.close();

    m_serial.setPortName(port);

    bool success = connectToPort();
    if (success) {
      emit connectionChanged();
      emit portChanged();
    }

    return success;
  } else {
    m_serial.setPortName(port);
    return true;
  }
}

void SerialParser::disconnectPort() {
  if (m_serial.isOpen()) {
    m_serial.close();
    emit connectionChanged();
    qDebug() << "Disconnected from" << m_serial.portName();
  }
}

void SerialParser::handleSerialError(QSerialPort::SerialPortError error) {
  if (error == QSerialPort::NoError) {
    return;
  }

  if (!m_serial.isOpen()) {
    return;
  }

  if (error == QSerialPort::ResourceError ||
      error == QSerialPort::DeviceNotFoundError) {
    qDebug() << "Serial connection lost:" << m_serial.errorString();
    m_serial.close();
    emit connectionChanged();
    emit errorOccurred("Serial connection lost.");
  }
}

QStringList SerialParser::availablePorts() {
  QStringList ports;
  const auto serialPorts = QSerialPortInfo::availablePorts();
  for (const QSerialPortInfo &info : serialPorts) {
    ports.append(info.portName());
  }
  return ports;
}

void SerialParser::refreshPorts() {
  QStringList newPorts = availablePorts();
  if (newPorts != m_availablePorts) {
    m_availablePorts = newPorts;
    emit availablePortsChanged();
  }
}

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

void SerialParser::readData() {
  if (!m_serial.isOpen()) {
    qDebug() << "Could not read serial data";
    emit errorOccurred(
        "Could not read serial data. Please check your connection.");
    return;
  }

  m_frameExtractor.appendData(m_serial.readAll());
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
    if (s.size() < 7) {
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
    if (v.size() < 6) {
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
  disconnectPort();
  setModels(snapshot.sensors, snapshot.vectors);
  return true;
}
