#include "featuretabletreemodel.h"

/*!
 * \brief FeatureTableTreeModel::FeatureTableTreeModel
 * \param job
 * \param parent
 */
FeatureTableTreeModel::FeatureTableTreeModel(const QPointer<OiJob> &job, QObject *parent)
    : QAbstractItemModel(parent)
{
    rootItem = new FeatureItem(NULL);

    this->setCurrentJob(job);

    this->setupModelData();
}

/*!
 * \brief FeatureTableTreeModel::FeatureTableTreeModel
 * \param parent
 */
FeatureTableTreeModel::FeatureTableTreeModel(QObject *parent) : QAbstractItemModel(parent)
{
    rootItem = new FeatureItem(NULL);
}

/*!
 * \brief FeatureTableTreeModel::~FeatureTableTreeModel
 */
FeatureTableTreeModel::~FeatureTableTreeModel()
{
    delete rootItem;
}

/*!
 * \brief FeatureTableTreeModel::data
 * \param index
 * \param role
 * \return
 */
QVariant FeatureTableTreeModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid()){
        return QVariant();
    }

    FeatureItem *item = static_cast<FeatureItem*>(index.internalPointer());

    if(!item){
        return QVariant();
    }

    if(role == Qt::DisplayRole){
        return item->data(index.column());
    }else if(role == Qt::FontRole){

        if(item->parentItem() == rootItem){
            QFont font;
            font.setBold(true);
            return font;
        }

    }else if(role == Qt::CheckStateRole && item->getState() == eActualFeature){
        //TODO implement check states
    }

    return QVariant();
}

/*!
 * \brief FeatureTableTreeModel::flags
 * \param index
 * \return
 */
Qt::ItemFlags FeatureTableTreeModel::flags(const QModelIndex &index) const
{
    if(!index.isValid()){
        return 0;
    }
    return QAbstractItemModel::flags(index);
}

/*!
 * \brief FeatureTableTreeModel::headerData
 * \param section
 * \param orientation
 * \param role
 * \return
 */
