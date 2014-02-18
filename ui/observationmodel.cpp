#include "observationmodel.h"

ObservationModel::ObservationModel(FeatureWrapper &activeFeature,QObject *parent ) :
    QAbstractTableModel(parent),selectedFeature(activeFeature)
{
    /*
    m_columns.append("id");
    m_columns.append("station");
    m_columns.append("target geometry");
    m_columns.append("x");
    m_columns.append("y");
    m_columns.append("z");
    m_columns.append("valid");
    m_columns.append("sigma x");
    m_columns.append("sigma y");
    m_columns.append("sigma z");
    */
}

int ObservationModel::rowCount(const QModelIndex& ) const{

    if(this->selectedFeature.getGeometry() != NULL && this->selectedFeature.getGeometry()->myObservations.size() > 0){
        return this->selectedFeature.getGeometry()->myObservations.size();
    }
    if(this->selectedFeature.getStation() != NULL && this->selectedFeature.getStation()->position->myObservations.size() >0){
        return this->selectedFeature.getStation()->position->myObservations.size();
    }
    return 0;
}

int ObservationModel::columnCount(const QModelIndex &parent) const{
    //return m_columns.size();
    return 10;
}

QVariant ObservationModel::data(const QModelIndex &index, int role) const{

    if(!index.isValid())
        return QVariant();

    if(this->selectedFeature.getGeometry() != NULL){
        Geometry *geom = this->selectedFeature.getGeometry();
        QString targetgeoms;

        if(Qt::DisplayRole == role){

            switch (index.column()) {
            case 0:
                return QString::number(geom->myObservations.at(index.row())->id,'f',0);
                break;
            case 1:
                return geom->myObservations.at(index.row())->myStation->name;
                break;
            case 2:
                targetgeoms = geom->myObservations.at(index.row())->myTargetGeometries.at(0)->name;
                for(int i=1; i<geom->myObservations.at(index.row())->myTargetGeometries.size();i++){
                    targetgeoms += ", " + geom->myObservations.at(index.row())->myTargetGeometries.at(i)->name;
                }
                return targetgeoms;
                break;
            case 3:
                return QString::number((geom->myObservations.at(index.row())->myXyz.getAt(0))*UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits);
                break;
            case 4:
                return QString::number(geom->myObservations.at(index.row())->myXyz.getAt(1)*UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits);
                break;
            case 5:
                return QString::number(geom->myObservations.at(index.row())->myXyz.getAt(2)*UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits);
                break;
            case 6:
                return QString(geom->myObservations.at(index.row())->isValid?"true":"false");
                break;
            case 7:
                return QString::number(geom->myObservations.at(index.row())->sigmaXyz.getAt(0)*UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits);
                break;
            case 8:
                return QString::number(geom->myObservations.at(index.row())->sigmaXyz.getAt(1)*UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits);
                break;
            case 9:
                return QString::number(geom->myObservations.at(index.row())->sigmaXyz.getAt(2)*UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits);
                break;
            default:
                break;
            }
        }
    }
    if(this->selectedFeature.getStation() != NULL){
        Geometry *geom = this->selectedFeature.getStation()->position;
        QString targetgeoms;

        if(Qt::DisplayRole == role){

            switch (index.column()) {
            case 0:
                return QString::number(geom->myObservations.at(index.row())->id,'f',0);
                break;
            case 1:
                return geom->myObservations.at(index.row())->myStation->name;
                break;
            case 2:
                targetgeoms = geom->myObservations.at(index.row())->myTargetGeometries.at(0)->name;
                for(int i=1; i<geom->myObservations.at(index.row())->myTargetGeometries.size();i++){
                    targetgeoms += ", " + geom->myObservations.at(index.row())->myTargetGeometries.at(i)->name;
                }
                return targetgeoms;
                break;
            case 3:
                return QString::number((geom->myObservations.at(index.row())->myXyz.getAt(0))*UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits);
                break;
            case 4:
                return QString::number(geom->myObservations.at(index.row())->myXyz.getAt(1)*UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits);
                break;
            case 5:
                return QString::number(geom->myObservations.at(index.row())->myXyz.getAt(2)*UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits);
                break;
            case 6:
                return QString(geom->myObservations.at(index.row())->isValid?"true":"false");
                break;
            case 7:
                return QString::number(geom->myObservations.at(index.row())->sigmaXyz.getAt(0)*UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits);
                break;
            case 8:
                return QString::number(geom->myObservations.at(index.row())->sigmaXyz.getAt(1)*UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits);
                break;
            case 9:
                return QString::number(geom->myObservations.at(index.row())->sigmaXyz.getAt(2)*UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits);
                break;
            default:
                break;
            }
        }
    }
    return QVariant();
}

QVariant ObservationModel::headerData(int section, Qt::Orientation orientation, int role) const{

    QStringList m_columns;

    m_columns.append("id");
    m_columns.append("station");
    m_columns.append("target geometry");

    if(UnitConverter::distanceType == UnitConverter::eMETER){
        m_columns.append("x [m]");
        m_columns.append("y [m]");
        m_columns.append("z [m]");
    }

    if(UnitConverter::distanceType == UnitConverter::eMILLIMETER){
        m_columns.append("x [mm]");
        m_columns.append("y [mm]");
        m_columns.append("z [mm]");
    }


    m_columns.append("valid");
    if(UnitConverter::distanceType == UnitConverter::eMETER){
        m_columns.append("sigma x [m]");
        m_columns.append("sigma y [m]");
        m_columns.append("sigma z [m]");
    }

    if(UnitConverter::distanceType == UnitConverter::eMILLIMETER){
        m_columns.append("sigma x [mm]");
        m_columns.append("sigma y [mm]");
        m_columns.append("sigma z [mm]");
    }

    if((Qt::DisplayRole == role) &&
            (Qt::Horizontal == orientation) &&
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
