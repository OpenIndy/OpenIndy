#include "oirequesthandler.h"

OiRequestHandler *OiRequestHandler::myRequestHandler = NULL;

OiRequestHandler::OiRequestHandler(QObject *parent)
{
    //initially no watch window task in process
    this->myWatchWindowTask.request = NULL;
    this->myWatchWindowTask.taskInProcess = false;

    //move this class to thread
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

    if(request != NULL && !request->request.isNull() && !request->request.documentElement().isNull()
            && request->request.documentElement().tagName().compare("OiRequest") == 0
            && request->request.documentElement().hasAttribute("id")){

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

            //send error message
            request->myRequestType = OiRequestResponse::eGetProject;
            this->prepareResponse(request);
            request->response.documentElement().setAttribute("errorCode", OiRequestResponse::eUnknownRequestType);
            emit this->sendResponse(request);

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

    request->myRequestType = OiRequestResponse::eGetProject;
    this->prepareResponse(request);

    QDomDocument project = OiProjectExchanger::saveProject();

    if(!project.isNull()){
        request->response.documentElement().appendChild(request->response.importNode(project.documentElement(), true));
    }

    emit this->sendResponse(request);

}

/*!
 * \brief OiRequestHandler::setProject
 * \param request
 */
void OiRequestHandler::setProject(OiRequestResponse *request){

    request->myRequestType = OiRequestResponse::eSetProject;
    this->prepareResponse(request);

    //load xml file to DOM tree
    QDomDocument oiXml;
    try{
        OiProjectData::getDevice()->open(QIODevice::ReadOnly);
        oiXml.setContent(OiProjectData::getDevice());
        OiProjectData::getDevice()->close();
    }catch(const exception &e){
        Console::addLine("Error while opening OpenIndy xml file.");
        return;
    }

    bool success = OiProjectExchanger::loadProject(oiXml);

    if(!success){
        request->response.documentElement().setAttribute("errorCode", OiRequestResponse::eWrongFormat);
    }

    emit this->sendResponse(request);

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
        request->response.documentElement().setAttribute("errorCode", OiRequestResponse::eNoActiveFeature);
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

    int errorCode = 0;

    //set the active feature
    QDomElement activeFeature = request->request.documentElement().firstChildElement("activeFeature");
    if(!activeFeature.isNull() && activeFeature.hasAttribute("ref")){
        FeatureWrapper *myFeature = OiFeatureState::getFeature(activeFeature.attribute("ref").toInt());
        if(myFeature != NULL && myFeature->getFeature() != NULL){
            myFeature->getFeature()->setActiveFeatureState(true);
        }else{
            errorCode = OiRequestResponse::eNoFeatureWithId;
        }
    }else{
        errorCode = OiRequestResponse::eWrongFormat;
    }

    //add the new active feature to XML response
    QDomElement response = request->response.createElement("activeFeature");
    if(OiFeatureState::getActiveFeature() != NULL && OiFeatureState::getActiveFeature()->getFeature() != NULL){
        response.setAttribute("ref", OiFeatureState::getActiveFeature()->getFeature()->getId());
    }else{
        response.setAttribute("ref", -1);
    }
    request->response.documentElement().setAttribute("errorCode", errorCode);
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
        request->response.documentElement().setAttribute("errorCode", OiRequestResponse::eNoActiveStation);
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

    int errorCode = 0;

    //set the active station
    QDomElement activeStation = request->request.documentElement().firstChildElement("activeStation");
    if(!activeStation.isNull() && activeStation.hasAttribute("ref")){
        FeatureWrapper *myFeature = OiFeatureState::getFeature(activeStation.attribute("ref").toInt());
        if(myFeature != NULL && myFeature->getStation() != NULL){
            myFeature->getStation()->setActiveStationState(true);
        }else{
            errorCode = OiRequestResponse::eNoFeatureWithId;
        }
    }else{
        errorCode = OiRequestResponse::eWrongFormat;
    }

    //add the new active station to XML response
    QDomElement response = request->response.createElement("activeStation");
    if(OiFeatureState::getActiveStation() != NULL){
        response.setAttribute("ref", OiFeatureState::getActiveStation()->getId());
    }else{
        response.setAttribute("ref", -1);
    }
    request->response.documentElement().setAttribute("errorCode", errorCode);
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
        request->response.documentElement().setAttribute("errorCode", OiRequestResponse::eNoActiveCoordinateSystem);
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

    int errorCode = 0;

    //set the active coordinate system
    QDomElement activeCoordinateSystem = request->request.documentElement().firstChildElement("activeCoordinateSystem");
    if(!activeCoordinateSystem.isNull() && activeCoordinateSystem.hasAttribute("ref")){
        FeatureWrapper *myFeature = OiFeatureState::getFeature(activeCoordinateSystem.attribute("ref").toInt());
        if(myFeature != NULL && myFeature->getCoordinateSystem() != NULL){
            myFeature->getCoordinateSystem()->setActiveCoordinateSystemState(true);
        }else{
            errorCode = OiRequestResponse::eNoFeatureWithId;
        }
    }else{
        errorCode = OiRequestResponse::eWrongFormat;
    }

    //add the new active coordinate system to XML response
    QDomElement response = request->response.createElement("activeCoordinateSystem");
    if(OiFeatureState::getActiveCoordinateSystem() != NULL){
        response.setAttribute("ref", OiFeatureState::getActiveCoordinateSystem()->getId());
    }else{
        response.setAttribute("ref", -1);
    }
    request->response.documentElement().setAttribute("errorCode", errorCode);
    request->response.documentElement().appendChild(response);

    emit this->sendResponse(request);

}

/*!
 * \brief OiRequestHandler::aim
 * \param request
 */
void OiRequestHandler::aim(OiRequestResponse *request){

    request->myRequestType = OiRequestResponse::eAim;
    this->prepareResponse(request);

    //check if active feature is valid
    if(OiFeatureState::getActiveFeature() == NULL || OiFeatureState::getActiveFeature()->getGeometry() == NULL){
        request->response.documentElement().setAttribute("errorCode", OiRequestResponse::eNoActiveFeature);
        emit this->sendResponse(request);
        return;
    }

    //check if sensor is valid
    if(OiFeatureState::getActiveStation() == NULL || OiFeatureState::getActiveStation()->coordSys == NULL){
        request->response.documentElement().setAttribute("errorCode", OiRequestResponse::eNoActiveStation);
        emit this->sendResponse(request);
        return;
    }

    //check if active coordinate system is valid
    if(OiFeatureState::getActiveCoordinateSystem() == NULL){
        request->response.documentElement().setAttribute("errorCode", OiRequestResponse::eNoActiveCoordinateSystem);
        emit this->sendResponse(request);
        return;
    }

    //get XYZ coordinates of the active feature
    OiVec xyz = OiFeatureState::getActiveFeature()->getGeometry()->getXYZ();
    if(xyz.getSize() < 3){
        request->response.documentElement().setAttribute("errorCode", OiRequestResponse::eNoActiveFeature);
        emit this->sendResponse(request);
        return;
    }

    //convert XYZ coordinates to polar elements
    OiVec polarElements = Reading::toPolar(xyz.getAt(0),xyz.getAt(1),xyz.getAt(2));

    //if the station system and the active system are not the same the polar elements have to be converted
    if(OiFeatureState::getActiveStation()->coordSys != OiFeatureState::getActiveCoordinateSystem()){

        //get homogeneous matrix (from station system to active system)
        int success = false;
        QList<TrafoParam *> trafoParams = OiFeatureState::getActiveStation()->coordSys->getTransformationParameters();
        foreach(TrafoParam *tp, trafoParams){
            if(tp != NULL && tp->getDestinationSystem() == OiFeatureState::getActiveCoordinateSystem()){
                OiMat t = tp->getHomogenMatrix();
                if(t.getColCount() == 4 && t.getRowCount() == 4){
                    xyz = t.inv() * xyz;
                    polarElements = Reading::toPolar(xyz.getAt(0), xyz.getAt(1), xyz.getAt(2));
                    success = true;
                    break;
                }
            }
        }

        //set error code if no trafo params are available
        if(!success){
            request->response.documentElement().setAttribute("errorCode", OiRequestResponse::eNoTransformationParameters);
            emit this->sendResponse(request);
            return;
        }

    }

    //start aiming the active feature
    OiFeatureState::getActiveStation()->emitStartMove(polarElements.getAt(0), polarElements.getAt(1), polarElements.getAt(2), false);

    emit this->sendResponse(request);

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
        request->response.documentElement().setAttribute("errorCode", OiRequestResponse::eNoActiveFeature);
        emit this->sendResponse(request);
        return;
    }

    if(OiFeatureState::getActiveStation() == NULL || OiFeatureState::getActiveStation()->sensorPad == NULL
            || (OiFeatureState::getActiveStation()->sensorPad != NULL && OiFeatureState::getActiveStation()->sensorPad->instrument == NULL)){
        request->response.documentElement().setAttribute("errorCode", OiRequestResponse::eNoActiveStation);
        emit this->sendResponse(request);
        return;
    }

    if(OiFeatureState::getActiveFeature()->getGeometry()->getIsNominal()){
        request->response.documentElement().setAttribute("errorCode", OiRequestResponse::eCannotMeasureNominal);
        emit this->sendResponse(request);
        return;
    }

    //measure the active feature
    OiFeatureState::getActiveStation()->emitStartMeasure(OiFeatureState::getActiveFeature()->getGeometry(), OiFeatureState::getActiveCoordinateSystem() == OiFeatureState::getActiveStation()->coordSys);

    //TODO signals um bei sensorbefehlen die entsprechende GUI anzuzeigen

    emit this->sendResponse(request);

}

