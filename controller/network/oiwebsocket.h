#ifndef OIWEBSOCKET_H
#define OIWEBSOCKET_H

#include <QObject>
#include <QWebSocket>
#include <QtXml>
#include <QPointer>

#include "types.h"
#include "oirequestresponse.h"

/*!
 * \brief The OiWebSocket class
 * Represents a websocket connection of a client to OpenIndy
 */
class OiWebSocket : public QObject
{
    Q_OBJECT

public:
    explicit OiWebSocket(QObject *parent = 0);
    ~OiWebSocket();

    //##########
    //set socket
    //##########

    bool setSocket(const QPointer<QWebSocket> &mySocket);

    //#####################
    //get socket identifier
    //#####################

    int getInternalRef() const;

signals:

    //###############################
    //inform about connection changes
    //###############################

    void connected();
    void disconnected();

    //##########################
    //send a request to OpenIndy
    //##########################

    void sendRequest(const oi::OiRequestResponse &request);

public slots:

    //#######################
    //close socket connection
    //#######################

    void close();

    //##############################
    //recevie response from OpenIndy
    //##############################

    void receiveResponse(const oi::OiRequestResponse &response);

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
