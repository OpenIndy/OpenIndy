#include "sensoraccuracymodel.h"

using namespace oi;

/*!
 * \brief SensorAccuracyModel::SensorAccuracyModel
 * \param parent
 */
SensorAccuracyModel::SensorAccuracyModel(QObject *parent) : QAbstractTableModel(parent){

}

/*!
 * \brief SensorAccuracyModel::rowCount
 * \param parent
 * \return
 */
int SensorAccuracyModel::rowCount(const QModelIndex &parent) const{
    if(this->sConfig.getIsValid() && this->sConfig.getPluginName().compare("") != 0
            && this->sConfig.getSensorName().compare("") != 0){
        return (15 + this->sConfig.getAccuracy().sigmaUndefined.values().size());
    }
    return 0;
}

/*!
 * \brief SensorAccuracyModel::columnCount
 * \param parent
 * \return
 */
int SensorAccuracyModel::columnCount(const QModelIndex &parent) const{
    return 1;
}

/*!
 * \brief SensorAccuracyModel::data
 * \param index
 * \param role
 * \return
 */
QVariant SensorAccuracyModel::data(const QModelIndex &index, int role) const{

    //check model index
    if(!index.isValid()){
        return QVariant();
    }

    //get row and column indices
    int rowIndex = index.row();
    int columnIndex = index.column();

    if(role == Qt::DisplayRole){

        //sigma type headers
        switch(rowIndex){
        case 0:
            return "polar";
        case 4:
            return "cartesian";
        case 8:
            return "level";
        case 12:
            return "temperature";
        case 14:
            return "undefined";
        }

        //accuracy values
        switch(rowIndex){
        case 1:
            return QString::number(convertFromDefault(this->sConfig.getAccuracy().sigmaAzimuth,
                                                      this->parameterDisplayConfig.getDisplayUnit(eAngular)),
                                   'f', this->parameterDisplayConfig.getDisplayDigits(eAngular));
        case 2:
            return QString::number(convertFromDefault(this->sConfig.getAccuracy().sigmaZenith,
                                                      this->parameterDisplayConfig.getDisplayUnit(eAngular)),
                                   'f', this->parameterDisplayConfig.getDisplayDigits(eAngular));
        case 3:
            return QString::number(convertFromDefault(this->sConfig.getAccuracy().sigmaDistance,
                                                      this->parameterDisplayConfig.getDisplayUnit(eMetric)),
                                   'f', this->parameterDisplayConfig.getDisplayDigits(eMetric));
        case 5:
            return QString::number(convertFromDefault(this->sConfig.getAccuracy().sigmaXyz.getAt(0),
                                                      this->parameterDisplayConfig.getDisplayUnit(eMetric)),
                                   'f', this->parameterDisplayConfig.getDisplayDigits(eMetric));
        case 6:
            return QString::number(convertFromDefault(this->sConfig.getAccuracy().sigmaXyz.getAt(1),
                                                      this->parameterDisplayConfig.getDisplayUnit(eMetric)),
                                   'f', this->parameterDisplayConfig.getDisplayDigits(eMetric));
        case 7:
            return QString::number(convertFromDefault(this->sConfig.getAccuracy().sigmaXyz.getAt(2),
                                                      this->parameterDisplayConfig.getDisplayUnit(eMetric)),
                                   'f', this->parameterDisplayConfig.getDisplayDigits(eMetric));
        case 9:
            return QString::number(this->sConfig.getAccuracy().sigmaI, 'f', 6);
        case 10:
            return QString::number(this->sConfig.getAccuracy().sigmaJ, 'f', 6);
        case 11:
            return QString::number(this->sConfig.getAccuracy().sigmaK, 'f', 6);
        case 13:
            return QString::number(convertFromDefault(this->sConfig.getAccuracy().sigmaTemp,
                                                      this->parameterDisplayConfig.getDisplayUnit(eTemperature)),
                                   'f', this->parameterDisplayConfig.getDisplayDigits(eTemperature));
        default:
            int index = rowIndex - 15;
            if(index >= 0 && index < this->sConfig.getAccuracy().sigmaUndefined.size()){
                return QString::number(this->sConfig.getAccuracy().sigmaUndefined.values().at(index), 'f', 6);
            }
        }

    }else if(role == Qt::BackgroundRole ){

        //sigma type headers
        if(rowIndex == 0 || rowIndex == 4 || rowIndex == 8 || rowIndex == 12 || rowIndex == 14){
            return QColor(Qt::lightGray);
        }

    }

    return QVariant();

}

/*!
 * \brief SensorAccuracyModel::headerData
 * \param section
 * \param orientation
 * \param role
 * \return
 */
QVariant SensorAccuracyModel::headerData(int section, Qt::Orientation orientation, int role) const{

    if((Qt::DisplayRole == role) && (Qt::Vertical == orientation) &&
            (0 <= section) && (section < rowCount())){

        switch(section){
        case 1:
            return "azimuth";
        case 2:
            return "zenith";
        case 3:
            return "distance";
        case 5:
            return "x";
        case 6:
            return "y";
        case 7:
            return "z";
        case 9:
            return "i";
        case 10:
            return "j";
        case 11:
            return "k";
        case 13:
            return "temperature";
        default:
            int index = section - 15;
            if(index >= 0 && index < this->sConfig.getAccuracy().sigmaUndefined.size()){
                return this->sConfig.getAccuracy().sigmaUndefined.keys().at(index);
            }
        }

    }else if((Qt::DisplayRole == role) && (Qt::Horizontal == orientation) &&
             (0 <= section) && (section < columnCount())){

        switch(section){
        case 0:
            return QString("sigma - %1 %2 %3")
                    .arg(getUnitTypeName(this->parameterDisplayConfig.getDisplayUnit(eMetric)))
                    .arg(getUnitTypeName(this->parameterDisplayConfig.getDisplayUnit(eAngular)))
                    .arg(getUnitTypeName(this->parameterDisplayConfig.getDisplayUnit(eTemperature)));
        }

    }

    return QVariant();

}


