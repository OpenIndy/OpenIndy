#include "featuretabletreemodel.h"

/*!
 * \brief FeatureTableTreeModel::FeatureTableTreeModel
 * \param job
 * \param parent
 */
FeatureTableTreeModel::FeatureTableTreeModel(const QPointer<OiJob> &job, QObject *parent)
    : QAbstractItemModel(parent)
{
    this->currentJob = job;

    rootItem = new FeatureItem();
    this->setupModelData(rootItem);
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
    if(role != Qt::DisplayRole){
        return QVariant();
    }

    //Datenlogik implementieren
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

    FeatureItem *parentItem;
    if(!parent.isValid()){
        parentItem = rootItem;
    }else{
        parentItem = static_cast<FeatureItem*>(parent.internalPointer());
    }

    FeatureItem *childItem = parentItem->child(row);
    if(childItem){
        return createIndex(row, column, childItem);
    }else{
        return QModelIndex();
    }
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

    FeatureItem *childItem = static_cast<FeatureItem*>(child.internalPointer());
    FeatureItem *parentItem = childItem->parentItem();

    if(parentItem == rootItem){
        return QModelIndex();
    }

    return createIndex(parentItem->row(), 0, parentItem);
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
    }else{
        return rootItem->columnCount();
    }
}

/*!
 * \brief FeatureTableTreeModel::setupModelData
 */
void FeatureTableTreeModel::setupModelData(FeatureItem *parent)
{
    if(parent == NULL){
        return;
    }

    foreach (QPointer<FeatureWrapper> feature, this->currentJob->getFeaturesList()) {

        switch (feature->getFeatureTypeEnum()) {
        case eMasterGeometryFeature:{
            FeatureItem *fw = new FeatureItem(feature, eMasterGeomHeader);

            bool act = false;
            bool nom = false;
            if(!feature->getMasterGeometry()->getActual().isNull()){
                FeatureItem *fwAct = new FeatureItem(feature->getMasterGeometry()->getActual()->getFeatureWrapper(), eActualFeature);
            }
            if(feature->getMasterGeometry()->getNominals().size()> 0){
                FeatureItem *fwNom = new FeatureItem(feature->getMasterGeometry()->getNominals().first()->getFeatureWrapper(),eNominalFeature);
            }
            if(act && nom){
                FeatureItem *fwActNom = new FeatureItem(feature, eDifferenceFeature);
            }
            break;
        }default:{
            FeatureItem *fw = new FeatureItem(feature,eActualFeature);
            parent->appendChild(fw);
            break;
        }
        }
    }
}
