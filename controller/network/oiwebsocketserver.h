#ifndef OIWEBSOCKETSERVER_H
#define OIWEBSOCKETSERVER_H

#include <QWebSocketServer>
#include <QList>
#include <QPointer>

#include "types.h"
#include "oiwebsocket.h"
#include "oirequesthandler.h"
#include "oiwebsocketstreamer.h"

// define OI_WEBSOCKETSERVER to enable
#undef OI_WEBSOCKETSERVER

/*!
 * \brief The OiWebSocketServer class
 * Class to handle all web socket connections to OpenIndy
 */
class OiWebSocketServer : public QWebSocketServer
{
    Q_OBJECT

public:
    explicit OiWebSocketServer(QObject *parent = 0);
    ~OiWebSocketServer();

    static int generateUniqueId();

signals:

    //#########################
    //send messages to OpenIndy
    //#########################

    void sendMessage(const QString &msg, const MessageTypes &msgType, const MessageDestinations &msgDest = eConsoleMessage);

    //##########################
    //send a request to OpenIndy
    //##########################

    void sendRequest(oi::OiRequestResponse request);

    //##############################
    //send response to the requester
    //##############################

    void receiveResponse(const OiRequestResponse &response);

public slots:

    //#################
    //server connection
    //#################

    void startServer();
    void stopServer();

private slots:

    //######################
    //new client connections
    //######################

    void incomingConnection();

private:

    //#################
    //helper attributes
    //#################

    QPointer<OiWebSocketStreamer> streamer;

    static int currentId;
    QList<QPointer<OiWebSocket> > usedSockets;

};

#endif // OIWEBSOCKETSERVER_H
