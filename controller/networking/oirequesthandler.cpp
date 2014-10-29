#include "oirequesthandler.h"

OiRequestHandler *OiRequestHandler::myRequestHandler = NULL;

OiRequestHandler::OiRequestHandler(QObject *parent)
{
    this->moveToThread(&this->workerThread);
    this->workerThread.start();
}

/*!
 * \brief OiRequestHandler::getInstance
 * \return
 */
OiRequestHandler *OiRequestHandler::getInstance(){
    if(OiRequestHandler::myRequestHandler == NULL){
        OiRequestHandler::myRequestHandler = new OiRequestHandler();
    }
    return OiRequestHandler::myRequestHandler;
}

/*!
 * \brief OiRequestHandler::receiveRequest
 * Parses a XML request and calls the corresponding method to do the requested task
 * \param request
 * \return
 */
bool OiRequestHandler::receiveRequest(OiRequestResponse *request){

    qDebug() << "in receive request";

    qDebug() << (request->request.isNull()?"NULL":"not NULL");

    if(request != NULL && !request->request.isNull() && !request->request.documentElement().isNull()
            && request->request.documentElement().tagName().compare("OiRequest") == 0
            && request->request.documentElement().hasAttribute("id")){

        qDebug() << "in if" << request->request.documentElement().attribute("id", "-1").toInt();

        if(request->request.documentElement().attribute("id", "-1").toInt() == OiRequestResponse::eGetProject){
            this->getProject(request);
        }else if(request->request.documentElement().attribute("id", "-1").toInt() == OiRequestResponse::eSetProject){
            this->setProject(request);
        }else if(request->request.documentElement().attribute("id", "-1").toInt() == OiRequestResponse::eGetActiveFeature){
            this->getActiveFeature(request);
        }else if(request->request.documentElement().attribute("id", "-1").toInt() == OiRequestResponse::eSetActiveFeature){
            this->setActiveFeature(request);
        }else if(request->request.documentElement().attribute("id", "-1").toInt() == OiRequestResponse::eGetActiveStation){
            this->getActiveStation(request);
        }else if(request->request.documentElement().attribute("id", "-1").toInt() == OiRequestResponse::eSetActiveStation){
            this->setActiveStation(request);
        }else if(request->request.documentElement().attribute("id", "-1").toInt() == OiRequestResponse::eGetActiveCoordinateSystem){
            this->getActiveCoordinateSystem(request);
        }else if(request->request.documentElement().attribute("id", "-1").toInt() == OiRequestResponse::eSetActiveCoordinateSystem){
            this->setActiveCoordinateSystem(request);
        }else if(request->request.documentElement().attribute("id", "-1").toInt() == OiRequestResponse::eAim){
            this->aim(request);
        }else if(request->request.documentElement().attribute("id", "-1").toInt() == OiRequestResponse::eMove){
            this->move(request);
        }else if(request->request.documentElement().attribute("id", "-1").toInt() == OiRequestResponse::eMeasure){
            this->measure(request);
        }else if(request->request.documentElement().attribute("id", "-1").toInt() == OiRequestResponse::eStartWatchwindow){
            this->startWatchwindow(request);
        }else if(request->request.documentElement().attribute("id", "-1").toInt() == OiRequestResponse::eStopWatchwindow){
            this->stopWatchwindow(request);
        }else{
            return false;
        }
        return true;

    }

    return false;

}

/*!
 * \brief OiRequestHandler::getProject
 * \param request
 */
void OiRequestHandler::getProject(OiRequestResponse *request){

    qDebug() << "in get project";

    request->myRequestType = OiRequestResponse::eGetProject;
    this->prepareResponse(request);

    QDomDocument project = OiProjectExchanger::saveProject();
    qDebug() << "project check: " << project.toString();
    request->response.documentElement().appendChild(request->response.importNode(project.documentElement(), true));

    emit this->sendResponse(request);

}

void OiRequestHandler::setProject(OiRequestResponse *request)
{

}

/*!
 * \brief OiRequestHandler::getActiveFeature
 * \param request
 */
void OiRequestHandler::getActiveFeature(OiRequestResponse *request){

    request->myRequestType = OiRequestResponse::eGetActiveFeature;
    this->prepareResponse(request);

    QDomElement response = request->response.createElement("activeFeature");
    if(OiFeatureState::getActiveFeature() != NULL && OiFeatureState::getActiveFeature()->getFeature() != NULL){
        response.setAttribute("ref", OiFeatureState::getActiveFeature()->getFeature()->getId());
    }else{
        response.setAttribute("ref", -1);
    }
    request->response.documentElement().appendChild(response);

    emit this->sendResponse(request);

}

/*!
 * \brief OiRequestHandler::setActiveFeature
 * \param request
 */
void OiRequestHandler::setActiveFeature(OiRequestResponse *request){

    request->myRequestType = OiRequestResponse::eSetActiveFeature;
    this->prepareResponse(request);

    //set the active feature
    QDomElement activeFeature = request->request.documentElement().firstChildElement("activeFeature");
    if(!activeFeature.isNull() && activeFeature.hasAttribute("ref")){
        FeatureWrapper *myFeature = OiFeatureState::getFeature(activeFeature.attribute("ref").toInt());
        if(myFeature != NULL && myFeature->getFeature() != NULL){
            myFeature->getFeature()->setActiveFeatureState(true);
        }
    }

    //add the new active feature to XML response
    QDomElement response = request->response.createElement("activeFeature");
    if(OiFeatureState::getActiveFeature() != NULL && OiFeatureState::getActiveFeature()->getFeature() != NULL){
        response.setAttribute("ref", OiFeatureState::getActiveFeature()->getFeature()->getId());
    }else{
        response.setAttribute("ref", -1);
    }
    request->response.documentElement().appendChild(response);

    emit this->sendResponse(request);

}

