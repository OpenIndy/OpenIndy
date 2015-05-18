#include "featuretablemodel.h"

/*!
 * \brief FeatureTableModel::FeatureTableModel
 * \param job
 * \param parent
 */
FeatureTableModel::FeatureTableModel(const QPointer<OiJob> &job, QObject *parent) : QAbstractTableModel(parent){
    this->setCurrentJob(job);
}

/*!
 * \brief FeatureTableModel::FeatureTableModel
 * \param parent
 */
FeatureTableModel::FeatureTableModel(QObject *parent) : QAbstractTableModel(parent){

}

/*!
 * \brief FeatureTableModel::rowCount
 * \param parent
 * \return
 */
int FeatureTableModel::rowCount(const QModelIndex &parent) const{
    if(this->currentJob.isNull()){
        return 0;
    }
    return this->currentJob->getFeatureCount();
}

/*!
 * \brief FeatureTableModel::columnCount
 * \param parent
 * \return
 */
int FeatureTableModel::columnCount(const QModelIndex &parent) const{
    return getFeatureDisplayAttributes().size();
}

/*!
 * \brief FeatureTableModel::data
 * \param index
 * \param role
 * \return
 */
QVariant FeatureTableModel::data(const QModelIndex &index, int role) const{

    //check current job and model index
    if(this->currentJob.isNull() || !index.isValid()){
        return QVariant();
    }

    //get row and column indices
    int rowIndex = index.row();
    int columnIndex = index.column();

    //get the feature to display at index.row()
    if(this->currentJob->getFeatureCount() <= rowIndex){
        return QVariant();
    }
    QPointer<FeatureWrapper> feature = this->currentJob->getFeaturesList().at(rowIndex);

    //check the feature
    if(feature.isNull() || feature->getFeature().isNull()){
        return QVariant();
    }

    if(role == Qt::DisplayRole){

        //return the string to display for the feature at index.column()
        return this->getDisplayValue(feature, columnIndex);

    }else if(role == Qt::BackgroundRole){

        //return the color in which to display the feature at index.column()
        return this->getBackgroundValue(feature, columnIndex);

    }

    return QVariant();

}

/*!
 * \brief FeatureTableModel::headerData
 * \param section
 * \param orientation
 * \param role
 * \return
 */
QVariant FeatureTableModel::headerData(int section, Qt::Orientation orientation, int role) const{

    if((Qt::DisplayRole == role) &&
            (Qt::Horizontal == orientation) &&
            (0 <= section) &&
            (section < columnCount())){

        return getFeatureDisplayAttributeName(getFeatureDisplayAttributes().at(section));

    }

    return QVariant();

}


/*!
 * \brief FeatureTableModel::flags
 * Add edit-functionality to some cells of tablemodel
 * \param index
 * \return
 */
Qt::ItemFlags FeatureTableModel::flags(const QModelIndex &index) const{
    Qt::ItemFlags myFlags = QAbstractTableModel::flags(index);
    return (myFlags | Qt::ItemIsEditable);
}

/*!
 * \brief FeatureTableModel::setData
 * Update the edited features
 * \param index
 * \param value
 * \param role
 * \return
 */
