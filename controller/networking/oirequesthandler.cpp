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
        }else if(request->request.documentElement().attribute("id", "-1").toInt() == OiRequestResponse::eStartStakeOut){
            this->startStakeOut(request);
        }else if(request->request.documentElement().attribute("id", "-1").toInt() == OiRequestResponse::eStopStakeOut){
            this->stopStakeOut(request);
        }else if(request->request.documentElement().attribute("id", "-1").toInt() == OiRequestResponse::eGetNextGeometry){
            this->GetNextGeometry(request);
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

    if(!project.isNull()){
        qDebug() << "project check: " << project.toString();
        request->response.documentElement().appendChild(request->response.importNode(project.documentElement(), true));
    }


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
 * \brief OiRequestHandler::startStakeOut
 * \param request
 */
void OiRequestHandler::startStakeOut(OiRequestResponse *request){

    qDebug() << "in start stake out";

    request->myRequestType = OiRequestResponse::eStartStakeOut;
    this->prepareResponse(request);

    //stake out parameter
    OiStakeOut::StakeOutMode stakeOutMode;
    bool stakeOutAllGeometries;
    QStringList stakeOutGroups;
    QList<int> stakeOutGeometries;
    int stakeOutId;

    //get XML nodes
    QDomElement mode = request->request.documentElement().firstChildElement("mode");
    QDomElement allGeometries = request->request.documentElement().firstChildElement("allGeometries");
    QDomElement groups = request->request.documentElement().firstChildElement("groups");
    QDomElement geometries = request->request.documentElement().firstChildElement("geometries");

    //check if minimum configuration of XML request is available
    if(mode.isNull() || !mode.hasAttribute("value") || allGeometries.isNull() || !allGeometries.hasAttribute("value")){
        return;
    }

    //get the stake out mode
    if(mode.attribute("value").compare("sequence") == 0){
        stakeOutMode = OiStakeOut::eSequence;
    }else if(mode.attribute("value").compare("nearest") == 0){
        stakeOutMode = OiStakeOut::eNearest;
    }else{
        return;
    }

    //get allGeometries parameter
    if(allGeometries.attribute("value").toInt() == 1){
        stakeOutAllGeometries = true;
    }else if(allGeometries.attribute("value").toInt() == 0){
        stakeOutAllGeometries = false;
    }else{
        return;
    }

    //get stake out groups
    if(!groups.isNull()){
        QDomNodeList myGroups = groups.childNodes();
        for(int i = 0; i < myGroups.size(); i++){
            QDomNode node = myGroups.at(i);
            QDomNamedNodeMap attributes = node.attributes();
            if(attributes.contains("name")){
                stakeOutGroups.append(attributes.namedItem("name").nodeValue());
            }
        }
    }

    //get stake out geometries
    if(!geometries.isNull()){
        QDomNodeList myGeometries = geometries.childNodes();
        for(int i = 0; i < myGeometries.size(); i++){
            QDomNode node = myGeometries.at(i);
            QDomNamedNodeMap attributes = node.attributes();
            if(attributes.contains("ref")){
                stakeOutGeometries.append(attributes.namedItem("ref").nodeValue().toInt());
            }
        }
    }

    if(stakeOutAllGeometries){ //if all geometries shall be staked out
        stakeOutId = OiStakeOut::startStakeOut(stakeOutMode, stakeOutAllGeometries);
    }else if(stakeOutGroups.size() > 0){ //if special groups shall be staked out
        stakeOutId = OiStakeOut::startStakeOut(stakeOutMode, stakeOutAllGeometries, stakeOutGroups);
    }else if(stakeOutGeometries.size() > 0){ //if special geometries shall be staked out
        stakeOutId = OiStakeOut::startStakeOut(stakeOutMode, stakeOutAllGeometries, stakeOutGroups, stakeOutGeometries);
    }else{
        return;
    }

    //add stake out id
    QDomElement response = request->response.createElement("stakeOutId");
    response.setAttribute("id", stakeOutId);
    request->response.documentElement().appendChild(response);

    emit this->sendResponse(request);

}

/*!
 * \brief OiRequestHandler::stopStakeOut
 * \param request
 */
void OiRequestHandler::stopStakeOut(OiRequestResponse *request){

    request->myRequestType = OiRequestResponse::eStopStakeOut;
    this->prepareResponse(request);

    QDomElement stakeOutId = request->request.documentElement().firstChildElement("stakeOutId");

    if(stakeOutId.isNull()){
        return;
    }

    OiStakeOut::stopStakeOut(stakeOutId.attribute("id").toInt());

    emit this->sendResponse(request);

}

/*!
 * \brief OiRequestHandler::GetNextGeometry
 * \param request
 */
void OiRequestHandler::GetNextGeometry(OiRequestResponse *request){

    qDebug() << "in get next geometry";

    request->myRequestType = OiRequestResponse::eGetNextGeometry;
    this->prepareResponse(request);

    QDomElement stakeOutId = request->request.documentElement().firstChildElement("stakeOutId");

    if(stakeOutId.isNull()){
        return;
    }

    qDebug() << "vor get geom";

    //get the next geometry that shall be staked out
    FeatureWrapper *geom = OiStakeOut::getNextGeometry(stakeOutId.attribute("id").toInt());

    if(geom == NULL || geom->getGeometry() == NULL){
        return;
    }

    //set geom as active feature and aim it
    geom->getGeometry()->setActiveFeatureState(true);

    qDebug() << "nach get geom";

    //set geometry id as response
    QDomElement response = request->response.createElement("geometry");
    response.setAttribute("ref", geom->getGeometry()->getId());
    request->response.documentElement().appendChild(response);

    emit this->sendResponse(request);

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
