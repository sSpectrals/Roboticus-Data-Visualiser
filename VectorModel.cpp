#include "VectorModel.h"

VectorModel::VectorModel(QObject *parent) : QAbstractListModel(parent) {}

int VectorModel::rowCount(const QModelIndex &parent) const {
  Q_UNUSED(parent)
  return m_vectors.size();
}

QVariant VectorModel::data(const QModelIndex &index, int role) const {
  if (!index.isValid() || index.row() < 0 || index.row() >= m_vectors.size())
    return QVariant();

  const Vector &vector = m_vectors[index.row()];

  switch (role) {
  case IdRole:
    return vector.id;
  case NameRole:
    return vector.name;
  case RotationRole:
    return vector.rotation;
  case ScaleRole:
    return vector.scale;
  case ColorRole:
    return vector.color;
  case LayerRole:
    return vector.layer;
  case XRole:
    return vector.x;
  case YRole:
    return vector.y;
  }
  return QVariant();
}

bool VectorModel::setData(const QModelIndex &index, const QVariant &value,
                          int role) {
  if (!index.isValid() || index.row() < 0 || index.row() >= m_vectors.size())
    return false;

  Vector &vector = m_vectors[index.row()];
  bool changed = false;

  switch (role) {
  case NameRole:
    if (vector.name != value.toString()) {
      vector.name = value.toString();
      changed = true;
    }
    break;
  case RotationRole:
    if (vector.rotation != value.toDouble()) {
      vector.rotation = value.toDouble();
      changed = true;
    }
    break;
  case ScaleRole:
    if (vector.scale != value.toDouble()) {
      vector.scale = value.toDouble();
      changed = true;
    }
    break;
  case ColorRole:
    if (vector.color != value.toString()) {
      vector.color = value.toString();
      changed = true;
    }
    break;
  case LayerRole:
    if (vector.layer != value.toString()) {
      vector.layer = value.toString();
      changed = true;
    }
    break;
  case XRole:
    if (vector.x != value.toDouble()) {
      vector.x = value.toDouble();
      changed = true;
    }
    break;
  case YRole:
    if (vector.y != value.toDouble()) {
      vector.y = value.toDouble();
      changed = true;
    }
    break;
  default:
    return false;
  }

  if (changed) {
    // built in signal to update repeater/listView
    emit dataChanged(index, index, {role});

    // signal to update graph
    emit vectorUpdated(vector.id, vector.name, vector.rotation, vector.scale,
                       vector.color, vector.layer, vector.x, vector.y);
  }

  return changed;
}

void VectorModel::clear() {
  beginResetModel();
  m_vectors.clear();
  endResetModel();
}

QHash<int, QByteArray> VectorModel::roleNames() const {
  static const QHash<int, QByteArray> mapping{
      {IdRole, "id"},       {NameRole, "name"},   {RotationRole, "rotation"},
      {ScaleRole, "scale"}, {ColorRole, "color"}, {LayerRole, "layer"},
      {XRole, "x"},         {YRole, "y"}};
  return mapping;
}

Vector VectorModel::addVector(QString name, double rotation, double scale,
                              QColor color, QString layer, double x, double y) {
  Vector vector;
  vector.name = name;
  vector.rotation = rotation;
  vector.scale = scale;
  vector.color = color;
  vector.layer = layer;
  vector.x = x;
  vector.y = y;

  beginInsertRows(QModelIndex(), m_vectors.size(), m_vectors.size());
  m_vectors.append(vector);
  endInsertRows();

  emit vectorAdded(vector.id, vector.name, vector.rotation, vector.scale,
                   vector.color, vector.layer, vector.x, vector.y);
  return vector;
}

bool VectorModel::removeVector(const QUuid &id) {
  int i = getIndexFromId(id);
  if (i < 0)
    return false;

  beginRemoveRows(QModelIndex(), i, i);
  m_vectors.removeAt(i);
  endRemoveRows();

  emit vectorRemoved(id);
  return true;
}

Vector VectorModel::getVectorById(const QUuid &id) const {
  for (const Vector &vector : m_vectors) {
    if (vector.id == id)
      return vector;
  }
  return Vector();
}

int VectorModel::getIndexFromId(const QUuid &id) const {
  for (int i = 0; i < m_vectors.size(); ++i)
    if (m_vectors[i].id == id)
      return i;
  return -1;
}

int VectorModel::getIndexByName(const QString &name) const {
  for (int i = 0; i < m_vectors.size(); ++i)
    if (m_vectors[i].name == name)
      return i;
  return -1;
}