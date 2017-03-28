#include "functionweightstablemodel.h"

/*!
 * \brief FunctionWeightsTableModel::FunctionWeightsTableModel
 * \param parent
 */
FunctionWeightsTableModel::FunctionWeightsTableModel(QPointer<OiJob> job, QObject *parent) : QAbstractTableModel(parent)
{
    this->functionPosition = -1;
    this->setCurrentJob(job);
}

/*!
 * \brief FunctionWeightsTableModel::FunctionWeightsTableModel
 * \param parent
 */
FunctionWeightsTableModel::FunctionWeightsTableModel(QObject *parent) : QAbstractTableModel(parent)
{
    this->functionPosition = -1;
}

/*!
 * \brief FunctionWeightsTableModel::getCurrentJob
 * \return
 */
const QPointer<OiJob> &FunctionWeightsTableModel::getCurrentJob() const
{
    return this->currentJob;
}

/*!
 * \brief FunctionWeightsTableModel::setCurrentJob
 * \param job
 */
void FunctionWeightsTableModel::setCurrentJob(const QPointer<OiJob> &job)
{
    if(!job.isNull()){

        //disconnect current job
        if(!this->currentJob.isNull()){
            this->disconnectJob();
        }

        this->currentJob = job;
        this->connectJob();
        this->updateModel();
    }
}

/*!
 * \brief FunctionWeightsTableModel::rowCount
 * \param parent
 * \return
 */
int FunctionWeightsTableModel::rowCount(const QModelIndex &parent) const
{
    //check current job
    if(this->currentJob.isNull()){
        return 0;
    }

    //get and check active feature
    if(this->currentJob->getActiveFeature().isNull() || this->currentJob->getActiveFeature()->getFeature().isNull()){
        return 0;
    }
    //check feature for valid trafo param
    QPointer<FeatureWrapper> feature = this->currentJob->getActiveFeature();
    if(feature.isNull() || feature->getTrafoParam().isNull()){
        return 0;
    }

    //check selected function position
    if(this->functionPosition < 0 || this->functionPosition >= feature->getTrafoParam()->getFunctions().size()
            || feature->getTrafoParam()->getFunctions().at(this->functionPosition).isNull()){
        return 0;
    }
    Function *function = feature->getTrafoParam()->getFunctions().at(this->functionPosition);

    if(function->getInputElements().size() >= 1){
        return function->getInputElements().value(0).size();
    }
    return 0;
}

/*!
 * \brief FunctionWeightsTableModel::columnCount
 * \param parent
 * \return
 */
int FunctionWeightsTableModel::columnCount(const QModelIndex &parent) const
{
    return 4;
}

/*!
 * \brief FunctionWeightsTableModel::data
 * \param index
 * \param role
 * \return
 */
QVariant FunctionWeightsTableModel::data(const QModelIndex &index, int role) const
{
    //check current job and model index
    if(this->currentJob.isNull() || !index.isValid()){
        return QVariant();
    }

    //get and check active feature
    if(this->currentJob->getActiveFeature().isNull() || this->currentJob->getActiveFeature()->getFeature().isNull()){
        return QVariant();
    }
    QPointer<Feature> feature = this->currentJob->getActiveFeature()->getFeature();

    //check selected function position
    if(this->functionPosition < 0 || this->functionPosition >= feature->getFunctions().size()
            || feature->getFunctions().at(this->functionPosition).isNull()){
        return QVariant();
    }
    Function *function = feature->getFunctions().at(this->functionPosition);

    QList<InputElement> inputElem = function->getInputElements().value(0);

    FeatureWrapper *fw = this->currentJob->getFeatureById(inputElem.at(index.row()).id);

    if(role == Qt::DisplayRole){

        switch (index.column()) {
        case 0:
            return fw->getFeature()->getFeatureName();
        }

    }else if(role == Qt::CheckStateRole){

        switch (index.column()) {
        case 1:
            if(inputElem.at(index.row()).ignoredDestinationParams.contains(eUnknownX)){
                return Qt::Unchecked;
            }else{
                return Qt::Checked;
            }
        case 2:
            if(inputElem.at(index.row()).ignoredDestinationParams.contains(eUnknownY)){
                return Qt::Unchecked;
            }else{
                return Qt::Checked;
            }
        case 3:
            if(inputElem.at(index.row()).ignoredDestinationParams.contains(eUnknownZ)){
                return Qt::Unchecked;
            }else{
                return Qt::Checked;
            }
        default:
            break;
        }
    }
    return QVariant();
}

