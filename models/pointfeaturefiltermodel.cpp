#include "pointfeaturefiltermodel.h"

PointFeatureFilterModel::PointFeatureFilterModel(QObject *parent) : QSortFilterProxyModel(parent)
{
    this->filterGroup = "All groups";
}

/*!
 * \brief PointFeatureFilterModel::data
 * \param index
 * \param role
 * \return
 */
QVariant PointFeatureFilterModel::data(const QModelIndex &index, int role) const{

    if(!index.isValid()){
        return QVariant();
    }

    if(role == Qt::DisplayRole){

        int currentIndex = 0;
        QList<FeatureWrapper *> geometries = OiJob::getGeometries();
        foreach(FeatureWrapper *fw, geometries){
            if(fw != NULL && fw->getPoint() != NULL && (this->filterGroup.compare("All groups") == 0
                                                        || fw->getPoint()->getGroupName().compare(this->filterGroup) == 0)){
                if(currentIndex == index.row()){
                    return fw->getPoint()->getFeatureName();
                }
                currentIndex++;
            }
        }

    }

    return QVariant();

}

/*!
 * \brief PointFeatureFilterModel::setFilter
 * \param group
 */
void PointFeatureFilterModel::setFilter(QString group){
    this->filterGroup = group;
}

/*!
 * \brief PointFeatureFilterModel::getSelectedPoints
 * \param selectedIndices
 * \return
 */
QList<FeatureWrapper *> PointFeatureFilterModel::getSelectedPoints(QModelIndexList selectedIndices){

    QList<FeatureWrapper *> result;

    QList<FeatureWrapper *> geometries = OiJob::getGeometries();

    int currentIndex = 0;
    foreach(FeatureWrapper *fw, geometries){
        if(fw != NULL && fw->getPoint() != NULL && (this->filterGroup.compare("All groups") == 0
                                                    || fw->getPoint()->getGroupName().compare(this->filterGroup) == 0)){

            bool addPoint = false;
            for(int i = 0; i < selectedIndices.size(); i++){
                if(currentIndex == selectedIndices.at(i).row()){
                    addPoint = true;
                }
            }

            if(addPoint){
                result.append(fw);
            }

            currentIndex++;
        }
    }

    return result;

}
