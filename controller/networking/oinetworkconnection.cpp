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

/*!
 * \brief OiNetworkConnection::receiveResponse
 * Is called whenever a response to a request of this client is available
 * \param response
 */
void OiNetworkConnection::receiveResponse(OiRequestResponse *response){

    if(response == NULL){
        return;
    }

    this->socket->write(response->response.toByteArray());

    if(response->myRequestType != OiRequestResponse::eStartWatchwindow){
        delete response;
    }

}

/*!
 * \brief OiNetworkConnection::readMessage
 * Is called whenever a client has sent a message to OpenIndy
 */
void OiNetworkConnection::readMessage(){

    OiRequestResponse *request;
    request = new OiRequestResponse();
    request->requesterId = this->internalRef;

    QString xmlRequest = this->socket->readAll();

    qDebug() << "XML Request: " << xmlRequest;

    request->request.setContent(xmlRequest);

    emit this->sendRequest(request);

}
