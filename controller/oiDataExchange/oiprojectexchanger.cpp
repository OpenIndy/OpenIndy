#include "oiprojectexchanger.h"

#include <QFile>

QMap<int, Observation *> OiProjectExchanger::myObservations;
QMap<int, Reading *> OiProjectExchanger::myReadings;
QMap<int, FeatureWrapper *> OiProjectExchanger::myStations;
QMap<int, FeatureWrapper *> OiProjectExchanger::myCoordinateSystems;
QMap<int, FeatureWrapper *> OiProjectExchanger::myTransformationParameters;
QMap<int, FeatureWrapper *> OiProjectExchanger::myGeometries;
QList<int> OiProjectExchanger::stationPoints;
QMap<QString, MeasurementConfig> OiProjectExchanger::myMConfigs;
QMap<QString, SensorConfiguration> OiProjectExchanger::mySConfigs;

/*!
 * \brief OiProjectExchanger::saveProject
 * \return
 */
QDomDocument OiProjectExchanger::saveProject(){

    QDomDocument project;

    if(!OiProjectData::getIsValid()){
        Console::addLine("No project available");
        return project;
    }

    //get current date and time
    QString dateTime = QDateTime::currentDateTime().toString(Qt::ISODate);

    //create XML root and set general project attributes
    QDomElement root = project.createElement("oiProjectData");
    root.setAttribute("name", OiProjectData::getProjectName());
    root.setAttribute("date", dateTime);
    root.setAttribute("idCount", QString::number(Configuration::idCount));
    project.appendChild(root);

    //add active station and active coordinate system
    QDomElement activeSystem = project.createElement("activeCoordinateSystem");
    QDomElement activeStation = project.createElement("activeStation");
    int activeSystemId, activeStationId = -1;
    if(OiFeatureState::getActiveCoordinateSystem() != NULL){
        activeSystemId = OiFeatureState::getActiveCoordinateSystem()->getId();
    }
    if(OiFeatureState::getActiveStation() != NULL){
        activeStationId = OiFeatureState::getActiveStation()->getId();
    }
    activeSystem.setAttribute("ref", activeSystemId);
    activeStation.setAttribute("ref", activeStationId);
    root.appendChild(activeSystem);
    root.appendChild(activeStation);

    //add stations
    QDomElement stations = project.createElement("stations");
    foreach(Station *s, OiFeatureState::getStations()){
        if(s != NULL){
            QDomElement station = s->toOpenIndyXML(project);
            if(!station.isNull()){
                stations.appendChild(station);
            }
        }
    }
    root.appendChild(stations);

    //add coordinate systems (nominal + station systems)
    QDomElement coordinateSystems = project.createElement("coordinateSystems");
    foreach(CoordinateSystem *c, OiFeatureState::getCoordinateSystems()){
        if(c != NULL){
            QDomElement coordinateSystem = c->toOpenIndyXML(project);
            if(!coordinateSystem.isNull()){
                coordinateSystems.appendChild(coordinateSystem);
            }
        }
    }
    foreach(Station *s, OiFeatureState::getStations()){
        if(s != NULL && s->coordSys != NULL){
            QDomElement coordinateSystem = s->coordSys->toOpenIndyXML(project);
            if(!coordinateSystem.isNull()){
                coordinateSystems.appendChild(coordinateSystem);
            }
        }
    }
    root.appendChild(coordinateSystems);

    //add trafo params
    QDomElement trafoParams = project.createElement("transformationParameters");
    foreach(TrafoParam *tp, OiFeatureState::getTransformationParameters()){
        if(tp != NULL){
            QDomElement trafoParam = tp->toOpenIndyXML(project);
            if(!trafoParam.isNull()){
                trafoParams.appendChild(trafoParam);
            }
        }
    }
    root.appendChild(trafoParams);

    //add geometries
    QDomElement geometries = project.createElement("geometries");
    foreach(FeatureWrapper *geom, OiFeatureState::getGeometries()){ //normal geometries
        if(geom != NULL){
            QDomElement geometry = geom->getGeometry()->toOpenIndyXML(project);
            if(!geometry.isNull()){
                geometries.appendChild(geometry);
            }
        }
    }
    foreach(Station *s, OiFeatureState::getStations()){ //station points
        if(s != NULL && s->position != NULL){
            QDomElement geometry = s->position->toOpenIndyXML(project);
            if(!geometry.isNull()){
                geometries.appendChild(geometry);
            }
        }
    }
    root.appendChild(geometries);

    //add observations (search the station coordinate systems for observations)
    QDomElement observations = project.createElement("observations");
    foreach(Station *s, OiFeatureState::getStations()){
        if(s != NULL && s->coordSys != NULL && s->coordSys->getObservations().size() > 0){
            foreach(Observation *obs, s->coordSys->getObservations()){
                if(obs != NULL){
                    QDomElement observation = obs->toOpenIndyXML(project);
                    if(!observation.isNull()){
                        observations.appendChild(observation);
                    }
                }
            }
        }
    }
    root.appendChild(observations);

    //add configs

    //TODO only save necessary mConfigs

    QDomElement configs = project.createElement("configs");
    QDomElement measurementConfigs = project.createElement("measurementConfigs");
    QDomElement sensorConfigs = project.createElement("sensorConfigs");

    //add measurement configs
    QList<MeasurementConfig> mConfigs = OiConfigState::getAllMeasurementConfigs();
    foreach(const MeasurementConfig &mConfig, mConfigs){
        QDomElement config = mConfig.toOpenIndyXML(project);
        if(!config.isNull()){
            measurementConfigs.appendChild(config);
        }
    }
    configs.appendChild(measurementConfigs);

    //add sensor configs
    QList<SensorConfiguration> sConfigs = OiConfigState::getAllSensorConfigs();
    foreach(const SensorConfiguration &sConfig, sConfigs){
        QDomElement config = sConfig.toOpenIndyXML(project);
        if(!config.isNull()){
            sensorConfigs.appendChild(config);
        }
    }
    configs.appendChild(sensorConfigs);

    root.appendChild(configs);

    return project;

}

/*!
 * \brief OiProjectExchanger::loadProject
 * \param project
 * \return
 */
