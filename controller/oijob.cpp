#include "oijob.h"

OiJob::OiJob(QObject *parent) : QObject(parent){

    this->currentId = 0;

}

/*
bool sortID(FeatureWrapper *f1, FeatureWrapper *f2){
   return f1->getFeature()->getId() < f2->getFeature()->getId();
}*/

/*!
 * \brief OiFeatureState::getFeatureCount
 * \return
 */
int OiJob::getFeatureCount(){
    return this->myFeatureContainer.getFeatureCount();
}

/*!
 * \brief OiFeatureState::getStationCount
 * \return
 */
int OiJob::getStationCount(){
    return this->myFeatureContainer.getStationCount();
}

/*!
 * \brief OiFeatureState::getCoordinateSystemCount
 * \return
 */
int OiJob::getCoordinateSystemCount(){
    return this->myFeatureContainer.getCoordinateSystemCount();
}

/*!
 * \brief OiFeatureState::getGeometryCount
 * \return
 */
int OiJob::getGeometryCount(){
    return this->myFeatureContainer.getGeometryCount();
}

/*!
 * \brief OiFeatureState::getTrafoParamCount
 * \return
 */
int OiJob::getTrafoParamCount(){
    return this->myFeatureContainer.getTransformationParameterCount();
}

/*!
 * \brief OiFeatureState::getFeatures
 * \return
 */
const QList<FeatureWrapper *> &OiJob::getFeatures(){
    return this->myFeatureContainer.getFeaturesList();
}

/*!
 * \brief OiFeatureState::getStations
 * \return
 */
const QList<Station *> &OiJob::getStations(){
    return this->myFeatureContainer.getStationsList();
}

/*!
 * \brief OiFeatureState::getCoordinateSystems
 * Returns a list of all nominal coordinate systems
 * \return
 */
const QList<CoordinateSystem *> &OiJob::getCoordinateSystems(){
    return this->myFeatureContainer.getCoordinateSystemsList();
}

/*!
 * \brief OiFeatureState::getTransformationParameters
 * \return
 */
const QList<TrafoParam *> &OiJob::getTransformationParameters(){
    return this->myFeatureContainer.getTransformationParametersList();
}

/*!
 * \brief OiFeatureState::getGeometries
 * \return
 */
const QList<FeatureWrapper *> &OiJob::getGeometries(){
    return this->myFeatureContainer.getGeometriesList();
}

/*!
 * \brief OiFeatureState::activateFeature
 * \param featureId
 * \return
 */
bool OiJob::activateFeature(const int &featureId){
    FeatureWrapper *myFeature = this->getFeature(featureId);
    if(myFeature != NULL && myFeature->getFeature() != NULL){
        myFeature->getFeature()->setActiveFeatureState(true);
        return true;
    }
    return false;
}

/*!
 * \brief OiFeatureState::activateStation
 * \param featureId
 * \return
 */
bool OiJob::activateStation(const int &featureId){
    FeatureWrapper *myFeature = this->getFeature(featureId);
    if(myFeature != NULL && myFeature->getStation() != NULL){
        myFeature->getStation()->setActiveStationState(true);
        return true;
    }
    return false;
}

/*!
 * \brief OiFeatureState::activateCoordinateSystem
 * \param featureId
 * \return
 */
bool OiJob::activateCoordinateSystem(const int &featureId){
    FeatureWrapper *myFeature = this->getFeature(featureId);
    if(myFeature != NULL && myFeature->getCoordinateSystem() != NULL){
        myFeature->getCoordinateSystem()->setActiveCoordinateSystemState(true);
        return true;
    }
    return false;
}

/*!
 * \brief OiFeatureState::getFeaturesOfGroup
 * Get all features of a special group
 * \param group
 * \return
 */
QList<FeatureWrapper *> OiJob::getFeaturesByGroup(const QString &group){
    return this->myFeatureContainer.getFeaturesByGroup(group);
}

/*!
 * \brief OiFeatureState::getFeaturesWithName
 * \param name
 * \return
 */
QList<FeatureWrapper *> OiJob::getFeaturesByName(const QString &name){
    return this->myFeatureContainer.getFeaturesByName(name);
}

/*!
 * \brief OiFeatureState::getActiveFeature
 * Returns a pointer to the active feature's feature wrapper or NULL.
 * \return
 */
FeatureWrapper* OiJob::getActiveFeature(){
    return this->myActiveFeature;
}

/*!
 * \brief OiFeatureState::getActiveStation
 * Returns a pointer to the active station or NULL.
 * \return
 */
Station* OiJob::getActiveStation(){
    return this->myActiveStation;
}

/*!
 * \brief OiFeatureState::getActiveCoordinateSystem
 * Returns a pointer to the active coordinate system or NULL.
 * \return
 */
CoordinateSystem* OiJob::getActiveCoordinateSystem(){
    return this->myActiveCoordinateSystem;
}

/*!
 * \brief OiFeatureState::getAvailableGroups
 * \return
 */
QStringList OiJob::getAvailableGroups(){
    return this->myFeatureContainer.getFeatureGroupList();
}

