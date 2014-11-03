#include "oiserver.h"

OiServer::OiServer(QObject *parent) :
    QTcpServer(parent)
{
    connect(OiRequestHandler::getInstance(), SIGNAL(sendResponse(OiRequestResponse*)), this, SLOT(receiveResponse(OiRequestResponse*)));
}

/*!
 * \brief OiServer::startServer
 * Starts the server so that clients are able to connect to OpenIndy
 */
void OiServer::startServer(){
    if(!this->listen(QHostAddress::Any,1234)){
        Console::addLine("could not start local server");
    }else{
        Console::addLine("local server ready ...");
    }
}

/*!
 * \brief OiServer::stopServer
 * Stops the server so that clients are not able to connect to OpenIndy anymore
 */
void OiServer::stopServer(){

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
void OiServer::incomingConnection(qintptr socketDescriptor){

    Console::addLine("Connecting to client " + QString::number(socketDescriptor) + " ...");

    OiNetworkConnection *myConnection;
    myConnection = new OiNetworkConnection();

    connect(myConnection, SIGNAL(finished()), myConnection, SLOT(deleteLater()));
    connect(myConnection, SIGNAL(sendRequest(OiRequestResponse*)), OiRequestHandler::getInstance(), SLOT(receiveRequest(OiRequestResponse*)));

    myConnection->setSocket(socketDescriptor);
    this->usedSockets.append(myConnection);
    myConnection->start();

}

/*!
 * \brief OiServer::receiveResponse
 * Is called from OiRequestHandler whenever a client request was done and the response is available
 * \param response
 */
void OiServer::receiveResponse(OiRequestResponse *response){

    if(response == NULL){
        return;
    }

    //send the response to the correct client
    foreach(OiNetworkConnection *socket, this->usedSockets){
        if(socket->getInternalRef() == response->requesterId){

            socket->receiveResponse(response);

        }
    }

}
