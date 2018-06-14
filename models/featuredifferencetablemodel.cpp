#include "featuredifferencetablemodel.h"

/*!
 * \brief FeatureDifferenceTableModel::FeatureDifferenceTableModel
 * \param job
 * \param parent
 */
FeatureDifferenceTableModel::FeatureDifferenceTableModel(const QPointer<OiJob> &job, QObject *parent)
{
    this->setCurrentJob(job);
    tolerance = 0.2;
    value = 0.0;
}

/*!
 * \brief FeatureDifferenceTableModel::FeatureDifferenceTableModel
 * \param parent
 */
FeatureDifferenceTableModel::FeatureDifferenceTableModel(QObject *parent)
{

}

/*!
 * \brief FeatureDifferenceTableModel::rowCount
 * \param parent
 * \return
 */
int FeatureDifferenceTableModel::rowCount(const QModelIndex &parent) const
{
    if(this->currentJob.isNull()){
        return 0;
    }
    return this->currentJob->getFeatureCount();
}

/*!
 * \brief FeatureDifferenceTableModel::columnCount
 * \param parent
 * \return
 */
int FeatureDifferenceTableModel::columnCount(const QModelIndex &parent) const
{
    return 7; // name x y z dI dJ dK
}

/*!
 * \brief FeatureDifferenceTableModel::data
 * \param index
 * \param role
 * \return
 */
QVariant FeatureDifferenceTableModel::data(const QModelIndex &index, int role) const
{
    //check current job and model index
    if(this->currentJob.isNull() || !index.isValid()){
        return QVariant();
    }

    //get the feature to display at index.row()
    if(this->currentJob->getFeatureCount() <= index.row()){
        return QVariant();
    }

    QPointer<FeatureWrapper> feature = this->currentJob->getFeaturesList().at(index.row());

    //check the feature
    if(feature.isNull() || feature->getFeature().isNull()){
        return QVariant();
    }

    if(feature->getGeometry().isNull() || feature->getGeometry()->getIsNominal() || feature->getGeometry()->getNominals().isEmpty()){
        return QVariant();
    }

    double value = 0.0;
    if(role == Qt::DisplayRole){

        if(index.column() == 0){
            return feature->getFeature()->getFeatureName();
        }else{
            QMap<bool, double> result = this->getDifference(feature, index);

            if(result.keys().at(0)){
                value = result.value(true);
                value = convertFromDefault(value, this->parameterDisplayConfig.getDisplayUnit(eMetric));
                return QString::number(value, 'f', this->parameterDisplayConfig.getDisplayDigits(eMetric));
            }else{
                return "-/-";
            }
        }
    }else if(role == Qt::BackgroundRole){

        if(abs(value) > abs(convertFromDefault(this->tolerance, this->parameterDisplayConfig.getDisplayUnit(eMetric)))){
            return QColor(Qt::red);
        }else{
            return QVariant();
        }
    }
    return QVariant();
}

/*!
 * \brief FeatureDifferenceTableModel::headerData
 * \param section
 * \param orientation
 * \param role
 * \return
 */
QVariant FeatureDifferenceTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if((Qt::DisplayRole == role) &&
            (Qt::Horizontal == orientation) &&
            (0 <= section) &&
            (section < columnCount())){

        switch (section) {
        case 0:
            return "feature name";
        case 1:
            return QString("dX" + getUnitTypeName(this->parameterDisplayConfig.getDisplayUnit(eMetric)));
        case 2:
            return QString("dY" + getUnitTypeName(this->parameterDisplayConfig.getDisplayUnit(eMetric)));
        case 3:
            return QString("dZ" + getUnitTypeName(this->parameterDisplayConfig.getDisplayUnit(eMetric)));
        case 4:
            return QString("dI" + getUnitTypeName(this->parameterDisplayConfig.getDisplayUnit(eDimensionless)));
        case 5:
            return QString("dJ" + getUnitTypeName(this->parameterDisplayConfig.getDisplayUnit(eDimensionless)));
        case 6:
            return QString("dK" + getUnitTypeName(this->parameterDisplayConfig.getDisplayUnit(eDimensionless)));
        default:
            break;
        }
    }
    return QVariant();
}

