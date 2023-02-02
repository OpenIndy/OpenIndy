#include "projectexchanger.h"

QMap<int, QPointer<Observation> > ProjectExchanger::myObservations;
QMap<int, QPointer<Reading> > ProjectExchanger::myReadings;
QMap<int, QPointer<FeatureWrapper> > ProjectExchanger::myStations;
QMap<int, QPointer<FeatureWrapper> > ProjectExchanger::myCoordinateSystems;
QMap<int, QPointer<FeatureWrapper> > ProjectExchanger::myTransformationParameters;
QMap<int, QPointer<FeatureWrapper> > ProjectExchanger::myGeometries;
QList<int> ProjectExchanger::stationPoints;
QMap<QString, MeasurementConfig> ProjectExchanger::myMConfigs;
QMap<QString, SensorConfiguration> ProjectExchanger::mySConfigs;
QPointer<MeasurementConfigManager> ProjectExchanger::mConfigManager;

/*!
 * \brief ProjectExchanger::saveProject create XML-Document, "saves" the project / job to XML and compute a digest
 * \param job
 * \return
 */
QDomDocument ProjectExchanger::saveProject(const QPointer<OiJob> &job){

    QDomDocument project;

    //check job
    if(job.isNull()){
        Console::getInstance()->addLine("No project available", eErrorMessage);
        return project;
    }

    //get current date and time
    QString dateTime = QDateTime::currentDateTime().toString(Qt::ISODate);

    //create XML root, set general project attributes later
    QDomElement root = project.createElement("oiProjectData");
    project.appendChild(root);

    //add active station and active coordinate system
    QDomElement activeSystem = project.createElement("activeCoordinateSystem");
    QDomElement activeStation = project.createElement("activeStation");
    int activeSystemId, activeStationId = -1;
    if(!job->getActiveCoordinateSystem().isNull()){
        activeSystemId = job->getActiveCoordinateSystem()->getId();
    }
    if(!job->getActiveStation().isNull()){
        activeStationId = job->getActiveStation()->getId();
    }
    activeSystem.setAttribute("ref", activeSystemId);
    activeStation.setAttribute("ref", activeStationId);
    root.appendChild(activeSystem);
    root.appendChild(activeStation);

    //add stations
    QDomElement stations = project.createElement("stations");
    foreach(const QPointer<Station> &s, job->getStationsList()){
        if(!s.isNull()){
            QDomElement station = s->toOpenIndyXML(project);
            if(!station.isNull()){
                stations.appendChild(station);
            }
        }
    }
    root.appendChild(stations);

    //add coordinate systems (nominal + station systems)
    QDomElement coordinateSystems = project.createElement("coordinateSystems");
    foreach(const QPointer<CoordinateSystem> &c, job->getCoordinateSystemsList()){
        if(!c.isNull()){
            QDomElement coordinateSystem = c->toOpenIndyXML(project);
            if(!coordinateSystem.isNull()){
                coordinateSystems.appendChild(coordinateSystem);
            }
        }
    }
    foreach(const QPointer<Station> &s, job->getStationsList()){
        if(!s.isNull() && !s->getCoordinateSystem().isNull()){
            QDomElement coordinateSystem = s->getCoordinateSystem()->toOpenIndyXML(project);
            if(!coordinateSystem.isNull()){
                coordinateSystems.appendChild(coordinateSystem);
            }
        }
    }
    root.appendChild(coordinateSystems);

    //add trafo params
    QDomElement trafoParams = project.createElement("transformationParameters");
    foreach(const QPointer<TrafoParam> &tp, job->getTransformationParametersList()){
        if(!tp.isNull()){
            QDomElement trafoParam = tp->toOpenIndyXML(project);
            if(!trafoParam.isNull()){
                trafoParams.appendChild(trafoParam);
            }
        }
    }
    root.appendChild(trafoParams);

    //add geometries
    QDomElement geometries = project.createElement("geometries");
    foreach(const QPointer<FeatureWrapper> &geom, job->getGeometriesList()){ //normal geometries
        if(!geom.isNull() && !geom->getGeometry().isNull()){
            QDomElement geometry = geom->getGeometry()->toOpenIndyXML(project);
            if(!geometry.isNull()){
                geometries.appendChild(geometry);
            }
        }
    }
    foreach(const QPointer<Station> &s, job->getStationsList()){ //station points
        if(!s.isNull() && !s->getPosition().isNull()){
            QDomElement geometry = s->getPosition()->toOpenIndyXML(project);
            if(!geometry.isNull()){
                geometries.appendChild(geometry);
            }
        }
    }
    root.appendChild(geometries);

    //add observations (search the station coordinate systems for observations)
    QDomElement observations = project.createElement("observations");
    foreach(const QPointer<Station> &s, job->getStationsList()){
        if(!s.isNull() && !s->getCoordinateSystem().isNull() && s->getCoordinateSystem()->getObservations().size() > 0){
            foreach(const QPointer<Observation> &obs, s->getCoordinateSystem()->getObservations()){
                if(!obs.isNull()){
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
    // QDomElement sensorConfigs = project.createElement("sensorConfigs");

    //add measurement configs
    QList<MeasurementConfig> mConfigs;
    if(!ProjectExchanger::mConfigManager.isNull()){
        mConfigs = ProjectExchanger::mConfigManager->getUserConfigs();
        mConfigs.append(ProjectExchanger::mConfigManager->getProjectConfigs());
    }
    foreach(const MeasurementConfig &mConfig, mConfigs){
        QDomElement config = mConfig.toOpenIndyXML(project);
        if(!config.isNull()){
            measurementConfigs.appendChild(config);
        }
    }
    configs.appendChild(measurementConfigs);

    //add sensor configs
    /*QList<SensorConfiguration> sConfigs = OiConfigState::getAllSensorConfigs();
    foreach(const SensorConfiguration &sConfig, sConfigs){
        QDomElement config = sConfig.toOpenIndyXML(project);
        if(!config.isNull()){
            sensorConfigs.appendChild(config);
        }
    }
    configs.appendChild(sensorConfigs);*/

    root.appendChild(configs);

    // create hash / digest over xml with out oiProjectData Attributes
    QByteArray arr;
    QTextStream stream(&arr);
    project.save(stream, 4 /*indent*/);
    stream.flush();
    QCryptographicHash hash(QCryptographicHash::Sha256);
    hash.addData(arr);
    QString digest = hash.result().toHex();

    // set hash / digest
    job->setDigest(digest);

    // set general project attributes
    root.setAttribute("name", job->getJobName());
    root.setAttribute("date", dateTime);
    root.setAttribute("idCount", QString::number(job->generateUniqueId()));
    root.setAttribute("version", QString(OPENINDY_VERSION));
    root.setAttribute("digest", digest);

    return project;

}

/*!
 * \brief ProjectExchanger::loadProject
 * \param project
 * \return
 */
const QPointer<OiJob> ProjectExchanger::loadProject(const QDomDocument &project){

    QPointer<OiJob> job = new OiJob();

    //load all elements from xml into helper lists
    if(!ProjectExchanger::loadObservations(project)
            || !ProjectExchanger::loadStations(project)
            || !ProjectExchanger::loadCoordinateSystems(project)
            || !ProjectExchanger::loadTransformationParameters(project)
            || !ProjectExchanger::loadGeometries(project)
            || !ProjectExchanger::loadConfigs(project)){

        //clear all created elements if an error occured in one of the loading helpers (e.g. no station available)
        ProjectExchanger::clearHelperMaps(true);
        return NULL;

    }

    //restore dependencies between the elements
    if(!ProjectExchanger::restoreObservationDependencies(project)
            || !ProjectExchanger::restoreStationDependencies(project)
            || !ProjectExchanger::restoreCoordinateSystemDependencies(project)
            || !ProjectExchanger::restoreTrafoParamDependencies(project)
            || !ProjectExchanger::restoreGeometryDependencies(project)){

        //clear all created elements if an error occured in one of the loading helpers (e.g. no station available)
        ProjectExchanger::clearHelperMaps(true);
        return NULL;

    }

    //remove station points
    foreach(const QPointer<FeatureWrapper> &station, ProjectExchanger::myStations){
        if(!station.isNull() && !station->getStation().isNull()){
            if(!station->getStation()->getPosition().isNull()){
                ProjectExchanger::myGeometries.remove(station->getStation()->getPosition()->getId());
            }
        }
    }

    //add features to the job
    job->addFeaturesFromXml(ProjectExchanger::myStations.values());
    job->addFeaturesFromXml(ProjectExchanger::myCoordinateSystems.values());
    job->addFeaturesFromXml(ProjectExchanger::myTransformationParameters.values());
    job->addFeaturesFromXml(ProjectExchanger::myGeometries.values());

    //set id count
    if(project.documentElement().hasAttribute("idCount")){
        job->nextId = project.documentElement().attribute("idCount").toInt() + 1;
    }

    if(project.documentElement().hasAttribute("digest")){
        job->setDigest(project.documentElement().attribute("digest"));
    } else { // not available, then compute
        // compute digest and store digest in job
        ProjectExchanger::saveProject(job);
    }

    if(project.documentElement().hasAttribute("version")) {
        job->setLoadedProjectVersion(project.documentElement().attribute("version"));
    }

    //add project measurement configs to config manager
    if(!ProjectExchanger::mConfigManager.isNull()){
        foreach(const MeasurementConfig &mConfig, ProjectExchanger::myMConfigs.values()){
            if( mConfig.getIsValid()
                && mConfig.isProjectConfig()
                && !ProjectExchanger::mConfigManager->isStandardConfig(mConfig.getName())
                ){
                ProjectExchanger::mConfigManager->addProjectConfig(mConfig);
            }
        }
    }

    //set active station and active coordinate system
    QDomElement activeStation = project.documentElement().firstChildElement("activeStation");
    QDomElement activeCoordinateSystem = project.documentElement().firstChildElement("activeCoordinateSystem");
    if(activeStation.isNull() || activeCoordinateSystem.isNull()
            || !activeStation.hasAttribute("ref") || !activeCoordinateSystem.hasAttribute("ref")
            || !ProjectExchanger::myStations.contains(activeStation.attribute("ref").toInt())
            || !ProjectExchanger::myCoordinateSystems.contains(activeCoordinateSystem.attribute("ref").toInt())){

        //clear all created elements if no active station or active coordinate system is available
        //OiProjectExchanger::clearHelperMaps(true);
        //return NULL;

    }
    QPointer<FeatureWrapper> myActiveStation = ProjectExchanger::myStations.value(activeStation.attribute("ref").toInt());
    QPointer<FeatureWrapper> myActiveSystem = ProjectExchanger::myCoordinateSystems.value(activeCoordinateSystem.attribute("ref").toInt());
    myActiveStation->getStation()->setActiveStationState(true);
    myActiveSystem->getCoordinateSystem()->setActiveCoordinateSystemState(true);

    //clear the helper maps
    ProjectExchanger::clearHelperMaps(false);

    return job;

}

/*!
 * \brief ProjectExchanger::getMeasurementConfigManager
 * \return
 */
QPointer<MeasurementConfigManager> &ProjectExchanger::getMeasurementConfigManager(){
    return ProjectExchanger::mConfigManager;
}

/*!
 * \brief ProjectExchanger::setMeasurementConfigManager
 * \param mConfigManager
 */
void ProjectExchanger::setMeasurementConfigManager(const QPointer<MeasurementConfigManager> &mConfigManager){
    ProjectExchanger::mConfigManager = mConfigManager;
}

/*!
 * \brief ProjectExchanger::loadObservations
 * \param project
 * \return
 */
bool ProjectExchanger::loadObservations(const QDomDocument &project){

    QDomElement observations = project.documentElement().firstChildElement("observations");
    if(!observations.isNull()){

        //get a list of all observations tags
        QDomNodeList observationList = observations.childNodes();

        for(int i = 0; i < observationList.size(); i++){

            //for each observations tag create an observation object and try to parse xml
            QPointer<Observation> obs = new Observation();
            QDomElement obsTag = observationList.at(i).toElement();
            if(!obs->fromOpenIndyXML(obsTag)){
                delete obs;
                continue;
            }

            //add the observation to the list of successfully loaded observations
            ProjectExchanger::myObservations.insert(obs->getId(), obs);
            ProjectExchanger::myReadings.insert(obs->getReading()->getId(), obs->getReading());

        }

    }

    return true;

}

/*!
 * \brief ProjectExchanger::loadStations
 * \param project
 * \return
 */
bool ProjectExchanger::loadStations(const QDomDocument &project){

    QDomElement stations = project.documentElement().firstChildElement("stations");

    //at least one station has to be available
    if(stations.isNull()){
        return false;
    }

    //get a list of all station tags
    QDomNodeList stationList = stations.childNodes();

    for(int i = 0; i < stationList.size(); i++){

        //for each station tag create a station object and try to parse xml
        QPointer<Station> station = new Station();
        QDomElement stationTag = stationList.at(i).toElement();
        if(!station->fromOpenIndyXML(stationTag)){
            delete station;
            continue;
        }

        //create a feature wrapper for the station
        QPointer<FeatureWrapper> stationWrapper = new FeatureWrapper();
        stationWrapper->setStation(station);

        //add the station to the list of successfully loaded stations
        ProjectExchanger::myStations.insert(station->getId(), stationWrapper);

    }

    //at least one station has to be available
    if(ProjectExchanger::myStations.size() == 0){
        return false;
    }

    return true;

}

/*!
 * \brief ProjectExchanger::loadCoordinateSystems
 * \param project
 * \return
 */
bool ProjectExchanger::loadCoordinateSystems(const QDomDocument &project){

    QDomElement coordinateSystems = project.documentElement().firstChildElement("coordinateSystems");

    //at least one coordinate system has to be available
    if(coordinateSystems.isNull()){
        return false;
    }

    //get a list of all coordinate system tags
    QDomNodeList coordinateSystemList = coordinateSystems.childNodes();

    for(int i = 0; i < coordinateSystemList.size(); i++){

        //for each coordinate system tag create a coordinate system object and try to parse xml
        QPointer<CoordinateSystem> coordinateSystem = new CoordinateSystem();
        QDomElement coordTag = coordinateSystemList.at(i).toElement();
        if(!coordinateSystem->fromOpenIndyXML(coordTag)){
            delete coordinateSystem;
            continue;
        }

        //create a feature wrapper for the station
        QPointer<FeatureWrapper> coordinateSystemWrapper = new FeatureWrapper();
        coordinateSystemWrapper->setCoordinateSystem(coordinateSystem);

        //add the coordinate system to the list of successfully loaded coordinate systems
        ProjectExchanger::myCoordinateSystems.insert(coordinateSystem->getId(), coordinateSystemWrapper);

    }

    //at least one coordinate system has to be available
    if(ProjectExchanger::myCoordinateSystems.size() == 0){
        return false;
    }

    return true;

}

/*!
 * \brief ProjectExchanger::loadTransformationParameters
 * \param project
 * \return
 */
bool ProjectExchanger::loadTransformationParameters(const QDomDocument &project){

    QDomElement trafoParams = project.documentElement().firstChildElement("transformationParameters");
    if(!trafoParams.isNull()){

        //get a list of all trafo param tags
        QDomNodeList trafoParamList = trafoParams.childNodes();

        for(int i = 0; i < trafoParamList.size(); i++){

            //for each trafo param tag create a trafo param object and try to parse xml
            QPointer<TrafoParam> trafoParam = new TrafoParam();
            QDomElement trafoTag = trafoParamList.at(i).toElement();
            if(!trafoParam->fromOpenIndyXML(trafoTag)){
                delete trafoParam;
                continue;
            }

            //create a feature wrapper for the station
            QPointer<FeatureWrapper> trafoParamWrapper = new FeatureWrapper();
            trafoParamWrapper->setTrafoParam(trafoParam);

            //add the trafo param to the list of successfully loaded trafo params
            ProjectExchanger::myTransformationParameters.insert(trafoParam->getId(), trafoParamWrapper);

        }

    }

    return true;

}

/*!
 * \brief ProjectExchanger::loadGeometries
 * \param project
 * \return
 */
bool ProjectExchanger::loadGeometries(const QDomDocument &project){

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
            FeatureTypes geomType = getFeatureTypeEnum(geometry.attribute("type"));

            //create a feature wrapper object to hold the geometry
            QPointer<FeatureWrapper> myFeatureWrapper;
            myFeatureWrapper = new FeatureWrapper();

            //depending on the type of geometry call the right fromOpenIndyXML method
            switch(geomType){
            case eCircleFeature:{
                QPointer<Circle> myCircle = new Circle(false);
                myFeatureWrapper->setCircle(myCircle);
                if(!myCircle->fromOpenIndyXML(geometry)){
                    delete myCircle;
                    delete myFeatureWrapper;
                    continue;
                }
                break;
            }case eConeFeature:{
                QPointer<Cone> myCone = new Cone(false);
                myFeatureWrapper->setCone(myCone);
                if(!myCone->fromOpenIndyXML(geometry)){
                    delete myCone;
                    delete myFeatureWrapper;
                    continue;
                }
                break;
            }case eCylinderFeature:{
                QPointer<Cylinder> myCylinder = new Cylinder(false);
                myFeatureWrapper->setCylinder(myCylinder);
                if(!myCylinder->fromOpenIndyXML(geometry)){
                    delete myCylinder;
                    delete myFeatureWrapper;
                    continue;
                }
                break;
            }case eEllipseFeature:{
                QPointer<Ellipse> myEllipse = new Ellipse(false);
                myFeatureWrapper->setEllipse(myEllipse);
                if(!myEllipse->fromOpenIndyXML(geometry)){
                    delete myEllipse;
                    delete myFeatureWrapper;
                    continue;
                }
                break;
            }case eEllipsoidFeature:{
                QPointer<Ellipsoid> myEllipsoid = new Ellipsoid(false);
                myFeatureWrapper->setEllipsoid(myEllipsoid);
                if(!myEllipsoid->fromOpenIndyXML(geometry)){
                    delete myEllipsoid;
                    delete myFeatureWrapper;
                    continue;
                }
                break;
            }case eHyperboloidFeature:{
                QPointer<Hyperboloid> myHyperboloid = new Hyperboloid(false);
                myFeatureWrapper->setHyperboloid(myHyperboloid);
                if(!myHyperboloid->fromOpenIndyXML(geometry)){
                    delete myHyperboloid;
                    delete myFeatureWrapper;
                    continue;
                }
                break;
            }case eLineFeature:{
                QPointer<Line> myLine = new Line(false);
                myFeatureWrapper->setLine(myLine);
                if(!myLine->fromOpenIndyXML(geometry)){
                    delete myLine;
                    delete myFeatureWrapper;
                    continue;
                }
                break;
            }case eNurbsFeature:{
                QPointer<Nurbs> myNurbs = new Nurbs(false);
                myFeatureWrapper->setNurbs(myNurbs);
                if(!myNurbs->fromOpenIndyXML(geometry)){
                    delete myNurbs;
                    delete myFeatureWrapper;
                    continue;
                }
                break;
            }case eParaboloidFeature:{
                QPointer<Paraboloid> myParaboloid = new Paraboloid(false);
                myFeatureWrapper->setParaboloid(myParaboloid);
                if(!myParaboloid->fromOpenIndyXML(geometry)){
                    delete myParaboloid;
                    delete myFeatureWrapper;
                    continue;
                }
                break;
            }case ePlaneFeature:{
                QPointer<Plane> myPlane = new Plane(false);
                myFeatureWrapper->setPlane(myPlane);
                if(!myPlane->fromOpenIndyXML(geometry)){
                    delete myPlane;
                    delete myFeatureWrapper;
                    continue;
                }
                break;
            }case ePointFeature:{
                QPointer<Point> myPoint = new Point(false);
                myFeatureWrapper->setPoint(myPoint);
                if(!myPoint->fromOpenIndyXML(geometry)){
                    delete myPoint;
                    delete myFeatureWrapper;
                    continue;
                }
                break;
            }case ePointCloudFeature:{
                QPointer<PointCloud> myPointCloud = new PointCloud(false);
                myFeatureWrapper->setPointCloud(myPointCloud);
                if(!myPointCloud->fromOpenIndyXML(geometry)){
                    delete myPointCloud;
                    delete myFeatureWrapper;
                    continue;
                }
                break;
            }case eScalarEntityAngleFeature:{
                QPointer<ScalarEntityAngle> myAngle = new ScalarEntityAngle(false);
                myFeatureWrapper->setScalarEntityAngle(myAngle);
                if(!myAngle->fromOpenIndyXML(geometry)){
                    delete myAngle;
                    delete myFeatureWrapper;
                    continue;
                }
                break;
            }case eScalarEntityDistanceFeature:{
                QPointer<ScalarEntityDistance> myDistance = new ScalarEntityDistance(false);
                myFeatureWrapper->setScalarEntityDistance(myDistance);
                if(!myDistance->fromOpenIndyXML(geometry)){
                    delete myDistance;
                    delete myFeatureWrapper;
                    continue;
                }
                break;
            }case eScalarEntityMeasurementSeriesFeature:{
                QPointer<ScalarEntityMeasurementSeries> myMeasurementSeries = new ScalarEntityMeasurementSeries(false);
                myFeatureWrapper->setScalarEntityMeasurementSeries(myMeasurementSeries);
                if(!myMeasurementSeries->fromOpenIndyXML(geometry)){
                    delete myMeasurementSeries;
                    delete myFeatureWrapper;
                    continue;
                }
                break;
            }case eScalarEntityTemperatureFeature:{
                QPointer<ScalarEntityTemperature> myTemperature = new ScalarEntityTemperature(false);
                myFeatureWrapper->setScalarEntityTemperature(myTemperature);
                if(!myTemperature->fromOpenIndyXML(geometry)){
                    delete myTemperature;
                    delete myFeatureWrapper;
                    continue;
                }
                break;
            }case eSlottedHoleFeature:{
                QPointer<SlottedHole> mySlottedHole = new SlottedHole(false);
                myFeatureWrapper->setSlottedHole(mySlottedHole);
                if(!mySlottedHole->fromOpenIndyXML(geometry)){
                    delete mySlottedHole;
                    delete myFeatureWrapper;
                    continue;
                }
                break;
            }case eSphereFeature:{
                QPointer<Sphere> mySphere = new Sphere(false);
                myFeatureWrapper->setSphere(mySphere);
                if(!mySphere->fromOpenIndyXML(geometry)){
                    delete mySphere;
                    delete myFeatureWrapper;
                    continue;
                }
                break;
            }case eTorusFeature:{
                QPointer<Torus> myTorus = new Torus(false);
                myFeatureWrapper->setTorus(myTorus);
                if(!myTorus->fromOpenIndyXML(geometry)){
                    delete myTorus;
                    delete myFeatureWrapper;
                    continue;
                }
                break;
            }default:{
                delete myFeatureWrapper;
                continue;
            }}

            //add the feature wrapper to the list of successfully loaded geometries
            ProjectExchanger::myGeometries.insert(myFeatureWrapper->getFeature()->getId(), myFeatureWrapper);

        }

    }

    return true;

}

/*!
 * \brief ProjectExchanger::loadConfigs
 * \param project
 * \return
 */
bool ProjectExchanger::loadConfigs(const QDomDocument &project){

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
                    mConfig.isUserConfig(!mConfigManager.isNull() && mConfigManager->isUserConfig(mConfig));
                    ProjectExchanger::myMConfigs.insert(mConfig.getName(), mConfig);
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
                    ProjectExchanger::mySConfigs.insert(sConfig.getName(), sConfig);
                }
            }
        }

    }

    return true;

}