bool FeatureTableModel::setData(const QModelIndex & index, const QVariant & value, int role){

    //check current job and model index
    if(this->currentJob.isNull() || !index.isValid()){
        return false;
    }

    //get the feature to at index.row()
    if(this->currentJob->getFeatureCount() <= index.row()){
        return false;
    }
    QPointer<FeatureWrapper> feature = this->currentJob->getFeaturesList().at(index.row());

    //check the feature
    if(feature.isNull() || feature->getFeature().isNull()){
        return false;
    }

    //get and check column index
    int column = index.column();
    if(column < 0 || getFeatureDisplayAttributes().size() <= column){
        return false;
    }

    //get display attribute
    int attr = getFeatureDisplayAttributes().at(column);

    //if a non-trafo param feature has been edited
    if(getIsFeatureDisplayAttribute(attr)){

        switch((FeatureDisplayAttributes)attr){
        case eFeatureDisplayName:{

            //check if the feature is a nominal geometry
            bool isNominal = (!feature->getGeometry().isNull() && feature->getGeometry()->getIsNominal());
            QPointer<CoordinateSystem> nominalSystem(NULL);
            if(isNominal){
                nominalSystem = feature->getGeometry()->getNominalSystem();
            }

            //only commit the new feature name if it is valid
            if(!this->currentJob->validateFeatureName(value.toString(), feature->getFeatureTypeEnum(), isNominal, nominalSystem)){
                return false;
            }

            //commit the new feature name
            feature->getFeature()->setFeatureName(value.toString());

            return true;

        }case eFeatureDisplayComment:{
            feature->getFeature()->setComment(value.toString());
            return true;
        }case eFeatureDisplayGroup:{
            feature->getFeature()->setGroupName(value.toString());
            return true;
        }
        }

    }else if(getIsTrafoParamDisplayAttribute(attr)){

        switch((TrafoParamDisplayAttributes)attr){
        case eTrafoParamDisplayName:{

            //check if the feature is a nominal geometry
            bool isNominal = (!feature->getGeometry().isNull() && feature->getGeometry()->getIsNominal());
            QPointer<CoordinateSystem> nominalSystem(NULL);
            if(isNominal){
                nominalSystem = feature->getGeometry()->getNominalSystem();
            }

            //only commit the new feature name if it is valid
            if(!this->currentJob->validateFeatureName(value.toString(), feature->getFeatureTypeEnum(), isNominal, nominalSystem)){
                return false;
            }

            //commit the new feature name
            feature->getFeature()->setFeatureName(value.toString());

            return true;

        }case eTrafoParamDisplayComment:{
            feature->getFeature()->setComment(value.toString());
            return true;
        }case eTrafoParamDisplayGroup:{
            feature->getFeature()->setGroupName(value.toString());
            return true;
        }case eTrafoParamDisplayIsUsed:{
            feature->getTrafoParam()->setIsUsed(value.toBool());
            return true;
        }
        }

    }

    return false;



    /*
    //get the active feature
    FeatureWrapper *myFeature = OiJob::getActiveFeature();
    if(myFeature == NULL || myFeature->getFeature() == NULL){
        return false;
    }

    if(index.column() == 3){ //feature name

        //get attributes necessary for name validation
        bool isNominal = false;
        CoordinateSystem *nominalSystem = NULL;
        if(myFeature->getGeometry() != NULL){
            isNominal = myFeature->getGeometry()->getIsNominal();
            nominalSystem = myFeature->getGeometry()->getNominalSystem();
        }

        //check if the feature name is ok
        //if(!OiFeatureState::validateFeatureName(myFeature->getTypeOfFeature(), value.toString(),
          //                                     isNominal, nominalSystem)){
            //return false;
        //}

        //if active feature is a geometry then corresponding nominals have to be renamed, too
        if(myFeature->getGeometry() != NULL){
            if(myFeature->getGeometry()->getIsNominal() && myFeature->getGeometry()->getMyActual() != NULL){
                myFeature->getGeometry()->getMyActual()->setFeatureName(value.toString());
                foreach(Geometry *nomGeom, myFeature->getGeometry()->getMyActual()->getMyNominals()){
                    if(nomGeom != NULL){
                        nomGeom->setFeatureName(value.toString());
                    }
                }
            }else{
                myFeature->getGeometry()->setFeatureName(value.toString());
                foreach(Geometry *nomGeom, myFeature->getGeometry()->getMyNominals()){
                    if(nomGeom != NULL){
                        nomGeom->setFeatureName(value.toString());
                    }
                }
            }
        }else{
            myFeature->getGeometry()->setFeatureName(value.toString());
        }

    }else if(index.column() == 2){ //feature group

        QString oldValue = myFeature->getFeature()->getGroupName();
        myFeature->getFeature()->setGroupName(value.toString());
        emit this->groupNameChanged(oldValue, value.toString());

    }else if(index.column() == 12){ //feature comment

        myFeature->getFeature()->setComment(value.toString());

    }else if(index.column() == 25){ //trafo param use

        if(myFeature->getTrafoParam() == NULL){
            return false;
        }
        myFeature->getTrafoParam()->setIsUsed(value.toBool());

    }else if(index.column() == 38){ //trafo param time

        if(myFeature->getTrafoParam() == NULL){
            return false;
        }
        myFeature->getTrafoParam()->setValidTime(value.toDateTime());

    }else if(index.column() == 26){ //trafo param datum transformation

        if(myFeature->getTrafoParam() == NULL){
            return false;
        }
        myFeature->getTrafoParam()->setisDatumTrafo(value.toBool());

    }else if(index.column() == 22){ //expansion origin x

        if(myFeature->getCoordinateSystem() == NULL){
            return false;
        }
        //myFeature->getCoordinateSystem()->setExpansionOriginX(value.toDouble()/OiUnitConverter::getDistanceMultiplier());

    }else if(index.column() == 23){ //expansion origin y

        if(myFeature->getCoordinateSystem() == NULL){
            return false;
        }
        //myFeature->getCoordinateSystem()->setExpansionOriginY(value.toDouble()/OiUnitConverter::getDistanceMultiplier());

    }else if(index.column() == 24){ //expansion origin z

        if(myFeature->getCoordinateSystem() == NULL){
            return false;
        }
        //myFeature->getCoordinateSystem()->setExpansionOriginZ(value.toDouble()/OiUnitConverter::getDistanceMultiplier());

    }

    this->updateModel();
*/
    return true;

}

