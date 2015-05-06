#include "activefeaturefunctionsmodel.h"

/*!
 * \brief ActiveFeatureFunctionsModel::ActiveFeatureFunctionsModel
 * \param job
 * \param parent
 */
ActiveFeatureFunctionsModel::ActiveFeatureFunctionsModel(QPointer<OiJob> job, QObject *parent) : QAbstractItemModel(parent){

    this->rootItem = new FunctionTreeItem("functions");
    this->setCurrentJob(job);

}

/*!
 * \brief ActiveFeatureFunctionsModel::ActiveFeatureFunctionsModel
 * \param parent
 */
ActiveFeatureFunctionsModel::ActiveFeatureFunctionsModel(QObject *parent){

    this->rootItem = new FunctionTreeItem("functions");

}

/*!
 * \brief ActiveFeatureFunctionsModel::rowCount
 * \param parent
 * \return
 */
int ActiveFeatureFunctionsModel::rowCount(const QModelIndex &parent) const{

    FunctionTreeItem *childItem = NULL;
    if(!parent.isValid()){
        childItem = rootItem;
    }else{
        childItem = static_cast<FunctionTreeItem*>(parent.internalPointer());
    }
    if(childItem != NULL){
        return childItem->getChildCount();
    }
    return 0;

}

/*!
 * \brief ActiveFeatureFunctionsModel::columnCount
 * \param parent
 * \return
 */
int ActiveFeatureFunctionsModel::columnCount(const QModelIndex &parent) const{
    return 1;
}

/*!
 * \brief ActiveFeatureFunctionsModel::data
 * \param index
 * \param role
 * \return
 */
QVariant ActiveFeatureFunctionsModel::data(const QModelIndex &index, int role) const{

    if(!index.isValid())
        return QVariant();

    FunctionTreeItem *item = static_cast<FunctionTreeItem*>(index.internalPointer());

    if(item != NULL){
        if(role == Qt::DisplayRole){ //return display text for tree view item
            return item->getDisplayValue();
        }
    }

    return QVariant();

}

/*!
 * \brief ActiveFeatureFunctionsModel::index
 * Create model index for every node
 * \param row
 * \param column
 * \param parent
 * \return
 */
QModelIndex ActiveFeatureFunctionsModel::index(int row, int column, const QModelIndex &parent) const{

    if(!hasIndex(row, column, parent))
        return QModelIndex();

    FunctionTreeItem *parentItem = NULL;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<FunctionTreeItem*>(parent.internalPointer());

    if(parentItem != NULL){
        FunctionTreeItem *childItem = parentItem->getChild(row);
        if (childItem != NULL)
            return createIndex(row, column, childItem);
        else
            return QModelIndex();
    }else{
        return QModelIndex();
    }

}

/*!
 * \brief ActiveFeatureFunctionsModel::parent
 * \param index
 * \return
 */
QModelIndex ActiveFeatureFunctionsModel::parent(const QModelIndex &index) const{

    if (!index.isValid())
        return QModelIndex();

    FunctionTreeItem *childItem = static_cast<FunctionTreeItem*>(index.internalPointer());
    if(childItem != NULL){
        FunctionTreeItem *parentItem = childItem->getParent();
        if (parentItem == NULL || parentItem == rootItem)
            return QModelIndex();
        return createIndex(parentItem->getIndex(), 0, parentItem);
    }else{
        return QModelIndex();
    }

}

/*!
 * \brief ActiveFeatureFunctionsModel::headerData
 * \param section
 * \param orientation
 * \param role
 * \return
 */
QVariant ActiveFeatureFunctionsModel::headerData(int section, Qt::Orientation orientation, int role) const{
    if((Qt::DisplayRole == role) &&
            (Qt::Horizontal == orientation) &&
            (0 <= section) &&
            (section < columnCount())){

        return "functions";

    }
    return QVariant();
}

/*!
 * \brief ActiveFeatureFunctionsModel::canRemoveFunction
 * \param index
 * \return
 */