QVariant FeatureTableTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if((Qt::DisplayRole == role) && (Qt::Horizontal == orientation) && (0 <= section) && (section < columnCount())){

        //get header
        QString header = getFeatureDisplayAttributeName(getFeatureDisplayAttributes().at(section));

        //add unit information to header
        if(getIsFeatureDisplayAttribute(getFeatureDisplayAttributes().at(section))){

            switch((FeatureDisplayAttributes)getFeatureDisplayAttributes().at(section)){
            case eFeatureDisplayStDev:
                header.append(QString(" %1").arg(getUnitTypeName(this->parameterDisplayConfig.getDisplayUnit(eMetric))));
                break;
            case eFeatureDisplayX:
                header.append(QString(" %1").arg(getUnitTypeName(this->parameterDisplayConfig.getDisplayUnit(eMetric))));
                break;
            case eFeatureDisplayY:
                header.append(QString(" %1").arg(getUnitTypeName(this->parameterDisplayConfig.getDisplayUnit(eMetric))));
                break;
            case eFeatureDisplayZ:
                header.append(QString(" %1").arg(getUnitTypeName(this->parameterDisplayConfig.getDisplayUnit(eMetric))));
                break;
            case eFeatureDisplayRadiusA:
                header.append(QString(" %1").arg(getUnitTypeName(this->parameterDisplayConfig.getDisplayUnit(eMetric))));
                break;
            case eFeatureDisplayRadiusB:
                header.append(QString(" %1").arg(getUnitTypeName(this->parameterDisplayConfig.getDisplayUnit(eMetric))));
                break;
            case eFeatureDisplayAperture:
                header.append(QString(" %1").arg(getUnitTypeName(this->parameterDisplayConfig.getDisplayUnit(eAngular))));
                break;
            case eFeatureDisplayA:
                header.append(QString(" %1").arg(getUnitTypeName(this->parameterDisplayConfig.getDisplayUnit(eMetric))));
                break;
            case eFeatureDisplayB:
                header.append(QString(" %1").arg(getUnitTypeName(this->parameterDisplayConfig.getDisplayUnit(eMetric))));
                break;
            case eFeatureDisplayC:
                header.append(QString(" %1").arg(getUnitTypeName(this->parameterDisplayConfig.getDisplayUnit(eMetric))));
                break;
            case eFeatureDisplayAngle:
                header.append(QString(" %1").arg(getUnitTypeName(this->parameterDisplayConfig.getDisplayUnit(eAngular))));
                break;
            case eFeatureDisplayDistance:
                header.append(QString(" %1").arg(getUnitTypeName(this->parameterDisplayConfig.getDisplayUnit(eMetric))));
                break;
            case eFeatureDisplayTemperature:
                header.append(QString(" %1").arg(getUnitTypeName(this->parameterDisplayConfig.getDisplayUnit(eTemperature))));
                break;
            case eFeatureDisplayLength:
                header.append(QString(" %1").arg(getUnitTypeName(this->parameterDisplayConfig.getDisplayUnit(eMetric))));
                break;
            case eFeatureDisplayExpansionOriginX:
                header.append(QString(" %1").arg(getUnitTypeName(this->parameterDisplayConfig.getDisplayUnit(eMetric))));
                break;
            case eFeatureDisplayExpansionOriginY:
                header.append(QString(" %1").arg(getUnitTypeName(this->parameterDisplayConfig.getDisplayUnit(eMetric))));
                break;
            case eFeatureDisplayExpansionOriginZ:
                header.append(QString(" %1").arg(getUnitTypeName(this->parameterDisplayConfig.getDisplayUnit(eMetric))));
                break;
            }

        }else if(getIsTrafoParamDisplayAttribute(getFeatureDisplayAttributes().at(section))){

            switch((TrafoParamDisplayAttributes)getFeatureDisplayAttributes().at(section)){
            case eTrafoParamDisplayStDev:
                header.append(QString(" %1").arg(getUnitTypeName(this->parameterDisplayConfig.getDisplayUnit(eMetric))));
                break;
            case eTrafoParamDisplayTranslationX:
                header.append(QString(" %1").arg(getUnitTypeName(this->parameterDisplayConfig.getDisplayUnit(eMetric))));
                break;
            case eTrafoParamDisplayTranslationY:
                header.append(QString(" %1").arg(getUnitTypeName(this->parameterDisplayConfig.getDisplayUnit(eMetric))));
                break;
            case eTrafoParamDisplayTranslationZ:
                header.append(QString(" %1").arg(getUnitTypeName(this->parameterDisplayConfig.getDisplayUnit(eMetric))));
                break;
            case eTrafoParamDisplayRotationX:
                header.append(QString(" %1").arg(getUnitTypeName(this->parameterDisplayConfig.getDisplayUnit(eAngular))));
                break;
            case eTrafoParamDisplayRotationY:
                header.append(QString(" %1").arg(getUnitTypeName(this->parameterDisplayConfig.getDisplayUnit(eAngular))));
                break;
            case eTrafoParamDisplayRotationZ:
                header.append(QString(" %1").arg(getUnitTypeName(this->parameterDisplayConfig.getDisplayUnit(eAngular))));
                break;
            }
        }
        return header;
    }
    return QVariant();

}

/*!
 * \brief FeatureTableTreeModel::index
 * \param row
 * \param column
 * \param parent
 * \return
 */
QModelIndex FeatureTableTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if(!hasIndex(row, column, parent)){
        return QModelIndex();
    }

    FeatureItem *parentItem= NULL;
    if(!parent.isValid()){
        parentItem = rootItem;
    }else{
        parentItem = static_cast<FeatureItem*>(parent.internalPointer());
    }
    if(parentItem != NULL){
        FeatureItem *childItem = parentItem->child(row);
        if(childItem){
            return createIndex(row, column, childItem);
        }else{
            return QModelIndex();
        }
    }
    return QModelIndex();
}

