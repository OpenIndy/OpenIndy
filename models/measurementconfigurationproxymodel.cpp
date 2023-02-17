#include "measurementconfigurationproxymodel.h"

/*!
 * \brief MeasurementConfigurationProxyModel::MeasurementConfigurationProxyModel
 * \param parent
 */
MeasurementConfigurationProxyModel::MeasurementConfigurationProxyModel(QObject *parent) : QSortFilterProxyModel(parent), filterType(eNo_MeasurementConfigurationFilter){

}

/*!
 * \brief MeasurementConfigurationProxyModel::setFilter
 * \param showAll
 */
void MeasurementConfigurationProxyModel::setFilter(const bool &showAll){
    this->resetFilter();

    this->filterType = showAll ? eAll_MeasurementConfigurationFilter : eNo_MeasurementConfigurationFilter;

    this->invalidateFilter();
    this->sort(0);
}
void MeasurementConfigurationProxyModel::setFilter(const QList<ElementTypes> neededElements, FeatureTypes typeOfFeature, QList<FeatureTypes> applicableFor) {
    this->resetFilter();

    this->filterType = eCreateFeature_MeasurementConfigurationFilter;
    this->neededElements = neededElements;
    this->typeOfFeature = typeOfFeature;
    this->functionIsApplicableFor = applicableFor;

    this->invalidateFilter();
    this->sort(0);
}
void MeasurementConfigurationProxyModel::setFilterProjectConfig() {
    this->resetFilter();

    this->filterType = eProject_MeasurementConfigurationFilter;

    this->invalidateFilter();
    this->sort(0);
}
void MeasurementConfigurationProxyModel::setFilterUserConfig(){
    this->resetFilter();

    this->filterType = eUser_MeasurementConfigurationFilter;

    this->invalidateFilter();
    this->sort(0);
}

void MeasurementConfigurationProxyModel::resetFilter() {
    this->filterType = eNo_MeasurementConfigurationFilter;

    this->neededElements.clear();
    this->typeOfFeature = FeatureTypes::eUndefinedFeature;
    this->functionIsApplicableFor.clear();
    this->sort(0);
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

    const QList<MeasurementConfig> configs = sourceModel->getMeasurementConfigurationManager()->getConfigs();
    MeasurementConfig mc = configs.at(source_row);
    const bool isProject = mc.isValid() && mc.isProjectConfig();
    const bool isUser = mc.isValid() && mc.isUserConfig();

    switch(this->filterType) {
    case eProject_MeasurementConfigurationFilter:
        return isProject;
    case eUser_MeasurementConfigurationFilter:
        return isUser;
    case eCreateFeature_MeasurementConfigurationFilter:
    case eNo_MeasurementConfigurationFilter:
    case eAll_MeasurementConfigurationFilter:
    default:
        //check if the index is a saved config
        if(isUser || isProject){

            if(this->neededElements.isEmpty()) {
                return true;
            }

            for (ElementTypes elementType : this->neededElements) {
                if(mc.applicableFor(elementType, this->functionIsApplicableFor)) {
                    return true;
                }
            }
        }
        return false;
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

bool MeasurementConfigurationProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const {
    MeasurementConfigurationModel *sourceModel = static_cast<MeasurementConfigurationModel *>(this->sourceModel());
    if(sourceModel == NULL){
        return false;
    }
    QVariant leftData = sourceModel->data(left);
    QVariant rightData = sourceModel->data(right);

    return leftData.toString().compare(rightData.toString(), Qt::CaseInsensitive) <0;
}

QVariant MeasurementConfigurationProxyModel::data(const QModelIndex &index, int role) const{
    MeasurementConfigurationModel *sourceModel = static_cast<MeasurementConfigurationModel *>(this->sourceModel());
    if(sourceModel == NULL){
        return QVariant();
    }
    QVariant data = sourceModel->data(this->mapToSource(index), role);

    if(role != Qt::DisplayRole){
        return data;
    }

    if( this->filterType == eCreateFeature_MeasurementConfigurationFilter
        && sourceModel->isUserConfig(this->mapToSource(index))
        ){
        return QString("%1 [user]").arg(data.toString());
    }

    return data;
}
