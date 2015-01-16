#ifndef OIWEBSOCKETSERVER_H
#define OIWEBSOCKETSERVER_H

#include <QWebSocketServer>
#include <QList>

#include "console.h"
#include "oiwebsocket.h"
#include "oirequesthandler.h"

class OiWebSocketServer : public QWebSocketServer
{
    Q_OBJECT
public:
    explicit OiWebSocketServer(QObject *parent = 0);

public slots:
    void startServer();
    void stopServer();
    void incomingConnection();

private slots:
    void receiveResponse(OiRequestResponse *response);

private:
    QList<OiWebSocket*> usedSockets;

};

#endif // OIWEBSOCKETSERVER_H
