#ifndef OINETWORKCONNECTION_H
#define OINETWORKCONNECTION_H

#include <QObject>
#include <QTcpSocket>
#include <QThread>
#include "console.h"
#include "oiprojectdata.h"
#include "oiprojectexchanger.h"

class OiNetworkConnection : public QThread
{
    Q_OBJECT
public:
    explicit OiNetworkConnection(QObject *parent = 0);

signals:

public slots:
    bool setSocket(qintptr socketDescriptor);

private slots:

    void readMessage();

private:
    QTcpSocket* socket;



};

#endif // OINETWORKCONNECTION_H