/*!
 * \brief FeatureTableTreeModel::parent
 * \param child
 * \return
 */
QModelIndex FeatureTableTreeModel::parent(const QModelIndex &child) const
{
    if(!child.isValid()){
        return QModelIndex();
    }

    /*FeatureItem *childItem = static_cast<FeatureItem*>(child.internalPointer());
    FeatureItem *parentItem = childItem->parentItem();

    if(parentItem == rootItem){
        return QModelIndex();
    }
    return createIndex(parentItem->row(), 0, parentItem);*/

    FeatureItem *childItem = static_cast<FeatureItem*>(child.internalPointer());
    if(childItem != NULL){
        FeatureItem *parentItem = childItem->parentItem();
        if (parentItem == NULL || parentItem == rootItem)
            return QModelIndex();
        return createIndex(parentItem->row(), 0, parentItem);
    }else{
        return QModelIndex();
    }
}

/*!
 * \brief FeatureTableTreeModel::rowCount
 * \param parent
 * \return
 */
int FeatureTableTreeModel::rowCount(const QModelIndex &parent) const
{
    FeatureItem *parentItem;
    if(parent.column() > 0){
        return 0;
    }
    if(!parent.isValid()){
        parentItem = rootItem;
    }else{
        parentItem = static_cast<FeatureItem*>(parent.internalPointer());
    }
    if(parentItem == NULL){
        return 0;
    }
    return parentItem->childCount();
}

/*!
 * \brief FeatureTableTreeModel::columnCount
 * \param parent
 * \return
 */
int FeatureTableTreeModel::columnCount(const QModelIndex &parent) const
{
    if(parent.isValid()){
        return static_cast<FeatureItem*>(parent.internalPointer())->columnCount();
    }else if(rootItem != NULL){
        return rootItem->columnCount();
    }else{
        return 0;
    }
}

/*!
 * \brief FeatureTableTreeModel::getCurrentJob
 * \return
 */
const QPointer<OiJob> &FeatureTableTreeModel::getCurrentJob() const
{
    return this->currentJob;
}

/*!
 * \brief FeatureTableTreeModel::setCurrentJob
 * \param job
 */
void FeatureTableTreeModel::setCurrentJob(const QPointer<OiJob> &job)
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
 * \brief FeatureTableTreeModel::getRootItem
 * \return
 */
FeatureItem *FeatureTableTreeModel::getRootItem()
{
    return rootItem;
}

/*!
 * \brief FeatureTableTreeModel::setupModelData
 */
void FeatureTableTreeModel::setupModelData()
{
    if(rootItem == NULL){
        return;
    }

    //start resetting the model
    emit this->beginResetModel();

    //delete the old tree hierarchy
    this->rootItem->deleteChildren();

    foreach (QPointer<FeatureWrapper> feature, this->currentJob->getFeaturesList()) {

        switch (feature->getFeatureTypeEnum()) {
        case eMasterGeometryFeature:{
            FeatureItem *fw = new FeatureItem(feature, eActualFeature);

            bool act = false;
            bool nom = false;
            if(!feature->getMasterGeometry()->getActual().isNull()){
                act = true;
            }

            //solved state für nominals durchlaufen um aktives nominal zu finden
            if(feature->getMasterGeometry()->getNominals().size()> 0){
                foreach (QPointer<Geometry> geom, feature->getMasterGeometry()->getNominals()) {
                    if(geom->getIsSolved()){
                        FeatureItem *fwNom = new FeatureItem(feature,eNominalFeature);
                        fw->appendChild(fwNom);
                        nom = true;
                    }
                }
            }
            if(act && nom){
                FeatureItem *fwActNom = new FeatureItem(feature, eDifferenceFeature);
                fw->appendChild(fwActNom);
            }
            rootItem->appendChild(fw);
            break;
        }default:{
            FeatureItem *fw = new FeatureItem(feature,eNoMasterGeometry);
            rootItem->appendChild(fw);
            break;
        }
        }
    }
    //reset finished
    emit this->endResetModel();
}

