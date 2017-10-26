#include "functionweightproxymodel.h"

/*!
 * \brief FunctionWeightProxyModel::FunctionWeightProxyModel
 * \param parent
 */
FunctionWeightProxyModel::FunctionWeightProxyModel(QObject *parent) : QSortFilterProxyModel(parent)
{
    this->setDynamicSortFilter(true);
}

/*!
 * \brief FunctionWeightProxyModel::getCurrentJob
 * \return
 */
const QPointer<OiJob> &FunctionWeightProxyModel::getCurrentJob() const
{
    return this->job;
}

/*!
 * \brief FunctionWeightProxyModel::setCurrentJob
 * \param job
 */
void FunctionWeightProxyModel::setCurrentJob(const QPointer<OiJob> &job)
{
    if(!job.isNull()){
        this->job = job;
    }
}

/*!
 * \brief FunctionWeightProxyModel::mapFromSource
 * \param sourceIndex
 * \return
 */
QModelIndex FunctionWeightProxyModel::mapFromSource(const QModelIndex &sourceIndex) const
{
    //get and check proxyIndex
    QModelIndex proxyIndex = QSortFilterProxyModel::mapFromSource(sourceIndex);
    if(!proxyIndex.isValid()){
        return proxyIndex;
    }

    return proxyIndex;
}

/*!
 * \brief FunctionWeightProxyModel::mapToSource
 * \param proxyIndex
 * \return
 */
QModelIndex FunctionWeightProxyModel::mapToSource(const QModelIndex &proxyIndex) const
{
    //get and check sourceIndex
    QModelIndex sourceIndex = QSortFilterProxyModel::mapToSource(proxyIndex);
    if(!sourceIndex.isValid()){
        return sourceIndex;
    }

    //get and cast source model
    FunctionWeightsTableModel *source_model = dynamic_cast<FunctionWeightsTableModel *>(this->sourceModel());
    if(source_model == NULL){
        return QModelIndex();
    }

    return sourceIndex;
}

/*!
 * \brief FunctionWeightProxyModel::filterAcceptsRow
 * \param source_row
 * \param source_parent
 * only allow to display actual feature
 * \return
 */
bool FunctionWeightProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    //get and cast source model
    FunctionWeightsTableModel *source_model = dynamic_cast<FunctionWeightsTableModel *>(this->sourceModel());
    if(source_model == NULL){
        return false;
    }

    //check if job is set
    if(source_model->getCurrentJob().isNull()){
        return false;
    }

    //check feature and function valid
    if(!source_model->getCurrentJob()->getActiveFeature().isNull() &&
            source_model->getCurrentJob()->getActiveFeature()->getFeature()->getFunctions().size() >= 1){

        QPointer<Function> function = source_model->getCurrentJob()->getActiveFeature()->getFeature()->getFunctions().at(0);
        QList<InputElement> inputElem = function->getInputElements().value(0);

        QPointer<FeatureWrapper> fw = source_model->getCurrentJob()->getFeatureById(inputElem.at(source_row).id);

        if(!fw->getGeometry().isNull() && !fw->getGeometry()->getIsNominal()){
            return true;
        }
    }
    return false;
}

/*!
 * \brief FunctionWeightProxyModel::filterAcceptsColumn
 * allows to display all columns in the table view
 * \param source_column
 * \param source_parent
 * \return
 */
bool FunctionWeightProxyModel::filterAcceptsColumn(int source_column, const QModelIndex &source_parent) const
{
    return true;
}

/*!
 * \brief FunctionWeightProxyModel::lessThan
 * \param source_left
 * \param source_right
 * \return
 */
bool FunctionWeightProxyModel::lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const
{

    //get and cast source model
    FunctionWeightsTableModel *source_model = dynamic_cast<FunctionWeightsTableModel *>(this->sourceModel());
    if(source_model == NULL){
        return false;
    }

    //check if job is set
    if(source_model->getCurrentJob().isNull()){
        return false;
    }

    //check feature and function valid
    if(!source_model->getCurrentJob()->getActiveFeature().isNull() &&
            source_model->getCurrentJob()->getActiveFeature()->getFeature()->getFunctions().size() >= 1){

        QPointer<Function> function = source_model->getCurrentJob()->getActiveFeature()->getFeature()->getFunctions().at(0);
        QList<InputElement> inputElem = function->getInputElements().value(0);

        QPointer<FeatureWrapper> fwRight = source_model->getCurrentJob()->getFeatureById(inputElem.at(source_right.row()).id);
        QPointer<FeatureWrapper> fwLeft = source_model->getCurrentJob()->getFeatureById(inputElem.at(source_left.row()).id);

        return QString::localeAwareCompare(fwLeft->getFeature()->getFeatureName(), fwRight->getFeature()->getFeatureName()) > 0;
    }
    return false;
}