/*!
 * \brief FeatureTableModel::getFeatureIdAtIndex
 * Returns the id of the feature at index or -1
 * \param index
 * \return
 */
int FeatureTableModel::getFeatureIdAtIndex(const QModelIndex &index){

    //check index
    if(!index.isValid()){
        return -1;
    }

    //check current job
    if(this->currentJob.isNull()){
        return -1;
    }

    //check if row index is valid
    if(index.row() >= this->currentJob->getFeatureCount()){
        return -1;
    }

    //check feature at index
    if(this->currentJob->getFeaturesList().at(index.row()).isNull()
            || this->currentJob->getFeaturesList().at(index.row())->getFeature().isNull()){
        return -1;
    }

    return this->currentJob->getFeaturesList().at(index.row())->getFeature()->getId();

}

/*!
 * \brief FeatureTableModel::getActiveFeature
 * \return
 */
QPointer<FeatureWrapper> FeatureTableModel::getActiveFeature() const{

    //check current job
    if(this->currentJob.isNull()){
        return QPointer<FeatureWrapper>(NULL);
    }

    return this->currentJob->getActiveFeature();

}

/*!
 * \brief FeatureTableModel::setActiveFeature
 * \param index
 */
void FeatureTableModel::setActiveFeature(const QModelIndex &index){

    //check index
    if(!index.isValid()){
        return;
    }

    //check current job
    if(this->currentJob.isNull()){
        return;
    }

    //check if row index is valid
    if(index.row() >= this->currentJob->getFeatureCount()){
        return;
    }

    //check feature at index
    if(this->currentJob->getFeaturesList().at(index.row()).isNull()
            || this->currentJob->getFeaturesList().at(index.row())->getFeature().isNull()){
        return;
    }

    //set active feature
    this->currentJob->getFeaturesList().at(index.row())->getFeature()->setActiveFeatureState(true);

}

/*!
 * \brief FeatureTableModel::getActiveStation
 * \return
 */
QPointer<Station> FeatureTableModel::getActiveStation() const{

    //check current job
    if(this->currentJob.isNull()){
        return QPointer<Station>(NULL);
    }

    return this->currentJob->getActiveStation();

}

/*!
 * \brief FeatureTableModel::setActiveStation
 * \param index
 */
