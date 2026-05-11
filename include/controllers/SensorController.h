#pragma once

#include <QObject>
#include <QQmlEngine>
#include "../models/SensorModel.h"

/**
 * @brief Facade over SensorModel that provides a QML-friendly API for
 *        adding, removing, and updating sensors.
 *
 *        Owns the SensorModel and forwards its signals outward. Also emits
 *        clearChartSeries and sensorAdded when the active layer changes,
 *        so the graph can repopulate without knowing about the model directly.
 */
class SensorController : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    /** @brief The underlying list model, exposed for use in QML views and delegates. */
    Q_PROPERTY(SensorModel *model READ model CONSTANT)

    /** @brief Mirrors SensorModel::layers; updated whenever a new layer appears. */
    Q_PROPERTY(QList<QString> layers READ layers NOTIFY layersChanged)

public:
    explicit SensorController(QObject *parent = nullptr);

    SensorModel *model() const { return m_model; }
    QList<QString> layers() const { return m_model ? m_model->layers() : QList<QString>(); }

    /**
     * @brief Creates and registers a new sensor in the model.
     *        If name is empty, the model assigns a default name.
     * @return The newly created Sensor, including its generated id.
     */
    Q_INVOKABLE Sensor addSensor(const QString &name = QString(),
                                 double input = 0.0,
                                 double threshold = 100.0,
                                 const bool &isTriggered = false,
                                 QString layer = "Layer 1",
                                 double x = 0.0,
                                 double y = 0.0);

    /**
     * @brief Removes the sensor with the given id from the model.
     * @return True if found and removed, false if the id did not exist.
     */
    Q_INVOKABLE bool removeSensor(const QUuid &id);

    /**
     * @brief Updates the live input reading for a sensor.
     * @return True if the value changed, false if not found or unchanged.
     */
    Q_INVOKABLE bool setSensorValue(const QUuid &id, double value);

    /**
     * @brief Updates the threshold a sensor compares its input against.
     * @return True if the value changed, false if not found or unchanged.
     */
    Q_INVOKABLE bool setSensorThreshold(const QUuid &id, double threshold);

    /**
     * @brief Renames a sensor.
     * @return True if the name changed, false if not found or unchanged.
     */
    Q_INVOKABLE bool setSensorName(const QUuid &id, const QString &name);

    /**
     * @brief Updates the triggered state of a sensor.
     * @return True if the state changed, false if not found or unchanged.
     */
    Q_INVOKABLE bool setTriggered(const QUuid &id, const bool &isTriggered);

    /**
     * @brief Updates both X and Y position of a sensor in one call.
     * @return True if either coordinate changed, false if not found or both unchanged.
     */
    Q_INVOKABLE bool setSensorPositionXY(const QUuid &id, double x, double y);

    /**
     * @brief Updates the X position of a sensor.
     * @return True if the value changed, false if not found or unchanged.
     */
    Q_INVOKABLE bool setSensorPositionX(const QUuid &id, double x);

    /**
     * @brief Updates the Y position of a sensor.
     * @return True if the value changed, false if not found or unchanged.
     */
    Q_INVOKABLE bool setSensorPositionY(const QUuid &id, double y);

    /**
     * @brief Switches the active layer, emitting clearChartSeries followed by
     *        sensorAdded for every sensor that belongs to the new layer.
     *        This lets the graph repopulate without querying the model directly.
     * @param layerName The layer to switch to.
     */
    Q_INVOKABLE void setActiveLayer(const QString &layerName);

signals:
    /** @brief Forwarded from SensorModel::sensorAdded. Also emitted by setActiveLayer. */
    void sensorAdded(const QUuid &id,
                     const QString &name,
                     double input,
                     double threshold,
                     const bool &isTriggered,
                     const QString &layer,
                     double x,
                     double y);

    /** @brief Forwarded from SensorModel::sensorRemoved. */
    void sensorRemoved(const QUuid &id);

    /** @brief Forwarded from SensorModel::sensorUpdated. */
    void sensorUpdated(const QUuid &id,
                       const QString &name,
                       double input,
                       double threshold,
                       const bool &isTriggered,
                       const QString &layer,
                       double x,
                       double y);

    /** @brief Emitted by setActiveLayer before re-emitting sensors for the new layer. */
    void clearChartSeries();

    /** @brief Forwarded from SensorModel::layersChanged. */
    void layersChanged();

    /** @brief Emitted when a model operation fails, e.g. unknown id. */
    void errorOccurred(QString message);

private:
    SensorModel *m_model;
};
