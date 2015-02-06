#ifndef OIWEBSOCKET_H
#define OIWEBSOCKET_H

#include <QObject>
#include <QWebSocket>
#include <QThread>
#include <QtXml>

#include "console.h"
#include "configuration.h"
#include "oirequestresponse.h"

class OiWebSocket : public QThread
{
    Q_OBJECT
public:
    explicit OiWebSocket(QObject *parent = 0);

    int getInternalRef();

signals:
    void sendRequest(OiRequestResponse *request);

public slots:
    bool setSocket(QWebSocket *mySocket);
    void receiveResponse(OiRequestResponse *response);

private slots:
    void readMessage(QString msg);

    //void socketDisconnected();
    //void stateChanged(QAbstractSocket::SocketState);

private:
    QWebSocket* socket;
    int internalRef;

};

#endif // OIWEBSOCKET_H
