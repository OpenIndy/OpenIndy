#include "oijob.h"

/*
OiJob::OiJob(QObject *parent) : QObject(parent){

    this->currentId = 0;

}

int OiJob::getFeatureCount(){
    return this->myFeatureContainer.getFeatureCount();
}

int OiJob::getStationCount(){
    return this->myFeatureContainer.getStationCount();
}

int OiJob::getCoordinateSystemCount(){
    return this->myFeatureContainer.getCoordinateSystemCount();
}

int OiJob::getGeometryCount(){
    return this->myFeatureContainer.getGeometryCount();
}

int OiJob::getTrafoParamCount(){
    return this->myFeatureContainer.getTransformationParameterCount();
}

const QList<FeatureWrapper *> &OiJob::getFeatures(){
    return this->myFeatureContainer.getFeaturesList();
}

const QList<Station *> &OiJob::getStations(){
    return this->myFeatureContainer.getStationsList();
}

const QList<CoordinateSystem *> &OiJob::getCoordinateSystems(){
    return this->myFeatureContainer.getCoordinateSystemsList();
}

const QList<TrafoParam *> &OiJob::getTransformationParameters(){
    return this->myFeatureContainer.getTransformationParametersList();
}

const QList<FeatureWrapper *> &OiJob::getGeometries(){
    return this->myFeatureContainer.getGeometriesList();
}

bool OiJob::activateFeature(const int &featureId){
    FeatureWrapper *myFeature = this->getFeature(featureId);
    if(myFeature != NULL && myFeature->getFeature() != NULL){
        myFeature->getFeature()->setActiveFeatureState(true);
        return true;
    }
    return false;
}

bool OiJob::activateStation(const int &featureId){
    FeatureWrapper *myFeature = this->getFeature(featureId);
    if(myFeature != NULL && myFeature->getStation() != NULL){
        myFeature->getStation()->setActiveStationState(true);
        return true;
    }
    return false;
}

bool OiJob::activateCoordinateSystem(const int &featureId){
    FeatureWrapper *myFeature = this->getFeature(featureId);
    if(myFeature != NULL && myFeature->getCoordinateSystem() != NULL){
        myFeature->getCoordinateSystem()->setActiveCoordinateSystemState(true);
        return true;
    }
    return false;
}

QList<FeatureWrapper *> OiJob::getFeaturesByGroup(const QString &group){
    return this->myFeatureContainer.getFeaturesByGroup(group);
}

QList<FeatureWrapper *> OiJob::getFeaturesByName(const QString &name){
    return this->myFeatureContainer.getFeaturesByName(name);
}

FeatureWrapper* OiJob::getActiveFeature(){
    return this->myActiveFeature;
}

Station* OiJob::getActiveStation(){
    return this->myActiveStation;
}

CoordinateSystem* OiJob::getActiveCoordinateSystem(){
    return this->myActiveCoordinateSystem;
}

QStringList OiJob::getAvailableGroups(){
    return this->myFeatureContainer.getFeatureGroupList();
}

const QString &OiJob::getActiveGroup(){
    return this->myActiveGroup;
}

bool OiJob::setActiveGroup(const QString &group){
    if(this->myFeatureContainer.getFeatureGroupList().contains(group)){
        this->myActiveGroup = group;
        return true;
    }
    return false;
}

int OiJob::generateUniqueId(){
    this->currentId++;
    return this->currentId;
}

FeatureWrapper *OiJob::getFeature(const int &featureId){
    return this->myFeatureContainer.getFeatureById(featureId);
}

QList<FeatureWrapper *> OiJob::getFeaturesByFeatureType(const FeatureTypes &type){
    return this->myFeatureContainer.getFeaturesByType(type);
}

FeatureWrapper *OiFeatureState::addFeature(FeatureAttributes attributes){
    try{

        QList<FeatureWrapper *> myFeatures = OiFeatureState::createFeatures(attributes);

        if(myFeatures.size() == 1){

            //if a group is set for the new feature emit the group changed signal
            if(attributes.group.compare("") != 0){
                QString group = attributes.group;
                if(OiFeatureState::myAvailableGroups.contains(group)){
                    OiFeatureState::myAvailableGroups.insert(group, 1);
                }else{
                    int count = OiFeatureState::myAvailableGroups.find(group).value();
                    OiFeatureState::myAvailableGroups.insert(group, count+1);
                }
                OiFeatureState::getInstance()->emitSignal(eAvailableGroupsChanged);
            }

            OiFeatureState::getInstance()->emitSignal(eFeatureSetChanged);
            if(attributes.typeOfFeature == Configuration::eCoordinateSystemFeature
                    || attributes.typeOfFeature == Configuration::eStationFeature){
                OiFeatureState::getInstance()->emitSignal(eCoordSysSetChanged);
            }

            return myFeatures.at(0);

        }

        return NULL;

    }catch(exception &e){
        Console::addLine(e.what());
        return NULL;
    }
}

bool OiJob::addFeature(FeatureWrapper *myFeature){

    if(myFeature == NULL || myFeature->getFeature() == NULL || myFeature->getFeature()->getFeatureName().compare("") == 0){
        return false;
    }

    //check if feature with this id already exists
    if(this->myFeatureContainer.getFeatureById(myFeature->getFeature()->getId()) != NULL){
        myFeature->getFeature()->setId(this->generateUniqueId());
    }

    //check feature's name
    bool nameValid = false;
    if(myFeature->getGeometry() != NULL && myFeature->getGeometry()->getIsNominal()){
        nameValid = this->validateFeatureName(myFeature->getFeatureTypeEnum(), myFeature->getFeature()->getFeatureName(), true, myFeature->getGeometry()->getNominalSystem());
    }else{
        nameValid = this->validateFeatureName(myFeature->getFeatureTypeEnum(), myFeature->getFeature()->getFeatureName());
    }
    if(!nameValid){
        QString name = myFeature->getFeature()->getFeatureName();
        if(myFeature->getGeometry() != NULL && myFeature->getGeometry()->getIsNominal()){
            while(!this->validateFeatureName(myFeature->getFeatureTypeEnum(), name.append("_new"), true, myFeature->getGeometry()->getNominalSystem())){}
        }else{
            while(!this->validateFeatureName(myFeature->getFeatureTypeEnum(), name.append("_new"))){}
        }
        myFeature->getFeature()->setFeatureName(name);
    }

    //add the feature to the list of features, stations, coordinate systems, trafo params and geometries
    this->myFeatureContainer.addFeature(myFeature);

    //add nominal to nominal list of coordinate system
    if(myFeature->getGeometry() != NULL && myFeature->getGeometry()->getNominalSystem() != NULL){
        myFeature->getGeometry()->getNominalSystem()->addNominal(myFeature);
    }

    //add UnitConverter to feature
    //myFeature->getFeature()->convertMetricValue = &OiUnitConverter::convertMetricValue;

    //connect the feature's signals to slots in OiFeatureState
    this->connectFeature(myFeature);

    //if a group is set for the new feature emit the group changed signal
    if(myFeature->getFeature()->getGroupName().compare("") != 0
            && !myFeatureContainer.getFeatureGroupList().contains(myFeature->getFeature()->getGroupName())){
        emit this->availableGroupsChanged();
    }

    emit this->featureSetChanged();
    if(myFeature->getFeatureTypeEnum() == eCoordinateSystemFeature
            || myFeature->getFeatureTypeEnum() == eStationFeature){
        emit this->coordSystemSetChanged();
    }

    return true;

}

QList<FeatureWrapper *> OiJob::addFeatures(const FeatureAttributes &attributes){

    QList<FeatureWrapper *> result;
    result = this->createFeatures(attributes);

    //if a group is set for the new feature emit the group changed signal
    if(attributes.group.compare("") != 0 && !this->myFeatureContainer.getFeatureGroupList().contains(attributes.group)){
        emit this->availableGroupsChanged();
    }

    emit this->featureSetChanged();
    if(attributes.typeOfFeature == eCoordinateSystemFeature
            || attributes.typeOfFeature == eStationFeature){
        emit this->coordSystemSetChanged();
    }

    return result;

}

bool OiJob::addFeatures(const QList<FeatureWrapper *> &myFeatures){

    foreach(FeatureWrapper *myFeature, myFeatures){

        if(myFeature == NULL || myFeature->getFeature() == NULL || myFeature->getFeature()->getFeatureName().compare("") == 0){
            continue;
        }

        //check if feature with this id already exists
        if(this->myFeatureContainer.getFeatureById(myFeature->getFeature()->getId()) != NULL){
            myFeature->getFeature()->setId(this->generateUniqueId());
        }

        //check feature's name
        bool nameValid = false;
        if(myFeature->getGeometry() != NULL && myFeature->getGeometry()->getIsNominal()){
            nameValid = this->validateFeatureName(myFeature->getFeatureTypeEnum(), myFeature->getFeature()->getFeatureName(), true, myFeature->getGeometry()->getNominalSystem());
        }else{
            nameValid = this->validateFeatureName(myFeature->getFeatureTypeEnum(), myFeature->getFeature()->getFeatureName());
        }
        if(!nameValid){
            QString name = myFeature->getFeature()->getFeatureName();
            if(myFeature->getGeometry() != NULL && myFeature->getGeometry()->getIsNominal()){
                while(!this->validateFeatureName(myFeature->getFeatureTypeEnum(), name.append("_new"), true, myFeature->getGeometry()->getNominalSystem())){}
            }else{
                while(!this->validateFeatureName(myFeature->getFeatureTypeEnum(), name.append("_new"))){}
            }
            myFeature->getFeature()->setFeatureName(name);
        }

        //add the feature to the list of features, stations, coordinate systems, trafo params and geometries
        this->myFeatureContainer.addFeature(myFeature);

        //add nominal to nominal list of coordinate system
        if(myFeature->getGeometry() != NULL && myFeature->getGeometry()->getNominalSystem() != NULL){
            myFeature->getGeometry()->getNominalSystem()->addNominal(myFeature);
        }

        //connect the feature's signals to slots in OiFeatureState
        this->connectFeature(myFeature);

        //if a group is set for the new feature emit the group changed signal
        if(myFeature->getFeature()->getGroupName().compare("") != 0
                && !this->myFeatureContainer.getFeatureGroupList().contains(myFeature->getFeature()->getGroupName())){
            emit this->availableGroupsChanged();
        }

        if(myFeature->getFeatureTypeEnum() == eCoordinateSystemFeature
                || myFeature->getFeatureTypeEnum() == eStationFeature){
            emit this->coordSystemSetChanged();
        }

    }

    emit this->featureSetChanged();

    return true;

}

bool OiJob::removeFeature(FeatureWrapper *myFeature){

    if(myFeature != NULL && myFeature->getFeature() != NULL){

        //disconnect feature's signals from OiFeatureState
        this->disconnectFeature(myFeature);

        //if needed set active pointers to NULL
        if(this->myActiveCoordinateSystem != NULL
                && this->myActiveCoordinateSystem->getId() == myFeature->getFeature()->getId()){
            this->myActiveCoordinateSystem = NULL;
            emit this->activeCoordinateSystemChanged();
        }
        if(this->myActiveStation != NULL
                && this->myActiveStation->getId() == myFeature->getFeature()->getId()){
            this->myActiveStation = NULL;
            emit this->activeStationChanged();
        }
        if(this->myActiveFeature != NULL && this->myActiveFeature->getFeature() != NULL
                && this->myActiveFeature->getFeature()->getId() == myFeature->getFeature()->getId()){
            this->myActiveFeature = NULL;
            emit this->activeFeatureChanged();
        }

        //remove group from groups map if needed
        if(myFeature->getFeature()->getGroupName().compare("") != 0
                && this->myFeatureContainer.getFeaturesByGroup(myFeature->getFeature()->getGroupName()).size() <= 1){
            emit this->availableGroupsChanged();
        }

        //remove feature from list of features and delete it
        this->myFeatureContainer.removeAndDeleteFeature(myFeature->getFeature()->getId());

        emit this->featureSetChanged();

        return true;

    }
    return false;

}

QList<FeatureWrapper *> OiJob::createFeatures(const FeatureAttributes &attributes){



    QList<FeatureWrapper *> result;

    //get the feature name
    QString name;
    int index;
    this->createFeatureName(name, index, attributes.name, attributes.count);

    bool nominal = attributes.isNominal;

    int numIterations = 1;

    if(attributes.isNominal && attributes.isActual && getIsGeometry(attributes.typeOfFeature)){
        numIterations++;
    }

    for(int j = 0; j < numIterations; j++){

        //create all features
        for(int i = 0; i < attributes.count; i++){

            //create feature + feature wrapper and set measurement config
            FeatureWrapper *myFeature = new FeatureWrapper();
            switch(attributes.typeOfFeature){
            case ePointFeature: {
                Point *myPoint = new Point(nominal);
                //myPoint->setMeasurementConfig(Point::defaultMeasurementConfig);
                myFeature->setPoint(myPoint);
                break;
            }case eLineFeature: {
                Line *myLine = new Line(nominal);
                //myLine->setMeasurementConfig(Line::defaultMeasurementConfig);
                myFeature->setLine(myLine);
                break;
            }case ePlaneFeature:{
                Plane *myPlane = new Plane(nominal);
                //myPlane->setMeasurementConfig(Plane::defaultMeasurementConfig);
                myFeature->setPlane(myPlane);
                break;
            }case eSphereFeature:{
                Sphere *mySphere = new Sphere(nominal);
                //mySphere->setMeasurementConfig(Sphere::defaultMeasurementConfig);
                myFeature->setSphere(mySphere);
                break;
            }case eCircleFeature:{
                Circle *myCircle = new Circle(nominal);
                //myCircle->setMeasurementConfig(Circle::defaultMeasurementConfig);
                myFeature->setCircle(myCircle);
                break;
            }case eConeFeature:{
                Cone *myCone = new Cone(nominal);
                //myCone->setMeasurementConfig(Cone::defaultMeasurementConfig);
                myFeature->setCone(myCone);
                break;
            }case eCylinderFeature:{
                Cylinder *myCylinder = new Cylinder(nominal);
                //myCylinder->setMeasurementConfig(Cylinder::defaultMeasurementConfig);
                myFeature->setCylinder(myCylinder);
                break;
            }case eEllipsoidFeature:{
                Ellipsoid *myEllipsoid = new Ellipsoid(nominal);
                //myEllipsoid->setMeasurementConfig(Ellipsoid::defaultMeasurementConfig);
                myFeature->setEllipsoid(myEllipsoid);
                break;
            }case eHyperboloidFeature:{
                Hyperboloid *myHyperboloid = new Hyperboloid(nominal);
                //myHyperboloid->setMeasurementConfig(Hyperboloid::defaultMeasurementConfig);
                myFeature->setHyperboloid(myHyperboloid);
                break;
            }case eParaboloidFeature:{
                Paraboloid *myParaboloid = new Paraboloid(nominal);
                //myParaboloid->setMeasurementConfig(Paraboloid::defaultMeasurementConfig);
                myFeature->setParaboloid(myParaboloid);
                break;
            }case ePointCloudFeature:{
                PointCloud *myPointCloud = new PointCloud(nominal);
                //myPointCloud->setMeasurementConfig(PointCloud::defaultMeasurementConfig);
                myFeature->setPointCloud(myPointCloud);
                break;
            }case eNurbsFeature:{
                Nurbs *myNurbs = new Nurbs(nominal);
                //myNurbs->setMeasurementConfig(Nurbs::defaultMeasurementConfig);
                myFeature->setNurbs(myNurbs);
                break;
            }case eStationFeature:{
                Station *myStation = new Station(name);
                myFeature->setStation(myStation);
                break;
            }case eCoordinateSystemFeature:{
                CoordinateSystem *myCoordinateSystem = new CoordinateSystem();
                myFeature->setCoordinateSystem(myCoordinateSystem);
                break;
            }case eTrafoParamFeature:{
                TrafoParam *myTrafoParam = new TrafoParam();
                myTrafoParam->setCoordinateSystems(attributes.startSystem, attributes.destSystem);
                myTrafoParam->setIsMovement(attributes.isMovement);
                for(int i=0; i<OiFeatureState::getCoordinateSystems().size();i++){
                    if(OiFeatureState::getCoordinateSystems().at(i) == attributes.startSystem){
                        OiFeatureState::getCoordinateSystems().at(i)->addTransformationParameter(myTrafoParam);
                    }
                    if(OiFeatureState::getCoordinateSystems().at(i) == attributes.destSystem){
                        OiFeatureState::getCoordinateSystems().at(i)->addTransformationParameter(myTrafoParam);
                    }
                }
                for(int i=0; i<OiFeatureState::getStations().size();i++){
                    if(OiFeatureState::getStations().at(i)->coordSys == attributes.startSystem){
                        OiFeatureState::getStations().at(i)->coordSys->addTransformationParameter(myTrafoParam);
                    }
                    if(OiFeatureState::getStations().at(i)->coordSys == attributes.destSystem){
                        OiFeatureState::getStations().at(i)->coordSys->addTransformationParameter(myTrafoParam);
                    }
                }
                myFeature->setTrafoParam(myTrafoParam);
                break;
            }case eScalarEntityAngleFeature:{
                ScalarEntityAngle *myAngle = new ScalarEntityAngle(nominal);
                //myAngle->setMeasurementConfig(ScalarEntityAngle::defaultMeasurementConfig);
                myFeature->setScalarEntityAngle(myAngle);
                break;
            }case eScalarEntityDistanceFeature:{
                ScalarEntityDistance *myDistance = new ScalarEntityDistance(nominal);
                //myDistance->setMeasurementConfig(ScalarEntityDistance::defaultMeasurementConfig);
                myFeature->setScalarEntityDistance(myDistance);
                break;
            }case eScalarEntityTemperatureFeature:{
                ScalarEntityTemperature *myTemperature = new ScalarEntityTemperature(nominal);
                //myTemperature->setMeasurementConfig(ScalarEntityTemperature::defaultMeasurementConfig);
                myFeature->setScalarEntityTemperature(myTemperature);
                break;
            }case eScalarEntityMeasurementSeriesFeature:{
                ScalarEntityMeasurementSeries *myMeasurementSeries = new ScalarEntityMeasurementSeries(nominal);
                //myMeasurementSeries->setMeasurementConfig(ScalarEntityMeasurementSeries::defaultMeasurementConfig);
                myFeature->setScalarEntityMeasurementSeries(myMeasurementSeries);
                break;
            }default:
                break;
            }

            //set feature attributes
            QString featureName = name;
            if(attributes.count > 1){
                featureName = name + QString::number(index+i);
            }
            myFeature->getFeature()->setFeatureName(featureName);
            myFeature->getFeature()->setGroupName(attributes.group);
            if(myFeature->getGeometry() != NULL){
                myFeature->getGeometry()->setCommonState(attributes.isCommon);
                if(myFeature->getGeometry()->getIsNominal()){
                    //myFeature->getGeometry()->setNominalSystem(attributes.nominalSystem);
                }
            }

            //add the feature to the list of features, stations, coordinate systems, trafo params and geometries
            this->myFeatureContainer.addFeature(myFeature);

            //add nominal to nominal list of coordinate system
            if(myFeature->getGeometry() != NULL && myFeature->getGeometry()->getNominalSystem() != NULL){
                myFeature->getGeometry()->getNominalSystem()->addNominal(myFeature);
            }

            //connect the feature's signals to slots in OiFeatureState
            this->connectFeature(myFeature);

            //set function
            if(!nominal){
                QString filePath;
                SystemDbManager::getFunctionPluginFilePath(filePath, attributes.functionPlugin.first, attributes.functionPlugin.second);
                Function *checkFunction = PluginLoader::loadFunctionPlugin(filePath, attributes.functionPlugin.second);

                myFeature->getFeature()->addFunction(checkFunction);
            }

            result.append(myFeature);

        }

        nominal = !nominal;

    }

    return result;

}

void OiJob::createFeatureName(QString &outputName, int &index, QString inputName, int count){

    int startIndex = inputName.size() - 1;

    //split the input name into name and postfix
    while(startIndex > 0 && inputName.at(startIndex).isDigit()){
        startIndex--;
    }
    outputName = inputName.mid(0, startIndex + 1);
    index = inputName.mid(startIndex, inputName.size() - startIndex - 1).toInt();

}

void OiJob::connectFeature(FeatureWrapper *myFeature){

    //TODO references

    //general feature connects
    connect(myFeature->getFeature(), SIGNAL(featureIsActiveChanged(int)),
            this, SLOT(setActiveFeature(int)), Qt::DirectConnection);
    connect(myFeature->getFeature(), SIGNAL(featureGroupChanged(int)),
            this, SLOT(setFeatureGroup(int)), Qt::DirectConnection);
    connect(myFeature->getFeature(), SIGNAL(featureNameChanged(int,QString)),
            this, SLOT(setFeatureName(int,QString)), Qt::DirectConnection);
    connect(myFeature->getFeature(), SIGNAL(featureAboutToBeDeleted(int)),
            this, SLOT(removeFeature(int)), Qt::DirectConnection);

    //geometry specific connects
    if(myFeature->getGeometry() != NULL){

        connect(myFeature->getGeometry(), SIGNAL(geomMyObservationsChanged(int)),
                this, SLOT(setGeometryObservations(int)), Qt::DirectConnection);
        connect(myFeature->getGeometry(), SIGNAL(geomMyActualChanged(int)),
                this, SLOT(setGeometryActual(int)), Qt::DirectConnection);
        connect(myFeature->getGeometry(), SIGNAL(geomMyMeasurementConfigChanged(int)),
                this, SLOT(setGeometryMeasurementConfig(int)), Qt::DirectConnection);

        //only for point clouds
        if(myFeature->getPointCloud() != NULL){
            connect(myFeature->getPointCloud(), SIGNAL(pcSegmentAdded(FeatureWrapper*)),
                    this, SLOT(addPCSegmentAsFeature(FeatureWrapper*)), Qt::DirectConnection);
        }

    }

    //station specific connects
    if(myFeature->getStation() != NULL){

        connect(myFeature->getStation(), SIGNAL(activeStationChanged(int)),
                this, SLOT(setActiveStation(int)), Qt::DirectConnection);
        connect(myFeature->getStation()->getCoordinateSystem(), SIGNAL(activeCoordinateSystemChanged(int)),
                this, SLOT(setActiveCoordinateSystem(int)), Qt::DirectConnection);
        connect(myFeature->getStation()->getCoordinateSystem(), SIGNAL(observationsChanged(int, int)),
                this, SLOT(setSystemObservations(int, int)), Qt::DirectConnection);

    }

    //coordinate system specific connects
    if(myFeature->getCoordinateSystem() != NULL){

        connect(myFeature->getCoordinateSystem(), SIGNAL(activeCoordinateSystemChanged(int)),
                this, SLOT(setActiveCoordinateSystem(int)), Qt::DirectConnection);
        connect(myFeature->getCoordinateSystem(), SIGNAL(nominalsChanged(int)),
                this, SLOT(setSystemsNominals(int)), Qt::DirectConnection);
        connect(myFeature->getCoordinateSystem(), SIGNAL(observationsChanged(int, int)),
                this, SLOT(setSystemObservations(int, int)), Qt::DirectConnection);

    }

    //call OiConfigState's connect method
    //OiConfigState::connectFeature(myFeature);

}

void OiJob::disconnectFeature(FeatureWrapper *myFeature){
    disconnect(myFeature->getFeature(), SIGNAL(featureIsActiveChanged(int)),
            this, SLOT(setActiveFeature(int)));
    disconnect(myFeature->getFeature(), SIGNAL(featureGroupChanged(int)),
               this, SLOT(setFeatureGroup(int)));

    //call OiConfigState's disconnect method
    //OiConfigState::disconnectFeature(myFeature);
}

 * \brief OiFeatureState::validateFeatureName
 * \param featureType the type of the feature that shall be added
 * \param featureName the name of the feature that shall be added
 * \param isNominal true if a nominal shall be added, false if not
 * \param myNomSys a pointer to the nominal system of the feature (only if isNominal = true)
 * \return

bool OiJob::validateFeatureName(const FeatureTypes &featureType, const QString &featureName, bool isNominal, CoordinateSystem *myNomSys){

    //do not accept empty names
    if(featureName.compare("") == 0){
        return false;
    }

    //get a list of all features with name featureName
    QList<FeatureWrapper *> equalNameFeatures = this->myFeatureContainer.getFeaturesByName(featureName);

    //accept featureName if no other feature with the same name exists
    if(equalNameFeatures.size() == 0){
        return true;
    }

    if(isNominal){

        //reject featureName if there is another nominal with the same type, name and nominal system or
        //if there is a station, coordinate system or trafo param feature with the same name
        foreach(FeatureWrapper *myFeature, equalNameFeatures){

            if(myFeature->getGeometry() != NULL && myFeature->getGeometry()->getIsNominal()
                    && myFeature->getGeometry()->getNominalSystem() == myNomSys
                    && myFeature->getFeatureTypeEnum() == featureType){
                return false;
            }else if(myFeature->getFeatureTypeEnum() == eCoordinateSystemFeature
                     || myFeature->getFeatureTypeEnum() == eTrafoParamFeature
                     || myFeature->getFeatureTypeEnum() == eStationFeature){
                return false;
            }

        }

    }else{

        //reject featureName if a station, coordinate system or trafo param feature shall be added and its name is already used
        if(featureType == eCoordinateSystemFeature || featureType == eStationFeature
                || featureType == eTrafoParamFeature){
            return false;
        }

        //reject featureName if an actual geometry shall be added with the same name and type
        foreach(FeatureWrapper *myFeature, equalNameFeatures){
            if(myFeature->getGeometry() != NULL && !myFeature->getGeometry()->getIsNominal()
                    && myFeature->getFeatureTypeEnum() == featureType){
                return false;
            }
        }

    }

    return true;

}

void OiJob::setActiveFeature(const int &featureId){
    try{

        //get the feature with id featureId
        FeatureWrapper *myFeature = this->myFeatureContainer.getFeatureById(featureId);

        //if no feature exists with that id
        if(myFeature == NULL){
            return;
        }

        //if the feature was set to be the active feature
        if(myFeature->getFeature()->getIsActiveFeature()){

            //set the active feature pointer to that feature ...
            this->myActiveFeature = myFeature;

            //... and set isActive of active feature to true and all other feature's isActive property to false
            foreach(FeatureWrapper *f, this->myFeatureContainer.getFeaturesList()){
                if(f->getFeature()->getId() == featureId){
                    f->getFeature()->setActiveFeatureState(true);
                }else{
                    f->getFeature()->setActiveFeatureState(false);
                }
            }

            //emit signal to inform that active feature has changed
            emit this->activeFeatureChanged();

        }

    }catch(exception &e){
        Console::addLine(e.what());
    }
}

void OiJob::setActiveStation(const int &featureId){
    try{

        //get the station with id featureId
        FeatureWrapper *myStation = this->myFeatureContainer.getFeatureById(featureId);

        //if no station exists with that id
        if(myStation == NULL || myStation->getStation() == NULL){
            return;
        }

        //if the station was set to be the active station
        if(myStation->getStation()->getIsActiveStation()){

            //set the active station pointer to that station ...
            this->myActiveStation = myStation->getStation();

            //... and set isActiveStation of active station to true and all other station's isActiveStation property to false
            foreach(Station *s, this->myFeatureContainer.getStationsList()){
                if(s->getId() == featureId){
                    s->setActiveStationState(true);
                }else{
                    s->setActiveStationState(false);
                }
            }

            //emit signal to inform that active station has changed
            emit this->activeStationChanged();

        }

    }catch(exception &e){
        Console::addLine(e.what());
    }
}

void OiJob::setActiveCoordinateSystem(const int &featureId){
    try{

        //get the coordinate system with id featureId
        CoordinateSystem *mySystem = NULL;
        FeatureWrapper *myCoordinateSystem = this->myFeatureContainer.getFeatureById(featureId);

        if(myCoordinateSystem != NULL){
            mySystem = myCoordinateSystem->getCoordinateSystem();
        }

        //if no coordinate system with that id exists
        if(myCoordinateSystem == NULL || myCoordinateSystem->getCoordinateSystem() == NULL){

            //check if there is a station system with id featureId
            foreach(Station *s, this->myFeatureContainer.getStationsList()){
                if(s->coordSys->getId() == featureId){
                    mySystem = s->coordSys;
                    break;
                }
            }

        }

        if(mySystem == NULL){
            return;
        }

        //if the coordinate system was set to be the active coordinate system
        if(mySystem->getIsActiveCoordinateSystem()){

            //set the active coordinate system pointer to that coordinate system
            this->myActiveCoordinateSystem = mySystem;

            //set isActiveCoordinateSystem of active system to true and all other system's isActiveCoordinateSystem property to false
            foreach(CoordinateSystem *c, this->myFeatureContainer.getCoordinateSystemsList()){
                if(c->getId() == featureId){
                    c->setActiveCoordinateSystemState(true);
                }else{
                    c->setActiveCoordinateSystemState(false);
                }
            }
            foreach(Station *s, this->myFeatureContainer.getStationsList()){
                if(s->coordSys->getId() == featureId){
                    s->coordSys->setActiveCoordinateSystemState(true);
                }else{
                    s->coordSys->setActiveCoordinateSystemState(false);
                }
            }

            //emit signal to inform that active coordinate system has changed
            emit this->activeCoordinateSystemChanged();

        }

    }catch(exception &e){
        Console::addLine(e.what());
    }
}

void OiJob::setFeatureGroup(const int &featureId, const QString &oldGroup){
    this->myFeatureContainer.featureGroupChanged(featureId, oldGroup);
    emit this->availableGroupsChanged();
}

void OiJob::setFeatureName(const int &featureId, const QString &oldName){
    this->myFeatureContainer.featureNameChanged(featureId, oldName);
    emit this->featureAttributesChanged();
}

void OiJob::setFeatureComment(const int &featureId){
    emit this->featureAttributesChanged();
}

void OiJob::setFeatureFunctions(const int &featureId){
    emit this->featureFunctionsChanged();
}

void OiJob::setGeometryActual(const int &featureId){
    try{

        FeatureWrapper *myNominal = this->getFeature(featureId);

        if(myNominal == NULL || myNominal->getGeometry() == NULL){
            return;
        }


        myNominal->getGeometry()->getActual();
        FeatureWrapper *newActual;
        newActual= new FeatureWrapper();

        switch (myNominal->getTypeOfFeature()) {
        case Configuration::ePointFeature:
            newActual->setPoint((Point*)myNominal->getGeometry()->getMyActual());
            break;
        case Configuration::eLineFeature:
            newActual->setPoint((Point*)myNominal->getGeometry()->getMyActual());
            break;
        case Configuration::ePlaneFeature:
            newActual->setPoint((Point*)myNominal->getGeometry()->getMyActual());
            break;
        case Configuration::eSphereFeature:
            newActual->setPoint((Point*)myNominal->getGeometry()->getMyActual());
            break;
        case Configuration::eStationFeature:

            break;
        case Configuration::eCoordinateSystemFeature:

            break;
        case Configuration::eTrafoParamFeature:

            break;
        case Configuration::ePointCloudFeature:
            newActual->setPoint((Point*)myNominal->getGeometry()->getMyActual());
            break;
        case Configuration::eCircleFeature:
            newActual->setPoint((Point*)myNominal->getGeometry()->getMyActual());
            break;
        case Configuration::eConeFeature:
            newActual->setPoint((Point*)myNominal->getGeometry()->getMyActual());
            break;
        case Configuration::eCylinderFeature:
            newActual->setPoint((Point*)myNominal->getGeometry()->getMyActual());
            break;
        case Configuration::eEllipsoidFeature:
            newActual->setPoint((Point*)myNominal->getGeometry()->getMyActual());
            break;
        case Configuration::eHyperboloidFeature:
            newActual->setPoint((Point*)myNominal->getGeometry()->getMyActual());
            break;
        case Configuration::eNurbsFeature:
            newActual->setPoint((Point*)myNominal->getGeometry()->getMyActual());
            break;
        case Configuration::eParaboloidFeature:
            newActual->setPoint((Point*)myNominal->getGeometry()->getMyActual());
            break;
        case Configuration::eScalarEntityAngleFeature:
            newActual->setPoint((Point*)myNominal->getGeometry()->getMyActual());
            break;
        case Configuration::eScalarEntityDistanceFeature:
            newActual->setPoint((Point*)myNominal->getGeometry()->getMyActual());
            break;
        case Configuration::eScalarEntityTemperatureFeature:
            newActual->setPoint((Point*)myNominal->getGeometry()->getMyActual());
            break;
        case Configuration::eScalarEntityMeasurementSeriesFeature:
            newActual->setPoint((Point*)myNominal->getGeometry()->getMyActual());
            break;
        default:
            break;
        }

        emit this->featureSetChanged();

    }catch(exception &e){
        Console::addLine(e.what());
    }
}

void OiJob::setGeometryNominals(const int &featureId){
    emit this->featureSetChanged();
}

void OiJob::setGeometryObservations(const int &featureId){
    emit this->geometryObservationsChanged();
}

void OiJob::setGeometryMeasurementConfig(const int &featureId){
    emit this->geometryMeasurementConfigChanged();
}

void OiJob::setSystemsNominals(const int &featureId){
    try{

        int featureIndex = OiFeatureState::getFeatureListIndex(featureId);
        if(featureIndex >= 0){

            //check if the added nominal already exists in OpenIndy
            FeatureWrapper *nominalSystem = OiFeatureState::myFeatures.at(featureIndex);
            FeatureWrapper *myNominal = nominalSystem->getCoordinateSystem()->getNominals().last();
            int nominalIndex = OiFeatureState::getFeatureListIndex(myNominal->getGeometry()->getId());
            if(nominalIndex >= 0){
                OiFeatureState::getInstance()->emitSignal(eSystemNominalsChanged);
            }else{
                myNominal->getGeometry()->setId(Configuration::generateID());
                OiFeatureState::addFeature(myNominal);
                OiFeatureState::getInstance()->emitSignal(eSystemNominalsChanged);
            }

        }

    }catch(exception &e){
        Console::addLine(e.what());
    }
}

void OiJob::removeFeature(const int &featureId){
    this->myFeatureContainer.removeFeature(featureId);
}

 * \brief OiFeatureState::addPCSegmentAsFeature
 * Add a segment, detected from a point cloud to the list of features in OpenIndy
 * \param segment

void OiJob::addPCSegmentAsFeature(FeatureWrapper *segment){

    qDebug() << "pc segment as feature featurestate";

    this->addFeature(segment);

}

void OiJob::setSystemObservations(const int &featureId, const int &obsId){

}
*/

