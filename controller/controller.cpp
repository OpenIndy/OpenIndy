#include "controller.h"

#include "feature.h"
#include "coordinatesystem.h"
#include "station.h"
#include "pi_lasertracker.h"

/*!
 * \brief Controller::Controller
 * \param parent
 */
Controller::Controller(QObject *parent) :
    QObject(parent)
{
    c = new Console(0);
    Console::c = c;

    this->myDeleteFeaturesCallback = new DeleteFeaturesFunctor();
    this->myDeleteFeaturesCallback->c = this;

    pluginsModel = new QSqlQueryModel();
    neededElementsModel = new QSqlQueryModel();

    this->activeStation = NULL;
    this->activeFeature = NULL;
    this->activeCoordinateSystem = NULL;

    lastmConfig = new MeasurementConfig();
    this->defaultLastmConfig();

    this->activeFeature = NULL;

    if(this->features.size()==0){
        CoordinateSystem *cs = new CoordinateSystem();
        cs->name ="PART";
        FeatureWrapper *fwPart = new FeatureWrapper();
        fwPart->setCoordinateSystem(cs);
        this->features.append(fwPart);
        this->coordSys.append(fwPart->getCoordinateSystem());

        Station *firstStation = new Station("STATION01");
        firstStation->position->mConfig = *this->lastmConfig;
        firstStation->position->isCommon = false;
        firstStation->position->isNominal = false;
        this->activeStation = firstStation;
        //TODO station solved Problem. siehe unten
        //this->activeStation->isSolved = true;
        FeatureWrapper *fwFirstStation = new FeatureWrapper();
        fwFirstStation->setStation(firstStation);

        this->features.append(fwFirstStation);

        this->stations.append(fwFirstStation->getStation());

        this->activeCoordinateSystem = activeStation->coordSys;

    }else{
        this->activeStation = NULL;
        this->activeCoordinateSystem  = NULL;
    }

    tblModel = new TableModel(features,activeStation,activeFeature);

    this->featureOverviewModel = new FeatureOvserviewProxyModel(this->features);
    this->featureOverviewModel->setSourceModel(this->tblModel);

    this->trafoParamModel = new TrafoParamProxyModel(this->features);
    this->trafoParamModel->setSourceModel(this->tblModel);

    this->functionTreeViewModel = new QStandardItemModel();
    this->functionTreeViewModel->setHorizontalHeaderItem(0, new QStandardItem("functions"));

    this->usedElementsModel = new UsedElementsModel();

    //set up feature treeview models
    this->featureTreeViewModel = new FeatureTreeViewModel(this->features);
    this->featureTreeViewModel->refreshModel();
    this->availableElementsModel = new AvailableElementsTreeViewProxyModel();
    this->availableElementsModel->setHeader("available elements");
    this->availableElementsModel->setSourceModel(this->featureTreeViewModel);
    this->featureGraphicsModel = new FeatureGraphicsTreeViewProxyModel();
    this->featureGraphicsModel->setHeader("feature overview");
    this->featureGraphicsModel->setSourceModel(this->featureTreeViewModel);

    //set up filter mechanism for available elements treeview
    connect(this, SIGNAL(sendAvailableElementsFilter(Configuration::ElementTypes,bool)), this->availableElementsModel, SLOT(setFilter(Configuration::ElementTypes,bool)));


    connect(PluginLoader::myMetaInfo,SIGNAL(sendMe(PluginMetaData*)),this,SLOT(savePluginData(PluginMetaData*)));


    connect(this->activeStation,SIGNAL(actionFinished(bool)),this,SLOT(showResults(bool)));
    connect(this,SIGNAL(refreshGUI(FeatureWrapper*,Station*)),this->tblModel,SLOT(updateModel(FeatureWrapper*,Station*)));

    emit refreshGUI(this->activeFeature,this->activeStation);
}

/*!
 * \brief Controller::addFeature
 * Creates new features with given parameters. After creating, the features are set to the feature wrapper and added
 * to the feature list in the controller.
 * \param count
 * \param featureType
 * \param name
 * \param actualNominal
 * \param isCommonPoint
 */