/*!
 * \brief OiRequestHandler::startWatchwindow
 * \param request
 */
void OiRequestHandler::startWatchwindow(OiRequestResponse *request){

    //only one watch window task should be open at once
    if(this->myWatchWindowTask.taskInProcess){
        request->response.documentElement().setAttribute("errorCode", OiRequestResponse::eTaskInProcess);
        emit this->sendResponse(request);
        return;
    }

    request->myRequestType = OiRequestResponse::eStartWatchwindow;
    this->prepareResponse(request);

    //get requested reading type
    Configuration::ReadingTypes myReadingType;
    QDomElement readingType = request->request.documentElement().firstChildElement("readingType");
    if(!readingType.isNull() && readingType.hasAttribute("type")){
        myReadingType = (Configuration::ReadingTypes)readingType.attribute("type").toInt();
    }else{
        request->response.documentElement().setAttribute("errorCode", OiRequestResponse::eWrongFormat);
        emit this->sendResponse(request);
        return;
    }

    if(OiFeatureState::getActiveStation() != NULL && OiFeatureState::getActiveStation()->sensorPad != NULL
            && OiFeatureState::getActiveStation()->sensorPad->instrumentListener != NULL){

        //connect the reading stream to the request handler
        connect(OiFeatureState::getActiveStation()->sensorPad->instrumentListener, SIGNAL(sendReadingMap(QVariantMap)),
                this, SLOT(receiveWatchWindowData(QVariantMap)));

        //start watch window
        OiFeatureState::getActiveStation()->emitStartReadingStream(myReadingType); //TODO Übergabeparameter

        //save active watch window task
        this->myWatchWindowTask.taskInProcess = true;
        this->myWatchWindowTask.request = request;

    }else{
        request->response.documentElement().setAttribute("errorCode", OiRequestResponse::eNoActiveStation);
        emit this->sendResponse(request);
        return;
    }

    emit this->sendResponse(request);

}