/*!
 * \brief FeatureTableTreeModel::updateModel
 */
void FeatureTableTreeModel::updateModel()
{
    this->setupModelData();
    emit this->layoutAboutToBeChanged();
    emit this->layoutChanged();
}

/*!
 * \brief FeatureTableTreeModel::connectJob
 */
void FeatureTableTreeModel::connectJob()
{
    QObject::connect(this->currentJob.data(), &OiJob::featureSetChanged, this, &FeatureTableTreeModel::updateModel, Qt::AutoConnection);
    QObject::connect(this->currentJob.data(), &OiJob::activeCoordinateSystemChanged, this, &FeatureTableTreeModel::updateModel, Qt::AutoConnection);
    QObject::connect(this->currentJob.data(), &OiJob::activeFeatureChanged, this, &FeatureTableTreeModel::updateModel, Qt::AutoConnection);
    QObject::connect(this->currentJob.data(), &OiJob::activeStationChanged, this, &FeatureTableTreeModel::updateModel, Qt::AutoConnection);
    QObject::connect(this->currentJob.data(), &OiJob::featureAttributesChanged, this, &FeatureTableTreeModel::updateModel, Qt::AutoConnection);
    QObject::connect(this->currentJob.data(), &OiJob::featureRecalculated, this, &FeatureTableTreeModel::updateModel, Qt::AutoConnection);
    QObject::connect(this->currentJob.data(), &OiJob::featuresRecalculated, this, &FeatureTableTreeModel::updateModel, Qt::AutoConnection);
    QObject::connect(this->currentJob.data(), &OiJob::geometryMeasurementConfigChanged, this, &FeatureTableTreeModel::updateModel, Qt::AutoConnection);
    QObject::connect(this->currentJob.data(), &OiJob::activeGroupChanged, this, &FeatureTableTreeModel::updateModel, Qt::AutoConnection);
    QObject::connect(this->currentJob.data(), &OiJob::geometryIsCommonChanged, this, &FeatureTableTreeModel::updateModel, Qt::AutoConnection);
}

/*!
 * \brief FeatureTableTreeModel::disconnectJob
 */
void FeatureTableTreeModel::disconnectJob()
{
    QObject::disconnect(this->currentJob.data(), &OiJob::featureSetChanged, this, &FeatureTableTreeModel::updateModel);
    QObject::disconnect(this->currentJob.data(), &OiJob::activeCoordinateSystemChanged, this, &FeatureTableTreeModel::updateModel);
    QObject::disconnect(this->currentJob.data(), &OiJob::activeFeatureChanged, this, &FeatureTableTreeModel::updateModel);
    QObject::disconnect(this->currentJob.data(), &OiJob::activeStationChanged, this, &FeatureTableTreeModel::updateModel);
    QObject::disconnect(this->currentJob.data(), &OiJob::featureAttributesChanged, this, &FeatureTableTreeModel::updateModel);
    QObject::disconnect(this->currentJob.data(), &OiJob::featureRecalculated, this, &FeatureTableTreeModel::updateModel);
    QObject::disconnect(this->currentJob.data(), &OiJob::featuresRecalculated, this, &FeatureTableTreeModel::updateModel);
    QObject::disconnect(this->currentJob.data(), &OiJob::geometryMeasurementConfigChanged, this, &FeatureTableTreeModel::updateModel);
    QObject::disconnect(this->currentJob.data(), &OiJob::activeGroupChanged, this, &FeatureTableTreeModel::updateModel);
    QObject::disconnect(this->currentJob.data(), &OiJob::geometryIsCommonChanged, this, &FeatureTableTreeModel::updateModel);
}
