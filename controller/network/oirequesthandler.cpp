#include "oirequesthandler.h"

//QPointer<OiRequestHandler> OiRequestHandler::myRequestHandler(NULL);

OiRequestHandler::OiRequestHandler(QObject *parent){

    //initially no watch window task in process
    this->watchWindowTask.taskInProgress = false;

}

/*!
 * \brief OiRequestHandler::getCurrentJob
 * \return
 */
const QPointer<OiJob> &OiRequestHandler::getCurrentJob() const{
    return this->currentJob;
}

/*!
 * \brief OiRequestHandler::setCurrentJob
 * \param job
 */
void OiRequestHandler::setCurrentJob(const QPointer<OiJob> &job){
    if(!job.isNull()){
        this->currentJob = job;
        sorter.setCurrentJob(job);
    }
}

/*!
 * \brief OiRequestHandler::getSensorConfigManager
 * \return
 */
const QPointer<SensorConfigurationManager> &OiRequestHandler::getSensorConfigManager() const{
    return this->sensorConfigManager;
}

/*!
 * \brief OiRequestHandler::setSensorConfigManager
 * \param sensorConfigManager
 */
void OiRequestHandler::setSensorConfigManager(const QPointer<SensorConfigurationManager> &sensorConfigManager){
    this->sensorConfigManager = sensorConfigManager;
}

/*!
 * \brief OiRequestHandler::getMeasurementConfigManager
 * \return
 */
const QPointer<MeasurementConfigManager> &OiRequestHandler::getMeasurementConfigManager() const{
    return this->measurementConfigManager;
}

/*!
 * \brief OiRequestHandler::setMeasurementConfigManager
 * \param measurementConfigManager
 */
void OiRequestHandler::setMeasurementConfigManager(const QPointer<MeasurementConfigManager> &measurementConfigManager){
    this->measurementConfigManager = measurementConfigManager;
}

/*!
 * \brief OiRequestHandler::receiveRequest
 * Parses a XML request and calls the corresponding method to do the requested task
 * \param request
 * \return
 */
bool OiRequestHandler::receiveRequest(OiRequestResponse request){

    //check request
    if(request.request.isNull() || request.request.documentElement().isNull()
            || request.request.documentElement().tagName().compare("OiRequest") != 0
            || !request.request.documentElement().hasAttribute("id")){
        this->sendErrorMessage(request, OiRequestResponse::eGetProject, OiRequestResponse::eWrongFormat);
        return false;
    }

    //get request type
    int id = request.request.documentElement().attribute("id").toInt();
    OiRequestResponse::RequestType type = this->getRequestType(id);

    //check current job
    if(this->currentJob.isNull()){
        this->sendErrorMessage(request, type, OiRequestResponse::eNoJob);
        return false;
    }

    //perform the task defined by type
    switch(type){
    case OiRequestResponse::eGetProject:
        this->getProject(request);
        return true;
    case OiRequestResponse::eGetActiveFeature:
        this->getActiveFeature(request);
        return true;
    case OiRequestResponse::eSetActiveFeature:
        this->setActiveFeature(request);
        return true;
    case OiRequestResponse::eGetActiveStation:
        this->getActiveStation(request);
        return true;
    case OiRequestResponse::eSetActiveStation:
        this->setActiveStation(request);
        return true;
    case OiRequestResponse::eGetActiveCoordinateSystem:
        this->getActiveCoordinateSystem(request);
        return true;
    case OiRequestResponse::eSetActiveCoordinateSystem:
        this->setActiveCoordinateSystem(request);
        return true;
    case OiRequestResponse::eAim:
        this->aim(request);
        return true;
    case OiRequestResponse::eMeasure:
        this->measure(request);
        return true;
    case OiRequestResponse::eStartWatchwindow:
        this->startWatchwindow(request);
        return true;
    case OiRequestResponse::eStopWatchwindow:
        this->stopWatchwindow(request);
        return true;
    case OiRequestResponse::eOiToolRequest:
        //emit this->sendOiToolRequest(request);
        return true;
    case OiRequestResponse::eGetFeatures:
        this->getFeatures(request);
        return true;
    case OiRequestResponse::eAddFeatures:
        this->addFeatures(request);
        return true;
    case OiRequestResponse::eGetObservations:
        this->getObservations(request);
        return true;
    case OiRequestResponse::eRemoveObservations:
        this->removeObservations(request);
        return true;
    case OiRequestResponse::eGetParameters:
        this->getParameters(request);
        return true;
    case OiRequestResponse::eGetMeasurementConfigs:
        this->getMeasurementConfigs(request);
        return true;
    case OiRequestResponse::eGetMeasurementConfig:
        this->getMeasurementConfig(request);
        return true;
    case OiRequestResponse::eSetMeasurementConfig:
        this->setMeasurementConfig(request);
        return true;
    case OiRequestResponse::eGetCoordinateSystems:
        this->getCoordinateSystems(request);
        return true;
    default:
        this->sendErrorMessage(request, OiRequestResponse::eGetProject, OiRequestResponse::eUnknownRequestType);
        return false;
    }

}

/*!
 * \brief OiRequestHandler::sensorActionStarted
 * \param name
 */
void OiRequestHandler::sensorActionStarted(const QString &name){

    //set up request
    OiRequestResponse request;
    request.requesterId = -1;
    request.myRequestType = OiRequestResponse::eSensorActionStarted;
    this->prepareResponse(request);

    //set up action
    QDomElement response = request.response.createElement("action");
    response.setAttribute("name", name);
    request.response.documentElement().appendChild(response);

    emit this->sendResponse(request);

}

/*!
 * \brief OiRequestHandler::sensorActionFinished
 * \param success
 * \param msg
 */
void OiRequestHandler::sensorActionFinished(const bool &success, const QString &msg){

    //set up request
    OiRequestResponse request;
    request.requesterId = -1;
    request.myRequestType = OiRequestResponse::eSensorActionFinished;
    this->prepareResponse(request);

    //set up action
    QDomElement response = request.response.createElement("action");
    response.setAttribute("success", success?"1":"0");
    response.setAttribute("message", msg);
    request.response.documentElement().appendChild(response);

    emit this->sendResponse(request);

}

/*!
 * \brief OiRequestHandler::log
 * \param msg
 * \param msgType
 */
void OiRequestHandler::log(const QString &msg, const MessageTypes &msgType){

    //set up request
    OiRequestResponse request;
    request.requesterId = -1;
    request.myRequestType = OiRequestResponse::eMessageBox;
    this->prepareResponse(request);

    //set up message
    QDomElement response = request.response.createElement("message");
    response.setAttribute("text", msg);
    response.setAttribute("type", QString::number(msgType));
    request.response.documentElement().appendChild(response);

    emit this->sendResponse(request);

}

/*!
 * \brief OiRequestHandler::realTimeReading
 * \param reading
 */
void OiRequestHandler::realTimeReading(const QVariantMap &reading){

    //set up request
    OiRequestResponse request;
    request.requesterId = -1;
    request.myRequestType = OiRequestResponse::eRealTimeReading;
    this->prepareResponse(request);

    //check watch window task
    if(!this->watchWindowTask.taskInProgress){
        return;
    }
    request.requesterId = this->watchWindowTask.request.requesterId;

    //set up real time data
    QDomElement measurement = request.response.createElement("measurement");
    if(!this->buildWatchWindowMessage(measurement, reading)){
        return;
    }
    request.response.documentElement().appendChild(measurement);

    emit this->sendResponse(request);

}

/*!
 * \brief OiRequestHandler::activeFeatureChanged
 */
void OiRequestHandler::activeFeatureChanged(){

    //set up request
    OiRequestResponse request;
    request.requesterId = -1;
    request.myRequestType = OiRequestResponse::eActiveFeatureChanged;
    this->prepareResponse(request);

    emit this->sendResponse(request);

}

