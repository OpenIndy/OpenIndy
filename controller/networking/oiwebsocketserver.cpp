#include "oiwebsocketserver.h"

OiWebSocketServer::OiWebSocketServer(QObject *parent) :
    QWebSocketServer("", QWebSocketServer::NonSecureMode, parent)
{
    connect(OiRequestHandler::getInstance(), SIGNAL(sendResponse(OiRequestResponse*)), this, SLOT(receiveResponse(OiRequestResponse*)));
    connect(this, SIGNAL(newConnection()), this, SLOT(incomingConnection()));
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
        this->usedSockets.at(i)->exit();
    }

}

/*!
 * \brief OiServer::incomingConnection
 * Is called whenever a client tries to connect to OpenIndy
 * \param socketDescriptor
 */
void OiWebSocketServer::incomingConnection(){

    QWebSocket *mySocket = this->nextPendingConnection();

    OiWebSocket *myConnection = new OiWebSocket();
    myConnection->setSocket(mySocket);

    Console::addLine("Connecting to client " + QString::number(myConnection->getInternalRef()) + " ...");

    connect(myConnection, SIGNAL(finished()), myConnection, SLOT(deleteLater()));
    connect(myConnection, SIGNAL(sendRequest(OiRequestResponse*)), OiRequestHandler::getInstance(), SLOT(receiveRequest(OiRequestResponse*)));

    this->usedSockets.append(myConnection);
    myConnection->start();

}

/*!
 * \brief OiServer::receiveResponse
 * Is called from OiRequestHandler whenever a client request was done and the response is available
 * \param response
 */
void OiWebSocketServer::receiveResponse(OiRequestResponse *response){

    if(response == NULL){
        return;
    }

    //send the response to the correct client
    foreach(OiWebSocket *socket, this->usedSockets){
        if(socket->getInternalRef() == response->requesterId){

            socket->receiveResponse(response);
            return;

        }
    }

}
