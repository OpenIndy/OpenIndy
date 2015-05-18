#include "observationmodel.h"

/*!
 * \brief ObservationModel::ObservationModel
 * \param parent
 */
ObservationModel::ObservationModel(QObject *parent ) :
    QAbstractTableModel(parent)
{
}

/*!
 * \brief ObservationModel::rowCount
 * \return
 */
int ObservationModel::rowCount(const QModelIndex &parent) const{

    //check current job and model index
    if(this->currentJob.isNull()){
        return 0;
    }

    //get and check active feature
    QPointer<FeatureWrapper> feature = this->currentJob->getActiveFeature();
    if(feature.isNull() || feature->getFeature().isNull()){
        return 0;
    }

    //get and check active geometry
    QPointer<Geometry> geometry(NULL);
    if(getIsGeometry(feature->getFeatureTypeEnum())){
        geometry = feature->getGeometry();
    }else if(feature->getFeatureTypeEnum() == eStationFeature && !feature->getStation().isNull()){
        geometry = feature->getStation()->getPosition();
    }
    if(geometry.isNull()){
        return 0;
    }

    return geometry->getObservations().size();

}

/*!
 * \brief ObservationModel::columnCount
 * \param parent
 * \return
 */
int ObservationModel::columnCount(const QModelIndex &parent) const{
    return getObservationDisplayAttributes().size();
}

/*!
 * \brief ObservationModel::data
 * \param index
 * \param role
 * \return
 */
QVariant ObservationModel::data(const QModelIndex &index, int role) const{

    //check role
    if(role != Qt::DisplayRole){
        return QVariant();
    }

    //check current job and model index
    if(this->currentJob.isNull() || !index.isValid()){
        return QVariant();
    }

    //get and check active feature
    QPointer<FeatureWrapper> feature = this->currentJob->getActiveFeature();
    if(feature.isNull() || feature->getFeature().isNull()){
        return QVariant();
    }

    //get and check active geometry
    QPointer<Geometry> geometry(NULL);
    if(getIsGeometry(feature->getFeatureTypeEnum())){
        geometry = feature->getGeometry();
    }else if(feature->getFeatureTypeEnum() == eStationFeature && !feature->getStation().isNull()){
        geometry = feature->getStation()->getPosition();
    }
    if(geometry.isNull()){
        return QVariant();
    }

    //get row and column indices
    int rowIndex = index.row();
    int columnIndex = index.column();

    //get and check observation
    QPointer<Observation> observation(NULL);
    if(rowIndex < geometry->getObservations().size()){
        observation = geometry->getObservations().at(rowIndex);
    }
    if(observation.isNull()){
        return QVariant();
    }

    switch((ObservationDisplayattributes)columnIndex){
    case eObservationDisplayId:
        return observation->getDisplayId();
    case eObservationDisplayStation:
        return observation->getDisplayStation();
    case eObservationDisplayTargetGeometries:
        return observation->getDisplayTargetGeometries();
    case eObservationDisplayX:
        return observation->getDisplayX(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                        this->parameterDisplayConfig.getDisplayDigits(eMetric));
    case eObservationDisplayY:
        return observation->getDisplayY(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                        this->parameterDisplayConfig.getDisplayDigits(eMetric));
    case eObservationDisplayZ:
        return observation->getDisplayZ(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                        this->parameterDisplayConfig.getDisplayDigits(eMetric));
    case eObservationDisplaySigmaX:
        return observation->getDisplaySigmaX(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                             this->parameterDisplayConfig.getDisplayDigits(eMetric));
    case eObservationDisplaySigmaY:
        return observation->getDisplaySigmaY(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                             this->parameterDisplayConfig.getDisplayDigits(eMetric));
    case eObservationDisplaySigmaZ:
        return observation->getDisplaySigmaZ(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                             this->parameterDisplayConfig.getDisplayDigits(eMetric));
    case eObservationDisplayIsValid:
        return observation->getDisplayIsValid();
    case eObservationDisplayIsSolved:
        return observation->getDisplayIsSolved();
    }

    return QVariant();

}

/*!
 * \brief ObservationModel::headerData
 * \param section
 * \param orientation
 * \param role
 * \return
 */
QVariant ObservationModel::headerData(int section, Qt::Orientation orientation, int role) const{

    if((Qt::DisplayRole == role) &&
            (Qt::Horizontal == orientation)&&
            (0 <= section) &&
            (section < columnCount())){

        return getObservationDisplayAttributesName(getObservationDisplayAttributes().at(section));

    }

    return QVariant();

}

/*!
 * \brief ObservationModel::flags
 * \param index
 * \return
 */
Qt::ItemFlags ObservationModel::flags(const QModelIndex &index) const{
    Qt::ItemFlags myFlags = QAbstractTableModel::flags(index);
    return (myFlags | Qt::ItemIsEditable);
}

/*!
 * \brief ObservationModel::setData
 * \param index
 * \param value
 * \param role
 * \return
 */
bool ObservationModel::setData(const QModelIndex &index, const QVariant &value, int role){
    /*
    if(!index.isValid()){
        return false;
    }

    if(OiJob::getActiveFeature() != NULL){
        if(index.column() == 11){ //use state of observation
            OiJob::getActiveFeature()->getGeometry()->getObservations().at(index.row())->setIsUsed(value.toBool());
            emit recalcFeature();
        }
        return true;
    }*/
    return false;
}

/*!
 * \brief ObservationModel::getCurrentJob
 * \return
 */
const QPointer<OiJob> &ObservationModel::getCurrentJob() const{
    return this->currentJob;
}

/*!
 * \brief ObservationModel::setCurrentJob
 * \param job
 */
void ObservationModel::setCurrentJob(const QPointer<OiJob> &job){
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
 * \brief ObservationModel::getParameterDisplayConfig
 * \return
 */
const ParameterDisplayConfig &ObservationModel::getParameterDisplayConfig() const{
    return this->parameterDisplayConfig;
}

/*!
 * \brief ObservationModel::setParameterDisplayConfig
 * \param config
 */
void ObservationModel::setParameterDisplayConfig(const ParameterDisplayConfig &config){
    this->parameterDisplayConfig = config;
    this->updateModel();
}

/*!
 * \brief ObservationModel::updateModel
 */
void ObservationModel::updateModel(){
    emit layoutAboutToBeChanged();
    emit layoutChanged();
}

/*!
 * \brief ObservationModel::connectJob
 */
void ObservationModel::connectJob(){

    QObject::connect(this->currentJob.data(), &OiJob::activeFeatureChanged, this, &ObservationModel::updateModel, Qt::AutoConnection);
    QObject::connect(this->currentJob.data(), &OiJob::geometryObservationsChanged, this, &ObservationModel::updateModel, Qt::AutoConnection);

}

/*!
 * \brief ObservationModel::disconnectJob
 */
void ObservationModel::disconnectJob(){

    //check current job
    if(this->currentJob.isNull()){
        return;
    }

    QObject::disconnect(this->currentJob.data(), &OiJob::activeFeatureChanged, this, &ObservationModel::updateModel);
    QObject::disconnect(this->currentJob.data(), &OiJob::geometryObservationsChanged, this, &ObservationModel::updateModel);

}
