#include "sensorparametersmodel.h"

/*!
 * \brief SensorParametersModel::SensorParametersModel
 * \param parent
 */
SensorParametersModel::SensorParametersModel(QObject *parent) : QAbstractTableModel(parent){

}

/*!
 * \brief SensorParametersModel::rowCount
 * \param parent
 * \return
 */
int SensorParametersModel::rowCount(const QModelIndex &parent) const{
    return this->sConfig.getStringParameter().uniqueKeys().size()
            + this->sConfig.getDoubleParameter().size()
            + this->sConfig.getIntegerParameter().size();
}

/*!
 * \brief SensorParametersModel::columnCount
 * \param parent
 * \return
 */
int SensorParametersModel::columnCount(const QModelIndex &parent) const{
    return 1;
}

/*!
 * \brief SensorParametersModel::data
 * \param index
 * \param role
 * \return
 */
QVariant SensorParametersModel::data(const QModelIndex &index, int role) const{

    //check model index
    if(!index.isValid()){
        return QVariant();
    }

    //get row and column indices
    int rowIndex = index.row();
    int columnIndex = index.column();

    if(role == Qt::DisplayRole){

        if(rowIndex < this->sConfig.getStringParameter().uniqueKeys().size()){
            return this->sConfig.getStringParameter().value(this->sConfig.getStringParameter().uniqueKeys().at(rowIndex));
        }else if(rowIndex < this->sConfig.getDoubleParameter().size() + this->sConfig.getStringParameter().uniqueKeys().size()){
            rowIndex = rowIndex - this->sConfig.getStringParameter().uniqueKeys().size();
            return QString::number(this->sConfig.getDoubleParameter().values().at(rowIndex), 'f', 8);
        }else{
            rowIndex = rowIndex - this->sConfig.getStringParameter().uniqueKeys().size()
                    - this->sConfig.getDoubleParameter().size();
            return QString::number(this->sConfig.getIntegerParameter().values().at(rowIndex), 'f', 8);
        }

    }

    return QVariant();

}

/*!
 * \brief SensorParametersModel::headerData
 * \param section
 * \param orientation
 * \param role
 * \return
 */
QVariant SensorParametersModel::headerData(int section, Qt::Orientation orientation, int role) const{

    if((Qt::DisplayRole == role) && (Qt::Vertical == orientation) &&
            (0 <= section) && (section < rowCount())){

        if(section < this->sConfig.getStringParameter().uniqueKeys().size()){
            return this->sConfig.getStringParameter().uniqueKeys().at(section);
        }else if(section < this->sConfig.getDoubleParameter().size() + this->sConfig.getStringParameter().uniqueKeys().size()){
            section = section - this->sConfig.getStringParameter().uniqueKeys().size();
            return this->sConfig.getDoubleParameter().keys().at(section);
        }else{
            section = section - this->sConfig.getStringParameter().uniqueKeys().size()
                    - this->sConfig.getDoubleParameter().size();
            return this->sConfig.getIntegerParameter().keys().at(section);
        }

    }else if((Qt::DisplayRole == role) && (Qt::Horizontal == orientation) &&
             (0 <= section) && (section < columnCount())){

        switch(section){
        case 0:
            return QString("parameter");
        }

    }

    return QVariant();

}


/*!
 * \brief SensorParametersModel::flags
 * Add edit-functionality to some cells of tablemodel
 * \param index
 * \return
 */
Qt::ItemFlags SensorParametersModel::flags(const QModelIndex &index) const{

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

    return (myFlags | Qt::ItemIsEditable);

}

/*!
 * \brief SensorParametersModel::setData
 * Update the edited sensor parameters
 * \param index
 * \param value
 * \param role
 * \return
 */
bool SensorParametersModel::setData(const QModelIndex & index, const QVariant & value, int role){

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

        if(rowIndex < this->sConfig.getStringParameter().uniqueKeys().size()){
            QMap<QString, QString> params = this->sConfig.getStringParameter();
            params.insert(this->sConfig.getStringParameter().uniqueKeys().at(rowIndex), value.toString());
            this->sConfig.setStringParameter(params);
        }else if(rowIndex < this->sConfig.getDoubleParameter().size() + this->sConfig.getStringParameter().uniqueKeys().size()){
            rowIndex = rowIndex - this->sConfig.getStringParameter().uniqueKeys().size();
            QMap<QString, double> params = this->sConfig.getDoubleParameter();
            params.insert(this->sConfig.getDoubleParameter().uniqueKeys().at(rowIndex), value.toDouble());
            this->sConfig.setDoubleParameter(params);
        }else{
            rowIndex = rowIndex - this->sConfig.getStringParameter().uniqueKeys().size()
                    - this->sConfig.getDoubleParameter().size();
            QMap<QString, int> params = this->sConfig.getIntegerParameter();
            params.insert(this->sConfig.getIntegerParameter().uniqueKeys().at(rowIndex), value.toInt());
            this->sConfig.setIntegerParameter(params);
        }

        emit this->sensorConfigurationChanged(this->sConfig);

        this->updateModel();

        return true;

    }

    return false;

}

/*!
 * \brief SensorParametersModel::getParameterDisplayConfig
 * \return
 */
const ParameterDisplayConfig &SensorParametersModel::getParameterDisplayConfig() const{
    return this->parameterDisplayConfig;
}

/*!
 * \brief SensorParametersModel::setParameterDisplayConfig
 * \param config
 */
void SensorParametersModel::setParameterDisplayConfig(const ParameterDisplayConfig &config){
    this->parameterDisplayConfig = config;
    this->updateModel();
}

/*!
 * \brief SensorParametersModel::getSensorConfiguration
 * \return
 */
const SensorConfiguration &SensorParametersModel::getSensorConfiguration() const{
    return this->sConfig;
}

/*!
 * \brief SensorParametersModel::setSensorConfiguration
 * \param sConfig
 */
void SensorParametersModel::setSensorConfiguration(const SensorConfiguration &sConfig){
    this->sConfig = sConfig;
    this->updateModel();
}

/*!
 * \brief SensorParametersModel::getIsReadOnly
 * \return
 */
bool SensorParametersModel::getIsReadOnly() const{
    return this->isReadOnly;
}

/*!
 * \brief SensorParametersModel::setIsReadOnly
 * \param isReadOnly
 */
void SensorParametersModel::setIsReadOnly(bool isReadOnly){
    this->isReadOnly = isReadOnly;
    this->updateModel();
}

/*!
 * \brief SensorParametersModel::updateModel
 */
void SensorParametersModel::updateModel(){
    emit this->layoutAboutToBeChanged();
    emit this->layoutChanged();
}