void Controller::addFeature(int count, int featureType, QString name, QString group, bool actualNominal, bool isCommonPoint, CoordinateSystem *nominalSystem){

    for(int k=0; k<count;k++){
        QString featureName;
        QString letterName;
        int numberName;

        if(k == 0){
            featureName = name;
        }else{
            featureName = name + QString::number(k);
        }

        for (int i=0; i<name.length();i++){
            if(name.at(i).isDigit()){
                letterName = name.left(i);
                numberName = name.mid(i).toInt();
                numberName += k;
                if(numberName == 0){
                    featureName = name;
                }else {
                    featureName = letterName + QString::number(numberName);
                }

                break;
            }
        }

        for(int k=0; k<this->features.size();k++){
            if(this->features.at(k)->getFeature()->name == featureName){
                if(actualNominal == true && this->features.at(k)->getFeature()->getDisplayIsNominal() == "true"){
                    if(this->features.at(k)->getGeometry() != NULL && this->features.at(k)->getGeometry()->myNominalCoordSys == nominalSystem){
                        Console::addLine("Feature name already exists");
                        return;
                    }
                }
                if(actualNominal == false && this->features.at(k)->getFeature()->getDisplayIsNominal() == "false"){
                    Console::addLine("Feature name already exists");
                    return;
                }
            }
        }

        switch(featureType){
        case Configuration::ePointFeature: {
            Point *tmpPoint = new Point();

            tmpPoint->mConfig = *this->lastmConfig;
            tmpPoint->name = featureName;
            tmpPoint->group = group;
            tmpPoint->isNominal = actualNominal;
            tmpPoint->isCommon = isCommonPoint;
            tmpPoint->myNominalCoordSys = nominalSystem;

            if(actualNominal){
                tmpPoint->isSolved = true;
            }

            FeatureWrapper *fw = new FeatureWrapper();
            fw->setPoint(tmpPoint);

            if(actualNominal){ //if nominal feature
                for(int i=0; i< this->coordSys.size();i++){
                    if(this->coordSys.at(i) == nominalSystem){
                        this->coordSys.at(i)->nominals.append(fw->getGeometry());
                        this->features.append(fw);
                        break;
                    }
                }
            }else{ //if no nominal feature
                this->features.append(fw);
            }

            break;
        }
        case Configuration::eLineFeature: {
            Line *tmp = new Line();

            tmp->mConfig = *this->lastmConfig;
            tmp->name = featureName;
            tmp->group = group;
            tmp->isNominal = actualNominal;
            tmp->isCommon = isCommonPoint;
            tmp->myNominalCoordSys = nominalSystem;

            if(actualNominal){
                tmp->isSolved = true;
            }

            FeatureWrapper *fw = new FeatureWrapper();
            fw->setLine(tmp);

            if(actualNominal){ //if nominal feature
                for(int i=0; i< this->coordSys.size();i++){
                    if(this->coordSys.at(i) == nominalSystem){
                        this->coordSys.at(i)->nominals.append(fw->getGeometry());
                        this->features.append(fw);
                        break;
                    }
                }
            }else{ //if no nominal feature
                this->features.append(fw);
            }

            break;
        }
        case Configuration::ePlaneFeature:{
            Plane *tmp = new Plane();

            tmp->mConfig = *this->lastmConfig;
            tmp->name = featureName;
            tmp->group = group;
            tmp->isNominal = actualNominal;
            tmp->isCommon = isCommonPoint;
            tmp->myNominalCoordSys = nominalSystem;

            if(actualNominal){
                tmp->isSolved = true;
            }

            FeatureWrapper *fw = new FeatureWrapper();
            fw->setPlane(tmp);

            if(actualNominal){ //if nominal feature
                for(int i=0; i< this->coordSys.size();i++){
                    if(this->coordSys.at(i) == nominalSystem){
                        this->coordSys.at(i)->nominals.append(fw->getGeometry());
                        this->features.append(fw);
                        break;
                    }
                }
            }else{ //if no nominal feature
                this->features.append(fw);
            }

            break;
        }
        case Configuration::eSphereFeature:{
            Sphere *tmp = new Sphere();

            tmp->mConfig = *this->lastmConfig;
            tmp->name = featureName;
            tmp->group = group;
            tmp->isNominal = actualNominal;
            tmp->isCommon = isCommonPoint;
            tmp->myNominalCoordSys = nominalSystem;

            if(actualNominal){
                tmp->isSolved = true;
            }

            FeatureWrapper *fw = new FeatureWrapper();
            fw->setSphere(tmp);

            if(actualNominal){ //if nominal feature
                for(int i=0; i< this->coordSys.size();i++){
                    if(this->coordSys.at(i) == nominalSystem){
                        this->coordSys.at(i)->nominals.append(fw->getGeometry());
                        this->features.append(fw);
                        break;
                    }
                }
            }else{ //if no nominal feature
                this->features.append(fw);
            }

            break;
        }
        case Configuration::eCircleFeature:{
            Circle *tmp = new Circle();

            tmp->mConfig = *this->lastmConfig;
            tmp->name = featureName;
            tmp->group = group;
            tmp->isNominal = actualNominal;
            tmp->isCommon = isCommonPoint;
            tmp->myNominalCoordSys = nominalSystem;

            FeatureWrapper *fw = new FeatureWrapper();
            fw->setCircle(tmp);

            if(actualNominal){ //if nominal feature
                for(int i=0; i< this->coordSys.size();i++){
                    if(this->coordSys.at(i) == nominalSystem){
                        this->coordSys.at(i)->nominals.append(fw->getGeometry());
                        this->features.append(fw);
                        break;
                    }
                }
            }else{ //if no nominal feature
                this->features.append(fw);
            }

            break;
        }
        case Configuration::eConeFeature:{
            Cone *tmp = new Cone();

            tmp->mConfig = *this->lastmConfig;
            tmp->name = featureName;
            tmp->group = group;
            tmp->isNominal = actualNominal;
            tmp->isCommon = isCommonPoint;
            tmp->myNominalCoordSys = nominalSystem;

            FeatureWrapper *fw = new FeatureWrapper();
            fw->setCone(tmp);

            if(actualNominal){ //if nominal feature
                for(int i=0; i< this->coordSys.size();i++){
                    if(this->coordSys.at(i) == nominalSystem){
                        this->coordSys.at(i)->nominals.append(fw->getGeometry());
                        this->features.append(fw);
                        break;
                    }
                }
            }else{ //if no nominal feature
                this->features.append(fw);
            }

            break;
        }
        case Configuration::eCylinderFeature:{
            Cylinder *tmp = new Cylinder();

            tmp->mConfig = *this->lastmConfig;
            tmp->name = featureName;
            tmp->group = group;
            tmp->isNominal = actualNominal;
            tmp->isCommon = isCommonPoint;
            tmp->myNominalCoordSys = nominalSystem;

            FeatureWrapper *fw = new FeatureWrapper();
            fw->setCylinder(tmp);

            if(actualNominal){ //if nominal feature
                for(int i=0; i< this->coordSys.size();i++){
                    if(this->coordSys.at(i) == nominalSystem){
                        this->coordSys.at(i)->nominals.append(fw->getGeometry());
                        this->features.append(fw);
                        break;
                    }
                }
            }else{ //if no nominal feature
                this->features.append(fw);
            }

            break;
        }
        case Configuration::eEllipsoidFeature:{
            Ellipsoid *tmp = new Ellipsoid();

            tmp->mConfig = *this->lastmConfig;
            tmp->name = featureName;
            tmp->group = group;
            tmp->isNominal = actualNominal;
            tmp->isCommon = isCommonPoint;
            tmp->myNominalCoordSys = nominalSystem;

            FeatureWrapper *fw = new FeatureWrapper();
            fw->setEllipsoid(tmp);

            if(actualNominal){ //if nominal feature
                for(int i=0; i< this->coordSys.size();i++){
                    if(this->coordSys.at(i) == nominalSystem){
                        this->coordSys.at(i)->nominals.append(fw->getGeometry());
                        this->features.append(fw);
                        break;
                    }
                }
            }else{ //if no nominal feature
                this->features.append(fw);
            }

            break;
        }
        case Configuration::eHyperboloidFeature:{
            Hyperboloid *tmp = new Hyperboloid();

            tmp->mConfig = *this->lastmConfig;
            tmp->name = featureName;
            tmp->group = group;
            tmp->isNominal = actualNominal;
            tmp->isCommon = isCommonPoint;
            tmp->myNominalCoordSys = nominalSystem;

            FeatureWrapper *fw = new FeatureWrapper();
            fw->setHyperboloid(tmp);

            if(actualNominal){ //if nominal feature
                for(int i=0; i< this->coordSys.size();i++){
                    if(this->coordSys.at(i) == nominalSystem){
                        this->coordSys.at(i)->nominals.append(fw->getGeometry());
                        this->features.append(fw);
                        break;
                    }
                }
            }else{ //if no nominal feature
                this->features.append(fw);
            }

            break;
        }
        case Configuration::eParaboloidFeature:{
            Paraboloid *tmp = new Paraboloid();

            tmp->mConfig = *this->lastmConfig;
            tmp->name = featureName;
            tmp->group = group;
            tmp->isNominal = actualNominal;
            tmp->isCommon = isCommonPoint;
            tmp->myNominalCoordSys = nominalSystem;

            FeatureWrapper *fw = new FeatureWrapper();
            fw->setParaboloid(tmp);

            if(actualNominal){ //if nominal feature
                for(int i=0; i< this->coordSys.size();i++){
                    if(this->coordSys.at(i) == nominalSystem){
                        this->coordSys.at(i)->nominals.append(fw->getGeometry());
                        this->features.append(fw);
                        break;
                    }
                }
            }else{ //if no nominal feature
                this->features.append(fw);
            }

            break;
        }
        case Configuration::ePointCloudFeature:{
            PointCloud *tmp = new PointCloud();

            tmp->mConfig = *this->lastmConfig;
            tmp->name = featureName;
            tmp->group = group;
            tmp->isNominal = actualNominal;
            tmp->isCommon = isCommonPoint;
            tmp->myNominalCoordSys = nominalSystem;

            FeatureWrapper *fw = new FeatureWrapper();
            fw->setPointCloud(tmp);

            if(actualNominal){ //if nominal feature
                for(int i=0; i< this->coordSys.size();i++){
                    if(this->coordSys.at(i) == nominalSystem){
                        this->coordSys.at(i)->nominals.append(fw->getGeometry());
                        this->features.append(fw);
                        break;
                    }
                }
            }else{ //if no nominal feature
                this->features.append(fw);
            }

            break;
        }
        case Configuration::eNurbsFeature:{
            Nurbs *tmp = new Nurbs();

            tmp->mConfig = *this->lastmConfig;
            tmp->name = featureName;
            tmp->group = group;
            tmp->isNominal = actualNominal;
            tmp->isCommon = isCommonPoint;
            tmp->myNominalCoordSys = nominalSystem;

            FeatureWrapper *fw = new FeatureWrapper();
            fw->setNurbs(tmp);

            if(actualNominal){ //if nominal feature
                for(int i=0; i< this->coordSys.size();i++){
                    if(this->coordSys.at(i) == nominalSystem){
                        this->coordSys.at(i)->nominals.append(fw->getGeometry());
                        this->features.append(fw);
                        break;
                    }
                }
            }else{ //if no nominal feature
                this->features.append(fw);
            }

            break;
        }
        case Configuration::eStationFeature:{
            Station *tmp = new Station(featureName);

            tmp->group = group;

            tmp->position->mConfig = *this->lastmConfig;
            tmp->position->isNominal = actualNominal;
            tmp->position->isCommon = isCommonPoint;
            tmp->position->myNominalCoordSys = nominalSystem;

            if(actualNominal){
                tmp->position->isSolved = true;
            }

            FeatureWrapper *fw = new FeatureWrapper();
            fw->setStation(tmp);

            if(actualNominal){
                for(int i=0; i< this->coordSys.size();i++){
                    if(this->coordSys.at(i) == nominalSystem){
                        this->coordSys.at(i)->nominals.append(fw->getGeometry());
                        this->features.append(fw);
                        this->stations.append(fw->getStation());
                        break;
                    }
                }
            }else{
                this->features.append(fw);
                this->stations.append(fw->getStation());
            }

            emit CoordSystemAdded();

            break;
        }
        case Configuration::eCoordinateSystemFeature:{
            CoordinateSystem *tmp = new CoordinateSystem();

            tmp->name = featureName;
            tmp->group = group;

            FeatureWrapper *fw = new FeatureWrapper();
            fw->setCoordinateSystem(tmp);

            this->features.append(fw);
            this->coordSys.append(fw->getCoordinateSystem());

            emit CoordSystemAdded();

            break;
        }
        }

        //refresh feature tree view models
        this->featureTreeViewModel->refreshModel();



        //this->availableElementsModel->setSourceModel(this->featureTreeViewModel);
        //this->availableElementsModel->layoutAboutToBeChanged();
        //this->availableElementsModel->layoutChanged();

        emit refreshGUI(this->activeFeature, this->activeStation);
        emit featureAdded();
    }

    //update available group names
    int currentCount = 0;
    if(this->availableGroups.contains(group)){
        currentCount = this->availableGroups.find(group).value();
        currentCount += count;
    }else{
        currentCount += count;
    }
    this->availableGroups.insert(group, currentCount);
    emit this->availableGroupsChanged(this->availableGroups);

}

