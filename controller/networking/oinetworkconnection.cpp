#include "oinetworkconnection.h"

OiNetworkConnection::OiNetworkConnection(QObject *parent) :
    QThread(parent)
{
    this->socket = new QTcpSocket();

    connect(socket,SIGNAL(readyRead()),this,SLOT(readMessage()));
}

bool OiNetworkConnection::setSocket(qintptr socketDescriptor)
{
    bool connectionCheck =  socket->setSocketDescriptor(socketDescriptor);

    if(connectionCheck == true){
        this->socket->write("connected_to_OpenIndy");
    }

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
        OiProjectData d;

        d.setDevice(socket);
        d.setProjectName("streamData");

        OiProjectExchanger::saveProject(d);

    }else if(msg.compare("Hello Server")==0){

        this->socket->write("HelloClient");

    }
}
