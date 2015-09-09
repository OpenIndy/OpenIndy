#include "functionstatisticmodel.h"

/*!
 * \brief FunctionStatisticModel::FunctionStatisticModel
 * \param feature
 * \param parent
 */
FunctionStatisticModel::FunctionStatisticModel(const QPointer<Feature> &feature, QObject *parent) : feature(feature), functionIndex(-1){

}

/*!
 * \brief FunctionStatisticModel::rowCount
 * \param parent
 * \return
 */
int FunctionStatisticModel::rowCount(const QModelIndex &parent) const{

    //check feature and function index
    if(this->feature.isNull() || this->functionIndex < 0 || this->functionIndex >= this->feature->getFunctions().size()){
        return 0;
    }

    //check function
    if(this->feature->getFunctions()[this->functionIndex].isNull()){
        return 0;
    }

    return this->feature->getFunctions()[this->functionIndex]->getStatistic().getDisplayResiduals().size();

}

/*!
 * \brief FunctionStatisticModel::columnCount
 * \param parent
 * \return
 */
int FunctionStatisticModel::columnCount(const QModelIndex &parent) const{

    //check feature and function index
    if(this->feature.isNull() || this->functionIndex < 0 || this->functionIndex >= this->feature->getFunctions().size()){
        return 0;
    }

    //check function
    if(this->feature->getFunctions()[this->functionIndex].isNull()){
        return 0;
    }

    //check if there is at least one display residual
    if(this->feature->getFunctions()[this->functionIndex]->getStatistic().getDisplayResiduals().size() == 0){
        return 0;
    }

    return this->feature->getFunctions()[this->functionIndex]->getStatistic().getDisplayResiduals()[0].corrections.size() + 1;

}

/*!
 * \brief FunctionStatisticModel::data
 * \param index
 * \param role
 * \return
 */
QVariant FunctionStatisticModel::data(const QModelIndex &index, int role) const{

    //check role
    if(role != Qt::DisplayRole || !index.isValid()){
        return QVariant();
    }

    //check feature and function index
    if(this->feature.isNull() || this->functionIndex < 0 || this->functionIndex >= this->feature->getFunctions().size()){
        return QVariant();
    }

    //check function
    if(this->feature->getFunctions()[this->functionIndex].isNull()){
        return QVariant();
    }

    //check display residuals
    const Statistic &statistic = this->feature->getFunctions()[this->functionIndex]->getStatistic();
    if(index.row() >= statistic.getDisplayResiduals().size()
            || statistic.getDisplayResiduals()[index.row()].corrections.size() + 1 <= index.column()){
        return QVariant();
    }

    Residual residual = statistic.getDisplayResiduals()[index.row()];
    if(index.column() == 0){
        return residual.elementId;
    }else{
        return QString::number(convertFromDefault(residual.corrections.values()[index.column() - 1],
                                                  this->parameterDisplayConfig.getDisplayUnit(eMetric)),
                               'f', this->parameterDisplayConfig.getDisplayDigits(eMetric));
    }

}

/*!
 * \brief FunctionStatisticModel::headerData
 * \param section
 * \param orientation
 * \param role
 * \return
 */
QVariant FunctionStatisticModel::headerData(int section, Qt::Orientation orientation, int role) const{

    if((Qt::DisplayRole == role) &&
            (Qt::Horizontal == orientation) &&
            (0 <= section) &&
            (section < columnCount())){

        if(section == 0){
            return "id";
        }else{
            return this->feature->getFunctions()[this->functionIndex]->getStatistic().getDisplayResiduals()[0].corrections.keys().at(section-1);
        }

    }

    return QVariant();
}

/*!
 * \brief FunctionStatisticModel::getFeature
 * \return
 */
const QPointer<Feature> &FunctionStatisticModel::getFeature() const{
    return this->feature;
}

/*!
 * \brief FunctionStatisticModel::setFeature
 * \param feature
 */
void FunctionStatisticModel::setFeature(const QPointer<Feature> &feature){
    this->feature = feature;
    this->updateModel();
}

/*!
 * \brief FunctionStatisticModel::getFunctionIndex
 * \return
 */
const int &FunctionStatisticModel::getFunctionIndex() const{
    return this->functionIndex;
}

/*!
 * \brief FunctionStatisticModel::setFunctionIndex
 * \param index
 */
void FunctionStatisticModel::setFunctionIndex(const int &index){
    this->functionIndex = index;
    this->updateModel();
}

/*!
 * \brief FunctionStatisticModel::getParameterDisplayConfig
 * \return
 */
const ParameterDisplayConfig &FunctionStatisticModel::getParameterDisplayConfig() const{
    return this->parameterDisplayConfig;
}

/*!
 * \brief FunctionStatisticModel::setParameterDisplayConfig
 * \param config
 */
void FunctionStatisticModel::setParameterDisplayConfig(const ParameterDisplayConfig &config){
    this->parameterDisplayConfig = config;
    this->updateModel();
}

/*!
 * \brief FunctionStatisticModel::updateModel
 */
void FunctionStatisticModel::updateModel(){
    emit this->layoutAboutToBeChanged();
    emit this->layoutChanged();
}

