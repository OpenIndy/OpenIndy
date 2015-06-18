#include "oiserver.h"

int OiServer::currentId = 0;

/*!
 * \brief OiServer::OiServer
 * \param parent
 */
OiServer::OiServer(QObject *parent) :
    QTcpServer(parent)
{
    QObject::connect(OiRequestHandler::getInstance(), &OiRequestHandler::sendResponse, this, &OiServer::receiveResponse, Qt::AutoConnection);
}

/*!
 * \brief OiServer::generateUniqueId
 * Generates unique id's for OiNetworkConnection's
 * \return
 */
int OiServer::generateUniqueId(){
    OiServer::currentId++;
    return OiServer::currentId;
}

/*!
 * \brief OiServer::startServer
 * Starts the server so that clients are able to connect to OpenIndy
 */
void OiServer::startServer(){
    if(!this->listen(QHostAddress::Any, 1234)){
        Console::getInstance()->addLine("could not start local server");
    }else{
        Console::getInstance()->addLine("local server ready ...");
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

    //create and connect the network connection
    QPointer<OiNetworkConnection> myConnection = new OiNetworkConnection();
    QObject::connect(myConnection, &OiNetworkConnection::finished, myConnection, &OiNetworkConnection::deleteLater, Qt::AutoConnection);
    QObject::connect(myConnection, &OiNetworkConnection::sendRequest, OiRequestHandler::getInstance(), &OiRequestHandler::receiveRequest, Qt::AutoConnection);

    myConnection->setSocket(socketDescriptor);
    this->usedSockets.append(myConnection);
    myConnection->start();

}

/*!
 * \brief OiServer::receiveResponse
 * Is called from OiRequestHandler whenever a client request was done and the response is available
 * \param response
 */
void OiServer::receiveResponse(const QPointer<OiRequestResponse> &response){

    if(response.isNull()){
        return;
    }

    //send the response to the correct client
    foreach(const QPointer<OiNetworkConnection> &socket, this->usedSockets){
        if(!socket.isNull() && socket->getInternalRef() == response->requesterId){
            socket->receiveResponse(response);
            return;
        }
    }

}