void FeatureTableModel::setActiveStation(const QModelIndex &index){

    //check index
    if(!index.isValid()){
        return;
    }

    //check current job
    if(this->currentJob.isNull()){
        return;
    }

    //check if row index is valid
    if(index.row() >= this->currentJob->getFeatureCount()){
        return;
    }

    //check feature at index
    QPointer<FeatureWrapper> feature = this->currentJob->getFeaturesList().at(index.row());
    if(feature.isNull() || feature->getStation().isNull()){
        Console::getInstance()->addLine("No station selected");
        return;
    }

    //set active station
    feature->getStation()->setActiveStationState(true);

}

/*!
 * \brief FeatureTableModel::getActiveCoordinateSystem
 * \return
 */
QPointer<CoordinateSystem> FeatureTableModel::getActiveCoordinateSystem() const{

    //check current job
    if(this->currentJob.isNull()){
        return QPointer<CoordinateSystem>(NULL);
    }

    return this->currentJob->getActiveCoordinateSystem();

}

/*!
 * \brief FeatureTableModel::setActiveCoordinateSystem
 * \param name
 */
void FeatureTableModel::setActiveCoordinateSystem(const QString &name){

    //check current job
    if(this->currentJob.isNull()){
        return;
    }

    //get and check feature with the given name
    QList<QPointer<FeatureWrapper> > features = this->currentJob->getFeaturesByName(name);
    if(features.size() != 1){
        return;
    }
    QPointer<FeatureWrapper> feature = features.at(0);

    //get and check coordinate system
    QPointer<CoordinateSystem> system(NULL);
    if(!feature->getStation().isNull() && !feature->getStation()->getCoordinateSystem().isNull()){ //station system
        system = feature->getStation()->getCoordinateSystem();
    }else if(!feature->getCoordinateSystem().isNull()){ //nominal system
        system = feature->getCoordinateSystem();
    }
    if(system.isNull()){
        return;
    }

    //set active system
    system->setActiveCoordinateSystemState(true);

}

/*!
 * \brief FeatureTableModel::getActiveGroupName
 * \return
 */
const QString &FeatureTableModel::getActiveGroupName() const{

    //check current job
    if(this->currentJob.isNull()){
        return "";
    }

    return this->currentJob->getActiveGroup();

}

/*!
 * \brief FeatureTableModel::setActiveGroupName
 * \param group
 */
void FeatureTableModel::setActiveGroupName(const QString &group){

    //check current job
    if(this->currentJob.isNull()){
        return;
    }

    this->currentJob->setActiveGroup(group);

}

/*!
 * \brief FeatureTableModel::getCurrentJob
 * \return
 */
const QPointer<OiJob> &FeatureTableModel::getCurrentJob() const{
    return this->currentJob;
}

/*!
 * \brief FeatureTableModel::setCurrentJob
 * \param job
 */
void FeatureTableModel::setCurrentJob(const QPointer<OiJob> &job){
    if(!job.isNull()){

        //disconnect current job
        if(!this->currentJob.isNull()){
            this->disconnectJob();
        }

        this->currentJob = job;
        this->connectJob();
        this->updateModel();

    }
}

/*!
 * \brief FeatureTableModel::getParameterDisplayConfig
 * \return
 */
const ParameterDisplayConfig &FeatureTableModel::getParameterDisplayConfig() const{
    return this->parameterDisplayConfig;
}

/*!
 * \brief FeatureTableModel::setParameterDisplayConfig
 * \param config
 */
void FeatureTableModel::setParameterDisplayConfig(const ParameterDisplayConfig &config){
    this->parameterDisplayConfig = config;
    this->updateModel();
}

/*!
 * \brief FeatureTableModel::updateModel
 */
void FeatureTableModel::updateModel(){
    emit this->layoutAboutToBeChanged();
    emit this->layoutChanged();
}

/*!
 * \brief FeatureTableModel::getDisplayValue
 * Returns the string that shall be displayed for the given feature at the specified column
 * \param feature
 * \param column
 * \return
 */
