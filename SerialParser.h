#ifndef SERIALPARSER_H
#define SERIALPARSER_H

#include <QDebug>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QObject>
#include <QQmlEngine>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QTimer>
#include <QUrl>

#include "SensorModel.h"
#include "VectorModel.h"

struct FrameSnapshot {
  qint64 timestamp;
  QList<Sensor> sensors;
  QList<Vector> vectors;
};

class SerialParser : public QObject {
  Q_OBJECT
  QML_ELEMENT

  Q_PROPERTY(bool isConnected READ isConnected NOTIFY connectionChanged)
  Q_PROPERTY(QString currentPort READ currentPort NOTIFY portChanged)
  Q_PROPERTY(QStringList availablePortsList READ availablePortsList NOTIFY
                 availablePortsChanged)

public:
  explicit SerialParser(QObject *parent = nullptr);

  Q_INVOKABLE bool connectToPort();

  Q_INVOKABLE bool setBaudRate(int baudRate);
  Q_INVOKABLE bool setComPort(QString port);
  Q_INVOKABLE void disconnectPort();

  QStringList availablePortsList() const { return m_availablePorts; }
  Q_INVOKABLE void refreshPorts();

  bool isConnected() const { return m_serial.isOpen(); }
  QString currentPort() const { return m_serial.portName(); }

  Q_INVOKABLE QList<qint64> availableTimestamps() const;
  Q_INVOKABLE bool restoreToIndex(int index);

  Q_INVOKABLE void setModels(QList<Sensor> sensors, QList<Vector> vectors);
  Q_INVOKABLE void setModels(SensorModel *sensorModel,
                             VectorModel *vectorModel);
  Q_INVOKABLE void readData();

  Q_INVOKABLE int snapshotCount() const { return m_snapshots.size(); }
  Q_INVOKABLE qint64 timestampAt(int index) const;

  Q_INVOKABLE bool saveToFile(QUrl filePath);
  Q_INVOKABLE bool loadFromFile(QUrl filePath);

signals:
  void connectionChanged();
  void portChanged();
  void dataReceived(QByteArray data);
  void availablePortsChanged();
  void snapshotsChanged();
  void errorOccurred(QString message);

private:
  QStringList m_availablePorts;
  QTimer m_portRefreshTimer;

  QList<FrameSnapshot> m_snapshots;
  QSerialPort m_serial;
  QByteArray m_buffer;
  SensorModel *m_sensorModel = nullptr;
  VectorModel *m_vectorModel = nullptr;

  void configureDefaultSettings();
  void processJsonData(const QByteArray &jsonData);
  void processMsgPackData(const QByteArray &jsonData);
  void updateSensorsFromVariant(const QVariantList &sensors);
  void updateVectorsFromVariant(const QVariantList &vectors);
  void updateSensorsFromJson(const QJsonArray &sensors);
  void updateVectorsFromJson(const QJsonArray &vectors);
  double roundTo2Decimals(float val);
  QStringList availablePorts();
};

#endif // SERIALPARSER_H
