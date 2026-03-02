#ifndef VECTORCONTROLLER_H
#define VECTORCONTROLLER_H

#include <QObject>
#include <QQmlEngine>
#include <SensorModel.h>
#include <VectorModel.h>

class VectorController : public QObject {
  Q_OBJECT
  QML_ELEMENT

  Q_PROPERTY(VectorModel *model READ model CONSTANT)
public:
  explicit VectorController(QObject *parent = nullptr);

  VectorModel *model() const { return m_model; }

  Q_INVOKABLE Vector addVector(const QString &name = QString(),
                               double rotation = 0, double scale = 1,
                               QColor color = "white",
                               QString layer = "Layer 1", double x = 0.0,
                               double y = 0.0);
  Q_INVOKABLE bool removeVector(const QUuid &id);

  Q_INVOKABLE bool setVectorRotation(const QUuid &id, double rotation);
  Q_INVOKABLE bool setVectorScale(const QUuid &id, double scale);
  Q_INVOKABLE bool setVectorColor(const QUuid &id, const QColor &color);
  Q_INVOKABLE bool setVectorName(const QUuid &id, const QString &name);
  Q_INVOKABLE bool setVectorPositionXY(const QUuid &id, double x, double y);
  Q_INVOKABLE bool setVectorPositionX(const QUuid &id, double x);
  Q_INVOKABLE bool setVectorPositionY(const QUuid &id, double y);

signals:
  void vectorAdded(const QUuid &id, const QString &name, double rotation,
                   double scale, const QColor &color, const QString &layer,
                   double x, double y);
  void vectorRemoved(const QUuid &id);
  void vectorUpdated(const QUuid &id, const QString &name, double rotation,
                     double scale, const QColor &color, const QString &layer,
                     double x, double y);

private:
  VectorModel *m_model;
};

#endif // VECTORCONTROLLER_H
