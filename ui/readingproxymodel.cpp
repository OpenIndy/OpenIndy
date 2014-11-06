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

    if(OiFeatureState::getActiveFeature()->getFeature() == NULL){
        return false;
    }

    if(OiFeatureState::getActiveFeature()->getGeometry() != NULL){
        geom = OiFeatureState::getActiveFeature()->getGeometry();
    }else if(OiFeatureState::getActiveFeature()->getStation() != NULL){
        geom = OiFeatureState::getActiveFeature()->getStation()->position;
    }else{
        return false;
    }

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

    delete geom;
}
