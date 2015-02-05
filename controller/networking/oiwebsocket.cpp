#include "oiwebsocket.h"

OiWebSocket::OiWebSocket(QObject *parent) :
    QThread(parent)
{
    this->internalRef = Configuration::generateID();
}

/*!
 * \brief OiNetworkConnection::getInternalRef
 * \return
 */
int OiWebSocket::getInternalRef(){
   return this->internalRef;
}

/*!
 * \brief OiNetworkConnection::setSocket
 * \param socketDescriptor
 * \return
 */
bool OiWebSocket::setSocket(QWebSocket *mySocket){

    this->socket = mySocket;
    connect(this->socket, SIGNAL(textMessageReceived(QString)), this, SLOT(readMessage(QString)));
    connect(this->socket, SIGNAL(disconnected()), this, SLOT(socketDisconnected()));
    connect(this->socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(stateChanged(QAbstractSocket::SocketState)));
    return true;

}

/*!
 * \brief OiNetworkConnection::receiveResponse
 * Is called whenever a response to a request of this client is available
 * \param response
 */
void OiWebSocket::receiveResponse(OiRequestResponse *response){

    if(response == NULL){
        return;
    }

    this->socket->sendTextMessage(response->response.toByteArray());

    if(response->myRequestType != OiRequestResponse::eStartWatchwindow){
        delete response;
    }

}

/*!
 * \brief OiNetworkConnection::readMessage
 * Is called whenever a client has sent a message to OpenIndy
 */
void OiWebSocket::readMessage(QString msg){

    OiRequestResponse *request;
    request = new OiRequestResponse();
    request->requesterId = this->internalRef;

    qDebug() << "XML Request: " << msg;

    request->request.setContent(msg);

    emit this->sendRequest(request);

}

/*!
 * \brief OiWebSocket::socketDisconnected
 */
void OiWebSocket::socketDisconnected(){

    qDebug() << "socket disconnected";

}

void OiWebSocket::stateChanged(QAbstractSocket::SocketState){

    qDebug() << "socket state changed";

}