/*!
 * \brief SensorAccuracyModel::flags
 * Add edit-functionality to some cells of tablemodel
 * \param index
 * \return
 */
Qt::ItemFlags SensorAccuracyModel::flags(const QModelIndex &index) const{

    Qt::ItemFlags myFlags = QAbstractTableModel::flags(index);

    //check read only state
    if(this->isReadOnly){
        return myFlags;
    }

    //check sensor config
    if(!this->sConfig.getIsValid()){
        return myFlags;
    }

    //check model index
    if(!index.isValid()){
        return myFlags;
    }

    //get row and column indices
    int rowIndex = index.row();
    int columnIndex = index.column();

    if(rowIndex != 0 && rowIndex != 4 && rowIndex != 8 && rowIndex != 12 && rowIndex != 14){
        return (myFlags | Qt::ItemIsEditable);
    }
    return myFlags;

}

/*!
 * \brief SensorAccuracyModel::setData
 * Update the edited accuracy parameters
 * \param index
 * \param value
 * \param role
 * \return
 */
bool SensorAccuracyModel::setData(const QModelIndex & index, const QVariant & value, int role){

    //check model index
    if(!index.isValid()){
        return false;
    }

    //check read only state
    if(this->isReadOnly){
        return false;
    }

    //check sensor config
    if(!this->sConfig.getIsValid()){
        return false;
    }

    //get row and column indices
    int rowIndex = index.row();
    int columnIndex = index.column();

    if(rowIndex < rowCount()){

        Accuracy accuracy = this->sConfig.getAccuracy();

        switch(rowIndex){
        case 1:
            accuracy.sigmaAzimuth = convertToDefault(value.toDouble(), this->parameterDisplayConfig.getDisplayUnit(eAngular));
            break;
        case 2:
            accuracy.sigmaZenith = convertToDefault(value.toDouble(), this->parameterDisplayConfig.getDisplayUnit(eAngular));
            break;
        case 3:
            accuracy.sigmaDistance = convertToDefault(value.toDouble(), this->parameterDisplayConfig.getDisplayUnit(eMetric));
            break;
        case 5:
            accuracy.sigmaXyz.setAt(0, convertToDefault(value.toDouble(), this->parameterDisplayConfig.getDisplayUnit(eMetric)));
            break;
        case 6:
            accuracy.sigmaXyz.setAt(1, convertToDefault(value.toDouble(), this->parameterDisplayConfig.getDisplayUnit(eMetric)));
            break;
        case 7:
            accuracy.sigmaXyz.setAt(2, convertToDefault(value.toDouble(), this->parameterDisplayConfig.getDisplayUnit(eMetric)));
            break;
        case 9:
            accuracy.sigmaI = value.toDouble();
            break;
        case 10:
            accuracy.sigmaJ = value.toDouble();
            break;
        case 11:
            accuracy.sigmaK = value.toDouble();
            break;
        case 13:
            accuracy.sigmaTemp = convertToDefault(value.toDouble(), this->parameterDisplayConfig.getDisplayUnit(eTemperature));
            break;
        default:
            int index = rowIndex - 15;
            if(index >= 0 && index < this->sConfig.getAccuracy().sigmaUndefined.size()){
                accuracy.sigmaUndefined.insert(this->sConfig.getAccuracy().sigmaUndefined.keys().at(index),
                                               value.toDouble());
            }
            break;
        }

        //set new accuracy
        this->sConfig.setAccuracy(accuracy);

        emit this->sensorConfigurationChanged(this->sConfig);

        this->updateModel();

        return true;

    }

    return false;

}

/*!
 * \brief SensorAccuracyModel::getParameterDisplayConfig
 * \return
 */
const ParameterDisplayConfig &SensorAccuracyModel::getParameterDisplayConfig() const{
    return this->parameterDisplayConfig;
}

/*!
 * \brief SensorAccuracyModel::setParameterDisplayConfig
 * \param config
 */
void SensorAccuracyModel::setParameterDisplayConfig(const ParameterDisplayConfig &config){
    this->parameterDisplayConfig = config;
    this->updateModel();
}

/*!
 * \brief SensorAccuracyModel::getSensorConfiguration
 * \return
 */
const SensorConfiguration &SensorAccuracyModel::getSensorConfiguration() const{
    return this->sConfig;
}

/*!
 * \brief SensorAccuracyModel::setSensorConfiguration
 * \param sConfig
 */
void SensorAccuracyModel::setSensorConfiguration(const SensorConfiguration &sConfig){
    this->sConfig = sConfig;
    this->updateModel();
}

/*!
 * \brief SensorAccuracyModel::getIsReadOnly
 * \return
 */
bool SensorAccuracyModel::getIsReadOnly() const{
    return this->isReadOnly;
}

/*!
 * \brief SensorAccuracyModel::setIsReadOnly
 * \param isReadOnly
 */
void SensorAccuracyModel::setIsReadOnly(bool isReadOnly){
    this->isReadOnly = isReadOnly;
    this->updateModel();
}

/*!
 * \brief SensorAccuracyModel::updateModel
 */
void SensorAccuracyModel::updateModel(){
    emit this->layoutAboutToBeChanged();
    emit this->layoutChanged();
}
