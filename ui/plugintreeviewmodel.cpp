#include "plugintreeviewmodel.h"

PluginTreeViewModel::PluginTreeViewModel(QObject *parent)
{
    this->rootItem = new PluginTreeItem("available plugins");
}

/*!
 * \brief PluginTreeViewModel::refreshModel
 * Refresh the model by querying the database for available plugins
 */
void PluginTreeViewModel::refreshModel(){
    //delete old tree view items
    this->rootItem->deleteChildren();

    //query database for available plugins
    QList<Plugin> myPlugins = SystemDbManager::getAvailablePlugins();

    //create tree view hierarchy
    foreach(Plugin myPlugin, myPlugins){
        PluginTreeItem *plugin = new PluginTreeItem(myPlugin.name);
        plugin->setIsPlugin(true);

        //function hierarchy
        PluginTreeItem *functionHeader = new PluginTreeItem("functions:");
        PluginTreeItem *fit = new PluginTreeItem("fit functions:");
        PluginTreeItem *construct = new PluginTreeItem("construct functions:");
        PluginTreeItem *objectTrafo = new PluginTreeItem("object transformations:");
        PluginTreeItem *systemTrafo = new PluginTreeItem("system transformations:");
        PluginTreeItem *geodetic = new PluginTreeItem("geodetic functions:");
        foreach(FunctionPlugin myFunction, myPlugin.myFunctions){
            PluginTreeItem *function = new PluginTreeItem(myFunction.name);
            if(myFunction.iid.compare(FitFunction_iidd) == 0){ fit->addChild(function); }
            else if(myFunction.iid.compare(ConstructFunction_iidd) == 0){ construct->addChild(function); }
            else if(myFunction.iid.compare(ObjectTransformation_iidd) == 0){ objectTrafo->addChild(function); }
            else if(myFunction.iid.compare(SystemTransformation_iidd) == 0){ systemTrafo->addChild(function); }
            else if(myFunction.iid.compare(GeodeticFunction_iidd) == 0){ geodetic->addChild(function); }
        }
        functionHeader->addChild(fit);
        functionHeader->addChild(construct);
        functionHeader->addChild(objectTrafo);
        functionHeader->addChild(systemTrafo);
        functionHeader->addChild(geodetic);
        functionHeader->setIsPluginType(true);

        //sensor hierarchy
        PluginTreeItem *sensorHeader = new PluginTreeItem("sensors:");
        PluginTreeItem *laserTracker = new PluginTreeItem("lasertrackers:");
        PluginTreeItem *totalStation = new PluginTreeItem("totalstations:");
        PluginTreeItem *customSensor = new PluginTreeItem("custom sensors:");
        foreach(SensorPlugin mySensor, myPlugin.mySensors){
            PluginTreeItem *sensor = new PluginTreeItem(mySensor.name);
            if(mySensor.iid.compare(LaserTracker_iidd) == 0){ laserTracker->addChild(sensor); }
            else if(mySensor.iid.compare(TotalStation_iidd) == 0){ totalStation->addChild(sensor); }
            else if(mySensor.iid.compare(Sensor_iidd) == 0){ customSensor->addChild(sensor); }
        }
        sensorHeader->addChild(laserTracker);
        sensorHeader->addChild(totalStation);
        sensorHeader->addChild(customSensor);
        sensorHeader->setIsPluginType(true);

        plugin->addChild(functionHeader);
        plugin->addChild(sensorHeader);

        this->rootItem->addChild(plugin);
    }

    //update view
    emit this->beginResetModel();
    emit this->endResetModel();

    emit this->layoutAboutToBeChanged();
    emit this->layoutChanged();
}

/*!
 * \brief PluginTreeViewModel::index
 * \param row
 * \param column
 * \param parent
 * \return
 */
QModelIndex PluginTreeViewModel::index(int row, int column, const QModelIndex &parent) const{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    PluginTreeItem *parentItem = NULL;

    if (!parent.isValid())
        parentItem = this->rootItem;
    else
        parentItem = static_cast<PluginTreeItem*>(parent.internalPointer());

    if(parentItem != NULL){
        PluginTreeItem *childItem = parentItem->getChild(row);
        if (childItem != NULL)
            return createIndex(row, column, childItem);
        else
            return QModelIndex();
    }else{
        return QModelIndex();
    }
}

/*!
 * \brief PluginTreeViewModel::parent
 * \param index
 * \return
 */
QModelIndex PluginTreeViewModel::parent(const QModelIndex &index) const{
    if (!index.isValid())
        return QModelIndex();

    PluginTreeItem *childItem = static_cast<PluginTreeItem*>(index.internalPointer());


    if(childItem != NULL){
        PluginTreeItem *parentItem = childItem->getParent();

        if (parentItem == NULL || parentItem == this->rootItem)
            return QModelIndex();

        return createIndex(parentItem->getIndex(), 0, parentItem);
    }else{
        return QModelIndex();
    }
}

/*!
 * \brief PluginTreeViewModel::rowCount
 * \param parent
 * \return
 */
int PluginTreeViewModel::rowCount(const QModelIndex &parent) const{
    PluginTreeItem *childItem = NULL;

    if(!parent.isValid()){
        childItem = this->rootItem;
    }else{
        childItem = static_cast<PluginTreeItem*>(parent.internalPointer());
    }
    if(childItem != NULL){
        return childItem->getChildCount();
    }

    return 0;
}

/*!
 * \brief PluginTreeViewModel::columnCount
 * \param parent
 * \return
 */
int PluginTreeViewModel::columnCount(const QModelIndex &parent) const{
    return 1;
}

/*!
 * \brief PluginTreeViewModel::data
 * \param index
 * \param role
 * \return
 */
QVariant PluginTreeViewModel::data(const QModelIndex &index, int role) const{
    if (!index.isValid())
        return QVariant();

    PluginTreeItem *item = static_cast<PluginTreeItem*>(index.internalPointer());

    if(item != NULL){
        if(role == Qt::DisplayRole){ //return display text for tree view item
            return item->getDisplayValue();
        }else if(role == Qt::DecorationRole){ //return icon for tree view item
            if(item->getIsPlugin()){
                QPixmap pix(":/Images/icons/loadPlugin.png");
                return pix.scaledToHeight(20, Qt::SmoothTransformation);
            }
        }else if(role == Qt::FontRole){
            if(item->getIsPluginType()){
                QFont boldFont;
                boldFont.setBold(true);
                return boldFont;
            }
            return QFont();
        }
    }

    return QVariant();
}

/*!
 * \brief PluginTreeViewModel::headerData
 * \param section
 * \param orientation
 * \param role
 * \return
 */
QVariant PluginTreeViewModel::headerData(int section, Qt::Orientation orientation, int role) const{
    if(section == 0 && role == Qt::DisplayRole){
        return QVariant("available plugins");
    }
    return QVariant();
}