/*!
 * \brief OiJob::OiJob
 * \param parent
 */
OiJob::OiJob(QObject *parent) : QObject(parent), nextId(1){

}

/*!
 * \brief OiJob::~OiJob
 */
OiJob::~OiJob(){

    //remove all features
    this->featureContainer.removeAll();

    //delete device pointer
    if(!this->jobDevice.isNull()){
        delete this->jobDevice;
    }

}

/*!
 * \brief OiJob::getJobName
 * \return
 */
const QString &OiJob::getJobName() const{
    return this->jobName;
}

/*!
 * \brief OiJob::setJobName
 * \param jobName
 */
void OiJob::setJobName(const QString &jobName){
    this->jobName = jobName;
}

/*!
 * \brief OiJob::getJobDevice
 * \return
 */
const QPointer<QIODevice> &OiJob::getJobDevice() const{
    return this->jobDevice;
}

/*!
 * \brief OiJob::setJobDevice
 * \param jobDevice
 */
void OiJob::setJobDevice(const QPointer<QIODevice> &jobDevice){
    this->jobDevice = jobDevice;
}

/*!
 * \brief OiJob::generateUniqueId
 * \return
 */
int OiJob::generateUniqueId(){
    this->nextId++;
    return (this->nextId - 1);
}

/*!
 * \brief OiJob::validateFeatureName
 * \param name
 * \param type
 * \param isNominal
 * \param nominalSystem
 * \return
 */
