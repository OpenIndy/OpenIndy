#include "plugintreeitem.h"
/*

PluginTreeItem::PluginTreeItem(QVariant val) : myParent(NULL), displayValue(val), isPlugin(false), isFunction(false),
    isSensor(false)
{
}

PluginTreeItem::~PluginTreeItem(){
    if(this->myParent != NULL){
        this->myParent->deleteChild(this);
    }
    this->deleteChildren();
}

QVariant PluginTreeItem::getDisplayValue(){
    return this->displayValue;
}

bool PluginTreeItem::getIsPlugin(){
    return this->isPlugin;
}

bool PluginTreeItem::getIsFunction(){
    return this->isFunction;
}

bool PluginTreeItem::getIsSensor(){
    return this->isSensor;
}

Plugin PluginTreeItem::getPlugin(){
    if(!this->isPlugin && this->myParent != NULL){
        return this->myParent->getPlugin();
    }
    return this->myPlugin;
}

FunctionPlugin PluginTreeItem::getFunction(){
    return this->myFunction;
}

SensorPlugin PluginTreeItem::getSensor(){
    return this->mySensor;
}

void PluginTreeItem::setPlugin(Plugin myPlugin){
    this->myPlugin = myPlugin;
    this->isPlugin = true;
    this->isFunction = false;
    this->isSensor = false;
}

void PluginTreeItem::setFunction(FunctionPlugin myFunction){
    this->myFunction = myFunction;
    this->isPlugin = false;
    this->isFunction = true;
    this->isSensor = false;
}

void PluginTreeItem::setSensor(SensorPlugin mySensor){
    this->mySensor = mySensor;
    this->isPlugin = false;
    this->isFunction = false;
    this->isSensor = true;
}

int PluginTreeItem::getChildCount(){
    return this->myChildren.size();
}

PluginTreeItem* PluginTreeItem::getChild(int row){
    if(row >= 0 && this->myChildren.size() > row){
        return this->myChildren.at(row);
    }
    return NULL;
}

bool PluginTreeItem::hasParent(){
    if(this->myParent != NULL){
        return true;
    }
    return false;
}

PluginTreeItem* PluginTreeItem::getParent(){
    return this->myParent;
}

int PluginTreeItem::getIndex(){
    if(this->myParent != NULL){
        for(int i = 0; i < this->myParent->myChildren.size(); i++){
            if(this->myParent->myChildren.at(i) == this){
                return i;
            }
        }
    }
    return 0;
}

void PluginTreeItem::deleteChildren(){
    qDeleteAll(this->myChildren);
    this->myChildren.clear();
}

void PluginTreeItem::deleteChild(PluginTreeItem *child){
    int index = -1;
    for(int i = 0; i < this->myChildren.size(); i++){
        if(this->myChildren.at(i) == child){
            index = i;
        }
    }
    if(index > -1){
        this->myChildren.removeAt(index);
    }
}

void PluginTreeItem::addChild(PluginTreeItem *child){
    if(child != NULL){
        this->myChildren.append(child);
        child->setParent(this);
    }
}

void PluginTreeItem::setParent(PluginTreeItem *parent){
    this->myParent = parent;
}
*/

/*!
 * \brief PluginTreeItem::PluginTreeItem
 * \param display
 * \param parent
 */
PluginTreeItem::PluginTreeItem(const QVariant &display, QObject *parent) : QObject(parent){

    //set general tree item attributes
    this->childCount = 0;
    this->index = -1;

    //define what kind of tree item this item is
    this->setIsHeader();

    //set display value
    this->displayValue = display;

}

/*!
 * \brief PluginTreeItem::PluginTreeItem
 * \param plugin
 * \param parent
 */
PluginTreeItem::PluginTreeItem(const sdb::Plugin &plugin, QObject *parent) : QObject(parent){

    //set general tree item attributes
    this->childCount = 0;
    this->index = -1;

    //define what kind of tree item this item is
    this->setIsPlugin();

    //set plugin
    this->plugin = plugin;

    //set display value
    this->displayValue = plugin.name;

    //set up item and its children
    this->setUpPlugin();

}