bool OiProjectExchanger::loadProject(const QDomDocument &project){

    //delete old features (from a previous project)
    OiFeatureState::resetFeatureLists();

    //load all elements from xml into helper lists
    if(!OiProjectExchanger::loadObservations(project)
            || !OiProjectExchanger::loadStations(project)
            || !OiProjectExchanger::loadCoordinateSystems(project)
            || !OiProjectExchanger::loadTransformationParameters(project)
            || !OiProjectExchanger::loadGeometries(project)
            || !OiProjectExchanger::loadConfigs(project)){

        //clear all created elements if an error occured in one of the loading helpers (e.g. no station available)
        OiProjectExchanger::clearHelperMaps(true);
        return false;

    }

    //restore dependencies between the elements
    if(!OiProjectExchanger::restoreObservationDependencies(project)
            || !OiProjectExchanger::restoreStationDependencies(project)
            || !OiProjectExchanger::restoreCoordinateSystemDependencies(project)
            || !OiProjectExchanger::restoreTrafoParamDependencies(project)
            || !OiProjectExchanger::restoreGeometryDependencies(project)){

        //clear all created elements if an error occured in one of the loading helpers (e.g. no station available)
        OiProjectExchanger::clearHelperMaps(true);
        return false;

    }

    //add loaded features to OiFeatureState
    foreach(FeatureWrapper *station, OiProjectExchanger::myStations){
        OiFeatureState::addFeature(station);
        if(station->getStation()->position != NULL){
            OiProjectExchanger::stationPoints.append(station->getStation()->position->getId());
        }
    }
    foreach(FeatureWrapper *system, OiProjectExchanger::myCoordinateSystems){
        if(!system->getCoordinateSystem()->isStationSystem){
            OiFeatureState::addFeature(system);
        }
    }
    foreach(FeatureWrapper *trafoParam, OiProjectExchanger::myTransformationParameters){
        OiFeatureState::addFeature(trafoParam);
    }
    foreach(FeatureWrapper *geometry, OiProjectExchanger::myGeometries){
        if(!OiProjectExchanger::stationPoints.contains(geometry->getGeometry()->getId())){
            OiFeatureState::addFeature(geometry);
        }
    }

    //add configs to OiConfigState
    foreach(const MeasurementConfig &mConfig, OiProjectExchanger::myMConfigs){
        OiConfigState::addProjectMeasurementConfig(mConfig);
    }
    foreach(const SensorConfiguration &sConfig, OiProjectExchanger::mySConfigs){
        OiConfigState::addProjectSensorConfig(sConfig);
    }

    //set active station and active coordinate system
    QDomElement activeStation = project.documentElement().firstChildElement("activeStation");
    QDomElement activeCoordinateSystem = project.documentElement().firstChildElement("activeCoordinateSystem");
    if(activeStation.isNull() || activeCoordinateSystem.isNull()
            || !activeStation.hasAttribute("ref") || !activeCoordinateSystem.hasAttribute("ref")
            || !OiProjectExchanger::myStations.contains(activeStation.attribute("ref").toInt())
            || !OiProjectExchanger::myCoordinateSystems.contains(activeCoordinateSystem.attribute("ref").toInt())){

        //clear all created elements if no active station or active coordinate system is available
        //OiProjectExchanger::clearHelperMaps(true);
        //return false;

    }
    FeatureWrapper *myActiveStation = OiProjectExchanger::myStations.value(activeStation.attribute("ref").toInt());
    FeatureWrapper *myActiveSystem = OiProjectExchanger::myCoordinateSystems.value(activeCoordinateSystem.attribute("ref").toInt());
    myActiveStation->getStation()->setActiveStationState(true);
    myActiveSystem->getCoordinateSystem()->setActiveCoordinateSystemState(true);

    //clear the helper maps
    OiProjectExchanger::clearHelperMaps(false);

    //set configuration max id to a high unique value
    Configuration::idCount = project.documentElement().attribute("idCount").toInt() + 1;

    return true;

}

/*!
 * \brief OiProjectExchanger::loadObservations
 * \param project
 * \return
 */
bool OiProjectExchanger::loadObservations(const QDomDocument &project){

    QDomElement observations = project.documentElement().firstChildElement("observations");
    if(!observations.isNull()){

        //get a list of all observations tags
        QDomNodeList observationList = observations.childNodes();

        for(int i = 0; i < observationList.size(); i++){

            //for each observations tag create an observation object and try to parse xml
            Observation *obs = new Observation();
            QDomElement obsTag = observationList.at(i).toElement();
            if(!obs->fromOpenIndyXML(obsTag)){
                delete obs;
                continue;
            }

            //add the observation to the list of successfully loaded observations
            OiProjectExchanger::myObservations.insert(obs->getId(), obs);
            OiProjectExchanger::myReadings.insert(obs->myReading->id, obs->myReading);

        }

    }

    return true;

}

/*!
 * \brief OiProjectExchanger::loadStations
 * \param project
 * \return
 */
bool OiProjectExchanger::loadStations(const QDomDocument &project){

    QDomElement stations = project.documentElement().firstChildElement("stations");

    //at least one station has to be available
    if(stations.isNull()){
        return false;
    }

    //get a list of all station tags
    QDomNodeList stationList = stations.childNodes();

    for(int i = 0; i < stationList.size(); i++){

        //for each station tag create a station object and try to parse xml
        Station *station = new Station();
        QDomElement stationTag = stationList.at(i).toElement();
        if(!station->fromOpenIndyXML(stationTag)){
            delete station;
            continue;
        }

        //create a feature wrapper for the station
        FeatureWrapper *stationWrapper = new FeatureWrapper();
        stationWrapper->setStation(station);

        //add the station to the list of successfully loaded stations
        OiProjectExchanger::myStations.insert(station->getId(), stationWrapper);

    }

    //at least one station has to be available
    if(OiProjectExchanger::myStations.size() == 0){
        return false;
    }

    return true;

}

/*!
 * \brief OiProjectExchanger::loadCoordinateSystems
 * \param project
 * \return
 */
bool OiProjectExchanger::loadCoordinateSystems(const QDomDocument &project){

    QDomElement coordinateSystems = project.documentElement().firstChildElement("coordinateSystems");

    //at least one coordinate system has to be available
    if(coordinateSystems.isNull()){
        return false;
    }

    //get a list of all coordinate system tags
    QDomNodeList coordinateSystemList = coordinateSystems.childNodes();

    for(int i = 0; i < coordinateSystemList.size(); i++){

        //for each coordinate system tag create a coordinate system object and try to parse xml
        CoordinateSystem *coordinateSystem = new CoordinateSystem();
        QDomElement coordTag = coordinateSystemList.at(i).toElement();
        if(!coordinateSystem->fromOpenIndyXML(coordTag)){
            delete coordinateSystem;
            continue;
        }

        //create a feature wrapper for the station
        FeatureWrapper *coordinateSystemWrapper = new FeatureWrapper();
        coordinateSystemWrapper->setCoordinateSystem(coordinateSystem);

        //add the coordinate system to the list of successfully loaded coordinate systems
        OiProjectExchanger::myCoordinateSystems.insert(coordinateSystem->getId(), coordinateSystemWrapper);

    }

    //at least one coordinate system has to be available
    if(OiProjectExchanger::myCoordinateSystems.size() == 0){
        return false;
    }

    return true;

}

