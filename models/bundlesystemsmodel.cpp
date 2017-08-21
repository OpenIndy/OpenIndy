#include "bundlesystemsmodel.h"

/*!
 * \brief BundleSystemsModel::BundleSystemsModel
 * \param parent
 */
BundleSystemsModel::BundleSystemsModel(QObject *parent) : QAbstractListModel(parent){

}

/*!
 * \brief BundleSystemsModel::rowCount
 * \param parent
 * \return
 */
int BundleSystemsModel::rowCount(const QModelIndex &parent) const{
    return this->bundleSystems.size();
}

/*!
 * \brief BundleSystemsModel::columnCount
 * \param parent
 * \return
 */
int BundleSystemsModel::columnCount(const QModelIndex &parent) const{
    return 1;
}

/*!
 * \brief BundleSystemsModel::data
 * \param index
 * \param role
 * \return
 */
QVariant BundleSystemsModel::data(const QModelIndex &index, int role) const{

    //check index
    if(!index.isValid()){
        return QVariant();
    }

    //check role
    if(role != Qt::DisplayRole){
        return QVariant();
    }

    //get row index
    int row = index.row();

    //get and check bundle system
    QPointer<CoordinateSystem> bundle(NULL);
    if(row < this->bundleSystems.size()){
        bundle = this->bundleSystems.at(row);
    }
    if(bundle.isNull()){
        return QVariant();
    }

    return bundle->getFeatureName();

}

/*!
 * \brief BundleSystemsModel::headerData
 * \param section
 * \param orientation
 * \param role
 * \return
 */
QVariant BundleSystemsModel::headerData(int section, Qt::Orientation orientation, int role) const{
    if((Qt::DisplayRole == role) &&
            (Qt::Horizontal == orientation) &&
            (0 <= section) &&
            (section < columnCount())){

        return "bundle systems";

    }
    return QVariant();
}

/*!
 * \brief BundleSystemsModel::flags
 * \param index
 * \return
 */
Qt::ItemFlags BundleSystemsModel::flags(const QModelIndex &index) const{
    Qt::ItemFlags myFlags = QAbstractListModel::flags(index);
    return myFlags;
}

/*!
 * \brief BundleSystemsModel::setData
 * \param index
 * \param value
 * \param role
 * \return
 */
bool BundleSystemsModel::setData(const QModelIndex &index, const QVariant &value, int role){
    return false;
}

/*!
 * \brief BundleSystemsModel::getCurrentJob
 * \return
 */
const QPointer<OiJob> &BundleSystemsModel::getCurrentJob() const{
    return this->currentJob;
}

/*!
 * \brief BundleSystemsModel::setCurrentJob
 * \param job
 */
void BundleSystemsModel::setCurrentJob(const QPointer<OiJob> &job){
    if(!job.isNull()){

        //disconnect current job
        if(!this->currentJob.isNull()){
            this->disconnectJob();
        }

        this->currentJob = job;
        this->connectJob();

        //get bundle systems
        emit this->beginResetModel();
        this->bundleSystems = this->currentJob->getBundleSystemList();
        emit this->endResetModel();
        this->updateModel();

    }
}

/*!
 * \brief BundleSystemsModel::getSelectedBundleSystem
 * \param index
 * \return
 */
int BundleSystemsModel::getSelectedBundleSystem(const QModelIndex &index){

    //check index
    if(!index.isValid()){
        return -1;
    }

    //get bundle system
    int row = index.row();
    if(this->bundleSystems.size() <= row){
        return -1;
    }
    QPointer<CoordinateSystem> system = this->bundleSystems.at(row);
    if(system.isNull()){
        return -1;
    }

    return system->getId();

}

/*!
 * \brief BundleSystemsModel::updateModel
 */
void BundleSystemsModel::updateModel(){
    emit this->layoutAboutToBeChanged();
    emit this->layoutChanged();
}

/*!
 * \brief BundleSystemsModel::connectJob
 */
void BundleSystemsModel::connectJob(){

    //check current job
    if(this->currentJob.isNull()){
        return;
    }

    QObject::connect(this->currentJob, &OiJob::featureSetChanged, this, &BundleSystemsModel::featureSetChanged, Qt::AutoConnection);

}

/*!
 * \brief BundleSystemsModel::disconnectJob
 */
void BundleSystemsModel::disconnectJob(){

    //check current job
    if(this->currentJob.isNull()){
        return;
    }

    QObject::disconnect(this->currentJob, &OiJob::featureSetChanged, this, &BundleSystemsModel::featureSetChanged);

}

/*!
 * \brief BundleSystemsModel::featureSetChanged
 */
void BundleSystemsModel::featureSetChanged(){

    //check current job
    if(this->currentJob.isNull()){
        return;
    }

    //get bundle systems
    emit this->beginResetModel();
    this->bundleSystems = this->currentJob->getBundleSystemList();
    emit this->endResetModel();
    this->updateModel();

}
