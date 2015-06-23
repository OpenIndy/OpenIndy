#include "oirequesthandler.h"

QPointer<OiRequestHandler> OiRequestHandler::myRequestHandler(NULL);

OiRequestHandler::OiRequestHandler(QObject *parent)
{
    //initially no watch window or measurement task in process
    this->watchWindowTask.taskInProcess = false;
    this->measurementTask.taskInProcess = false;

    //move this class to thread
    this->moveToThread(&this->workerThread);
    this->workerThread.start();
}

/*!
 * \brief OiRequestHandler::getInstance
 * \return
 */
QPointer<OiRequestHandler> OiRequestHandler::getInstance(){
    if(OiRequestHandler::myRequestHandler.isNull()){
        OiRequestHandler::myRequestHandler = new OiRequestHandler();
    }
    return OiRequestHandler::myRequestHandler;
}

/*!
 * \brief OiRequestHandler::getJob
 * \return
 */
const QPointer<OiJob> &OiRequestHandler::getJob() const{
    return this->currentJob;
}

/*!
 * \brief OiRequestHandler::setJob
 * \param job
 */
void OiRequestHandler::setJob(const QPointer<OiJob> &job){
    if(!job.isNull()){
        this->currentJob = job;
    }
}

/*!
 * \brief OiRequestHandler::receiveRequest
 * Parses a XML request and calls the corresponding method to do the requested task
 * \param request
 * \return
 */
bool OiRequestHandler::receiveRequest(OiRequestResponse request){

    if(!request.request.isNull() && !request.request.documentElement().isNull()
            && request.request.documentElement().tagName().compare("OiRequest") == 0
            && request.request.documentElement().hasAttribute("id")){

        //check current job
        if(this->currentJob.isNull()){

            //send error message
            request.myRequestType = OiRequestResponse::eGetProject;
            this->prepareResponse(request);
            request.response.documentElement().setAttribute("errorCode", OiRequestResponse::eUnknownRequestType);
            emit this->sendResponse(request);

            return false;

        }

        if(request.request.documentElement().attribute("id", "-1").toInt() == OiRequestResponse::eGetProject){
            this->getProject(request);
        }else if(request.request.documentElement().attribute("id", "-1").toInt() == OiRequestResponse::eSetProject){
            this->setProject(request);
        }else if(request.request.documentElement().attribute("id", "-1").toInt() == OiRequestResponse::eGetActiveFeature){
            this->getActiveFeature(request);
        }else if(request.request.documentElement().attribute("id", "-1").toInt() == OiRequestResponse::eSetActiveFeature){
            this->setActiveFeature(request);
        }else if(request.request.documentElement().attribute("id", "-1").toInt() == OiRequestResponse::eGetActiveStation){
            this->getActiveStation(request);
        }else if(request.request.documentElement().attribute("id", "-1").toInt() == OiRequestResponse::eSetActiveStation){
            this->setActiveStation(request);
        }else if(request.request.documentElement().attribute("id", "-1").toInt() == OiRequestResponse::eGetActiveCoordinateSystem){
            this->getActiveCoordinateSystem(request);
        }else if(request.request.documentElement().attribute("id", "-1").toInt() == OiRequestResponse::eSetActiveCoordinateSystem){
            this->setActiveCoordinateSystem(request);
        }else if(request.request.documentElement().attribute("id", "-1").toInt() == OiRequestResponse::eAim){
            this->aim(request);
        }else if(request.request.documentElement().attribute("id", "-1").toInt() == OiRequestResponse::eMove){
            this->move(request);
        }else if(request.request.documentElement().attribute("id", "-1").toInt() == OiRequestResponse::eMeasure){
            this->measure(request);
        }else if(request.request.documentElement().attribute("id", "-1").toInt() == OiRequestResponse::eStartWatchwindow){
            this->startWatchwindow(request);
        }else if(request.request.documentElement().attribute("id", "-1").toInt() == OiRequestResponse::eStopWatchwindow){
            this->stopWatchwindow(request);
        }else if(request.request.documentElement().attribute("id", "-1").toInt() == OiRequestResponse::eOiToolRequest){
            emit this->sendOiToolRequest(request);
        }else{

            //send error message
            request.myRequestType = OiRequestResponse::eGetProject;
            this->prepareResponse(request);
            request.response.documentElement().setAttribute("errorCode", OiRequestResponse::eUnknownRequestType);
            emit this->sendResponse(request);

            return false;

        }

        return true;

    }

    return false;

}

