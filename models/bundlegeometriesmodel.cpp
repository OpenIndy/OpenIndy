#include "bundlegeometriesmodel.h"

/*
SettingsModel::SettingsModel(QObject *parent) : QAbstractItemModel(parent){
    rootItem = new SettingsItem(eSettingsUndefined, "Einstellungsparameter");
}

SettingsModel::~SettingsModel(){
    delete rootItem;
}

int SettingsModel::rowCount(const QModelIndex &parent) const{

    SettingsItem *parentItem;
    if(parent.column() > 0)
        return 0;

    if(!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<SettingsItem*>(parent.internalPointer());

    //check parent item
    if(!parentItem)
        return 0;

    return parentItem->childCount();

}

int SettingsModel::columnCount(const QModelIndex &parent) const{

    if(parent.isValid()){

        //get and check item
        SettingsItem *item;
        item = static_cast<SettingsItem*>(parent.internalPointer());
        if(!item)
            return 0;

        return item->columnCount();

    }else{
        return rootItem->columnCount();
    }
}

QVariant SettingsModel::data(const QModelIndex &index, int role) const{

    if(!index.isValid()){
        return QVariant();
    }

    //get and check item
    SettingsItem *item = static_cast<SettingsItem*>(index.internalPointer());
    if(!item)
        return QVariant();

    if(role == Qt::DisplayRole){
        return item->data(index.column());
    }else if(role == Qt::BackgroundRole){

        if(item->parentItem() == rootItem){
            return QColor(QColor::fromCmykF(0.25, 0.0, 0.09, 0.29));
        }
        if(item->getKey() == eSettingsColorWarning && index.column() == 1){
            return QColor(item->getValue().toString());
        }

    }else if(role == Qt::FontRole){

        if(item->getKey() == eSettingsUndefined && index.column() == 0){
            return QFont("Times", 9, QFont::Bold);
        }

    }

    return QVariant();

}

QVariant SettingsModel::headerData(int section, Qt::Orientation orientation, int role) const{

    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->data(section);

    return QVariant();

}

Qt::ItemFlags SettingsModel::flags(const QModelIndex &index) const{
    Qt::ItemFlags myFlags = QAbstractItemModel::flags(index);
    return (myFlags | Qt::ItemIsEditable);
}

bool SettingsModel::setData(const QModelIndex & index, const QVariant & value, int role){

    if(!index.isValid()){
        return false;
    }

    //get and check item
    SettingsItem *item = static_cast<SettingsItem*>(index.internalPointer());
    if(!item)
        return false;

    // get & set item
    SettingTypes sType = item->getKey();
    item->setValue(value);

    //get value from editor for the model
    switch(sType){
    case eSettingsCloseProgramAfterMeasurement:
        return this->updateGeneralSetting(item);
    case eSettingsTakeMeasurementTaskFromProDat:
        return this->updateGeneralSetting(item);
    case eSettingsDeinitializeTrackerAfterMeasurement:
        return this->updateGeneralSetting(item);
    case eSettingsOnlySaveCompleteMeasurement:
        return this->updateGeneralSetting(item);
    case eSettingsInterpolatedPointCount:
        return this->updateGeneralSetting(item);
    case eSettingsSizeIcons:
        return this->updateVisualSetting(item);
    case eSettingsSizeFont:
        return this->updateVisualSetting(item);
    case eSettingsDefaultSize:
        return this->updateVisualSetting(item);
    case eSettingsColorWarning:
        return this->updateVisualSetting(item);
    case eSettingsArchive:
        return this->updateProDatSetting(item);
    case eSettingsPathArchiveDir:
        return this->updateProDatSetting(item);
    case eSettingsNameInputFile:
        return this->updateProDatSetting(item);
    case eSettingsPathInputDir:
        return this->updateProDatSetting(item);
    case eSettingsNameOutputFile:
        return this->updateProDatSetting(item);
    case eSettingsPathOutputDir:
        return this->updateProDatSetting(item);
    case eSettingsTrackerType:
        return this->updateSensorSetting(item);
    case eSettingsIp:
        return this->updateSensorSetting(item);
    case eSettingsProbe:
        return this->updateSensorSetting(item);
    case eSettingsScanFrequency:
        return this->updateMeasurementSetting(item);
    case eSettingsScanDelta:
        return this->updateMeasurementSetting(item);
    case eSettingsSinglePointFrequency:
        return this->updateMeasurementSetting(item);
    default:
        return false;
    }

    return true;

}

QModelIndex SettingsModel::index(int row, int column, const QModelIndex &parent) const{

    if(!hasIndex(row, column, parent)){
        return QModelIndex();
    }

    SettingsItem *parentItem;
    if(!parent.isValid()){
        parentItem = this->rootItem;
    }else{
        parentItem = static_cast<SettingsItem*>(parent.internalPointer());
    }

    SettingsItem *childItem = parentItem->child(row);
    if(childItem){
        return createIndex(row, column, childItem);
    }else{
        return QModelIndex();
    }

}

QModelIndex SettingsModel::parent(const QModelIndex &index) const{

    if(!index.isValid())
        return QModelIndex();

    SettingsItem *childItem = static_cast<SettingsItem*>(index.internalPointer());
    if(!childItem)
        return QModelIndex();

    SettingsItem *parentItem = childItem->parentItem();
    if(parentItem == rootItem || !parentItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);

}

void SettingsModel::updateModel(){
    emit this->layoutAboutToBeChanged();
    emit this->layoutChanged();
}*/
