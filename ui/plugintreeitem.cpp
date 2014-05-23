#include "plugintreeitem.h"

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

/*!
 * \brief PluginTreeItem::getDisplayValue
 * \return
 */
QVariant PluginTreeItem::getDisplayValue(){
    return this->displayValue;
}

/*!
 * \brief PluginTreeItem::getIsPlugin
 * Returns true if this represents a plugin
 * \return
 */
bool PluginTreeItem::getIsPlugin(){
    return this->isPlugin;
}

/*!
 * \brief PluginTreeItem::getIsFunction
 * Returns true if this represents a function plugin (fit, construct, ...)
 * \return
 */
bool PluginTreeItem::getIsFunction(){
    return this->isFunction;
}

/*!
 * \brief PluginTreeItem::getIsSensorType
 * Returns true if this represents a sensor plugin (lasertracker, tachymeter, ...)
 * \return
 */
bool PluginTreeItem::getIsSensor(){
    return this->isSensor;
}

/*!
 * \brief PluginTreeItem::getPlugin
 * Returns the plugin this treeitem belongs to
 * \return
 */
Plugin PluginTreeItem::getPlugin(){
    if(!this->isPlugin && this->myParent != NULL){
        return this->myParent->getPlugin();
    }
    return this->myPlugin;
}

/*!
 * \brief PluginTreeItem::getFunction
 * \return
 */
FunctionPlugin PluginTreeItem::getFunction(){
    return this->myFunction;
}

/*!
 * \brief PluginTreeItem::getSensor
 * \return
 */
SensorPlugin PluginTreeItem::getSensor(){
    return this->mySensor;
}

/*!
 * \brief PluginTreeItem::setPlugin
 * \param myPlugin
 */
void PluginTreeItem::setPlugin(Plugin myPlugin){
    this->myPlugin = myPlugin;
    this->isPlugin = true;
    this->isFunction = false;
    this->isSensor = false;
}

/*!
 * \brief PluginTreeItem::setFunction
 * \param myFunction
 */
void PluginTreeItem::setFunction(FunctionPlugin myFunction){
    this->myFunction = myFunction;
    this->isPlugin = false;
    this->isFunction = true;
    this->isSensor = false;
}

/*!
 * \brief PluginTreeItem::setSensor
 * \param mySensor
 */
void PluginTreeItem::setSensor(SensorPlugin mySensor){
    this->mySensor = mySensor;
    this->isPlugin = false;
    this->isFunction = false;
    this->isSensor = true;
}

/*!
 * \brief PluginTreeItem::getChildCount
 * \return
 */
int PluginTreeItem::getChildCount(){
    return this->myChildren.size();
}

/*!
 * \brief PluginTreeItem::getChild
 * \param row
 * \return
 */
PluginTreeItem* PluginTreeItem::getChild(int row){
    if(row >= 0 && this->myChildren.size() > row){
        return this->myChildren.at(row);
    }
    return NULL;
}

/*!
 * \brief PluginTreeItem::hasParent
 * \return
 */
bool PluginTreeItem::hasParent(){
    if(this->myParent != NULL){
        return true;
    }
    return false;
}

/*!
 * \brief PluginTreeItem::getParent
 * \return
 */
PluginTreeItem* PluginTreeItem::getParent(){
    return this->myParent;
}

/*!
 * \brief PluginTreeItem::getIndex
 * \return
 */
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

/*!
 * \brief PluginTreeItem::deleteChildren
 */
void PluginTreeItem::deleteChildren(){
    qDeleteAll(this->myChildren);
    this->myChildren.clear();
}

/*!
 * \brief PluginTreeItem::deleteChild
 * \param child
 */
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

/*!
 * \brief PluginTreeItem::addChild
 * \param child
 */
void PluginTreeItem::addChild(PluginTreeItem *child){
    if(child != NULL){
        this->myChildren.append(child);
        child->setParent(this);
    }
}

/*!
 * \brief PluginTreeItem::setParent
 * \param parent
 */
void PluginTreeItem::setParent(PluginTreeItem *parent){
    this->myParent = parent;
}
