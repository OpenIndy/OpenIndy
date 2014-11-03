#ifndef OISERVER_H
#define OISERVER_H

#include <QTcpServer>
#include <QList>

#include "console.h"
#include "oinetworkconnection.h"
#include "oirequesthandler.h"

class OiServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit OiServer(QObject *parent = 0);

public slots:
    void startServer();
    void stopServer();
    void incomingConnection(qintptr socketDescriptor);

private slots:
    void receiveResponse(OiRequestResponse *response);

private:
    QList<OiNetworkConnection*> usedSockets;

};

#endif // OISERVER_H