/*!
 * \brief PluginTreeItem::PluginTreeItem
 * \param function
 * \param parent
 */
PluginTreeItem::PluginTreeItem(const sdb::Function &function, QObject *parent) : QObject(parent){

    //set general tree item attributes
    this->childCount = 0;
    this->index = -1;

    //define what kind of tree item this item is
    this->setIsFunction();

    //set function
    this->function = function;

    //set display value
    this->displayValue = function.name;

}

/*!
 * \brief PluginTreeItem::PluginTreeItem
 * \param sensor
 * \param parent
 */
PluginTreeItem::PluginTreeItem(const sdb::Sensor &sensor, QObject *parent) : QObject(parent){

    //set general tree item attributes
    this->childCount = 0;
    this->index = -1;

    //define what kind of tree item this item is
    this->setIsSensor();

    //set sensor
    this->sensor = sensor;

    //set display value
    this->displayValue = sensor.name;

}

/*!
 * \brief PluginTreeItem::PluginTreeItem
 * \param networkAdjustment
 * \param parent
 */
PluginTreeItem::PluginTreeItem(const sdb::NetworkAdjustment &networkAdjustment, QObject *parent) : QObject(parent){

    //set general tree item attributes
    this->childCount = 0;
    this->index = -1;

    //define what kind of tree item this item is
    this->setIsNetworkAdjustment();

    //set networkAdjustment
    this->networkAdjustment = networkAdjustment;

    //set display value
    this->displayValue = networkAdjustment.name;

}

/*!
 * \brief PluginTreeItem::PluginTreeItem
 * \param tool
 * \param parent
 */
PluginTreeItem::PluginTreeItem(const sdb::Tool &tool, QObject *parent) : QObject(parent){

    //set general tree item attributes
    this->childCount = 0;
    this->index = -1;

    //define what kind of tree item this item is
    this->setIsTool();

    //set tool
    this->tool = tool;

    //set display value
    this->displayValue = tool.name;

}

/*!
 * \brief PluginTreeItem::PluginTreeItem
 * \param simulation
 * \param parent
 */
PluginTreeItem::PluginTreeItem(const sdb::Simulation &simulation, QObject *parent) : QObject(parent){

    //set general tree item attributes
    this->childCount = 0;
    this->index = -1;

    //define what kind of tree item this item is
    this->setIsSimulation();

    //set simulation
    this->simulation = simulation;

    //set display value
    this->displayValue = simulation.name;

}

/*!
 * \brief PluginTreeItem::PluginTreeItem
 * \param exchange
 * \param parent
 */
PluginTreeItem::PluginTreeItem(const sdb::Exchange &exchange, QObject *parent) : QObject(parent){

    //set general tree item attributes
    this->childCount = 0;
    this->index = -1;

    //define what kind of tree item this item is
    this->setIsExchange();

    //set exchange
    this->exchange = exchange;

    //set display value
    this->displayValue = exchange.name;

}

/*!
 * \brief PluginTreeItem::~PluginTreeItem
 * Deletes this item and all its children
 */
PluginTreeItem::~PluginTreeItem(){
    this->deleteChildren();
}

/*!
 * \brief PluginTreeItem::getDisplayValue
 * \return
 */
const QVariant &PluginTreeItem::getDisplayValue() const{
    return this->displayValue;
}

/*!
 * \brief PluginTreeItem::getChildCount
 * \return
 */
int PluginTreeItem::getChildCount() const{
    return this->childCount;
}

/*!
 * \brief PluginTreeItem::appendChild
 * \param child
 */
void PluginTreeItem::appendChild(const QPointer<PluginTreeItem> &child){

    //check child
    if(child.isNull()){
        return;
    }

    //set index and parent for child
    child->index = this->children.size();
    child->parent = this;

    //add the child
    this->children.append(child);
    this->childCount++;

}

