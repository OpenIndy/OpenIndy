#include "bundlestationitem.h"

/*!
 * \brief BundleStationItem::BundleStationItem
 * \param key
 * \param value
 * \param stationId
 * \param parentItem
 */
BundleStationItem::BundleStationItem(const QString &key, const bool &value, const int &stationId, BundleStationItem *parentItem){

    //set parent
    this->m_parentItem = parentItem;

    //set display values
    this->value = value;
    this->key = key;
    this->stationId = stationId;

}

/*!
 * \brief BundleStationItem::~BundleStationItem
 */
BundleStationItem::~BundleStationItem(){

    //delete all child items
    qDeleteAll(this->m_childItems);

}

/*!
 * \brief BundleStationItem::appendChild
 * \param child
 */
void BundleStationItem::appendChild(BundleStationItem *child){
    this->m_childItems.append(child);
    child->setParentItem(this);
}

/*!
 * \brief BundleStationItem::removeAllChildren
 */
void BundleStationItem::removeAllChildren(){
    qDeleteAll(this->m_childItems);
    this->m_childItems.clear();
}

/*!
 * \brief BundleStationItem::child
 * \param row
 * \return
 */
BundleStationItem *BundleStationItem::child(int row){
    if(this->m_childItems.size() > row){
        return this->m_childItems.value(row);
    }
    return NULL;
}

/*!
 * \brief BundleStationItem::childCount
 * \return
 */
int BundleStationItem::childCount() const{
    return m_childItems.count();
}

/*!
 * \brief BundleStationItem::columnCount
 * \return
 */
int BundleStationItem::columnCount() const{
    return 2;
}

/*!
 * \brief BundleStationItem::data
 * \param column
 * \return
 */
QVariant BundleStationItem::data(int column) const{
    switch(column){
    case 0:
        return this->key;
    case 1:
        return this->value;
    default:
        return QVariant();
    }
}

/*!
 * \brief BundleStationItem::row
 * \return
 */
int BundleStationItem::row() const{

    //check parent
    if(this->m_parentItem == NULL){
        return 0;
    }

    return this->m_parentItem->m_childItems.indexOf(const_cast<BundleStationItem*>(this));

}

/*!
 * \brief BundleStationItem::parentItem
 * \return
 */
BundleStationItem *BundleStationItem::parentItem(){
    return this->m_parentItem;
}

/*!
 * \brief BundleStationItem::setParentItem
 * \param parent
 */
void BundleStationItem::setParentItem(BundleStationItem *parent){
    this->m_parentItem = parent;
}

/*!
 * \brief BundleStationItem::getStationId
 * \return
 */
const int &BundleStationItem::getStationId() const{
    return this->stationId;
}

/*!
 * \brief BundleStationItem::getKey
 * \return
 */
const QString &BundleStationItem::getKey() const{
    return this->key;
}

/*!
 * \brief BundleStationItem::getValue
 * \return
 */
bool BundleStationItem::getValue() const{
    return this->value;
}

/*!
 * \brief BundleStationItem::setValue
 * \param value
 */
void BundleStationItem::setValue(const bool &value){
    this->value = value;
}
