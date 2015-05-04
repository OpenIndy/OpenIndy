#include "functiontreeitem.h"

/*!
 * \brief FunctionTreeItem::FunctionTreeItem
 * \param display
 * \param parent
 */
FunctionTreeItem::FunctionTreeItem(const QVariant &display, QObject *parent) : QObject(parent){

    //set general tree item attributes
    this->childCount = 0;
    this->index = -1;

    //set display value
    this->displayValue = display;

}

/*!
 * \brief FunctionTreeItem::~FunctionTreeItem
 * Deletes this item and all its children
 */
FunctionTreeItem::~FunctionTreeItem(){
    this->deleteChildren();
}

/*!
 * \brief FunctionTreeItem::getChildCount
 * \return
 */
int FunctionTreeItem::getChildCount() const{
    return this->childCount;
}

/*!
 * \brief FunctionTreeItem::appendChild
 * \param child
 */
void FunctionTreeItem::appendChild(const QPointer<FunctionTreeItem> &child){

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
 * \brief FunctionTreeItem::getChild
 * \param index
 * \return
 */
QPointer<FunctionTreeItem> FunctionTreeItem::getChild(const int &index){
    if(this->childCount > index){
        return this->children.at(index);
    }
    return QPointer<FunctionTreeItem>(NULL);
}

/*!
 * \brief FunctionTreeItem::deleteChildren
 */
void FunctionTreeItem::deleteChildren(){
    qDeleteAll(this->children);
    this->children.clear();
    this->childCount = 0;
}

/*!
 * \brief FunctionTreeItem::getIndex
 * \return
 */
const int &FunctionTreeItem::getIndex() const{
    return this->index;
}

/*!
 * \brief FunctionTreeItem::hasParent
 * \return
 */
bool FunctionTreeItem::hasParent() const{
    if(this->parent.isNull()){
        return false;
    }
    return true;
}

/*!
 * \brief FunctionTreeItem::getParent
 * \return
 */
const QPointer<FunctionTreeItem> &FunctionTreeItem::getParent() const{
    return this->parent;
}

/*!
 * \brief FunctionTreeItem::getDisplayValue
 * \return
 */
const QVariant &FunctionTreeItem::getDisplayValue() const{
    return this->displayValue;
}
