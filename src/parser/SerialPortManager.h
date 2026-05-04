#pragma once

#include <QObject>
#include <QQmlEngine>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QStringList>
#include <QTimer>

class SerialPortManager : public QObject {
  Q_OBJECT
  QML_ELEMENT

  Q_PROPERTY(bool isConnected READ isConnected NOTIFY connectionChanged)
  Q_PROPERTY(QString portName READ currentPort NOTIFY portChanged)
  Q_PROPERTY(QStringList availablePortsList READ availablePortsList NOTIFY
                 availablePortsChanged)

public:
  explicit SerialPortManager(QObject *parent = nullptr);

  bool isConnected() const { return m_serial.isOpen(); }
  QString currentPort() const { return m_serial.portName(); }
  QStringList availablePortsList() const { return m_availablePorts; }

  Q_INVOKABLE bool connectToPort();
  Q_INVOKABLE void disconnectPort();
  Q_INVOKABLE bool setBaudRate(int baudRate);
  Q_INVOKABLE bool setComPort(QString port);

signals:
  void connectionChanged();
  void portChanged();
  void availablePortsChanged();
  void parseRawData(const QByteArray &data);
  void errorOccurred(const QString &message);

private slots:
  void readData();
  void refreshPorts();
  void handleSerialError(QSerialPort::SerialPortError error);

private:
  void configureDefaultSettings();

  QSerialPort m_serial;
  QTimer m_portRefreshTimer;
  QStringList m_availablePorts;
};