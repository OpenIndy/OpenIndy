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
 * \brief OiRequestHandler::receiveOiToolResponse
 * \param response
 */
/*void OiRequestHandler::receiveOiToolResponse(OiRequestResponse response){

    response.myRequestType = OiRequestResponse::eOiToolRequest;

    emit this->sendResponse(response);

}*/

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

    //check if active feature is valid
    if(this->currentJob->getActiveFeature().isNull() || this->currentJob->getActiveFeature()->getGeometry().isNull()){
        request.response.documentElement().setAttribute("errorCode", OiRequestResponse::eNoActiveFeature);
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

    //check if active feature is valid
    if(this->currentJob->getActiveFeature().isNull() || this->currentJob->getActiveFeature()->getGeometry().isNull()){
        request.response.documentElement().setAttribute("errorCode", OiRequestResponse::eNoActiveFeature);
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

    //start measurement
    emit this->startMeasurement();

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
            || !getAvailableFeatureTypes().contains(type.text().toInt())){
        this->sendErrorMessage(request, OiRequestResponse::eAddFeatures, OiRequestResponse::eWrongFormat);
        return;
    }

    //set up feature attributes
    FeatureAttributes attr;
    attr.typeOfFeature = type.text().toInt();
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
    this->currentJob->addFeatures(attr);

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
        QDomText xText = request.response.createTextNode(QString::number(obs->getXYZ().getAt(0)));
        x.appendChild(xText);
        observation.appendChild(x);
        QDomElement y = request.response.createElement("y");
        QDomText yText = request.response.createTextNode(QString::number(obs->getXYZ().getAt(1)));
        y.appendChild(yText);
        observation.appendChild(y);
        QDomElement z = request.response.createElement("z");
        QDomText zText = request.response.createTextNode(QString::number(obs->getXYZ().getAt(2)));
        z.appendChild(zText);
        observation.appendChild(z);
        QDomElement vx = request.response.createElement("vx");
        QDomText vxText = request.response.createTextNode(QString::number(0.0));
        vx.appendChild(vxText);
        observation.appendChild(isNominal);
        QDomElement vy = request.response.createElement("vy");
        QDomText vyText = request.response.createTextNode(QString::number(0.0));
        vy.appendChild(vyText);
        observation.appendChild(vy);
        QDomElement vz = request.response.createElement("vz");
        QDomText vzText = request.response.createTextNode(QString::number(0.0));
        vz.appendChild(vzText);
        observation.appendChild(vz);
        QDomElement v = request.response.createElement("v");
        QDomText vText = request.response.createTextNode(QString::number(0.0));
        v.appendChild(vText);
        observation.appendChild(v);
        QDomElement isUsed = request.response.createElement("isUsed");
        QDomText isUsedText = request.response.createTextNode(true);
        isUsed.appendChild(isUsedText);
        observation.appendChild(isUsed);
        QDomElement isValid = request.response.createElement("isValid");
        QDomText isValidText = request.response.createTextNode(obs->getIsValid()?"1":"0");
        isValid.appendChild(isValidText);
        observation.appendChild(isValid);

        /*
         * case eObservationDisplayVX:
            if(geometry->getFunctions().size() > 0 && !geometry->getFunctions().at(0).isNull()){
                const Statistic &statistic = geometry->getFunctions().at(0)->getStatistic();
                Residual residual = statistic.getDisplayResidual(observation->getId());
                QString attr = getObservationDisplayAttributesName(eObservationDisplayVX);
                if(residual.elementId == observation->getId() && residual.corrections.contains(attr)){
                    return QString::number(convertFromDefault(residual.corrections[attr],
                                                              this->parameterDisplayConfig.getDisplayUnit(eMetric)),
                                           'f', this->parameterDisplayConfig.getDisplayDigits(eMetric));
                }
            }
            break;
        case eObservationDisplayVY:
            if(geometry->getFunctions().size() > 0 && !geometry->getFunctions().at(0).isNull()){
                const Statistic &statistic = geometry->getFunctions().at(0)->getStatistic();
                Residual residual = statistic.getDisplayResidual(observation->getId());
                QString attr = getObservationDisplayAttributesName(eObservationDisplayVY);
                if(residual.elementId == observation->getId() && residual.corrections.contains(attr)){
                    return QString::number(convertFromDefault(residual.corrections[attr],
                                                              this->parameterDisplayConfig.getDisplayUnit(eMetric)),
                                           'f', this->parameterDisplayConfig.getDisplayDigits(eMetric));
                }
            }
            break;
        case eObservationDisplayVZ:
            if(geometry->getFunctions().size() > 0 && !geometry->getFunctions().at(0).isNull()){
                const Statistic &statistic = geometry->getFunctions().at(0)->getStatistic();
                Residual residual = statistic.getDisplayResidual(observation->getId());
                QString attr = getObservationDisplayAttributesName(eObservationDisplayVZ);
                if(residual.elementId == observation->getId() && residual.corrections.contains(attr)){
                    return QString::number(convertFromDefault(residual.corrections[attr],
                                                              this->parameterDisplayConfig.getDisplayUnit(eMetric)),
                                           'f', this->parameterDisplayConfig.getDisplayDigits(eMetric));
                }
            }
            break;
        case eObservationDisplayV:
            if(geometry->getFunctions().size() > 0 && !geometry->getFunctions().at(0).isNull()){
                const Statistic &statistic = geometry->getFunctions().at(0)->getStatistic();
                Residual residual = statistic.getDisplayResidual(observation->getId());
                QString attr = getObservationDisplayAttributesName(eObservationDisplayV);
                if(residual.elementId == observation->getId() && residual.corrections.contains(attr)){
                    return QString::number(convertFromDefault(residual.corrections[attr],
                                                              this->parameterDisplayConfig.getDisplayUnit(eMetric)),
                                           'f', this->parameterDisplayConfig.getDisplayDigits(eMetric));
                }
            }
            break;
        }
         * */

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

}

/*!
 * \brief OiRequestHandler::getParameters
 * \param request
 */
void OiRequestHandler::getParameters(OiRequestResponse &request){

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
    if(feature.isNull() || feature->getFeature().isNull()){
        this->sendErrorMessage(request, OiRequestResponse::eGetObservations, OiRequestResponse::eNoFeatureWithId);
        return;
    }

    //add id
    request.response.documentElement().appendChild(request.response.importNode(id, true));

    emit this->sendResponse(request);

}

/*!
 * \brief OiRequestHandler::getMeasurementConfigs
 * \param request
 */
void OiRequestHandler::getMeasurementConfigs(OiRequestResponse &request){

}

/*!
 * \brief OiRequestHandler::getMeasurementConfig
 * \param request
 */
void OiRequestHandler::getMeasurementConfig(OiRequestResponse &request){

}

/*!
 * \brief OiRequestHandler::setMeasurementConfig
 * \param request
 */
void OiRequestHandler::setMeasurementConfig(OiRequestResponse &request){

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
