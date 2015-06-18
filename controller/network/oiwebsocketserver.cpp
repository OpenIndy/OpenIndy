#include "oiwebsocketserver.h"

int OiWebSocketServer::currentId = 0;

/*!
 * \brief OiWebSocketServer::OiWebSocketServer
 * \param parent
 */
OiWebSocketServer::OiWebSocketServer(QObject *parent) :
    QWebSocketServer("", QWebSocketServer::NonSecureMode, parent)
{
    QObject::connect(OiRequestHandler::getInstance(), &OiRequestHandler::sendResponse, this, &OiWebSocketServer::receiveResponse, Qt::AutoConnection);
    QObject::connect(this, &OiWebSocketServer::newConnection, this, &OiWebSocketServer::incomingConnection, Qt::AutoConnection);
}

/*!
 * \brief OiWebSocketServer::generateUniqueId
 * Generates unique id's for OiWebsocket's
 * \return
 */
int OiWebSocketServer::generateUniqueId(){
    OiWebSocketServer::currentId++;
    return OiWebSocketServer::currentId;
}

/*!
 * \brief OiServer::startServer
 * Starts the server so that clients are able to connect to OpenIndy
 */
void OiWebSocketServer::startServer(){
    if(!this->listen(QHostAddress::Any,1235)){
        Console::addLine("could not start local server");
    }else{
        Console::addLine("local server ready ...");
    }
}

/*!
 * \brief OiServer::stopServer
 * Stops the server so that clients are not able to connect to OpenIndy anymore
 */
void OiWebSocketServer::stopServer(){

    //stop listening for new connections
    if(this->isListening()){
        this->close();
    }

    //stop the already existing threads
    for(int i = 0; i < this->usedSockets.size(); i++){
        if(!this->usedSockets[i].isNull()){
            this->usedSockets[i]->exit();
        }
    }

}

/*!
 * \brief OiServer::incomingConnection
 * Is called whenever a client tries to connect to OpenIndy
 * \param socketDescriptor
 */
void OiWebSocketServer::incomingConnection(){

    //create web socket instance
    QPointer<QWebSocket> mySocket = this->nextPendingConnection();
    QPointer<OiWebSocket> myConnection = new OiWebSocket();
    myConnection->setSocket(mySocket);

    Console::addLine("Connecting to client " + QString::number(myConnection->getInternalRef()) + " ...");

    QObject::connect(myConnection, &OiWebSocket::finished, myConnection, &OiWebSocket::deleteLater, Qt::AutoConnection);
    QObject::connect(myConnection, &OiWebSocket::sendRequest, OiRequestHandler::getInstance(), &OiRequestHandler::receiveRequest, Qt::AutoConnection);

    this->usedSockets.append(myConnection);
    myConnection->start();

}

/*!
 * \brief OiServer::receiveResponse
 * Is called from OiRequestHandler whenever a client request was done and the response is available
 * \param response
 */
void OiWebSocketServer::receiveResponse(const QPointer<OiRequestResponse> &response){

    if(response.isNull()){
        return;
    }

    //send the response to the correct client
    foreach(const QPointer<OiWebSocket> &socket, this->usedSockets){
        if(!socket.isNull() && socket->getInternalRef() == response->requesterId){
            socket->receiveResponse(response);
            return;
        }
    }

}