/*!
 * \brief OiRequestHandler::receiveOiToolResponse
 * \param response
 */
void OiRequestHandler::receiveOiToolResponse(OiRequestResponse response){

    response.myRequestType = OiRequestResponse::eOiToolRequest;

    emit this->sendResponse(response);

}

/*!
 * \brief OiRequestHandler::getProject
 * \param request
 */
void OiRequestHandler::getProject(OiRequestResponse request){

    request.myRequestType = OiRequestResponse::eGetProject;
    this->prepareResponse(request);

    QDomDocument project = ProjectExchanger::saveProject(this->currentJob);

    if(!project.isNull()){
        request.response.documentElement().appendChild(request.response.importNode(project.documentElement(), true));
    }

    emit this->sendResponse(request);

}

/*!
 * \brief OiRequestHandler::setProject
 * \param request
 */
void OiRequestHandler::setProject(OiRequestResponse request){

    request.myRequestType = OiRequestResponse::eSetProject;
    this->prepareResponse(request);

    //load xml file to DOM tree
    QDomDocument oiXml;
    try{
        this->currentJob->getJobDevice()->open(QIODevice::ReadOnly);
        oiXml.setContent(this->currentJob->getJobDevice());
        this->currentJob->getJobDevice()->close();
    }catch(const exception &e){
        Console::getInstance()->addLine("Error while opening OpenIndy xml file.");
        return;
    }

    bool success = ProjectExchanger::loadProject(oiXml);

    if(!success){
        request.response.documentElement().setAttribute("errorCode", OiRequestResponse::eWrongFormat);
    }

    emit this->sendResponse(request);

}

/*!
 * \brief OiRequestHandler::getActiveFeature
 * \param request
 */
void OiRequestHandler::getActiveFeature(OiRequestResponse request){

    request.myRequestType = OiRequestResponse::eGetActiveFeature;
    this->prepareResponse(request);

    QDomElement response = request.response.createElement("activeFeature");
    if(!this->currentJob->getActiveFeature().isNull() && !this->currentJob->getActiveFeature()->getFeature().isNull()){
        response.setAttribute("ref", this->currentJob->getActiveFeature()->getFeature()->getId());
    }else{
        response.setAttribute("ref", -1);
        request.response.documentElement().setAttribute("errorCode", OiRequestResponse::eNoActiveFeature);
    }
    request.response.documentElement().appendChild(response);

    emit this->sendResponse(request);

}

/*!
 * \brief OiRequestHandler::setActiveFeature
 * \param request
 */
void OiRequestHandler::setActiveFeature(OiRequestResponse request){

    request.myRequestType = OiRequestResponse::eSetActiveFeature;
    this->prepareResponse(request);

    int errorCode = 0;

    //set the active feature
    QDomElement activeFeature = request.request.documentElement().firstChildElement("activeFeature");
    if(!activeFeature.isNull() && activeFeature.hasAttribute("ref")){
        QPointer<FeatureWrapper> myFeature = this->currentJob->getFeatureById(activeFeature.attribute("ref").toInt());
        if(!myFeature.isNull() && myFeature->getFeature().isNull()){
            myFeature->getFeature()->setActiveFeatureState(true);
        }else{
            errorCode = OiRequestResponse::eNoFeatureWithId;
        }
    }else{
        errorCode = OiRequestResponse::eWrongFormat;
    }

    //add the new active feature to XML response
    QDomElement response = request.response.createElement("activeFeature");
    if(!this->currentJob->getActiveFeature().isNull() && !this->currentJob->getActiveFeature()->getFeature().isNull()){
        response.setAttribute("ref", this->currentJob->getActiveFeature()->getFeature()->getId());
    }else{
        response.setAttribute("ref", -1);
    }
    request.response.documentElement().setAttribute("errorCode", errorCode);
    request.response.documentElement().appendChild(response);

    emit this->sendResponse(request);

}

