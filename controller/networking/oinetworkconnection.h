#ifndef OINETWORKCONNECTION_H
#define OINETWORKCONNECTION_H

#include <QObject>
#include <QTcpSocket>
#include <QThread>
#include <QtXml>

#include "console.h"
#include "configuration.h"
#include "oirequestresponse.h"

class OiNetworkConnection : public QThread
{
    Q_OBJECT
public:
    explicit OiNetworkConnection(QObject *parent = 0);

    int getInternalRef();

signals:
    void sendRequest(OiRequestResponse *request);

public slots:
    bool setSocket(qintptr socketDescriptor);

private slots:
    void readMessage();

private:
    QTcpSocket* socket;
    int internalRef;

};

#endif // OINETWORKCONNECTION_H