QVariant FeatureTableModel::getDisplayValue(const QPointer<FeatureWrapper> &feature, const int &column) const{

    //check if the column exists in available display attributes
    if(column < 0 || getFeatureDisplayAttributes().size() <= column){
        return QVariant();
    }

    //get the display attribute
    int attr = getFeatureDisplayAttributes().at(column);

    if(getIsFeatureDisplayAttribute(attr)){ //feature attributes

        switch((FeatureDisplayAttributes)attr){
        case eFeatureDisplayType:
            return feature->getFeature()->getDisplayType();
        case eFeatureDisplayName:
            return feature->getFeature()->getFeatureName();
        case eFeatureDisplayComment:
            return feature->getFeature()->getComment();
        case eFeatureDisplayGroup:
            return feature->getFeature()->getGroupName();
        case eFeatureDisplayIsSolved:
            return feature->getFeature()->getDisplayIsSolved();
        case eFeatureDisplayIsUpdated:
            return feature->getFeature()->getDisplayIsUpdated();
        case eFeatureDisplayFunctions:
            return feature->getFeature()->getDisplayFunctions();
        case eFeatureDisplayUsedFor:
            return feature->getFeature()->getDisplayUsedFor();
        case eFeatureDisplayPreviouslyNeeded:
            return feature->getFeature()->getDisplayPreviouslyNeeded();
        case eFeatureDisplayStDev:
            return feature->getFeature()->getDisplayStDev(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                          this->parameterDisplayConfig.getDisplayDigits(eMetric));
        case eFeatureDisplayMeasurementConfig:
            return feature->getFeature()->getDisplayMeasurementConfig();
        case eFeatureDisplayObservations:
            return feature->getFeature()->getDisplayObservations();
        case eFeatureDisplayX:
            return feature->getFeature()->getDisplayX(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                      this->parameterDisplayConfig.getDisplayDigits(eMetric));
        case eFeatureDisplayY:
            return feature->getFeature()->getDisplayY(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                      this->parameterDisplayConfig.getDisplayDigits(eMetric));
        case eFeatureDisplayZ:
            return feature->getFeature()->getDisplayZ(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                      this->parameterDisplayConfig.getDisplayDigits(eMetric));
        case eFeatureDisplayPrimaryI:
            return feature->getFeature()->getDisplayPrimaryI(this->parameterDisplayConfig.getDisplayDigits(eDimensionless));
        case eFeatureDisplayPrimaryJ:
            return feature->getFeature()->getDisplayPrimaryJ(this->parameterDisplayConfig.getDisplayDigits(eDimensionless));
        case eFeatureDisplayPrimaryK:
            return feature->getFeature()->getDisplayPrimaryK(this->parameterDisplayConfig.getDisplayDigits(eDimensionless));
        case eFeatureDisplayRadiusA:
            return feature->getFeature()->getDisplayRadiusA(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                            this->parameterDisplayConfig.getDisplayDigits(eMetric));
        case eFeatureDisplayRadiusB:
            return feature->getFeature()->getDisplayRadiusB(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                            this->parameterDisplayConfig.getDisplayDigits(eMetric));
        case eFeatureDisplaySecondaryI:
            return feature->getFeature()->getDisplaySecondaryI(this->parameterDisplayConfig.getDisplayDigits(eDimensionless));
        case eFeatureDisplaySecondaryJ:
            return feature->getFeature()->getDisplaySecondaryJ(this->parameterDisplayConfig.getDisplayDigits(eDimensionless));
        case eFeatureDisplaySecondaryK:
            return feature->getFeature()->getDisplaySecondaryK(this->parameterDisplayConfig.getDisplayDigits(eDimensionless));
        case eFeatureDisplayAperture:
            return feature->getFeature()->getDisplayAperture(this->parameterDisplayConfig.getDisplayUnit(eAngular),
                                                             this->parameterDisplayConfig.getDisplayDigits(eAngular));
        case eFeatureDisplayA:
            return feature->getFeature()->getDisplayA(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                      this->parameterDisplayConfig.getDisplayDigits(eMetric));
        case eFeatureDisplayB:
            return feature->getFeature()->getDisplayB(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                      this->parameterDisplayConfig.getDisplayDigits(eMetric));
        case eFeatureDisplayC:
            return feature->getFeature()->getDisplayC(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                      this->parameterDisplayConfig.getDisplayDigits(eMetric));
        case eFeatureDisplayAngle:
            return feature->getFeature()->getDisplayAngle(this->parameterDisplayConfig.getDisplayUnit(eAngular),
                                                          this->parameterDisplayConfig.getDisplayDigits(eAngular));
        case eFeatureDisplayDistance:
            return feature->getFeature()->getDisplayDistance(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                             this->parameterDisplayConfig.getDisplayDigits(eMetric));
        case eFeatureDisplayMeasurementSeries:
            return feature->getFeature()->getDisplayMeasurementSeries(this->parameterDisplayConfig.getDisplayUnit(eDimensionless),
                                                                      this->parameterDisplayConfig.getDisplayDigits(eDimensionless));
        case eFeatureDisplayTemperature:
            return feature->getFeature()->getDisplayTemperature(this->parameterDisplayConfig.getDisplayUnit(eTemperature),
                                                                this->parameterDisplayConfig.getDisplayDigits(eTemperature));
        case eFeatureDisplayLength:
            return feature->getFeature()->getDisplayLength(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                           this->parameterDisplayConfig.getDisplayDigits(eMetric));
        case eFeatureDisplayExpansionOriginX:
            return feature->getFeature()->getDisplayExpansionOriginX(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                                     this->parameterDisplayConfig.getDisplayDigits(eMetric));
        case eFeatureDisplayExpansionOriginY:
            return feature->getFeature()->getDisplayExpansionOriginY(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                                     this->parameterDisplayConfig.getDisplayDigits(eMetric));
        case eFeatureDisplayExpansionOriginZ:
            return feature->getFeature()->getDisplayExpansionOriginZ(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                                     this->parameterDisplayConfig.getDisplayDigits(eMetric));
        }

    }else if(getIsTrafoParamDisplayAttribute(attr)){ //trafo param attributes

        switch((TrafoParamDisplayAttributes)attr){
        case eTrafoParamDisplayStartSystem:
            return feature->getFeature()->getDisplayStartSystem();
        case eTrafoParamDisplayDestinationSystem:
            return feature->getFeature()->getDisplayDestinationSystem();
        case eTrafoParamDisplayTranslationX:
            return feature->getFeature()->getDisplayTranslationX(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                                 this->parameterDisplayConfig.getDisplayDigits(eMetric));
        case eTrafoParamDisplayTranslationY:
            return feature->getFeature()->getDisplayTranslationY(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                                 this->parameterDisplayConfig.getDisplayDigits(eMetric));
        case eTrafoParamDisplayTranslationZ:
            return feature->getFeature()->getDisplayTranslationZ(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                                 this->parameterDisplayConfig.getDisplayDigits(eMetric));
        case eTrafoParamDisplayRotationX:
            return feature->getFeature()->getDisplayRotationX(this->parameterDisplayConfig.getDisplayUnit(eAngular),
                                                              this->parameterDisplayConfig.getDisplayDigits(eAngular));
        case eTrafoParamDisplayRotationY:
            return feature->getFeature()->getDisplayRotationY(this->parameterDisplayConfig.getDisplayUnit(eAngular),
                                                              this->parameterDisplayConfig.getDisplayDigits(eAngular));
        case eTrafoParamDisplayRotationZ:
            return feature->getFeature()->getDisplayRotationZ(this->parameterDisplayConfig.getDisplayUnit(eAngular),
                                                              this->parameterDisplayConfig.getDisplayDigits(eAngular));
        case eTrafoParamDisplayScaleX:
            return feature->getFeature()->getDisplayScaleX(this->parameterDisplayConfig.getDisplayDigits(eDimensionless));
        case eTrafoParamDisplayScaleY:
            return feature->getFeature()->getDisplayScaleY(this->parameterDisplayConfig.getDisplayDigits(eDimensionless));
        case eTrafoParamDisplayScaleZ:
            return feature->getFeature()->getDisplayScaleZ(this->parameterDisplayConfig.getDisplayDigits(eDimensionless));
        case eTrafoParamDisplayIsUsed:
            return feature->getFeature()->getDisplayIsUsed();
        case eTrafoParamDisplayValidTime:
            return feature->getFeature()->getDisplayValidTime();
        case eTrafoParamDisplayIsMovement:
            return feature->getFeature()->getDisplayIsMovement();
        }

    }

    return QVariant();

}