/*!
 * \brief OiRequestHandler::getActiveStation
 * \param request
 */
void OiRequestHandler::getActiveStation(OiRequestResponse request){

    request.myRequestType = OiRequestResponse::eGetActiveStation;
    this->prepareResponse(request);

    QDomElement response = request.response.createElement("activeStation");
    if(!this->currentJob->getActiveStation().isNull()){
        response.setAttribute("ref", this->currentJob->getActiveStation()->getId());
    }else{
        response.setAttribute("ref", -1);
        request.response.documentElement().setAttribute("errorCode", OiRequestResponse::eNoActiveStation);
    }
    request.response.documentElement().appendChild(response);

    emit this->sendResponse(request);

}

/*!
 * \brief OiRequestHandler::setActiveStation
 * \param request
 */
void OiRequestHandler::setActiveStation(OiRequestResponse request){

    request.myRequestType = OiRequestResponse::eSetActiveStation;
    this->prepareResponse(request);

    int errorCode = 0;

    //set the active station
    QDomElement activeStation = request.request.documentElement().firstChildElement("activeStation");
    if(!activeStation.isNull() && activeStation.hasAttribute("ref")){
        QPointer<FeatureWrapper> myFeature = this->currentJob->getFeatureById(activeStation.attribute("ref").toInt());
        if(myFeature != NULL && myFeature->getStation() != NULL){
            myFeature->getStation()->setActiveStationState(true);
        }else{
            errorCode = OiRequestResponse::eNoFeatureWithId;
        }
    }else{
        errorCode = OiRequestResponse::eWrongFormat;
    }

    //add the new active station to XML response
    QDomElement response = request.response.createElement("activeStation");
    if(!this->currentJob->getActiveStation().isNull()){
        response.setAttribute("ref", this->currentJob->getActiveStation()->getId());
    }else{
        response.setAttribute("ref", -1);
    }
    request.response.documentElement().setAttribute("errorCode", errorCode);
    request.response.documentElement().appendChild(response);

    emit this->sendResponse(request);

}

/*!
 * \brief OiRequestHandler::getActiveCoordinateSystem
 * \param request
 */
void OiRequestHandler::getActiveCoordinateSystem(OiRequestResponse request){

    request.myRequestType = OiRequestResponse::eGetActiveCoordinateSystem;
    this->prepareResponse(request);

    QDomElement response = request.response.createElement("activeCoordinateSystem");
    if(!this->currentJob->getActiveCoordinateSystem().isNull()){
        response.setAttribute("ref", this->currentJob->getActiveCoordinateSystem()->getId());
    }else{
        response.setAttribute("ref", -1);
        request.response.documentElement().setAttribute("errorCode", OiRequestResponse::eNoActiveCoordinateSystem);
    }
    request.response.documentElement().appendChild(response);

    emit this->sendResponse(request);

}

/*!
 * \brief OiRequestHandler::setActiveCoordinateSystem
 * \param request
 */