void Controller::addScalarEntity(int count, int featureType, QString name, QString group, bool actual, bool commonPoint, double value, CoordinateSystem *nominalSystem){
//TODO scalar entities in neue Liste und nicht in features Liste speichern
    for(int k=0;k<count;k++){
        QString featureName;
        QString letterName;
        int numberName;

        if(k == 0){
            featureName = name;
        }else{
            featureName = name + QString::number(k);
        }

        for (int i=0; i<name.length();i++){
            if(name.at(i).isDigit()){
                letterName = name.left(i);
                numberName = name.mid(i).toInt();
                numberName += k;
                if(numberName == 0){
                    featureName = name;
                }else {
                    featureName = letterName + QString::number(numberName);
                }

                break;
            }
        }

        for(int k=0; k<this->features.size();k++){
            if(this->features.at(k)->getFeature()->name == featureName){
                if(actual == true && this->features.at(k)->getFeature()->getDisplayIsNominal() == "true"){
                    if(this->features.at(k)->getGeometry() != NULL && this->features.at(k)->getGeometry()->myNominalCoordSys == nominalSystem){
                        Console::addLine("Feature name already exists");
                        return;
                    }
                }
                if(actual == false && this->features.at(k)->getFeature()->getDisplayIsNominal() == "false"){
                    Console::addLine("Feature name already exists");
                    return;
                }
            }
        }

        switch(featureType){
        case Configuration::eScalarentityAngleFeature:{
            ScalarEntityAngle *tmpSEAngle = new ScalarEntityAngle();

            tmpSEAngle->mConfig = *this->lastmConfig;
            tmpSEAngle->name = featureName;
            tmpSEAngle->group = group;
            tmpSEAngle->isNominal = actual;
            tmpSEAngle->isCommon = commonPoint;
            tmpSEAngle->setAngle(value);
            tmpSEAngle->myNominalCoordSys = nominalSystem;
            tmpSEAngle->isSolved = true;


            FeatureWrapper *fw = new FeatureWrapper();
            fw->setScalarEntityAngle(tmpSEAngle);

            this->features.append(fw);

            break;
        }
        case Configuration::eScalarEntityDistanceFeature:{
            ScalarEntityDistance *tmpSEDistance = new ScalarEntityDistance();

            tmpSEDistance->mConfig = *this->lastmConfig;
            tmpSEDistance->name = featureName;
            tmpSEDistance->group = group;
            tmpSEDistance->isNominal = actual;
            tmpSEDistance->isCommon = commonPoint;
            tmpSEDistance->setDistance(value);
            tmpSEDistance->myNominalCoordSys = nominalSystem;
            tmpSEDistance->isSolved = true;


            FeatureWrapper *fw = new FeatureWrapper();
            fw->setScalarEntityDistance(tmpSEDistance);

            this->features.append(fw);
            break;
        }
        case Configuration::eScalarEntityTemperatureFeature:{
            ScalarEntityTemperature *tmpSETemperature = new ScalarEntityTemperature();

            tmpSETemperature->mConfig = *this->lastmConfig;
            tmpSETemperature->name = featureName;
            tmpSETemperature->group = group;
            tmpSETemperature->isNominal = actual;
            tmpSETemperature->isCommon = commonPoint;
            tmpSETemperature->setTemperature(value);
            tmpSETemperature->myNominalCoordSys = nominalSystem;
            tmpSETemperature->isSolved = true;

            FeatureWrapper *fw = new FeatureWrapper();
            fw->setScalarEntityTemperature(tmpSETemperature);

            this->features.append(fw);

            break;
        }
        case Configuration::eScalarEntityMeasurementSeriesFeature:{
            ScalarEntityMeasurementSeries *tmpSEMSeries = new ScalarEntityMeasurementSeries();

            tmpSEMSeries->mConfig = *this->lastmConfig;
            tmpSEMSeries->name = featureName;
            tmpSEMSeries->group = group;
            tmpSEMSeries->isNominal = actual;
            tmpSEMSeries->isCommon = commonPoint;
            tmpSEMSeries->setSeriesValue(value);
            tmpSEMSeries->myNominalCoordSys = nominalSystem;
            tmpSEMSeries->isSolved = true;


            FeatureWrapper *fw = new FeatureWrapper();
            fw->setScalarEntityMeasurementSeries(tmpSEMSeries);

            this->features.append(fw);

            break;
        }
        default:
            break;
        }

        emit refreshGUI(this->activeFeature, this->activeStation);
        emit featureAdded();
    }

    //update available group names
    int currentCount = 0;
    if(this->availableGroups.contains(group)){
        currentCount = this->availableGroups.find(group).value();
        currentCount += count;
    }else{
        currentCount += count;
    }
    this->availableGroups.insert(group, currentCount);
    emit this->availableGroupsChanged(this->availableGroups);

    //refresh feature tree view models
    this->featureTreeViewModel->refreshModel();

}

void Controller::addTrafoParam(int count, int featureType, QString name,CoordinateSystem *startSystem, CoordinateSystem *destSystem){
    for(int k=0;k<count;k++){
        QString featureName;
        QString letterName;
        int numberName;

        if(k == 0){
            featureName = name;
        }else{
            featureName = name + QString::number(k);
        }

        for (int i=0; i<name.length();i++){
            if(name.at(i).isDigit()){
                letterName = name.left(i);
                numberName = name.mid(i).toInt();
                numberName += k;
                if(numberName == 0){
                    featureName = name;
                }else {
                    featureName = letterName + QString::number(numberName);
                }

                break;
            }
        }

        for(int k=0; k<this->features.size();k++){
            if(this->features.at(k)->getFeature()->name == featureName){
                Console::addLine("Feature name already exists");
                return;
            }
        }

        for(int l=0; l<this->features.size();l++){
            if(this->features.at(l)->getTrafoParam() != NULL){
                if(this->features.at(l)->getTrafoParam()->from == startSystem && this->features.at(l)->getTrafoParam()->to == destSystem){
                    Console::addLine("Transformation parameter for this configuration already exist.");
                    return;
                }
                if(this->features.at(l)->getTrafoParam()->from == destSystem && this->features.at(l)->getTrafoParam()->to == startSystem){
                    Console::addLine("Transformation parameter for this configuration already exist.");
                    return;
                }
            }
        }

        TrafoParam *trafopara = new TrafoParam();

        trafopara->name = featureName;
        trafopara->isSolved = false;
        trafopara->from = startSystem;
        trafopara->to = destSystem;

        startSystem->trafoParams.append(trafopara);
        destSystem->trafoParams.append(trafopara);

        FeatureWrapper *fw = new FeatureWrapper();
        fw->setTrafoParam(trafopara);

        this->features.append(fw);
        break;
    }

    //refresh feature tree view models
    this->featureTreeViewModel->refreshModel();

    emit refreshGUI(this->activeFeature, this->activeStation);

}
/*!
 * \brief Controller::startMeasurement
 * After checking some conditions, it calls the measure function of the active sensor.
 */
void Controller::startMeasurement(){

    bool checkActiveCoordSys = false;

    if (activeStation->coordSys == activeCoordinateSystem){
        checkActiveCoordSys = true;
    }

    if(checkSensorValid() && checkFeatureValid()){
        this->activeStation->emitStartMeasure(this->activeFeature->getGeometry(),checkActiveCoordSys);
        emit sensorWorks("measuring...");
    }
}

/*!
 * \brief Controller::startMove
 * After checking some conditions, it calls the move function of the active sensor.
 */
void Controller::startMove(Reading *parameter){

    //TODO check function
    if (parameter->typeofReading == Configuration::ePolar){
        activeStation->startMove(parameter->rPolar.azimuth,parameter->rPolar.zenith,parameter->rPolar.distance,false);
        emit sensorWorks("moving...");
    }else if (parameter->typeofReading == Configuration::eCartesian){
        activeStation->startMove(parameter->rCartesian.xyz.getAt(0),parameter->rCartesian.xyz.getAt(1),parameter->rCartesian.xyz.getAt(2));
        emit sensorWorks("moving...");
    }

}


void Controller::startAim(){

    if(checkFeatureValid() && checkSensorValid()){
        OiVec polarElements = Reading::toPolar(activeFeature->getGeometry()->getDisplayX().toDouble(),
                                                                 activeFeature->getGeometry()->getDisplayY().toDouble(),
                                                                 activeFeature->getGeometry()->getDisplayZ().toDouble());


        this->activeStation->emitStartMove(polarElements.getAt(0),polarElements.getAt(1),polarElements.getAt(2),false);
        emit sensorWorks("moving...");
    }

}

/*!
 * \brief Controller::startConnect
 * After checking some conditions, it calls the connect function of the active sensor.
 */
void Controller::startConnect(){

    if(this->activeStation != NULL){
        if(this->activeStation->instrument != NULL){
            this->activeStation->emitStartConnect(this->activeStation->getInstrumentConfig()->connConfig);
            emit sensorWorks("connecting...");
        }else{
            Console::addLine("sensor not connected");
        }
    }else{
        Console::addLine("no active station");
    }
}
/*!
 * \brief Controller::startDisconnect
 * After checking some conditions, it calls the disconnect function of the active sensor.
 */
void Controller::startDisconnect(){

    if(checkSensorValid()){
        this->activeStation->emitStartDisconnect();
        emit sensorWorks("disconnecting...");
    }
}

/*!
 * \brief Controller::startToggleSight
 * After checking some conditions, it calls the toggle sight function of the active sensor.
 */
void Controller::startToggleSight(){

    if(checkSensorValid()){
        this->activeStation->emitStartToggleSight();
        emit sensorWorks("toggle sight...");
    }
}

/*!
 * \brief Controller::sendCmdString
 * After checking some conditions, it calls the command function of the active sensor with the given parameter.
 * \param cmd
 */
void Controller::sendCmdString(QString cmd){

    if(checkSensorValid()){
        this->activeStation->emitStartCommand(cmd);
        emit sensorWorks(QString("sending command..."+ cmd));
    }
}

/*!
 * \brief Controller::startInitialize
 * After checking some conditions, it calls the initialize function of the active sensor.
 */
void Controller::startInitialize(){

    if(checkSensorValid()){
        this->activeStation->emitStartInitialize();
        emit sensorWorks("initialize...");
    }
}

/*!
 * \brief Controller::startHome
 * After checking some conditions, it calls the home function of the active sensor.
 */
void Controller::startHome(){

    if(checkSensorValid()){
        this->activeStation->emitStartHome();
        emit sensorWorks("home...");
    }
}

/*!
 * \brief Controller::startCompensation
 * After checking some conditions, it calls the compensation function of the active sensor.
 */
void Controller::startCompensation(){

    if(checkSensorValid()){
        this->activeStation->emitStartCompensation();
        emit sensorWorks("compensation...");
    }
}

/*!
 * \brief Controller::startChangeMotorState
 * After checking some conditions, it calls the change motor state function of the active sensor.
 */
void Controller::startChangeMotorState(){

    if(checkSensorValid()){
        //this->activeStation->emitStartMotorState();
        //emit sensorWorks("change motor state...");
    }
}

/*!
 * \brief Controller::recalcActiveFeature
 * Call recalcFeature function for active feature
 */
