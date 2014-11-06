#include "observationmodel.h"

ObservationModel::ObservationModel(QObject *parent ) :
    QAbstractTableModel(parent)
{
}

int ObservationModel::rowCount(const QModelIndex& ) const{

    if(OiFeatureState::getActiveFeature()->getFeature() == NULL){
        return 0;
    }

    if(OiFeatureState::getActiveFeature()->getGeometry() != NULL && OiFeatureState::getActiveFeature()->getGeometry()->getObservations().size() > 0){
        return OiFeatureState::getActiveFeature()->getGeometry()->getObservations().size();
    }
    if(OiFeatureState::getActiveFeature()->getStation() != NULL && OiFeatureState::getActiveFeature()->getStation()->position->getObservations().size() >0){
        return OiFeatureState::getActiveFeature()->getStation()->position->getObservations().size();
    }
    return 0;
}

int ObservationModel::columnCount(const QModelIndex &parent) const{
    return GUIConfiguration::allObsAttributes.size();
}

QVariant ObservationModel::data(const QModelIndex &index, int role) const{

    if(OiFeatureState::getActiveFeature()->getFeature() == NULL){
        return QVariant();
    }

    if(!index.isValid())
        return QVariant();

    Geometry *geom = NULL;
    QString targetgeoms;

    if(OiFeatureState::getActiveFeature()->getGeometry() != NULL){
        geom = OiFeatureState::getActiveFeature()->getGeometry();
    }else if(OiFeatureState::getActiveFeature()->getStation() != NULL){
        geom = OiFeatureState::getActiveFeature()->getStation()->position;
    }

    if(Qt::DisplayRole == role){

        switch (index.column()) {
        case 0:
            return QString::number(geom->getObservations().at(index.row())->getId(),'f',0);
            break;
        case 1:
            return geom->getObservations().at(index.row())->myStation->getFeatureName();
            break;
        case 2:
            targetgeoms = geom->getObservations().at(index.row())->myTargetGeometries.at(0)->getFeatureName();
            for(int i=1; i<geom->getObservations().at(index.row())->myTargetGeometries.size();i++){
                targetgeoms += ", " + geom->getObservations().at(index.row())->myTargetGeometries.at(i)->getFeatureName();
            }
            return targetgeoms;
            break;
        case 3:
            return QString::number((geom->getObservations().at(index.row())->myXyz.getAt(0))*UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits);
            break;
        case 4:
            return QString::number(geom->getObservations().at(index.row())->myXyz.getAt(1)*UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits);
            break;
        case 5:
            return QString::number(geom->getObservations().at(index.row())->myXyz.getAt(2)*UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits);
            break;
        case 6:
            return QString(geom->getObservations().at(index.row())->isValid?"true":"false");
            break;
        case 7:
            return QString::number(geom->getObservations().at(index.row())->sigmaXyz.getAt(0)*UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits);
            break;
        case 8:
            return QString::number(geom->getObservations().at(index.row())->sigmaXyz.getAt(1)*UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits);
            break;
        case 9:
            return QString::number(geom->getObservations().at(index.row())->sigmaXyz.getAt(2)*UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits);
            break;
        default:
            break;
        }
    }

    return QVariant();
}

QVariant ObservationModel::headerData(int section, Qt::Orientation orientation, int role) const{

    QStringList m_columns  = GUIConfiguration::allObsAttributes;

    if((Qt::DisplayRole == role) &&
            (Qt::Horizontal == orientation)&&
            (0 <= section) &&
            (section < columnCount())){

        return m_columns.at(section);
    }
    return QVariant();
}

void ObservationModel::updateModel(){
    emit layoutAboutToBeChanged();
    emit layoutChanged();
}
