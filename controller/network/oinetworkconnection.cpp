#include "oinetworkconnection.h"

#include "oiserver.h"

/*!
 * \brief OiNetworkConnection::OiNetworkConnection
 * \param parent
 */
OiNetworkConnection::OiNetworkConnection(QObject *parent) :
    QThread(parent)
{
    this->socket = new QTcpSocket();
    this->internalRef = OiServer::generateUniqueId();
}

/*!
 * \brief OiNetworkConnection::setSocket
 * \param socketDescriptor
 * \return
 */
bool OiNetworkConnection::setSocket(qintptr socketDescriptor){
    bool connectionCheck =  this->socket->setSocketDescriptor(socketDescriptor);
    QObject::connect(this->socket, &QTcpSocket::readyRead, this, &OiNetworkConnection::readMessage, Qt::AutoConnection);
    return connectionCheck;
}

/*!
 * \brief OiNetworkConnection::getInternalRef
 * \return
 */
int OiNetworkConnection::getInternalRef(){
   return this->internalRef;
}

/*!
 * \brief OiNetworkConnection::receiveResponse
 * Is called whenever a response to a request of this client is available
 * \param response
 */
void OiNetworkConnection::receiveResponse(const QPointer<OiRequestResponse> &response){

    if(response.isNull()){
        return;
    }

    //send response to client
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

    //create new xml request
    QPointer<OiRequestResponse> request = new OiRequestResponse();
    request->requesterId = this->internalRef;
    QString xmlRequest = this->socket->readAll();
    request->request.setContent(xmlRequest);

    emit this->sendRequest(request);

}