bool ActiveFeatureFunctionsModel::canRemoveFunction(const QModelIndex &index) const{

    //check index (has to be valid and its parent has to be the root item)
    if(!index.isValid() || index.parent().isValid()){
        return false;
    }

    //check current job
    if(this->currentJob.isNull()){
        return false;
    }

    //check and get active feature
    if(this->currentJob->getActiveFeature().isNull() || this->currentJob->getActiveFeature()->getFeature().isNull()){
        return false;
    }
    Feature *feature = this->currentJob->getActiveFeature()->getFeature();

    //check function
    if(index.row() >= feature->getFunctions().size() || feature->getFunctions().at(index.row()).isNull()){
        return false;
    }

    //do not delete the first function if more than one function is available
    if(feature->getFunctions().size() > 1 && index.row() == 0){
        return false;
    }

    return true;

}

/*!
 * \brief ActiveFeatureFunctionsModel::removeFunction
 * \param index
 */
void ActiveFeatureFunctionsModel::removeFunction(const QModelIndex &index){

    //check index (has to be valid and its parent has to be the root item)
    if(!index.isValid() || index.parent().isValid()){
        return;
    }

    //check current job
    if(this->currentJob.isNull()){
        return;
    }

    //check and get active feature
    if(this->currentJob->getActiveFeature().isNull() || this->currentJob->getActiveFeature()->getFeature().isNull()){
        return;
    }
    Feature *feature = this->currentJob->getActiveFeature()->getFeature();

    //check and get function
    if(index.row() >= feature->getFunctions().size() || feature->getFunctions().at(index.row()).isNull()){
        return;
    }

    //emit signal to remove the function and its dependencies
    int functionIndex = index.row();
    emit this->startRemoveFunction(functionIndex);

}

/*!
 * \brief ActiveFeatureFunctionsModel::getFunctionDescription
 * \param index
 * \return
 */
QString ActiveFeatureFunctionsModel::getFunctionDescription(const QModelIndex &index) const{

    //check index (has to be valid and its parent has to be the root item)
    if(!index.isValid() || index.parent().isValid()){
        return "";
    }

    //check current job
    if(this->currentJob.isNull()){
        return "";
    }

    //check and get active feature
    if(this->currentJob->getActiveFeature().isNull() || this->currentJob->getActiveFeature()->getFeature().isNull()){
        return "";
    }
    Feature *feature = this->currentJob->getActiveFeature()->getFeature();

    //check and get function
    if(index.row() >= feature->getFunctions().size() || feature->getFunctions().at(index.row()).isNull()){
        return "";
    }
    Function *function = feature->getFunctions().at(index.row());

    return function->getMetaData().description;

}

/*!
 * \brief ActiveFeatureFunctionsModel::getInputElementDexcription
 * \param index
 * \return
 */
QString ActiveFeatureFunctionsModel::getInputElementDescription(const QModelIndex &index) const{

    //check index (has to be valid and its parent must not be the root item)
    if(!index.isValid() || !index.parent().isValid()){
        return "";
    }

    //check current job
    if(this->currentJob.isNull()){
        return "";
    }

    //check and get active feature
    if(this->currentJob->getActiveFeature().isNull() || this->currentJob->getActiveFeature()->getFeature().isNull()){
        return "";
    }
    Feature *feature = this->currentJob->getActiveFeature()->getFeature();

    //check and get function
    if(index.parent().row() >= feature->getFunctions().size() || feature->getFunctions().at(index.parent().row()).isNull()){
        return "";
    }
    Function *function = feature->getFunctions().at(index.parent().row());

    //check and get needed element
    if(index.row() >= function->getNeededElements().size()){
        return "";
    }
    NeededElement element = function->getNeededElements().at(index.row());

    return element.description;

}

/*!
 * \brief ActiveFeatureFunctionsModel::getIntegerParameter
 * \param index
 * \return
 */
QMap<QString, int> ActiveFeatureFunctionsModel::getIntegerParameter(const QModelIndex &index) const{

    //check index (has to be valid and its parent has to be the root item)
    if(!index.isValid() || index.parent().isValid()){
        return QMap<QString, int>();
    }

    //check current job
    if(this->currentJob.isNull()){
        return QMap<QString, int>();
    }

    //check and get active feature
    if(this->currentJob->getActiveFeature().isNull() || this->currentJob->getActiveFeature()->getFeature().isNull()){
        return QMap<QString, int>();
    }
    Feature *feature = this->currentJob->getActiveFeature()->getFeature();

    //check and get function
    if(index.row() >= feature->getFunctions().size() || feature->getFunctions().at(index.row()).isNull()){
        return QMap<QString, int>();
    }
    Function *function = feature->getFunctions().at(index.row());

    return function->getScalarInputParams().intParameter;

}

