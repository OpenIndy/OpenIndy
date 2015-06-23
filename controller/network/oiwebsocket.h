#ifndef OIWEBSOCKET_H
#define OIWEBSOCKET_H

#include <QObject>
#include <QWebSocket>
#include <QThread>
#include <QtXml>
#include <QPointer>

#include "types.h"
#include "console.h"
#include "oirequestresponse.h"

using namespace oi;

/*!
 * \brief The OiWebSocket class
 * Represents a websocket connection of a client to OpenIndy
 */
class OiWebSocket : public QThread
{
    Q_OBJECT

public:
    explicit OiWebSocket(QObject *parent = 0);

    bool setSocket(const QPointer<QWebSocket> &mySocket);

    //#####################
    //get socket identifier
    //#####################

    int getInternalRef();

signals:

    //##########################
    //send a request to OpenIndy
    //##########################

    void sendRequest(OiRequestResponse request);

public slots:

    //##############################
    //recevie response from OpenIndy
    //##############################

    void receiveResponse(OiRequestResponse response);

private slots:

    //##############
    //helper methods
    //##############

    void readMessage(const QString &msg);

private:

    //#################
    //helper attributes
    //#################

    QPointer<QWebSocket> socket;
    int internalRef;

};

#endif // OIWEBSOCKET_H