bool OiJob::validateFeatureName(const QString &name, const FeatureTypes &type, const bool &isNominal, const QPointer<CoordinateSystem> &nominalSystem){

    //do not accept empty names
    if(name.compare("") == 0){
        return false;
    }

    //get a list of features with name name
    QList<QPointer<FeatureWrapper> > features = this->featureContainer.getFeaturesByName(name);

    //accept name if no other feature with that name exists
    if(features.size() == 0){
        return true;
    }

    if(type != eCoordinateSystemFeature && type != eTrafoParamFeature && type != eStationFeature
            && type != eUndefinedFeature && isNominal == true){ //nominal geometry

        //check if nominal system is valid
        if(nominalSystem.isNull()){
            return false;
        }

        //all equal name features have to be geometries, but no nominal with the same type and nominal system
        foreach(const QPointer<FeatureWrapper> &feature, features){

            //check if feature is valid
            if(feature.isNull()){
                continue;
            }

            //check if feature is a geometry
            if(feature->getGeometry().isNull()){
                return false;
            }

            //check if feature is a nominal with same type and nominal system
            if(feature->getFeatureTypeEnum() == type && feature->getGeometry()->getIsNominal()
                    && feature->getGeometry()->getNominalSystem() == nominalSystem){
                return false;
            }

        }

        return true;

    }else if(type != eCoordinateSystemFeature && type != eTrafoParamFeature && type != eStationFeature
             && type != eUndefinedFeature){ //actual geometry

        //all equal name features have to be geometries, but no actual with the same type
        foreach(const QPointer<FeatureWrapper> &feature, features){

            //check if feature is valid
            if(feature.isNull()){
                continue;
            }

            //check if feature is a geometry
            if(feature->getGeometry().isNull()){
                return false;
            }

            //check if feature is an actual with same type
            if(feature->getFeatureTypeEnum() == type && !feature->getGeometry()->getIsNominal()){
                return false;
            }

        }

        return true;

    }else{ //non-geometry feature
        return false;
    }

}

