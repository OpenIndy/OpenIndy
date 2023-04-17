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
    if(role != Qt::DisplayRole && role != Qt::CheckStateRole && role != Qt::TextAlignmentRole){
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

    ObservationDisplayAttributes key = (ObservationDisplayAttributes)columnIndex;
    if(role == Qt::DisplayRole){

        switch(key){
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
        case eObservationDisplayI:
            return observation->getDisplayI(6);
        case eObservationDisplayJ:
            return observation->getDisplayJ(6);
        case eObservationDisplayK:
            return observation->getDisplayK(6);
        case eObservationDisplaySigmaX:
            return observation->getDisplaySigmaX(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                 this->parameterDisplayConfig.getDisplayDigits(eMetric));
        case eObservationDisplaySigmaY:
            return observation->getDisplaySigmaY(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                 this->parameterDisplayConfig.getDisplayDigits(eMetric));
        case eObservationDisplaySigmaZ:
            return observation->getDisplaySigmaZ(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                 this->parameterDisplayConfig.getDisplayDigits(eMetric));
        case eObservationDisplaySigmaI:
            return observation->getDisplaySigmaI(6);
        case eObservationDisplaySigmaJ:
            return observation->getDisplaySigmaJ(6);
        case eObservationDisplaySigmaK:
            return observation->getDisplaySigmaK(6);
        case eObservationDisplayIsValid:
            //return observation->getDisplayIsValid();
            return QVariant();
        case eObservationDisplayIsSolved:
            //return observation->getDisplayIsSolved();
            return QVariant();
        case eObservationDisplayVX:
        case eObservationDisplayVY:
        case eObservationDisplayVZ:
        case eObservationDisplayV:
        case eObservationDisplayVR:
            if(geometry->getFunctions().size() > 0 && !geometry->getFunctions().at(0).isNull()){
                const Statistic &statistic = geometry->getFunctions().at(0)->getStatistic();
                Residual residual = statistic.getDisplayResidual(observation->getId());
                QString attr = getObservationDisplayAttributesName(key);
                if(residual.elementId == observation->getId() && residual.corrections.contains(attr)){
                    return QString::number(convertFromDefault(residual.corrections[attr],
                                                              this->parameterDisplayConfig.getDisplayUnit(eMetric)),
                                           'f', this->parameterDisplayConfig.getDisplayDigits(eMetric));
                }
            }
            break;
        case eObservationDisplayReadingTime:
            return observation->getReading().isNull() ? QVariant() : observation->getReading()->getDisplayTime();
        }

    }else if(role == Qt::CheckStateRole){

        switch(key){
        case eObservationDisplayIsUsed:
            if(geometry->getFunctions().size() > 0 && !geometry->getFunctions().at(0).isNull()){
                QPointer<Function> function = geometry->getFunctions()[0];
                return function->getShouldBeUsed(0, observation->getId())?Qt::Checked:Qt::Unchecked;
            }
            return Qt::Unchecked;
        case eObservationDisplayIsSolved:
            if(observation->getIsSolved()){
                return Qt::Checked;
            }
            return Qt::Unchecked;
        case eObservationDisplayIsValid:
            if(observation->getIsValid()){
                return Qt::Checked;
            }
            return Qt::Unchecked;
        case eObservationDisplayIsDummyPoint:
            if(observation->getIsDummyPoint()){
                return Qt::Checked;
            }
            return Qt::Unchecked;
        }

    }else if(role == Qt::TextAlignmentRole){

        switch(key){
        case eObservationDisplayX:
        case eObservationDisplayY:
        case eObservationDisplayZ:
        case eObservationDisplayI:
        case eObservationDisplayJ:
        case eObservationDisplayK:
        case eObservationDisplaySigmaX:
        case eObservationDisplaySigmaY:
        case eObservationDisplaySigmaZ:
        case eObservationDisplaySigmaI:
        case eObservationDisplaySigmaJ:
        case eObservationDisplaySigmaK:
        case eObservationDisplayVX:
        case eObservationDisplayVY:
        case eObservationDisplayVZ:
        case eObservationDisplayV:
        case eObservationDisplayVR:
            return QVariant(Qt::AlignRight | Qt::AlignVCenter);
        }

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

    //check index
    if(!index.isValid()){
        return myFlags;
    }

    QList<ObservationDisplayAttributes> attributes = getObservationDisplayAttributes();
    ObservationDisplayAttributes attr = attributes.at(index.column());
    if(attr == eObservationDisplayIsUsed){
        return (myFlags | Qt::ItemIsEditable | Qt::ItemIsUserCheckable);
    }else if(attr = eObservationDisplayIsSolved){
        return (myFlags | Qt::ItemIsUserCheckable);
    }else if(attr = eObservationDisplayIsValid){
        return (myFlags | Qt::ItemIsUserCheckable);
    }

    return myFlags;

}

/*!
 * \brief ObservationModel::setData
 * \param index
 * \param value
 * \param role
 * \return
 */
bool ObservationModel::setData(const QModelIndex &index, const QVariant &value, int role){

    //check current job and model index
    if(this->currentJob.isNull() || !index.isValid()){
        return false;
    }

    //get and check active feature
    QPointer<FeatureWrapper> feature = this->currentJob->getActiveFeature();
    if(feature.isNull() || feature->getFeature().isNull()){
        return false;
    }

    //get and check active geometry
    QPointer<Geometry> geometry(NULL);
    if(getIsGeometry(feature->getFeatureTypeEnum())){
        geometry = feature->getGeometry();
    }else if(feature->getFeatureTypeEnum() == eStationFeature && !feature->getStation().isNull()){
        geometry = feature->getStation()->getPosition();
    }
    if(geometry.isNull()){
        return false;
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
        return false;
    }

    if(role == Qt::CheckStateRole){

        ObservationDisplayAttributes attr = getObservationDisplayAttributes().at(columnIndex);
        if(attr == eObservationDisplayIsUsed){

            //add or remove input observation
            if((Qt::CheckState)value.toInt() == Qt::Checked){
                emit this->setShouldBeUsed(feature, 0, 0, observation->getId(), true, true);
                return true;
            }else{
                emit this->setShouldBeUsed(feature, 0, 0, observation->getId(), false, true);
                return true;
            }

        }

    }

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
 * \brief ObservationModel::getObservation
 * \param index
 * \return
 */
QPointer<Observation> ObservationModel::getObservation(const QModelIndex &index) {
    QPointer<Observation> observation(NULL);
    //check current job and model index
    if(this->currentJob.isNull() || !index.isValid()){
        return observation;
    }

    //get and check active feature
    QPointer<FeatureWrapper> feature = this->currentJob->getActiveFeature();
    if(feature.isNull() || feature->getFeature().isNull()){
        return observation;
    }

    //get and check active geometry
    QPointer<Geometry> geometry(NULL);
    if(getIsGeometry(feature->getFeatureTypeEnum())){
        geometry = feature->getGeometry();
    }else if(feature->getFeatureTypeEnum() == eStationFeature && !feature->getStation().isNull()){
        geometry = feature->getStation()->getPosition();
    }
    if(geometry.isNull()){
        return observation;
    }

    //get row and column indices
    int rowIndex = index.row();
    int columnIndex = index.column();

    //get and check observation

    if(rowIndex < geometry->getObservations().size()){
        observation = geometry->getObservations().at(rowIndex);
    }
    return observation;
}

/*!
 * \brief ObservationModel::setObservationUseStateByContextmenu
 * \param use
 * \param index
 */
void ObservationModel::setObservationUseStateByContextmenu(bool use, const QModelIndex &index)
{
    QPointer<Observation> observation = this->getObservation(index);
    if(observation.isNull()){
        return;
    }
    emit this->setShouldBeUsed(this->currentJob->getActiveFeature(), 0, 0, observation->getId(), use, true);
}
/*!
 * \brief ObservationModel::getObservationIdByIndex
 * \param index
 * \return
 */
int ObservationModel::getObservationIdByIndex(const QModelIndex &index)
{
    QPointer<Observation> observation = this->getObservation(index);
    if(observation.isNull()){
        return -1;
    }
    return observation->getId();
}

/*!
 * \brief ObservationModel::updateModel
 */
void ObservationModel::updateModel(){
    emit layoutAboutToBeChanged();
    emit layoutChanged();
}

/*!
 * \brief ObservationModel::featureRecalculated
 * \param featureId
 */
void ObservationModel::featureRecalculated(const int &featureId){

    //check current job and model index
    if(this->currentJob.isNull()){
        return;
    }

    //get and check active feature
    QPointer<FeatureWrapper> feature = this->currentJob->getActiveFeature();
    if(feature.isNull() || feature->getFeature().isNull()){
        return;
    }

    //check wether the recalculated feature equals the active feature
    if(feature->getFeature()->getId() == featureId){
        this->updateModel();
    }

}

/*!
 * \brief ObservationModel::connectJob
 */
void ObservationModel::connectJob(){

    //check job
    if(this->currentJob.isNull()){
        return;
    }

    QObject::connect(this->currentJob.data(), &OiJob::activeFeatureChanged, this, &ObservationModel::updateModel, Qt::AutoConnection);
    QObject::connect(this->currentJob.data(), &OiJob::geometryObservationsChanged, this, &ObservationModel::updateModel, Qt::AutoConnection);
    QObject::connect(this->currentJob.data(), &OiJob::featureRecalculated, this, &ObservationModel::featureRecalculated, Qt::AutoConnection);

    QObject::connect(this, &ObservationModel::setShouldBeUsed, this->currentJob.data(), &OiJob::setShouldBeUsed, Qt::AutoConnection);

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
    QObject::disconnect(this->currentJob.data(), &OiJob::featureRecalculated, this, &ObservationModel::featureRecalculated);

    QObject::disconnect(this, &ObservationModel::setShouldBeUsed, this->currentJob.data(), &OiJob::setShouldBeUsed);

}
