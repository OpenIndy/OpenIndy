#include "readingmodel.h"

ReadingModel::ReadingModel(QObject *parent) :
    QAbstractTableModel(parent)
{
    /*
    m_columns.append("id");
    m_columns.append("type of reading");
    m_columns.append("measured at");
    m_columns.append("instrument");
    m_columns.append("azimuth");
    m_columns.append("zenith");
    m_columns.append("distance");
    m_columns.append("sigma azimuth");
    m_columns.append("sigma zenith");
    m_columns.append("sigma distance");
    m_columns.append("x");
    m_columns.append("y");
    m_columns.append("z");
    m_columns.append("sigma x");
    m_columns.append("sigma y");
    m_columns.append("sigma z");
    m_columns.append("fs bs");
    m_columns.append("temperature");
    m_columns.append("is valid");
    */

}

int ReadingModel::rowCount(const QModelIndex& ) const{

    if(OiFeatureState::getActiveFeature()->getGeometry() != NULL && OiFeatureState::getActiveFeature()->getGeometry()->getObservations().size() > 0){
        return OiFeatureState::getActiveFeature()->getGeometry()->getObservations().size();
    }
    if(OiFeatureState::getActiveFeature()->getStation() != NULL && OiFeatureState::getActiveFeature()->getStation()->position->getObservations().size() >0){
        return OiFeatureState::getActiveFeature()->getStation()->position->getObservations().size();
    }
    return 0;
}

int ReadingModel::columnCount(const QModelIndex &parent) const{
    //return m_columns.size();
    return 19;
}

QVariant ReadingModel::data(const QModelIndex &index, int role) const{

    if(!index.isValid())
        return QVariant();

    if(OiFeatureState::getActiveFeature()->getGeometry() != NULL){
        Geometry *geom = OiFeatureState::getActiveFeature()->getGeometry();

        if(Qt::DisplayRole == role){

            for(int i=0; i<geom->getObservations().size();i++){
                switch (geom->getObservations().at(i)->myReading->typeofReading) {
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
                        //was soll hier dargestellt werden ?
                        //return geom->myObservations.at(index.row())->myReading->instrument;
                        return "instrument ?!";
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
                        //was soll hier dargestellt werden ?
                        //return geom->myObservations.at(index.row())->myReading->instrument;
                        return "instrument ?!";
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
                        return "cartesian reading";
                        break;
                    case 2:
                        return geom->getObservations().at(index.row())->myReading->measuredAt.toString();
                        break;
                    case 3:
                        //was soll hier dargestellt werden ?
                        //return geom->myObservations.at(index.row())->myReading->instrument;
                        return "instrument ?!";
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
                        //was soll hier dargestellt werden ?
                        //return geom->myObservations.at(index.row())->myReading->instrument;
                        return "instrument ?!";
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
                        //was soll hier dargestellt werden ?
                        //return geom->myObservations.at(index.row())->myReading->instrument;
                        return "instrument ?!";
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


        }
    }
    if(OiFeatureState::getActiveFeature()->getStation() != NULL){
        Geometry *geom = OiFeatureState::getActiveFeature()->getStation()->position;

        if(Qt::DisplayRole == role){

            for(int i=0; i<geom->getObservations().size();i++){
                switch (geom->getObservations().at(i)->myReading->typeofReading) {
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
                        //was soll hier dargestellt werden ?
                        //return geom->myObservations.at(index.row())->myReading->instrument;
                        return "instrument ?!";
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
                        //was soll hier dargestellt werden ?
                        //return geom->myObservations.at(index.row())->myReading->instrument;
                        return "instrument ?!";
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
                        return "cartesian reading";
                        break;
                    case 2:
                        return geom->getObservations().at(index.row())->myReading->measuredAt.toString();
                        break;
                    case 3:
                        //was soll hier dargestellt werden ?
                        //return geom->myObservations.at(index.row())->myReading->instrument;
                        return "instrument ?!";
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
                        //was soll hier dargestellt werden ?
                        //return geom->myObservations.at(index.row())->myReading->instrument;
                        return "instrument ?!";
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
                        //was soll hier dargestellt werden ?
                        //return geom->myObservations.at(index.row())->myReading->instrument;
                        return "instrument ?!";
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


        }
    }
    return QVariant();
}

QVariant ReadingModel::headerData(int section, Qt::Orientation orientation, int role) const{

    QStringList m_columns;

    m_columns.append("id");
    m_columns.append("type of reading");
    m_columns.append("measured at");
    m_columns.append("instrument");

    if(UnitConverter::angleType == UnitConverter::eDECIMALDEGREE){
        m_columns.append("azimuth [dec. degree]");
        m_columns.append("zenith [dec. degree]");
    }
    if(UnitConverter::angleType == UnitConverter::eRADIANT){
        m_columns.append("azimuth [rad]");
        m_columns.append("zenith [rad]");
    }
    if(UnitConverter::angleType == UnitConverter::eGON){
        m_columns.append("azimuth [gon]");
        m_columns.append("zenith [gon]");
    }

    if(UnitConverter::distanceType == UnitConverter::eMETER){
        m_columns.append("distance [m]");
    }
    if(UnitConverter::distanceType == UnitConverter::eMILLIMETER){
        m_columns.append("distance [mm]");
    }

    if(UnitConverter::angleType == UnitConverter::eDECIMALDEGREE){
        m_columns.append("sigma azimuth [dec. degree]");
        m_columns.append("sigma zenith [dec. degree]");
    }
    if(UnitConverter::angleType == UnitConverter::eRADIANT){
        m_columns.append("sigma azimuth [rad]");
        m_columns.append("sigma zenith [rad]");
    }
    if(UnitConverter::angleType == UnitConverter::eGON){
        m_columns.append("sigma azimuth [gon]");
        m_columns.append("sigma zenith [gon]");
    }

    if(UnitConverter::distanceType == UnitConverter::eMETER){
        m_columns.append("sigma distance [m]");
    }
    if(UnitConverter::distanceType == UnitConverter::eMILLIMETER){
        m_columns.append("sigma distance [mm]");
    }

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
    m_columns.append("fs bs");

    if(UnitConverter::temperatureType == UnitConverter::eGRAD){
        m_columns.append("temperature [°C]");
    }

    m_columns.append("is valid");

    if((Qt::DisplayRole == role) &&
            (Qt::Horizontal == orientation) &&
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
