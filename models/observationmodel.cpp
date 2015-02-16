#include "observationmodel.h"

ObservationModel::ObservationModel(QObject *parent ) :
    QAbstractTableModel(parent)
{
}

/*!
 * \brief rowCount calculates the number of rows depending on the number of observations of this feature.
 * \return
 */
int ObservationModel::rowCount(const QModelIndex& ) const{

    if(OiJob::getActiveFeature()->getFeature() == NULL){
        return 0;
    }

    if(OiJob::getActiveFeature()->getGeometry() != NULL && OiJob::getActiveFeature()->getGeometry()->getObservations().size() > 0){
        return OiJob::getActiveFeature()->getGeometry()->getObservations().size();
    }
    if(OiJob::getActiveFeature()->getStation() != NULL && OiJob::getActiveFeature()->getStation()->position->getObservations().size() >0){
        return OiJob::getActiveFeature()->getStation()->position->getObservations().size();
    }
    return 0;
}

/*!
 * \brief columnCount returns the number of columns. Its valud depends on the attribute definition in the GUIConfiguration class.
 * \param parent
 * \return
 */
int ObservationModel::columnCount(const QModelIndex &parent) const{
    return GUIConfiguration::allObsAttributes.size();
}

/*!
 * \brief data displays all the attribute values
 * \param index
 * \param role
 * \return
 */
QVariant ObservationModel::data(const QModelIndex &index, int role) const{

    if(OiJob::getActiveFeature()->getFeature() == NULL){
        return QVariant();
    }

    if(!index.isValid())
        return QVariant();

    Geometry *geom = NULL;
    QString targetgeoms;

    if(OiJob::getActiveFeature()->getGeometry() != NULL){
        geom = OiJob::getActiveFeature()->getGeometry();
    }else if(OiJob::getActiveFeature()->getStation() != NULL){
        geom = OiJob::getActiveFeature()->getStation()->position;
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
            //return QString::number((geom->getObservations().at(index.row())->myXyz.getAt(0))*OiUnitConverter::getDistanceMultiplier(),'f',OiUnitConverter::distanceDigits);
            break;
        case 4:
            //return QString::number(geom->getObservations().at(index.row())->myXyz.getAt(1)*OiUnitConverter::getDistanceMultiplier(),'f',OiUnitConverter::distanceDigits);
            break;
        case 5:
            //return QString::number(geom->getObservations().at(index.row())->myXyz.getAt(2)*OiUnitConverter::getDistanceMultiplier(),'f',OiUnitConverter::distanceDigits);
            break;
        case 6:
            //return QString(geom->getObservations().at(index.row())->getIsValid()?"true":"false");
            break;
        case 7:
            //return QString::number(geom->getObservations().at(index.row())->sigmaXyz.getAt(0)*OiUnitConverter::getDistanceMultiplier(),'f',OiUnitConverter::distanceDigits);
            break;
        case 8:
            //return QString::number(geom->getObservations().at(index.row())->sigmaXyz.getAt(1)*OiUnitConverter::getDistanceMultiplier(),'f',OiUnitConverter::distanceDigits);
            break;
        case 9:
            //return QString::number(geom->getObservations().at(index.row())->sigmaXyz.getAt(2)*OiUnitConverter::getDistanceMultiplier(),'f',OiUnitConverter::distanceDigits);
            break;
        case 10:
            //return QString(geom->getObservations().at(index.row())->getIsSolved()?"true":"false");
            break;
        case 11:
            //return geom->getObservations().at(index.row())->getIsUsed();
            break;
        default:
            break;
        }
    }
    return QVariant();
}

/*!
 * \brief headerData displays the specified column names
 * \param section
 * \param orientation
 * \param role
 * \return
 */
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

/*!
 * \brief flags
 * Add edit-functionality to some cells of the model
 * \param index
 * \return
 */
Qt::ItemFlags ObservationModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags myFlags = QAbstractTableModel::flags(index);
    return (myFlags | Qt::ItemIsEditable);
}

/*!
 * \brief setData updates the edited features
 * \param index
 * \param value
 * \param role
 * \return
 */
bool ObservationModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(!index.isValid()){
        return false;
    }

    if(OiJob::getActiveFeature() != NULL){
        if(index.column() == 11){ //use state of observation
            OiJob::getActiveFeature()->getGeometry()->getObservations().at(index.row())->setIsUsed(value.toBool());
            emit recalcFeature();
        }
        return true;
    }
    return false;
}

/*!
 * \brief updateModel
 */
void ObservationModel::updateModel(){
    emit layoutAboutToBeChanged();
    emit layoutChanged();
    emit resizeView();
}
