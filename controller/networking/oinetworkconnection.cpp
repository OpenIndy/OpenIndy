#include "oinetworkconnection.h"

OiNetworkConnection::OiNetworkConnection(QObject *parent) :
    QThread(parent)
{
    this->socket = new QTcpSocket();

}

bool OiNetworkConnection::setSocket(qintptr socketDescriptor)
{
    bool connectionCheck =  socket->setSocketDescriptor(socketDescriptor);

   connect(this->socket,SIGNAL(readyRead()),this,SLOT(readMessage()));

    return connectionCheck;
}

void OiNetworkConnection::readMessage()
{
    //read incomming string
    QString msg = this->socket->readLine();
    Console::addLine(msg);

    if(msg == "c"){
        this->socket->write("close connection");

        this->socket->close();

        socket->deleteLater();
        this->deleteLater();

    }else if(msg == "p"){

        OiProjectData *d = new OiProjectData();

        d->setDevice(socket);
        d->setProjectName("streamData");

        emit this->getProject(d);


    }else if(msg.compare("Hello Server")==0){

        this->socket->write("HelloClient");

    }
}