void Controller::recalcActiveFeature(){
    if(this->activeFeature != NULL && this->activeFeature->getTrafoParam() != NULL){
        this->recalcTrafoParam(this->activeFeature->getTrafoParam());
    }else if(this->activeFeature != NULL && this->activeFeature->getFeature() != NULL){
        this->recalcFeature(this->activeFeature->getFeature());
    }
}

/*!
 * \brief Controller::recalcFeature
 * Recalcs the given feature
 * \param f
 */
void Controller::recalcFeature(Feature *f){
    //start recalcing
    this->myFeatureUpdater.recalcFeature(f);
    //refresh feature tree view models
    this->featureTreeViewModel->refreshModel();
}

/*!
 * \brief Controller::recalcTrafoParam
 * Recalcs the given trafo param
 * \param tp
 */
void Controller::recalcTrafoParam(TrafoParam *tp){
    //start recalcing
    this->myFeatureUpdater.recalcTrafoParam(tp, this->features, this->coordSys, this->stations, this->activeCoordinateSystem);
    //refresh feature tree view models
    this->featureTreeViewModel->refreshModel();
}

/*!
 * \brief Controller::changeActiveStation
 * this function changes the active station to the given one.
 * \param s
 */
void Controller::changeActiveStation(){

    if(this->activeFeature->getStation() != NULL){

            if(this->activeStation->instrument != NULL && this->activeStation->instrument->isConnected){
                this->activeStation->startDisconnect();
            }
            //this->activeStation->isSolved = false;
            //TODO solved für station setzen !

            this->activeStation = NULL;
            this->activeStation = this->activeFeature->getStation();
            //this->activeStation->isSolved = true;

            connect(this->activeStation,SIGNAL(actionFinished(bool)),this,SLOT(showResults(bool)));
            emit changedStation();
            emit refreshGUI(this->activeFeature,this->activeStation);
    }
}

/*!
 * \brief Controller::showResults
 * After a sensor action finish, this funtion is called, to show the result in the console.
 * \param b
 */
void Controller::showResults(bool b){
    if(b){
        Console::addLine("sensor action completed");
        Console::addLine("count observation: ", activeStation->coordSys->observations.size());
    }
}

/*!
 * \brief Controller::defaultLastmConfig
 * Setting up a default constellation of the last measurement configuration.
 * It can be changed at runtime.
 */
void Controller::defaultLastmConfig(){
    lastmConfig->name = "default configuration";
    lastmConfig->count = 1;
    lastmConfig->measureTwoSides = false;
    if(this->activeStation != NULL && this->activeStation->instrument != NULL){
        lastmConfig->typeOfReading = this->activeStation->instrument->getSupportedReadingTypes()->at(0);
    }
    lastmConfig->timeDependent = false;
    lastmConfig->distanceDependent = false;
    lastmConfig->timeInterval = 0.0;
    lastmConfig->distanceInterval = 0.0;
}

/*!
 * \brief Controller::savePluginData
 * This function saves the plugin data and meta data in the database of openIndy.
 * So the plugin can be chosen in runtime out of all available plugins in the database.
 * \param metaInfo
 */
void Controller::savePluginData(PluginMetaData* metaInfo){

if (!metaInfo->alreadyExists){


    QList<Sensor*> sensorList = PluginLoader::loadSensorPlugins(metaInfo->path);
    QList<Function*> functionList = PluginLoader::loadFunctionPlugins(metaInfo->path);
    QList<NetworkAdjustment*> networkAdjustmentList = PluginLoader::loadNetworkAdjustmentPlugins(metaInfo->path);

    SystemDbManager::savePlugin(metaInfo,functionList,sensorList,networkAdjustmentList);

    /*for (int i = 0;i<sensorList.size();i++){
        SystemDbManager::savePlugin(sensorList.at(i)->getMetaData(),functionList,sensorList,networkAdjustmentList);
    }

    for (int i = 0;i<functionList.size();i++){
        SystemDbManager::savePlugin(functionList.at(i)->getMetaData());
    }

    for (int i = 0;i<networkAdjustmentList.size();i++){
        SystemDbManager::savePlugin(networkAdjustmentList.at(i)->getMetaData());
    }*/

   /* if(metaInfo->iid==OiMetaData::iid_LaserTracker){
            metaInfo->pluginType = OiMetaData::sensorPlugin;
            SystemDbManager::savePlugin(metaInfo);
            Console::addLine(QString("laser tracker plugin added: "+metaInfo->name +"-" +metaInfo->path));
    }else if(metaInfo->iid==OiMetaData::iid_ConstructFunction){
        metaInfo->pluginType = OiMetaData::functionPlugin;
        SystemDbManager::savePlugin(metaInfo);
        Console::addLine("construct plugin added");
    }else if(metaInfo->iid==OiMetaData::iid_FitFunction){
        metaInfo->pluginType = OiMetaData::functionPlugin;
        Function *tmpFunction = PluginLoader::loadFunctionPlugin(metaInfo->path);
        SystemDbManager::savePlugin(metaInfo,tmpFunction);
        Console::addLine("fit plugin added");
    }else if(metaInfo->iid==OiMetaData::iid_GeodeticFunction){
        metaInfo->pluginType = OiMetaData::functionPlugin;
        SystemDbManager::savePlugin(metaInfo);
        Console::addLine("geodetic plugin added");
    }else if(metaInfo->iid==OiMetaData::iid_NetworkAdjustment){
        metaInfo->pluginType = OiMetaData::functionPlugin;
        SystemDbManager::savePlugin(metaInfo);
        Console::addLine("network adjustment plugin added");
    }else if(metaInfo->iid==OiMetaData::iid_ObjectTransformation){
        metaInfo->pluginType = OiMetaData::functionPlugin;
        SystemDbManager::savePlugin(metaInfo);
        Console::addLine("object transformation plugin added");
    }else if(metaInfo->iid==OiMetaData::iid_SystemTransformation){
        metaInfo->pluginType = OiMetaData::functionPlugin;
        SystemDbManager::savePlugin(metaInfo);
        Console::addLine("system transformation plugin added");
    }else if(metaInfo->iid==OiMetaData::iid_TotalStation){
        metaInfo->pluginType = OiMetaData::sensorPlugin;
        SystemDbManager::savePlugin(metaInfo);
        Console::addLine("total station plugin added");
    }*/
  }
}

/*!
 * \brief Controller::setSensorModel
 * Loads all available plugins for the chosen sensor. You can chose the sensor at runtime in the set instrument menu.
 * \param sT
 */
void Controller::setSensorModel(Configuration::SensorTypes sT){

    switch(sT){
    case Configuration::eLaserTracker:
        SystemDbManager::getLaserTrackerModel(pluginsModel);
        break;
    case Configuration::eTotalStation:
        SystemDbManager::getTotalStationModel(pluginsModel);
        break;
    case Configuration::eUndefinedSensor:
        SystemDbManager::getUndefinedSensorModel(pluginsModel);
        break;
    }
    emit sendSQLModel(pluginsModel);
}

/*!
 * \brief Controller::getSelectedPlugin
 * Loads the selected plugin and sets it up as active station, after an instance was build.
 * \param index
 * \param sT
 */
void Controller::getSelectedPlugin(int index){
    Console::addLine("index: ", index);

    QString path = pluginsModel->record(index).value("file_path").toString();
    QString name = pluginsModel->record(index).value("name").toString();
    Console::addLine(path);

    if(path != NULL){

        //this->activeStation->InstrumentConfig = new SensorConfiguration();
        this->activeStation->instrument = PluginLoader::loadSensorPlugin(path, name);
        defaultLastmConfig();
    }
}

void Controller::getTempSensor(int index)
{
    QString path = pluginsModel->record(index).value("file_path").toString();
    QString name = pluginsModel->record(index).value("name").toString();

    if(path != NULL){
        Sensor *s = PluginLoader::loadSensorPlugin(path, name);
        emit sendTempSensor(s);
    }

}

/*!
 * \brief Controller::getSelectedFeature
 * Sets up the selected feature from the tableview as active feature.
 * So it can be used for measuring and other actions.
 * \param index
 */
void Controller::getSelectedFeature(int index){
    //if a new feature was selected
    if(this->activeFeature != this->features.at(getActiveFeatureIndex(index))){
        this->changeUsedElementsModel(-1, -1);
    }
    this->activeFeature = this->features.at(getActiveFeatureIndex(index));
    if(this->activeFeature->getGeometry()!= NULL){
        double x  = this->activeFeature->getGeometry()->getDisplayX().toDouble();
        double y = this->activeFeature->getGeometry()->getDisplayY().toDouble();
        double z = this->activeFeature->getGeometry()->getDisplayZ().toDouble();

        emit sendPositionOfActiveFeature(x,  y,  z);
    }
    emit refreshGUI(this->activeFeature, this->activeStation);
    //set up tree view model with all functions of selected feature
    this->changeFunctionTreeViewModel();
    //give activeFeature to graphicsmodel
    this->featureGraphicsModel->setActiveFeature(this->activeFeature);
}

/*!
 * \brief Controller::receiveSensorConfiguration
 * \param sc
 */
void Controller::receiveSensorConfiguration(SensorConfiguration *sc, bool connect){

    if(connect){
        this->activeStation->setInstrumentConfig(sc);
        this->activeStation->emitStartConnect(sc->connConfig);
    }else{
        this->activeStation->setInstrumentConfig(sc);
    }

}

/*!
 * \brief Controller::setFunction
 * Opens the menu to set a function for the active feature. The dialog shows all available functions for the selected feature.
 */
