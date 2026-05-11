#pragma once

#include <QDebug>
#include <QObject>
#include <QQmlEngine>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QStringList>
#include <QTimer>

/**
 * @brief Manages the serial port connection, port discovery, and raw data
 *        forwarding. Exposed to QML via QML_ELEMENT.
 *
 *        Available ports are refreshed every second. When data arrives it is
 *        emitted as-is via rawDataReceived for the parser to handle.
 */
class SerialPortManager : public QObject {
    Q_OBJECT
    QML_ELEMENT

    /** @brief True when the serial port is open and active. */
    Q_PROPERTY(bool isConnected READ isConnected NOTIFY connectionChanged)

    /** @brief The name of the currently configured port, e.g. "COM3" or "ttyUSB0". */
    Q_PROPERTY(QString portName READ currentPort NOTIFY portChanged)

    /** @brief Live list of available port names, refreshed every second. */
    Q_PROPERTY(QStringList availablePortsList READ availablePortsList NOTIFY availablePortsChanged)

public:
    explicit SerialPortManager(QObject *parent = nullptr);

    bool isConnected() const { return m_serial.isOpen(); }
    QString currentPort() const { return m_serial.portName(); }
    QStringList availablePortsList() const { return m_availablePorts; }

    /**
     * @brief Opens the serial port with the current port name and baud rate.
     *        If already open, closes it first. Emits errorOccurred on failure.
     * @return True if the port was opened successfully, false otherwise.
     */
    Q_INVOKABLE bool connectToPort();

    /** @brief Closes the serial port if open and emits connectionChanged. */
    Q_INVOKABLE void disconnectPort();

    /**
     * @brief Sets the baud rate. If the port is currently open, reconnects
     *        immediately with the new rate.
     * @param baudRate The desired baud rate, e.g. 115200.
     * @return True on success, false if the baud rate could not be applied.
     */
    Q_INVOKABLE bool setBaudRate(int baudRate);

    /**
     * @brief Sets the port name. If the port is currently open, reconnects
     *        immediately on the new port.
     * @param port Port name, e.g. "COM3" or "ttyUSB0".
     * @return True on success, false if reconnection failed.
     */
    Q_INVOKABLE bool setComPort(QString port);

signals:
    /** @brief Emitted when the port opens or closes. */
    void connectionChanged();

    /** @brief Emitted when the active port name changes. */
    void portChanged();

    /** @brief Emitted when the list of available ports changes. */
    void availablePortsChanged();

    /**
     * @brief Emitted whenever bytes arrive on the serial port.
     *        Connect to SerialParser::onRawDataReady to decode frames.
     */
    void rawDataReceived(const QByteArray &data);

    /** @brief Emitted with a human-readable message when an error occurs. */
    void errorOccurred(const QString &message);

private slots:
    /** @brief Reads all available bytes from the port and emits rawDataReceived. */
    void readData();

    /** @brief Polls QSerialPortInfo and updates m_availablePorts if the list changed. */
    void refreshPorts();

    /** @brief Handles unexpected serial errors such as device disconnection. */
    void handleSerialError(QSerialPort::SerialPortError error);

private:
    /** @brief Applies fixed line settings: 8N1, no flow control. */
    void configureDefaultSettings();

    QSerialPort m_serial;
    QTimer m_portRefreshTimer;      ///< Fires every 1000ms to refresh available ports.
    QStringList m_availablePorts;   ///< Cached list of currently available port names.
};