/*!
 * \brief OiJob::getFeatureCount
 * \return
 */
int OiJob::getFeatureCount() const{
    return this->featureContainer.getFeatureCount();
}

/*!
 * \brief OiJob::getGeometryCount
 * \return
 */
int OiJob::getGeometryCount() const{
    return this->featureContainer.getGeometryCount();
}

/*!
 * \brief OiJob::getFeatureCount
 * \param type
 * \return
 */
int OiJob::getFeatureCount(const FeatureTypes &type) const{
    return this->featureContainer.getFeatureCount(type);
}

/*!
 * \brief OiJob::getFeatureIdList
 * \return
 */
const QList<int> &OiJob::getFeatureIdList() const{
    return this->featureContainer.getFeatureIdList();
}

/*!
 * \brief OiJob::getFeatureNameList
 * \return
 */
const QStringList &OiJob::getFeatureNameList() const{
    return this->featureContainer.getFeatureNameList();
}

/*!
 * \brief OiJob::getFeatureGroupList
 * \return
 */
const QStringList &OiJob::getFeatureGroupList() const{
    return this->featureContainer.getFeatureGroupList();
}

/*!
 * \brief OiJob::getFeaturesList
 * \return
 */
const QList<QPointer<FeatureWrapper> > &OiJob::getFeaturesList() const{
    return this->featureContainer.getFeaturesList();
}

