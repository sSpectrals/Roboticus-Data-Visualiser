#ifndef APPCONTROLLER_H
#define APPCONTROLLER_H

#include <QObject>
#include <QQmlEngine>
#include <QUrl>

#include "../include/SensorModel.h"
#include "../include/SerialParser.h"
#include "../include/SerialPortManager.h"
#include "../include/VectorModel.h"
#include "../include/parser/SnapshotLoader.h"
#include "../include/parser/SnapshotStore.h"
#include "parser/FrameTypes.h"

class AppController : public QObject {
  Q_OBJECT
  QML_ELEMENT

  // Expose the managers so QML can bind to their properties (e.g., isConnected)
  Q_PROPERTY(SerialPortManager *portManager READ portManager CONSTANT)
  Q_PROPERTY(SerialParser *parser READ parser CONSTANT)
  Q_PROPERTY(int snapshotCount READ snapshotCount NOTIFY snapshotsChanged)

public:
  explicit AppController(QObject *parent = nullptr);

  // Getters for QML
  SerialPortManager *portManager() const { return m_portManager; }
  SerialParser *parser() const { return m_parser; }
  int snapshotCount() const { return m_snapshotStore.count(); }

  // Initialization
  Q_INVOKABLE void setModels(SensorModel *sensorModel,
                             VectorModel *vectorModel);

  Q_INVOKABLE bool saveToFile(QUrl filePath);
  Q_INVOKABLE bool loadFromFile(QUrl filePath);

  // Business Logic - Timeline
  Q_INVOKABLE bool restoreToIndex(int index);
  Q_INVOKABLE qint64 timestampAt(int index) const;
  Q_INVOKABLE QList<qint64> availableTimestamps() const;

signals:
  void snapshotsChanged();
  void errorOccurred(const QString &message);

private slots:
  void onFrameParsed(const DecodedFrame &frame);

private:
  // Members
  SerialPortManager *m_portManager = nullptr;
  SerialParser *m_parser = nullptr;
  SnapshotStore m_snapshotStore;
  SnapshotLoader m_snapshotLoader;

  // UI Models (Weak references)
  SensorModel *m_sensorModel = nullptr;
  VectorModel *m_vectorModel = nullptr;

  void updateModelsFromFrame(const DecodedFrame &frame);
  void updateSensorsFromFrame(const QVariantList &sensors);
  void updateVectorsFromFrame(const QVariantList &vectors);
  double roundTo2Decimals(float val) { return qRound(val * 100) / 100.0; }
};

#endif // APPCONTROLLER_H