/*!
 * \brief FeatureTableModel::getBackgroundValue
 * \param feature
 * \param column
 * \return
 */
QVariant FeatureTableModel::getBackgroundValue(const QPointer<FeatureWrapper> &feature, const int &column) const{

    //check if the column exists in available display attributes
    if(column < 0 || getFeatureDisplayAttributes().size() <= column){
        return QVariant();
    }

    //active feature
    if(feature->getFeature()->getIsActiveFeature()){
        return QColor(QColor::fromCmykF(0.59, 0.40, 0.10, 0.10).lighter());
    }

    //active station
    if(!feature->getStation().isNull() && feature->getStation()->getIsActiveStation()){
        return QColor(Qt::darkGray);
    }

    //inactive station
    if(!feature->getStation().isNull()){
        return QColor(Qt::lightGray);
    }

    //get the display attribute
    int attr = getFeatureDisplayAttributes().at(column);

    //set background for parameter cells of unsolved features
    if(!feature->getFeature()->getIsSolved() && getIsFeatureDisplayAttribute(attr)){ //feature attributes

        switch((FeatureDisplayAttributes)attr){
        case eFeatureDisplayX:
            return QColor(Qt::yellow);
        case eFeatureDisplayY:
            return QColor(Qt::yellow);
        case eFeatureDisplayZ:
            return QColor(Qt::yellow);
        case eFeatureDisplayPrimaryI:
            return QColor(Qt::yellow);
        case eFeatureDisplayPrimaryJ:
            return QColor(Qt::yellow);
        case eFeatureDisplayPrimaryK:
            return QColor(Qt::yellow);
        case eFeatureDisplayRadiusA:
            return QColor(Qt::yellow);
        case eFeatureDisplayRadiusB:
            return QColor(Qt::yellow);
        case eFeatureDisplaySecondaryI:
            return QColor(Qt::yellow);
        case eFeatureDisplaySecondaryJ:
            return QColor(Qt::yellow);
        case eFeatureDisplaySecondaryK:
            return QColor(Qt::yellow);
        case eFeatureDisplayAperture:
            return QColor(Qt::yellow);
        case eFeatureDisplayA:
            return QColor(Qt::yellow);
        case eFeatureDisplayB:
            return QColor(Qt::yellow);
        case eFeatureDisplayC:
            return QColor(Qt::yellow);
        case eFeatureDisplayAngle:
            return QColor(Qt::yellow);
        case eFeatureDisplayDistance:
            return QColor(Qt::yellow);
        case eFeatureDisplayMeasurementSeries:
            return QColor(Qt::yellow);
        case eFeatureDisplayTemperature:
            return QColor(Qt::yellow);
        case eFeatureDisplayLength:
            return QColor(Qt::yellow);
        case eFeatureDisplayExpansionOriginX:
            return QColor(Qt::yellow);
        case eFeatureDisplayExpansionOriginY:
            return QColor(Qt::yellow);
        case eFeatureDisplayExpansionOriginZ:
            return QColor(Qt::yellow);
        }

    }else if(getIsTrafoParamDisplayAttribute(attr)){ //trafo param attributes

        switch((TrafoParamDisplayAttributes)attr){
        case eTrafoParamDisplayTranslationX:
            return QColor(Qt::yellow);
        case eTrafoParamDisplayTranslationY:
            return QColor(Qt::yellow);
        case eTrafoParamDisplayTranslationZ:
            return QColor(Qt::yellow);
        case eTrafoParamDisplayRotationX:
            return QColor(Qt::yellow);
        case eTrafoParamDisplayRotationY:
            return QColor(Qt::yellow);
        case eTrafoParamDisplayRotationZ:
            return QColor(Qt::yellow);
        case eTrafoParamDisplayScaleX:
            return QColor(Qt::yellow);
        case eTrafoParamDisplayScaleY:
            return QColor(Qt::yellow);
        case eTrafoParamDisplayScaleZ:
            return QColor(Qt::yellow);
        }

    }

    //nominals
    if(!feature->getGeometry().isNull() && feature->getGeometry()->getIsNominal()){
        return QColor(QColor::fromRgb(230,230,180));
    }

    return QVariant();

}

