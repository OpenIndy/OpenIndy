#include "plugintreeitem.h"

PluginTreeItem::PluginTreeItem(QVariant val) : myParent(NULL), displayValue(val)
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
 * Returns true if this represents the name of a plugin
 * \return
 */
bool PluginTreeItem::getIsPlugin(){
    return this->isPlugin;
}

/*!
 * \brief PluginTreeItem::getIsPluginType
 * Returns true if this represents the plugin type (function, sensor)
 * \return
 */
bool PluginTreeItem::getIsPluginType(){
    return this->isPluginType;
}

/*!
 * \brief PluginTreeItem::getIsSensorType
 * Returns true if this represents the sensor type of a sensor plugin (lasertracker, tachymeter, ...)
 * \return
 */
bool PluginTreeItem::getIsSensorType(){
    return this->isSensorType;
}

/*!
 * \brief PluginTreeItem::getIsFeatureType
 * Returns true if this represents the feature type of a function plugin (point, line , plane, ...)
 * \return
 */
bool PluginTreeItem::getIsFeatureType(){
    return this->isFeatureType;
}

/*!
 * \brief PluginTreeItem::setIsFeatureType
 * \param state
 */
void PluginTreeItem::setIsFeatureType(bool state){
    this->isFeatureType = state;
    this->isPlugin = !state;
    this->isPluginType = !state;
    this->isSensorType = !state;
}

/*!
 * \brief PluginTreeItem::setIsPlugin
 * \param state
 */
void PluginTreeItem::setIsPlugin(bool state){
    this->isFeatureType = !state;
    this->isPlugin = state;
    this->isPluginType = !state;
    this->isSensorType = !state;
}

/*!
 * \brief PluginTreeItem::setIsPluginType
 * \param state
 */
void PluginTreeItem::setIsPluginType(bool state){
    this->isFeatureType = !state;
    this->isPlugin = !state;
    this->isPluginType = state;
    this->isSensorType = !state;
}

/*!
 * \brief PluginTreeItem::setIsSensorType
 * \param state
 */
void PluginTreeItem::setIsSensorType(bool state){
    this->isFeatureType = !state;
    this->isPlugin = !state;
    this->isPluginType = !state;
    this->isSensorType = state;
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
