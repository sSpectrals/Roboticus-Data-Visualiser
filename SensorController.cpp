#include "SensorController.h"
#include <QDebug>

SensorController::SensorController(QObject *parent)
    : QObject(parent)
    , m_model(new SensorModel(this))
{
    connect(m_model, &SensorModel::sensorAdded, this, &SensorController::sensorAdded);
    connect(m_model, &SensorModel::sensorRemoved, this, &SensorController::sensorRemoved);

    connect(m_model, &SensorModel::sensorUpdated, this, &SensorController::sensorUpdated);
    connect(m_model, &SensorModel::layersChanged, this, &SensorController::layersChanged);
}

Q_INVOKABLE Sensor SensorController::addSensor(const QString &name,
                                               double input,
                                               double threshold,
                                               const bool &isTriggered,
                                               QString layer,
                                               double x,
                                               double y)
{
    if (!m_model)
        return Sensor();

    Sensor newSensor = m_model->addSensor(name, input, threshold, isTriggered, layer, x, y);

    return newSensor;
}

Q_INVOKABLE bool SensorController::removeSensor(const QUuid &id)
{
    if (!m_model)
        return false;

    Sensor removedSensor = m_model->getSensorById(id);

    return m_model->removeSensor(id);
}

Q_INVOKABLE bool SensorController::setSensorValue(const QUuid &id, double value)
{
    if (!m_model)
        return false;

    int index = m_model->getIndexFromId(id);
    if (index < 0) {
        emit errorOccurred(QString("Attempted to set value for non-existent sensor with id: %1")
                               .arg(id.toString()));
        return false;
    }

    return m_model->setData(m_model->index(index), value, SensorModel::InputRole);
}

Q_INVOKABLE bool SensorController::setSensorThreshold(const QUuid &id, double threshold)
{
    if (!m_model)
        return false;

    int index = m_model->getIndexFromId(id);
    if (index < 0) {
        emit errorOccurred(QString("Attempted to set threshold for non-existent sensor with id: %1")
                               .arg(id.toString()));
        return false;
    }

    return m_model->setData(m_model->index(index), threshold, SensorModel::ThresholdRole);
}

Q_INVOKABLE bool SensorController::setTriggered(const QUuid &id, const bool &isTriggered)
{
    if (!m_model)
        return false;
    int index = m_model->getIndexFromId(id);
    if (index < 0) {
        emit errorOccurred(QString("Attempted to set triggered for non-existent sensor with id: %1")
                               .arg(id.toString()));
        return false;
    }

    return m_model->setData(m_model->index(index), isTriggered, SensorModel::TriggerRole);
}

Q_INVOKABLE bool SensorController::setSensorName(const QUuid &id, const QString &name)
{
    if (!m_model)
        return false;

    int index = m_model->getIndexFromId(id);
    if (index < 0) {
        emit errorOccurred(
            QString("Attempted to set name for non-existent sensor with id: %1").arg(id.toString()));
        return false;
    }

    return m_model->setData(m_model->index(index), name, SensorModel::NameRole);
}

Q_INVOKABLE bool SensorController::setSensorPositionXY(const QUuid &id, double x, double y)
{
    if (!m_model)
        return false;

    int index = m_model->getIndexFromId(id);
    if (index < 0) {
        emit errorOccurred(QString("Attempted to set position for non-existent sensor with id: %1")
                               .arg(id.toString()));
        return false;
    }

    bool xChanged = m_model->setData(m_model->index(index), x, SensorModel::XRole);
    bool yChanged = m_model->setData(m_model->index(index), y, SensorModel::YRole);

    return xChanged || yChanged;
}

Q_INVOKABLE bool SensorController::setSensorPositionX(const QUuid &id, double x)
{
    if (!m_model)
        return false;

    int index = m_model->getIndexFromId(id);
    if (index < 0) {
        emit errorOccurred(
            QString("Attempted to set position X for non-existent sensor with id: %1")
                .arg(id.toString()));
        return false;
    }

    return m_model->setData(m_model->index(index), x, SensorModel::XRole);
}

Q_INVOKABLE bool SensorController::setSensorPositionY(const QUuid &id, double y)
{
    if (!m_model)
        return false;

    int index = m_model->getIndexFromId(id);
    if (index < 0) {
        emit errorOccurred(
            QString("Attempted to set position Y for non-existent sensor with id: %1")
                .arg(id.toString()));
        return false;
    }

    return m_model->setData(m_model->index(index), y, SensorModel::YRole);
}

Q_INVOKABLE void SensorController::setActiveLayer(const QString &layerName)
{
    if (!m_model)
        return;

    emit clearChartSeries();

    const auto &sensors = m_model->getAllSensors();
    for (const auto &sensor : sensors) {
        if (sensor.layer == layerName) {
            emit sensorAdded(sensor.id,
                             sensor.name,
                             sensor.inputValue,
                             sensor.threshold,
                             sensor.isTriggered,
                             sensor.layer,
                             sensor.x,
                             sensor.y);
        }
    }
}
