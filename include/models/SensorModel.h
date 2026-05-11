#ifndef SENSORMODEL_H
#define SENSORMODEL_H

#include <QAbstractListModel>
#include <QQmlEngine>
#include <QUuid>

/**
 * @brief Represents a single sensor with its properties and position.
 *        The id is automatically generated on construction.
 */
struct Sensor
{
    QUuid id;
    QString name = "No Name Set";
    double inputValue = -1.0;
    double threshold = -1.0;
    bool isTriggered = false;
    QString layer = "Layer 1";
    double x = 0.0;
    double y = 0.0;

    Sensor()
        : id(QUuid::createUuid())
    {}
};

/**
 * @brief List model that stores and manages sensors, exposed to QML via QML_ELEMENT.
 *        Emits signals when sensors are added, removed, or updated so other
 *        components (e.g. the graph) can react without polling the model.
 */
class SensorModel : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT

    /**
     * @brief The list of unique layer names currently present in the model.
     *        Updated automatically when a sensor with a new layer is added.
     */
    Q_PROPERTY(QList<QString> layers READ layers NOTIFY layersChanged)

public:
    /**
     * @brief Roles used to access sensor fields from QML delegates.
     */
    enum Roles {
        IdRole = Qt::UserRole + 1,
        NameRole,
        InputRole,
        ThresholdRole,
        TriggerRole,
        LayerRole,
        XRole,
        YRole
    };
    Q_ENUM(Roles)

    explicit SensorModel(QObject *parent = nullptr);

    /** @brief Returns the list of unique layer names across all sensors. */
    QList<QString> layers() const { return m_layers; }

    /** @brief Returns all sensors as a flat list, e.g. for serialization. */
    QList<Sensor> getAllSensors() const { return m_sensors; }

    // QAbstractListModel overrides
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    virtual QHash<int, QByteArray> roleNames() const override;

    /**
     * @brief Creates a new sensor, appends it to the model, and emits sensorAdded.
     *        If name is empty, a default name is generated based on the current count.
     * @return The newly created Sensor, including its generated id.
     */
    Q_INVOKABLE Sensor addSensor(QString name = QString(),
                                 double input = 0.0,
                                 double threshold = 100.0,
                                 bool isTriggered = false,
                                 QString layer = "Layer 1",
                                 double x = 0.0,
                                 double y = 0.0);

    /**
     * @brief Removes the sensor with the given id from the model and emits sensorRemoved.
     * @return True if the sensor was found and removed, false if the id was not found.
     */
    Q_INVOKABLE bool removeSensor(const QUuid &id);

    /** @brief Removes all sensors from the model and resets it. */
    Q_INVOKABLE void clear();

    /**
     * @brief Returns the row index of the sensor with the given id.
     * @return The zero-based index, or -1 if not found.
     */
    Q_INVOKABLE int getIndexFromId(const QUuid &id) const;

    /**
     * @brief Returns the sensor with the given id.
     * @return The matching Sensor, or a default-constructed Sensor if not found.
     */
    Q_INVOKABLE Sensor getSensorById(const QUuid &id) const;

    /**
     * @brief Returns the row index of the first sensor with the given name.
     * @return The zero-based index, or -1 if not found.
     */
    Q_INVOKABLE int getIndexByName(const QString &name) const;

signals:
    /** @brief Emitted after a sensor is successfully added to the model. */
    void sensorAdded(const QUuid &id,
                     const QString &name,
                     double input,
                     double threshold,
                     const bool &isTriggered,
                     const QString &layer,
                     double x,
                     double y);

    /** @brief Emitted after a sensor is successfully removed from the model. */
    void sensorRemoved(const QUuid &id);

    /**
     * @brief Emitted after any property of an existing sensor changes via setData.
     *        Used by the graph to update the visual representation of the sensor.
     */
    void sensorUpdated(const QUuid &id,
                       const QString &name,
                       double input,
                       double threshold,
                       const bool &isTriggered,
                       const QString &layer,
                       double x,
                       double y);

    /** @brief Emitted when a sensor with a previously unseen layer name is added. */
    void layersChanged();

private:
    QList<Sensor> m_sensors;
    QList<QString> m_layers;
};

#endif // SENSORMODEL_H