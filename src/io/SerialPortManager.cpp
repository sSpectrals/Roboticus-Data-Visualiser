#include "SerialPortManager.h"

SerialPortManager::SerialPortManager(QObject *parent) : QObject(parent) {
  refreshPorts();

  connect(&m_portRefreshTimer, &QTimer::timeout, this,
          &SerialPortManager::refreshPorts);
  m_portRefreshTimer.start(1000);

  connect(&m_serial, &QSerialPort::errorOccurred, this,
          &SerialPortManager::handleSerialError);
}

bool SerialPortManager::connectToPort() {
  if (m_serial.isOpen()) {
    m_serial.close();
  }

  if (m_serial.portName().isEmpty()) {
    qDebug() << "No COM port selected";
    emit errorOccurred(
        "No COM port selected. Please choose a port to connect.");
    return false;
  }

  if (m_serial.baudRate() <= 0) {
    qDebug() << "Failed to set baudrate";
    emit errorOccurred("Failed to set baud rate. Please check your settings.");
    return false;
  }

  configureDefaultSettings();

  bool success = m_serial.open(QIODevice::ReadOnly);

  if (success) {
    m_serial.setDataTerminalReady(true);
    m_serial.clear();

    connect(&m_serial, &QSerialPort::readyRead, this,
            &SerialPortManager::readData, Qt::UniqueConnection);

    qDebug() << "Successfully connected";
    qDebug() << "Port:" << m_serial.portName()
             << "Baud:" << m_serial.baudRate();
    emit connectionChanged();
    emit portChanged();
  } else {
    qDebug() << "Error:" << m_serial.error() << m_serial.errorString()
             << "\nCheck if you have a serial monitor open somewhere else";
    qDebug() << "Port:" << m_serial.portName()
             << "Baud:" << m_serial.baudRate();
    emit errorOccurred("Failed to connect to port. Please check if the port is "
                       "correct and not in use by another application.");
  }

  return success;
}

void SerialPortManager::configureDefaultSettings() {
  m_serial.setParity(QSerialPort::NoParity);
  m_serial.setDataBits(QSerialPort::Data8);
  m_serial.setStopBits(QSerialPort::OneStop);
  m_serial.setFlowControl(QSerialPort::NoFlowControl);
}

bool SerialPortManager::setBaudRate(int baudRate) {
  if (m_serial.isOpen()) {
    m_serial.close();
    m_serial.setBaudRate(baudRate);

    bool success = connectToPort();
    if (success) {
      emit connectionChanged();
    }
    return success;
  }

  return m_serial.setBaudRate(baudRate);
}

bool SerialPortManager::setComPort(QString port) {
  if (m_serial.isOpen()) {
    m_serial.close();
    m_serial.setPortName(port);

    bool success = connectToPort();
    if (success) {
      emit connectionChanged();
      emit portChanged();
    }
    return success;
  }

  m_serial.setPortName(port);
  return true;
}

void SerialPortManager::disconnectPort() {
  if (m_serial.isOpen()) {
    m_serial.close();
    emit connectionChanged();
    qDebug() << "Disconnected from" << m_serial.portName();
  }
}

void SerialPortManager::handleSerialError(QSerialPort::SerialPortError error) {
  if (error == QSerialPort::NoError || !m_serial.isOpen()) {
    return;
  }

  if (error == QSerialPort::ResourceError ||
      error == QSerialPort::DeviceNotFoundError) {
    qDebug() << "Serial connection lost:" << m_serial.errorString();
    m_serial.close();
    emit connectionChanged();
    emit errorOccurred("Serial connection lost.");
  }
}

void SerialPortManager::readData() {
  if (!m_serial.isOpen()) {
    qDebug() << "Could not read serial data";
    emit errorOccurred(
        "Could not read serial data. Please check your connection.");
    return;
  }

  emit rawDataReceived(m_serial.readAll());
}

void SerialPortManager::refreshPorts() {
  QStringList newPorts;
  const auto serialPorts = QSerialPortInfo::availablePorts();
  for (const QSerialPortInfo &info : serialPorts) {
    newPorts.append(info.portName());
  }

  if (newPorts != m_availablePorts) {
    m_availablePorts = newPorts;
    emit availablePortsChanged();
  }
}