/*!
 * \brief OiJob::getCoordinateSystemsList
 * Returns a list of all nominal systems
 * \return
 */
const QList<QPointer<CoordinateSystem> > &OiJob::getCoordinateSystemsList() const{
    return this->featureContainer.getCoordinateSystemsList();
}

/*!
 * \brief OiJob::getStationsList
 * \return
 */
const QList<QPointer<Station> > &OiJob::getStationsList() const{
    return this->featureContainer.getStationsList();
}

/*!
 * \brief OiJob::getTransformationParametersList
 * \return
 */
const QList<QPointer<TrafoParam> > &OiJob::getTransformationParametersList() const{
    return this->featureContainer.getTransformationParametersList();
}

/*!
 * \brief OiJob::getGeometriesList
 * \return
 */
const QList<QPointer<FeatureWrapper> > &OiJob::getGeometriesList() const{
    return this->featureContainer.getGeometriesList();
}

/*!
 * \brief OiJob::getFeatureById
 * \param featureId
 * \return
 */
QPointer<FeatureWrapper> OiJob::getFeatureById(const int &featureId) const{
    return this->featureContainer.getFeatureById(featureId);
}

/*!
 * \brief OiJob::getFeaturesByName
 * \param name
 * \return
 */
QList<QPointer<FeatureWrapper> > OiJob::getFeaturesByName(const QString &name) const{
    return this->featureContainer.getFeaturesByName(name);
}