/*!
 * \brief ProjectExchanger::restoreStationDependencies
 * \param project
 * \return
 */
bool ProjectExchanger::restoreStationDependencies(const QDomDocument &project){

    bool result = true;

    //iterate through the station tags in the xml project
    QDomElement stations = project.documentElement().firstChildElement("stations");
    if(!stations.isNull()){
        QDomNodeList stationList = stations.childNodes();
        for(int i = 0; i < stationList.size(); i++){

            //get the station tag at position i and the corresponding FeatureWrapper
            QDomElement station = stationList.at(i).toElement();
            if(!station.hasAttribute("id") || !ProjectExchanger::myStations.contains(station.attribute("id").toInt())){
                continue;
            }
            QPointer<FeatureWrapper> myStation = ProjectExchanger::myStations.value(station.attribute("id").toInt());
            if(myStation.isNull() || myStation->getStation().isNull()){
                continue;
            }

            result = true;

            //load used sensor plugins
            QDomElement usedSensors = station.firstChildElement("usedSensors");
            if(!usedSensors.isNull()){
                QDomNodeList usedSensorsList = usedSensors.childNodes();
                QList<Sensor> myUsedSensors;
                for(int j = 0; j < usedSensorsList.size(); j++){
                    QDomElement usedSensor = usedSensorsList.at(j).toElement();
                    if(usedSensor.hasAttribute("name") && usedSensor.hasAttribute("plugin")){
                        sdb::Plugin plugin = SystemDbManager::getPlugin(usedSensor.attribute("plugin"));
                        if(plugin.file_path.compare("") != 0){
                            QPointer<Sensor> mySensor = PluginLoader::loadSensorPlugin(plugin.file_path, usedSensor.attribute("name"));
                            if(!mySensor.isNull()){
                                myUsedSensors.append(Sensor(*mySensor.data()));
                                delete mySensor;
                            }
                        }
                    }
                }
                myStation->getStation()->setUsedSensors(myUsedSensors);
            }

            //load active sensor plugin
            QDomElement activeSensor = station.firstChildElement("activeSensor");
            if(!activeSensor.isNull() && activeSensor.hasAttribute("name") && activeSensor.hasAttribute("plugin")){
                sdb::Plugin plugin = SystemDbManager::getPlugin(activeSensor.attribute("plugin"));
                if(plugin.file_path.compare("") != 0){
                    QPointer<Sensor> mySensor = PluginLoader::loadSensorPlugin(plugin.file_path, activeSensor.attribute("name"));
                    if(!mySensor.isNull()){
                        myStation->getStation()->setSensor(mySensor);
                    }
                }
            }

            //set position point
            QDomElement position = station.firstChildElement("position");
            if(!position.isNull() && position.hasAttribute("ref") && ProjectExchanger::myGeometries.contains(position.attribute("ref").toInt())){
                QPointer<FeatureWrapper> myPosition = ProjectExchanger::myGeometries.value(position.attribute("ref").toInt());
                if(!myPosition.isNull() && !myPosition->getPoint().isNull()){
                    myStation->getStation()->position = myPosition->getPoint();
                }
            }

            //set coordinate system
            QDomElement coordinateSystem = station.firstChildElement("coordinateSystem");
            if(!coordinateSystem.isNull() && coordinateSystem.hasAttribute("ref")){
                QPointer<FeatureWrapper> mySystem = ProjectExchanger::myCoordinateSystems.value(coordinateSystem.attribute("ref").toInt());
                if(!mySystem.isNull() && !mySystem->getCoordinateSystem().isNull()){
                    myStation->getStation()->stationSystem = mySystem->getCoordinateSystem();
                    myStation->getStation()->stationSystem->isStationSystem = true;
                    mySystem->getCoordinateSystem()->station = myStation->getStation();
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
                        QPointer<FeatureWrapper> myFeature(NULL);
                        if(ProjectExchanger::myGeometries.contains(feature.attribute("ref").toInt())){
                            myFeature = ProjectExchanger::myGeometries.value(feature.attribute("ref").toInt());
                        }else if(ProjectExchanger::myCoordinateSystems.contains(feature.attribute("ref").toInt())){
                            myFeature = ProjectExchanger::myCoordinateSystems.value(feature.attribute("ref").toInt());
                        }else if(ProjectExchanger::myTransformationParameters.contains(feature.attribute("ref").toInt())){
                            myFeature = ProjectExchanger::myTransformationParameters.value(feature.attribute("ref").toInt());
                        }
                        if(!myFeature.isNull()){
                            myStation->getStation()->addUsedFor(myFeature);
                        }
                    }
                }
            }

        }
    }

    return result;

}

