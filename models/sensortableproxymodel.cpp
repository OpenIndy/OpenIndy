#include "sensortableproxymodel.h"

/*!
 * \brief SensorTableProxyModel::SensorTableProxyModel
 * \param parent
 */
SensorTableProxyModel::SensorTableProxyModel(QObject *parent) : QSortFilterProxyModel(parent)
{
    this->sensorType = eUndefinedSensor;
}

/*!
 * \brief SensorTableProxyModel::getSensorType
 * \return
 */
const SensorTypes &SensorTableProxyModel::getSensorType() const{
    return this->sensorType;
}

/*!
 * \brief SensorTableProxyModel::setSensorType
 * \param type
 */
void SensorTableProxyModel::setSensorType(const SensorTypes &type){

    //check sensor type
    if(type != eLaserTracker && type != eTotalStation){
        return;
    }

    this->sensorType = type;
    this->invalidateFilter();

}

/*!
 * \brief SensorTableProxyModel::filterAcceptsRow
 * \param source_row
 * \param source_parent
 * \return
 */
bool SensorTableProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const{

    //get and cast source model
    SensorTableModel *source_model = dynamic_cast<SensorTableModel *>(this->sourceModel());
    if(source_model == NULL){
        return false;
    }

    //get and check source index
    QModelIndex sourceIndex;
    if(!source_parent.isValid()){
        sourceIndex = source_model->index(source_row, 0);
    }

    //check the sensor type of the index
    if(source_model->getSensorType(sourceIndex) == this->sensorType){
        return true;
    }

    return false;

}

/*!
 * \brief SensorTableProxyModel::filterAcceptsColumn
 * \param source_column
 * \param source_parent
 * \return
 */
bool SensorTableProxyModel::filterAcceptsColumn(int source_column, const QModelIndex &source_parent) const{
    return true;
}