/*!
 * \brief OiJob::getFeaturesByGroup
 * \param group
 * \return
 */
QList<QPointer<FeatureWrapper> > OiJob::getFeaturesByGroup(const QString &group) const{
    return this->featureContainer.getFeaturesByGroup(group);
}

/*!
 * \brief OiJob::getFeaturesByType
 * \param type
 * \return
 */
QList<QPointer<FeatureWrapper> > OiJob::getFeaturesByType(const FeatureTypes &type) const{
    return this->featureContainer.getFeaturesByType(type);
}

/*!
 * \brief OiJob::getActiveFeature
 * \return
 */
const QPointer<FeatureWrapper> &OiJob::getActiveFeature() const{
    return this->activeFeature;
}

/*!
 * \brief OiJob::getActiveStation
 * \return
 */
const QPointer<Station> &OiJob::getActiveStation() const{
    return this->activeStation;
}

/*!
 * \brief OiJob::getActiveCoordinateSystem
 * \return
 */
const QPointer<CoordinateSystem> &OiJob::getActiveCoordinateSystem() const{
    return this->activeCoordinateSystem;
}

/*!
 * \brief OiJob::getActiveGroup
 * \return
 */
const QString &OiJob::getActiveGroup() const{
    return this->activeGroup;
}

/*!
 * \brief OiJob::setActiveGroup
 * \param group
 * \return
 */
bool OiJob::setActiveGroup(const QString &group){
    if(this->featureContainer.getFeatureGroupList().contains(group) && this->activeGroup.compare(group) != 0){
        this->activeGroup = group;
        emit this->activeGroupChanged();
        return true;
    }
    return false;
}

/*!
 * \brief OiJob::addFeature
 * \param feature
 * \return
 */
bool OiJob::addFeature(const QPointer<FeatureWrapper> &feature){

    //check if feature is valid
    if(!feature.isNull() && !feature->getFeature().isNull()){
        return false;
    }

    //check if feature with this id already exists
    if(this->featureContainer.getFeatureIdList().contains(feature->getFeature()->getId())){
        feature->getFeature()->setId(this->generateUniqueId());
    }
    this->nextId = feature->getFeature()->getId() + 1;

    //check feature name
    bool isNominal = (!feature->getGeometry().isNull() && feature->getGeometry()->getIsNominal());
    QPointer<CoordinateSystem> nominalSystem = isNominal?feature->getGeometry()->getNominalSystem():QPointer<CoordinateSystem>(NULL);
    if(!this->validateFeatureName(feature->getFeature()->getFeatureName(), feature->getFeatureTypeEnum(),
                                 isNominal, nominalSystem)){
        emit this->sendMessage("feature name already exists");
        return false;
    }

    //add nominal to nominal list of coordinate system
    if(isNominal && !nominalSystem.isNull()){
        feature->getGeometry()->getNominalSystem()->addNominal(feature);
    }

    //add the feature to the internal lists and maps
    this->featureContainer.addFeature(feature);

    //connect the feature's signals to slots in OiJob
    this->connectFeature(feature);

    //if a group is set for the new feature emit the group changed signal
    if(feature->getFeature()->getGroupName().compare("") != 0
            && this->featureContainer.getFeaturesByGroup(feature->getFeature()->getGroupName()).size() == 1){
        emit this->availableGroupsChanged();
    }

    //features added signals
    emit this->featureSetChanged();
    if(feature->getFeatureTypeEnum() == eCoordinateSystemFeature){
        emit this->coordSystemSetChanged();
    }else if(feature->getFeatureTypeEnum() == eStationFeature){
        emit this->stationSetChanged();
    }else if(feature->getFeatureTypeEnum() == eTrafoParamFeature){
        emit this->trafoParamSetChanged();
    }else{
        emit this->geometrySetChanged();
    }

    return true;

}

/*!
 * \brief OiJob::addFeatures
 * \param fAttr
 * \return
 */