/*!
 * \brief PluginTreeItem::getChild
 * \param index
 * \return
 */
QPointer<PluginTreeItem> PluginTreeItem::getChild(const int &index){
    if(this->childCount > index){
        return this->children.at(index);
    }
    return QPointer<PluginTreeItem>(NULL);
}

/*!
 * \brief PluginTreeItem::deleteChildren
 */
void PluginTreeItem::deleteChildren(){
    qDeleteAll(this->children);
    this->children.clear();
    this->childCount = 0;
}

/*!
 * \brief PluginTreeItem::hasParent
 * \return
 */
bool PluginTreeItem::hasParent() const{
    if(this->parent.isNull()){
        return false;
    }
    return true;
}

/*!
 * \brief PluginTreeItem::getParent
 * \return
 */
const QPointer<PluginTreeItem> &PluginTreeItem::getParent() const{
    return this->parent;
}

/*!
 * \brief PluginTreeItem::getIndex
 * \return
 */
const int &PluginTreeItem::getIndex() const{
    return this->index;
}

/*!
 * \brief PluginTreeItem::getIsHeader
 * \return
 */
const bool &PluginTreeItem::getIsHeader() const{
    return this->isHeader;
}

/*!
 * \brief PluginTreeItem::getIsPlugin
 * \return
 */
const bool &PluginTreeItem::getIsPlugin() const{
    return this->isPlugin;
}

/*!
 * \brief PluginTreeItem::getIsFunction
 * \return
 */
const bool &PluginTreeItem::getIsFunction() const{
    return this->isFunction;
}

/*!
 * \brief PluginTreeItem::getIsSensor
 * \return
 */
const bool &PluginTreeItem::getIsSensor() const{
    return this->isSensor;
}

/*!
 * \brief PluginTreeItem::getIsNetworkAdjustment
 * \return
 */
const bool &PluginTreeItem::getIsNetworkAdjustment() const{
    return this->isNetworkAdjustment;
}

/*!
 * \brief PluginTreeItem::getIsTool
 * \return
 */
const bool &PluginTreeItem::getIsTool() const{
    return this->isTool;
}

/*!
 * \brief PluginTreeItem::getIsSimulation
 * \return
 */
const bool &PluginTreeItem::getIsSimulation() const{
    return this->isSimulation;
}

/*!
 * \brief PluginTreeItem::getIsExchange
 * \return
 */
const bool &PluginTreeItem::getIsExchange() const{
    return this->isExchange;
}

/*!
 * \brief PluginTreeItem::getPlugin
 * \return
 */
const sdb::Plugin &PluginTreeItem::getPlugin() const{
    return this->plugin;
}

/*!
 * \brief PluginTreeItem::getFunction
 * \return
 */
const sdb::Function &PluginTreeItem::getFunction() const{
    return this->function;
}

/*!
 * \brief PluginTreeItem::getSensor
 * \return
 */
const sdb::Sensor &PluginTreeItem::getSensor() const{
    return this->sensor;
}

/*!
 * \brief PluginTreeItem::getNetworkAdjustment
 * \return
 */
const sdb::NetworkAdjustment &PluginTreeItem::getNetworkAdjustment() const{
    return this->networkAdjustment;
}

/*!
 * \brief PluginTreeItem::getTool
 * \return
 */
const sdb::Tool &PluginTreeItem::getTool() const{
    return this->tool;
}

/*!
 * \brief PluginTreeItem::getSimulation
 * \return
 */
const sdb::Simulation &PluginTreeItem::getSimulation() const{
    return this->simulation;
}

/*!
 * \brief PluginTreeItem::getExchange
 * \return
 */
const sdb::Exchange &PluginTreeItem::getExchange() const{
    return this->exchange;
}

/*!
 * \brief PluginTreeItem::setIsHeader
 */
void PluginTreeItem::setIsHeader(){
    this->isHeader = true;
    this->isPlugin = false;
    this->isFunction = false;
    this->isSensor = false;
    this->isNetworkAdjustment = false;
    this->isTool = false;
    this->isSimulation = false;
    this->isExchange = false;
}

