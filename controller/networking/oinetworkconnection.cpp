#include "oinetworkconnection.h"

OiNetworkConnection::OiNetworkConnection(QObject *parent) :
    QThread(parent)
{
    this->socket = new QTcpSocket();
    this->internalRef = Configuration::generateID();
}

/*!
 * \brief OiNetworkConnection::getInternalRef
 * \return
 */
int OiNetworkConnection::getInternalRef(){
   return this->internalRef;
}

/*!
 * \brief OiNetworkConnection::setSocket
 * \param socketDescriptor
 * \return
 */
bool OiNetworkConnection::setSocket(qintptr socketDescriptor){

    bool connectionCheck =  this->socket->setSocketDescriptor(socketDescriptor);
    connect(this->socket, SIGNAL(readyRead()), this, SLOT(readMessage()));
    return connectionCheck;

}

void OiNetworkConnection::readMessage()
{

    OiRequestResponse *request;
    request = new OiRequestResponse();

    QString xmlRequest = this->socket->readAll();

    qDebug() << "XML Request: " << xmlRequest;

    request->request.setContent(xmlRequest);

    emit this->sendRequest(request);

    return;









    //read incomming string
    QString msg = this->socket->readLine();    
    Console::addLine(msg);

    if(msg == "c"){
        this->socket->write("close connection");

        this->socket->close();

        socket->deleteLater();
        this->deleteLater();

    }else if(msg == "p"){

        Console::addLine("p entered");

        OiRequestResponse *test;
        test = new OiRequestResponse();

        qDebug() << "test: " << test->request.isNull();

        QDomElement elem = test->request.createElement("OiRequest");
        elem.setAttribute("id", OiRequestResponse::eGetActiveFeature);
        test->request.appendChild(elem);

        emit this->sendRequest(test);

    }else if(msg.compare("Hello Server")==0){

        this->socket->write("HelloClient");

    }
}
