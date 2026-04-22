#include "SerialParser.h"

SerialParser::SerialParser(QObject *parent)
    : QObject(parent)
{
    refreshPorts();

    // Setup timer to poll for port changes every 1 second
    connect(&m_portRefreshTimer, &QTimer::timeout, this, &SerialParser::refreshPorts);
    m_portRefreshTimer.start(1000);
}

Q_INVOKABLE bool SerialParser::connectToPort()
{
    if (m_serial.isOpen()) {
        m_serial.close();
    }

    if (m_serial.portName().isEmpty()) {
        qDebug() << "No COM port selected";
        emit errorOccurred("No COM port selected. Please choose a port to connect.");
        return false;
    }

    if (m_serial.baudRate() <= 0) {
        qDebug() << "failed to set port or baudrate";
        emit errorOccurred("Failed to set baud rate. Please check your settings.");
        return false;
    }

    configureDefaultSettings();

    bool success = m_serial.open(QIODevice::ReadOnly);

    if (success) {
        m_serial.setDataTerminalReady(false);

        // Clear any startup garbage (null bytes from Arduino reset)
        m_buffer.clear();
        m_serial.clear();

        connect(&m_serial,
                &QSerialPort::readyRead,
                this,
                &SerialParser::readData,
                Qt::UniqueConnection);

        qDebug() << "successfully connected";
        qDebug() << "Port:" << m_serial.portName() << "Baud:" << m_serial.baudRate();
        emit connectionChanged();
        emit portChanged();
    } else {
        qDebug() << "Error:" << m_serial.error() << m_serial.errorString()
                 << "\nCheck if you have a serial monitor open somewhere else";
        qDebug() << "Port:" << m_serial.portName() << "Baud:" << m_serial.baudRate();
        emit errorOccurred("Failed to connect to port. Please check if the port is "
                           "correct and not in use by another application.");
    }

    return success;
}

void SerialParser::configureDefaultSettings()
{
    m_serial.setParity(QSerialPort::NoParity);
    m_serial.setDataBits(QSerialPort::Data8);
    m_serial.setStopBits(QSerialPort::OneStop);
    m_serial.setFlowControl(QSerialPort::NoFlowControl);
}