/*!
 * \brief OiProjectExchanger::loadTransformationParameters
 * \param project
 * \return
 */
bool OiProjectExchanger::loadTransformationParameters(const QDomDocument &project){

    QDomElement trafoParams = project.documentElement().firstChildElement("transformationParameters");
    if(!trafoParams.isNull()){

        //get a list of all trafo param tags
        QDomNodeList trafoParamList = trafoParams.childNodes();

        for(int i = 0; i < trafoParamList.size(); i++){

            //for each trafo param tag create a trafo param object and try to parse xml
            TrafoParam *trafoParam = new TrafoParam();
            QDomElement trafoTag = trafoParamList.at(i).toElement();
            if(!trafoParam->fromOpenIndyXML(trafoTag)){
                delete trafoParam;
                continue;
            }

            //create a feature wrapper for the station
            FeatureWrapper *trafoParamWrapper = new FeatureWrapper();
            trafoParamWrapper->setTrafoParam(trafoParam);

            //add the trafo param to the list of successfully loaded trafo params
            OiProjectExchanger::myTransformationParameters.insert(trafoParam->getId(), trafoParamWrapper);

        }

    }

    return true;

}

/*!
 * \brief OiProjectExchanger::loadGeometries
 * \param project
 * \return
 */
bool OiProjectExchanger::loadGeometries(const QDomDocument &project){

    QDomElement geometries = project.documentElement().firstChildElement("geometries");
    if(!geometries.isNull()){

        //get a list of all geometry tags
        QDomNodeList geometryList = geometries.childNodes();

        for(int i = 0; i < geometryList.size(); i++){

            //get the geometry tag and check if the type attribute is available
            QDomElement geometry = geometryList.at(i).toElement();
            if(geometry.isNull() || !geometry.hasAttribute("type")){
                continue;
            }

            //get the type of geometry
            Configuration::FeatureTypes geomType = Configuration::getFeatureTypeEnum(geometry.attribute("type"));

            //create a feature wrapper object to hold the geometry
            FeatureWrapper *myFeatureWrapper;
            myFeatureWrapper = new FeatureWrapper();

            //depending on the type of geometry call the right fromOpenIndyXML method
            switch(geomType){
            case Configuration::eCircleFeature:{
                Circle *myCircle = new Circle(false);
                myFeatureWrapper->setCircle(myCircle);
                if(!myCircle->fromOpenIndyXML(geometry)){
                    delete myCircle;
                    delete myFeatureWrapper;
                    continue;
                }
                break;
            }case Configuration::eConeFeature:{
                Cone *myCone = new Cone(false);
                myFeatureWrapper->setCone(myCone);
                if(!myCone->fromOpenIndyXML(geometry)){
                    delete myCone;
                    delete myFeatureWrapper;
                    continue;
                }
                break;
            }case Configuration::eCylinderFeature:{
                Cylinder *myCylinder = new Cylinder(false);
                myFeatureWrapper->setCylinder(myCylinder);
                if(!myCylinder->fromOpenIndyXML(geometry)){
                    delete myCylinder;
                    delete myFeatureWrapper;
                    continue;
                }
                break;
            }case Configuration::eEllipsoidFeature:{
                Ellipsoid *myEllipsoid = new Ellipsoid(false);
                myFeatureWrapper->setEllipsoid(myEllipsoid);
                if(!myEllipsoid->fromOpenIndyXML(geometry)){
                    delete myEllipsoid;
                    delete myFeatureWrapper;
                    continue;
                }
                break;
            }case Configuration::eHyperboloidFeature:{
                Hyperboloid *myHyperboloid = new Hyperboloid(false);
                myFeatureWrapper->setHyperboloid(myHyperboloid);
                if(!myHyperboloid->fromOpenIndyXML(geometry)){
                    delete myHyperboloid;
                    delete myFeatureWrapper;
                    continue;
                }
                break;
            }case Configuration::eLineFeature:{
                Line *myLine = new Line(false);
                myFeatureWrapper->setLine(myLine);
                if(!myLine->fromOpenIndyXML(geometry)){
                    delete myLine;
                    delete myFeatureWrapper;
                    continue;
                }
                break;
            }case Configuration::eNurbsFeature:{
                Nurbs *myNurbs = new Nurbs(false);
                myFeatureWrapper->setNurbs(myNurbs);
                if(!myNurbs->fromOpenIndyXML(geometry)){
                    delete myNurbs;
                    delete myFeatureWrapper;
                    continue;
                }
                break;
            }case Configuration::eParaboloidFeature:{
                Paraboloid *myParaboloid = new Paraboloid(false);
                myFeatureWrapper->setParaboloid(myParaboloid);
                if(!myParaboloid->fromOpenIndyXML(geometry)){
                    delete myParaboloid;
                    delete myFeatureWrapper;
                    continue;
                }
                break;
            }case Configuration::ePlaneFeature:{
                Plane *myPlane = new Plane(false);
                myFeatureWrapper->setPlane(myPlane);
                if(!myPlane->fromOpenIndyXML(geometry)){
                    delete myPlane;
                    delete myFeatureWrapper;
                    continue;
                }
                break;
            }case Configuration::ePointFeature:{
                Point *myPoint = new Point(false);
                myFeatureWrapper->setPoint(myPoint);
                if(!myPoint->fromOpenIndyXML(geometry)){
                    delete myPoint;
                    delete myFeatureWrapper;
                    continue;
                }
                break;
            }case Configuration::ePointCloudFeature:{
                PointCloud *myPointCloud = new PointCloud(false);
                myFeatureWrapper->setPointCloud(myPointCloud);
                if(!myPointCloud->fromOpenIndyXML(geometry)){
                    delete myPointCloud;
                    delete myFeatureWrapper;
                    continue;
                }
                break;
            }case Configuration::eScalarEntityAngleFeature:{
                ScalarEntityAngle *myAngle = new ScalarEntityAngle(false);
                myFeatureWrapper->setScalarEntityAngle(myAngle);
                if(!myAngle->fromOpenIndyXML(geometry)){
                    delete myAngle;
                    delete myFeatureWrapper;
                    continue;
                }
                break;
            }case Configuration::eScalarEntityDistanceFeature:{
                ScalarEntityDistance *myDistance = new ScalarEntityDistance(false);
                myFeatureWrapper->setScalarEntityDistance(myDistance);
                if(!myDistance->fromOpenIndyXML(geometry)){
                    delete myDistance;
                    delete myFeatureWrapper;
                    continue;
                }
                break;
            }case Configuration::eScalarEntityMeasurementSeriesFeature:{
                ScalarEntityMeasurementSeries *myMeasurementSeries = new ScalarEntityMeasurementSeries(false);
                myFeatureWrapper->setScalarEntityMeasurementSeries(myMeasurementSeries);
                if(!myMeasurementSeries->fromOpenIndyXML(geometry)){
                    delete myMeasurementSeries;
                    delete myFeatureWrapper;
                    continue;
                }
                break;
            }case Configuration::eScalarEntityTemperatureFeature:{
                ScalarEntityTemperature *myTemperature = new ScalarEntityTemperature(false);
                myFeatureWrapper->setScalarEntityTemperature(myTemperature);
                if(!myTemperature->fromOpenIndyXML(geometry)){
                    delete myTemperature;
                    delete myFeatureWrapper;
                    continue;
                }
                break;
            }case Configuration::eSphereFeature:{
                Sphere *mySphere = new Sphere(false);
                myFeatureWrapper->setSphere(mySphere);
                if(!mySphere->fromOpenIndyXML(geometry)){
                    delete mySphere;
                    delete myFeatureWrapper;
                    continue;
                }
                break;
            }default:{
                delete myFeatureWrapper;
                continue;
            }}

            //add the feature wrapper to the list of successfully loaded geometries
            OiProjectExchanger::myGeometries.insert(myFeatureWrapper->getFeature()->getId(), myFeatureWrapper);

        }

    }

    return true;

}

