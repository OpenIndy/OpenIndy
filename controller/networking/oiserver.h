#ifndef OISERVER_H
#define OISERVER_H

#include <QTcpServer>
#include <QList>
#include "console.h"
#include "oinetworkconnection.h"

class OiServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit OiServer(QObject *parent = 0);

signals:
    void getProject(OiProjectData *d);

public slots:

    void startServer();
    void stopServer();
    void incomingConnection(qintptr socketDescriptor);

private slots:
    void sendGetProject(OiProjectData *d);

private:
    QList<QThread*> usedSockets;


};

#endif // OISERVER_H