/*!
 * \brief FeatureDifferenceTableModel::flags
 * \param index
 * \return
 */
Qt::ItemFlags FeatureDifferenceTableModel::flags(const QModelIndex &index) const
{
    //get parent flags
    Qt::ItemFlags myFlags = QAbstractTableModel::flags(index);
    return myFlags;
}

/*!
 * \brief FeatureDifferenceTableModel::setData
 * \param index
 * \param value
 * \param role
 * \return
 */
/*bool FeatureDifferenceTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    return false;
}*/

/*!
 * \brief FeatureDifferenceTableModel::getCurrentJob
 * \return
 */
const QPointer<OiJob> &FeatureDifferenceTableModel::getCurrentJob() const
{
    return this->currentJob;
}

/*!
 * \brief FeatureDifferenceTableModel::setCurrentJob
 * \param job
 */
void FeatureDifferenceTableModel::setCurrentJob(const QPointer<OiJob> &job)
{
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
 * \brief FeatureDifferenceTableModel::getParameterDisplayConfig
 * \return
 */
const ParameterDisplayConfig &FeatureDifferenceTableModel::getParameterDisplayConfig() const
{
    return this->parameterDisplayConfig;
}

/*!
 * \brief FeatureDifferenceTableModel::setParameterDisplayConfig
 * \param config
 */
void FeatureDifferenceTableModel::setParameterDisplayConfig(const ParameterDisplayConfig &config)
{
    this->parameterDisplayConfig = config;
    this->updateModel();
}

/*!
 * \brief FeatureDifferenceTableModel::setTolerance
 * \param tolerance
 */
void FeatureDifferenceTableModel::setTolerance(double tolerance)
{
    this->tolerance = tolerance;
}

/*!
 * \brief FeatureDifferenceTableModel::updateModel
 */
void FeatureDifferenceTableModel::updateModel()
{
    emit this->layoutAboutToBeChanged();
    emit this->layoutChanged();
}

/*!
 * \brief FeatureDifferenceTableModel::connectJob
 */
void FeatureDifferenceTableModel::connectJob()
{
    QObject::connect(this->currentJob.data(), &OiJob::featureSetChanged, this, &FeatureDifferenceTableModel::updateModel, Qt::AutoConnection);
    QObject::connect(this->currentJob.data(), &OiJob::activeCoordinateSystemChanged, this, &FeatureDifferenceTableModel::updateModel, Qt::AutoConnection);
    QObject::connect(this->currentJob.data(), &OiJob::activeFeatureChanged, this, &FeatureDifferenceTableModel::updateModel, Qt::AutoConnection);
    QObject::connect(this->currentJob.data(), &OiJob::activeStationChanged, this, &FeatureDifferenceTableModel::updateModel, Qt::AutoConnection);
    QObject::connect(this->currentJob.data(), &OiJob::featureAttributesChanged, this, &FeatureDifferenceTableModel::updateModel, Qt::AutoConnection);
    QObject::connect(this->currentJob.data(), &OiJob::featureRecalculated, this, &FeatureDifferenceTableModel::updateModel, Qt::AutoConnection);
    QObject::connect(this->currentJob.data(), &OiJob::featuresRecalculated, this, &FeatureDifferenceTableModel::updateModel, Qt::AutoConnection);
    QObject::connect(this->currentJob.data(), &OiJob::geometryMeasurementConfigChanged, this, &FeatureDifferenceTableModel::updateModel, Qt::AutoConnection);
    QObject::connect(this->currentJob.data(), &OiJob::activeGroupChanged, this, &FeatureDifferenceTableModel::updateModel, Qt::AutoConnection);
    QObject::connect(this->currentJob.data(), &OiJob::geometryIsCommonChanged, this, &FeatureDifferenceTableModel::updateModel, Qt::AutoConnection);
    QObject::connect(this->currentJob.data(), &OiJob::trafoParamIsDatumChanged, this, &FeatureDifferenceTableModel::updateModel, Qt::AutoConnection);
}

/*!
 * \brief FeatureDifferenceTableModel::disconnectJob
 */
void FeatureDifferenceTableModel::disconnectJob()
{
    QObject::disconnect(this->currentJob.data(), &OiJob::featureSetChanged, this, &FeatureDifferenceTableModel::updateModel);
    QObject::disconnect(this->currentJob.data(), &OiJob::activeCoordinateSystemChanged, this, &FeatureDifferenceTableModel::updateModel);
    QObject::disconnect(this->currentJob.data(), &OiJob::activeFeatureChanged, this, &FeatureDifferenceTableModel::updateModel);
    QObject::disconnect(this->currentJob.data(), &OiJob::activeStationChanged, this, &FeatureDifferenceTableModel::updateModel);
    QObject::disconnect(this->currentJob.data(), &OiJob::featureAttributesChanged, this, &FeatureDifferenceTableModel::updateModel);
    QObject::disconnect(this->currentJob.data(), &OiJob::featureRecalculated, this, &FeatureDifferenceTableModel::updateModel);
    QObject::disconnect(this->currentJob.data(), &OiJob::featuresRecalculated, this, &FeatureDifferenceTableModel::updateModel);
    QObject::disconnect(this->currentJob.data(), &OiJob::geometryMeasurementConfigChanged, this, &FeatureDifferenceTableModel::updateModel);
    QObject::disconnect(this->currentJob.data(), &OiJob::activeGroupChanged, this, &FeatureDifferenceTableModel::updateModel);
    QObject::disconnect(this->currentJob.data(), &OiJob::geometryIsCommonChanged, this, &FeatureDifferenceTableModel::updateModel);
    QObject::disconnect(this->currentJob.data(), &OiJob::trafoParamIsDatumChanged, this, &FeatureDifferenceTableModel::updateModel);
}

/*!
 * \brief FeatureDifferenceTableModel::getDifference
 * \param feature
 * calculate the difference between actual and nominal, if there is no actual, or no nominal return -/-
 * \return
 */
QMap<bool, double> FeatureDifferenceTableModel::getDifference(QPointer<FeatureWrapper> feature, const QModelIndex index) const
{
    double value = 0.0;
    QMap<bool, double> result;

    if(feature.isNull() || feature->getGeometry().isNull()){
        result.insert(false, value);
        return result;
    }

    if(!feature->getGeometry()->getIsNominal() && !feature->getGeometry()->getNominals().isEmpty() && feature->getGeometry()->hasPosition()){

        foreach (QPointer<Geometry> geom, feature->getGeometry()->getNominals()) {
            if(geom->getIsSolved() && feature->getGeometry()->getIsSolved()){

                switch (index.column()) {
                case 1: //x
                    value = feature->getGeometry()->getPosition().getVector().getAt(0) - geom->getPosition().getVector().getAt(0);
                    result.insert(true, value);
                    return result;
                case 2: //y
                    value = feature->getGeometry()->getPosition().getVector().getAt(1) - geom->getPosition().getVector().getAt(1);
                    result.insert(true, value);
                    return result;
                case 3: //z
                    value = feature->getGeometry()->getPosition().getVector().getAt(2) - geom->getPosition().getVector().getAt(2);
                    result.insert(true, value);
                    return result;
                default:
                    break;
                }
            }else{
                result.insert(false, value);
                return result;
            }
        }
    }

    if(!feature->getGeometry()->getIsNominal() && !feature->getGeometry()->getNominals().isEmpty() && feature->getGeometry()->hasDirection()){

        foreach (QPointer<Geometry> geom, feature->getGeometry()->getNominals()) {
            if(geom->getIsSolved() && feature->getGeometry()->getIsSolved()){

                switch (index.column()) {
                case 4: //i
                    value = feature->getGeometry()->getDirection().getVector().getAt(0) - geom->getDirection().getVector().getAt(0);
                    result.insert(true, value);
                    return result;
                case 5: //j
                    value = feature->getGeometry()->getDirection().getVector().getAt(1) - geom->getDirection().getVector().getAt(1);
                    result.insert(true, value);
                    return result;
                case 6: //k
                    value = feature->getGeometry()->getDirection().getVector().getAt(2) - geom->getDirection().getVector().getAt(2);
                    result.insert(true, value);
                    return result;
                default:
                    break;
                }
            }else{
                result.insert(false, value);
                return result;
            }
        }
    }

    result.insert(false, value);
    return result;
}