/*!
 * \brief FunctionWeightsTableModel::headerData
 * \param section
 * \param orientation
 * \param role
 * \return
 */
QVariant FunctionWeightsTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if((Qt::DisplayRole == role) &&
            (Qt::Horizontal == orientation) &&
            (0 <= section) &&
            (section < columnCount())){

        switch (section) {
        case 0:
            return "feature name";
            break;
        case 1:
            return "use X";
            break;
        case 2:
            return "use Y";
            break;
        case 3:
            return "use Z";
            break;
        }
    }

    return QVariant();
}

/*!
 * \brief FunctionWeightsTableModel::flags
 * \param index
 * \return
 */
Qt::ItemFlags FunctionWeightsTableModel::flags(const QModelIndex &index) const
{
    //get parent flags
    Qt::ItemFlags myFlags = QAbstractTableModel::flags(index);

    //check index
    if(!index.isValid()){
        return myFlags;
    }
    switch (index.column()) {
    case 1:
        return (myFlags | Qt::ItemIsEditable | Qt::ItemIsUserCheckable);
    case 2:
        return (myFlags | Qt::ItemIsEditable | Qt::ItemIsUserCheckable);
    case 3:
        return (myFlags | Qt::ItemIsEditable | Qt::ItemIsUserCheckable);
    }
    return myFlags;
}

/*!
 * \brief FunctionWeightsTableModel::setData
 * \param index
 * \param value
 * \param role
 * \return
 */
