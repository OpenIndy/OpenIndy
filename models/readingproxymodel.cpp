#include "readingproxymodel.h"

ReadingProxyModel::ReadingProxyModel(QObject *parent) :
    QSortFilterProxyModel(parent)
{
}

/*!
 * \brief ReadingProxyModel::filterAcceptsColumn filters the displayed columns in the reading model
 * \param source_column
 * \param source_parent
 * \return
 */
bool ReadingProxyModel::filterAcceptsColumn(int source_column, const QModelIndex &source_parent) const
{
    //display only the reading attributes that are specified in the guiconfiguration class.
    QList<int> displayColumns = GUIConfiguration::displayReadingAttributes();
    if(displayColumns.size() == 0){
        return true;
    }else{
        if(displayColumns.contains(source_column)){
            return true;
        }else{
            return false;
        }
    }

    return true;
}

/*!
 * \brief ReadingProxyModel::filterAcceptsRow filters the displayed rows in the reading model
 * \param source_row
 * \param source_parent
 * \return
 */
bool ReadingProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{

    Geometry *geom = NULL;

    if(OiJob::getActiveFeature()->getFeature() == NULL){
        return false;
    }
    //get geometry of feature
    if(OiJob::getActiveFeature()->getGeometry() != NULL){
        geom = OiJob::getActiveFeature()->getGeometry();
    //get geometry of station position
    }else if(OiJob::getActiveFeature()->getStation() != NULL){
        geom = OiJob::getActiveFeature()->getStation()->position;
    }else{
        return false;
    }

    if(geom->getObservations().size() <= source_row){
        qDebug() << "obs liste in proxy";
        return false;
    }


    //get selected reading type and display only the readings of the geom that have the same reading type.

    if(GUIConfiguration::readingType.compare(Configuration::sPolar) == 0){
        if(geom->getObservations().at(source_row)->myReading->typeofReading == Configuration::ePolar){
            return true;
        }else{
            return false;
        }
    }else if(GUIConfiguration::readingType.compare(Configuration::sCartesian) == 0){
        if(geom->getObservations().at(source_row)->myReading->typeofReading == Configuration::eCartesian){
            return true;
        }else{
            return false;
        }
    }else if(GUIConfiguration::readingType.compare(Configuration::sDistance) == 0){
        if(geom->getObservations().at(source_row)->myReading->typeofReading == Configuration::eDistance){
            return true;
        }else{
            return false;
        }
    }else if(GUIConfiguration::readingType.compare(Configuration::sDirection) == 0){
        if(geom->getObservations().at(source_row)->myReading->typeofReading == Configuration::eDirection){
            return true;
        }else{
            return false;
        }
    }else if(GUIConfiguration::readingType.compare(Configuration::sLevel) == 0){
        if(geom->getObservations().at(source_row)->myReading->typeofReading == Configuration::eLevel){
            return true;
        }else{
            return false;
        }
    }else if(GUIConfiguration::readingType.compare(Configuration::sTemperatur) == 0){
        if(geom->getObservations().at(source_row)->myReading->typeofReading == Configuration::eTemperatur){
            return true;
        }else{
            return false;
        }
    }else if(GUIConfiguration::readingType.compare("undefined") == 0){
        if(geom->getObservations().at(source_row)->myReading->typeofReading == Configuration::eUndefined){
            return true;
        }else{
            return false;
        }
    }
}