void Controller::setFunction(){
    if(this->activeFeature != NULL && this->activeFeature->getFeature() != NULL){
        if(this->activeFeature->getFeature()->functionList.size() == 0
                && (this->activeFeature->getGeometry() == NULL || !this->activeFeature->getGeometry()->isNominal)){
            SystemDbManager::getCreateFunctionModel(this->pluginsModel,this->activeFeature->getTypeOfFeature());
        }else{
            SystemDbManager::getChangeFunctionModel(this->pluginsModel,this->activeFeature->getTypeOfFeature());
        }
    }else{
        Console::addLine("no feature selected");
    }
}

/*!
 * \brief Controller::receiveFunctionId
 * Receives the id of the selected function for the selected feature and querys the needed elements from database for that function.
 * \param id
 */
void Controller::receiveFunctionId(int id){
    SystemDbManager::getNeededElements(neededElementsModel, id);
}

/*!
 * \brief Controller::createFunction
 * Creates an instance of the selected function and adds it to the function list of the active and selected feature.
 * \param index
 */
void Controller::createFunction(int index){
    qDebug() << "creat func";
    qDebug() << index;
    qDebug() << this->pluginsModel->record(index).value("file_path").toString();
    QString path = pluginsModel->record(index).value("file_path").toString();
    QString name = pluginsModel->record(index).value("name").toString();
    if(this->activeFeature != NULL && this->activeFeature->getFeature() != NULL){
        //if the active feature is not a nominal geometry
        if(this->activeFeature->getGeometry() == NULL
                || !this->activeFeature->getGeometry()->isNominal){
            //create function and connect oiemitter to console
            Function *newFunction = PluginLoader::loadFunctionPlugin(path,name);
            if(newFunction != NULL){
                connect(&newFunction->getOiEmitter(), SIGNAL(sendString(QString)), this, SLOT(printToConsole(QString)));
                this->activeFeature->getFeature()->addFunction(newFunction);
            }
            //set up tree view model with all functions of selected feature
            this->changeFunctionTreeViewModel();
            //set up available functions for the active feature
            this->setFunction();
        }else{
            Console::addLine("A nominal geometry cannot have a function!");
        }
    }
}

/*!
 * \brief Controller::deleteFunctionFromFeature
 * Delete the function at the specified index from the active feature
 * \param index
 */
void Controller::deleteFunctionFromFeature(int index){
    if(this->activeFeature != NULL && this->activeFeature->getFeature() != NULL){
        Feature *activeFeature = this->activeFeature->getFeature();
        if(activeFeature->functionList.size() > index){
            //the first function of a feature can only be deleted if it is the only function
            if(activeFeature->functionList.size() > 1 && index == 0){
                Console::addLine("Cannot delete the function of a feature that defines it!");
            }else{
                delete activeFeature->functionList.at(index);
                activeFeature->functionList.removeAt(index);
                //set up tree view model with all functions of selected feature
                this->changeFunctionTreeViewModel();
                //set up available functions for the active feature
                this->setFunction();
            }
        }
    }
}

bool Controller::checkFeatureValid(){
    if(this->activeFeature != NULL && this->activeFeature->getFeature() != NULL){
        if(this->activeFeature->getGeometry() != NULL && this->activeFeature->getGeometry() != this->activeStation->position &&
                this->activeFeature->getGeometry()->isNominal == false){
            return true;
        }else{
            Console::addLine("you cannot measure this feature");
            return false;
        }
    }else{
        Console::addLine("no feature selected");
        return false;
    }
}

bool Controller::checkSensorValid(){

    if(this->activeStation != NULL){
        if(this->activeStation->instrument != NULL && this->activeStation->instrument->isConnected){
            return true;
        }else{
            Console::addLine("sensor not connected");
            return false;
        }
    }else{
        Console::addLine("no active station");
        return false;
    }
}

void Controller::importFeatures(QList<FeatureWrapper*> f){
    for(int i = 0;i<f.size();i++){

        this->features.append(f.at(i));

    }

    //refresh feature tree view models
    this->featureTreeViewModel->refreshModel();

    refreshGUI(this->activeFeature, this->activeStation);
}

void Controller::setActiveCoordSystem(QString CoordSysName){

    qDebug() << CoordSysName;
    for(int i=0; i<this->features.size();i++){
        if(this->features.at(i)->getCoordinateSystem() != NULL && this->features.at(i)->getCoordinateSystem()->name == CoordSysName){
            this->activeCoordinateSystem = this->features.at(i)->getCoordinateSystem();
        }
        if(this->features.at(i)->getStation() != NULL &&
                this->features.at(i)->getStation()->coordSys != NULL &&
                this->features.at(i)->getStation()->name == CoordSysName){
            this->activeCoordinateSystem = this->features.at(i)->getStation()->coordSys;

        }
    }

    //transform observations to current system and recalc all features
    this->myFeatureUpdater.switchCoordinateSystem(this->coordSys, this->stations, this->features, this->activeCoordinateSystem);

    //update table view for all features
    emit this->refreshGUI(this->activeFeature, this->activeStation);
}

/*!
 * \brief Controller::changeFunctionTreeViewModel
 * Update model for tree view with all functions of selected feature
 */
void Controller::changeFunctionTreeViewModel(){
    this->functionTreeViewModel->clear();
    this->functionTreeViewModel->setHorizontalHeaderItem(0, new QStandardItem("functions"));
    QStandardItem *rootItem = this->functionTreeViewModel->invisibleRootItem();
    if(this->activeFeature != NULL && this->activeFeature->getFeature() != NULL){
        foreach(Function *f, this->activeFeature->getFeature()->functionList){
            if(f != NULL){
                QStandardItem *function = new QStandardItem(f->getMetaData()->name);
                foreach(InputParams param, f->getNeededElements()){
                    if(param.infinite){
                        QStandardItem *element = new QStandardItem(QString("n %1s").arg(OiMetaData::findElement(param.typeOfElement)));
                        function->appendRow(element);
                    }else{
                        QStandardItem *element = new QStandardItem(QString("1 %1").arg(OiMetaData::findElement(param.typeOfElement)));
                        function->appendRow(element);
                    }
                }
                rootItem->appendRow(function);
            }
        }
    }
}

/*!
 * \brief Controller::changeUsedElementsModel
 * Update model for listview with used elements
 * \param functionIndex
 * \param elementIndex
 */
