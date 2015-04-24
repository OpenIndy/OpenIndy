#include "featuretablemodel.h"

/*!
 * \brief FeatureTableModel::FeatureTableModel
 * \param job
 * \param parent
 */
FeatureTableModel::FeatureTableModel(const QPointer<OiJob> &job, QObject *parent) : QAbstractTableModel(parent){
    this->currentJob = job;
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
    return getDisplayAttributes().size();
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

        return getDisplayAttributeName(getDisplayAttributes().at(section));

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
QString FeatureTableModel::getDisplayValue(const QPointer<FeatureWrapper> &feature, const int &column) const{

    //check if the column exists in available display attributes
    if(column < 0 || getDisplayAttributes().size() <= column){
        return QString("");
    }

    //get the display attribute
    int attr = getDisplayAttributes().at(column);

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

    return QString("");

}

/*!
 * \brief FeatureTableModel::connectJob
 */
void FeatureTableModel::connectJob(){

    QObject::connect(this->currentJob.data(), SIGNAL(featureSetChanged()), this, SLOT(updateModel()), Qt::AutoConnection);
    QObject::connect(this->currentJob.data(), SIGNAL(activeCoordinateSystemChanged()), this, SLOT(updateModel()), Qt::AutoConnection);
    QObject::connect(this->currentJob.data(), SIGNAL(activeFeatureChanged()), this, SLOT(updateModel()), Qt::AutoConnection);
    QObject::connect(this->currentJob.data(), SIGNAL(activeStationChanged()), this, SLOT(updateModel()), Qt::AutoConnection);
    QObject::connect(this->currentJob.data(), SIGNAL(featureAttributesChanged()), this, SLOT(updateModel()), Qt::AutoConnection);

}

/*!
 * \brief FeatureTableModel::disconnectJob
 */
void FeatureTableModel::disconnectJob()
{

}