QList<QPointer<FeatureWrapper> > OiJob::addFeatures(const FeatureAttributes &fAttr){

    QList<QPointer<FeatureWrapper> > result;

    //at least one of isNominal and isActual has to be set to true
    if(!fAttr.isNominal && !fAttr.isActual){
        return result;
    }

    //check nominal system is valid (only if isNominal = true)
    QPointer<FeatureWrapper> nominalSystem;
    if(fAttr.isNominal){
        QList<QPointer<FeatureWrapper> > features = this->featureContainer.getFeaturesByName(fAttr.nominalSystem);
        if(features.size() == 0){
            return result;
        }
        nominalSystem = features.at(0);
        if(nominalSystem.isNull() || nominalSystem->getCoordinateSystem().isNull()){
            return result;
        }
    }

    //check feature name
    QStringList featureNames = this->createFeatureNames(fAttr.name, fAttr.count);
    if(fAttr.isNominal){
        foreach(const QString &name, featureNames){
            if(!this->validateFeatureName(name, fAttr.typeOfFeature, true, nominalSystem)){
                return result;
            }
        }
    }
    if(fAttr.isActual){
        foreach(const QString &name, featureNames){
            if(!this->validateFeatureName(name, fAttr.typeOfFeature)){
                return result;
            }
        }
    }

    /*if(featureNames.size() != fAttr.count){
        return result;
    }
    QSet<QString> intersection = featureNames.toSet().intersect(this->featureContainer.getFeatureNameList().toSet());
    if(intersection.size() > 0){
        emit this->sendMessage("feature name already exists");
        return result;
    }*/

    //create features and add them to internal lists and maps
    for(int i = 0; i < fAttr.count; i++){

        QPointer<FeatureWrapper> feature = new FeatureWrapper();
        switch(fAttr.typeOfFeature){
        case ePointFeature: {
            myFeature->setPoint(myPoint);
            break;
        }case eLineFeature: {
            Line *myLine = new Line(nominal);
            //myLine->setMeasurementConfig(Line::defaultMeasurementConfig);
            myFeature->setLine(myLine);
            break;
        }case ePlaneFeature:{
            Plane *myPlane = new Plane(nominal);
            //myPlane->setMeasurementConfig(Plane::defaultMeasurementConfig);
            myFeature->setPlane(myPlane);
            break;
        }case eSphereFeature:{
            Sphere *mySphere = new Sphere(nominal);
            //mySphere->setMeasurementConfig(Sphere::defaultMeasurementConfig);
            myFeature->setSphere(mySphere);
            break;
        }case eCircleFeature:{
            Circle *myCircle = new Circle(nominal);
            //myCircle->setMeasurementConfig(Circle::defaultMeasurementConfig);
            myFeature->setCircle(myCircle);
            break;
        }case eConeFeature:{
            Cone *myCone = new Cone(nominal);
            //myCone->setMeasurementConfig(Cone::defaultMeasurementConfig);
            myFeature->setCone(myCone);
            break;
        }case eCylinderFeature:{
            Cylinder *myCylinder = new Cylinder(nominal);
            //myCylinder->setMeasurementConfig(Cylinder::defaultMeasurementConfig);
            myFeature->setCylinder(myCylinder);
            break;
        }case eEllipsoidFeature:{
            Ellipsoid *myEllipsoid = new Ellipsoid(nominal);
            //myEllipsoid->setMeasurementConfig(Ellipsoid::defaultMeasurementConfig);
            myFeature->setEllipsoid(myEllipsoid);
            break;
        }case eHyperboloidFeature:{
            Hyperboloid *myHyperboloid = new Hyperboloid(nominal);
            //myHyperboloid->setMeasurementConfig(Hyperboloid::defaultMeasurementConfig);
            myFeature->setHyperboloid(myHyperboloid);
            break;
        }case eParaboloidFeature:{
            Paraboloid *myParaboloid = new Paraboloid(nominal);
            //myParaboloid->setMeasurementConfig(Paraboloid::defaultMeasurementConfig);
            myFeature->setParaboloid(myParaboloid);
            break;
        }case ePointCloudFeature:{
            PointCloud *myPointCloud = new PointCloud(nominal);
            //myPointCloud->setMeasurementConfig(PointCloud::defaultMeasurementConfig);
            myFeature->setPointCloud(myPointCloud);
            break;
        }case eNurbsFeature:{
            Nurbs *myNurbs = new Nurbs(nominal);
            //myNurbs->setMeasurementConfig(Nurbs::defaultMeasurementConfig);
            myFeature->setNurbs(myNurbs);
            break;
        }case eStationFeature:{
            Station *myStation = new Station(name);
            myFeature->setStation(myStation);
            break;
        }case eCoordinateSystemFeature:{
            CoordinateSystem *myCoordinateSystem = new CoordinateSystem();
            myFeature->setCoordinateSystem(myCoordinateSystem);
            break;
        }case eTrafoParamFeature:{
            TrafoParam *myTrafoParam = new TrafoParam();
            myTrafoParam->setCoordinateSystems(attributes.startSystem, attributes.destSystem);
            myTrafoParam->setIsMovement(attributes.isMovement);
            for(int i=0; i<OiFeatureState::getCoordinateSystems().size();i++){
                if(OiFeatureState::getCoordinateSystems().at(i) == attributes.startSystem){
                    OiFeatureState::getCoordinateSystems().at(i)->addTransformationParameter(myTrafoParam);
                }
                if(OiFeatureState::getCoordinateSystems().at(i) == attributes.destSystem){
                    OiFeatureState::getCoordinateSystems().at(i)->addTransformationParameter(myTrafoParam);
                }
            }
            for(int i=0; i<OiFeatureState::getStations().size();i++){
                if(OiFeatureState::getStations().at(i)->coordSys == attributes.startSystem){
                    OiFeatureState::getStations().at(i)->coordSys->addTransformationParameter(myTrafoParam);
                }
                if(OiFeatureState::getStations().at(i)->coordSys == attributes.destSystem){
                    OiFeatureState::getStations().at(i)->coordSys->addTransformationParameter(myTrafoParam);
                }
            }
            myFeature->setTrafoParam(myTrafoParam);
            break;
        }case eScalarEntityAngleFeature:{
            ScalarEntityAngle *myAngle = new ScalarEntityAngle(nominal);
            //myAngle->setMeasurementConfig(ScalarEntityAngle::defaultMeasurementConfig);
            myFeature->setScalarEntityAngle(myAngle);
            break;
        }case eScalarEntityDistanceFeature:{
            ScalarEntityDistance *myDistance = new ScalarEntityDistance(nominal);
            //myDistance->setMeasurementConfig(ScalarEntityDistance::defaultMeasurementConfig);
            myFeature->setScalarEntityDistance(myDistance);
            break;
        }case eScalarEntityTemperatureFeature:{
            ScalarEntityTemperature *myTemperature = new ScalarEntityTemperature(nominal);
            //myTemperature->setMeasurementConfig(ScalarEntityTemperature::defaultMeasurementConfig);
            myFeature->setScalarEntityTemperature(myTemperature);
            break;
        }case eScalarEntityMeasurementSeriesFeature:{
            ScalarEntityMeasurementSeries *myMeasurementSeries = new ScalarEntityMeasurementSeries(nominal);
            //myMeasurementSeries->setMeasurementConfig(ScalarEntityMeasurementSeries::defaultMeasurementConfig);
            myFeature->setScalarEntityMeasurementSeries(myMeasurementSeries);
            break;
        }default:
            break;
        }

    }





    //features added signals
    emit this->featureSetChanged();
    if(feature->getFeatureTypeEnum() == eCoordinateSystemFeature){
        emit this->coordSystemSetChanged();
    }else if(feature->getFeatureTypeEnum() == eStationFeature){
        emit this->stationSetChanged();
    }else if(feature->getFeatureTypeEnum() == eTrafoParamFeature){
        emit this->trafoParamSetChanged();
    }else{
        emit this->geometrySetChanged();
    }










    //get the feature name
    QString name;
    int index;
    this->createFeatureName(name, index, attributes.name, attributes.count);

    bool nominal = attributes.isNominal;

    int numIterations = 1;

    if(attributes.isNominal && attributes.isActual && getIsGeometry(attributes.typeOfFeature)){
        numIterations++;
    }

    for(int j = 0; j < numIterations; j++){

        //create all features
        for(int i = 0; i < attributes.count; i++){

            //create feature + feature wrapper and set measurement config
            FeatureWrapper *myFeature = new FeatureWrapper();
            switch(attributes.typeOfFeature){
            case ePointFeature: {
                Point *myPoint = new Point(nominal);
                //myPoint->setMeasurementConfig(Point::defaultMeasurementConfig);
                myFeature->setPoint(myPoint);
                break;
            }case eLineFeature: {
                Line *myLine = new Line(nominal);
                //myLine->setMeasurementConfig(Line::defaultMeasurementConfig);
                myFeature->setLine(myLine);
                break;
            }case ePlaneFeature:{
                Plane *myPlane = new Plane(nominal);
                //myPlane->setMeasurementConfig(Plane::defaultMeasurementConfig);
                myFeature->setPlane(myPlane);
                break;
            }case eSphereFeature:{
                Sphere *mySphere = new Sphere(nominal);
                //mySphere->setMeasurementConfig(Sphere::defaultMeasurementConfig);
                myFeature->setSphere(mySphere);
                break;
            }case eCircleFeature:{
                Circle *myCircle = new Circle(nominal);
                //myCircle->setMeasurementConfig(Circle::defaultMeasurementConfig);
                myFeature->setCircle(myCircle);
                break;
            }case eConeFeature:{
                Cone *myCone = new Cone(nominal);
                //myCone->setMeasurementConfig(Cone::defaultMeasurementConfig);
                myFeature->setCone(myCone);
                break;
            }case eCylinderFeature:{
                Cylinder *myCylinder = new Cylinder(nominal);
                //myCylinder->setMeasurementConfig(Cylinder::defaultMeasurementConfig);
                myFeature->setCylinder(myCylinder);
                break;
            }case eEllipsoidFeature:{
                Ellipsoid *myEllipsoid = new Ellipsoid(nominal);
                //myEllipsoid->setMeasurementConfig(Ellipsoid::defaultMeasurementConfig);
                myFeature->setEllipsoid(myEllipsoid);
                break;
            }case eHyperboloidFeature:{
                Hyperboloid *myHyperboloid = new Hyperboloid(nominal);
                //myHyperboloid->setMeasurementConfig(Hyperboloid::defaultMeasurementConfig);
                myFeature->setHyperboloid(myHyperboloid);
                break;
            }case eParaboloidFeature:{
                Paraboloid *myParaboloid = new Paraboloid(nominal);
                //myParaboloid->setMeasurementConfig(Paraboloid::defaultMeasurementConfig);
                myFeature->setParaboloid(myParaboloid);
                break;
            }case ePointCloudFeature:{
                PointCloud *myPointCloud = new PointCloud(nominal);
                //myPointCloud->setMeasurementConfig(PointCloud::defaultMeasurementConfig);
                myFeature->setPointCloud(myPointCloud);
                break;
            }case eNurbsFeature:{
                Nurbs *myNurbs = new Nurbs(nominal);
                //myNurbs->setMeasurementConfig(Nurbs::defaultMeasurementConfig);
                myFeature->setNurbs(myNurbs);
                break;
            }case eStationFeature:{
                Station *myStation = new Station(name);
                myFeature->setStation(myStation);
                break;
            }case eCoordinateSystemFeature:{
                CoordinateSystem *myCoordinateSystem = new CoordinateSystem();
                myFeature->setCoordinateSystem(myCoordinateSystem);
                break;
            }case eTrafoParamFeature:{
                TrafoParam *myTrafoParam = new TrafoParam();
                myTrafoParam->setCoordinateSystems(attributes.startSystem, attributes.destSystem);
                myTrafoParam->setIsMovement(attributes.isMovement);
                for(int i=0; i<OiFeatureState::getCoordinateSystems().size();i++){
                    if(OiFeatureState::getCoordinateSystems().at(i) == attributes.startSystem){
                        OiFeatureState::getCoordinateSystems().at(i)->addTransformationParameter(myTrafoParam);
                    }
                    if(OiFeatureState::getCoordinateSystems().at(i) == attributes.destSystem){
                        OiFeatureState::getCoordinateSystems().at(i)->addTransformationParameter(myTrafoParam);
                    }
                }
                for(int i=0; i<OiFeatureState::getStations().size();i++){
                    if(OiFeatureState::getStations().at(i)->coordSys == attributes.startSystem){
                        OiFeatureState::getStations().at(i)->coordSys->addTransformationParameter(myTrafoParam);
                    }
                    if(OiFeatureState::getStations().at(i)->coordSys == attributes.destSystem){
                        OiFeatureState::getStations().at(i)->coordSys->addTransformationParameter(myTrafoParam);
                    }
                }
                myFeature->setTrafoParam(myTrafoParam);
                break;
            }case eScalarEntityAngleFeature:{
                ScalarEntityAngle *myAngle = new ScalarEntityAngle(nominal);
                //myAngle->setMeasurementConfig(ScalarEntityAngle::defaultMeasurementConfig);
                myFeature->setScalarEntityAngle(myAngle);
                break;
            }case eScalarEntityDistanceFeature:{
                ScalarEntityDistance *myDistance = new ScalarEntityDistance(nominal);
                //myDistance->setMeasurementConfig(ScalarEntityDistance::defaultMeasurementConfig);
                myFeature->setScalarEntityDistance(myDistance);
                break;
            }case eScalarEntityTemperatureFeature:{
                ScalarEntityTemperature *myTemperature = new ScalarEntityTemperature(nominal);
                //myTemperature->setMeasurementConfig(ScalarEntityTemperature::defaultMeasurementConfig);
                myFeature->setScalarEntityTemperature(myTemperature);
                break;
            }case eScalarEntityMeasurementSeriesFeature:{
                ScalarEntityMeasurementSeries *myMeasurementSeries = new ScalarEntityMeasurementSeries(nominal);
                //myMeasurementSeries->setMeasurementConfig(ScalarEntityMeasurementSeries::defaultMeasurementConfig);
                myFeature->setScalarEntityMeasurementSeries(myMeasurementSeries);
                break;
            }default:
                break;
            }

            //set feature attributes
            QString featureName = name;
            if(attributes.count > 1){
                featureName = name + QString::number(index+i);
            }
            myFeature->getFeature()->setFeatureName(featureName);
            myFeature->getFeature()->setGroupName(attributes.group);
            if(myFeature->getGeometry() != NULL){
                myFeature->getGeometry()->setCommonState(attributes.isCommon);
                if(myFeature->getGeometry()->getIsNominal()){
                    //myFeature->getGeometry()->setNominalSystem(attributes.nominalSystem);
                }
            }

            //add the feature to the list of features, stations, coordinate systems, trafo params and geometries
            this->myFeatureContainer.addFeature(myFeature);

            //add nominal to nominal list of coordinate system
            if(myFeature->getGeometry() != NULL && myFeature->getGeometry()->getNominalSystem() != NULL){
                myFeature->getGeometry()->getNominalSystem()->addNominal(myFeature);
            }

            //connect the feature's signals to slots in OiFeatureState
            this->connectFeature(myFeature);

            //set function
            if(!nominal){
                QString filePath;
                SystemDbManager::getFunctionPluginFilePath(filePath, attributes.functionPlugin.first, attributes.functionPlugin.second);
                Function *checkFunction = PluginLoader::loadFunctionPlugin(filePath, attributes.functionPlugin.second);

                myFeature->getFeature()->addFunction(checkFunction);
            }

            result.append(myFeature);

        }

        nominal = !nominal;

    }

    return result;

}