/*!
 * \brief OiRequestHandler::activeStationChanged
 */
void OiRequestHandler::activeStationChanged(){

    //set up request
    OiRequestResponse request;
    request.requesterId = -1;
    request.myRequestType = OiRequestResponse::eActiveStationChanged;
    this->prepareResponse(request);

    emit this->sendResponse(request);

}

/*!
 * \brief OiRequestHandler::activeCoordinateSystemChanged
 */
void OiRequestHandler::activeCoordinateSystemChanged(){

    //set up request
    OiRequestResponse request;
    request.requesterId = -1;
    request.myRequestType = OiRequestResponse::eActiveCoordinateSystemChanged;
    this->prepareResponse(request);

    emit this->sendResponse(request);

}

/*!
 * \brief OiRequestHandler::featureSetChanged
 */
void OiRequestHandler::featureSetChanged(){

    //set up request
    OiRequestResponse request;
    request.requesterId = -1;
    request.myRequestType = OiRequestResponse::eFeatureSetChanged;
    this->prepareResponse(request);

    emit this->sendResponse(request);

}

/*!
 * \brief OiRequestHandler::featureAttributesChanged
 */
void OiRequestHandler::featureAttributesChanged(){

    //set up request
    OiRequestResponse request;
    request.requesterId = -1;
    request.myRequestType = OiRequestResponse::eFeatureAttributesChanged;
    this->prepareResponse(request);

    emit this->sendResponse(request);

}

/*!
 * \brief OiRequestHandler::getProject
 * \param request
 */
void OiRequestHandler::getProject(OiRequestResponse &request){

    //set up request
    request.myRequestType = OiRequestResponse::eGetProject;
    this->prepareResponse(request);

    //get and set project xml
    QDomDocument project = ProjectExchanger::saveProject(this->currentJob);
    if(!project.isNull()){
        request.response.documentElement().appendChild(request.response.importNode(project.documentElement(), true));
    }

    emit this->sendResponse(request);

}

/*!
 * \brief OiRequestHandler::getActiveFeature
 * \param request
 */
