#ifndef SENSORMODEL_H
#define SENSORMODEL_H

#include <QAbstractListModel>
#include <QQmlEngine>
#include <QUuid>

struct Sensor {
  QUuid id;
  QString name = "No Name Set";
  double inputValue = -1.0;
  double threshold = -1.0;
  bool isTriggered = false;
  QString layer = "Layer 1";
  double x = 0.0;
  double y = 0.0;

  Sensor() : id(QUuid::createUuid()) {}
};

class SensorModel : public QAbstractListModel {
  Q_OBJECT
  QML_ELEMENT
public:
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

  virtual int
  rowCount(const QModelIndex &parent = QModelIndex()) const override;
  virtual QVariant data(const QModelIndex &index, int role) const override;
  virtual bool setData(const QModelIndex &index, const QVariant &value,
                       int role) override;
  virtual QHash<int, QByteArray> roleNames() const override;

  Q_INVOKABLE Sensor addSensor(QString name = QString(), double input = 0.0,
                               double threshold = 100.0,
                               bool isTriggered = false,
                               QString layer = "Layer 1", double x = 0.0,
                               double y = 0.0);

  Q_INVOKABLE bool removeSensor(const QUuid &id);
  Q_INVOKABLE void clear();

  Q_INVOKABLE int getIndexFromId(const QUuid &id) const;
  Q_INVOKABLE Sensor getSensorById(const QUuid &id) const;
  Q_INVOKABLE int getIndexByName(const QString &name) const;
  QList<Sensor> getAllSensors() const { return m_sensors; }

signals:
  void sensorAdded(const QUuid &id, const QString &name, double input,
                   double threshold, const bool &isTriggered,
                   const QString &layer, double x, double y);
  void sensorRemoved(const QUuid &id);
  void sensorUpdated(const QUuid &id, const QString &name, double input,
                     double threshold, const bool &isTriggered,
                     const QString &layer, double x, double y);

private:
  QList<Sensor> m_sensors;
};

#endif // SENSORMODEL_H
