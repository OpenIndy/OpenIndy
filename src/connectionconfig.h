#ifndef CONNECTIONCONFIG_H
#define CONNECTIONCONFIG_H

#include <QString>
#include "QtSerialPort/QSerialPort"

#include "types.h"

class ConnectionConfig
{
public:
    ConnectionConfig();

    ConnectionTypes typeOfConnection;

    QString ip;
    QString port;
    QString comPort;
    QSerialPort::BaudRate baudRate;
    QSerialPort::DataBits dataBits;
    QSerialPort::Parity parity;
    QSerialPort::StopBits stopBits;
    QSerialPort::FlowControl flowControl;
};

#endif // CONNECTIONCONFIG_H
