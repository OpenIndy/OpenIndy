#include "bundlestationsmodel.h"

/*!
 * \brief BundleStationsModel::BundleStationsModel
 * \param parent
 */
BundleStationsModel::BundleStationsModel(QObject *parent) : QAbstractItemModel(parent){
    this->rootItem = new BundleStationItem("stations", false, -1);
}

/*!
 * \brief BundleStationsModel::~BundleStationsModel
 */
BundleStationsModel::~BundleStationsModel(){
    delete this->rootItem;
}

/*!
 * \brief BundleStationsModel::rowCount
 * \param parent
 * \return
 */
int BundleStationsModel::rowCount(const QModelIndex &parent) const{

    BundleStationItem *parentItem;
    if(parent.column() > 0)
        return 0;

    if(!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<BundleStationItem*>(parent.internalPointer());

    //check parent item
    if(!parentItem)
        return 0;

    return parentItem->childCount();

}

/*!
 * \brief BundleStationsModel::columnCount
 * \param parent
 * \return
 */
int BundleStationsModel::columnCount(const QModelIndex &parent) const{

    if(parent.isValid()){

        //get and check item
        BundleStationItem *item;
        item = static_cast<BundleStationItem*>(parent.internalPointer());
        if(!item)
            return 0;

        return item->columnCount();

    }else{
        return rootItem->columnCount();
    }

}

/*!
 * \brief BundleStationsModel::data
 * \param index
 * \param role
 * \return
 */
QVariant BundleStationsModel::data(const QModelIndex &index, int role) const{

    if(!index.isValid()){
        return QVariant();
    }

    //get and check item
    BundleStationItem *item = static_cast<BundleStationItem*>(index.internalPointer());
    if(!item)
        return QVariant();

    int columnIndex = index.column();
    if(role == Qt::DisplayRole && columnIndex == 0){
        return item->data(columnIndex);
    }else if(role == Qt::CheckStateRole && columnIndex == 1){
        if(item->data(columnIndex).toBool()){
            return Qt::Checked;
        }
        return Qt::Unchecked;
    }

    return QVariant();

}

/*!
 * \brief BundleStationsModel::headerData
 * \param section
 * \param orientation
 * \param role
 * \return
 */
QVariant BundleStationsModel::headerData(int section, Qt::Orientation orientation, int role) const{
    if(orientation == Qt::Horizontal && section == 0 && role == Qt::DisplayRole){
        return "stations";
    }
    return QVariant();
}


/*!
 * \brief BundleStationsModel::flags
 * Add edit-functionality to some cells of tablemodel
 * \param index
 * \return
 */
Qt::ItemFlags BundleStationsModel::flags(const QModelIndex &index) const{

    //get parent flags
    Qt::ItemFlags myFlags = QAbstractItemModel::flags(index);

    //check column index
    if(index.column() == 1){
        //return (myFlags | Qt::ItemIsEditable | Qt::ItemIsUserCheckable);
        return (myFlags | Qt::ItemIsUserCheckable);
    }

    return myFlags;

}

/*!
 * \brief BundleStationsModel::setData
 * \param index
 * \param value
 * \param role
 * \return
 */
bool BundleStationsModel::setData(const QModelIndex & index, const QVariant & value, int role){

    //check index
    if(!index.isValid()){
        return false;
    }

    //get and check item
    int rowIndex = index.row();
    int columnIndex = index.column();
    BundleStationItem *item = static_cast<BundleStationItem*>(index.internalPointer());
    if(!item){
        return false;
    }

    //update item
    if(columnIndex == 1){

        //set used state of item
        bool isUsed = value.toBool();
        if(isUsed == true && item->parentItem() != NULL && item->parentItem() != this->rootItem
                && item->parentItem()->getValue() == false){
            return false;
        }
        item->setValue(isUsed);

        //check if item is station item
        if(item->parentItem() == this->rootItem){

            //update child items according to their parent
            if(isUsed){
                QJsonArray stations = this->bundleTemplate.value("parameters").toArray();
                for(int i = 0; i < item->childCount(); i++){
                    bool useItem = false;
                    for(int j = 0; j < stations.size(); j++){
                        if(stations.at(j).toString().compare(item->child(i)->getKey()) == 0){
                            useItem = true;
                            break;
                        }
                    }
                    item->child(i)->setValue(useItem);
                }
            }else{
                for(int i = 0; i < item->childCount(); i++){
                    item->child(i)->setValue(isUsed);
                }
            }

            //update model
            emit this->layoutAboutToBeChanged();
            emit this->layoutChanged();

        }

        emit this->stationsChanged();
        return true;

    }

    return false;

}

/*!
 * \brief BundleStationsModel::index
 * \param row
 * \param column
 * \param parent
 * \return
 */
QModelIndex BundleStationsModel::index(int row, int column, const QModelIndex &parent) const{

    if(!hasIndex(row, column, parent)){
        return QModelIndex();
    }

    BundleStationItem *parentItem;
    if(!parent.isValid()){
        parentItem = this->rootItem;
    }else{
        parentItem = static_cast<BundleStationItem*>(parent.internalPointer());
    }

    BundleStationItem *childItem = parentItem->child(row);
    if(childItem){
        return createIndex(row, column, childItem);
    }else{
        return QModelIndex();
    }

}

/*!
 * \brief BundleStationsModel::parent
 * \param index
 * \return
 */
QModelIndex BundleStationsModel::parent(const QModelIndex &index) const{

    if(!index.isValid())
        return QModelIndex();

    BundleStationItem *childItem = static_cast<BundleStationItem*>(index.internalPointer());
    if(!childItem)
        return QModelIndex();

    BundleStationItem *parentItem = childItem->parentItem();
    if(parentItem == rootItem || !parentItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);

}

/*!
 * \brief BundleStationsModel::getCurrentJob
 * \return
 */
const QPointer<oi::OiJob> &BundleStationsModel::getCurrentJob() const{
    return this->currentJob;
}

/*!
 * \brief BundleStationsModel::setCurrentJob
 * \param job
 */
void BundleStationsModel::setCurrentJob(const QPointer<oi::OiJob> &job){

    //disconnect old job
    this->disconnectJob();

    //set up new job
    this->currentJob = job;
    this->connectJob();

    this->updateModel();

}

/*!
 * \brief BundleStationsModel::getStations
 * \return
 */
const QJsonArray &BundleStationsModel::getStations(){
    this->updateJson();
    return this->usedStations;
}

/*!
 * \brief BundleStationsModel::setStations
 * \param stations
 */
void BundleStationsModel::setStations(const QJsonArray &stations){
    this->usedStations = stations;
    this->updateModel();
}

/*!
 * \brief BundleStationsModel::getBundleTemplate
 * \return
 */
const QJsonObject &BundleStationsModel::getBundleTemplate(){
    return this->bundleTemplate;
}

/*!
 * \brief BundleStationsModel::setBundleTemplate
 * \param bundleTemplate
 */
void BundleStationsModel::setBundleTemplate(const QJsonObject &bundleTemplate){
    this->bundleTemplate = bundleTemplate;
}

/*!
 * \brief BundleStationsModel::updateModel
 */
void BundleStationsModel::updateModel(){

    //check job
    if(this->currentJob.isNull()){
        return;
    }

    emit this->beginResetModel();

    //remove current items
    this->rootItem->removeAllChildren();

    //add new items
    for(int i = 0; i < this->usedStations.size(); i++){

        //get and check station
        QJsonObject station = this->usedStations.at(i).toObject();
        if(station.isEmpty()){
            continue;
        }

        //get station attributes
        int id = station.value("id").toInt();
        bool used = station.value("used").toBool();
        bool tx = station.value("tx").toBool();
        bool ty = station.value("ty").toBool();
        bool tz = station.value("tz").toBool();
        bool rx = station.value("rx").toBool();
        bool ry = station.value("ry").toBool();
        bool rz = station.value("rz").toBool();
        bool m = station.value("m").toBool();

        //get station name
        QPointer<oi::FeatureWrapper> feature = this->currentJob->getFeatureById(id);
        if(feature.isNull() || feature->getStation().isNull()){
            continue;
        }
        QString name = feature->getStation()->getFeatureName();

        //create station items
        BundleStationItem *stationItem = new BundleStationItem(name, used, id);
        BundleStationItem *stationTx = new BundleStationItem("tx", tx, id);
        BundleStationItem *stationTy = new BundleStationItem("ty", ty, id);
        BundleStationItem *stationTz = new BundleStationItem("tz", tz, id);
        BundleStationItem *stationRx = new BundleStationItem("rx", rx, id);
        BundleStationItem *stationRy = new BundleStationItem("ry", ry, id);
        BundleStationItem *stationRz = new BundleStationItem("rz", rz, id);
        BundleStationItem *stationM = new BundleStationItem("m", m, id);

        //add station items
        stationItem->appendChild(stationTx);
        stationItem->appendChild(stationTy);
        stationItem->appendChild(stationTz);
        stationItem->appendChild(stationRx);
        stationItem->appendChild(stationRy);
        stationItem->appendChild(stationRz);
        stationItem->appendChild(stationM);
        this->rootItem->appendChild(stationItem);

    }

    //add remaining stations
    QList<QPointer<oi::Station> > stations = this->currentJob->getStationsList();
    foreach(const QPointer<oi::Station> &station, stations){

        //check station
        if(station.isNull()){
            continue;
        }

        //get and check id
        int id = station->getId();
        for(int i = 0; i < this->rootItem->childCount(); i++){
            if(id == this->rootItem->child(i)->getStationId()){
                id = -1;
                break;
            }
        }
        if(id == -1){
            continue;
        }
        QString name = station->getFeatureName();

        //create station items
        BundleStationItem *stationItem = new BundleStationItem(name, false, id);
        BundleStationItem *stationTx = new BundleStationItem("tx", false, id);
        BundleStationItem *stationTy = new BundleStationItem("ty", false, id);
        BundleStationItem *stationTz = new BundleStationItem("tz", false, id);
        BundleStationItem *stationRx = new BundleStationItem("rx", false, id);
        BundleStationItem *stationRy = new BundleStationItem("ry", false, id);
        BundleStationItem *stationRz = new BundleStationItem("rz", false, id);
        BundleStationItem *stationM = new BundleStationItem("m", false, id);

        //add station items
        stationItem->appendChild(stationTx);
        stationItem->appendChild(stationTy);
        stationItem->appendChild(stationTz);
        stationItem->appendChild(stationRx);
        stationItem->appendChild(stationRy);
        stationItem->appendChild(stationRz);
        stationItem->appendChild(stationM);
        this->rootItem->appendChild(stationItem);

    }

    emit this->endResetModel();

    //update layout
    emit this->layoutAboutToBeChanged();
    emit this->layoutChanged();

    emit this->stationsChanged();

}

/*!
 * \brief BundleStationsModel::connectJob
 */
void BundleStationsModel::connectJob(){

    //check job
    if(this->currentJob.isNull()){
        return;
    }

    QObject::connect(this->currentJob, &oi::OiJob::stationSetChanged, this, &BundleStationsModel::updateModel, Qt::AutoConnection);
    QObject::connect(this->currentJob, &oi::OiJob::featureAttributesChanged, this, &BundleStationsModel::updateModel, Qt::AutoConnection);

}

/*!
 * \brief BundleStationsModel::disconnectJob
 */
void BundleStationsModel::disconnectJob(){

    //check job
    if(this->currentJob.isNull()){
        return;
    }

    QObject::disconnect(this->currentJob, &oi::OiJob::stationSetChanged, this, &BundleStationsModel::updateModel);
    QObject::disconnect(this->currentJob, &oi::OiJob::featureAttributesChanged, this, &BundleStationsModel::updateModel);

}

/*!
 * \brief BundleStationsModel::updateJson
 * Updates the json stations
 */
void BundleStationsModel::updateJson(){

    //check root item
    if(!this->rootItem){
        return;
    }

    //clear old json array
    this->usedStations = QJsonArray();

    //iterate over all stations
    for(int i = 0; i < this->rootItem->childCount(); i++){

        //get and check station item
        BundleStationItem *station = this->rootItem->child(i);
        if(!station){
            continue;
        }

        //create json object
        QJsonObject inputStation;
        inputStation.insert("id", station->getStationId());
        inputStation.insert("used", station->getValue());
        for(int j = 0; j < station->childCount(); j++){
            BundleStationItem *item = station->child(j);
            if(item != NULL){
                inputStation.insert(item->getKey(), item->getValue());
            }
        }

        //add input station
        this->usedStations.append(inputStation);

    }

}