/*!
 * \brief OiProjectExchanger::loadConfigs
 * \param project
 * \return
 */
bool OiProjectExchanger::loadConfigs(const QDomDocument &project){

    QDomElement configs = project.documentElement().firstChildElement("configs");

    if(!configs.isNull()){

        //measurement configs
        QDomElement mConfigs = configs.firstChildElement("measurementConfigs");
        if(!mConfigs.isNull()){
            QDomNodeList mConfigList = mConfigs.childNodes();
            for(int i = 0; i < mConfigList.size(); ++i){
                QDomElement mConfigElement = mConfigList.at(i).toElement();
                MeasurementConfig mConfig;
                if(mConfig.fromOpenIndyXML(mConfigElement)){
                    OiProjectExchanger::myMConfigs.insert(mConfig.getName(), mConfig);
                }
            }
        }

        //sensor configs
        QDomElement sConfigs = configs.firstChildElement("sensorConfigs");
        if(!sConfigs.isNull()){
            QDomNodeList sConfigList = sConfigs.childNodes();
            for(int i = 0; i < sConfigList.size(); ++i){
                QDomElement sConfigElement = sConfigList.at(i).toElement();
                SensorConfiguration sConfig;
                if(sConfig.fromOpenIndyXML(sConfigElement)){
                    OiProjectExchanger::mySConfigs.insert(sConfig.getName(), sConfig);
                }
            }
        }

    }

    return true;

}

/*!
 * \brief OiProjectExchanger::restoreStationDependencies
 * \param project
 * \return
 */
bool OiProjectExchanger::restoreStationDependencies(const QDomDocument &project){

    bool result = true;

    //iterate through the station tags in the xml project
    QDomElement stations = project.documentElement().firstChildElement("stations");
    if(!stations.isNull()){
        QDomNodeList stationList = stations.childNodes();
        for(int i = 0; i < stationList.size(); i++){

            //get the station tag at position i and the corresponding FeatureWrapper
            QDomElement station = stationList.at(i).toElement();
            if(!station.hasAttribute("id") || !OiProjectExchanger::myStations.contains(station.attribute("id").toInt())){
                continue;
            }
            FeatureWrapper *myStation = OiProjectExchanger::myStations.value(station.attribute("id").toInt());
            if(myStation == NULL || myStation->getStation() == NULL || myStation->getStation()->sensorPad == NULL){
                continue;
            }

            result = true;

            //load used sensor plugins
            QDomElement usedSensors = station.firstChildElement("usedSensors");
            if(!usedSensors.isNull()){
                QDomNodeList usedSensorsList = usedSensors.childNodes();
                for(int j = 0; j < usedSensorsList.size(); j++){
                    QDomElement usedSensor = usedSensorsList.at(j).toElement();
                    if(usedSensor.hasAttribute("name") && usedSensor.hasAttribute("plugin")){
                        QString pluginFilePath = SystemDbManager::getPluginFilePath(usedSensor.attribute("name"), usedSensor.attribute("plugin"));
                        Sensor *mySensor = PluginLoader::loadSensorPlugin(pluginFilePath, usedSensor.attribute("name"));
                        if(mySensor != NULL){
                            myStation->getStation()->sensorPad->usedSensors.append(mySensor);
                        }
                    }
                }
            }

            //load active sensor plugin
            QDomElement activeSensor = station.firstChildElement("activeSensor");
            if(!activeSensor.isNull() && activeSensor.hasAttribute("name") && activeSensor.hasAttribute("plugin")){
                QString pluginFilePath = SystemDbManager::getPluginFilePath(activeSensor.attribute("name"), activeSensor.attribute("plugin"));
                Sensor *mySensor = PluginLoader::loadSensorPlugin(pluginFilePath, activeSensor.attribute("name"));
                if(mySensor != NULL){
                    myStation->getStation()->sensorPad->instrument = mySensor;
                }else{
                    myStation->getStation()->sensorPad->instrument = NULL;
                    //result = false;
                }
            }/*else{
                result = false;
            }*/

            //set position point
            QDomElement position = station.firstChildElement("position");
            if(!position.isNull() && position.hasAttribute("ref") && OiProjectExchanger::myGeometries.contains(position.attribute("ref").toInt())){
                FeatureWrapper *myPosition = OiProjectExchanger::myGeometries.value(position.attribute("ref").toInt());
                if(myPosition != NULL && myPosition->getPoint() != NULL){
                    myStation->getStation()->position = myPosition->getPoint();
                }
            }

            //set coordinate system
            QDomElement coordinateSystem = station.firstChildElement("coordinateSystem");
            if(!coordinateSystem.isNull() && coordinateSystem.hasAttribute("ref")){
                FeatureWrapper *mySystem = OiProjectExchanger::myCoordinateSystems.value(coordinateSystem.attribute("ref").toInt());
                if(mySystem != NULL && mySystem->getCoordinateSystem() != NULL){
                    myStation->getStation()->coordSys = mySystem->getCoordinateSystem();
                    myStation->getStation()->coordSys->isStationSystem = true;
                }else{
                    result = false;
                }
            }else{
                result = false;
            }

            //set used for
            QDomElement usedFor = station.firstChildElement("usedFor");
            if(!usedFor.isNull()){
                QDomNodeList usedForList = usedFor.childNodes();
                for(int j = 0; j < usedForList.size(); j++){
                    QDomElement feature = usedForList.at(j).toElement();
                    if(feature.hasAttribute("ref")){
                        FeatureWrapper *myFeature = NULL;
                        if(OiProjectExchanger::myGeometries.contains(feature.attribute("ref").toInt())){
                            myFeature = OiProjectExchanger::myGeometries.value(feature.attribute("ref").toInt());
                        }else if(OiProjectExchanger::myCoordinateSystems.contains(feature.attribute("ref").toInt())){
                            myFeature = OiProjectExchanger::myCoordinateSystems.value(feature.attribute("ref").toInt());
                        }else if(OiProjectExchanger::myTransformationParameters.contains(feature.attribute("ref").toInt())){
                            myFeature = OiProjectExchanger::myTransformationParameters.value(feature.attribute("ref").toInt());
                        }
                        if(myFeature != NULL){
                            myStation->getStation()->usedFor.append(myFeature);
                        }
                    }
                }
            }

        }
    }

    return result;

}

