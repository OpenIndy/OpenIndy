#include "readingmodel.h"

ReadingModel::ReadingModel(QObject *parent) :
    QAbstractTableModel(parent)
{
}

int ReadingModel::rowCount(const QModelIndex& ) const{

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

int ReadingModel::columnCount(const QModelIndex &parent) const{
    return GUIConfiguration::allReadAttributes.size();
}

QVariant ReadingModel::data(const QModelIndex &index, int role) const{

    if(OiFeatureState::getActiveFeature()->getFeature() == NULL){
        return QVariant();
    }

    if(!index.isValid())
        return QVariant();

    Geometry *geom = NULL;

    if(OiFeatureState::getActiveFeature()->getGeometry() != NULL){
        geom = OiFeatureState::getActiveFeature()->getGeometry();
    }else if(OiFeatureState::getActiveFeature()->getStation() != NULL){
        geom = OiFeatureState::getActiveFeature()->getStation()->position;
    }

    if(geom == NULL){
        return QVariant();
    }

    if(Qt::DisplayRole == role){

        QString instrument = geom->getObservations().at(index.row())->myReading->instrument->getMetaData()->name;

        switch (geom->getObservations().at(index.row())->myReading->typeofReading) {
        case Configuration::ePolar:

            switch (index.column()) {
            case 0:
                return QString::number(geom->getObservations().at(index.row())->myReading->id);
                break;
            case 1:
                return "polar reading";
                break;
            case 2:
                return geom->getObservations().at(index.row())->myReading->measuredAt.toString();
                break;
            case 3:
                return instrument;
                break;
            case 4:
                return QString::number(geom->getObservations().at(index.row())->myReading->rPolar.azimuth*UnitConverter::getAngleMultiplier(),'f',UnitConverter::angleDigits);
                break;
            case 5:
                return QString::number(geom->getObservations().at(index.row())->myReading->rPolar.zenith*UnitConverter::getAngleMultiplier(),'f',UnitConverter::angleDigits);
                break;
            case 6:
                return QString::number(geom->getObservations().at(index.row())->myReading->rPolar.distance*UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits);
                break;
            case 7:
                return QString::number(geom->getObservations().at(index.row())->myReading->rPolar.sigmaAzimuth*UnitConverter::getAngleMultiplier(),'f',UnitConverter::angleDigits);
                break;
            case 8:
                return QString::number(geom->getObservations().at(index.row())->myReading->rPolar.sigmaZenith*UnitConverter::getAngleMultiplier(),'f',UnitConverter::angleDigits);
                break;
            case 9:
                return QString::number(geom->getObservations().at(index.row())->myReading->rPolar.sigmaDistance*UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits);
                break;
            case 10:
                return "-/-";
                break;
            case 11:
                return "-/-";
                break;
            case 12:
                return "-/-";
                break;
            case 13:
                return "-/-";
                break;
            case 14:
                return "-/-";
                break;
            case 15:
                return "-/-";
                break;
            case 16:
                return QString(geom->getObservations().at(index.row())->myReading->rPolar.fsBs?"true":"false");
                break;
            case 17:
                return "-/-";
                break;
            case 18:
                return QString(geom->getObservations().at(index.row())->myReading->rPolar.isValid?"true":"false");
                break;
            default:
                break;
            }
            break;
        case Configuration::eCartesian:
            switch (index.column()) {
            case 0:
                return QString::number(geom->getObservations().at(index.row())->myReading->id);
                break;
            case 1:
                return "cartesian reading";
                break;
            case 2:
                return geom->getObservations().at(index.row())->myReading->measuredAt.toString();
                break;
            case 3:
                return instrument;
                break;
            case 4:
                return "-/-";
                break;
            case 5:
                return "-/-";
                break;
            case 6:
                return "-/-";
                break;
            case 7:
                return "-/-";
                break;
            case 8:
                return "-/-";
                break;
            case 9:
                return "-/-";
                break;
            case 10:
                return QString::number(geom->getObservations().at(index.row())->myReading->rCartesian.xyz.getAt(0)*UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits);
                break;
            case 11:
                return QString::number(geom->getObservations().at(index.row())->myReading->rCartesian.xyz.getAt(1)*UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits);
                break;
            case 12:
                return QString::number(geom->getObservations().at(index.row())->myReading->rCartesian.xyz.getAt(2)*UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits);
                break;
            case 13:
                return QString::number(geom->getObservations().at(index.row())->myReading->rCartesian.sigmaXyz.getAt(0)*UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits);
                break;
            case 14:
                return QString::number(geom->getObservations().at(index.row())->myReading->rCartesian.sigmaXyz.getAt(1)*UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits);
                break;
            case 15:
                return QString::number(geom->getObservations().at(index.row())->myReading->rCartesian.sigmaXyz.getAt(2)*UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits);
                break;
            case 16:
                return "-/-";
                break;
            case 17:
                return "-/-";
                break;
            case 18:
                return QString(geom->getObservations().at(index.row())->myReading->rCartesian.isValid?"true":"false");
                break;
            default:
                break;
            }
            break;
        case Configuration::eDistance:
            switch (index.column()) {
            case 0:
                return QString::number(geom->getObservations().at(index.row())->myReading->id);
                break;
            case 1:
                return "distance reading";
                break;
            case 2:
                return geom->getObservations().at(index.row())->myReading->measuredAt.toString();
                break;
            case 3:
                return instrument;
                break;
            case 4:
                return "-/-";
                break;
            case 5:
                return "-/-";
                break;
            case 6:
                return QString::number(geom->getObservations().at(index.row())->myReading->rDistance.distance * UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits);
                break;
            case 7:
                return "-/-";
                break;
            case 8:
                return "-/-";
                break;
            case 9:
                return QString::number(geom->getObservations().at(index.row())->myReading->rDistance.sigmaDistance*UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits);
                break;
            case 10:
                return "-/-";
                break;
            case 11:
                return "-/-";
                break;
            case 12:
                return "-/-";
                break;
            case 13:
                return "-/-";
                break;
            case 14:
                return "-/-";
                break;
            case 15:
                return "-/-";
                break;
            case 16:
                return "-/-";
                break;
            case 17:
                return "-/-";
                break;
            case 18:
                return QString(geom->getObservations().at(index.row())->myReading->rCartesian.isValid?"true":"false");
                break;
            default:
                break;
            }
            break;
        case Configuration::eDirection:
            switch (index.column()) {
            case 0:
                return QString::number(geom->getObservations().at(index.row())->myReading->id);
                break;
            case 1:
                return "direction reading";
                break;
            case 2:
                return geom->getObservations().at(index.row())->myReading->measuredAt.toString();
                break;
            case 3:
                return instrument;
                break;
            case 4:
                return QString::number(geom->getObservations().at(index.row())->myReading->rDirection.azimuth*UnitConverter::getAngleMultiplier(),'f',UnitConverter::angleDigits);
                break;
            case 5:
                return QString::number(geom->getObservations().at(index.row())->myReading->rDirection.zenith*UnitConverter::getAngleMultiplier(),'f',UnitConverter::angleDigits);
                break;
            case 6:
                return "-/-";
                break;
            case 7:
                return QString::number(geom->getObservations().at(index.row())->myReading->rDirection.sigmaAzimuth*UnitConverter::getAngleMultiplier(),'f',UnitConverter::angleDigits);
                break;
            case 8:
                return QString::number(geom->getObservations().at(index.row())->myReading->rDirection.sigmaZenith*UnitConverter::getAngleMultiplier(),'f',UnitConverter::angleDigits);
                break;
            case 9:
                return "-/-";
                break;
            case 10:
                return "-/-";
                break;
            case 11:
                return "-/-";
                break;
            case 12:
                return "-/-";
                break;
            case 13:
                return "-/-";
                break;
            case 14:
                return "-/-";
                break;
            case 15:
                return "-/-";
                break;
            case 16:
                return QString(geom->getObservations().at(index.row())->myReading->rDirection.fsBs?"true":"false");
                break;
            case 17:
                return "-/-";
                break;
            case 18:
                return QString(geom->getObservations().at(index.row())->myReading->rDirection.isValid?"true":"false");
                break;
            default:
                break;
            }
            break;
        case Configuration::eTemperatur:
            switch (index.column()) {
            case 0:
                return QString::number(geom->getObservations().at(index.row())->myReading->id);
                break;
            case 1:
                return "temperatur reading";
                break;
            case 2:
                return geom->getObservations().at(index.row())->myReading->measuredAt.toString();
                break;
            case 3:
                return instrument;
                break;
            case 4:
                return "-/-";
                break;
            case 5:
                return "-/-";
                break;
            case 6:
                return "-/-";
                break;
            case 7:
                return "-/-";
                break;
            case 8:
                return "-/-";
                break;
            case 9:
                return "-/-";
                break;
            case 10:
                return "-/-";
                break;
            case 11:
                return "-/-";
                break;
            case 12:
                return "-/-";
                break;
            case 13:
                return "-/-";
                break;
            case 14:
                return "-/-";
                break;
            case 15:
                return "-/-";
                break;
            case 16:
                return "-/-";
                break;
            case 17:
                return QString::number(UnitConverter::getTemperature(geom->getObservations().at(index.row())->myReading->rTemperature.tempDeg),'f',UnitConverter::temperatureDigits);
                break;
            case 18:
                return QString(geom->getObservations().at(index.row())->myReading->rTemperature.isValid?"true":"false");
                break;
            default:
                break;
            }
            break;
        default:
            break;
        }
    }
    return QVariant();
}

QVariant ReadingModel::headerData(int section, Qt::Orientation orientation, int role) const{

    QStringList m_columns  = GUIConfiguration::allReadAttributes;

    if((Qt::DisplayRole == role) &&
            (Qt::Horizontal == orientation)&&
            (0 <= section) &&
            (section < columnCount())){

        return m_columns.at(section);
    }
    return QVariant();
}

void ReadingModel::updateModel(){
    emit layoutAboutToBeChanged();
    emit layoutChanged();
}