/*!
 * \brief ProjectExchanger::restoreCoordinateSystemDependencies
 * \param project
 * \return
 */
bool ProjectExchanger::restoreCoordinateSystemDependencies(const QDomDocument &project){

    bool result = true;

    //iterate through the coordinate system tags in the xml project
    QDomElement coordinateSystems = project.documentElement().firstChildElement("coordinateSystems");
    if(!coordinateSystems.isNull()){
        QDomNodeList coordinateSystemList = coordinateSystems.childNodes();
        for(int i = 0; i < coordinateSystemList.size(); i++){

            //get the coordinate system tag at position i and the corresponding FeatureWrapper
            QDomElement coordinateSystem = coordinateSystemList.at(i).toElement();
            if(!coordinateSystem.hasAttribute("id") || !ProjectExchanger::myCoordinateSystems.contains(coordinateSystem.attribute("id").toInt())){
                continue;
            }
            QPointer<FeatureWrapper> myCoordinateSystem = ProjectExchanger::myCoordinateSystems.value(coordinateSystem.attribute("id").toInt());
            if(myCoordinateSystem.isNull() || myCoordinateSystem->getCoordinateSystem().isNull()){
                continue;
            }

            result = true;

            //load function plugins
            QDomElement functions = coordinateSystem.firstChildElement("functions");
            if(!functions.isNull()){
                QList<QPointer<Function> > myFunctions = ProjectExchanger::restoreFunctionDependencies(functions);
                for(int j = 0; j < myFunctions.size(); j++){
                    myCoordinateSystem->getCoordinateSystem()->addFunction(myFunctions.at(j));
                }
            }

            //load bundle adjustments
            QDomElement bundle_plugin = coordinateSystem.firstChildElement("bundle");
            if(!bundle_plugin.isNull()){
                QPointer<BundleAdjustment> myBundle = ProjectExchanger::restoreBundleDependencies(bundle_plugin);
                if(!myBundle.isNull()){
                    myCoordinateSystem->getCoordinateSystem()->setBundlePlugin(myBundle);
                }
            }

            //set observations (made from a station system)
            QDomElement observations = coordinateSystem.firstChildElement("observations");
            if(!observations.isNull()){
                QDomNodeList observationList = observations.childNodes();
                for(int j = 0; j < observationList.size(); j++){
                    QDomElement observation = observationList.at(j).toElement();
                    if(observation.hasAttribute("ref") && ProjectExchanger::myObservations.contains(observation.attribute("ref").toInt())){
                        QPointer<Observation> myObservation = ProjectExchanger::myObservations.value(observation.attribute("ref").toInt());
                        myCoordinateSystem->getCoordinateSystem()->observationsList.append(myObservation);
                        myCoordinateSystem->getCoordinateSystem()->observationsMap.insert(myObservation->getId(), myObservation);
                    }
                }
            }

            //set nominal geometries
            QDomElement nominalGeometries = coordinateSystem.firstChildElement("nominalGeometries");
            if(!nominalGeometries.isNull()){
                QDomNodeList geometryList = nominalGeometries.childNodes();
                for(int j = 0; j < geometryList.size(); j++){
                    QDomElement geometry = geometryList.at(j).toElement();
                    if(geometry.hasAttribute("ref") && ProjectExchanger::myGeometries.contains(geometry.attribute("ref").toInt())){
                        QPointer<FeatureWrapper> myGeometry = ProjectExchanger::myGeometries.value(geometry.attribute("ref").toInt());
                        if(myGeometry->getGeometry()->getIsNominal()){
                            myCoordinateSystem->getCoordinateSystem()->nominalsList.append(myGeometry);
                            myCoordinateSystem->getCoordinateSystem()->nominalsMap.insert(myGeometry->getGeometry()->getId(), myGeometry);
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

                        if(ProjectExchanger::myGeometries.contains(feature.attribute("ref").toInt())){
                            myCoordinateSystem->getCoordinateSystem()->addUsedFor(ProjectExchanger::myGeometries.value(feature.attribute("ref").toInt()));
                        }else if(ProjectExchanger::myCoordinateSystems.contains(feature.attribute("ref").toInt())){
                            myCoordinateSystem->getCoordinateSystem()->addUsedFor(ProjectExchanger::myCoordinateSystems.value(feature.attribute("ref").toInt()));
                        }else if(ProjectExchanger::myTransformationParameters.contains(feature.attribute("ref").toInt())){
                            myCoordinateSystem->getCoordinateSystem()->addUsedFor(ProjectExchanger::myTransformationParameters.value(feature.attribute("ref").toInt()));
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

                        if(ProjectExchanger::myGeometries.contains(feature.attribute("ref").toInt())){
                            myCoordinateSystem->getCoordinateSystem()->addPreviouslyNeeded(ProjectExchanger::myGeometries.value(feature.attribute("ref").toInt()));
                        }else if(ProjectExchanger::myCoordinateSystems.contains(feature.attribute("ref").toInt())){
                            myCoordinateSystem->getCoordinateSystem()->addPreviouslyNeeded(ProjectExchanger::myCoordinateSystems.value(feature.attribute("ref").toInt()));
                        }else if(ProjectExchanger::myTransformationParameters.contains(feature.attribute("ref").toInt())){
                            myCoordinateSystem->getCoordinateSystem()->addPreviouslyNeeded(ProjectExchanger::myTransformationParameters.value(feature.attribute("ref").toInt()));
                        }else if(ProjectExchanger::myStations.contains(feature.attribute("ref").toInt())){
                            myCoordinateSystem->getCoordinateSystem()->addPreviouslyNeeded(ProjectExchanger::myStations.value(feature.attribute("ref").toInt()));
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
                    if(trafoParam.hasAttribute("ref") && ProjectExchanger::myTransformationParameters.contains(trafoParam.attribute("ref").toInt())){
                        QPointer<FeatureWrapper> myTrafoParam = ProjectExchanger::myTransformationParameters.value(trafoParam.attribute("ref").toInt());
                        //set the dependency for bundle system and trafoParam
                        myCoordinateSystem->getCoordinateSystem()->addTransformationParameter(myTrafoParam->getTrafoParam());
                    }
                }
            }
        }
    }

    return result;

}

/*!
 * \brief ProjectExchanger::restoreTrafoParamDependencies
 * \param project
 * \return
 */
bool ProjectExchanger::restoreTrafoParamDependencies(const QDomDocument &project){

    bool result = true;

    //iterate through the trafo param tags in the xml project
    QDomElement trafoParams = project.documentElement().firstChildElement("transformationParameters");
    if(!trafoParams.isNull()){
        QDomNodeList trafoParamList = trafoParams.childNodes();
        for(int i = 0; i < trafoParamList.size(); i++){

            //get the trafo param tag at position i and the corresponding FeatureWrapper
            QDomElement trafoParam = trafoParamList.at(i).toElement();
            if(!trafoParam.hasAttribute("id") || !ProjectExchanger::myTransformationParameters.contains(trafoParam.attribute("id").toInt())){
                continue;
            }
            QPointer<FeatureWrapper> myTrafoParam = ProjectExchanger::myTransformationParameters.value(trafoParam.attribute("id").toInt());
            if(myTrafoParam.isNull() || myTrafoParam->getTrafoParam().isNull()){
                continue;
            }

            //set start and destination system
            QDomElement from = trafoParam.firstChildElement("from");
            QDomElement to = trafoParam.firstChildElement("to");
            if(from.isNull() || to.isNull() || !from.hasAttribute("ref") || !to.hasAttribute("ref")
                    || !ProjectExchanger::myCoordinateSystems.contains(from.attribute("ref").toInt())
                    || !ProjectExchanger::myCoordinateSystems.contains(from.attribute("ref").toInt())){
                continue;
            }
            myTrafoParam->getTrafoParam()->setCoordinateSystems(ProjectExchanger::myCoordinateSystems.value(from.attribute("ref").toInt())->getCoordinateSystem(),
                                                                ProjectExchanger::myCoordinateSystems.value(to.attribute("ref").toInt())->getCoordinateSystem());

            //load function plugins
            QDomElement functions = trafoParam.firstChildElement("functions");
            if(!functions.isNull()){
                QList<QPointer<Function> > myFunctions = ProjectExchanger::restoreFunctionDependencies(functions);
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

                        if(ProjectExchanger::myGeometries.contains(feature.attribute("ref").toInt())){
                            myTrafoParam->getTrafoParam()->addUsedFor(ProjectExchanger::myGeometries.value(feature.attribute("ref").toInt()));
                        }else if(ProjectExchanger::myCoordinateSystems.contains(feature.attribute("ref").toInt())){
                            myTrafoParam->getTrafoParam()->addUsedFor(ProjectExchanger::myCoordinateSystems.value(feature.attribute("ref").toInt()));
                        }else if(ProjectExchanger::myTransformationParameters.contains(feature.attribute("ref").toInt())){
                            myTrafoParam->getTrafoParam()->addUsedFor(ProjectExchanger::myTransformationParameters.value(feature.attribute("ref").toInt()));
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

                        if(ProjectExchanger::myGeometries.contains(feature.attribute("ref").toInt())){
                            myTrafoParam->getTrafoParam()->addPreviouslyNeeded(ProjectExchanger::myGeometries.value(feature.attribute("ref").toInt()));
                        }else if(ProjectExchanger::myCoordinateSystems.contains(feature.attribute("ref").toInt())){
                            myTrafoParam->getTrafoParam()->addPreviouslyNeeded(ProjectExchanger::myCoordinateSystems.value(feature.attribute("ref").toInt()));
                        }else if(ProjectExchanger::myTransformationParameters.contains(feature.attribute("ref").toInt())){
                            myTrafoParam->getTrafoParam()->addPreviouslyNeeded(ProjectExchanger::myTransformationParameters.value(feature.attribute("ref").toInt()));
                        }else if(ProjectExchanger::myStations.contains(feature.attribute("ref").toInt())){
                            myTrafoParam->getTrafoParam()->addPreviouslyNeeded(ProjectExchanger::myStations.value(feature.attribute("ref").toInt()));
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
 * \brief ProjectExchanger::restoreGeometryDependencies
 * \param project
 * \return
 */
bool ProjectExchanger::restoreGeometryDependencies(const QDomDocument &project){

    bool result = true;

    //iterate through the geometry tags in the xml project
    QDomElement geometries = project.documentElement().firstChildElement("geometries");
    if(!geometries.isNull()){
        QDomNodeList geometryList = geometries.childNodes();
        for(int i = 0; i < geometryList.size(); i++){

            //get the geometry tag at position i and the corresponding FeatureWrapper
            QDomElement geometry = geometryList.at(i).toElement();
            if(!geometry.hasAttribute("id") || !ProjectExchanger::myGeometries.contains(geometry.attribute("id").toInt())){
                continue;
            }
            QPointer<FeatureWrapper> myGeometry = ProjectExchanger::myGeometries.value(geometry.attribute("id").toInt());
            if(myGeometry.isNull() || myGeometry->getGeometry().isNull()){
                continue;
            }

            //set observations
            QDomElement observations = geometry.firstChildElement("observations");
            if(!observations.isNull()){
                QDomNodeList observationList = observations.childNodes();
                for(int j = 0; j < observationList.size(); j++){
                    QDomElement observation = observationList.at(j).toElement();
                    if(observation.hasAttribute("ref")){
                        QPointer<Observation> myObservation = ProjectExchanger::myObservations.value(observation.attribute("ref").toInt());
                        if(!myObservation.isNull()){
                            myGeometry->getGeometry()->addObservation(myObservation);
                        }
                    }
                }
            }

            //load function plugins
            QDomElement functions = geometry.firstChildElement("functions");
            if(!functions.isNull()){
                QList<QPointer<Function> > myFunctions = ProjectExchanger::restoreFunctionDependencies(functions);
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

                        if(ProjectExchanger::myGeometries.contains(feature.attribute("ref").toInt())){
                            myGeometry->getGeometry()->addUsedFor(ProjectExchanger::myGeometries.value(feature.attribute("ref").toInt()));
                        }else if(ProjectExchanger::myCoordinateSystems.contains(feature.attribute("ref").toInt())){
                            myGeometry->getGeometry()->addUsedFor(ProjectExchanger::myCoordinateSystems.value(feature.attribute("ref").toInt()));
                        }else if(ProjectExchanger::myTransformationParameters.contains(feature.attribute("ref").toInt())){
                            myGeometry->getGeometry()->addUsedFor(ProjectExchanger::myTransformationParameters.value(feature.attribute("ref").toInt()));
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

                        if(ProjectExchanger::myGeometries.contains(feature.attribute("ref").toInt())){
                            myGeometry->getGeometry()->addPreviouslyNeeded(ProjectExchanger::myGeometries.value(feature.attribute("ref").toInt()));
                        }else if(ProjectExchanger::myCoordinateSystems.contains(feature.attribute("ref").toInt())){
                            myGeometry->getGeometry()->addPreviouslyNeeded(ProjectExchanger::myCoordinateSystems.value(feature.attribute("ref").toInt()));
                        }else if(ProjectExchanger::myTransformationParameters.contains(feature.attribute("ref").toInt())){
                            myGeometry->getGeometry()->addPreviouslyNeeded(ProjectExchanger::myTransformationParameters.value(feature.attribute("ref").toInt()));
                        }else if(ProjectExchanger::myStations.contains(feature.attribute("ref").toInt())){
                            myGeometry->getGeometry()->addPreviouslyNeeded(ProjectExchanger::myStations.value(feature.attribute("ref").toInt()));
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
                    if(nominalGeometry.hasAttribute("ref") && ProjectExchanger::myGeometries.contains(nominalGeometry.attribute("ref").toInt())){
                        QPointer<FeatureWrapper> myNominalGeometry = ProjectExchanger::myGeometries.value(nominalGeometry.attribute("ref").toInt());
                        myGeometry->getGeometry()->addNominal(myNominalGeometry->getGeometry());
                    }
                }
            }

            //set actual
            QDomElement actualGeometry = geometry.firstChildElement("actual");
            if(!actualGeometry.isNull() && actualGeometry.hasAttribute("ref") && ProjectExchanger::myGeometries.contains(actualGeometry.attribute("ref").toInt())){
                QPointer<FeatureWrapper> myActualGeometry = ProjectExchanger::myGeometries.value(actualGeometry.attribute("ref").toInt());
                myGeometry->getGeometry()->setActual(myActualGeometry->getGeometry());
            }

            //set nominal system
            QDomElement nominalSystem = geometry.firstChildElement("nominalCoordinateSystem");
            if(!nominalSystem.isNull() && nominalSystem.hasAttribute("ref") && ProjectExchanger::myCoordinateSystems.contains(nominalSystem.attribute("ref").toInt())){
                QPointer<FeatureWrapper> myNominalSystem = ProjectExchanger::myCoordinateSystems.value(nominalSystem.attribute("ref").toInt());
                myGeometry->getGeometry()->setNominalSystem(myNominalSystem->getCoordinateSystem());
            }

            //set measurement configs
            QDomElement mConfigElement = geometry.firstChildElement("measurementConfig");
            const QVariant name = mConfigElement.attribute("name");
            if(!mConfigElement.isNull()&& mConfigElement.hasAttribute("name")){
                MeasurementConfig savedConfig = ProjectExchanger::mConfigManager->getUserConfig(name.toString());
                if(ProjectExchanger::mConfigManager->isStandardConfig(name.toString()) && savedConfig.getIsValid()) {
                    myGeometry->getGeometry()->setMeasurementConfig(savedConfig);
                } else if(ProjectExchanger::myMConfigs.contains(name.toString())) {// is not saved / user measurement config
                    MeasurementConfig mConfig = ProjectExchanger::myMConfigs.value(name.toString());
                    myGeometry->getGeometry()->setMeasurementConfig(mConfig);
                } else if(savedConfig.getIsValid()) {
                    myGeometry->getGeometry()->setMeasurementConfig(savedConfig);
                }
            }

        }
    }

    return result;

}

/*!
 * \brief ProjectExchanger::restoreObservationDependencies
 * \param project
 * \return
 */
bool ProjectExchanger::restoreObservationDependencies(const QDomDocument &project){

    bool result = true;

    //iterate through the observation tags in the xml project
    QDomElement observations = project.documentElement().firstChildElement("observations");
    if(!observations.isNull()){
        QDomNodeList observationList = observations.childNodes();
        for(int i = 0; i < observationList.size(); i++){

            //get the observation tag at position i and the corresponding Observation
            QDomElement observation = observationList.at(i).toElement();
            if(!observation.hasAttribute("id") || !ProjectExchanger::myObservations.contains(observation.attribute("id").toInt())){
                continue;
            }
            QPointer<Observation> myObservation = ProjectExchanger::myObservations.value(observation.attribute("id").toInt());
            if(myObservation.isNull() || myObservation->getReading().isNull()){
                continue;
            }

            //set station and sensor
            QDomElement station = observation.firstChildElement("station");
            if(!station.isNull() && station.hasAttribute("ref") && ProjectExchanger::myStations.contains(station.attribute("ref").toInt())){

                //get station and assign it to the observation
                QPointer<FeatureWrapper> myStation = ProjectExchanger::myStations.value(station.attribute("ref").toInt());
                myObservation->setStation(myStation->getStation());
                if(myStation->getStation()->getIsSensorSet()){
                    myObservation->getReading()->setSensorConfiguration(myStation->getStation()->getSensorConfiguration());
                }

                //assign the observation's reading to the station
                switch(myObservation->getReading()->getTypeOfReading()){
                case eCartesianReading:
                    myStation->getStation()->cartesianReadings.append(myObservation->getReading());
                    break;
                case ePolarReading:
                    myStation->getStation()->polarReadings.append(myObservation->getReading());
                    break;
                case eDirectionReading:
                    myStation->getStation()->directionReadings.append(myObservation->getReading());
                    break;
                case eDistanceReading:
                    myStation->getStation()->distanceReadings.append(myObservation->getReading());
                    break;
                case eLevelReading:
                    myStation->getStation()->levelReadings.append(myObservation->getReading());
                    break;
                case eTemperatureReading:
                    myStation->getStation()->temperatureRadings.append(myObservation->getReading());
                    break;
                }
            }

            //set measuredTargetGeometry
            QDomElement measuredTargetGeom = observation.firstChildElement("measuredTargetGeometry");
            if(!measuredTargetGeom.isNull() && measuredTargetGeom.hasAttribute("ref") && ProjectExchanger::myGeometries.contains(measuredTargetGeom.attribute("ref").toInt())){

                //get geometry and assign it
                QPointer<FeatureWrapper> myMeasuredTargetGeom = ProjectExchanger::myGeometries.value(measuredTargetGeom.attribute("ref").toInt());
                myObservation->setMeasuredTargetGeometry(myMeasuredTargetGeom->getGeometry());
            }

        }
    }

    return result;

}

/*!
 * \brief ProjectExchanger::restoreFunctionDependencies
 * Helper method which gets a functions tag as input and then returns a list of function objects ordered by their execution index
 * \param functions
 * \return
 */
QList<QPointer<Function> > ProjectExchanger::restoreFunctionDependencies(const QDomElement &functions){

    QList<QPointer<Function> > result;

    if(!functions.isNull()){

        //get a list of all function tags
        QDomNodeList functionList = functions.childNodes();

        //map to save all found functions with their executionIndex as key
        QMap<int, QPointer<Function> > functionOrder;

        for(int i = 0; i < functionList.size(); i++){

            //get a function tag
            QDomElement function = functionList.at(i).toElement();
            if(!function.hasAttribute("name") || !function.hasAttribute("plugin") || !function.hasAttribute("executionIndex")){
                continue;
            }

            //load function plugin
            sdb::Plugin plugin = SystemDbManager::getPlugin(function.attribute("plugin"));
            QPointer<Function> myFunction(NULL);
            if(plugin.file_path.compare("") != 0){
                myFunction = PluginLoader::loadFunctionPlugin(plugin.file_path, function.attribute("name"));
                if(myFunction.isNull()){
                    continue;
                }
            }else{
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

                    //get shouldBeUsed
                    bool shouldBeUsed = true;
                    if(inputElement.hasAttribute("shouldBeUsed")){
                        shouldBeUsed = inputElement.attribute("shouldBeUsed").toInt();
                    }

                    //set ignored destination parameters

                    //create the list of ignored parameters
                    QList<GeometryParameters> restoreIgnoredParameter;
                    //look for the ignored destination parameter section in the xml file
                    QDomElement ignoredDestinationParams = inputElement.firstChildElement("ignoredDestinationParams");
                    // fill the list with ignored parameters
                    if(!ignoredDestinationParams.isNull()){
                        QDomNodeList ignoredParamList = ignoredDestinationParams.childNodes();
                        for(int a=0; a<ignoredParamList.size();a++){

                            QDomElement ignore = ignoredParamList.at(a).toElement();

                            if(ignore.hasAttribute("parameter")){
                                restoreIgnoredParameter.append(getGeometryParameterEnum(ignore.attribute("parameter")));
                            }
                        }
                    }

                    //create and add input elements
                    if(inputElement.hasAttribute("index") && inputElement.hasAttribute("type") && inputElement.hasAttribute("ref")){

                        if(ProjectExchanger::myStations.contains(inputElement.attribute("ref").toInt())){
                            QPointer<Station> station = ProjectExchanger::myStations.value(inputElement.attribute("ref").toInt())->getStation();
                            InputElement element;
                            element.station = station;
                            element.id = station->getId();
                            element.typeOfElement = eStationElement;
                            element.shouldBeUsed = shouldBeUsed;
                            element.ignoredDestinationParams =restoreIgnoredParameter;
                            myFunction->addInputElement(element, inputElement.attribute("index").toInt());
                        }else if(ProjectExchanger::myCoordinateSystems.contains(inputElement.attribute("ref").toInt())){
                            QPointer<CoordinateSystem> coordinateSystem = ProjectExchanger::myCoordinateSystems.value(inputElement.attribute("ref").toInt())->getCoordinateSystem();
                            InputElement element;
                            element.coordSystem = coordinateSystem;
                            element.id = coordinateSystem->getId();
                            element.typeOfElement = eCoordinateSystemElement;
                            element.shouldBeUsed = shouldBeUsed;
                            element.ignoredDestinationParams =restoreIgnoredParameter;
                            myFunction->addInputElement(element, inputElement.attribute("index").toInt());
                        }else if(ProjectExchanger::myTransformationParameters.contains(inputElement.attribute("ref").toInt())){
                            QPointer<TrafoParam> trafoParam = ProjectExchanger::myTransformationParameters.value(inputElement.attribute("ref").toInt())->getTrafoParam();
                            InputElement element;
                            element.trafoParam = trafoParam;
                            element.id = trafoParam->getId();
                            element.typeOfElement = eTrafoParamElement;
                            element.shouldBeUsed = shouldBeUsed;
                            element.ignoredDestinationParams =restoreIgnoredParameter;
                            myFunction->addInputElement(element, inputElement.attribute("index").toInt());
                        }else if(ProjectExchanger::myGeometries.contains(inputElement.attribute("ref").toInt())){
                            QPointer<FeatureWrapper> geometry = ProjectExchanger::myGeometries.value(inputElement.attribute("ref").toInt());
                            InputElement element;
                            element.geometry = geometry->getGeometry();
                            element.id = geometry->getGeometry()->getId();
                            element.shouldBeUsed = shouldBeUsed;
                            element.ignoredDestinationParams =restoreIgnoredParameter;
                            switch(geometry->getFeatureTypeEnum()){
                            case eCircleFeature:
                                element.circle = geometry->getCircle();
                                element.typeOfElement = eCircleElement;
                                break;
                            case eConeFeature:
                                element.cone = geometry->getCone();
                                element.typeOfElement = eConeElement;
                                break;
                            case eCylinderFeature:
                                element.cylinder = geometry->getCylinder();
                                element.typeOfElement = eCylinderElement;
                                break;
                            case eEllipseFeature:
                                element.ellipse = geometry->getEllipse();
                                element.typeOfElement = eEllipseElement;
                                break;
                            case eEllipsoidFeature:
                                element.ellipsoid = geometry->getEllipsoid();
                                element.typeOfElement = eEllipsoidElement;
                                break;
                            case eHyperboloidFeature:
                                element.hyperboloid = geometry->getHyperboloid();
                                element.typeOfElement = eHyperboloidElement;
                                break;
                            case eLineFeature:
                                element.line = geometry->getLine();
                                element.typeOfElement = eLineElement;
                                break;
                            case eNurbsFeature:
                                element.nurbs = geometry->getNurbs();
                                element.typeOfElement = eNurbsElement;
                                break;
                            case eParaboloidFeature:
                                element.paraboloid = geometry->getParaboloid();
                                element.typeOfElement = eParaboloidElement;
                                break;
                            case ePlaneFeature:
                                element.plane = geometry->getPlane();
                                element.typeOfElement = ePlaneElement;
                                break;
                            case ePointFeature:
                                element.point = geometry->getPoint();
                                element.typeOfElement = ePointElement;
                                break;
                            case ePointCloudFeature:
                                element.pointCloud = geometry->getPointCloud();
                                element.typeOfElement = ePointCloudElement;
                                break;
                            case eScalarEntityAngleFeature:
                                element.scalarEntityAngle = geometry->getScalarEntityAngle();
                                element.typeOfElement = eScalarEntityAngleElement;
                                break;
                            case eScalarEntityDistanceFeature:
                                element.scalarEntityDistance = geometry->getScalarEntityDistance();
                                element.typeOfElement = eScalarEntityDistanceElement;
                                break;
                            case eScalarEntityMeasurementSeriesFeature:
                                element.scalarEntityMeasurementSeries = geometry->getScalarEntityMeasurementSeries();
                                element.typeOfElement = eScalarEntityMeasurementSeriesElement;
                                break;
                            case eScalarEntityTemperatureFeature:
                                element.scalarEntityTemperature = geometry->getScalarEntityTemperature();
                                element.typeOfElement = eScalarEntityTemperatureElement;
                                break;
                            case eSlottedHoleFeature:
                                element.slottedHole = geometry->getSlottedHole();
                                element.typeOfElement = eSlottedHoleElement;
                                break;
                            case eSphereFeature:
                                element.sphere = geometry->getSphere();
                                element.typeOfElement = eSphereElement;
                                break;
                            case eTorusFeature:
                                element.torus = geometry->getTorus();
                                element.typeOfElement = eTorusElement;
                                break;
                            }
                            myFunction->addInputElement(element, inputElement.attribute("index").toInt());
                        }else if(ProjectExchanger::myObservations.contains(inputElement.attribute("ref").toInt())){
                            QPointer<Observation> observation = ProjectExchanger::myObservations.value(inputElement.attribute("ref").toInt());
                            InputElement element;
                            element.observation = observation;
                            element.id = observation->getId();
                            element.typeOfElement = eObservationElement;
                            element.shouldBeUsed = shouldBeUsed;
                            element.ignoredDestinationParams =restoreIgnoredParameter;
                            myFunction->addInputElement(element, inputElement.attribute("index").toInt());
                        }else if(ProjectExchanger::myReadings.contains(inputElement.attribute("ref").toInt())){
                            QPointer<Reading> reading = ProjectExchanger::myReadings.value(inputElement.attribute("ref").toInt());
                            InputElement element;
                            element.shouldBeUsed = shouldBeUsed;
                            element.ignoredDestinationParams =restoreIgnoredParameter;
                            switch(reading->getTypeOfReading()){
                            case eDistanceReading:
                                element.distanceReading = reading;
                                element.typeOfElement = eReadingDistanceElement;
                                break;
                            case eCartesianReading:
                                element.cartesianReading = reading;
                                element.typeOfElement = eReadingCartesianElement;
                                break;
                            case ePolarReading:
                                element.polarReading = reading;
                                element.typeOfElement = eReadingPolarElement;
                                break;
                            case eDirectionReading:
                                element.directionReading = reading;
                                element.typeOfElement = eReadingDirectionElement;
                                break;
                            case eTemperatureReading:
                                element.temperatureReading = reading;
                                element.typeOfElement = eReadingTemperatureElement;
                                break;
                            case eLevelReading:
                                element.levelReading = reading;
                                element.typeOfElement = eReadingLevelElement;
                                break;
                            }
                            element.id = reading->getId();
                            myFunction->addInputElement(element, inputElement.attribute("index").toInt());
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
 * \brief ProjectExchanger::restoreBundleDependencies
 * Helper method which gets a bundle adjustment tag as input and then returns a pointer to the bundle adjustment
 * \param bundle
 * \return
 */
QPointer<BundleAdjustment> ProjectExchanger::restoreBundleDependencies(QDomElement &bundle){
    QPointer<BundleAdjustment> result;

    if(!bundle.isNull()){

        if(!bundle.hasAttribute("name") || !bundle.hasAttribute("plugin")){
            return result;
        }

        sdb::Plugin plugin = SystemDbManager::getPlugin(bundle.attribute("plugin"));
        QPointer<BundleAdjustment> myBundle(NULL);
        if(plugin.file_path.compare("") != 0){
            myBundle = PluginLoader::loadBundleAdjustmentPlugin(plugin.file_path, bundle.attribute("name"));
            if(myBundle.isNull()){
                return result;
            }
        }else{
            return result;
        }

        //load bundle from xml
        myBundle->fromOpenIndyXML(bundle);

        //add bundleCoordinateSystem
        QDomElement bundleCoordSys = bundle.firstChildElement("bundleCoordinateSystem");
        if(!bundleCoordSys.isNull() && bundleCoordSys.hasAttribute("ref")){
            QPointer<FeatureWrapper> mySystem = ProjectExchanger::myCoordinateSystems.value(bundleCoordSys.attribute("ref").toInt());
            if(!mySystem.isNull() && !mySystem->getCoordinateSystem().isNull()){
                myBundle->setBundleSystem(mySystem->getCoordinateSystem());
                result = myBundle;
            }else{
                return result;
            }
        }else{
            return result;
        }
    }
    return result;
}

/*!
 * \brief ProjectExchanger::clearHelperMaps
 * Clear the helper maps for loading an OpenIndy project. If deleteOnClear is true then the list objects
 * are not only removed but also deleted.
 * \param deleteOnClear
 */
void ProjectExchanger::clearHelperMaps(bool deleteOnClear){

    //delete the helper maps objects
    if(deleteOnClear){

        //delete observations
        QList<QPointer<Observation> > myObservations = ProjectExchanger::myObservations.values();
        foreach(QPointer<Observation> obs, myObservations){
            delete obs;
        }

        //delete stations
        QList<QPointer<FeatureWrapper> > myStations = ProjectExchanger::myStations.values();
        foreach(QPointer<FeatureWrapper> s, myStations){
            delete s->getStation();
            delete s;
        }

        //delete coordinate systems
        QList<QPointer<FeatureWrapper> > myCoordinateSystems = ProjectExchanger::myCoordinateSystems.values();
        foreach(QPointer<FeatureWrapper> c, myCoordinateSystems){
            delete c->getCoordinateSystem();
            delete c;
        }

        //delete trafo params
        QList<QPointer<FeatureWrapper> > myTrafoParams = ProjectExchanger::myTransformationParameters.values();
        foreach(QPointer<FeatureWrapper> t, myTrafoParams){
            delete t->getTrafoParam();
            delete t;
        }

        //delete geometries
        QList<QPointer<FeatureWrapper> > myGeometries = ProjectExchanger::myGeometries.values();
        foreach(QPointer<FeatureWrapper> g, myGeometries){
            delete g->getFeature();
            delete g;
        }

    }

    //clear the helper maps
    ProjectExchanger::myObservations.clear();
    ProjectExchanger::myStations.clear();
    ProjectExchanger::myCoordinateSystems.clear();
    ProjectExchanger::myReadings.clear();
    ProjectExchanger::myTransformationParameters.clear();
    ProjectExchanger::myGeometries.clear();
    ProjectExchanger::myMConfigs.clear();
    ProjectExchanger::mySConfigs.clear();
    ProjectExchanger::stationPoints.clear();

}