/*!
 * \brief OiProjectExchanger::restoreCoordinateSystemDependencies
 * \param project
 * \return
 */
bool OiProjectExchanger::restoreCoordinateSystemDependencies(const QDomDocument &project){

    bool result = true;

    //iterate through the coordinate system tags in the xml project
    QDomElement coordinateSystems = project.documentElement().firstChildElement("coordinateSystems");
    if(!coordinateSystems.isNull()){
        QDomNodeList coordinateSystemList = coordinateSystems.childNodes();
        for(int i = 0; i < coordinateSystemList.size(); i++){

            //get the coordinate system tag at position i and the corresponding FeatureWrapper
            QDomElement coordinateSystem = coordinateSystemList.at(i).toElement();
            if(!coordinateSystem.hasAttribute("id") || !OiProjectExchanger::myCoordinateSystems.contains(coordinateSystem.attribute("id").toInt())){
                continue;
            }
            FeatureWrapper *myCoordinateSystem = OiProjectExchanger::myCoordinateSystems.value(coordinateSystem.attribute("id").toInt());
            if(myCoordinateSystem == NULL || myCoordinateSystem->getCoordinateSystem() == NULL){
                continue;
            }

            result = true;

            //load function plugins
            QDomElement functions = coordinateSystem.firstChildElement("functions");
            if(!functions.isNull()){
                QList<Function*> myFunctions = OiProjectExchanger::restoreFunctionDependencies(functions);
                for(int j = 0; j < myFunctions.size(); j++){
                    myCoordinateSystem->getCoordinateSystem()->addFunction(myFunctions.at(j));
                }
            }

            //set observations (made from a station system)
            QDomElement observations = coordinateSystem.firstChildElement("observations");
            if(!observations.isNull()){
                QDomNodeList observationList = observations.childNodes();
                for(int j = 0; j < observationList.size(); j++){
                    QDomElement observation = observationList.at(j).toElement();
                    if(observation.hasAttribute("ref") && OiProjectExchanger::myObservations.contains(observation.attribute("ref").toInt())){
                        Observation *myObservation = OiProjectExchanger::myObservations.value(observation.attribute("ref").toInt());
                        myCoordinateSystem->getCoordinateSystem()->addObservation(myObservation);
                    }
                }
            }

            //set nominal geometries
            QDomElement nominalGeometries = coordinateSystem.firstChildElement("nominalGeometries");
            if(!nominalGeometries.isNull()){
                QDomNodeList geometryList = nominalGeometries.childNodes();
                for(int j = 0; j < geometryList.size(); j++){
                    QDomElement geometry = geometryList.at(j).toElement();
                    if(geometry.hasAttribute("ref") && OiProjectExchanger::myGeometries.contains(geometry.attribute("ref").toInt())){
                        FeatureWrapper *myGeometry = OiProjectExchanger::myGeometries.value(geometry.attribute("ref").toInt());
                        if(myGeometry->getGeometry()->getIsNominal()){
                            myCoordinateSystem->getCoordinateSystem()->addNominal(myGeometry);
                        }
                    }
                }
            }

            //set used for
            QDomElement usedFor = coordinateSystem.firstChildElement("usedFor");
            if(!usedFor.isNull()){
                QDomNodeList featureList = usedFor.childNodes();
                for(int j = 0; j < featureList.size(); j++){
                    QDomElement feature = featureList.at(j).toElement();
                    if(feature.hasAttribute("ref")){

                        if(OiProjectExchanger::myGeometries.contains(feature.attribute("ref").toInt())){
                            myCoordinateSystem->getCoordinateSystem()->usedFor.append(OiProjectExchanger::myGeometries.value(feature.attribute("ref").toInt()));
                        }else if(OiProjectExchanger::myCoordinateSystems.contains(feature.attribute("ref").toInt())){
                            myCoordinateSystem->getCoordinateSystem()->usedFor.append(OiProjectExchanger::myCoordinateSystems.value(feature.attribute("ref").toInt()));
                        }else if(OiProjectExchanger::myTransformationParameters.contains(feature.attribute("ref").toInt())){
                            myCoordinateSystem->getCoordinateSystem()->usedFor.append(OiProjectExchanger::myTransformationParameters.value(feature.attribute("ref").toInt()));
                        }

                    }
                }
            }

            //set previously needed
            QDomElement previouslyNeeded = coordinateSystem.firstChildElement("previouslyNeeded");
            if(!previouslyNeeded.isNull()){
                QDomNodeList featureList = previouslyNeeded.childNodes();
                for(int j = 0; j < featureList.size(); j++){
                    QDomElement feature = featureList.at(j).toElement();
                    if(feature.hasAttribute("ref")){

                        if(OiProjectExchanger::myGeometries.contains(feature.attribute("ref").toInt())){
                            myCoordinateSystem->getCoordinateSystem()->previouslyNeeded.append(OiProjectExchanger::myGeometries.value(feature.attribute("ref").toInt()));
                        }else if(OiProjectExchanger::myCoordinateSystems.contains(feature.attribute("ref").toInt())){
                            myCoordinateSystem->getCoordinateSystem()->previouslyNeeded.append(OiProjectExchanger::myCoordinateSystems.value(feature.attribute("ref").toInt()));
                        }else if(OiProjectExchanger::myTransformationParameters.contains(feature.attribute("ref").toInt())){
                            myCoordinateSystem->getCoordinateSystem()->previouslyNeeded.append(OiProjectExchanger::myTransformationParameters.value(feature.attribute("ref").toInt()));
                        }else if(OiProjectExchanger::myStations.contains(feature.attribute("ref").toInt())){
                            myCoordinateSystem->getCoordinateSystem()->previouslyNeeded.append(OiProjectExchanger::myStations.value(feature.attribute("ref").toInt()));
                        }

                    }
                }
            }

            //set transformation params
            QDomElement trafoParams = coordinateSystem.firstChildElement("transformationParameters");
            if(!trafoParams.isNull()){
                QDomNodeList trafoParamList = trafoParams.childNodes();
                for(int j = 0; j < trafoParamList.size(); j++){
                    QDomElement trafoParam = trafoParamList.at(j).toElement();
                    if(trafoParam.hasAttribute("ref") && OiProjectExchanger::myTransformationParameters.contains(trafoParam.attribute("ref").toInt())){
                        FeatureWrapper *myTrafoParam = OiProjectExchanger::myTransformationParameters.value(trafoParam.attribute("ref").toInt());
                        myCoordinateSystem->getCoordinateSystem()->addTransformationParameter(myTrafoParam->getTrafoParam());
                    }
                }
            }

        }
    }

    return result;

}