/*!
 * \brief ActiveFeatureFunctionsModel::getDoubleParameter
 * \param index
 * \return
 */
QMap<QString, double> ActiveFeatureFunctionsModel::getDoubleParameter(const QModelIndex &index) const{

    //check index (has to be valid and its parent has to be the root item)
    if(!index.isValid() || index.parent().isValid()){
        return QMap<QString, double>();
    }

    //check current job
    if(this->currentJob.isNull()){
        return QMap<QString, double>();
    }

    //check and get active feature
    if(this->currentJob->getActiveFeature().isNull() || this->currentJob->getActiveFeature()->getFeature().isNull()){
        return QMap<QString, double>();
    }
    Feature *feature = this->currentJob->getActiveFeature()->getFeature();

    //check and get function
    if(index.row() >= feature->getFunctions().size() || feature->getFunctions().at(index.row()).isNull()){
        return QMap<QString, double>();
    }
    Function *function = feature->getFunctions().at(index.row());

    return function->getScalarInputParams().doubleParameter;

}

/*!
 * \brief ActiveFeatureFunctionsModel::getStringParameterSelection
 * \param index
 * \return
 */
QMap<QString, QString> ActiveFeatureFunctionsModel::getStringParameterSelection(const QModelIndex &index) const{

    //check index (has to be valid and its parent has to be the root item)
    if(!index.isValid() || index.parent().isValid()){
        return QMap<QString, QString>();
    }

    //check current job
    if(this->currentJob.isNull()){
        return QMap<QString, QString>();
    }

    //check and get active feature
    if(this->currentJob->getActiveFeature().isNull() || this->currentJob->getActiveFeature()->getFeature().isNull()){
        return QMap<QString, QString>();
    }
    Feature *feature = this->currentJob->getActiveFeature()->getFeature();

    //check and get function
    if(index.row() >= feature->getFunctions().size() || feature->getFunctions().at(index.row()).isNull()){
        return QMap<QString, QString>();
    }
    Function *function = feature->getFunctions().at(index.row());

    return function->getScalarInputParams().stringParameter;

}

/*!
 * \brief ActiveFeatureFunctionsModel::setScalarInputParameters
 * \param index
 * \param intParams
 * \param doubleParams
 * \param stringParams
 */
void ActiveFeatureFunctionsModel::setScalarInputParameters(const QModelIndex &index, const QMap<QString, int> &intParams, const QMap<QString, double> &doubleParams, const QMap<QString, QString> &stringParams){

    //check index (has to be valid and its parent has to be the root item)
    if(!index.isValid() || index.parent().isValid()){
        return;
    }

    //check current job
    if(this->currentJob.isNull()){
        return;
    }

    //check and get active feature
    if(this->currentJob->getActiveFeature().isNull() || this->currentJob->getActiveFeature()->getFeature().isNull()){
        return;
    }
    Feature *feature = this->currentJob->getActiveFeature()->getFeature();

    //check and get function
    if(index.row() >= feature->getFunctions().size() || feature->getFunctions().at(index.row()).isNull()){
        return;
    }
    Function *function = feature->getFunctions().at(index.row());

    //create scalar input parameters
    ScalarInputParams params;
    params.intParameter = intParams;
    params.doubleParameter = doubleParams;
    params.stringParameter = stringParams;

    //set scalar input parameters
    function->setScalarInputParams(params);

}

/*!
 * \brief ActiveFeatureFunctionsModel::getStringParameter
 * \param index
 * \return
 */
