#include "bundlegeometriesmodel.h"

/*!
 * \brief BundleGeometriesModel::BundleGeometriesModel
 * \param parent
 */
BundleGeometriesModel::BundleGeometriesModel(QObject *parent) : QAbstractItemModel(parent){
    this->rootItem = new BundleGeometryItem("geometries");
}

/*!
 * \brief BundleGeometriesModel::~BundleGeometriesModel
 */
BundleGeometriesModel::~BundleGeometriesModel(){
    delete this->rootItem;
}

/*!
 * \brief BundleGeometriesModel::rowCount
 * \param parent
 * \return
 */
int BundleGeometriesModel::rowCount(const QModelIndex &parent) const{

    BundleGeometryItem *parentItem;
    if(parent.column() > 0)
        return 0;

    if(!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<BundleGeometryItem*>(parent.internalPointer());

    //check parent item
    if(!parentItem)
        return 0;

    return parentItem->childCount();

}

/*!
 * \brief BundleGeometriesModel::columnCount
 * \param parent
 * \return
 */
int BundleGeometriesModel::columnCount(const QModelIndex &parent) const{

    if(parent.isValid()){

        //get and check item
        BundleGeometryItem *item;
        item = static_cast<BundleGeometryItem*>(parent.internalPointer());
        if(!item)
            return 0;

        return item->columnCount();

    }else{
        return rootItem->columnCount();
    }

}

/*!
 * \brief BundleGeometriesModel::data
 * \param index
 * \param role
 * \return
 */
QVariant BundleGeometriesModel::data(const QModelIndex &index, int role) const{

    if(!index.isValid()){
        return QVariant();
    }

    //get and check item
    BundleGeometryItem *item = static_cast<BundleGeometryItem*>(index.internalPointer());
    if(!item)
        return QVariant();

    int columnIndex = index.column();
    if(role == Qt::DisplayRole && columnIndex == 0){
        return item->data(columnIndex);
    }

    return QVariant();

}

/*!
 * \brief BundleGeometriesModel::headerData
 * \param section
 * \param orientation
 * \param role
 * \return
 */
QVariant BundleGeometriesModel::headerData(int section, Qt::Orientation orientation, int role) const{
    if (orientation == Qt::Horizontal && section == 0 && role == Qt::DisplayRole)
        return "geometries";
    return QVariant();
}

/*!
 * \brief BundleGeometriesModel::flags
 * \param index
 * \return
 */
Qt::ItemFlags BundleGeometriesModel::flags(const QModelIndex &index) const{
    Qt::ItemFlags myFlags = QAbstractItemModel::flags(index);
    return myFlags;
}

/*!
 * \brief BundleGeometriesModel::setData
 * \param index
 * \param value
 * \param role
 * \return
 */
bool BundleGeometriesModel::setData(const QModelIndex & index, const QVariant & value, int role){
    if(!index.isValid()){
        return false;
    }
    return false;
}

/*!
 * \brief BundleGeometriesModel::index
 * \param row
 * \param column
 * \param parent
 * \return
 */
QModelIndex BundleGeometriesModel::index(int row, int column, const QModelIndex &parent) const{

    if(!hasIndex(row, column, parent)){
        return QModelIndex();
    }

    BundleGeometryItem *parentItem;
    if(!parent.isValid()){
        parentItem = this->rootItem;
    }else{
        parentItem = static_cast<BundleGeometryItem*>(parent.internalPointer());
    }

    BundleGeometryItem *childItem = parentItem->child(row);
    if(childItem){
        return createIndex(row, column, childItem);
    }else{
        return QModelIndex();
    }

}

/*!
 * \brief BundleGeometriesModel::parent
 * \param index
 * \return
 */
QModelIndex BundleGeometriesModel::parent(const QModelIndex &index) const{

    if(!index.isValid())
        return QModelIndex();

    BundleGeometryItem *childItem = static_cast<BundleGeometryItem*>(index.internalPointer());
    if(!childItem)
        return QModelIndex();

    BundleGeometryItem *parentItem = childItem->parentItem();
    if(parentItem == rootItem || !parentItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);

}

/*!
 * \brief BundleGeometriesModel::getCurrentJob
 * \return
 */
const QPointer<oi::OiJob> &BundleGeometriesModel::getCurrentJob() const{
    return this->currentJob;
}

/*!
 * \brief BundleGeometriesModel::setCurrentJob
 * \param job
 */
void BundleGeometriesModel::setCurrentJob(const QPointer<oi::OiJob> &job){

    //disconnect old job
    this->disconnectJob();

    //set up new job
    this->currentJob = job;
    this->connectJob();

    this->updateModel();

}

/*!
 * \brief BundleGeometriesModel::getStations
 * \return
 */
const QJsonArray &BundleGeometriesModel::getStations(){
    return this->usedStations;
}

/*!
 * \brief BundleGeometriesModel::setStations
 * \param stations
 */
void BundleGeometriesModel::setStations(const QJsonArray &stations){
    this->usedStations = stations;
    this->updateModel();
}

/*!
 * \brief BundleGeometriesModel::updateModel
 */
void BundleGeometriesModel::updateModel(){

    //check job
    if(this->currentJob.isNull()){
        return;
    }

    emit this->beginResetModel();

    //remove current items
    this->rootItem->removeAllChildren();

    //iterate over used stations
    for(int i = 0; i < this->usedStations.size(); i++){

        //get and check station
        QJsonObject station = this->usedStations.at(i).toObject();
        if(station.isEmpty() || !station.value("used").toBool()){
            continue;
        }

        //get station id
        int id = station.value("id").toInt();

        //get and check station feature
        QPointer<oi::FeatureWrapper> feature = this->currentJob->getFeatureById(id);
        if(feature.isNull() || feature->getStation().isNull()){
            continue;
        }
        QString name = feature->getStation()->getFeatureName();

        //create station item
        BundleGeometryItem *stationItem = new BundleGeometryItem(name);

        //get target geometries
        QList<QPointer<oi::Geometry> > geometries = feature->getStation()->getTargetGeometries();
        foreach(const QPointer<oi::Geometry> &geom, geometries){
            if(!geom.isNull() && geom->getIsCommon()){
                BundleGeometryItem *geomItem = new BundleGeometryItem(geom->getFeatureName());
                stationItem->appendChild(geomItem);
            }
        }

        //add station item
        this->rootItem->appendChild(stationItem);

    }

    emit this->endResetModel();

    //update layout
    emit this->layoutAboutToBeChanged();
    emit this->layoutChanged();

}

/*!
 * \brief BundleGeometriesModel::connectJob
 */
void BundleGeometriesModel::connectJob(){

    //check job
    if(this->currentJob.isNull()){
        return;
    }

    QObject::connect(this->currentJob, &oi::OiJob::featureSetChanged, this, &BundleGeometriesModel::updateModel, Qt::AutoConnection);
    QObject::connect(this->currentJob, &oi::OiJob::featureNameChanged, this, &BundleGeometriesModel::updateModel, Qt::AutoConnection);
    QObject::connect(this->currentJob, &oi::OiJob::geometryIsCommonChanged, this, &BundleGeometriesModel::updateModel, Qt::AutoConnection);

}

/*!
 * \brief BundleGeometriesModel::disconnectJob
 */
void BundleGeometriesModel::disconnectJob(){

    //check job
    if(this->currentJob.isNull()){
        return;
    }

    QObject::disconnect(this->currentJob, &oi::OiJob::featureSetChanged, this, &BundleGeometriesModel::updateModel);
    QObject::disconnect(this->currentJob, &oi::OiJob::featureNameChanged, this, &BundleGeometriesModel::updateModel);
    QObject::disconnect(this->currentJob, &oi::OiJob::geometryIsCommonChanged, this, &BundleGeometriesModel::updateModel);

}