/*!
 * \brief OiProjectExchanger::restoreTrafoParamDependencies
 * \param project
 * \return
 */
bool OiProjectExchanger::restoreTrafoParamDependencies(const QDomDocument &project){

    bool result = true;

    //iterate through the trafo param tags in the xml project
    QDomElement trafoParams = project.documentElement().firstChildElement("transformationParameters");
    if(!trafoParams.isNull()){
        QDomNodeList trafoParamList = trafoParams.childNodes();
        for(int i = 0; i < trafoParamList.size(); i++){

            //get the trafo param tag at position i and the corresponding FeatureWrapper
            QDomElement trafoParam = trafoParamList.at(i).toElement();
            if(!trafoParam.hasAttribute("id") || !OiProjectExchanger::myTransformationParameters.contains(trafoParam.attribute("id").toInt())){
                continue;
            }
            FeatureWrapper *myTrafoParam = OiProjectExchanger::myTransformationParameters.value(trafoParam.attribute("id").toInt());
            if(myTrafoParam == NULL || myTrafoParam->getTrafoParam() == NULL){
                continue;
            }

            //set start and destination system
            QDomElement from = trafoParam.firstChildElement("from");
            QDomElement to = trafoParam.firstChildElement("to");
            if(from.isNull() || to.isNull() || !from.hasAttribute("ref") || !to.hasAttribute("ref")
                    || !OiProjectExchanger::myCoordinateSystems.contains(from.attribute("ref").toInt())
                    || !OiProjectExchanger::myCoordinateSystems.contains(from.attribute("ref").toInt())){
                continue;
            }
            myTrafoParam->getTrafoParam()->setCoordinateSystems(OiProjectExchanger::myCoordinateSystems.value(from.attribute("ref").toInt())->getCoordinateSystem(),
                                                                OiProjectExchanger::myCoordinateSystems.value(to.attribute("ref").toInt())->getCoordinateSystem());

            //load function plugins
            QDomElement functions = trafoParam.firstChildElement("functions");
            if(!functions.isNull()){
                QList<Function*> myFunctions = OiProjectExchanger::restoreFunctionDependencies(functions);
                for(int j = 0; j < myFunctions.size(); j++){
                    myTrafoParam->getTrafoParam()->addFunction(myFunctions.at(j));
                }
            }

            //set used for
            QDomElement usedFor = trafoParam.firstChildElement("usedFor");
            if(!usedFor.isNull()){
                QDomNodeList featureList = usedFor.childNodes();
                for(int j = 0; j < featureList.size(); j++){
                    QDomElement feature = featureList.at(j).toElement();
                    if(feature.hasAttribute("ref")){

                        if(OiProjectExchanger::myGeometries.contains(feature.attribute("ref").toInt())){
                            myTrafoParam->getTrafoParam()->usedFor.append(OiProjectExchanger::myGeometries.value(feature.attribute("ref").toInt()));
                        }else if(OiProjectExchanger::myCoordinateSystems.contains(feature.attribute("ref").toInt())){
                            myTrafoParam->getTrafoParam()->usedFor.append(OiProjectExchanger::myCoordinateSystems.value(feature.attribute("ref").toInt()));
                        }else if(OiProjectExchanger::myTransformationParameters.contains(feature.attribute("ref").toInt())){
                            myTrafoParam->getTrafoParam()->usedFor.append(OiProjectExchanger::myTransformationParameters.value(feature.attribute("ref").toInt()));
                        }

                    }
                }
            }

            //set previously needed
            QDomElement previouslyNeeded = trafoParam.firstChildElement("previouslyNeeded");
            if(!previouslyNeeded.isNull()){
                QDomNodeList featureList = previouslyNeeded.childNodes();
                for(int j = 0; j < featureList.size(); j++){
                    QDomElement feature = featureList.at(j).toElement();
                    if(feature.hasAttribute("ref")){

                        if(OiProjectExchanger::myGeometries.contains(feature.attribute("ref").toInt())){
                            myTrafoParam->getTrafoParam()->previouslyNeeded.append(OiProjectExchanger::myGeometries.value(feature.attribute("ref").toInt()));
                        }else if(OiProjectExchanger::myCoordinateSystems.contains(feature.attribute("ref").toInt())){
                            myTrafoParam->getTrafoParam()->previouslyNeeded.append(OiProjectExchanger::myCoordinateSystems.value(feature.attribute("ref").toInt()));
                        }else if(OiProjectExchanger::myTransformationParameters.contains(feature.attribute("ref").toInt())){
                            myTrafoParam->getTrafoParam()->previouslyNeeded.append(OiProjectExchanger::myTransformationParameters.value(feature.attribute("ref").toInt()));
                        }else if(OiProjectExchanger::myStations.contains(feature.attribute("ref").toInt())){
                            myTrafoParam->getTrafoParam()->previouslyNeeded.append(OiProjectExchanger::myStations.value(feature.attribute("ref").toInt()));
                        }

                    }
                }
            }

            result = true;

        }
    }

    return result;

}

/*!
 * \brief OiProjectExchanger::restoreGeometryDependencies
 * \param project
 * \return
 */