/*!
 * \brief OiFeatureState::getActiveGroup
 * \return
 */
const QString &OiJob::getActiveGroup(){
    return this->myActiveGroup;
}

/*!
 * \brief OiFeatureState::setActiveGroup
 */
bool OiJob::setActiveGroup(const QString &group){
    if(this->myFeatureContainer.getFeatureGroupList().contains(group)){
        this->myActiveGroup = group;
        return true;
    }
    return false;
}

/*!
 * \brief OiJob::generateUniqueId
 * \return
 */
int OiJob::generateUniqueId(){
    this->currentId++;
    return this->currentId;
}

/*!
 * \brief OiFeatureState::getFeature
 * \param featureId
 * \return
 */
FeatureWrapper *OiJob::getFeature(const int &featureId){
    return this->myFeatureContainer.getFeatureById(featureId);
}

/*!
 * \brief OiJob::getFeaturesByFeatureType
 * \param type
 * \return
 */
QList<FeatureWrapper *> OiJob::getFeaturesByFeatureType(const FeatureTypes &type){
    return this->myFeatureContainer.getFeaturesByType(type);
}

/*!
 * \brief OiFeatureState::getNominalSystem
 * \param name
 * \return
 */
/*CoordinateSystem *OiFeatureState::getNominalSystem(QString name){
    foreach(CoordinateSystem *c, OiFeatureState::myFeatureContainer.getCoordinateSystemsList()){
        if(c != NULL && c->getFeatureName().compare(name) == 0){
            return c;
        }
    }
    return NULL;
}*/

/*!
 * \brief OiFeatureState::resetFeatureLists
 * Delete all OpenIndy features and clear all feature lists
 */
/*void OiFeatureState::resetFeatureLists(){

    //reset active states
    myActiveFeature = NULL;
    myActiveStation = NULL;
    myActiveCoordinateSystem = NULL;
    myActiveGroup = "All Groups";

    //get a list of feature-ids and delete them
    QList<int> featureIds = OiFeatureState::myFeatureContainer.getFeatureIdList();
    foreach(int id, featureIds){
        OiFeatureState::myFeatureContainer.removeAndDeleteFeature(id);
    }

    OiFeatureState::getInstance()->emitSignal(OiFeatureState::eFeatureSetChanged);

}*/

/*
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
}*/

/*!
 * \brief OiFeatureState::addFeature
 * Adds a feature to the list of features
 * \param feature
 * \return
 */
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

/*!
 * \brief OiFeatureState::addFeatures
 * Creates and adds features by using the given feature attributes
 * \param attributes
 * \return
 */
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

/*!
 * \brief OiFeatureState::addFeatures
 * \param myFeatures
 * \return
 */
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

/*!
 * \brief OiFeatureState::removeFeature
 * \param feature
 * \return
 */
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

/*
void OiJob::emitSignal(this->SignalType mySignalType){
    switch(mySignalType){
    case eActiveCoordinateSystemChanged:
        emit this->activeCoordinateSystemChanged();
        break;
    case eActiveFeatureChanged:
        emit this->activeFeatureChanged();
        break;
    case eActiveStationChanged:
        emit this->activeStationChanged();
        break;
    case eAvailableGroupsChanged:
        emit this->availableGroupsChanged();
        break;
    case eFeatureSetChanged:
        emit this->featureSetChanged();
        break;
    case eCoordSysSetChanged:
        emit this->coordSystemSetChanged();
        break;
    case eFeatureAttributesChanged:
        emit this->featureAttributesChanged();
        break;
    case eGeomObservationsChanged:
        emit this->geometryObservationsChanged();
        break;
    case eSystemObservationsChanged:
        emit this->systemObservationsAdded();
        break;
    }
}*/

/*!
 * \brief OiFeatureState::createFeatures
 * \param attributes
 * \return
 */
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
                /*TrafoParam *myTrafoParam = new TrafoParam();
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
                break;*/
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
            /*if(!nominal){
                QString filePath;
                SystemDbManager::getFunctionPluginFilePath(filePath, attributes.functionPlugin.first, attributes.functionPlugin.second);
                Function *checkFunction = PluginLoader::loadFunctionPlugin(filePath, attributes.functionPlugin.second);

                myFeature->getFeature()->addFunction(checkFunction);
            }*/

            result.append(myFeature);

        }

        nominal = !nominal;

    }

    return result;

}

/*!
 * \brief OiFeatureState::createFeatureName
 * Creates a feature name from an input name
 * \param outputName: created name
 * \param index: numerical postfix
 * \param inputName: input feature name that may contain a name and a numerical postfix
 * \param count: number of features to be created
 */
void OiJob::createFeatureName(QString &outputName, int &index, QString inputName, int count){

    int startIndex = inputName.size() - 1;

    //split the input name into name and postfix
    while(startIndex > 0 && inputName.at(startIndex).isDigit()){
        startIndex--;
    }
    outputName = inputName.mid(0, startIndex + 1);
    index = inputName.mid(startIndex, inputName.size() - startIndex - 1).toInt();

}

