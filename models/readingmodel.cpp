#include "readingmodel.h"

/*!
 * \brief ReadingModel::ReadingModel
 * \param parent
 */
ReadingModel::ReadingModel(QObject *parent) :
    QAbstractTableModel(parent)
{
}

/*!
 * \brief ReadingModel::rowCount
 * \param parent
 * \return
 */
int ReadingModel::rowCount(const QModelIndex &parent) const{

    //check current job
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
 * \brief ReadingModel::columnCount
 * \param parent
 * \return
 */
int ReadingModel::columnCount(const QModelIndex &parent) const{
    return getReadingDisplayAttributes().size();
}

/*!
 * \brief ReadingModel::data
 * \param index
 * \param role
 * \return
 */
QVariant ReadingModel::data(const QModelIndex &index, int role) const{

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

    //get and check reading
    QPointer<Reading> reading(NULL);
    if(rowIndex < geometry->getObservations().size() && !geometry->getObservations().at(rowIndex).isNull()){
        reading = geometry->getObservations().at(rowIndex)->getReading();
    }
    if(reading.isNull()){
        return QVariant();
    }

    switch((ReadingDisplayAttributes)columnIndex){
    case eReadingDisplayId:
        return reading->getDisplayId();
    case eReadingDisplayType:
        return reading->getDisplayType();
    case eReadingDisplayTime:
        return reading->getDisplayTime();
    case eReadingDisplaySensor:
        return reading->getDisplaySensor();
    case eReadingDisplayIsFrontSide:
        return reading->getDisplayIsFrontside();
    case eReadingDisplayAzimuth:
        return reading->getDisplayAzimuth(this->parameterDisplayConfig.getDisplayUnit(eAngular),
                                          this->parameterDisplayConfig.getDisplayDigits(eAngular));
    case eReadingDisplayZenith:
        return reading->getDisplayZenith(this->parameterDisplayConfig.getDisplayUnit(eAngular),
                                         this->parameterDisplayConfig.getDisplayDigits(eAngular));
    case eReadingDisplayDistance:
        return reading->getDisplayDistance(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                           this->parameterDisplayConfig.getDisplayDigits(eMetric));
    case eReadingDisplayX:
        return reading->getDisplayX(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                    this->parameterDisplayConfig.getDisplayDigits(eMetric));
    case eReadingDisplayY:
        return reading->getDisplayY(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                    this->parameterDisplayConfig.getDisplayDigits(eMetric));
    case eReadingDisplayZ:
        return reading->getDisplayZ(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                    this->parameterDisplayConfig.getDisplayDigits(eMetric));
    case eReadingDisplayI:
        return reading->getDisplayI(6);
    case eReadingDisplayJ:
        return reading->getDisplayJ(6);
    case eReadingDisplayK:
        return reading->getDisplayK(6);
    case eReadingDisplayTemperature:
        return reading->getDisplayTemperature(this->parameterDisplayConfig.getDisplayUnit(eTemperature),
                                              this->parameterDisplayConfig.getDisplayDigits(eTemperature));
    case eReadingDisplaySigmaAzimuth:
        return reading->getDisplaySigmaAzimuth(this->parameterDisplayConfig.getDisplayUnit(eAngular),
                                               this->parameterDisplayConfig.getDisplayDigits(eAngular));
    case eReadingDisplaySigmaZenith:
        return reading->getDisplaySigmaZenith(this->parameterDisplayConfig.getDisplayUnit(eAngular),
                                              this->parameterDisplayConfig.getDisplayDigits(eAngular));
    case eReadingDisplaySigmaDistance:
        return reading->getDisplaySigmaDistance(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                this->parameterDisplayConfig.getDisplayDigits(eMetric));
    case eReadingDisplaySigmaX:
        return reading->getDisplaySigmaX(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                         this->parameterDisplayConfig.getDisplayDigits(eMetric));
    case eReadingDisplaySigmaY:
        return reading->getDisplaySigmaY(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                         this->parameterDisplayConfig.getDisplayDigits(eMetric));
    case eReadingDisplaySigmaZ:
        return reading->getDisplaySigmaZ(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                         this->parameterDisplayConfig.getDisplayDigits(eMetric));
    case eReadingDisplaySigmaI:
        return reading->getDisplaySigmaI(6);
    case eReadingDisplaySigmaJ:
        return reading->getDisplaySigmaJ(6);
    case eReadingDisplaySigmaK:
        return reading->getDisplaySigmaK(6);
    case eReadingDisplaySigmaTemperature:
        return reading->getDisplaySigmaTemperature(this->parameterDisplayConfig.getDisplayUnit(eTemperature),
                                                   this->parameterDisplayConfig.getDisplayDigits(eTemperature));
    }

    return QVariant();

}

/*!
 * \brief ReadingModel::headerData
 * \param section
 * \param orientation
 * \param role
 * \return
 */
QVariant ReadingModel::headerData(int section, Qt::Orientation orientation, int role) const{

    if((Qt::DisplayRole == role) &&
            (Qt::Horizontal == orientation)&&
            (0 <= section) &&
            (section < columnCount())){

        return getReadingDisplayAttributeName(getReadingDisplayAttributes().at(section));

    }

    return QVariant();

}

/*!
 * \brief ReadingModel::flags
 * \param index
 * \return
 */
Qt::ItemFlags ReadingModel::flags(const QModelIndex &index) const{
    Qt::ItemFlags myFlags = QAbstractTableModel::flags(index);
    return (myFlags | Qt::ItemIsEditable);
}

/*!
 * \brief ReadingModel::setData
 * \param index
 * \param value
 * \param role
 * \return
 */
bool ReadingModel::setData(const QModelIndex &index, const QVariant &value, int role){
    return false;
}

/*!
 * \brief ReadingModel::getCurrentJob
 * \return
 */
const QPointer<OiJob> &ReadingModel::getCurrentJob() const{
    return this->currentJob;
}

/*!
 * \brief ReadingModel::setCurrentJob
 * \param job
 */
void ReadingModel::setCurrentJob(const QPointer<OiJob> &job){
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
 * \brief ReadingModel::getParameterDisplayConfig
 * \return
 */
const ParameterDisplayConfig &ReadingModel::getParameterDisplayConfig() const{
    return this->parameterDisplayConfig;
}

/*!
 * \brief ReadingModel::setParameterDisplayConfig
 * \param config
 */
void ReadingModel::setParameterDisplayConfig(const ParameterDisplayConfig &config){
    this->parameterDisplayConfig = config;
    this->updateModel();
}

/*!
 * \brief ReadingModel::updateModel
 */
void ReadingModel::updateModel(){
    emit layoutAboutToBeChanged();
    emit layoutChanged();
}

/*!
 * \brief ReadingModel::connectJob
 */
void ReadingModel::connectJob(){

    //check current job
    if(this->currentJob.isNull()){
        return;
    }

    QObject::connect(this->currentJob.data(), &OiJob::activeFeatureChanged, this, &ReadingModel::updateModel, Qt::AutoConnection);
    QObject::connect(this->currentJob.data(), &OiJob::geometryObservationsChanged, this, &ReadingModel::updateModel, Qt::AutoConnection);

}

/*!
 * \brief ReadingModel::disconnectJob
 */
void ReadingModel::disconnectJob(){

    //check current job
    if(this->currentJob.isNull()){
        return;
    }

    QObject::disconnect(this->currentJob.data(), &OiJob::activeFeatureChanged, this, &ReadingModel::updateModel);
    QObject::disconnect(this->currentJob.data(), &OiJob::geometryObservationsChanged, this, &ReadingModel::updateModel);

}