/*!
 * \brief FeatureTableModel::connectJob
 */
void FeatureTableModel::connectJob(){

    QObject::connect(this->currentJob.data(), &OiJob::featureSetChanged, this, &FeatureTableModel::updateModel, Qt::AutoConnection);
    QObject::connect(this->currentJob.data(), &OiJob::activeCoordinateSystemChanged, this, &FeatureTableModel::updateModel, Qt::AutoConnection);
    QObject::connect(this->currentJob.data(), &OiJob::activeFeatureChanged, this, &FeatureTableModel::updateModel, Qt::AutoConnection);
    QObject::connect(this->currentJob.data(), &OiJob::activeStationChanged, this, &FeatureTableModel::updateModel, Qt::AutoConnection);
    QObject::connect(this->currentJob.data(), &OiJob::featureAttributesChanged, this, &FeatureTableModel::updateModel, Qt::AutoConnection);
    QObject::connect(this->currentJob.data(), &OiJob::featureRecalculated, this, &FeatureTableModel::updateModel, Qt::AutoConnection);
    QObject::connect(this->currentJob.data(), &OiJob::featuresRecalculated, this, &FeatureTableModel::updateModel, Qt::AutoConnection);
    QObject::connect(this->currentJob.data(), &OiJob::geometryMeasurementConfigChanged, this, &FeatureTableModel::updateModel, Qt::AutoConnection);
    QObject::connect(this->currentJob.data(), &OiJob::activeGroupChanged, this, &FeatureTableModel::updateModel, Qt::AutoConnection);

}

