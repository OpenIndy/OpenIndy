#include "pointfeaturemodel.h"

PointFeatureModel::PointFeatureModel(QObject *parent) : QAbstractListModel(parent)
{
}

/*!
 * \brief PointFeatureModel::rowCount
 * \param parent
 * \return
 */
int PointFeatureModel::rowCount(const QModelIndex &parent) const{

    int rowCount = 0;

    QList<FeatureWrapper *> geometries = OiFeatureState::getGeometries();
    foreach(FeatureWrapper *fw, geometries){
        if(fw != NULL && fw->getPoint() != NULL){
            rowCount++;
        }
    }

    return rowCount;

}

/*!
 * \brief PointFeatureModel::data
 * \param index
 * \param role
 * \return
 */
QVariant PointFeatureModel::data(const QModelIndex &index, int role) const{

    if(!index.isValid()){
        return QVariant();
    }

    if(role == Qt::DisplayRole){

        int currentIndex = 0;
        QList<FeatureWrapper *> geometries = OiFeatureState::getGeometries();
        foreach(FeatureWrapper *fw, geometries){
            if(fw != NULL && fw->getPoint() != NULL){
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
 * \brief PointFeatureModel::headerData
 * \param section
 * \param orientation
 * \param role
 * \return
 */
QVariant PointFeatureModel::headerData(int section, Qt::Orientation orientation, int role) const{

    if(role == Qt::DisplayRole){
        return "available point features";
    }

}