void OiRequestHandler::getActiveFeature(OiRequestResponse &request){

    //set up request
    request.myRequestType = OiRequestResponse::eGetActiveFeature;
    this->prepareResponse(request);

    //get and set active feature
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
void OiRequestHandler::setActiveFeature(OiRequestResponse &request){

    //set up request
    request.myRequestType = OiRequestResponse::eSetActiveFeature;
    this->prepareResponse(request);

    int errorCode = OiRequestResponse::eNoError;

    //set the active feature
    QDomElement activeFeature = request.request.documentElement().firstChildElement("activeFeature");
    if(!activeFeature.isNull() && activeFeature.hasAttribute("ref")){
        QPointer<FeatureWrapper> myFeature = this->currentJob->getFeatureById(activeFeature.attribute("ref").toInt());
        if(!myFeature.isNull() && !myFeature->getFeature().isNull()){
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
void OiRequestHandler::getActiveStation(OiRequestResponse &request){

    //set up request
    request.myRequestType = OiRequestResponse::eGetActiveStation;
    this->prepareResponse(request);

    //get and set active station
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
void OiRequestHandler::setActiveStation(OiRequestResponse &request){

    //set up request
    request.myRequestType = OiRequestResponse::eSetActiveStation;
    this->prepareResponse(request);

    int errorCode = OiRequestResponse::eNoError;

    //set the active station
    QDomElement activeStation = request.request.documentElement().firstChildElement("activeStation");
    if(!activeStation.isNull() && activeStation.hasAttribute("ref")){
        QPointer<FeatureWrapper> myFeature = this->currentJob->getFeatureById(activeStation.attribute("ref").toInt());
        if(!myFeature.isNull() && !myFeature->getStation().isNull()){
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
void OiRequestHandler::getActiveCoordinateSystem(OiRequestResponse &request){

    //set up request
    request.myRequestType = OiRequestResponse::eGetActiveCoordinateSystem;
    this->prepareResponse(request);

    //get and set active coordinate system
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
void OiRequestHandler::setActiveCoordinateSystem(OiRequestResponse &request){

    //set up request
    request.myRequestType = OiRequestResponse::eSetActiveCoordinateSystem;
    this->prepareResponse(request);

    int errorCode = OiRequestResponse::eNoError;

    //set the active coordinate system
    QDomElement activeCoordinateSystem = request.request.documentElement().firstChildElement("activeCoordinateSystem");
    if(!activeCoordinateSystem.isNull() && activeCoordinateSystem.hasAttribute("ref")){
        QPointer<FeatureWrapper> myFeature = this->currentJob->getFeatureById(activeCoordinateSystem.attribute("ref").toInt());
        if(!myFeature.isNull() && !myFeature->getCoordinateSystem().isNull()){
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
void OiRequestHandler::aim(OiRequestResponse &request){

    //set up request
    request.myRequestType = OiRequestResponse::eAim;
    this->prepareResponse(request);

    //get feature id
    QDomElement id = request.request.documentElement().firstChildElement("feature");
    if(id.isNull() || !id.hasAttribute("ref")){
        request.response.documentElement().setAttribute("errorCode", OiRequestResponse::eWrongFormat);
        emit this->sendResponse(request);
        return;
    }

    //check if active station is valid
    if(this->currentJob->getActiveStation().isNull()){
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

    //set the active feature
    QPointer<FeatureWrapper> feature = this->currentJob->getFeatureById(id.attribute("ref").toInt());
    if(feature.isNull() || feature->getGeometry().isNull()){
        request.response.documentElement().setAttribute("errorCode", OiRequestResponse::eNoFeatureWithId);
        emit this->sendResponse(request);
        return;
    }
    feature->getGeometry()->setActiveFeatureState(true);

    //start aiming the active feature
    emit this->startAim();

    emit this->sendResponse(request);

}

/*!
 * \brief OiRequestHandler::measure
 * \param request
 */
void OiRequestHandler::measure(OiRequestResponse &request){

    //set up request
    request.myRequestType = OiRequestResponse::eMeasure;
    this->prepareResponse(request);

    //get feature id
    QDomElement id = request.request.documentElement().firstChildElement("feature");
    if(id.isNull() || !id.hasAttribute("ref")){
        request.response.documentElement().setAttribute("errorCode", OiRequestResponse::eWrongFormat);
        emit this->sendResponse(request);
        return;
    }

    //check if active station is valid
    if(this->currentJob->getActiveStation().isNull()){
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

    //set the active feature
    QPointer<FeatureWrapper> feature = this->currentJob->getFeatureById(id.attribute("ref").toInt());
    if(feature.isNull() || feature->getGeometry().isNull()){
        request.response.documentElement().setAttribute("errorCode", OiRequestResponse::eNoFeatureWithId);
        emit this->sendResponse(request);
        return;
    }
    feature->getGeometry()->setActiveFeatureState(true);

    //start measurement
    emit this->startMeasurement();

    emit this->sendResponse(request);

}

/*!
 * \brief OiRequestHandler::startWatchwindow
 * \param request
 */
void OiRequestHandler::startWatchwindow(OiRequestResponse &request){

    //set up request
    request.myRequestType = OiRequestResponse::eStartWatchwindow;
    this->prepareResponse(request);

    //only one watch window task should be open at once
    if(this->watchWindowTask.taskInProgress){
        request.response.documentElement().setAttribute("errorCode", OiRequestResponse::eTaskInProcess);
        emit this->sendResponse(request);
        return;
    }

    //get requested reading type
    ReadingTypes myReadingType;
    QDomElement readingType = request.request.documentElement().firstChildElement("readingType");
    if(!readingType.isNull() && readingType.hasAttribute("type")){
        myReadingType = (ReadingTypes)readingType.attribute("type").toInt();
    }else{
        request.response.documentElement().setAttribute("errorCode", OiRequestResponse::eWrongFormat);
        emit this->sendResponse(request);
        return;
    }

    //check active feature
    if(this->currentJob->getActiveFeature().isNull() || this->currentJob->getActiveFeature()->getFeature().isNull()){
        request.response.documentElement().setAttribute("errorCode", OiRequestResponse::eNoActiveFeature);
        emit this->sendResponse(request);
        return;
    }

    //get and check active station
    QPointer<Station> activeStation = this->currentJob->getActiveStation();
    if(activeStation.isNull()){
        request.response.documentElement().setAttribute("errorCode", OiRequestResponse::eNoActiveStation);
        emit this->sendResponse(request);
        return;
    }

    //check sensor
    if(!activeStation->getIsSensorConnected()){
        request.response.documentElement().setAttribute("errorCode", OiRequestResponse::eNoSensorConnected);
        emit this->sendResponse(request);
        return;
    }

    //connect the reading stream to the request handler
    QObject::connect(activeStation, &Station::realTimeReading,
                     this, &OiRequestHandler::realTimeReading, Qt::AutoConnection);

    //start watch window
    emit this->startReadingStream(myReadingType);

    //save active watch window task
    this->watchWindowTask.taskInProgress = true;
    this->watchWindowTask.type = myReadingType;
    this->watchWindowTask.request = request;

    emit this->sendResponse(request);

}

/*!
 * \brief OiRequestHandler::stopWatchwindow
 * \param request
 */
void OiRequestHandler::stopWatchwindow(OiRequestResponse &request){

    //set up request
    request.myRequestType = OiRequestResponse::eStopWatchwindow;
    this->prepareResponse(request);

    //if no task is in process, no task has to be stopped
    if(!this->watchWindowTask.taskInProgress){
        request.response.documentElement().setAttribute("errorCode", OiRequestResponse::eNoTask);
        emit this->sendResponse(request);
        return;
    }

    //get and check active station
    QPointer<Station> activeStation = this->currentJob->getActiveStation();
    if(activeStation.isNull()){
        request.response.documentElement().setAttribute("errorCode", OiRequestResponse::eNoActiveStation);
        emit this->sendResponse(request);
        return;
    }

    //disconnect the reading stream from the request handler
    QObject::disconnect(activeStation, &Station::realTimeReading,
                        this, &OiRequestHandler::realTimeReading);

    //stop watch window
    emit this->stopReadingStream();

    //reset active watch window task
    this->watchWindowTask.taskInProgress = false;
    this->watchWindowTask.request = OiRequestResponse();

    emit this->sendResponse(request);

}

/*!
 * \brief OiRequestHandler::toolRequest
 * \param request
 */
void OiRequestHandler::toolRequest(OiRequestResponse &request){

}

/*!
 * \brief OiRequestHandler::getFeatures
 * \param request
 */
void OiRequestHandler::getFeatures(OiRequestResponse &request){

    //set up request
    request.myRequestType = OiRequestResponse::eGetFeatures;
    this->prepareResponse(request);

    //get a list of all features
    QList<QPointer<FeatureWrapper> > features = this->currentJob->getFeaturesList();

    //sort the list according to the current sorting mode
    qSort(features.begin(), features.end(), this->sorter);

    //create and add features
    QDomElement response = request.response.createElement("features");
    foreach(const QPointer<FeatureWrapper> &feature, features){

        //check feature
        if(feature.isNull() || feature->getFeature().isNull()){
            continue;
        }

        //create feature element
        QDomElement featureTag = request.response.createElement("feature");
        featureTag.setAttribute("type", feature->getFeatureTypeEnum());

        //add feature information
        QDomElement id = request.response.createElement("id");
        QDomText idText = request.response.createTextNode(QString::number(feature->getFeature()->getId()));
        id.appendChild(idText);
        featureTag.appendChild(id);
        QDomElement name = request.response.createElement("name");
        QDomText nameText = request.response.createTextNode(feature->getFeature()->getFeatureName());
        name.appendChild(nameText);
        featureTag.appendChild(name);
        QDomElement group = request.response.createElement("group");
        QDomText groupText = request.response.createTextNode(feature->getFeature()->getGroupName());
        group.appendChild(groupText);
        featureTag.appendChild(group);
        QDomElement isSolved = request.response.createElement("isSolved");
        QDomText isSolvedText = request.response.createTextNode(feature->getFeature()->getIsSolved()?"1":"0");
        isSolved.appendChild(isSolvedText);
        featureTag.appendChild(isSolved);
        QDomElement isNominal = request.response.createElement("isNominal");
        QDomText isNominalText = request.response.createTextNode((feature->getGeometry().isNull() || !feature->getGeometry()->getIsNominal())?"0":"1");
        isNominal.appendChild(isNominalText);
        featureTag.appendChild(isNominal);

        //add feature
        response.appendChild(featureTag);

    }
    request.response.documentElement().appendChild(response);

    emit this->sendResponse(request);

}

/*!
 * \brief OiRequestHandler::addFeatures
 * \param request
 */
void OiRequestHandler::addFeatures(OiRequestResponse &request){

    //set up request
    request.myRequestType = OiRequestResponse::eAddFeatures;
    this->prepareResponse(request);

    //check measurement config manager
    if(this->measurementConfigManager.isNull()){
        this->sendErrorMessage(request, OiRequestResponse::eGetMeasurementConfigs, OiRequestResponse::eNoMeasurementConfigManager);
        return;
    }

    //check request
    QDomElement type = request.request.documentElement().firstChildElement("type");
    QDomElement name = request.request.documentElement().firstChildElement("name");
    QDomElement group = request.request.documentElement().firstChildElement("group");
    QDomElement count = request.request.documentElement().firstChildElement("count");
    QDomElement isActual = request.request.documentElement().firstChildElement("isActual");
    QDomElement isNominal = request.request.documentElement().firstChildElement("isNominal");
    QDomElement nominalSystem = request.request.documentElement().firstChildElement("nominalSystem");
    QDomElement measurementConfig = request.request.documentElement().firstChildElement("measurementConfig");
    if(type.isNull() || name.isNull() || group.isNull() || count.isNull()
            || this->getFeatureType(type.text().toInt()) == eUndefinedFeature){
        this->sendErrorMessage(request, OiRequestResponse::eAddFeatures, OiRequestResponse::eWrongFormat);
        return;
    }

    //set up feature attributes
    FeatureAttributes attr;
    attr.typeOfFeature = this->getFeatureType(type.text().toInt());
    attr.name = name.text();
    attr.group = group.text();
    attr.count = count.text().toInt();
    if(!isActual.isNull()){
        attr.isActual = isActual.text().toInt();
    }
    if(!isNominal.isNull()){
        attr.isNominal = isNominal.text().toInt();
    }
    if(!nominalSystem.isNull()){
        attr.nominalSystem = nominalSystem.text();
    }
    if(!measurementConfig.isNull()){
        attr.mConfig = measurementConfig.text();
    }

    //add features
    QList<QPointer<FeatureWrapper> > features = this->currentJob->addFeatures(attr);

    //get and check measurement config
    MeasurementConfig mConfig = this->measurementConfigManager->getProjectConfig(attr.mConfig);
    if(!mConfig.getIsValid()){
        mConfig = this->measurementConfigManager->getUserConfig(attr.mConfig);
    }

    //pass measurement config to features
    foreach(const QPointer<FeatureWrapper> &feature, features){
        if(!feature.isNull() && !feature->getGeometry().isNull() && !feature->getGeometry()->getIsNominal()){
            feature->getGeometry()->setMeasurementConfig(mConfig);
        }
    }

    emit this->sendResponse(request);

}

/*!
 * \brief OiRequestHandler::getObservations
 * \param request
 */
void OiRequestHandler::getObservations(OiRequestResponse &request){

    //set up request
    request.myRequestType = OiRequestResponse::eGetObservations;
    this->prepareResponse(request);

    //check request
    QDomElement id = request.request.documentElement().firstChildElement("id");
    if(id.isNull()){
        this->sendErrorMessage(request, OiRequestResponse::eGetObservations, OiRequestResponse::eWrongFormat);
        return;
    }

    //get and check feature by id
    QPointer<FeatureWrapper> feature = this->currentJob->getFeatureById(id.text().toInt());
    if(feature.isNull() || feature->getGeometry().isNull()){
        this->sendErrorMessage(request, OiRequestResponse::eGetObservations, OiRequestResponse::eNoFeatureWithId);
        return;
    }

    //add id
    request.response.documentElement().appendChild(request.response.importNode(id, true));

    //create and add observations
    QDomElement response = request.response.createElement("observations");
    foreach(const QPointer<Observation> &obs, feature->getGeometry()->getObservations()){

        //check observation
        if(obs.isNull()){
            continue;
        }

        //create observation element
        QDomElement observation = request.response.createElement("observation");

        //add observation information
        QDomElement id = request.response.createElement("id");
        QDomText idText = request.response.createTextNode(QString::number(obs->getId()));
        id.appendChild(idText);
        observation.appendChild(id);
        QDomElement x = request.response.createElement("x");
        QDomText xText = request.response.createTextNode(QString::number(obs->getXYZ().getAt(0), 'f', 7));
        x.appendChild(xText);
        observation.appendChild(x);
        QDomElement y = request.response.createElement("y");
        QDomText yText = request.response.createTextNode(QString::number(obs->getXYZ().getAt(1), 'f', 7));
        y.appendChild(yText);
        observation.appendChild(y);
        QDomElement z = request.response.createElement("z");
        QDomText zText = request.response.createTextNode(QString::number(obs->getXYZ().getAt(2), 'f', 7));
        z.appendChild(zText);
        observation.appendChild(z);
        QDomElement isValid = request.response.createElement("isValid");
        QDomText isValidText = request.response.createTextNode(obs->getIsValid()?"1":"0");
        isValid.appendChild(isValidText);
        observation.appendChild(isValid);

        //add deviations and use state
        if(feature->getGeometry()->getFunctions().size() > 0
                && !feature->getGeometry()->getFunctions().at(0).isNull()){

            //get function and statistic
            QPointer<Function> function = feature->getGeometry()->getFunctions().at(0);
            const Statistic &statistic = function->getStatistic();
            Residual residual = statistic.getDisplayResidual(obs->getId());

            //get deviation attributes
            QString attrVx = getObservationDisplayAttributesName(eObservationDisplayVX);
            QString attrVy = getObservationDisplayAttributesName(eObservationDisplayVY);
            QString attrVz = getObservationDisplayAttributesName(eObservationDisplayVZ);
            QString attrV = getObservationDisplayAttributesName(eObservationDisplayV);
            if(residual.elementId == obs->getId() && residual.corrections.contains(attrVx)
                    && residual.corrections.contains(attrVy) && residual.corrections.contains(attrVz)
                    && residual.corrections.contains(attrV)){

                //set up deviations
                QDomElement vx = request.response.createElement("vx");
                QDomText vxText = request.response.createTextNode(QString::number(residual.corrections[attrVx], 'f', 7));
                vx.appendChild(vxText);
                observation.appendChild(vx);
                QDomElement vy = request.response.createElement("vy");
                QDomText vyText = request.response.createTextNode(QString::number(residual.corrections[attrVy], 'f', 7));
                vy.appendChild(vyText);
                observation.appendChild(vy);
                QDomElement vz = request.response.createElement("vz");
                QDomText vzText = request.response.createTextNode(QString::number(residual.corrections[attrVz], 'f', 7));
                vz.appendChild(vzText);
                observation.appendChild(vz);
                QDomElement v = request.response.createElement("v");
                QDomText vText = request.response.createTextNode(QString::number(residual.corrections[attrV], 'f', 7));
                v.appendChild(vText);
                observation.appendChild(v);

            }

            //set up use state
            QDomElement isUsed = request.response.createElement("isUsed");
            QDomText isUsedText = request.response.createTextNode(function->getIsUsed(0, obs->getId())?"1":"0");
            isUsed.appendChild(isUsedText);
            observation.appendChild(isUsed);

        }

        //add observation
        response.appendChild(observation);

    }
    request.response.documentElement().appendChild(response);

    emit this->sendResponse(request);

}

/*!
 * \brief OiRequestHandler::removeObservations
 * \param request
 */
void OiRequestHandler::removeObservations(OiRequestResponse &request){

    //set up request
    request.myRequestType = OiRequestResponse::eRemoveObservations;
    this->prepareResponse(request);

    //check request
    QDomElement id = request.request.documentElement().firstChildElement("id");
    QDomElement observations = request.request.documentElement().firstChildElement("observations");
    if(id.isNull() || observations.isNull()){
        this->sendErrorMessage(request, OiRequestResponse::eRemoveObservations, OiRequestResponse::eWrongFormat);
        return;
    }

    //get and check feature
    QPointer<FeatureWrapper> feature = this->currentJob->getFeatureById(id.text().toInt());
    if(feature.isNull() || feature->getGeometry().isNull()){
        this->sendErrorMessage(request, OiRequestResponse::eRemoveObservations, OiRequestResponse::eNoFeatureWithId);
        return;
    }

    //get a list of observations and functions of the feature
    QList<QPointer<Observation> > featureObservations = feature->getGeometry()->getObservations();
    QList<QPointer<Function> > featureFunctions = feature->getGeometry()->getFunctions();

    //get a list of observation id's that shall be removed
    QList<int> obs2Remove;
    QDomNodeList observationList = observations.childNodes();
    for(int i = 0; i < observationList.size(); i++){

        //parse node to element
        QDomElement elem = observationList.at(i).toElement();
        if(elem.isNull() || !elem.hasAttribute("id")){
            continue;
        }

        obs2Remove.append(elem.attribute("id").toInt());

    }

    //run through all observations and remove the marked ones
    foreach(const QPointer<Observation> &obs, featureObservations){

        //check observation
        if(obs.isNull() || !obs2Remove.contains(obs->getId())){
            continue;
        }

        //remove observation from functions
        foreach(const QPointer<Function> &function, featureFunctions){
            if(!function.isNull()){
                function->removeInputElement(obs->getId());
            }
        }

        //reset dependency between feature and observation
        obs->removeTargetGeometry(feature->getGeometry()->getId());

    }

    emit this->sendResponse(request);

}

/*!
 * \brief OiRequestHandler::getParameters
 * \param request
 */
void OiRequestHandler::getParameters(OiRequestResponse &request){

    //set up request
    request.myRequestType = OiRequestResponse::eGetParameters;
    this->prepareResponse(request);

    //check request
    QDomElement id = request.request.documentElement().firstChildElement("id");
    if(id.isNull()){
        this->sendErrorMessage(request, OiRequestResponse::eGetParameters, OiRequestResponse::eWrongFormat);
        return;
    }

    //get and check feature by id
    QPointer<FeatureWrapper> feature = this->currentJob->getFeatureById(id.text().toInt());
    if(feature.isNull() || feature->getFeature().isNull()){
        this->sendErrorMessage(request, OiRequestResponse::eGetParameters, OiRequestResponse::eNoFeatureWithId);
        return;
    }

    //add id
    request.response.documentElement().appendChild(request.response.importNode(id, true));

    //add feature information
    QDomElement name = request.response.createElement("name");
    QDomText nameText = request.response.createTextNode(feature->getFeature()->getFeatureName());
    name.appendChild(nameText);
    request.response.documentElement().appendChild(name);
    QDomElement type = request.response.createElement("type");
    QDomText typeText = request.response.createTextNode(QString::number(feature->getFeatureTypeEnum()));
    type.appendChild(typeText);
    request.response.documentElement().appendChild(type);
    QDomElement group = request.response.createElement("group");
    QDomText groupText = request.response.createTextNode(feature->getFeature()->getGroupName());
    group.appendChild(groupText);
    request.response.documentElement().appendChild(group);
    QDomElement isSolved = request.response.createElement("isSolved");
    QDomText isSolvedText = request.response.createTextNode(feature->getFeature()->getIsSolved()?"1":"0");
    isSolved.appendChild(isSolvedText);
    request.response.documentElement().appendChild(isSolved);
    QDomElement isNominal = request.response.createElement("isNominal");
    QDomText isNominalText = request.response.createTextNode((feature->getGeometry().isNull() || !feature->getGeometry()->getIsNominal())?"0":"1");
    isNominal.appendChild(isNominalText);
    request.response.documentElement().appendChild(isNominal);

    //add parameters
    QDomElement parameters = request.response.createElement("parameters");
    this->addParameters(request.response, parameters, feature);
    request.response.documentElement().appendChild(parameters);

    //add standard deviation
    if(getIsGeometry(feature->getFeatureTypeEnum())){
        QDomElement stdev = request.response.createElement("stdev");
        QDomText stdevText = request.response.createTextNode(QString::number(feature->getGeometry()->getStatistic().getStdev(), 'f', 7));
        stdev.appendChild(stdevText);
        request.response.documentElement().appendChild(stdev);
    }

    emit this->sendResponse(request);

}

QDomElement OiRequestHandler::toXML(const MeasurementConfig &mConfig, const bool saved, QDomDocument &response){
    QDomElement config = response.createElement("measurementConfig");

    QDomElement name = response.createElement("name");
    QDomText nameText = response.createTextNode(mConfig.getName());
    name.appendChild(nameText);
    config.appendChild(name);

    QDomElement isSaved = response.createElement("isSaved");
    QDomText isSavedText = response.createTextNode(saved ? "1" : "0");
    isSaved.appendChild(isSavedText);
    config.appendChild(isSaved);

    QDomElement measurementType = response.createElement("measurementType");
    QDomText measurementTypeText = response.createTextNode(QString::number(mConfig.getMeasurementType()));
    measurementType.appendChild(measurementTypeText);
    config.appendChild(measurementType);

    QDomElement measurementMode = response.createElement("measurementMode");
    QDomText measurementModeText = response.createTextNode(QString::number(mConfig.getMeasurementMode()));
    measurementMode.appendChild(measurementModeText);
    config.appendChild(measurementMode);

    QDomElement maxObservations = response.createElement("maxObservations");
    QDomText maxObservationsText = response.createTextNode(QString::number(mConfig.getMaxObservations()));
    maxObservations.appendChild(maxObservationsText);
    config.appendChild(maxObservations);

    QDomElement measureTwoSides = response.createElement("measureTwoSides");
    QDomText measureTwoSidesText = response.createTextNode(mConfig.getMeasureTwoSides()?"1":"0");
    measureTwoSides.appendChild(measureTwoSidesText);
    config.appendChild(measureTwoSides);

    QDomElement timeInterval = response.createElement("timeInterval");
    QDomText timeIntervalText = response.createTextNode(QString::number(mConfig.getTimeInterval()));
    timeInterval.appendChild(timeIntervalText);
    config.appendChild(timeInterval);

    QDomElement distanceInterval = response.createElement("distanceInterval");
    QDomText distanceIntervalText = response.createTextNode(QString::number(mConfig.getDistanceInterval()));
    distanceInterval.appendChild(distanceIntervalText);
    config.appendChild(distanceInterval);

    return config;
}

/*!
 * \brief OiRequestHandler::getMeasurementConfigs
 * \param request
 */
void OiRequestHandler::getMeasurementConfigs(OiRequestResponse &request){

    //set up request
    request.myRequestType = OiRequestResponse::eGetMeasurementConfigs;
    this->prepareResponse(request);

    //check measurement config manager
    if(this->measurementConfigManager.isNull()){
        this->sendErrorMessage(request, OiRequestResponse::eGetMeasurementConfigs, OiRequestResponse::eNoMeasurementConfigManager);
        return;
    }

    //get all measurement configs
    QList<MeasurementConfig> userConfigs = this->measurementConfigManager->getUserConfigs();
    QList<MeasurementConfig> projectConfigs = this->measurementConfigManager->getProjectConfigs();

    //add configs
    QDomElement configs = request.response.createElement("measurementConfigs");
    foreach(const MeasurementConfig &mConfig, userConfigs){
        configs.appendChild(toXML(mConfig, true, request.response));
    }
    foreach(const MeasurementConfig &mConfig, projectConfigs){
        configs.appendChild(toXML(mConfig, false, request.response));
    }
    request.response.documentElement().appendChild(configs);

    emit this->sendResponse(request);

}

/*!
 * \brief OiRequestHandler::getMeasurementConfig
 * \param request
 */
void OiRequestHandler::getMeasurementConfig(OiRequestResponse &request){

    //set up request
    request.myRequestType = OiRequestResponse::eGetMeasurementConfig;
    this->prepareResponse(request);

    //check request
    QDomElement id = request.request.documentElement().firstChildElement("id");
    if(id.isNull()){
        this->sendErrorMessage(request, OiRequestResponse::eGetMeasurementConfig, OiRequestResponse::eWrongFormat);
        return;
    }

    //get and check feature by id
    QPointer<FeatureWrapper> feature = this->currentJob->getFeatureById(id.text().toInt());
    if(feature.isNull() || feature->getGeometry().isNull()){
        this->sendErrorMessage(request, OiRequestResponse::eGetMeasurementConfig, OiRequestResponse::eNoFeatureWithId);
        return;
    }

    //add id
    request.response.documentElement().appendChild(request.response.importNode(id, true));

    //add config
    MeasurementConfig mConfig = feature->getGeometry()->getMeasurementConfig();

    request.response.documentElement().appendChild(toXML(mConfig, false, request.response));

    emit this->sendResponse(request);

}

/*!
 * \brief OiRequestHandler::setMeasurementConfig
 * \param request
 */
void OiRequestHandler::setMeasurementConfig(OiRequestResponse &request){

    //set up request
    request.myRequestType = OiRequestResponse::eSetMeasurementConfig;
    this->prepareResponse(request);

    //check measurement config manager
    if(this->measurementConfigManager.isNull()){
        this->sendErrorMessage(request, OiRequestResponse::eSetMeasurementConfig, OiRequestResponse::eNoMeasurementConfigManager);
        return;
    }

    //check request
    QDomElement id = request.request.documentElement().firstChildElement("id");
    QDomElement measurementConfig = request.request.documentElement().firstChildElement("measurementConfig");
    QDomElement isSaved = request.request.documentElement().firstChildElement("isSaved");
    if(id.isNull() || measurementConfig.isNull() || isSaved.isNull()){
        this->sendErrorMessage(request, OiRequestResponse::eSetMeasurementConfig, OiRequestResponse::eWrongFormat);
        return;
    }

    //get and check feature by id
    QPointer<FeatureWrapper> feature = this->currentJob->getFeatureById(id.text().toInt());
    if(feature.isNull() || feature->getGeometry().isNull()){
        this->sendErrorMessage(request, OiRequestResponse::eSetMeasurementConfig, OiRequestResponse::eNoFeatureWithId);
        return;
    }

    //get and check measurement config
    MeasurementConfig mConfig;
    bool userConfig = (bool)isSaved.text().toInt();
    if(userConfig){
        mConfig = this->measurementConfigManager->getUserConfig(measurementConfig.text());
    }else{
        mConfig = this->measurementConfigManager->getProjectConfig(measurementConfig.text());
    }
    if(!mConfig.getIsValid()){
        this->sendErrorMessage(request, OiRequestResponse::eSetMeasurementConfig, OiRequestResponse::eNoMeasurementConfig);
        return;
    }

    //pass measurement config to feature
    feature->getGeometry()->setMeasurementConfig(mConfig);

    emit this->sendResponse(request);

}

/*!
 * \brief OiRequestHandler::getCoordinateSystems
 * \param request
 */
void OiRequestHandler::getCoordinateSystems(OiRequestResponse &request){

    //set up request
    request.myRequestType = OiRequestResponse::eGetCoordinateSystems;
    this->prepareResponse(request);

    //get all coordinate systems
    QList<QPointer<CoordinateSystem> > systems = this->currentJob->getCoordinateSystemsList();
    QList<QPointer<CoordinateSystem> > stations = this->currentJob->getStationSystemsList();

    //create and add coordinate systems
    QDomElement response = request.response.createElement("systems");
    foreach(const QPointer<CoordinateSystem> &system, systems){

        //check coordinate system
        if(system.isNull()){
            continue;
        }

        //create coordinate system element
        QDomElement systemTag = request.response.createElement("system");

        //add coordinate system information
        QDomElement id = request.response.createElement("id");
        QDomText idText = request.response.createTextNode(QString::number(system->getId()));
        id.appendChild(idText);
        systemTag.appendChild(id);
        QDomElement name = request.response.createElement("name");
        QDomText nameText = request.response.createTextNode(system->getFeatureName());
        name.appendChild(nameText);
        systemTag.appendChild(name);
        QDomElement group = request.response.createElement("group");
        QDomText groupText = request.response.createTextNode(system->getGroupName());
        group.appendChild(groupText);
        systemTag.appendChild(group);

        //add coordinate system
        response.appendChild(systemTag);

    }
    foreach(const QPointer<CoordinateSystem> &system, stations){

        //check coordinate system
        if(system.isNull()){
            continue;
        }

        //create coordinate system element
        QDomElement systemTag = request.response.createElement("system");

        //add coordinate system information
        QDomElement id = request.response.createElement("id");
        QDomText idText = request.response.createTextNode(QString::number(system->getId()));
        id.appendChild(idText);
        systemTag.appendChild(id);
        QDomElement name = request.response.createElement("name");
        QDomText nameText = request.response.createTextNode(system->getFeatureName());
        name.appendChild(nameText);
        systemTag.appendChild(name);
        QDomElement group = request.response.createElement("group");
        QDomText groupText = request.response.createTextNode(system->getGroupName());
        group.appendChild(groupText);
        systemTag.appendChild(group);

        //add coordinate system
        response.appendChild(systemTag);

    }
    request.response.documentElement().appendChild(response);

    emit this->sendResponse(request);

}

/*!
 * \brief OiRequestHandler::getRequestType
 * Converts the given id to the corresponding request type
 * \param id
 * \return
 */
OiRequestResponse::RequestType OiRequestHandler::getRequestType(int id) const{

    switch(id){
    case 0:
        return OiRequestResponse::eGetProject;
    case 1:
        return OiRequestResponse::eGetActiveFeature;
    case 2:
        return OiRequestResponse::eSetActiveFeature;
    case 3:
        return OiRequestResponse::eGetActiveStation;
    case 4:
        return OiRequestResponse::eSetActiveStation;
    case 5:
        return OiRequestResponse::eGetActiveCoordinateSystem;
    case 6:
        return OiRequestResponse::eSetActiveCoordinateSystem;
    case 7:
        return OiRequestResponse::eAim;
    case 8:
        return OiRequestResponse::eMeasure;
    case 9:
        return OiRequestResponse::eStartWatchwindow;
    case 10:
        return OiRequestResponse::eStopWatchwindow;
    case 11:
        return OiRequestResponse::eOiToolRequest;
    case 12:
        return OiRequestResponse::eGetFeatures;
    case 13:
        return OiRequestResponse::eAddFeatures;
    case 14:
        return OiRequestResponse::eGetObservations;
    case 15:
        return OiRequestResponse::eRemoveObservations;
    case 16:
        return OiRequestResponse::eGetParameters;
    case 17:
        return OiRequestResponse::eGetMeasurementConfigs;
    case 18:
        return OiRequestResponse::eGetMeasurementConfig;
    case 19:
        return OiRequestResponse::eSetMeasurementConfig;
    case 20:
        return OiRequestResponse::eGetCoordinateSystems;
    }

    return OiRequestResponse::eUnknownRequest;

}

/*!
 * \brief OiRequestHandler::getFeatureType
 * \param type
 * \return
 */
FeatureTypes OiRequestHandler::getFeatureType(int type) const{

    switch(type){
    case 0:
        return eCircleFeature;
    case 1:
        return eConeFeature;
    case 2:
        return eCylinderFeature;
    case 3:
        return eEllipseFeature;
    case 4:
        return eEllipsoidFeature;
    case 5:
        return eHyperboloidFeature;
    case 6:
        return eLineFeature;
    case 7:
        return eNurbsFeature;
    case 8:
        return eParaboloidFeature;
    case 9:
        return ePlaneFeature;
    case 10:
        return ePointFeature;
    case 11:
        return ePointCloudFeature;
    case 12:
        return eScalarEntityAngleFeature;
    case 13:
        return eScalarEntityDistanceFeature;
    case 14:
        return eScalarEntityMeasurementSeriesFeature;
    case 15:
        return eScalarEntityTemperatureFeature;
    case 16:
        return eSlottedHoleFeature;
    case 17:
        return eSphereFeature;
    case 18:
        return eTorusFeature;
    case 19:
        return eCoordinateSystemFeature;
    case 20:
        return eStationFeature;
    case 21:
        return eTrafoParamFeature;
    }

    return eUndefinedFeature;

}

/*!
 * \brief OiRequestHandler::addParameters
 * Adds parameter elements the the parameters tag depending on the type of feature
 * \param document
 * \param parameters
 * \param feature
 */
void OiRequestHandler::addParameters(QDomDocument &document, QDomElement &parameters, const QPointer<FeatureWrapper> &feature){

    QMap<QString, double> values;

    //get parameter/value pairs
    switch(feature->getFeatureTypeEnum()){
    case eCircleFeature:
        if(feature->getCircle().isNull()){
            return;
        }
        values.insert("x", feature->getCircle()->getPosition().getVector().getAt(0));
        values.insert("y", feature->getCircle()->getPosition().getVector().getAt(1));
        values.insert("z", feature->getCircle()->getPosition().getVector().getAt(2));
        values.insert("i", feature->getCircle()->getDirection().getVector().getAt(0));
        values.insert("j", feature->getCircle()->getDirection().getVector().getAt(1));
        values.insert("k", feature->getCircle()->getDirection().getVector().getAt(2));
        values.insert("radius", feature->getCircle()->getRadius().getRadius());
        break;
    case eConeFeature:
        if(feature->getCone().isNull()){
            return;
        }
        values.insert("x", feature->getCone()->getPosition().getVector().getAt(0));
        values.insert("y", feature->getCone()->getPosition().getVector().getAt(1));
        values.insert("z", feature->getCone()->getPosition().getVector().getAt(2));
        values.insert("i", feature->getCone()->getDirection().getVector().getAt(0));
        values.insert("j", feature->getCone()->getDirection().getVector().getAt(1));
        values.insert("k", feature->getCone()->getDirection().getVector().getAt(2));
        values.insert("aperture", feature->getCone()->getAperture());
        break;
    case eCylinderFeature:
        if(feature->getCylinder().isNull()){
            return;
        }
        values.insert("x", feature->getCylinder()->getPosition().getVector().getAt(0));
        values.insert("y", feature->getCylinder()->getPosition().getVector().getAt(1));
        values.insert("z", feature->getCylinder()->getPosition().getVector().getAt(2));
        values.insert("i", feature->getCylinder()->getDirection().getVector().getAt(0));
        values.insert("j", feature->getCylinder()->getDirection().getVector().getAt(1));
        values.insert("k", feature->getCylinder()->getDirection().getVector().getAt(2));
        values.insert("radius", feature->getCylinder()->getRadius().getRadius());
        break;
    case eEllipseFeature:
        if(feature->getEllipse().isNull()){
            return;
        }
        values.insert("x", feature->getEllipse()->getPosition().getVector().getAt(0));
        values.insert("y", feature->getEllipse()->getPosition().getVector().getAt(1));
        values.insert("z", feature->getEllipse()->getPosition().getVector().getAt(2));
        values.insert("i", feature->getEllipse()->getDirection().getVector().getAt(0));
        values.insert("j", feature->getEllipse()->getDirection().getVector().getAt(1));
        values.insert("k", feature->getEllipse()->getDirection().getVector().getAt(2));
        values.insert("a", feature->getEllipse()->getA());
        values.insert("b", feature->getEllipse()->getB());
        values.insert("i 2", feature->getEllipse()->getSemiMajorAxisDirection().getVector().getAt(0));
        values.insert("j 2", feature->getEllipse()->getSemiMajorAxisDirection().getVector().getAt(1));
        values.insert("k 2", feature->getEllipse()->getSemiMajorAxisDirection().getVector().getAt(2));
        break;
    case eEllipsoidFeature:
        if(feature->getEllipsoid().isNull()){
            return;
        }
        values.insert("x", feature->getEllipsoid()->getPosition().getVector().getAt(0));
        values.insert("y", feature->getEllipsoid()->getPosition().getVector().getAt(1));
        values.insert("z", feature->getEllipsoid()->getPosition().getVector().getAt(2));
        values.insert("i", feature->getEllipsoid()->getDirection().getVector().getAt(0));
        values.insert("j", feature->getEllipsoid()->getDirection().getVector().getAt(1));
        values.insert("k", feature->getEllipsoid()->getDirection().getVector().getAt(2));
        values.insert("a", feature->getEllipsoid()->getA());
        values.insert("b", feature->getEllipsoid()->getB());
        break;
    case eHyperboloidFeature:
        if(feature->getHyperboloid().isNull()){
            return;
        }
        values.insert("x", feature->getHyperboloid()->getPosition().getVector().getAt(0));
        values.insert("y", feature->getHyperboloid()->getPosition().getVector().getAt(1));
        values.insert("z", feature->getHyperboloid()->getPosition().getVector().getAt(2));
        values.insert("i", feature->getHyperboloid()->getDirection().getVector().getAt(0));
        values.insert("j", feature->getHyperboloid()->getDirection().getVector().getAt(1));
        values.insert("k", feature->getHyperboloid()->getDirection().getVector().getAt(2));
        values.insert("a", feature->getHyperboloid()->getA());
        values.insert("c", feature->getHyperboloid()->getC());
        break;
    case eLineFeature:
        if(feature->getLine().isNull()){
            return;
        }
        values.insert("x", feature->getLine()->getPosition().getVector().getAt(0));
        values.insert("y", feature->getLine()->getPosition().getVector().getAt(1));
        values.insert("z", feature->getLine()->getPosition().getVector().getAt(2));
        values.insert("i", feature->getLine()->getDirection().getVector().getAt(0));
        values.insert("j", feature->getLine()->getDirection().getVector().getAt(1));
        values.insert("k", feature->getLine()->getDirection().getVector().getAt(2));
        break;
    case eNurbsFeature:
        break;
    case eParaboloidFeature:
        if(feature->getParaboloid().isNull()){
            return;
        }
        values.insert("x", feature->getParaboloid()->getPosition().getVector().getAt(0));
        values.insert("y", feature->getParaboloid()->getPosition().getVector().getAt(1));
        values.insert("z", feature->getParaboloid()->getPosition().getVector().getAt(2));
        values.insert("i", feature->getParaboloid()->getDirection().getVector().getAt(0));
        values.insert("j", feature->getParaboloid()->getDirection().getVector().getAt(1));
        values.insert("k", feature->getParaboloid()->getDirection().getVector().getAt(2));
        values.insert("a", feature->getParaboloid()->getA());
        break;
    case ePlaneFeature:
        if(feature->getPlane().isNull()){
            return;
        }
        values.insert("x", feature->getPlane()->getPosition().getVector().getAt(0));
        values.insert("y", feature->getPlane()->getPosition().getVector().getAt(1));
        values.insert("z", feature->getPlane()->getPosition().getVector().getAt(2));
        values.insert("i", feature->getPlane()->getDirection().getVector().getAt(0));
        values.insert("j", feature->getPlane()->getDirection().getVector().getAt(1));
        values.insert("k", feature->getPlane()->getDirection().getVector().getAt(2));
        break;
    case ePointFeature:
        if(feature->getPoint().isNull()){
            return;
        }
        values.insert("x", feature->getPoint()->getPosition().getVector().getAt(0));
        values.insert("y", feature->getPoint()->getPosition().getVector().getAt(1));
        values.insert("z", feature->getPoint()->getPosition().getVector().getAt(2));
    case ePointCloudFeature:
        break;
    case eScalarEntityAngleFeature:
        if(feature->getScalarEntityAngle().isNull()){
            return;
        }
        values.insert("angle", feature->getScalarEntityAngle()->getAngle());
        break;
    case eScalarEntityDistanceFeature:
        if(feature->getScalarEntityDistance().isNull()){
            return;
        }
        values.insert("distance", feature->getScalarEntityDistance()->getDistance());
        break;
    case eScalarEntityMeasurementSeriesFeature:
        if(feature->getScalarEntityMeasurementSeries().isNull()){
            return;
        }
        values.insert("measurement series", feature->getScalarEntityMeasurementSeries()->getSeriesValue());
        break;
    case eScalarEntityTemperatureFeature:
        if(feature->getScalarEntityTemperature().isNull()){
            return;
        }
        values.insert("temperature", feature->getScalarEntityTemperature()->getTemperature());
        break;
    case eSlottedHoleFeature:
        if(feature->getSlottedHole().isNull()){
            return;
        }
        values.insert("x", feature->getSlottedHole()->getPosition().getVector().getAt(0));
        values.insert("y", feature->getSlottedHole()->getPosition().getVector().getAt(1));
        values.insert("z", feature->getSlottedHole()->getPosition().getVector().getAt(2));
        values.insert("i", feature->getSlottedHole()->getDirection().getVector().getAt(0));
        values.insert("j", feature->getSlottedHole()->getDirection().getVector().getAt(1));
        values.insert("k", feature->getSlottedHole()->getDirection().getVector().getAt(2));
        values.insert("radius", feature->getSlottedHole()->getRadius().getRadius());
        values.insert("length", feature->getSlottedHole()->getLength());
        values.insert("i", feature->getSlottedHole()->getHoleAxis().getVector().getAt(0));
        values.insert("j", feature->getSlottedHole()->getHoleAxis().getVector().getAt(1));
        values.insert("k", feature->getSlottedHole()->getHoleAxis().getVector().getAt(2));
        break;
    case eSphereFeature:
        if(feature->getSphere().isNull()){
            return;
        }
        values.insert("x", feature->getSphere()->getPosition().getVector().getAt(0));
        values.insert("y", feature->getSphere()->getPosition().getVector().getAt(1));
        values.insert("z", feature->getSphere()->getPosition().getVector().getAt(2));
        values.insert("radius", feature->getCircle()->getRadius().getRadius());
        break;
    case eTorusFeature:
        if(feature->getTorus().isNull()){
            return;
        }
        values.insert("x", feature->getTorus()->getPosition().getVector().getAt(0));
        values.insert("y", feature->getTorus()->getPosition().getVector().getAt(1));
        values.insert("z", feature->getTorus()->getPosition().getVector().getAt(2));
        values.insert("i", feature->getTorus()->getDirection().getVector().getAt(0));
        values.insert("j", feature->getTorus()->getDirection().getVector().getAt(1));
        values.insert("k", feature->getTorus()->getDirection().getVector().getAt(2));
        values.insert("radius", feature->getTorus()->getRadius().getRadius());
        values.insert("radius 2", feature->getTorus()->getSmallRadius().getRadius());
        break;
    case eCoordinateSystemFeature:
        if(feature->getCoordinateSystem().isNull()){
            return;
        }
        values.insert("x", feature->getCoordinateSystem()->getOrigin().getVector().getAt(0));
        values.insert("y", feature->getCoordinateSystem()->getOrigin().getVector().getAt(1));
        values.insert("z", feature->getCoordinateSystem()->getOrigin().getVector().getAt(2));
        values.insert("i", feature->getCoordinateSystem()->getXAxis().getVector().getAt(0));
        values.insert("j", feature->getCoordinateSystem()->getXAxis().getVector().getAt(1));
        values.insert("k", feature->getCoordinateSystem()->getXAxis().getVector().getAt(2));
        values.insert("i 2", feature->getCoordinateSystem()->getYAxis().getVector().getAt(0));
        values.insert("j 2", feature->getCoordinateSystem()->getYAxis().getVector().getAt(1));
        values.insert("k 2", feature->getCoordinateSystem()->getYAxis().getVector().getAt(2));
        values.insert("i 3", feature->getCoordinateSystem()->getZAxis().getVector().getAt(0));
        values.insert("j 3", feature->getCoordinateSystem()->getZAxis().getVector().getAt(1));
        values.insert("k 3", feature->getCoordinateSystem()->getZAxis().getVector().getAt(2));
        break;
    case eStationFeature:
        if(feature->getStation().isNull()){
            return;
        }
        values.insert("x", feature->getStation()->getPosition()->getPosition().getVector().getAt(0));
        values.insert("y", feature->getStation()->getPosition()->getPosition().getVector().getAt(1));
        values.insert("z", feature->getStation()->getPosition()->getPosition().getVector().getAt(2));
        break;
    case eTrafoParamFeature:
        if(feature->getTrafoParam().isNull()){
            return;
        }
        values.insert("tx", feature->getTrafoParam()->getTranslation().getAt(0));
        values.insert("ty", feature->getTrafoParam()->getTranslation().getAt(1));
        values.insert("tz", feature->getTrafoParam()->getTranslation().getAt(2));
        values.insert("rx", feature->getTrafoParam()->getRotation().getAt(0));
        values.insert("ry", feature->getTrafoParam()->getRotation().getAt(1));
        values.insert("rz", feature->getTrafoParam()->getRotation().getAt(2));
        values.insert("sx", feature->getTrafoParam()->getScale().getAt(0));
        values.insert("sy", feature->getTrafoParam()->getScale().getAt(1));
        values.insert("sz", feature->getTrafoParam()->getScale().getAt(2));
        break;
    }

    //add the parameters
    QStringList keys = values.keys();
    foreach(const QString &key, keys){
        QDomElement parameter = document.createElement("parameter");
        parameter.setAttribute("name", key);
        parameter.setAttribute("value", QString::number(values.value(key), 'f', 7));
        parameters.appendChild(parameter);
    }

}

/*!
 * \brief OiRequestHandler::sendErrorMessage
 * \param request
 * \param type
 * \param error
 */
void OiRequestHandler::sendErrorMessage(OiRequestResponse request, OiRequestResponse::RequestType type, OiRequestResponse::ErrorCode error){

    //set up error message
    request.myRequestType = type;
    this->prepareResponse(request);
    request.response.documentElement().setAttribute("errorCode", error);

    emit this->sendResponse(request);

}

/*!
 * \brief OiRequestHandler::prepareResponse
 * Creates a QDomElement for response and sets its attributes
 * \param request
 */
void OiRequestHandler::prepareResponse(OiRequestResponse &request) const{
    request.response.appendChild(request.response.createElement("OiResponse"));
    request.response.documentElement().setAttribute("ref", request.myRequestType);
    request.response.documentElement().setAttribute("errorCode", OiRequestResponse::eNoError);
}

/*!
 * \brief OiRequestHandler::buildWatchWindowMessage
 * \param wwTag: the XML tag that shall be filled
 * \param streamData: the sensor stream data
 * \return
 */
bool OiRequestHandler::buildWatchWindowMessage(QDomElement &wwTag, const QVariantMap &streamData){

    //check current job
    if(this->currentJob.isNull()){
        return false;
    }

    //get and check active feature
    QPointer<FeatureWrapper> activeFeature = this->currentJob->getActiveFeature();
    if(activeFeature.isNull() || activeFeature->getGeometry().isNull() ||
            !activeFeature->getGeometry()->getIsSolved()){
        return false;
    }

    //get and check active coordinate system
    QPointer<CoordinateSystem> activeSystem = this->currentJob->getActiveCoordinateSystem();
    if(activeSystem.isNull()){
        return false;
    }

    //get and check station
    QPointer<Station> activeStation = this->currentJob->getActiveStation();
    if(activeStation.isNull() || activeStation->getCoordinateSystem().isNull()){
        return false;
    }

    //get xyz of active geometry
    OiVec xyz = activeFeature->getGeometry()->getPosition().getVectorH();

    //get transformation matrix (station -> active system)
    TrafoController trafoControl;
    OiMat tMat(4, 4);
    if(!trafoControl.getTransformationMatrix(tMat, activeStation->getCoordinateSystem(), activeSystem)){
        return false;
    }

    //get and transform sensor stream data
    switch(this->watchWindowTask.type){
    case eCartesianReading:{

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

        //transform sensor xyz
        streamXyz = tMat * streamXyz;

        //build difference sensor - feature
        OiVec diff = streamXyz - xyz;

        //fill watch window tag
        wwTag.setTagName("cartesian");
        wwTag.setAttribute("x", QString::number(diff.getAt(0)));
        wwTag.setAttribute("y", QString::number(diff.getAt(1)));
        wwTag.setAttribute("z", QString::number(diff.getAt(2)));

        return true;

    }case ePolarReading:{

        //check if azimuth, zenith, distance are available
        if(!streamData.contains("azimuth") || !streamData.contains("zenith") || !streamData.contains("distance")){
            return false;
        }

        //get sensor xyz
        double azimuth, zenith, distance;
        azimuth = streamData.value("azimuth").toDouble();
        zenith = streamData.value("zenith").toDouble();
        distance = streamData.value("distance").toDouble();
        OiVec streamXyz(3);
        streamXyz = Reading::toCartesian(azimuth, zenith, distance);
        streamXyz.add(1.0);

        //transform sensor xyz
        streamXyz = tMat * streamXyz;

        //transform sensor cartesian to polar
        double x, y, z;
        x = streamXyz.getAt(0);
        y = streamXyz.getAt(1);
        z = streamXyz.getAt(2);
        OiVec streamPolar(3);
        streamPolar = Reading::toPolar(x, y, z);

        //transform feature cartesian to polar
        x = xyz.getAt(0);
        y = xyz.getAt(1);
        z = xyz.getAt(2);
        OiVec featurePolar(3);
        featurePolar = Reading::toPolar(x, y, z);

        //build difference sensor - feature
        OiVec diff = streamPolar - featurePolar;

        //fill watch window tag
        wwTag.setTagName("polar");
        wwTag.setAttribute("azimuth", QString::number(diff.getAt(0)));
        wwTag.setAttribute("zenith", QString::number(diff.getAt(1)));
        wwTag.setAttribute("distance", QString::number(diff.getAt(2)));

        return true;

    }default:
        return false;
    }

    return false;

}