/*!
 * \brief PluginTreeItem::setIsPlugin
 */
void PluginTreeItem::setIsPlugin(){
    this->isHeader = false;
    this->isPlugin = true;
    this->isFunction = false;
    this->isSensor = false;
    this->isNetworkAdjustment = false;
    this->isTool = false;
    this->isSimulation = false;
    this->isExchange = false;
}

/*!
 * \brief PluginTreeItem::setIsFunction
 */
void PluginTreeItem::setIsFunction(){
    this->isHeader = false;
    this->isPlugin = false;
    this->isFunction = true;
    this->isSensor = false;
    this->isNetworkAdjustment = false;
    this->isTool = false;
    this->isSimulation = false;
    this->isExchange = false;
}

/*!
 * \brief PluginTreeItem::setIsSensor
 */
void PluginTreeItem::setIsSensor(){
    this->isHeader = false;
    this->isPlugin = false;
    this->isFunction = false;
    this->isSensor = true;
    this->isNetworkAdjustment = false;
    this->isTool = false;
    this->isSimulation = false;
    this->isExchange = false;
}

/*!
 * \brief PluginTreeItem::setIsNetworkAdjustment
 */
void PluginTreeItem::setIsNetworkAdjustment(){
    this->isHeader = false;
    this->isPlugin = false;
    this->isFunction = false;
    this->isSensor = false;
    this->isNetworkAdjustment = true;
    this->isTool = false;
    this->isSimulation = false;
    this->isExchange = false;
}

/*!
 * \brief PluginTreeItem::setIsTool
 */
void PluginTreeItem::setIsTool(){
    this->isHeader = false;
    this->isPlugin = false;
    this->isFunction = false;
    this->isSensor = false;
    this->isNetworkAdjustment = false;
    this->isTool = true;
    this->isSimulation = false;
    this->isExchange = false;
}

/*!
 * \brief PluginTreeItem::setIsSimulation
 */
void PluginTreeItem::setIsSimulation(){
    this->isHeader = false;
    this->isPlugin = false;
    this->isFunction = false;
    this->isSensor = false;
    this->isNetworkAdjustment = false;
    this->isTool = false;
    this->isSimulation = true;
    this->isExchange = false;
}

/*!
 * \brief PluginTreeItem::setIsExchange
 */
void PluginTreeItem::setIsExchange(){
    this->isHeader = false;
    this->isPlugin = false;
    this->isFunction = false;
    this->isSensor = false;
    this->isNetworkAdjustment = false;
    this->isTool = false;
    this->isSimulation = false;
    this->isExchange = true;
}

/*!
 * \brief PluginTreeItem::setUpPlugin
 */
