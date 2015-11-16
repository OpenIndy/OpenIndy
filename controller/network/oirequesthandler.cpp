#include "oirequesthandler.h"

//QPointer<OiRequestHandler> OiRequestHandler::myRequestHandler(NULL);

OiRequestHandler::OiRequestHandler(QObject *parent){

    //initially no watch window or measurement task in process
    //this->watchWindowTask.taskInProcess = false;
    //this->measurementTask.taskInProcess = false;

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

    qDebug() << Q_FUNC_INFO << QThread::currentThreadId();

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
    default:
        this->sendErrorMessage(request, OiRequestResponse::eGetProject, OiRequestResponse::eUnknownRequestType);
        return false;
    }

}

void OiRequestHandler::sensorActionStarted(const QString &name)
{

}

void OiRequestHandler::sensorActionFinished(const bool &success, const QString &msg)
{

}

void OiRequestHandler::log(const QString &msg, const MessageTypes &msgType)
{

}

void OiRequestHandler::realTimeReading(const QVariantMap &reading)
{

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
void OiRequestHandler::stopWatchwindow(OiRequestResponse &request){
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
    MeasurementConfig mConfig = this->measurementConfigManager->getSavedMeasurementConfig(attr.mConfig);
    if(!mConfig.getIsValid()){
        mConfig = this->measurementConfigManager->getProjectMeasurementConfig(attr.mConfig);
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

    //add parameters
    QDomElement parameters = request.response.createElement("parameters");
    this->addParameters(request.response, parameters, feature);
    request.response.appendChild(parameters);

    emit this->sendResponse(request);

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
    QList<MeasurementConfig> savedConfigs = this->measurementConfigManager->getSavedMeasurementConfigs();
    QList<MeasurementConfig> projectConfigs = this->measurementConfigManager->getProjectMeasurementConfigs();

    //add configs
    QDomElement configs = request.response.createElement("measurementConfigs");
    foreach(const MeasurementConfig &mConfig, savedConfigs){
        QDomElement config = request.response.createElement("measurementConfig");
        QDomElement name = request.response.createElement("name");
        QDomText nameText = request.response.createTextNode(mConfig.getName());
        name.appendChild(nameText);
        config.appendChild(name);
        QDomElement isSaved = request.response.createElement("isSaved");
        QDomText isSavedText = request.response.createTextNode("1");
        isSaved.appendChild(isSavedText);
        config.appendChild(isSaved);
        QDomElement count = request.response.createElement("count");
        QDomText countText = request.response.createTextNode(QString::number(mConfig.getCount()));
        count.appendChild(countText);
        config.appendChild(count);
        QDomElement iterations = request.response.createElement("iterations");
        QDomText iterationsText = request.response.createTextNode(QString::number(mConfig.getIterations()));
        iterations.appendChild(iterationsText);
        config.appendChild(iterations);
        QDomElement measureTwoSides = request.response.createElement("measureTwoSides");
        QDomText measureTwoSidesText = request.response.createTextNode(mConfig.getMeasureTwoSides()?"1":"0");
        measureTwoSides.appendChild(measureTwoSidesText);
        config.appendChild(measureTwoSides);
        QDomElement timeDependent = request.response.createElement("timeDependent");
        QDomText timeDependentText = request.response.createTextNode(mConfig.getTimeDependent()?"1":"0");
        timeDependent.appendChild(timeDependentText);
        config.appendChild(timeDependent);
        QDomElement distanceDependent = request.response.createElement("distanceDependent");
        QDomText distanceDependentText = request.response.createTextNode(mConfig.getDistanceDependent()?"1":"0");
        distanceDependent.appendChild(distanceDependentText);
        config.appendChild(distanceDependent);
        QDomElement timeInterval = request.response.createElement("timeInterval");
        QDomText timeIntervalText = request.response.createTextNode(QString::number(mConfig.getTimeInterval()));
        timeInterval.appendChild(timeIntervalText);
        config.appendChild(timeInterval);
        QDomElement distanceInterval = request.response.createElement("distanceInterval");
        QDomText distanceIntervalText = request.response.createTextNode(QString::number(mConfig.getDistanceInterval()));
        distanceInterval.appendChild(distanceIntervalText);
        config.appendChild(distanceInterval);
        QDomElement typeOfReading = request.response.createElement("typeOfReading");
        QDomText typeOfReadingText = request.response.createTextNode(QString::number(mConfig.getTypeOfReading()));
        typeOfReading.appendChild(typeOfReadingText);
        config.appendChild(typeOfReading);
        configs.appendChild(config);
    }
    foreach(const MeasurementConfig &mConfig, projectConfigs){
        QDomElement config = request.response.createElement("measurementConfig");
        QDomElement name = request.response.createElement("name");
        QDomText nameText = request.response.createTextNode(mConfig.getName());
        name.appendChild(nameText);
        config.appendChild(name);
        QDomElement isSaved = request.response.createElement("isSaved");
        QDomText isSavedText = request.response.createTextNode("0");
        isSaved.appendChild(isSavedText);
        config.appendChild(isSaved);
        QDomElement count = request.response.createElement("count");
        QDomText countText = request.response.createTextNode(QString::number(mConfig.getCount()));
        count.appendChild(countText);
        config.appendChild(count);
        QDomElement iterations = request.response.createElement("iterations");
        QDomText iterationsText = request.response.createTextNode(QString::number(mConfig.getIterations()));
        iterations.appendChild(iterationsText);
        config.appendChild(iterations);
        QDomElement measureTwoSides = request.response.createElement("measureTwoSides");
        QDomText measureTwoSidesText = request.response.createTextNode(mConfig.getMeasureTwoSides()?"1":"0");
        measureTwoSides.appendChild(measureTwoSidesText);
        config.appendChild(measureTwoSides);
        QDomElement timeDependent = request.response.createElement("timeDependent");
        QDomText timeDependentText = request.response.createTextNode(mConfig.getTimeDependent()?"1":"0");
        timeDependent.appendChild(timeDependentText);
        config.appendChild(timeDependent);
        QDomElement distanceDependent = request.response.createElement("distanceDependent");
        QDomText distanceDependentText = request.response.createTextNode(mConfig.getDistanceDependent()?"1":"0");
        distanceDependent.appendChild(distanceDependentText);
        config.appendChild(distanceDependent);
        QDomElement timeInterval = request.response.createElement("timeInterval");
        QDomText timeIntervalText = request.response.createTextNode(QString::number(mConfig.getTimeInterval()));
        timeInterval.appendChild(timeIntervalText);
        config.appendChild(timeInterval);
        QDomElement distanceInterval = request.response.createElement("distanceInterval");
        QDomText distanceIntervalText = request.response.createTextNode(QString::number(mConfig.getDistanceInterval()));
        distanceInterval.appendChild(distanceIntervalText);
        config.appendChild(distanceInterval);
        QDomElement typeOfReading = request.response.createElement("typeOfReading");
        QDomText typeOfReadingText = request.response.createTextNode(QString::number(mConfig.getTypeOfReading()));
        typeOfReading.appendChild(typeOfReadingText);
        config.appendChild(typeOfReading);
        configs.appendChild(config);
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
    QDomElement config = request.response.createElement("measurementConfig");
    QDomElement name = request.response.createElement("name");
    QDomText nameText = request.response.createTextNode(mConfig.getName());
    name.appendChild(nameText);
    config.appendChild(name);
    QDomElement isSaved = request.response.createElement("isSaved");
    QDomText isSavedText = request.response.createTextNode("0");
    isSaved.appendChild(isSavedText);
    config.appendChild(isSaved);
    QDomElement count = request.response.createElement("count");
    QDomText countText = request.response.createTextNode(QString::number(mConfig.getCount()));
    count.appendChild(countText);
    config.appendChild(count);
    QDomElement iterations = request.response.createElement("iterations");
    QDomText iterationsText = request.response.createTextNode(QString::number(mConfig.getIterations()));
    iterations.appendChild(iterationsText);
    config.appendChild(iterations);
    QDomElement measureTwoSides = request.response.createElement("measureTwoSides");
    QDomText measureTwoSidesText = request.response.createTextNode(mConfig.getMeasureTwoSides()?"1":"0");
    measureTwoSides.appendChild(measureTwoSidesText);
    config.appendChild(measureTwoSides);
    QDomElement timeDependent = request.response.createElement("timeDependent");
    QDomText timeDependentText = request.response.createTextNode(mConfig.getTimeDependent()?"1":"0");
    timeDependent.appendChild(timeDependentText);
    config.appendChild(timeDependent);
    QDomElement distanceDependent = request.response.createElement("distanceDependent");
    QDomText distanceDependentText = request.response.createTextNode(mConfig.getDistanceDependent()?"1":"0");
    distanceDependent.appendChild(distanceDependentText);
    config.appendChild(distanceDependent);
    QDomElement timeInterval = request.response.createElement("timeInterval");
    QDomText timeIntervalText = request.response.createTextNode(QString::number(mConfig.getTimeInterval()));
    timeInterval.appendChild(timeIntervalText);
    config.appendChild(timeInterval);
    QDomElement distanceInterval = request.response.createElement("distanceInterval");
    QDomText distanceIntervalText = request.response.createTextNode(QString::number(mConfig.getDistanceInterval()));
    distanceInterval.appendChild(distanceIntervalText);
    config.appendChild(distanceInterval);
    QDomElement typeOfReading = request.response.createElement("typeOfReading");
    QDomText typeOfReadingText = request.response.createTextNode(QString::number(mConfig.getTypeOfReading()));
    typeOfReading.appendChild(typeOfReadingText);
    config.appendChild(typeOfReading);
    request.response.documentElement().appendChild(config);

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
    bool savedConfig = (bool)isSaved.text().toInt();
    if(savedConfig){
        mConfig = this->measurementConfigManager->getSavedMeasurementConfig(measurementConfig.text());
    }else{
        mConfig = this->measurementConfigManager->getProjectMeasurementConfig(measurementConfig.text());
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
 * \param readingType: the requested reading type
 * \param streamData: the sensor stream data
 * \return
 */
/*bool OiRequestHandler::buildWatchWindowMessage(QDomElement &wwTag, const int &readingType, const QVariantMap &streamData){

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

    return false;
}*/

/*!
 * \brief OiRequestHandler::receiveWatchWindowData
 * Each time a map with current coordinates is emitted by SensorListener
 * \param data
 */
/*void OiRequestHandler::receiveWatchWindowData(const QVariantMap &data){

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

}*/

/*!
 * \brief OiRequestHandler::measurementFinished
 * \param geomId
 * \param success
 */
/*void OiRequestHandler::measurementFinished(const bool &success){

    //check if a client-requested task is in progress
    if(!this->measurementTask.taskInProcess){
        return;
    }

    this->measurementTask.taskInProcess = false;

    this->measurementTask.request.response.documentElement().setAttribute("errorCode", success ? 0 : OiRequestResponse::eMeasurementError);

    emit this->sendResponse(this->measurementTask.request);

}*/