bool SerialParser::setBaudRate(int baudRate)
{
    QList<int> validRates = {38400, 57600, 115200, 230400, 460800, 921600};
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

bool SerialParser::setComPort(QString port)
{
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

void SerialParser::disconnectPort()
{
    if (m_serial.isOpen()) {
        m_serial.close();
        emit connectionChanged();
        qDebug() << "Disconnected from" << m_serial.portName();
    }
}

QStringList SerialParser::availablePorts()
{
    QStringList ports;
    const auto serialPorts = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &info : serialPorts) {
        ports.append(info.portName());
    }
    return ports;
}

void SerialParser::refreshPorts()
{
    QStringList newPorts = availablePorts();
    if (newPorts != m_availablePorts) {
        m_availablePorts = newPorts;
        emit availablePortsChanged();
    }
}

void SerialParser::setModels(QList<Sensor> sensors, QList<Vector> vectors)
{
    for (const Sensor &s : sensors) {
        m_sensorModel->addSensor(s.name, s.inputValue, s.threshold, s.isTriggered, s.layer, s.x, s.y);
    }

    for (const Vector &v : vectors) {
        m_vectorModel->addVector(v.name, v.rotation, v.scale, v.color, v.layer, v.x, v.y);
    }
}

void SerialParser::setModels(SensorModel *sensorModel, VectorModel *vectorModel)
{
    m_sensorModel = sensorModel;
    m_vectorModel = vectorModel;
}

void SerialParser::readData()
{
    if (!m_serial.isOpen()) {
        qDebug() << "Could not read serial data";
        emit errorOccurred("Could not read serial data. Please check your connection.");
        return;
    }

    m_buffer.append(m_serial.readAll());

    // Prevent buffer from growing too large (e.g., malformed data)
    constexpr int maxBufferSize = 65536;
    if (m_buffer.size() > maxBufferSize) {
        qDebug() << "Buffer overflow, keeping last 4KB";
        m_buffer = m_buffer.right(4096);
    }

    // Process all complete lines (newline-delimited JSON)
    int newlineIdx;
    while ((newlineIdx = m_buffer.indexOf('\n')) != -1) {
        QByteArray line = m_buffer.left(newlineIdx).trimmed();
        m_buffer.remove(0, newlineIdx + 1);

        // Skip empty lines or non-JSON data
        if (line.isEmpty()) {
            continue;
        }

        // Find start of JSON
        int jsonStart = line.indexOf('{');
        if (jsonStart > 0) {
            line = line.mid(jsonStart);
        }
        if (!line.startsWith('{') || !line.endsWith('}')) {
            qDebug() << "Discarded malformed line:" << line.left(100);
            continue;
        }

        // qDebug() << "Received JSON:" << line.size() << "bytes";

        emit dataReceived(line);
        processJsonData(line);
    }
}

Q_INVOKABLE qint64 SerialParser::timestampAt(int index) const
{
    if (index < 0 || index >= m_snapshots.size()) {
        return -1;
    }
    return m_snapshots[index].timestamp;
}

Q_INVOKABLE bool SerialParser::saveToFile(QUrl filePath)
{
    if (filePath.isEmpty()) {
        qDebug() << "No file path provided for saving.";
        return false;
    }

    QFile file(filePath.toLocalFile());
    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << "Failed to open file for writing:" << filePath;
        emit errorOccurred("Failed to open file for writing. Please check the file path.");
        return false;
    }
    QJsonArray snapshotsArray;
    for (int i = 0; i < m_snapshots.size(); ++i) {
        const auto &snap = m_snapshots.at(i);

        QJsonObject snapObj;
        snapObj["timestamp"] = snap.timestamp;
        QJsonArray sensorsArray;
        for (const auto &sensor : snap.sensors) {
            QJsonObject sensorObj;
            sensorObj["name"] = sensor.name;
            sensorObj["input"] = sensor.inputValue;
            sensorObj["threshold"] = sensor.threshold;
            sensorObj["isTriggered"] = sensor.isTriggered;
            sensorObj["layer"] = sensor.layer;

            QJsonObject locationObj;
            locationObj["x"] = sensor.x;
            locationObj["y"] = sensor.y;
            sensorObj["location"] = locationObj;
            sensorsArray.append(sensorObj);
        }
        snapObj["sensors"] = sensorsArray;

        QJsonArray vectorsArray;
        for (const auto &vector : snap.vectors) {
            QJsonObject vectorObj;
            vectorObj["name"] = vector.name;
            vectorObj["rotation"] = vector.rotation;
            vectorObj["scale"] = vector.scale;
            vectorObj["color"] = vector.color.name();
            vectorObj["layer"] = vector.layer;

            QJsonObject locationObj;
            locationObj["x"] = vector.x;
            locationObj["y"] = vector.y;
            vectorObj["location"] = locationObj;
            vectorsArray.append(vectorObj);
        }
        snapObj["vectors"] = vectorsArray;

        snapshotsArray.append(snapObj);
    }
    QJsonDocument doc(snapshotsArray);
    qint64 bytesWritten = file.write(doc.toJson(QJsonDocument::Compact));
    file.close();

    if (bytesWritten == -1) {
        qDebug() << "Failed to write to file:" << filePath;
        return false;
    }
    qDebug() << "Successfully saved" << snapshotsArray.size() << "snapshots to" << filePath;
    return true;
}