void Controller::changeUsedElementsModel(int functionIndex, int elementIndex){
    this->usedElementsModel->removeAllElements();

    if(functionIndex >= 0 && elementIndex >= 0 && this->activeFeature != NULL && this->activeFeature->getFeature() != NULL
            && this->activeFeature->getFeature()->functionList.size() > functionIndex
            && this->activeFeature->getFeature()->functionList.at(functionIndex)->getNeededElements().size() > elementIndex){
        //get function at functionIndex of active feature
        Function *func = this->activeFeature->getFeature()->functionList.at(functionIndex);
        QMap<int, QList<InputFeature> > featureOrder = func->getFeatureOrder();
        QMap<int, QList<InputFeature> >::iterator idx = featureOrder.find(elementIndex);
        if(idx != featureOrder.end() && idx.value().size() > 0){ //if the list with elements is not empty
            QList<InputFeature> featurePosition = idx.value(); //elements of function at elementIndex
            switch(this->activeFeature->getFeature()->functionList.at(functionIndex)->getNeededElements().at(elementIndex).typeOfElement){
                case Configuration::ePointElement:
                    for(int i = 0; i < featurePosition.size(); i++){
                        Point *p = func->getPoint(featurePosition.at(i).id);
                        if(p != NULL){
                            FeatureWrapper *pointWrapper = new FeatureWrapper();
                            pointWrapper->setPoint(p);
                            FeatureTreeItem *point = new FeatureTreeItem(p->name);
                            point->setFeature(pointWrapper);
                            this->usedElementsModel->addElement(point);
                        }
                    }
                    break;
                case Configuration::eLineElement:
                    for(int i = 0; i < featurePosition.size(); i++){
                        Line *l = func->getLine(featurePosition.at(i).id);
                        if(l != NULL){
                            FeatureWrapper *lineWrapper = new FeatureWrapper();
                            lineWrapper->setLine(l);
                            FeatureTreeItem *line = new FeatureTreeItem(l->name);
                            line->setFeature(lineWrapper);
                            this->usedElementsModel->addElement(line);
                        }
                    }
                    break;
                case Configuration::ePlaneElement:
                    for(int i = 0; i < featurePosition.size(); i++){
                        Plane *p = func->getPlane(featurePosition.at(i).id);
                        if(p != NULL){
                            FeatureWrapper *planeWrapper = new FeatureWrapper();
                            planeWrapper->setPlane(p);
                            FeatureTreeItem *plane = new FeatureTreeItem(p->name);
                            plane->setFeature(planeWrapper);
                            this->usedElementsModel->addElement(plane);
                        }
                    }
                    break;
                case Configuration::eSphereElement:
                    for(int i = 0; i < featurePosition.size(); i++){
                        Sphere *s = func->getSphere(featurePosition.at(i).id);
                        if(s != NULL){
                            FeatureWrapper *sphereWrapper = new FeatureWrapper();
                            sphereWrapper->setSphere(s);
                            FeatureTreeItem *sphere = new FeatureTreeItem(s->name);
                            sphere->setFeature(sphereWrapper);
                            this->usedElementsModel->addElement(sphere);
                        }
                    }
                    break;
                case Configuration::eScalarEntityAngleElement:
                    for(int i = 0; i < featurePosition.size(); i++){
                        ScalarEntityAngle *s = func->getScalarEntityAngle(featurePosition.at(i).id);
                        if(s != NULL){
                            FeatureWrapper *angleWrapper = new FeatureWrapper();
                            angleWrapper->setScalarEntityAngle(s);
                            FeatureTreeItem *angle = new FeatureTreeItem(s->name);
                            angle->setFeature(angleWrapper);
                            this->usedElementsModel->addElement(angle);
                        }
                    }
                    break;
                case Configuration::eScalarEntityDistanceElement:
                    for(int i = 0; i < featurePosition.size(); i++){
                        ScalarEntityDistance *s = func->getScalarEntityDistance(featurePosition.at(i).id);
                        if(s != NULL){
                            FeatureWrapper *distanceWrapper = new FeatureWrapper();
                            distanceWrapper->setScalarEntityDistance(s);
                            FeatureTreeItem *distance = new FeatureTreeItem(s->name);
                            distance->setFeature(distanceWrapper);
                            this->usedElementsModel->addElement(distance);
                        }
                    }
                    break;
                case Configuration::eTrafoParamElement:
                    for(int i = 0; i < featurePosition.size(); i++){
                        TrafoParam *tp = func->getTrafoParam(featurePosition.at(i).id);
                        if(tp != NULL){
                            FeatureWrapper *trafoParamWrapper = new FeatureWrapper();
                            trafoParamWrapper->setTrafoParam(tp);
                            FeatureTreeItem *trafoParam = new FeatureTreeItem(tp->name);
                            trafoParam->setFeature(trafoParamWrapper);
                            this->usedElementsModel->addElement(trafoParam);
                        }
                    }
                    break;
                case Configuration::eCoordinateSystemElement:
                    for(int i = 0; i < featurePosition.size(); i++){
                        CoordinateSystem *cs = func->getCoordinateSystem(featurePosition.at(i).id);
                        if(cs != NULL){
                            FeatureWrapper *coordinateSystemWrapper = new FeatureWrapper();
                            coordinateSystemWrapper->setCoordinateSystem(cs);
                            FeatureTreeItem *coordinateSystem = new FeatureTreeItem(cs->name);
                            coordinateSystem->setFeature(coordinateSystemWrapper);
                            this->usedElementsModel->addElement(coordinateSystem);
                        }
                    }
                    break;
                case Configuration::eStationElement:
                    for(int i = 0; i < featurePosition.size(); i++){
                        Station *s = func->getStation(featurePosition.at(i).id);
                        if(s != NULL){
                            FeatureWrapper *stationWrapper = new FeatureWrapper();
                            stationWrapper->setStation(s);
                            FeatureTreeItem *station = new FeatureTreeItem(s->name);
                            station->setFeature(stationWrapper);
                            this->usedElementsModel->addElement(station);
                        }
                    }
                    break;
                case Configuration::eObservationElement:
                    for(int i = 0; i < featurePosition.size(); i++){
                        Observation *o = func->getObservation(featurePosition.at(i).id);
                        if(o != NULL && o->myReading != NULL){
                            FeatureTreeItem *observation = new FeatureTreeItem(o->myReading->measuredAt.toString());
                            observation->setObservation(o);
                            this->usedElementsModel->addElement(observation);
                        }
                    }
                    break;
                case Configuration::eReadingCartesianElement:
                    for(int i = 0; i < featurePosition.size(); i++){
                        Reading *r = func->getReadingCartesian(featurePosition.at(i).id);
                        if(r != NULL){
                            FeatureTreeItem *reading = new FeatureTreeItem(r->measuredAt.toString());
                            reading->setReading(r);
                            this->usedElementsModel->addElement(reading);
                        }
                    }
                    break;
                case Configuration::eReadingDirectionElement:
                    for(int i = 0; i < featurePosition.size(); i++){
                        Reading *r = func->getReadingDirection(featurePosition.at(i).id);
                        if(r != NULL){
                            FeatureTreeItem *reading = new FeatureTreeItem(r->measuredAt.toString());
                            reading->setReading(r);
                            this->usedElementsModel->addElement(reading);
                        }
                    }
                    break;
                case Configuration::eReadingDistanceElement:
                    for(int i = 0; i < featurePosition.size(); i++){
                        Reading *r = func->getReadingDistance(featurePosition.at(i).id);
                        if(r != NULL){
                            FeatureTreeItem *reading = new FeatureTreeItem(r->measuredAt.toString());
                            reading->setReading(r);
                            this->usedElementsModel->addElement(reading);
                        }
                    }
                    break;
                case Configuration::eReadingPolarElement:
                    for(int i = 0; i < featurePosition.size(); i++){
                        Reading *r = func->getReadingPolar(featurePosition.at(i).id);
                        if(r != NULL){
                            FeatureTreeItem *reading = new FeatureTreeItem(r->measuredAt.toString());
                            reading->setReading(r);
                            this->usedElementsModel->addElement(reading);
                        }
                    }
                    break;
            }
        }
    }

    this->usedElementsModel->refreshModel();
}

/*!
 * \brief Controller::setSelectedFunction
 * Receive selected function from function plugin loader dialog
 * \param index
 * \param neededElement
 */
void Controller::setSelectedFunction(int functionIndex, int neededElementIndex){
    if(this->activeFeature->getFeature()->functionList.size() > functionIndex && functionIndex >= 0){ //if function index is valid
        if(this->activeFeature->getFeature()->functionList.at(functionIndex)->getNeededElements().size() > neededElementIndex
                && neededElementIndex >= 0){ //if needed element index is valid
            emit this->sendFunctionDescription(this->activeFeature->getFeature()->functionList.at(functionIndex)->getNeededElements()
                                               .at(neededElementIndex).description);
            emit this->sendAvailableElementsFilter(this->activeFeature->getFeature()->functionList.at(functionIndex)->getNeededElements()
                                                   .at(neededElementIndex).typeOfElement, false);
        }else{ //if function itself was clicked
            Function *func = this->activeFeature->getFeature()->functionList.at(functionIndex);
            emit this->sendFunctionDescription(func->getMetaData()->description);
            emit this->sendAvailableElementsFilter(Configuration::eUndefinedElement, true);
            emit this->sendExtraParameterForFunction(func->getIntegerParameter(), func->getDoubleParameter(), func->getStringParameter(), func->getFunctionConfiguration());
        }
    }else{
        emit this->sendAvailableElementsFilter(Configuration::eUndefinedElement, true);
    }
    //re-build used elements model
    this->changeUsedElementsModel(functionIndex, neededElementIndex);
}

/*!
 * \brief Controller::addElement2Function
 * Add element to the active function at index
 * \param element
 * \param functionIndex
 * \param elementIndex
 */
void Controller::addElement2Function(FeatureTreeItem *element, int functionIndex, int elementIndex){
    if(element != NULL && functionIndex >= 0 && elementIndex >= 0 && this->activeFeature != NULL && this->activeFeature->getFeature() != NULL){
        //check circle warning recursively
        if(element->getIsFeature() && element->getFeature() != NULL && element->getFeature()->getFeature() != NULL
                && this->checkCircleWarning(this->activeFeature->getFeature(), element->getFeature()->getFeature())){
            Console::addLine(QString("%1 %2")
                             .arg("You cannot use a feature for the calculation, which itself is directly or indirectly")
                             .arg("dependent on the feature to be calculated."));
            emit this->showMessageBox("circle warning", QString("%1 %2")
                                      .arg("You cannot use a feature for the calculation, which itself is directly or indirectly")
                                      .arg("dependent on the feature to be calculated."));
            return;
        }
        //get pointer to active feature and add used element
        Feature *feature = this->activeFeature->getFeature();
        if(feature->functionList.size() > functionIndex && feature->functionList.at(functionIndex)->getNeededElements().size() > elementIndex){
            if(element->getIsFeature() && element->getFeature() != NULL && element->getFeature()->getFeature() != NULL){
                switch(element->getFeature()->getTypeOfFeature()){
                    case Configuration::ePointFeature:
                        if(element->getFeature()->getPoint() != NULL
                                && feature->functionList.at(functionIndex)->getNeededElements().at(elementIndex).typeOfElement == Configuration::ePointElement){
                            feature->functionList.at(functionIndex)->addPoint(element->getFeature()->getPoint(), elementIndex);
                        }
                        break;
                    case Configuration::eLineFeature:
                        if(element->getFeature()->getLine() != NULL
                                && feature->functionList.at(functionIndex)->getNeededElements().at(elementIndex).typeOfElement == Configuration::eLineElement){
                            feature->functionList.at(functionIndex)->addLine(element->getFeature()->getLine(), elementIndex);
                        }
                        break;
                    case Configuration::ePlaneFeature:
                        if(element->getFeature()->getPlane() != NULL
                                && feature->functionList.at(functionIndex)->getNeededElements().at(elementIndex).typeOfElement == Configuration::ePlaneElement){
                            feature->functionList.at(functionIndex)->addPlane(element->getFeature()->getPlane(), elementIndex);
                        }
                        break;
                    case Configuration::eSphereFeature:
                        if(element->getFeature()->getSphere() != NULL
                                && feature->functionList.at(functionIndex)->getNeededElements().at(elementIndex).typeOfElement == Configuration::eSphereElement){
                            feature->functionList.at(functionIndex)->addSphere(element->getFeature()->getSphere(), elementIndex);
                        }
                        break;
                    case Configuration::eStationFeature:
                        if(element->getFeature()->getStation() != NULL
                                && feature->functionList.at(functionIndex)->getNeededElements().at(elementIndex).typeOfElement == Configuration::eStationElement){
                            feature->functionList.at(functionIndex)->addStation(element->getFeature()->getStation(), elementIndex);
                        }
                        break;
                    case Configuration::eCoordinateSystemFeature:
                        if(element->getFeature()->getCoordinateSystem() != NULL
                                && feature->functionList.at(functionIndex)->getNeededElements().at(elementIndex).typeOfElement == Configuration::eCoordinateSystemElement){
                            feature->functionList.at(functionIndex)->addCoordSystem(element->getFeature()->getCoordinateSystem(), elementIndex);
                        }
                        break;
                    case Configuration::eTrafoParamFeature:
                        if(element->getFeature()->getTrafoParam() != NULL
                                && feature->functionList.at(functionIndex)->getNeededElements().at(elementIndex).typeOfElement == Configuration::eTrafoParamElement){
                            feature->functionList.at(functionIndex)->addTrafoParam(element->getFeature()->getTrafoParam(), elementIndex);
                        }
                        break;
                    case Configuration::eScalarentityAngleFeature:
                        if(element->getFeature()->getScalarEntityAngle() != NULL
                                && feature->functionList.at(functionIndex)->getNeededElements().at(elementIndex).typeOfElement == Configuration::eScalarEntityAngleElement){
                            feature->functionList.at(functionIndex)->addScalarEntityAngle(element->getFeature()->getScalarEntityAngle(), elementIndex);
                        }
                        break;
                    case Configuration::eScalarEntityDistanceFeature:
                        if(element->getFeature()->getScalarEntityDistance() != NULL
                                && feature->functionList.at(functionIndex)->getNeededElements().at(elementIndex).typeOfElement == Configuration::eScalarEntityDistanceElement){
                            feature->functionList.at(functionIndex)->addScalarEntityDistance(element->getFeature()->getScalarEntityDistance(), elementIndex);
                        }
                        break;
                }
                //set usedFor and previouslyNeeded for active feature and used element
                feature->previouslyNeeded.append(element->getFeature());
                element->getFeature()->getFeature()->usedFor.append(this->activeFeature);
            }else if(element->getIsReading() && element->getReading() != NULL){
                switch(element->getReading()->typeofReading){
                    case Configuration::eCartesian:
                        if(feature->functionList.at(functionIndex)->getNeededElements().at(elementIndex).typeOfElement == Configuration::eReadingCartesianElement){
                            feature->functionList.at(functionIndex)->addReadingCartesian(element->getReading(), elementIndex);
                        }
                        break;
                    case Configuration::ePolar:
                        if(feature->functionList.at(functionIndex)->getNeededElements().at(elementIndex).typeOfElement == Configuration::eReadingPolarElement){
                            feature->functionList.at(functionIndex)->addReadingPolar(element->getReading(), elementIndex);
                        }
                        break;
                    case Configuration::eDistance:
                        if(feature->functionList.at(functionIndex)->getNeededElements().at(elementIndex).typeOfElement == Configuration::eReadingDistanceElement){
                            feature->functionList.at(functionIndex)->addReadingDistance(element->getReading(), elementIndex);
                        }
                        break;
                    case Configuration::eDirection:
                        if(feature->functionList.at(functionIndex)->getNeededElements().at(elementIndex).typeOfElement == Configuration::eReadingDirectionElement){
                            feature->functionList.at(functionIndex)->addReadingDirection(element->getReading(), elementIndex);
                        }
                        break;
                }
            }else if(element->getIsObservation() && element->getObservation() != NULL
                     && feature->functionList.at(functionIndex)->getNeededElements().at(elementIndex).typeOfElement == Configuration::eObservationElement){
                feature->functionList.at(functionIndex)->addObservation(element->getObservation(), elementIndex);
            }
        }
        this->changeUsedElementsModel(functionIndex, elementIndex);
    }
}

