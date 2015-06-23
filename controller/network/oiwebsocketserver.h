#ifndef OIWEBSOCKETSERVER_H
#define OIWEBSOCKETSERVER_H

#include <QWebSocketServer>
#include <QList>
#include <QPointer>

#include "console.h"
#include "oiwebsocket.h"
#include "oirequesthandler.h"

using namespace oi;

/*!
 * \brief The OiWebSocketServer class
 * Class to handle all web socket connections to OpenIndy
 */
class OiWebSocketServer : public QWebSocketServer
{
    Q_OBJECT

public:
    explicit OiWebSocketServer(QObject *parent = 0);

    static int generateUniqueId();

public slots:

    //#################
    //server connection
    //#################

    void startServer();
    void stopServer();
    void incomingConnection();

private slots:

    //##############################
    //send response to the requester
    //##############################

    void receiveResponse(OiRequestResponse response);

private:

    //#################
    //helper attributes
    //#################

    QList<QPointer<OiWebSocket> > usedSockets;
    static int currentId;

};

#endif // OIWEBSOCKETSERVER_H
