#ifndef OISERVER_H
#define OISERVER_H

#include <QTcpServer>
#include <QList>
#include <QPointer>

#include "console.h"
#include "oinetworkconnection.h"
#include "oirequesthandler.h"

/*!
 * \brief The OiServer class
 * Class to handle all socket connections to OpenIndy
 */
class OiServer : public QTcpServer
{
    Q_OBJECT

public:
    explicit OiServer(QObject *parent = 0);

    static int generateUniqueId();

public slots:

    //#################
    //server connection
    //#################

    void startServer();
    void stopServer();
    void incomingConnection(qintptr socketDescriptor);

private slots:

    //##############################
    //send response to the requester
    //##############################

    void receiveResponse(const QPointer<OiRequestResponse> &response);

private:

    //#################
    //helper attributes
    //#################

    QList<QPointer<OiNetworkConnection> > usedSockets;
    static int currentId;

};

#endif // OISERVER_H