/*!
 * \brief OiRequestHandler::getActiveStation
 * \param request
 */
void OiRequestHandler::getActiveStation(OiRequestResponse *request){

    request->myRequestType = OiRequestResponse::eGetActiveStation;
    this->prepareResponse(request);

    QDomElement response = request->response.createElement("activeStation");
    if(OiFeatureState::getActiveStation() != NULL){
        response.setAttribute("ref", OiFeatureState::getActiveStation()->getId());
    }else{
        response.setAttribute("ref", -1);
    }
    request->response.documentElement().appendChild(response);

    emit this->sendResponse(request);

}

/*!
 * \brief OiRequestHandler::setActiveStation
 * \param request
 */
void OiRequestHandler::setActiveStation(OiRequestResponse *request){

    request->myRequestType = OiRequestResponse::eSetActiveStation;
    this->prepareResponse(request);

    //set the active station
    QDomElement activeStation = request->request.documentElement().firstChildElement("activeStation");
    if(!activeStation.isNull() && activeStation.hasAttribute("ref")){
        FeatureWrapper *myFeature = OiFeatureState::getFeature(activeStation.attribute("ref").toInt());
        if(myFeature != NULL && myFeature->getStation() != NULL){
            myFeature->getStation()->setActiveStationState(true);
        }
    }

    //add the new active station to XML response
    QDomElement response = request->response.createElement("activeStation");
    if(OiFeatureState::getActiveStation() != NULL){
        response.setAttribute("ref", OiFeatureState::getActiveStation()->getId());
    }else{
        response.setAttribute("ref", -1);
    }
    request->response.documentElement().appendChild(response);

    emit this->sendResponse(request);

}

/*!
 * \brief OiRequestHandler::getActiveCoordinateSystem
 * \param request
 */
void OiRequestHandler::getActiveCoordinateSystem(OiRequestResponse *request){

    request->myRequestType = OiRequestResponse::eGetActiveCoordinateSystem;
    this->prepareResponse(request);

    QDomElement response = request->response.createElement("activeCoordinateSystem");
    if(OiFeatureState::getActiveCoordinateSystem() != NULL){
        response.setAttribute("ref", OiFeatureState::getActiveCoordinateSystem()->getId());
    }else{
        response.setAttribute("ref", -1);
    }
    request->response.documentElement().appendChild(response);

    emit this->sendResponse(request);

}

/*!
 * \brief OiRequestHandler::setActiveCoordinateSystem
 * \param request
 */
void OiRequestHandler::setActiveCoordinateSystem(OiRequestResponse *request){

    request->myRequestType = OiRequestResponse::eSetActiveCoordinateSystem;
    this->prepareResponse(request);

    //set the active coordinate system
    QDomElement activeCoordinateSystem = request->request.documentElement().firstChildElement("activeCoordinateSystem");
    if(!activeCoordinateSystem.isNull() && activeCoordinateSystem.hasAttribute("ref")){
        FeatureWrapper *myFeature = OiFeatureState::getFeature(activeCoordinateSystem.attribute("ref").toInt());
        if(myFeature != NULL && myFeature->getCoordinateSystem() != NULL){
            myFeature->getCoordinateSystem()->setActiveCoordinateSystemState(true);
        }
    }

    //add the new active coordinate system to XML response
    QDomElement response = request->response.createElement("activeCoordinateSystem");
    if(OiFeatureState::getActiveCoordinateSystem() != NULL){
        response.setAttribute("ref", OiFeatureState::getActiveCoordinateSystem()->getId());
    }else{
        response.setAttribute("ref", -1);
    }
    request->response.documentElement().appendChild(response);

    emit this->sendResponse(request);

}

void OiRequestHandler::aim(OiRequestResponse *request)
{

}

void OiRequestHandler::move(OiRequestResponse *request)
{

}

/*!
 * \brief OiRequestHandler::measure
 * \param request
 */
void OiRequestHandler::measure(OiRequestResponse *request){

    request->myRequestType = OiRequestResponse::eMeasure;
    this->prepareResponse(request);

    if(OiFeatureState::getActiveFeature() == NULL || OiFeatureState::getActiveFeature()->getGeometry() == NULL){
        return;
    }

    if(OiFeatureState::getActiveStation() == NULL || OiFeatureState::getActiveStation()->sensorPad == NULL
            || (OiFeatureState::getActiveStation()->sensorPad != NULL && OiFeatureState::getActiveStation()->sensorPad->instrument == NULL)){
        return;
    }

    if(OiFeatureState::getActiveFeature()->getGeometry()->getIsNominal()){
        return;
    }

    //measure the active feature
    OiFeatureState::getActiveStation()->emitStartMeasure(OiFeatureState::getActiveFeature()->getGeometry(), OiFeatureState::getActiveCoordinateSystem() == OiFeatureState::getActiveStation()->coordSys);

    //TODO signals um bei sensorbefehlen die entsprechende GUI anzuzeigen

    emit this->sendResponse(request);

}

void OiRequestHandler::startWatchwindow(OiRequestResponse *request)
{

}

void OiRequestHandler::stopWatchwindow(OiRequestResponse *request)
{

}

/*!
 * \brief OiRequestHandler::prepareResponse
 * Creates a QDomElement for response and sets its attributes
 * \param request
 */
void OiRequestHandler::prepareResponse(OiRequestResponse *request){
    request->response.appendChild(request->response.createElement("OiResponse"));
    request->response.documentElement().setAttribute("ref", request->myRequestType);
}