/*!
 * \brief OiJob::addFeatures
 * \param features
 * \return
 */
bool OiJob::addFeatures(const QList<QPointer<FeatureWrapper> > &features){

}

/*!
 * \brief OiJob::removeFeature
 * \param featureId
 * \return
 */
bool OiJob::removeFeature(const int &featureId){

}

/*!
 * \brief OiJob::removeFeature
 * \param feature
 * \return
 */
bool OiJob::removeFeature(const QPointer<FeatureWrapper> &feature){

}

/*!
 * \brief OiJob::removeAll
 */
void OiJob::removeAll(){
    this->featureContainer.removeAll();
}

/*!
 * \brief OiJob::setActiveFeature
 * \param featureId
 */
void OiJob::setActiveFeature(const int &featureId){

}

void OiJob::setActiveStation(const int &featureId)
{

}

void OiJob::setActiveCoordinateSystem(const int &featureId)
{

}

void OiJob::setFeatureName(const int &featureId, const QString &oldName)
{

}

void OiJob::setFeatureGroup(const int &featureId, const QString &oldGroup)
{

}

void OiJob::setFeatureComment(const int &featureId)
{

}

void OiJob::setFeatureIsUpdated(const int &featureId)
{

}

void OiJob::setFeatureIsSolved(const int &featureId)
{

}

void OiJob::setFeatureFunctions(const int &featureId)
{

}

void OiJob::setFeatureUsedFor(const int &featureId)
{

}

void OiJob::setFeaturePreviouslyNeeded(const int &featureId)
{

}

void OiJob::setGeometryIsCommon(const int &featureId)
{

}

void OiJob::setGeometryNominals(const int &featureId)
{

}

void OiJob::setGeometryActual(const int &featureId)
{

}

void OiJob::setGeometryObservations(const int &featureId)
{

}

void OiJob::setGeometryNominalSystem(const int &featureId)
{

}

void OiJob::setGeometryStatistic(const int &featureId)
{

}

void OiJob::setGeometrySimulationData(const int &featureId)
{

}

void OiJob::setGeometryMeasurementConfig(const int &featureId)
{

}

void OiJob::setSystemObservations(const int &featureId, const int &obsId)
{

}

void OiJob::setSystemTrafoParams(const int &featureId)
{

}

void OiJob::setSystemsNominals(const int &featureId)
{

}

void OiJob::setStationSensor(const int &featureId)
{

}

void OiJob::setTrafoParamParameters(const int &featureId)
{

}

void OiJob::setTrafoParamSystems(const int &featureId)
{

}

void OiJob::setTrafoParamIsUsed(const int &featureId)
{

}

void OiJob::setTrafoParamValidTime(const int &featureId)
{

}

void OiJob::setTrafoParamIsMovement(const int &featureId)
{

}

void OiJob::connectFeature(const QPointer<FeatureWrapper> &feature)
{

}

void OiJob::disconnectFeature(const QPointer<FeatureWrapper> &feature)
{

}

/*!
 * \brief OiJob::createFeatureNames
 * \param name
 * \return
 */
QStringList OiJob::createFeatureNames(const QString &name, const int &count) const{

    QStringList result;

    //split base name from postfix
    QString baseName;
    int postFix;
    int index = name.lastIndexOf(QRegExp("[0-9]*$"), name.length()-1);
    if(index == -1){
        baseName = name;
        postFix = 1;
    }else{
        baseName = QStringRef(name, 0, index);
        postFix = QStringRef(name, index, name.length() - index).toInt();
    }

    for(int i = 0; i < count; i++){
        result.append(QString("%1%2").arg(baseName).arg(postFix));
    }

    return result;

}