void OiRequestHandler::setActiveCoordinateSystem(OiRequestResponse request){

    request.myRequestType = OiRequestResponse::eSetActiveCoordinateSystem;
    this->prepareResponse(request);

    int errorCode = 0;

    //set the active coordinate system
    QDomElement activeCoordinateSystem = request.request.documentElement().firstChildElement("activeCoordinateSystem");
    if(!activeCoordinateSystem.isNull() && activeCoordinateSystem.hasAttribute("ref")){
        QPointer<FeatureWrapper> myFeature = this->currentJob->getFeatureById(activeCoordinateSystem.attribute("ref").toInt());
        if(myFeature != NULL && myFeature->getCoordinateSystem() != NULL){
            myFeature->getCoordinateSystem()->setActiveCoordinateSystemState(true);
        }else{
            errorCode = OiRequestResponse::eNoFeatureWithId;
        }
    }else{
        errorCode = OiRequestResponse::eWrongFormat;
    }

    //add the new active coordinate system to XML response
    QDomElement response = request.response.createElement("activeCoordinateSystem");
    if(!this->currentJob->getActiveCoordinateSystem().isNull()){
        response.setAttribute("ref", this->currentJob->getActiveCoordinateSystem()->getId());
    }else{
        response.setAttribute("ref", -1);
    }
    request.response.documentElement().setAttribute("errorCode", errorCode);
    request.response.documentElement().appendChild(response);

    emit this->sendResponse(request);

}

/*!
 * \brief OiRequestHandler::aim
 * \param request
 */
void OiRequestHandler::aim(OiRequestResponse request){

    request.myRequestType = OiRequestResponse::eAim;
    this->prepareResponse(request);

    //check if active feature is valid
    if(this->currentJob->getActiveFeature().isNull() || this->currentJob->getActiveFeature()->getGeometry().isNull()){
        request.response.documentElement().setAttribute("errorCode", OiRequestResponse::eNoActiveFeature);
        emit this->sendResponse(request);
        return;
    }

    //check if active station is valid
    if(this->currentJob->getActiveStation()){
        request.response.documentElement().setAttribute("errorCode", OiRequestResponse::eNoActiveStation);
        emit this->sendResponse(request);
        return;
    }

    //check if there is a sensor connected
    if(!this->currentJob->getActiveStation()->getIsSensorConnected()){
        request.response.documentElement().setAttribute("errorCode", OiRequestResponse::eNoSensorConnected);
        emit this->sendResponse(request);
        return;
    }

    //check if active coordinate system is valid
    if(this->currentJob->getActiveCoordinateSystem().isNull()){
        request.response.documentElement().setAttribute("errorCode", OiRequestResponse::eNoActiveCoordinateSystem);
        emit this->sendResponse(request);
        return;
    }

    //start aiming the active feature
    emit this->startAim();

    emit this->sendResponse(request);

}

void OiRequestHandler::move(OiRequestResponse request){

}

/*!
 * \brief OiRequestHandler::measure
 * \param request
 */
void OiRequestHandler::measure(OiRequestResponse request){

    request.myRequestType = OiRequestResponse::eMeasure;
    this->prepareResponse(request);

    //check if active feature is valid
    if(this->currentJob->getActiveFeature().isNull() || this->currentJob->getActiveFeature()->getGeometry().isNull()){
        request.response.documentElement().setAttribute("errorCode", OiRequestResponse::eNoActiveFeature);
        emit this->sendResponse(request);
        return;
    }

    //check if active station is valid
    if(this->currentJob->getActiveStation()){
        request.response.documentElement().setAttribute("errorCode", OiRequestResponse::eNoActiveStation);
        emit this->sendResponse(request);
        return;
    }

    //check if there is a sensor connected
    if(!this->currentJob->getActiveStation()->getIsSensorConnected()){
        request.response.documentElement().setAttribute("errorCode", OiRequestResponse::eNoSensorConnected);
        emit this->sendResponse(request);
        return;
    }

    //check if active coordinate system is valid
    if(this->currentJob->getActiveCoordinateSystem().isNull()){
        request.response.documentElement().setAttribute("errorCode", OiRequestResponse::eNoActiveCoordinateSystem);
        emit this->sendResponse(request);
        return;
    }

    //start measurement
    emit this->startMeasurement();

}

/*!
 * \brief OiRequestHandler::startWatchwindow
 * \param request
 */
