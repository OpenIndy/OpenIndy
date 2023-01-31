#include "measurementconfigurationproxymodel.h"

/*!
 * \brief MeasurementConfigurationProxyModel::MeasurementConfigurationProxyModel
 * \param parent
 */
MeasurementConfigurationProxyModel::MeasurementConfigurationProxyModel(QObject *parent) : QSortFilterProxyModel(parent){
    this->showAll = false;
}

/*!
 * \brief MeasurementConfigurationProxyModel::setFilter
 * \param showAll
 */
void MeasurementConfigurationProxyModel::setFilter(const bool &showAll){
    this->resetFilter();
    this->showAll = showAll;
    this->invalidateFilter();
}
void MeasurementConfigurationProxyModel::setFilter(const QList<ElementTypes> neededElements, FeatureTypes typeOfFeature, QList<FeatureTypes> applicableFor) {
    this->resetFilter();
    this->neededElements = neededElements;
    this->typeOfFeature = typeOfFeature;
    this->functionIsApplicableFor = applicableFor;
    this->invalidateFilter();
}
void MeasurementConfigurationProxyModel::resetFilter() {
    this->showAll = false;

    this->neededElements.clear();
    this->typeOfFeature = FeatureTypes::eUndefinedFeature;
    this->functionIsApplicableFor.clear();
}

/*!
 * \brief MeasurementConfigurationProxyModel::headerData
 * \param section
 * \param orientation
 * \param role
 * \return
 */
QVariant MeasurementConfigurationProxyModel::headerData(int section, Qt::Orientation orientation, int role) const{
    if((Qt::DisplayRole == role) &&
            (Qt::Horizontal == orientation) &&
            (0 <= section) &&
            (section < columnCount())){

        return "measurement configurations";

    }
    return QVariant();
}

/*!
 * \brief MeasurementConfigurationProxyModel::filterAcceptsRow
 * \param source_row
 * \param source_parent
 * \return
 */
bool MeasurementConfigurationProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const{

    //get and check source model
    MeasurementConfigurationModel *sourceModel = static_cast<MeasurementConfigurationModel *>(this->sourceModel());
    if(sourceModel == NULL){
        return false;
    }

    //check if the index is a saved config
    if(source_row >= 0 && source_row < sourceModel->getMeasurementConfigurationManager()->getUserConfigs().size()){

        if(this->neededElements.isEmpty()) {
            return true;
        }

        MeasurementConfig mConfig = sourceModel->getMeasurementConfig(sourceModel->index(source_row, 0));
        for (ElementTypes elementType : this->neededElements) {
            if(mConfig.applicableFor(elementType, this->functionIsApplicableFor)) {
                return true;
            }
        }
    }

    return false;

}

/*!
 * \brief MeasurementConfigurationProxyModel::filterAcceptsColumn
 * \param source_row
 * \param source_parent
 * \return
 */
bool MeasurementConfigurationProxyModel::filterAcceptsColumn(int source_row, const QModelIndex &source_parent) const{
    return true;
}
