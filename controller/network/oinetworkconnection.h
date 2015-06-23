#ifndef OINETWORKCONNECTION_H
#define OINETWORKCONNECTION_H

#include <QObject>
#include <QTcpSocket>
#include <QThread>
#include <QtXml>
#include <QPointer>

#include "types.h"
#include "console.h"
#include "oirequestresponse.h"

using namespace oi;

/*!
 * \brief The OiNetworkConnection class
 * Represents a socket connection of a client to OpenIndy
 */
class OiNetworkConnection : public QThread
{
    Q_OBJECT

public:
    explicit OiNetworkConnection(QObject *parent = 0);

    bool setSocket(qintptr socketDescriptor);

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

    void readMessage();

private:

    //#################
    //helper attributes
    //#################

    QPointer<QTcpSocket> socket;
    int internalRef;

};

#endif // OINETWORKCONNECTION_H
