#ifndef SERIALPARSER_H
#define SERIALPARSER_H

#include <QObject>
#include <QQmlEngine>
#include <QUrl>

#include "SensorModel.h"
#include "SerialPortManager.h"
#include "VectorModel.h"
#include "parser/FrameDecoder.h"
#include "parser/SerialFrameExtractor.h"
#include "parser/SnapshotLoader.h"
#include "parser/SnapshotStore.h"

class SerialParser : public QObject {
  Q_OBJECT
  QML_ELEMENT

  Q_PROPERTY(SerialPortManager *portManager READ portManager WRITE
                 setPortManager NOTIFY portManagerChanged)

public:
  explicit SerialParser(QObject *parent = nullptr);

  Q_INVOKABLE void setModels(QList<Sensor> sensors, QList<Vector> vectors);
  Q_INVOKABLE void setModels(SensorModel *sensorModel,
                             VectorModel *vectorModel);

  Q_INVOKABLE bool saveToFile(QUrl filePath);
  Q_INVOKABLE bool loadFromFile(QUrl filePath);

  Q_INVOKABLE qint64 timestampAt(int index) const;
  Q_INVOKABLE QList<qint64> availableTimestamps() const;
  Q_INVOKABLE bool restoreToIndex(int index);
  Q_INVOKABLE int snapshotCount() const { return m_snapshotStore.count(); }

  SerialPortManager *portManager() const { return m_portManager; }
  void setPortManager(SerialPortManager *pm);

signals:
  void portManagerChanged();
  void dataReceived(const QByteArray &data);
  void snapshotsChanged();
  void errorOccurred(const QString &message);

private slots:
  void onRawDataReady(const QByteArray &data);

private:
  SerialPortManager *m_portManager = nullptr;
  SensorModel *m_sensorModel = nullptr;
  VectorModel *m_vectorModel = nullptr;
  SerialFrameExtractor m_frameExtractor;
  FrameDecoder m_frameDecoder;
  SnapshotStore m_snapshotStore;
  SnapshotLoader m_snapshotLoader;

  void processMsgPackData(const QByteArray &jsonData);
  void updateSensorsFromMsgPack(const QVariantList &sensors);
  void updateVectorsFromMsgPack(const QVariantList &vectors);
  static double roundTo2Decimals(float val);
};

#endif // SERIALPARSER_H