/*!
 * \brief FeatureTableModel::disconnectJob
 */
void FeatureTableModel::disconnectJob(){

    QObject::disconnect(this->currentJob.data(), &OiJob::featureSetChanged, this, &FeatureTableModel::updateModel);
    QObject::disconnect(this->currentJob.data(), &OiJob::activeCoordinateSystemChanged, this, &FeatureTableModel::updateModel);
    QObject::disconnect(this->currentJob.data(), &OiJob::activeFeatureChanged, this, &FeatureTableModel::updateModel);
    QObject::disconnect(this->currentJob.data(), &OiJob::activeStationChanged, this, &FeatureTableModel::updateModel);
    QObject::disconnect(this->currentJob.data(), &OiJob::featureAttributesChanged, this, &FeatureTableModel::updateModel);
    QObject::disconnect(this->currentJob.data(), &OiJob::featureRecalculated, this, &FeatureTableModel::updateModel);
    QObject::disconnect(this->currentJob.data(), &OiJob::featuresRecalculated, this, &FeatureTableModel::updateModel);
    QObject::disconnect(this->currentJob.data(), &OiJob::geometryMeasurementConfigChanged, this, &FeatureTableModel::updateModel);
    QObject::disconnect(this->currentJob.data(), &OiJob::activeGroupChanged, this, &FeatureTableModel::updateModel);

}
