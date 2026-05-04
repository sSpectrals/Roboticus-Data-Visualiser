#ifndef SENSORCONTROLLER_H
#define SENSORCONTROLLER_H

#include <QObject>
#include <QQmlEngine>
#include "../models/SensorModel.h"

class SensorController : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(SensorModel *model READ model CONSTANT)
    Q_PROPERTY(QList<QString> layers READ layers NOTIFY layersChanged)
public:
    explicit SensorController(QObject *parent = nullptr);

    SensorModel *model() const { return m_model; }
    QList<QString> layers() const { return m_model ? m_model->layers() : QList<QString>(); }

    Q_INVOKABLE Sensor addSensor(const QString &name = QString(),
                                 double input = 0.0,
                                 double threshold = 100.0,
                                 const bool &isTriggered = false,
                                 QString layer = "Layer 1",
                                 double x = 0.0,
                                 double y = 0.0);

    Q_INVOKABLE bool removeSensor(const QUuid &id);

    Q_INVOKABLE bool setSensorValue(const QUuid &id, double value);
    Q_INVOKABLE bool setSensorThreshold(const QUuid &id, double threshold);
    Q_INVOKABLE bool setSensorName(const QUuid &id, const QString &name);
    Q_INVOKABLE bool setTriggered(const QUuid &id, const bool &isTriggered);
    Q_INVOKABLE bool setSensorPositionXY(const QUuid &id, double x, double y);
    Q_INVOKABLE bool setSensorPositionX(const QUuid &id, double x);
    Q_INVOKABLE bool setSensorPositionY(const QUuid &id, double y);

    Q_INVOKABLE void setActiveLayer(const QString &layerName);

signals:
    void sensorAdded(const QUuid &id,
                     const QString &name,
                     double input,
                     double threshold,
                     const bool &isTriggered,
                     const QString &layer,
                     double x,
                     double y);
    void sensorRemoved(const QUuid &id);
    void sensorUpdated(const QUuid &id,
                       const QString &name,
                       double input,
                       double threshold,
                       const bool &isTriggered,
                       const QString &layer,
                       double x,
                       double y);
    void clearChartSeries();
    void layersChanged();
    void errorOccurred(QString message);

private:
    SensorModel *m_model;
};

#endif // SENSORCONTROLLER_H
