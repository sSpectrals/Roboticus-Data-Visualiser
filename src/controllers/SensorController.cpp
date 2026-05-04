#include "SensorController.h"
#include "controllers/ControllerUtils.h"
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
    return setModelValueById(
        m_model, id, value, SensorModel::InputRole,
        [this](const QString &message) { emit errorOccurred(message); }, "value", "sensor");
}

Q_INVOKABLE bool SensorController::setSensorThreshold(const QUuid &id, double threshold)
{
    return setModelValueById(
        m_model, id, threshold, SensorModel::ThresholdRole,
        [this](const QString &message) { emit errorOccurred(message); }, "threshold", "sensor");
}

Q_INVOKABLE bool SensorController::setTriggered(const QUuid &id, const bool &isTriggered)
{
    return setModelValueById(
        m_model, id, isTriggered, SensorModel::TriggerRole,
        [this](const QString &message) { emit errorOccurred(message); }, "triggered", "sensor");
}

Q_INVOKABLE bool SensorController::setSensorName(const QUuid &id, const QString &name)
{
    return setModelValueById(
        m_model, id, name, SensorModel::NameRole,
        [this](const QString &message) { emit errorOccurred(message); }, "name", "sensor");
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
    return setModelValueById(
        m_model, id, x, SensorModel::XRole,
        [this](const QString &message) { emit errorOccurred(message); }, "position X", "sensor");
}

Q_INVOKABLE bool SensorController::setSensorPositionY(const QUuid &id, double y)
{
    return setModelValueById(
        m_model, id, y, SensorModel::YRole,
        [this](const QString &message) { emit errorOccurred(message); }, "position Y", "sensor");
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
