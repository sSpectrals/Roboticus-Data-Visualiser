#include "VectorController.h"

VectorController::VectorController(QObject *parent)
    : QObject(parent), m_model(new VectorModel(this)) {

  connect(m_model, &VectorModel::vectorAdded, this,
          &VectorController::vectorAdded);
  connect(m_model, &VectorModel::vectorRemoved, this,
          &VectorController::vectorRemoved);
  connect(m_model, &VectorModel::vectorUpdated, this,
          &VectorController::vectorUpdated);
}

Q_INVOKABLE Vector VectorController::addVector(const QString &name,
                                               double rotation, double scale,
                                               QColor color, QString layer,
                                               double x, double y) {

  if (!m_model)
    return Vector();

  Vector newVector =
      m_model->addVector(name, rotation, scale, color, layer, x, y);

  return newVector;
}

Q_INVOKABLE bool VectorController::removeVector(const QUuid &id) {

  if (!m_model)
    return false;

  Vector removedVector = m_model->getVectorById(id);

  return m_model->removeVector(id);
}

Q_INVOKABLE bool VectorController::setVectorRotation(const QUuid &id,
                                                     double rotation) {

  if (!m_model)
    return false;

  int index = m_model->getIndexFromId(id);
  if (index < 0) {
    qWarning() << "Attempted to set rotation for non-existent vector with id:"
               << id;
    return false;
  }

  return m_model->setData(m_model->index(index), rotation,
                          VectorModel::RotationRole);
}

Q_INVOKABLE bool VectorController::setVectorScale(const QUuid &id,
                                                  double scale) {

  if (!m_model)
    return false;

  int index = m_model->getIndexFromId(id);
  if (index < 0) {
    qWarning() << "Attempted to set scale for non-existent vector with id:"
               << id;
    return false;
  }

  return m_model->setData(m_model->index(index), scale, VectorModel::ScaleRole);
}

Q_INVOKABLE bool VectorController::setVectorColor(const QUuid &id,
                                                  const QColor &color) {

  if (!m_model)
    return false;

  int index = m_model->getIndexFromId(id);
  if (index < 0) {
    qWarning() << "Attempted to set color for non-existent vector with id:"
               << id;
    return false;
  }

  return m_model->setData(m_model->index(index), color, VectorModel::ColorRole);
}

Q_INVOKABLE bool VectorController::setVectorName(const QUuid &id,
                                                 const QString &name) {

  if (!m_model)
    return false;

  int index = m_model->getIndexFromId(id);
  if (index < 0) {
    qWarning() << "Attempted to set name for non-existent vector with id:"
               << id;
    return false;
  }

  return m_model->setData(m_model->index(index), name, VectorModel::NameRole);
}

Q_INVOKABLE bool VectorController::setVectorPositionXY(const QUuid &id,
                                                       double x, double y) {

  if (!m_model)
    return false;

  int index = m_model->getIndexFromId(id);
  if (index < 0) {
    qWarning() << "Attempted to set position for non-existent vector with id:"
               << id;
    return false;
  }

  bool xChanged =
      m_model->setData(m_model->index(index), x, VectorModel::XRole);
  bool yChanged =
      m_model->setData(m_model->index(index), y, VectorModel::YRole);

  return xChanged || yChanged;
}

Q_INVOKABLE bool VectorController::setVectorPositionX(const QUuid &id,
                                                      double x) {

  if (!m_model)
    return false;

  int index = m_model->getIndexFromId(id);
  if (index < 0) {
    qWarning() << "Attempted to set position X for non-existent vector with id:"
               << id;
    return false;
  }

  return m_model->setData(m_model->index(index), x, VectorModel::XRole);
}

Q_INVOKABLE bool VectorController::setVectorPositionY(const QUuid &id,
                                                      double y) {

  if (!m_model)
    return false;

  int index = m_model->getIndexFromId(id);
  if (index < 0) {
    qWarning() << "Attempted to set position Y for non-existent vector with id:"
               << id;
    return false;
  }

  return m_model->setData(m_model->index(index), y, VectorModel::YRole);
}