/*!
 * \brief OiRequestHandler::stopWatchwindow
 * \param request
 */
void OiRequestHandler::stopWatchwindow(OiRequestResponse *request){

    //if no task is in process, no task has to be stopped
    if(!this->myWatchWindowTask.taskInProcess){
        request->response.documentElement().setAttribute("errorCode", OiRequestResponse::eNoTask);
        emit this->sendResponse(request);
        return;
    }

    request->myRequestType = OiRequestResponse::eStopWatchwindow;
    this->prepareResponse(request);

    if(OiFeatureState::getActiveStation() != NULL && OiFeatureState::getActiveStation()->sensorPad != NULL
            && OiFeatureState::getActiveStation()->sensorPad->instrumentListener != NULL){

        //disconnect the reading stream from the request handler
        disconnect(OiFeatureState::getActiveStation()->sensorPad->instrumentListener, SIGNAL(sendReadingMap(QVariantMap)),
                this, SLOT(receiveWatchWindowData(QVariantMap)));

        //stop watch window
        OiFeatureState::getActiveStation()->stopReadingStream();

        //reset active watch window task
        this->myWatchWindowTask.taskInProcess = false;
        delete this->myWatchWindowTask.request;
        this->myWatchWindowTask.request = NULL;

    }else{
        request->response.documentElement().setAttribute("errorCode", OiRequestResponse::eNoActiveStation);
        emit this->sendResponse(request);
        return;
    }

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
    request->response.documentElement().setAttribute("errorCode", OiRequestResponse::eNoError);
}

/*!
 * \brief OiRequestHandler::buildWatchWindowMessage
 * \param wwTag: the XML tag that shall be filled
 * \param readingType: the requested reading type
 * \param streamData: the sensor stream data
 * \return
 */