void PluginTreeItem::setUpPlugin(){

    //set up sensors
    PluginTreeItem *sensors = new PluginTreeItem("sensors:");
    PluginTreeItem *laserTracker = new PluginTreeItem("laser tracker:");
    PluginTreeItem *totalStation = new PluginTreeItem("total stations:");
    PluginTreeItem *measurementArm = new PluginTreeItem("measurement arms:");
    foreach(const sdb::Sensor &sensor, plugin.sensors){
        PluginTreeItem *sensorItem = new PluginTreeItem(sensor);
        if(sensor.iid.compare(OiMetaData::iid_LaserTracker) == 0){
            laserTracker->appendChild(sensorItem);
        }else if(sensor.iid.compare(OiMetaData::iid_TotalStation) == 0){
            totalStation->appendChild(sensorItem);
        }else if(sensor.iid.compare(OiMetaData::iid_MeasurementArm) == 0){
            measurementArm->appendChild(sensorItem);
        }else{
            delete sensorItem;
            continue;
        }
    }
    sensors->appendChild(laserTracker);
    sensors->appendChild(totalStation);
    sensors->appendChild(measurementArm);

    //set up functions
    PluginTreeItem *functions = new PluginTreeItem("functions:");
    PluginTreeItem *fitFunctions = new PluginTreeItem("fit functions:");
    PluginTreeItem *constructFunctions = new PluginTreeItem("construct functions:");
    PluginTreeItem *specialFunctions = new PluginTreeItem("special functions:");
    PluginTreeItem *geodeticFunctions = new PluginTreeItem("geodetic functions:");
    PluginTreeItem *objectTransformations = new PluginTreeItem("object transformations:");
    PluginTreeItem *systemTransformations = new PluginTreeItem("system transformations:");
    foreach(const sdb::Function &function, plugin.functions){
        PluginTreeItem *functionItem = new PluginTreeItem(function);
        if(function.iid.compare(OiMetaData::iid_FitFunction) == 0){
            fitFunctions->appendChild(functionItem);
        }else if(function.iid.compare(OiMetaData::iid_ConstructFunction) == 0){
            constructFunctions->appendChild(functionItem);
        }else if(function.iid.compare(OiMetaData::iid_SpecialFunction) == 0){
            specialFunctions->appendChild(functionItem);
        }else if(function.iid.compare(OiMetaData::iid_GeodeticFunction) == 0){
            geodeticFunctions->appendChild(functionItem);
        }else if(function.iid.compare(OiMetaData::iid_ObjectTransformation) == 0){
            objectTransformations->appendChild(functionItem);
        }else if(function.iid.compare(OiMetaData::iid_SystemTransformation) == 0){
            systemTransformations->appendChild(functionItem);
        }else{
            delete functionItem;
            continue;
        }
    }
    functions->appendChild(fitFunctions);
    functions->appendChild(constructFunctions);
    functions->appendChild(geodeticFunctions);
    functions->appendChild(objectTransformations);
    functions->appendChild(systemTransformations);
    if(specialFunctions->getChildCount() > 0) {
        functions->appendChild(specialFunctions);
    }

    //set up tools
    PluginTreeItem *tools = new PluginTreeItem("tools:");
    foreach(const sdb::Tool &tool, plugin.tools){
        PluginTreeItem *toolItem = new PluginTreeItem(tool);
        tools->appendChild(toolItem);
    }

    //set up simulations
    PluginTreeItem *simulations = new PluginTreeItem("simulations:");
    foreach(const sdb::Simulation &simulation, plugin.simulations){
        PluginTreeItem *simulationItem = new PluginTreeItem(simulation);
        simulations->appendChild(simulationItem);
    }

    //set up exchanges
    PluginTreeItem *exchanges = new PluginTreeItem("exchanges:");
    PluginTreeItem *simpleAscii = new PluginTreeItem("simple ascii exchanges:");
    PluginTreeItem *definedFormat = new PluginTreeItem("defined format exchanges:");
    foreach(const sdb::Exchange &exchange, plugin.exchanges){
        PluginTreeItem *exchangeItem = new PluginTreeItem(exchange);
        if(exchange.iid.compare(OiMetaData::iid_ExchangeSimpleAscii) == 0){
            simpleAscii->appendChild(exchangeItem);
        }else if(exchange.iid.compare(OiMetaData::iid_ExchangeDefinedFormat) == 0){
            definedFormat->appendChild(exchangeItem);
        }else{
            delete exchangeItem;
            continue;
        }
    }
    exchanges->appendChild(simpleAscii);
    exchanges->appendChild(definedFormat);

    //set up network adjustments
    PluginTreeItem *networkAdjustments = new PluginTreeItem("network adjustments:");
    foreach(const sdb::NetworkAdjustment &networkAdjustment, plugin.networkAdjustments){
        PluginTreeItem *networkAdjustmentItem = new PluginTreeItem(networkAdjustment);
        networkAdjustments->appendChild(networkAdjustmentItem);
    }

    //add all children
    this->appendChild(sensors);
    this->appendChild(functions);
    this->appendChild(tools);
    this->appendChild(simulations);
    this->appendChild(exchanges);
    this->appendChild(networkAdjustments);

}