/*!
 * \brief OiFeatureState::connectFeature
 * \param myFeature
 */
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
        connect(myFeature->getStation()->coordSys, SIGNAL(activeCoordinateSystemChanged(int)),
                this, SLOT(setActiveCoordinateSystem(int)), Qt::DirectConnection);
        connect(myFeature->getStation()->coordSys, SIGNAL(observationsChanged(int, int)),
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

/*!
 * \brief OiFeatureState::disconnectFeature
 * \param myFeature
 */
void OiJob::disconnectFeature(FeatureWrapper *myFeature){
    disconnect(myFeature->getFeature(), SIGNAL(featureIsActiveChanged(int)),
            this, SLOT(setActiveFeature(int)));
    disconnect(myFeature->getFeature(), SIGNAL(featureGroupChanged(int)),
               this, SLOT(setFeatureGroup(int)));

    //call OiConfigState's disconnect method
    //OiConfigState::disconnectFeature(myFeature);
}

/*!
 * \brief OiFeatureState::validateFeatureName
 * \param featureType the type of the feature that shall be added
 * \param featureName the name of the feature that shall be added
 * \param isNominal true if a nominal shall be added, false if not
 * \param myNomSys a pointer to the nominal system of the feature (only if isNominal = true)
 * \return
 */
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

/*!
 * \brief OiFeatureState::setActiveFeature
 * \param featureId
 */
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

/*!
 * \brief OiFeatureState::setActiveStation
 * \param featureId
 */
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

/*!
 * \brief OiFeatureState::setActiveCoordinateSystem
 * \param featureId
 */
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

/*!
 * \brief OiFeatureState::setFeatureGroup
 * \param featureId
 * \param oldGroup
 */
void OiJob::setFeatureGroup(const int &featureId, const QString &oldGroup){
    this->myFeatureContainer.featureGroupChanged(featureId, oldGroup);
    emit this->availableGroupsChanged();
}

/*!
 * \brief OiFeatureState::setFeatureName
 * \param featureId
 * \param oldName
 */
void OiJob::setFeatureName(const int &featureId, const QString &oldName){
    this->myFeatureContainer.featureNameChanged(featureId, oldName);
    emit this->featureAttributesChanged();
}

/*!
 * \brief OiFeatureState::setFeatureComment
 * \param featureId
 */
void OiJob::setFeatureComment(const int &featureId){
    emit this->featureAttributesChanged();
}

/*!
 * \brief OiFeatureState::setFeatureFunctions
 * \param featureId
 */
void OiJob::setFeatureFunctions(const int &featureId){
    emit this->featureFunctionsChanged();
}

/*!
 * \brief OiFeatureState::setGeometryActual
 * \param featureId
 */
void OiJob::setGeometryActual(const int &featureId){
    try{

        FeatureWrapper *myNominal = this->getFeature(featureId);

        if(myNominal == NULL || myNominal->getGeometry() == NULL){
            return;
        }


        myNominal->getGeometry()->getActual();
        FeatureWrapper *newActual;
        newActual= new FeatureWrapper();

        /*switch (myNominal->getTypeOfFeature()) {
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
        }*/

        emit this->featureSetChanged();

    }catch(exception &e){
        Console::addLine(e.what());
    }
}

/*!
 * \brief OiFeatureState::setGeometryNominals
 * \param featureId
 */
void OiJob::setGeometryNominals(const int &featureId){
    emit this->featureSetChanged();
}

/*!
 * \brief OiFeatureState::setGeometryObservations
 * \param featureId
 */
void OiJob::setGeometryObservations(const int &featureId){
    emit this->geometryObservationsChanged();
}

/*!
 * \brief OiFeatureState::setGeometryMeasurementConfig
 * \param featureId
 */
void OiJob::setGeometryMeasurementConfig(const int &featureId){
    emit this->geometryMeasurementConfigChanged();
}

/*!
 * \brief OiFeatureState::setSystemsNominals
 * \param featureId
 */
void OiJob::setSystemsNominals(const int &featureId){
    try{

        /*int featureIndex = OiFeatureState::getFeatureListIndex(featureId);
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

        }*/

    }catch(exception &e){
        Console::addLine(e.what());
    }
}

/*!
 * \brief OiFeatureState::removeFeature
 * Remove a feature from lists and map
 * \param featureId
 */
void OiJob::removeFeature(const int &featureId){
    this->myFeatureContainer.removeFeature(featureId);
}
/*!
 * \brief OiFeatureState::addPCSegmentAsFeature
 * Add a segment, detected from a point cloud to the list of features in OpenIndy
 * \param segment
 */
void OiJob::addPCSegmentAsFeature(FeatureWrapper *segment){

    qDebug() << "pc segment as feature featurestate";

    this->addFeature(segment);

}

/*!
 * \brief OiFeatureState::setSystemObservations
 * \param featureId
 * \param obsId
 */
void OiJob::setSystemObservations(const int &featureId, const int &obsId){

}
