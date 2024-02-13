#include "functiontableproxymodel.h"

/*!
 * \brief FunctionTableProxyModel::FunctionTableProxyModel
 * \param parent
 */
FunctionTableProxyModel::FunctionTableProxyModel(QObject *parent) :
    QSortFilterProxyModel(parent)
{
}

/*!
 * \brief FunctionTableProxyModel::addFunction
 * \param index
 */
void FunctionTableProxyModel::addFunction(const QModelIndex &index){

    //ckeck index
    if(!index.isValid()){
        return;
    }

    //check current job
    if(this->currentJob.isNull()){
        return;
    }

    //check active feature
    if(this->currentJob->getActiveFeature().isNull() || this->currentJob->getActiveFeature()->getFeature().isNull()){
        return;
    }

    //get and check source model
    FunctionTableModel *sourceModel = static_cast<FunctionTableModel *>(this->sourceModel());
    if(sourceModel == NULL){
        return;
    }

    //get function name and plugin file path
    QString functionName = sourceModel->getFunctionName(this->mapToSource(index));
    QString filePath = sourceModel->getPluginFilePath(this->mapToSource(index));

    //load and check function
    QPointer<Function> function = PluginLoader::loadFunctionPlugin(filePath, functionName);
    if(function.isNull()){
        return;
    }

    //add the function and its dependencies
    emit this->startAddFunction(function);

}

/*!
 * \brief FunctionTableProxyModel::getCurrentJob
 * \return
 */
const QPointer<OiJob> &FunctionTableProxyModel::getCurrentJob() const{
    return this->currentJob;
}

/*!
 * \brief FunctionTableProxyModel::setCurrentJob
 * \param job
 */
void FunctionTableProxyModel::setCurrentJob(const QPointer<OiJob> &job){
    if(!job.isNull()){

        //disconnect current job
        if(!this->currentJob.isNull()){
            this->disconnectJob();
        }

        this->currentJob = job;
        this->connectJob();
        this->invalidateFilter();

    }
}

/*!
 * \brief FunctionTableProxyModel::filterAcceptsRow
 * \param source_row
 * \param source_parent
 * \return
 */
bool FunctionTableProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const{

    //check current job
    if(this->currentJob.isNull()){
        return false;
    }

    //check and get active feature
    if(this->currentJob->getActiveFeature().isNull() || this->currentJob->getActiveFeature()->getFeature().isNull()){
        return false;
    }

    //get and cast source model
    FunctionTableModel *source_model = dynamic_cast<FunctionTableModel *>(this->sourceModel());
    if(source_model == NULL){
        return false;
    }

    int numFunctionPlugins = 0;
    foreach(const sdb::Plugin &plugin, source_model->getPlugins()){

        //run through all function of the current plugin
        foreach(const sdb::Function &function, plugin.functions){

            //if the current function corresponds to the given row index
            if(source_row == numFunctionPlugins){

                //check if the function is applicable for the active feature's feature type
                if(!function.applicableFor.contains(this->currentJob->getActiveFeature()->getFeatureTypeEnum())){
                    return false;
                }

                if(getIsGeometry(this->currentJob->getActiveFeature()->getFeatureTypeEnum())){ //geometry

                    if(this->currentJob->getActiveFeature()->getGeometry()->getIsNominal()){ //nominal

                        if(function.iid.compare(OiMetaData::iid_SpecialFunction) != 0
                                && function.iid.compare(OiMetaData::iid_ConstructFunction) != 0
                                && function.iid.compare(OiMetaData::iid_FitFunction) != 0
                                && function.iid.compare(OiMetaData::iid_GeodeticFunction) != 0
                                && function.iid.compare(OiMetaData::iid_SystemTransformation) != 0){
                            return true;
                        }

                    }else{ //actual

                        if(this->currentJob->getActiveFeature()->getFeature()->getFunctions().size() == 0){ // first function

                            if(function.iid.compare(OiMetaData::iid_ObjectTransformation) != 0
                                    && function.iid.compare(OiMetaData::iid_SystemTransformation) != 0){
                                return true;
                            }

                        }else{ //not the first function

                            if(function.iid.compare(OiMetaData::iid_SpecialFunction) != 0
                                    && function.iid.compare(OiMetaData::iid_ConstructFunction) != 0
                                    && function.iid.compare(OiMetaData::iid_FitFunction) != 0
                                    && function.iid.compare(OiMetaData::iid_GeodeticFunction) != 0
                                    && function.iid.compare(OiMetaData::iid_SystemTransformation) != 0){
                                return true;
                            }

                        }

                    }

                }else if(this->currentJob->getActiveFeature()->getFeatureTypeEnum() == eTrafoParamFeature){ //trafo param

                    if(this->currentJob->getActiveFeature()->getFeature()->getFunctions().size() == 0){ // first function

                        if(function.iid.compare(OiMetaData::iid_SystemTransformation) == 0){
                            return true;
                        }

                    }else{ //not the first function

                        if(function.iid.compare(OiMetaData::iid_ObjectTransformation) == 0){
                            return true;
                        }

                    }

                }

                return false;

            }

            numFunctionPlugins++;

        }

    }

    return false;

}

/*!
 * \brief FunctionTableProxyModel::filterAcceptsColumn
 * \param source_column
 * \param source_parent
 * \return
 */
bool FunctionTableProxyModel::filterAcceptsColumn(int source_column, const QModelIndex &source_parent) const{
    return true;
}

/*!
 * \brief FunctionTableProxyModel::connectJob
 */
void FunctionTableProxyModel::connectJob(){

    QObject::connect(this->currentJob.data(), &OiJob::activeFeatureChanged, this, &FunctionTableProxyModel::invalidateFilter, Qt::AutoConnection);
    QObject::connect(this->currentJob.data(), &OiJob::featureFunctionsChanged, this, &FunctionTableProxyModel::invalidateFilter, Qt::AutoConnection);

    QObject::connect(this, &FunctionTableProxyModel::startAddFunction, this->currentJob.data(), &OiJob::addFunction, Qt::AutoConnection);

}

/*!
 * \brief FunctionTableProxyModel::disconnectJob
 */
void FunctionTableProxyModel::disconnectJob(){

    QObject::disconnect(this->currentJob.data(), &OiJob::activeFeatureChanged, this, &FunctionTableProxyModel::invalidateFilter);
    QObject::disconnect(this->currentJob.data(), &OiJob::featureFunctionsChanged, this, &FunctionTableProxyModel::invalidateFilter);

    QObject::disconnect(this, &FunctionTableProxyModel::startAddFunction, this->currentJob.data(), &OiJob::addFunction);

}

QVariant FunctionTableProxyModel::data(const QModelIndex &index, int role) const{

    if(role == Qt::ToolTipRole) {
       return "double click to use function";
    }
    return QSortFilterProxyModel::data(index, role);
}