/*!
 * \brief Controller::removeElementFromFunction
 * Remove element from the active function at elementIndex
 * \param element
 * \param functionIndex
 * \param elementIndex
 */
void Controller::removeElementFromFunction(FeatureTreeItem *element, int functionIndex, int elementIndex){
    if(element != NULL && functionIndex >= 0 && this->activeFeature != NULL && this->activeFeature && this->activeFeature->getFeature() != NULL){
        Feature *feature = this->activeFeature->getFeature();
        if(feature->functionList.size() > functionIndex && feature->functionList.at(functionIndex) != NULL){
            if(element->getIsFeature() && element->getFeature() != NULL && element->getFeature()->getFeature() != NULL){
                switch(element->getFeature()->getTypeOfFeature()){
                    case Configuration::ePointFeature:
                        if(element->getFeature()->getPoint() != NULL){
                            feature->functionList.at(functionIndex)->removePoint(element->getFeature()->getFeature()->id);
                        }
                        break;
                    case Configuration::eLineFeature:
                        if(element->getFeature()->getLine() != NULL){
                            feature->functionList.at(functionIndex)->removeLine(element->getFeature()->getFeature()->id);
                        }
                        break;
                    case Configuration::ePlaneFeature:
                        if(element->getFeature()->getPlane() != NULL){
                            feature->functionList.at(functionIndex)->removePlane(element->getFeature()->getFeature()->id);
                        }
                        break;
                    case Configuration::eSphereFeature:
                        if(element->getFeature()->getSphere() != NULL){
                            feature->functionList.at(functionIndex)->removeSphere(element->getFeature()->getFeature()->id);
                        }
                        break;
                    case Configuration::eStationFeature:
                        if(element->getFeature()->getStation() != NULL){
                            feature->functionList.at(functionIndex)->removeStation(element->getFeature()->getFeature()->id);
                        }
                        break;
                    case Configuration::eCoordinateSystemFeature:
                        if(element->getFeature()->getCoordinateSystem() != NULL){
                            feature->functionList.at(functionIndex)->removeCoordSystem(element->getFeature()->getFeature()->id);
                        }
                        break;
                    case Configuration::eTrafoParamFeature:
                        if(element->getFeature()->getTrafoParam() != NULL){
                            feature->functionList.at(functionIndex)->removeTrafoParam(element->getFeature()->getFeature()->id);
                        }
                        break;
                    case Configuration::eScalarentityAngleFeature:
                        if(element->getFeature()->getScalarEntityAngle() != NULL){
                            feature->functionList.at(functionIndex)->removeScalarEntityAngle(element->getFeature()->getFeature()->id);
                        }
                        break;
                    case Configuration::eScalarEntityDistanceFeature:
                    if(element->getFeature()->getScalarEntityDistance() != NULL){
                        feature->functionList.at(functionIndex)->removeScalarEntityDistance(element->getFeature()->getFeature()->id);
                    }
                    break;
                }
                //set usedFor and previouslyNeeded for active feature and used element
                feature->previouslyNeeded.removeOne(element->getFeature());
                element->getFeature()->getFeature()->usedFor.removeOne(this->activeFeature);
            }else if(element->getIsObservation() && element->getObservation() != NULL){
                feature->functionList.at(functionIndex)->removeObservation(element->getObservation()->id);
            }else if(element->getIsReading() && element->getReading() != NULL){
                switch(element->getReading()->typeofReading){
                    case Configuration::eCartesian:
                        feature->functionList.at(functionIndex)->removeReadingCartesian(element->getReading()->id);
                    case Configuration::eDirection:
                        feature->functionList.at(functionIndex)->removeReadingDirection(element->getReading()->id);
                    case Configuration::eDistance:
                        feature->functionList.at(functionIndex)->removeReadingDistance(element->getReading()->id);
                    case Configuration::ePolar:
                        feature->functionList.at(functionIndex)->removeReadingPolar(element->getReading()->id);
                }
            }
            this->changeUsedElementsModel(functionIndex, elementIndex);
        }
    }
}

/*!
 * \brief Controller::checkCircleWarning
 * Checks wether there is a circular reference. If so this function returns true, otherwise false.
 * \param activeFeature
 * \param usedForActiveFeature
 * \return
 */
bool Controller::checkCircleWarning(Feature *activeFeature, Feature *usedForActiveFeature){
    if(activeFeature != NULL && usedForActiveFeature != NULL){
        //if activeFeature and usedForActiveFeature are equal
        if(activeFeature->id == usedForActiveFeature->id){
            return true;
        }
        //if active feature is in list of previously needed features of usedForActiveFeature
        foreach(FeatureWrapper *f, usedForActiveFeature->previouslyNeeded){
            if(f != NULL && f->getFeature() != NULL){
                if(f->getFeature()->id == activeFeature->id){
                    return true;
                }else if(f->getFeature()->previouslyNeeded.size() > 0 && this->checkCircleWarning(activeFeature, f->getFeature())){
                    return true;
                }
            }
        }
        return false;
    }
    return true;
}

/*!
 * \brief Controller::getActiveFeatureIndex
 * searches the active feature in the features list. transformation parameters cannot be selected as active feature.
 * So they donot get marked in the tableview.
 * \param index
 * \return
 */
int Controller::getActiveFeatureIndex(int index){

    int tmpINdex = index;

    int result = checkActiveFeatureIndex(0, index);
    result += tmpINdex;

    return result;
}

int Controller::checkActiveFeatureIndex(int current, int index){

    //int tmpIndex = index;
    int trafoParamCount = 0;
    int featureIndex = 0;

    for (current; current <= index; current++){
        if(this->features.at(current)->getTrafoParam() != NULL){
            trafoParamCount += 1;
        }
    }
    featureIndex += trafoParamCount;
    if( trafoParamCount > 0){
        featureIndex += checkActiveFeatureIndex(index+1, index+trafoParamCount);
    }

    return featureIndex;
}

void Controller::handleTrafoParamClicked(const QModelIndex &idx){
    int index = idx.row();
    int tmpCount = 0;

    for(int i=0;i<this->features.size();i++){
        if(this->features.at(i)->getTrafoParam() != NULL){
            tmpCount += 1;

            if(tmpCount-1 == index){
                this->activeFeature = this->features.at(i);

                emit refreshGUI(this->activeFeature, this->activeStation);
                //set up tree view model with all functions of selected feature
                this->changeFunctionTreeViewModel();

                break;
            }
        }
    }
}