bool OiProjectExchanger::restoreGeometryDependencies(const QDomDocument &project){

    bool result = true;

    //iterate through the geometry tags in the xml project
    QDomElement geometries = project.documentElement().firstChildElement("geometries");
    if(!geometries.isNull()){
        QDomNodeList geometryList = geometries.childNodes();
        for(int i = 0; i < geometryList.size(); i++){

            //get the geometry tag at position i and the corresponding FeatureWrapper
            QDomElement geometry = geometryList.at(i).toElement();
            if(!geometry.hasAttribute("id") || !OiProjectExchanger::myGeometries.contains(geometry.attribute("id").toInt())){
                continue;
            }
            FeatureWrapper *myGeometry = OiProjectExchanger::myGeometries.value(geometry.attribute("id").toInt());
            if(myGeometry == NULL || myGeometry->getGeometry() == NULL){
                continue;
            }

            //set observations
            QDomElement observations = geometry.firstChildElement("observations");
            if(!observations.isNull()){
                QDomNodeList observationList = observations.childNodes();
                for(int j = 0; j < observationList.size(); j++){
                    QDomElement observation = observationList.at(j).toElement();
                    if(observation.hasAttribute("ref")){
                        Observation *myObservation = OiProjectExchanger::myObservations.value(observation.attribute("ref").toInt());
                        myGeometry->getGeometry()->addObservation(myObservation);
                        myObservation->myTargetGeometries.append(myGeometry->getGeometry());
                    }
                }
            }

            //load function plugins
            QDomElement functions = geometry.firstChildElement("functions");
            if(!functions.isNull()){
                QList<Function*> myFunctions = OiProjectExchanger::restoreFunctionDependencies(functions);
                for(int j = 0; j < myFunctions.size(); j++){
                    myGeometry->getGeometry()->addFunction(myFunctions.at(j));
                }
            }

            //set used for
            QDomElement usedFor = geometry.firstChildElement("usedFor");
            if(!usedFor.isNull()){
                QDomNodeList featureList = usedFor.childNodes();
                for(int j = 0; j < featureList.size(); j++){
                    QDomElement feature = featureList.at(j).toElement();
                    if(feature.hasAttribute("ref")){

                        if(OiProjectExchanger::myGeometries.contains(feature.attribute("ref").toInt())){
                            myGeometry->getGeometry()->usedFor.append(OiProjectExchanger::myGeometries.value(feature.attribute("ref").toInt()));
                        }else if(OiProjectExchanger::myCoordinateSystems.contains(feature.attribute("ref").toInt())){
                            myGeometry->getGeometry()->usedFor.append(OiProjectExchanger::myCoordinateSystems.value(feature.attribute("ref").toInt()));
                        }else if(OiProjectExchanger::myTransformationParameters.contains(feature.attribute("ref").toInt())){
                            myGeometry->getGeometry()->usedFor.append(OiProjectExchanger::myTransformationParameters.value(feature.attribute("ref").toInt()));
                        }

                    }
                }
            }

            //set previously needed
            QDomElement previouslyNeeded = geometry.firstChildElement("previouslyNeeded");
            if(!previouslyNeeded.isNull()){
                QDomNodeList featureList = previouslyNeeded.childNodes();
                for(int j = 0; j < featureList.size(); j++){
                    QDomElement feature = featureList.at(j).toElement();
                    if(feature.hasAttribute("ref")){

                        if(OiProjectExchanger::myGeometries.contains(feature.attribute("ref").toInt())){
                            myGeometry->getGeometry()->previouslyNeeded.append(OiProjectExchanger::myGeometries.value(feature.attribute("ref").toInt()));
                        }else if(OiProjectExchanger::myCoordinateSystems.contains(feature.attribute("ref").toInt())){
                            myGeometry->getGeometry()->previouslyNeeded.append(OiProjectExchanger::myCoordinateSystems.value(feature.attribute("ref").toInt()));
                        }else if(OiProjectExchanger::myTransformationParameters.contains(feature.attribute("ref").toInt())){
                            myGeometry->getGeometry()->previouslyNeeded.append(OiProjectExchanger::myTransformationParameters.value(feature.attribute("ref").toInt()));
                        }else if(OiProjectExchanger::myStations.contains(feature.attribute("ref").toInt())){
                            myGeometry->getGeometry()->previouslyNeeded.append(OiProjectExchanger::myStations.value(feature.attribute("ref").toInt()));
                        }

                    }
                }
            }

            //set nominal geometries
            QDomElement nominalGeometries = geometry.firstChildElement("nominalGeometries");
            if(!nominalGeometries.isNull()){
                QDomNodeList nominalList = nominalGeometries.childNodes();
                for(int j = 0; j < nominalList.size(); j++){
                    QDomElement nominalGeometry = nominalList.at(j).toElement();
                    if(nominalGeometry.hasAttribute("ref") && OiProjectExchanger::myGeometries.contains(nominalGeometry.attribute("ref").toInt())){
                        FeatureWrapper *myNominalGeometry = OiProjectExchanger::myGeometries.value(nominalGeometry.attribute("ref").toInt());
                        myGeometry->getGeometry()->addNominal(myNominalGeometry->getGeometry());
                    }
                }
            }

            //set actual
            QDomElement actualGeometry = geometry.firstChildElement("actual");
            if(!actualGeometry.isNull() && actualGeometry.hasAttribute("ref") && OiProjectExchanger::myGeometries.contains(actualGeometry.attribute("ref").toInt())){
                FeatureWrapper *myActualGeometry = OiProjectExchanger::myGeometries.value(actualGeometry.attribute("ref").toInt());
                myGeometry->getGeometry()->setMyActual(myActualGeometry->getGeometry());
            }

            //set nominal system
            QDomElement nominalSystem = geometry.firstChildElement("nominalCoordinateSystem");
            if(!nominalSystem.isNull() && nominalSystem.hasAttribute("ref") && OiProjectExchanger::myCoordinateSystems.contains(nominalSystem.attribute("ref").toInt())){
                FeatureWrapper *myNominalSystem = OiProjectExchanger::myCoordinateSystems.value(nominalSystem.attribute("ref").toInt());
                myGeometry->getGeometry()->setNominalSystem(myNominalSystem->getCoordinateSystem());
            }

            //set measurement configs
            QDomElement mConfigElement = geometry.firstChildElement("measurementConfig");
            if(!mConfigElement.isNull() && mConfigElement.hasAttribute("name") && OiProjectExchanger::myMConfigs.contains(mConfigElement.attribute("name"))){
                MeasurementConfig mConfig = OiProjectExchanger::myMConfigs.value(mConfigElement.attribute("name"));
                myGeometry->getGeometry()->setMeasurementConfig(mConfig);
            }

        }
    }

    return result;

}

/*!
 * \brief OiProjectExchanger::restoreObservationDependencies
 * \param project
 * \return
 */