Q_INVOKABLE bool SerialParser::loadFromFile(QUrl filePath)
{
    if (filePath.isEmpty()) {
        qDebug() << "No file path provided for loading.";
        return false;
    }

    QFile file(filePath.toLocalFile());
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Failed to open file for reading:" << filePath;
        emit errorOccurred("Failed to open file for reading. Please check the file path.");
        return false;
    }

    QByteArray jsonData = file.readAll();
    file.close();

    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(jsonData, &parseError);
    if (parseError.error != QJsonParseError::NoError) {
        qDebug() << "JSON parse error in file:" << parseError.errorString();
        emit errorOccurred("Failed to parse JSON file. Please check the file format.");
        return false;
    }

    if (!doc.isArray()) {
        qDebug() << "Expected JSON array in file";
        emit errorOccurred("Invalid file format. Please check the file.");
        return false;
    }
    m_snapshots.clear();
    QJsonArray snapshotsArray = doc.array();
    for (int i = 0; i < snapshotsArray.size(); ++i) {
        const QJsonValue &snapVal = snapshotsArray.at(i);
        if (!snapVal.isObject()) {
            qDebug() << "Skipping invalid snapshot entry";
            continue;
        }

        QJsonObject snapObj = snapVal.toObject();
        FrameSnapshot snapshot;
        snapshot.timestamp = snapObj["timestamp"].toInteger();

        QJsonArray sensorsArray = snapObj["sensors"].toArray();
        for (int j = 0; j < sensorsArray.size(); ++j) {
            const QJsonValue &sensorVal = sensorsArray.at(j);
            if (!sensorVal.isObject()) {
                qDebug() << "Skipping invalid sensor entry";
                continue;
            }

            QJsonObject sensorObj = sensorVal.toObject();
            Sensor sensor;
            sensor.name = sensorObj["name"].toString();
            sensor.inputValue = sensorObj["input"].toDouble();
            sensor.threshold = sensorObj["threshold"].toDouble();
            sensor.isTriggered = sensorObj["isTriggered"].toBool();
            sensor.layer = sensorObj["layer"].toString();

            if (sensorObj.contains("location") && sensorObj["location"].isObject()) {
                QJsonObject locationObj = sensorObj["location"].toObject();
                sensor.x = locationObj["x"].toDouble();
                sensor.y = locationObj["y"].toDouble();
            } else {
                qDebug() << "Sensor" << sensor.name
                         << "is missing location data, defaulting to (0,0)";
                sensor.x = 0;
                sensor.y = 0;
            }

            snapshot.sensors.append(sensor);
        }

        QJsonArray vectorsArray = snapObj["vectors"].toArray();
        for (int k = 0; k < vectorsArray.size(); ++k) {
            const QJsonValue &vectorVal = vectorsArray.at(k);
            if (!vectorVal.isObject()) {
                qDebug() << "Skipping invalid vector entry";
                continue;
            }

            QJsonObject vectorObj = vectorVal.toObject();
            Vector vector;
            vector.name = vectorObj["name"].toString();
            vector.rotation = vectorObj["rotation"].toDouble();
            vector.scale = vectorObj["scale"].toDouble();
            vector.color = QColor(vectorObj["color"].toString());
            vector.layer = vectorObj["layer"].toString();

            if (vectorObj.contains("location") && vectorObj["location"].isObject()) {
                QJsonObject locationObj = vectorObj["location"].toObject();
                vector.x = locationObj["x"].toDouble();
                vector.y = locationObj["y"].toDouble();
            } else {
                qDebug() << "Vector" << vector.name
                         << "is missing location data, defaulting to (0,0)";
                vector.x = 0;
                vector.y = 0;
            }

            snapshot.vectors.append(vector);
        }
        m_snapshots.append(snapshot);
    }
    qDebug() << "Successfully loaded" << m_snapshots.size() << "snapshots from" << filePath;
    return true;
}

void SerialParser::processJsonData(const QByteArray &jsonData)
{
    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(jsonData, &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        qDebug() << "JSON parse error:" << parseError.errorString();
        return;
    }

    QJsonObject frame = doc.object();

    qint64 timestamp = frame["timestamp"].toInteger();
    // Save snapshot for timeline replay
    FrameSnapshot snapshot;
    snapshot.timestamp = timestamp;

    snapshot.sensors = m_sensorModel ? m_sensorModel->getAllSensors() : QList<Sensor>();
    snapshot.vectors = m_vectorModel ? m_vectorModel->getAllVectors() : QList<Vector>();
    m_snapshots.append(snapshot);
    emit snapshotsChanged();

    if (frame.contains("sensors")) {
        updateSensorsFromJson(frame["sensors"].toArray());
    }

    if (frame.contains("vectors")) {
        updateVectorsFromJson(frame["vectors"].toArray());
    }
}