void OiRequestHandler::startWatchwindow(OiRequestResponse request){
/*
    //only one watch window task should be open at once
    if(this->watchWindowTask.taskInProcess){
        request.response.documentElement().setAttribute("errorCode", OiRequestResponse::eTaskInProcess);
        emit this->sendResponse(request);
        return;
    }

    request.myRequestType = OiRequestResponse::eStartWatchwindow;
    this->prepareResponse(request);

    //get requested reading type
    Configuration::ReadingTypes myReadingType;
    QDomElement readingType = request.request.documentElement().firstChildElement("readingType");
    if(!readingType.isNull() && readingType.hasAttribute("type")){
        myReadingType = (Configuration::ReadingTypes)readingType.attribute("type").toInt();
    }else{
        request.response.documentElement().setAttribute("errorCode", OiRequestResponse::eWrongFormat);
        emit this->sendResponse(request);
        return;
    }

    //check if active feature exists
    if(OiJob::getActiveFeature() == NULL || OiJob::getActiveFeature()->getGeometry() == NULL){
        request.response.documentElement().setAttribute("errorCode", OiRequestResponse::eNoActiveFeature);
        emit this->sendResponse(request);
        return;
    }

    if(OiJob::getActiveStation() != NULL && OiJob::getActiveStation()->sensorPad != NULL
            && OiJob::getActiveStation()->sensorPad->instrumentListener != NULL){

        //connect the reading stream to the request handler
        connect(OiJob::getActiveStation()->sensorPad->instrumentListener, SIGNAL(sendReadingMap(QVariantMap)),
                this, SLOT(receiveWatchWindowData(QVariantMap)));

        //start watch window
        OiJob::getActiveStation()->emitStartReadingStream(myReadingType); //TODO Übergabeparameter

        //save active watch window task
        this->watchWindowTask.taskInProcess = true;
        this->watchWindowTask.request = request;

    }else{
        request.response.documentElement().setAttribute("errorCode", OiRequestResponse::eNoActiveStation);
        emit this->sendResponse(request);
        return;
    }

    emit this->sendResponse(request);
*/
}

/*!
 * \brief OiRequestHandler::stopWatchwindow
 * \param request
 */
void OiRequestHandler::stopWatchwindow(OiRequestResponse request){
/*
    //if no task is in process, no task has to be stopped
    if(!this->watchWindowTask.taskInProcess){
        request.response.documentElement().setAttribute("errorCode", OiRequestResponse::eNoTask);
        emit this->sendResponse(request);
        return;
    }

    request.myRequestType = OiRequestResponse::eStopWatchwindow;
    this->prepareResponse(request);

    if(OiJob::getActiveStation() != NULL && OiJob::getActiveStation()->sensorPad != NULL
            && OiJob::getActiveStation()->sensorPad->instrumentListener != NULL){

        //disconnect the reading stream from the request handler
        disconnect(OiJob::getActiveStation()->sensorPad->instrumentListener, SIGNAL(sendReadingMap(QVariantMap)),
                this, SLOT(receiveWatchWindowData(QVariantMap)));

        //stop watch window
        OiJob::getActiveStation()->stopReadingStream();

        //reset active watch window task
        this->watchWindowTask.taskInProcess = false;
        delete this->watchWindowTask.request;
        this->watchWindowTask.request = NULL;

    }else{
        request.response.documentElement().setAttribute("errorCode", OiRequestResponse::eNoActiveStation);
        emit this->sendResponse(request);
        return;
    }

    emit this->sendResponse(request);
*/
}

/*!
 * \brief OiRequestHandler::prepareResponse
 * Creates a QDomElement for response and sets its attributes
 * \param request
 */
void OiRequestHandler::prepareResponse(OiRequestResponse &request){
    request.response.appendChild(request.response.createElement("OiResponse"));
    request.response.documentElement().setAttribute("ref", request.myRequestType);
    request.response.documentElement().setAttribute("errorCode", OiRequestResponse::eNoError);
}

/*!
 * \brief OiRequestHandler::buildWatchWindowMessage
 * \param wwTag: the XML tag that shall be filled
 * \param readingType: the requested reading type
 * \param streamData: the sensor stream data
 * \return
 */