bool FunctionWeightsTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    //check current job and model index
    if(this->currentJob.isNull() || !index.isValid()){
        return false;
    }

    //get the feature to index.row
    if(this->currentJob->getFeatureCount() <= index.row()){
        return false;
    }

    //get and check column index
    int column = index.column();
    if(column < 0 || 3 < column){
        return false;
    }

    //get and check active feature
    if(this->currentJob->getActiveFeature().isNull() || this->currentJob->getActiveFeature()->getFeature().isNull()){
        return false;
    }
    QPointer<Feature> feature = this->currentJob->getActiveFeature()->getFeature();

    //check selected function position
    if(this->functionPosition < 0 || this->functionPosition >= feature->getFunctions().size()
            || feature->getFunctions().at(this->functionPosition).isNull()){
        return false;
    }
    Function *function = feature->getFunctions().at(this->functionPosition);

    QList<InputElement> inputElem = function->getInputElements().value(0);

    FeatureWrapper *fw = this->currentJob->getFeatureById(inputElem.at(index.row()).id);

    if(fw->getGeometry().isNull()){
        return false;
    }

    if(role == Qt::CheckStateRole){

        switch (index.column()) {
        case 1:{
            bool useX = value.toBool();

            if(!fw->getGeometry()->getIsNominal()){

                for(int i=0; i<inputElem.size();i++){
                    if(fw->getGeometry()->getId() == inputElem.at(i).id){
                        if(useX){
                            if(inputElem.at(i).ignoredDestinationParams.contains(GeometryParameters::eUnknownX)){
                                inputElem[i].ignoredDestinationParams.removeOne(GeometryParameters::eUnknownX);
                            }
                            this->currentJob->getActiveFeature()->getFeature()->getFunctions()[this->functionPosition]->replaceInputElement(inputElem.at(i),this->functionPosition);
                            this->updateModel();
                            return true;
                        }else{
                            if(!inputElem.at(i).ignoredDestinationParams.contains(GeometryParameters::eUnknownX)){
                                inputElem[i].ignoredDestinationParams.append(GeometryParameters::eUnknownX);
                            }
                            this->currentJob->getActiveFeature()->getFeature()->getFunctions()[this->functionPosition]->replaceInputElement(inputElem.at(i),this->functionPosition);
                            this->updateModel();
                            return true;
                        }
                    }
                }
            }
            return false;}
        case 2:{
            bool useY = value.toBool();

            if(!fw->getGeometry()->getIsNominal()){

                for(int i=0; i<inputElem.size();i++){
                    if(fw->getGeometry()->getId() == inputElem.at(i).id){
                        if(useY){
                            if(inputElem.at(i).ignoredDestinationParams.contains(eUnknownY)){
                                inputElem[i].ignoredDestinationParams.removeOne(eUnknownY);
                            }
                            this->currentJob->getActiveFeature()->getFeature()->getFunctions()[this->functionPosition]->replaceInputElement(inputElem.at(i),this->functionPosition);
                            this->updateModel();
                            return true;
                        }else{
                            if(!inputElem.at(i).ignoredDestinationParams.contains(eUnknownY)){
                                inputElem[i].ignoredDestinationParams.append(eUnknownY);
                            }
                            this->currentJob->getActiveFeature()->getFeature()->getFunctions()[this->functionPosition]->replaceInputElement(inputElem.at(i),this->functionPosition);
                            this->updateModel();
                            return true;
                        }
                    }
                }
            }
            return false;}
        case 3:{
            bool useZ = value.toBool();

            if(!fw->getGeometry()->getIsNominal()){

                for(int i=0; i<inputElem.size();i++){
                    if(fw->getGeometry()->getId() == inputElem.at(i).id){
                        if(useZ){
                            if(inputElem.at(i).ignoredDestinationParams.contains(eUnknownZ)){
                                inputElem[i].ignoredDestinationParams.removeOne(eUnknownZ);
                            }
                            this->currentJob->getActiveFeature()->getFeature()->getFunctions()[this->functionPosition]->replaceInputElement(inputElem.at(i),this->functionPosition);
                            this->updateModel();
                            return true;
                        }else{
                            if(!inputElem.at(i).ignoredDestinationParams.contains(eUnknownZ)){
                                inputElem[i].ignoredDestinationParams.append(eUnknownZ);
                            }
                            this->currentJob->getActiveFeature()->getFeature()->getFunctions()[this->functionPosition]->replaceInputElement(inputElem.at(i),this->functionPosition);
                            this->updateModel();
                            return true;
                        }
                    }
                }
            }
            return false;}
        default:
            return false;
            break;
        }
    }
    return false;
}

/*!
 * \brief FunctionWeightsTableModel::setFunctionPosition
 * \param pos
 */
void FunctionWeightsTableModel::setFunctionPosition(int pos)
{
    this->functionPosition = pos;
}

/*!
 * \brief FunctionWeightsTableModel::updateModel
 */
void FunctionWeightsTableModel::updateModel()
{
    emit this->layoutAboutToBeChanged();
    emit this->layoutChanged();
}

/*!
 * \brief FunctionWeightsTableModel::connectJob
 */
void FunctionWeightsTableModel::connectJob()
{
    QObject::connect(this->currentJob.data(), &OiJob::activeFeatureChanged, this, &FunctionWeightsTableModel::updateModel, Qt::AutoConnection);
    QObject::connect(this->currentJob.data(), &OiJob::featureFunctionsChanged, this, &FunctionWeightsTableModel::updateModel, Qt::AutoConnection);
}

/*!
 * \brief FunctionWeightsTableModel::disconnectJob
 */
void FunctionWeightsTableModel::disconnectJob()
{
    QObject::disconnect(this->currentJob.data(), &OiJob::activeFeatureChanged, this, &FunctionWeightsTableModel::updateModel);
    QObject::disconnect(this->currentJob.data(), &OiJob::featureFunctionsChanged, this, &FunctionWeightsTableModel::updateModel);
}

