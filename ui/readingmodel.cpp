#include "readingmodel.h"

ReadingModel::ReadingModel(QObject *parent) :
    QAbstractTableModel(parent)
{
}

/*!
 * \brief rowCount returns the number of readings depending on the number of observations
 * \return
 */
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

/*!
 * \brief columnCount returns the number of columns that are specified in the GUIConfiguration class
 * \param parent
 * \return
 */
int ReadingModel::columnCount(const QModelIndex &parent) const{
    return GUIConfiguration::allReadAttributes.size();
}

/*!
 * \brief data shows all the attribute values of the readings.
 * \param index
 * \param role
 * \return
 */
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

        if(geom->getObservations().size() <= index.row()){
            qDebug() << "obs liste zu kurz";
            return QVariant();
        }

        QString instrument = "";

        if(geom->getObservations().at(index.row())->myReading->instrument != NULL){
            instrument = geom->getObservations().at(index.row())->myReading->instrument->getMetaData()->name;
        }

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
                return Configuration::getSight(geom->getObservations().at(index.row())->myReading->face);
                break;
            case 17:
                return "-/-";
                break;
            case 18:
                return QString(geom->getObservations().at(index.row())->myReading->rPolar.isValid?"true":"false");
                break;
            case 19:
                return "-/-";
                break;
            case 20:
                return "-/-";
                break;
            case 21:
                return "-/-";
                break;
            case 22:
                return "-/-";
                break;
            case 23:
                return "-/-";
                break;
            case 24:
                return "-/-";
                break;
            case 25:
                return "-/-";
                break;
            case 26:
                return "-/-";
                break;
            case 27:
                return "-/-";
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
                return Configuration::getSight(geom->getObservations().at(index.row())->myReading->face);
                break;
            case 17:
                return "-/-";
                break;
            case 18:
                return QString(geom->getObservations().at(index.row())->myReading->rCartesian.isValid?"true":"false");
                break;
            case 19:
                return "-/-";
                break;
            case 20:
                return "-/-";
                break;
            case 21:
                return "-/-";
                break;
            case 22:
                return "-/-";
                break;
            case 23:
                return "-/-";
                break;
            case 24:
                return "-/-";
                break;
            case 25:
                return "-/-";
                break;
            case 26:
                return "-/-";
                break;
            case 27:
                return "-/-";
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
                return Configuration::getSight(geom->getObservations().at(index.row())->myReading->face);
                break;
            case 17:
                return "-/-";
                break;
            case 18:
                return QString(geom->getObservations().at(index.row())->myReading->rDistance.isValid?"true":"false");
                break;
            case 19:
                return "-/-";
                break;
            case 20:
                return "-/-";
                break;
            case 21:
                return "-/-";
                break;
            case 22:
                return "-/-";
                break;
            case 23:
                return "-/-";
                break;
            case 24:
                return "-/-";
                break;
            case 25:
                return "-/-";
                break;
            case 26:
                return "-/-";
                break;
            case 27:
                return "-/-";
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
                return Configuration::getSight(geom->getObservations().at(index.row())->myReading->face);
                break;
            case 17:
                return "-/-";
                break;
            case 18:
                return QString(geom->getObservations().at(index.row())->myReading->rDirection.isValid?"true":"false");
                break;
            case 19:
                return "-/-";
                break;
            case 20:
                return "-/-";
                break;
            case 21:
                return "-/-";
                break;
            case 22:
                return "-/-";
                break;
            case 23:
                return "-/-";
                break;
            case 24:
                return "-/-";
                break;
            case 25:
                return "-/-";
                break;
            case 26:
                return "-/-";
                break;
            case 27:
                return "-/-";
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
            case 19:
                return QString::number(UnitConverter::getTemperature(geom->getObservations().at(index.row())->myReading->rTemperature.sigmaTempDeg),'f',UnitConverter::temperatureDigits);
                break;
            case 20:
                return "-/-";
                break;
            case 21:
                return "-/-";
                break;
            case 22:
                return "-/-";
                break;
            case 23:
                return "-/-";
                break;
            case 24:
                return "-/-";
                break;
            case 25:
                return "-/-";
                break;
            case 26:
                return "-/-";
                break;
            case 27:
                return "-/-";
                break;
            default:
                break;
            }
            break;
        case Configuration::eLevel:
            switch (index.column()) {
            case 0:
                return QString::number(geom->getObservations().at(index.row())->myReading->id);
                break;
            case 1:
                return "level reading";
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
                return "-/-";
                break;
            case 18:
                return QString(geom->getObservations().at(index.row())->myReading->rLevel.isValid?"true":"false");
                break;
            case 19:
                return "-/-";
                break;
            case 20:
                return "-/-";
                break;
            case 21:
                return "-/-";
                break;
            case 22:
                return QString::number(geom->getObservations().at(index.row())->myReading->rLevel.RX,'f',UnitConverter::angleDigits);
                break;
            case 23:
                return QString::number(geom->getObservations().at(index.row())->myReading->rLevel.RY,'f',UnitConverter::angleDigits);
                break;
            case 24:
                return QString::number(geom->getObservations().at(index.row())->myReading->rLevel.RZ,'f',UnitConverter::angleDigits);
                break;
            case 25:
                return QString::number(geom->getObservations().at(index.row())->myReading->rLevel.sigmaRX,'f',UnitConverter::angleDigits);
                break;
            case 26:
                return QString::number(geom->getObservations().at(index.row())->myReading->rLevel.sigmaRY,'f',UnitConverter::angleDigits);
                break;
            case 27:
                return QString::number(geom->getObservations().at(index.row())->myReading->rLevel.sigmaRZ,'f',UnitConverter::angleDigits);
                break;
            default:
                break;
            }
            break;
        case Configuration::eUndefined:
            switch (index.column()) {
            case 0:
                return QString::number(geom->getObservations().at(index.row())->myReading->id);
                break;
            case 1:
                return "undefined reading";
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
                return "-/-";
                break;
            case 18:
                return QString(geom->getObservations().at(index.row())->myReading->rUndefined.isValid?"true":"false");
                break;
            case 19:
                return "-/-";
                break;
            case 20:
                return getUndefValues(geom->getObservations().at(index.row())->myReading->rUndefined.values);
                break;
            case 21:
                return getUndefValues(geom->getObservations().at(index.row())->myReading->rUndefined.sigmaValues);
                break;
            case 22:
                return "-/-";
                break;
            case 23:
                return "-/-";
                break;
            case 24:
                return "-/-";
                break;
            case 25:
                return "-/-";
                break;
            case 26:
                return "-/-";
                break;
            case 27:
                return "-/-";
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

/*!
 * \brief headerData displays all the header column names. Depends on the specification in the GUIConfiguration class.
 * \param section
 * \param orientation
 * \param role
 * \return
 */
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

/*!
 * \brief updateModel
 */
void ReadingModel::updateModel(){
    emit layoutAboutToBeChanged();
    emit layoutChanged();
    emit resizeView();
}

/*!
 * \brief getUndefValues returns all values included in the map as one string to display in the model.
 * \param undefReading
 * \return
 */
QString ReadingModel::getUndefValues(QMap<QString, double> undefReading) const
{
    QString result = "";

    QMapIterator<QString, double> i(undefReading);
    while(i.hasNext()){
        i.next();
        result = result + QString(i.key() + " " + i.value() + " ");
    }
    return result;
}