QMultiMap<QString, QString> ActiveFeatureFunctionsModel::getStringParameter(const QModelIndex &index) const{

    //check index (has to be valid and its parent has to be the root item)
    if(!index.isValid() || index.parent().isValid()){
        return QMultiMap<QString, QString>();
    }

    //check current job
    if(this->currentJob.isNull()){
        return QMultiMap<QString, QString>();
    }

    //check and get active feature
    if(this->currentJob->getActiveFeature().isNull() || this->currentJob->getActiveFeature()->getFeature().isNull()){
        return QMultiMap<QString, QString>();
    }
    Feature *feature = this->currentJob->getActiveFeature()->getFeature();

    //check and get function
    if(index.row() >= feature->getFunctions().size() || feature->getFunctions().at(index.row()).isNull()){
        return QMultiMap<QString, QString>();
    }
    Function *function = feature->getFunctions().at(index.row());

    return function->getStringParameter();

}

/*!
 * \brief ActiveFeatureFunctionsModel::getCurrentJob
 * \return
 */
const QPointer<OiJob> &ActiveFeatureFunctionsModel::getCurrentJob() const{
    return this->currentJob;
}

/*!
 * \brief ActiveFeatureFunctionsModel::setCurrentJob
 * \param job
 */
void ActiveFeatureFunctionsModel::setCurrentJob(const QPointer<OiJob> &job){
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
 * \brief ActiveFeatureFunctionsModel::getRootItem
 * \return
 */
const QPointer<FunctionTreeItem> &ActiveFeatureFunctionsModel::getRootItem() const{
    return this->rootItem;
}

/*!
 * \brief ActiveFeatureFunctionsModel::updateModel
 */
void ActiveFeatureFunctionsModel::updateModel(){

    //check if a job is set and the root (header) item exists
    if(this->currentJob.isNull() || this->rootItem.isNull()){
        return;
    }

    //check and get active feature
    if(this->currentJob->getActiveFeature().isNull() || this->currentJob->getActiveFeature()->getFeature().isNull()){
        return;
    }
    Feature *feature = this->currentJob->getActiveFeature()->getFeature();

    //start resetting the model
    emit this->beginResetModel();

    //delete the old tree hierarchy
    this->rootItem->deleteChildren();

    //add all functions of the active feature to the tree hierarchy
    foreach(const QPointer<Function> &function, feature->getFunctions()){

        //check function
        if(function.isNull()){
            this->rootItem->deleteChildren();
            return;
        }

        //create function tree items and add needed elements
        FunctionTreeItem *itemFunction = new FunctionTreeItem(function->getMetaData().name);
        foreach(const NeededElement &element, function->getNeededElements()){

            //distinguish between 1 and n elements as input element
            if(element.infinite){
                FunctionTreeItem *itemElement = new FunctionTreeItem(QString("n %2").arg(getElementTypePluralName(element.typeOfElement)));
                itemFunction->appendChild(itemElement);
            }else{
                FunctionTreeItem *itemElement = new FunctionTreeItem(QString("1 %2").arg(getElementTypeName(element.typeOfElement)));
                itemFunction->appendChild(itemElement);
            }

        }
        this->rootItem->appendChild(itemFunction);

    }

    //reset finished
    emit this->endResetModel();

    emit this->layoutAboutToBeChanged();
    emit this->layoutChanged();

}

/*!
 * \brief ActiveFeatureFunctionsModel::connectJob
 */
void ActiveFeatureFunctionsModel::connectJob(){

    QObject::connect(this->currentJob.data(), &OiJob::activeFeatureChanged, this, &ActiveFeatureFunctionsModel::updateModel, Qt::AutoConnection);
    QObject::connect(this->currentJob.data(), &OiJob::featureFunctionsChanged, this, &ActiveFeatureFunctionsModel::updateModel, Qt::AutoConnection);

    QObject::connect(this, &ActiveFeatureFunctionsModel::startRemoveFunction, this->currentJob.data(), &OiJob::removeFunction, Qt::AutoConnection);

}

/*!
 * \brief ActiveFeatureFunctionsModel::disconnectJob
 */
void ActiveFeatureFunctionsModel::disconnectJob(){

    QObject::disconnect(this->currentJob.data(), &OiJob::activeFeatureChanged, this, &ActiveFeatureFunctionsModel::updateModel);
    QObject::disconnect(this->currentJob.data(), &OiJob::featureFunctionsChanged, this, &ActiveFeatureFunctionsModel::updateModel);

    QObject::disconnect(this, &ActiveFeatureFunctionsModel::startRemoveFunction, this->currentJob.data(), &OiJob::removeFunction);

}
