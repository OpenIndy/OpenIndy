#include "oiserver.h"

OiServer::OiServer(QObject *parent) :
    QTcpServer(parent)
{
}

void OiServer::startServer()
{
    if(!this->listen(QHostAddress::Any,1234)){

        Console::addLine("could not start local server");

    }else{

        Console::addLine("local server ready ...");

    }
}

void OiServer::stopServer()
{

}

void OiServer::incomingConnection(qintptr socketDescriptor)
{
    Console::addLine("connecting to client " + QString::number(socketDescriptor) + " ...");

    OiNetworkConnection *n = new OiNetworkConnection();

    connect(n,SIGNAL(finished()),n,SLOT(deleteLater()));

    n->setSocket(socketDescriptor);

    usedSockets.append(n);

    connect(n,SIGNAL(getProject(OiProjectData*)),this,SLOT(sendGetProject(OiProjectData*)));

    n->start();

}

void OiServer::sendGetProject(OiProjectData *d)
{
    emit this->getProject(d);
}