// Returns a list of all available timestamps
QList<qint64> SerialParser::availableTimestamps() const
{
    QList<qint64> result;
    for (const auto &snap : m_snapshots) {
        result.append(snap.timestamp);
    }
    return result;
}

// Restores the model state to the snapshot at the given index
bool SerialParser::restoreToIndex(int index)
{
    if (index < 0 || index >= m_snapshots.size())
        return false;
    if (m_sensorModel)
        m_sensorModel->clear();
    if (m_vectorModel)
        m_vectorModel->clear();
    disconnectPort();
    setModels(m_snapshots[index].sensors, m_snapshots[index].vectors);
    return true;
}

void SerialParser::updateSensorsFromJson(const QJsonArray &sensors)
{
    if (!m_sensorModel)
        return;

    for (const QJsonValue &sensorVal : sensors) {
        QJsonObject sensorObj = sensorVal.toObject();
        QString name = sensorObj["name"].toString("no name set");

        QJsonValue thresholdVal = sensorObj["threshold"];
        double threshold = thresholdVal.isBool() ? (thresholdVal.toBool() ? 1.0 : 0.0)
                                                 : thresholdVal.toDouble(-1);

        QJsonValue inputVal = sensorObj["input"];
        double input = inputVal.isBool() ? (inputVal.toBool() ? 1.0 : 0.0) : inputVal.toDouble(-1);

        bool isTriggered = sensorObj["isTriggered"].toBool(false);
        QString layer = sensorObj["layer"].toString("Layer 1");

        QJsonObject location = sensorObj["location"].toObject();
        double x = location["x"].toDouble(0.0);
        double y = location["y"].toDouble(0.0);

        int idx = m_sensorModel->getIndexByName(name);

        if (idx >= 0) {
            // Update existing sensor
            QModelIndex modelIdx = m_sensorModel->index(idx);
            m_sensorModel->setData(modelIdx, input, SensorModel::InputRole);
            m_sensorModel->setData(modelIdx, threshold, SensorModel::ThresholdRole);
            m_sensorModel->setData(modelIdx, isTriggered, SensorModel::TriggerRole);
            m_sensorModel->setData(modelIdx, x, SensorModel::XRole);
            m_sensorModel->setData(modelIdx, y, SensorModel::YRole);
            m_sensorModel->setData(modelIdx, layer, SensorModel::LayerRole);
        } else {
            // Add new sensor
            Sensor newSensor = m_sensorModel
                                   ->addSensor(name, input, threshold, isTriggered, layer, x, y);
        }
    }
}

void SerialParser::updateVectorsFromJson(const QJsonArray &vectors)
{
    if (!m_vectorModel)
        return;

    for (const QJsonValue &vectorVal : vectors) {
        QJsonObject vectorObj = vectorVal.toObject();
        QString name = vectorObj["name"].toString();
        double rotation = vectorObj["rotation"].toDouble();
        QString color = vectorObj["color"].toString("#ffffff");
        QString layer = vectorObj["layer"].toString("Layer 1");

        QJsonObject location = vectorObj["location"].toObject();
        double x = location["x"].toDouble();
        double y = location["y"].toDouble();

        int idx = m_vectorModel->getIndexByName(name);

        if (idx >= 0) {
            // Update existing vector
            QModelIndex modelIdx = m_vectorModel->index(idx);
            m_vectorModel->setData(modelIdx, rotation, VectorModel::RotationRole);
            m_vectorModel->setData(modelIdx, color, VectorModel::ColorRole);
            m_vectorModel->setData(modelIdx, x, VectorModel::XRole);
            m_vectorModel->setData(modelIdx, y, VectorModel::YRole);
            m_vectorModel->setData(modelIdx, layer, VectorModel::LayerRole);
        } else {
            // Add new vector
            m_vectorModel->addVector(name, rotation, 1.0, QColor(color), layer, x, y);
        }
    }
}