bool OiRequestHandler::buildWatchWindowMessage(QDomElement &wwTag, int readingType, QVariantMap streamData){

    //get xyz of active geometry
    OiVec xyz = OiFeatureState::getActiveFeature()->getGeometry()->getXYZ();

    //get and transform sensor stream data
    switch(OiFeatureState::getActiveStation()->getReadingStreamType()){
    case Configuration::eCartesian:{

        //check if x, y, z are available
        if(!streamData.contains("x") || !streamData.contains("y") || !streamData.contains("z")){
            return false;
        }

        //get sensor xyz
        OiVec streamXyz(4);
        streamXyz.setAt(0, streamData.value("x").toDouble());
        streamXyz.setAt(1, streamData.value("y").toDouble());
        streamXyz.setAt(2, streamData.value("z").toDouble());
        streamXyz.setAt(3, 1.0);

        //get trafo params and transform sensor xyz
        QList<TrafoParam*> trafoParams = OiFeatureState::getActiveStation()->coordSys->getTransformationParameters(OiFeatureState::getActiveCoordinateSystem());
        if(trafoParams.size() == 0){
            return false;
        }
        OiMat trafo = trafoParams.at(0)->getHomogenMatrix();
        if(trafo.getRowCount() != 4 || trafo.getColCount() != 4){
            return false;
        }
        streamXyz = trafo * streamXyz;

        //build difference sensor - feature
        OiVec diff = streamXyz - xyz;

        //fill watch window tag
        wwTag.setTagName("cartesian");
        wwTag.setAttribute("x", QString::number(diff.getAt(0)));
        wwTag.setAttribute("y", QString::number(diff.getAt(1)));
        wwTag.setAttribute("z", QString::number(diff.getAt(2)));

        return true;

    }case Configuration::ePolar:{
        break;
    }default:
        return false;
    }

}

/*!
 * \brief OiRequestHandler::receiveWatchWindowData
 * Each time a map with current coordinates is emitted by SensorListener
 * \param data
 */
void OiRequestHandler::receiveWatchWindowData(QVariantMap data){

    if(!this->myWatchWindowTask.taskInProcess || this->myWatchWindowTask.request == NULL){
        return;
    }

    //clear the old response
    this->myWatchWindowTask.request->response.clear();

    this->myWatchWindowTask.request->myRequestType = OiRequestResponse::eStartWatchwindow;
    this->prepareResponse(this->myWatchWindowTask.request);

    //get requested reading type
    int myReadingType;
    QDomElement readingType = this->myWatchWindowTask.request->request.documentElement().firstChildElement("readingType");
    if(!readingType.isNull() && readingType.hasAttribute("type")){
        myReadingType = readingType.attribute("type").toInt();
    }else{
        this->myWatchWindowTask.request->response.documentElement().setAttribute("errorCode", OiRequestResponse::eWrongFormat);
        emit this->sendResponse(this->myWatchWindowTask.request);
        return;
    }

    //check if active feature exists and is a geometry
    if(OiFeatureState::getActiveFeature() == NULL || OiFeatureState::getActiveFeature()->getGeometry() == NULL){
        this->myWatchWindowTask.request->response.documentElement().setAttribute("errorCode", OiRequestResponse::eWrongFormat);
        emit this->sendResponse(this->myWatchWindowTask.request);
        return;
    }

    //check if active station exists
    if(OiFeatureState::getActiveStation() == NULL){
        this->myWatchWindowTask.request->response.documentElement().setAttribute("errorCode", OiRequestResponse::eWrongFormat);
        emit this->sendResponse(this->myWatchWindowTask.request);
        return;
    }

    //create feature info tag
    QDomElement featureTag = this->myWatchWindowTask.request->response.createElement("geometry");
    featureTag.setAttribute("id", OiFeatureState::getActiveFeature()->getFeature()->getId());
    featureTag.setAttribute("name", OiFeatureState::getActiveFeature()->getFeature()->getFeatureName());

    //create tag with watch window data
    QDomElement wwTag = this->myWatchWindowTask.request->response.createElement("cartesian");
    this->buildWatchWindowMessage(wwTag, myReadingType, data);

    qDebug() << "wwtag " << wwTag.text();

    //build xml response
    this->myWatchWindowTask.request->response.documentElement().appendChild(featureTag);
    this->myWatchWindowTask.request->response.documentElement().appendChild(wwTag);

    qDebug() << "response: " << this->myWatchWindowTask.request->response.toString();

    emit this->sendResponse(this->myWatchWindowTask.request);

}