bool OiProjectExchanger::restoreObservationDependencies(const QDomDocument &project){

    bool result = true;

    //iterate through the observation tags in the xml project
    QDomElement observations = project.documentElement().firstChildElement("observations");
    if(!observations.isNull()){
        QDomNodeList observationList = observations.childNodes();
        for(int i = 0; i < observationList.size(); i++){

            //get the observation tag at position i and the corresponding Observation
            QDomElement observation = observationList.at(i).toElement();
            if(!observation.hasAttribute("id") || !OiProjectExchanger::myObservations.contains(observation.attribute("id").toInt())){
                continue;
            }
            Observation *myObservation = OiProjectExchanger::myObservations.value(observation.attribute("id").toInt());
            if(myObservation == NULL || myObservation->myReading == NULL){
                continue;
            }

            //set station and sensor
            QDomElement station = observation.firstChildElement("station");
            if(!station.isNull() && station.hasAttribute("ref") && OiProjectExchanger::myStations.contains(station.attribute("ref").toInt())){
                FeatureWrapper *myStation = OiProjectExchanger::myStations.value(station.attribute("ref").toInt());
                myObservation->myStation = myStation->getStation();
                if(myStation->getStation()->sensorPad != NULL && myStation->getStation()->sensorPad->instrument != NULL){
                    myObservation->myReading->instrument = myStation->getStation()->sensorPad->instrument;
                }
            }

        }
    }

    return result;

}

/*!
 * \brief OiProjectExchanger::restoreFunctionDependencies
 * Helper method which gets a functions tag as input and then returns a list of function objects ordered by their execution index
 * \param functions
 * \return
 */
QList<Function *> OiProjectExchanger::restoreFunctionDependencies(const QDomElement &functions){

    QList<Function*> result;

    if(!functions.isNull()){

        //get a list of all function tags
        QDomNodeList functionList = functions.childNodes();

        //map to save all found functions with their executionIndex as key
        QMap<int, Function*> functionOrder;

        for(int i = 0; i < functionList.size(); i++){

            //get a function tag
            QDomElement function = functionList.at(i).toElement();
            if(!function.hasAttribute("name") || !function.hasAttribute("plugin") || !function.hasAttribute("executionIndex")){
                continue;
            }

            //load function plugin
            QString pluginFilePath = SystemDbManager::getPluginFilePath(function.attribute("name"), function.attribute("plugin"));
            Function *myFunction = PluginLoader::loadFunctionPlugin(pluginFilePath, function.attribute("name"));
            if(myFunction == NULL){
                continue;
            }

            //load function from XML
            myFunction->fromOpenIndyXML(function);

            //add input elements
            QDomElement inputElements = function.firstChildElement("inputElements");
            if(!inputElements.isNull()){
                QDomNodeList elementList = inputElements.childNodes();
                for(int j = 0; j < elementList.size(); j++){
                    QDomElement inputElement = elementList.at(j).toElement();
                    if(inputElement.hasAttribute("index") && inputElement.hasAttribute("type") && inputElement.hasAttribute("ref")){

                        if(OiProjectExchanger::myStations.contains(inputElement.attribute("ref").toInt())){
                            Station *station = OiProjectExchanger::myStations.value(inputElement.attribute("ref").toInt())->getStation();
                            myFunction->addStation(station, inputElement.attribute("executionIndex").toInt());
                        }else if(OiProjectExchanger::myCoordinateSystems.contains(inputElement.attribute("ref").toInt())){
                            CoordinateSystem *coordinateSystem = OiProjectExchanger::myCoordinateSystems.value(inputElement.attribute("ref").toInt())->getCoordinateSystem();
                            myFunction->addCoordSystem(coordinateSystem, inputElement.attribute("executionIndex").toInt());
                        }else if(OiProjectExchanger::myTransformationParameters.contains(inputElement.attribute("ref").toInt())){
                            TrafoParam *trafoParam = OiProjectExchanger::myTransformationParameters.value(inputElement.attribute("ref").toInt())->getTrafoParam();
                            myFunction->addTrafoParam(trafoParam, inputElement.attribute("executionIndex").toInt());
                        }else if(OiProjectExchanger::myGeometries.contains(inputElement.attribute("ref").toInt())){
                            FeatureWrapper *geometry = OiProjectExchanger::myGeometries.value(inputElement.attribute("ref").toInt());
                            myFunction->addFeature(geometry, inputElement.attribute("executionIndex").toInt());
                        }else if(OiProjectExchanger::myObservations.contains(inputElement.attribute("ref").toInt())){
                            Observation *observation = OiProjectExchanger::myObservations.value(inputElement.attribute("ref").toInt());
                            myFunction->addObservation(observation, inputElement.attribute("executionIndex").toInt());
                        }else if(OiProjectExchanger::myReadings.contains(inputElement.attribute("ref").toInt())){
                            Reading *reading = OiProjectExchanger::myReadings.value(inputElement.attribute("ref").toInt());
                            myFunction->addReading(reading, inputElement.attribute("executionIndex").toInt());
                        }

                    }
                }
            }

            //add the function to the functionOrder map
            functionOrder.insert(function.attribute("executionIndex").toInt(), myFunction);

        }

        //iterate through all functions in functionOrder map
        for(int i = 0; i < functionOrder.size(); i++){
            if(!functionOrder.contains(i)){
                break;
            }
            result.append(functionOrder.value(i));
        }
    }

    return result;

}

/*!
 * \brief OiProjectExchanger::clearHelperMaps
 * Clear the helper maps for loading an OpenIndy project. If deleteOnClear is true then the list objects
 * are not only removed but also deleted.
 * \param deleteOnClear
 */
void OiProjectExchanger::clearHelperMaps(bool deleteOnClear){

    //delete the helper maps objects
    if(deleteOnClear){

        //delete observations
        QList<Observation *> myObservations = OiProjectExchanger::myObservations.values();
        foreach(Observation *obs, myObservations){
            delete obs;
        }

        //delete stations
        QList<FeatureWrapper *> myStations = OiProjectExchanger::myStations.values();
        foreach(FeatureWrapper *s, myStations){
            delete s->getStation();
            delete s;
        }

        //delete coordinate systems
        QList<FeatureWrapper *> myCoordinateSystems = OiProjectExchanger::myCoordinateSystems.values();
        foreach(FeatureWrapper *c, myCoordinateSystems){
            delete c->getCoordinateSystem();
            delete c;
        }

        //delete trafo params
        QList<FeatureWrapper *> myTrafoParams = OiProjectExchanger::myTransformationParameters.values();
        foreach(FeatureWrapper *t, myTrafoParams){
            delete t->getTrafoParam();
            delete t;
        }

        //delete geometries
        QList<FeatureWrapper *> myGeometries = OiProjectExchanger::myGeometries.values();
        foreach(FeatureWrapper *g, myGeometries){
            delete g->getFeature();
            delete g;
        }

    }

    //clear the helper maps
    OiProjectExchanger::myObservations.clear();
    OiProjectExchanger::myStations.clear();
    OiProjectExchanger::myCoordinateSystems.clear();

}
