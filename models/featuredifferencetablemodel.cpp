#include "featuredifferencetablemodel.h"

/*!
 * \brief FeatureDifferenceTableModel::FeatureDifferenceTableModel
 * \param job
 * \param parent
 */
FeatureDifferenceTableModel::FeatureDifferenceTableModel(const QPointer<OiJob> &job, QObject *parent)
{
    this->setCurrentJob(job);
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
        return 1;
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
            break;
        case 1: return "dX";
            break;
        case 2: return "dY";
            break;
        case 3: return "dZ";
            break;
        case 4: return "dI";
            break;
        case 5: return "dJ";
            break;
        case 6: return "dK";
            break;
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
    return Qt::ItemIsEnabled;
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
