#include "SensorModel.h"

SensorModel::SensorModel(QObject *parent) : QAbstractListModel(parent) {}

int SensorModel::rowCount(const QModelIndex &parent) const {
  Q_UNUSED(parent)
  return m_sensors.size();
}

QVariant SensorModel::data(const QModelIndex &index, int role) const {
  if (!index.isValid() || index.row() < 0 || index.row() >= m_sensors.size())
    return QVariant();

  const Sensor &sensor = m_sensors[index.row()];

  switch (role) {
  case IdRole:
    return sensor.id;
  case NameRole:
    return sensor.name;
  case InputRole:
    return sensor.inputValue;
  case ThresholdRole:
    return sensor.threshold;
  case TriggerRole:
    return sensor.isTriggered;
  case LayerRole:
    return sensor.layer;
  case XRole:
    return sensor.x;
  case YRole:
    return sensor.y;
  }
  return QVariant();
}

bool SensorModel::setData(const QModelIndex &index, const QVariant &value,
                          int role) {
  if (!index.isValid() || index.row() < 0 || index.row() >= m_sensors.size())
    return false;

  Sensor &sensor = m_sensors[index.row()];
  bool changed = false;

  switch (role) {
  case NameRole:
    if (sensor.name != value.toString()) {
      sensor.name = value.toString();
      changed = true;
    }
    break;
  case InputRole:
    if (sensor.inputValue != value.toDouble()) {
      sensor.inputValue = value.toDouble();
      changed = true;
    }
    break;
  case ThresholdRole:
    if (sensor.threshold != value.toDouble()) {
      sensor.threshold = value.toDouble();
      changed = true;
    }
    break;
  case TriggerRole:
    if (sensor.isTriggered != value.toBool()) {
      sensor.isTriggered = value.toBool();
      changed = true;
    }
    break;
  case XRole:
    if (sensor.x != value.toDouble()) {
      sensor.x = value.toDouble();
      changed = true;
    }
    break;
  case YRole:
    if (sensor.y != value.toDouble()) {
      sensor.y = value.toDouble();
      changed = true;
    }
    break;
  case LayerRole:
    if (sensor.layer != value.toString()) {
      sensor.layer = value.toString();
      changed = true;
    }
    break;
  default:
    return false;
  }

  if (changed) {
    // for graph update
    emit sensorUpdated(sensor.id, sensor.name, sensor.inputValue,
                       sensor.threshold, sensor.isTriggered, sensor.layer,
                       sensor.x, sensor.y);
    // for listView/repeater, built in signal so not really defined in header
    emit dataChanged(index, index, {role});
  }

  return changed;
}

void SensorModel::clear() {
  beginResetModel();
  m_sensors.clear();
  endResetModel();
}

QHash<int, QByteArray> SensorModel::roleNames() const {
  static const QHash<int, QByteArray> mapping{{IdRole, "id"},
                                              {NameRole, "name"},
                                              {InputRole, "inputValue"},
                                              {ThresholdRole, "threshold"},
                                              {TriggerRole, "isTriggered"},
                                              {LayerRole, "layer"},
                                              {XRole, "x"},
                                              {YRole, "y"}};
  return mapping;
}

Sensor SensorModel::addSensor(QString name, double input, double threshold,
                              bool isTriggered, QString layer, double x,
                              double y) {

  Sensor sensor;
  sensor.name =
      name.isEmpty() ? "Sensor " + QString::number(m_sensors.size() + 1) : name;
  sensor.inputValue = input;
  sensor.threshold = threshold;
  sensor.isTriggered = isTriggered;
  sensor.layer = layer;
  sensor.x = x;
  sensor.y = y;

  beginInsertRows(QModelIndex(), m_sensors.size(), m_sensors.size());
  m_sensors.append(sensor);
  endInsertRows();
  emit sensorAdded(sensor.id, sensor.name, sensor.inputValue, sensor.threshold,
                   sensor.isTriggered, sensor.layer, sensor.x, sensor.y);
  return sensor;
}

bool SensorModel::removeSensor(const QUuid &id) {
  int i = getIndexFromId(id);
  if (i < 0) {
    return false;
  }

  beginRemoveRows(QModelIndex(), i, i);
  m_sensors.removeAt(i);
  endRemoveRows();

  emit sensorRemoved(id);
  return true;
}

Sensor SensorModel::getSensorById(const QUuid &id) const {
  for (const auto &sensor : m_sensors) {
    if (sensor.id == id)
      return sensor;
  }
  return Sensor();
}

int SensorModel::getIndexFromId(const QUuid &id) const {
  for (int i = 0; i < m_sensors.size(); ++i)
    if (m_sensors[i].id == id)
      return i;
  return -1;
}

int SensorModel::getIndexByName(const QString &name) const {
  for (int i = 0; i < m_sensors.size(); ++i)
    if (m_sensors[i].name == name)
      return i;
  return -1;
}