void Controller::getNominalValues(double nomX, double nomY, double nomZ, double nomI, double nomJ, double nomK, double nomR, double nomSDE, double nomSAE, double nomSTE, double nomSMSE){

    switch (this->activeFeature->getTypeOfFeature()) {
    case Configuration::ePointFeature:
        this->activeFeature->getPoint()->xyz.setAt(0,nomX);
        this->activeFeature->getPoint()->xyz.setAt(1,nomY);
        this->activeFeature->getPoint()->xyz.setAt(2,nomZ);
        break;
    case Configuration::ePlaneFeature:
        this->activeFeature->getPlane()->xyz.setAt(0,nomX);
        this->activeFeature->getPlane()->xyz.setAt(1,nomY);
        this->activeFeature->getPlane()->xyz.setAt(2,nomZ);
        this->activeFeature->getPlane()->ijk.setAt(0,nomI);
        this->activeFeature->getPlane()->ijk.setAt(1,nomJ);
        this->activeFeature->getPlane()->ijk.setAt(2,nomK);
        break;
    case Configuration::eLineFeature:
        this->activeFeature->getLine()->xyz.setAt(0,nomX);
        this->activeFeature->getLine()->xyz.setAt(1,nomY);
        this->activeFeature->getLine()->xyz.setAt(2,nomZ);
        this->activeFeature->getLine()->ijk.setAt(0,nomI);
        this->activeFeature->getLine()->ijk.setAt(1,nomJ);
        this->activeFeature->getLine()->ijk.setAt(2,nomK);
        break;
    case Configuration::eSphereFeature:
        this->activeFeature->getSphere()->xyz.setAt(0,nomX);
        this->activeFeature->getSphere()->xyz.setAt(1,nomY);
        this->activeFeature->getSphere()->xyz.setAt(2,nomZ);
        this->activeFeature->getSphere()->radius = nomR;
        break;
    case Configuration::eScalarentityAngleFeature:
        this->activeFeature->getScalarEntityAngle()->setAngle(nomSAE);
        break;
    case Configuration::eScalarEntityDistanceFeature:
        this->activeFeature->getScalarEntityDistance()->setDistance(nomSDE);
        break;
    case Configuration::eScalarEntityTemperatureFeature:
        this->activeFeature->getScalarEntityTemperature()->setTemperature(nomSTE);
        break;
    case Configuration::eScalarEntityMeasurementSeriesFeature:
        this->activeFeature->getScalarEntityMeasurementSeries()->setSeriesValue(nomSMSE);
        break;
    default:
        break;
    }
}

/*!
 * \brief Controller::setFunctionConfiguration
 * Set function configuration for the function at functionIndex of active feature
 * \param functionIndex
 * \param config
 */
void Controller::setFunctionConfiguration(int functionIndex, FunctionConfiguration config){
    if(this->activeFeature != NULL && this->activeFeature->getFeature() != NULL
            && functionIndex >= 0 && this->activeFeature->getFeature()->functionList.size() > functionIndex){
        this->activeFeature->getFeature()->functionList.at(functionIndex)->setFunctionConfiguration(config);
    }
}
void Controller::loadProjectData(oiProjectData &data){

    features.clear();
    stations.clear();
    coordSys.clear();

    features = data.features;
    stations = data.stations;
    coordSys = data.coordSystems;

    if(features.size() > 0 && stations.size() > 0){
        refreshGUI(features.at(0),stations.at(0));
    }

}

/*!
 * \brief Controller::printToConsole
 * Connected to a functions oiemitter a plugin can print a text to the console
 * \param message
 */
void Controller::printToConsole(QString message){
    Console::addLine(message);
}

/*!
 * \brief Controller::deleteFeature
 * Delete the specified feature if possible
 * \param myFeature
 */
void Controller::deleteFeatures(QList<FeatureWrapper*> myFeatures){
    this->featuresToDelete = myFeatures;

    //check if delete task is valid
    int countCoordSys = 0;
    bool countCheck = true; //at least one coordinate system has to exist
    bool activeCheck = true; //the active station cannot be deleted
    bool displayCheck = true; //the display coordinate system cannot be deleted
    foreach(FeatureWrapper *f, this->featuresToDelete){
        if(f->getStation() != NULL && f->getStation() == this->activeStation){ //do not delete active station
            activeCheck = false;
            break;
        }else if(f->getCoordinateSystem() != NULL && f->getCoordinateSystem() == this->activeCoordinateSystem){ //do not delete display coordinate system
            displayCheck = false;
            break;
        }else if(f->getStation() != NULL || f->getCoordinateSystem() != NULL){
            countCoordSys++;
        }
    }
    if(countCoordSys >= (this->stations.size() + this->coordSys.size())){
        countCheck = false;
    }

    if(activeCheck && countCheck){ //if delete task is valid

        if(myFeatures.size() == 1){
            emit this->showMessageBoxForDecision("Delete features", QString("%1 %2 %3").arg("You have selected")
                                                 .arg(myFeatures.size())
                                                 .arg("feature. Do you really want to delete it, including all dependencies?"), this->myDeleteFeaturesCallback);
        }else{
            emit this->showMessageBoxForDecision("Delete features", QString("%1 %2 %3").arg("You have selected")
                                                 .arg(myFeatures.size())
                                                 .arg("features. Do you really want to delete them, including all dependencies?"), this->myDeleteFeaturesCallback);
        }

    }else{ //delete task is not valid

        if(activeCheck == false){
            emit this->showMessageBox("Delete error", "You cannot delete the active station!");
        }else{
            emit this->showMessageBox("Delete error", "At least one coordinate system has to exist!");
        }

    }
}

/*!
 * \brief Controller::deleteFeatureCallback
 * Is called after the user has decided wether to delete a feature with dependencies or not
 * \param command
 */
void Controller::deleteFeaturesCallback(bool command){
    //TODO create a backup file before deleting features

    if(command){ //if user decided to delete the selected features

        foreach(FeatureWrapper *delFeature, this->featuresToDelete){
            if(delFeature != NULL){

                //clear active feature and set it to active station
                if(this->activeFeature == delFeature){
                    this->activeFeature = NULL;
                    foreach(FeatureWrapper *f, this->features){
                        if(f->getStation() != NULL && f->getStation() == this->activeStation){
                            this->activeFeature = f;
                            break;
                        }
                    }
                }

                //remove feature from lists
                this->features.removeOne(delFeature);
                if(delFeature->getStation() != NULL){
                    this->stations.removeOne(delFeature->getStation());
                    //remove corresponding trafo param set
                    if(delFeature->getStation()->coordSys != NULL){
                        foreach(TrafoParam *myTrafo, delFeature->getStation()->coordSys->trafoParams){
                            if(myTrafo != NULL){
                                int index = -1;
                                for(int i = 0; i < this->features.size(); i++){
                                    if(this->features.at(i) != NULL && this->features.at(i)->getTrafoParam() != NULL
                                            && this->features.at(i)->getTrafoParam()->id == myTrafo->id){
                                        index = i;
                                        break;
                                    }
                                }
                                if(index >= 0){
                                    this->features.removeAt(index);
                                }
                            }
                        }
                    }
                }else if(delFeature->getCoordinateSystem() != NULL){
                    this->coordSys.removeOne(delFeature->getCoordinateSystem());
                    //remove corresponding trafo param set
                    foreach(TrafoParam *myTrafo, delFeature->getCoordinateSystem()->trafoParams){
                        if(myTrafo != NULL){
                            int index = -1;
                            for(int i = 0; i < this->features.size(); i++){
                                if(this->features.at(i) != NULL && this->features.at(i)->getTrafoParam() != NULL
                                        && this->features.at(i)->getTrafoParam()->id == myTrafo->id){
                                    index = i;
                                    break;
                                }
                            }
                            if(index >= 0){
                                this->features.removeAt(index);
                            }
                        }
                    }
                }

                //update group name map for combo boxes
                QString group = delFeature->getFeature()->group;
                if(this->availableGroups.contains(group)){
                    int count = this->availableGroups.find(group).value();
                    if(count <= 1){
                        this->availableGroups.remove(group);
                    }else{
                        this->availableGroups.insert(group, count-1);
                    }
                }

                //delete feature
                this->myFeatureUpdater.deleteFeature(delFeature, this->features);

            }
        }

        this->myFeatureUpdater.recalcFeatureSet(this->features);

        //refresh feature tree view models
        this->featureTreeViewModel->refreshModel();

        emit this->availableGroupsChanged(this->availableGroups);
        emit this->resetFeatureSelection();
        emit this->refreshGUI(this->activeFeature, this->activeStation);

    }
}

/*!
 * \brief Controller::groupNameChanged
 * Group name of one feature was edited in table view
 * \param oldValue
 * \param newValue
 */
void Controller::groupNameChanged(QString oldValue, QString newValue){

    if(oldValue.compare(newValue) != 0){
        //count down by 1 the number of occurences of oldValue as group name
        if(oldValue.compare("") != 0 && this->availableGroups.contains(oldValue)){
            int count = this->availableGroups.find(oldValue).value();
            if(count <= 1){
                this->availableGroups.remove(oldValue);
            }else{
                this->availableGroups.insert(oldValue, count-1);
            }
        }

        //count up by 1 the number of occurences of newValue as group name
        if(newValue.compare("") != 0){
            int count = 0;
            if(this->availableGroups.contains(newValue)){
                count += this->availableGroups.find(newValue).value();
            }
            this->availableGroups.insert(newValue, count+1);
        }

        emit this->availableGroupsChanged(this->availableGroups);
    }

}