bool OiRequestHandler::buildWatchWindowMessage(QDomElement &wwTag, const int &readingType, const QVariantMap &streamData){
/*
    //get xyz of active geometry
    OiVec xyz = OiJob::getActiveFeature()->getGeometry()->getXYZ();

    //get and transform sensor stream data
    switch(OiJob::getActiveStation()->getReadingStreamType()){
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
        if(!OiJob::getActiveStation()->coordSys->getIsActiveCoordinateSystem()){
            QList<TrafoParam*> trafoParams = OiJob::getActiveStation()->coordSys->getTransformationParameters(OiJob::getActiveCoordinateSystem());
            if(trafoParams.size() == 0){
                return false;
            }
            OiMat trafo = trafoParams.at(0)->getHomogenMatrix();
            if(trafo.getRowCount() != 4 || trafo.getColCount() != 4){
                return false;
            }
            streamXyz = trafo * streamXyz;
        }

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
*/
    return false;
}

/*!
 * \brief OiRequestHandler::receiveWatchWindowData
 * Each time a map with current coordinates is emitted by SensorListener
 * \param data
 */
void OiRequestHandler::receiveWatchWindowData(const QVariantMap &data){
/*
    if(!this->watchWindowTask.taskInProcess || this->watchWindowTask.request == NULL){
        return;
    }

    //clear the old response
    this->watchWindowTask.request.response.clear();

    this->watchWindowTask.request.myRequestType = OiRequestResponse::eStartWatchwindow;
    this->prepareResponse(this->watchWindowTask.request);

    //get requested reading type
    int myReadingType;
    QDomElement readingType = this->watchWindowTask.request.request.documentElement().firstChildElement("readingType");
    if(!readingType.isNull() && readingType.hasAttribute("type")){
        myReadingType = readingType.attribute("type").toInt();
    }else{
        this->watchWindowTask.request.response.documentElement().setAttribute("errorCode", OiRequestResponse::eWrongFormat);
        emit this->sendResponse(this->watchWindowTask.request);
        return;
    }

    //check if active feature exists and is a geometry
    if(OiJob::getActiveFeature() == NULL || OiJob::getActiveFeature()->getGeometry() == NULL){
        this->watchWindowTask.request.response.documentElement().setAttribute("errorCode", OiRequestResponse::eWrongFormat);
        emit this->sendResponse(this->watchWindowTask.request);
        return;
    }

    //check if active station exists
    if(OiJob::getActiveStation() == NULL){
        this->watchWindowTask.request.response.documentElement().setAttribute("errorCode", OiRequestResponse::eWrongFormat);
        emit this->sendResponse(this->watchWindowTask.request);
        return;
    }

    //create feature info tag
    QDomElement featureTag = this->watchWindowTask.request.response.createElement("geometry");
    featureTag.setAttribute("id", OiJob::getActiveFeature()->getFeature()->getId());
    featureTag.setAttribute("name", OiJob::getActiveFeature()->getFeature()->getFeatureName());

    //create tag with watch window data
    QDomElement wwTag = this->watchWindowTask.request.response.createElement("cartesian");
    this->buildWatchWindowMessage(wwTag, myReadingType, data);

    qDebug() << "wwtag " << wwTag.text();

    //build xml response
    this->watchWindowTask.request.response.documentElement().appendChild(featureTag);
    this->watchWindowTask.request.response.documentElement().appendChild(wwTag);

    qDebug() << "response: " << this->watchWindowTask.request.response.toString();

    emit this->sendResponse(this->watchWindowTask.request);
*/
}

/*!
 * \brief OiRequestHandler::measurementFinished
 * \param geomId
 * \param success
 */
void OiRequestHandler::measurementFinished(const bool &success){

    //check if a clien-requested task is in progress
    if(!this->measurementTask.taskInProcess){
        return;
    }

    this->measurementTask.taskInProcess = false;

    this->measurementTask.request.response.documentElement().setAttribute("errorCode", success ? 0 : OiRequestResponse::eMeasurementError);

    emit this->sendResponse(this->measurementTask.request);

}
