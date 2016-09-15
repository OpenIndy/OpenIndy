#include "featureitem.h"

/*!
 * \brief FeatureItem::FeatureItem
 * \param data
 * \param state
 * \param parantItem
 */
FeatureItem::FeatureItem(const QPointer<FeatureWrapper> &data, FeatureViewState state, FeatureItem *parantItem)
{
    m_itemData = data;
    m_state = state;
}

/*!
 * \brief FeatureItem::FeatureItem
 * \param parentItem
 */
FeatureItem::FeatureItem(FeatureItem *parentItem)
{
    m_parentItem = parentItem;
}

/*!
 * \brief FeatureItem::~FeatureItem
 */
FeatureItem::~FeatureItem()
{
    qDeleteAll(m_childItems);
}

/*!
 * \brief FeatureItem::appendChild
 * \param child
 */
void FeatureItem::appendChild(FeatureItem *child)
{
    m_childItems.append(child);
}

/*!
 * \brief FeatureItem::setData
 * \param data
 */
void FeatureItem::setData(const QPointer<FeatureWrapper> &data, FeatureViewState state)
{
    m_itemData = data;
    m_state = state;
}

/*!
 * \brief FeatureItem::child
 * \param row
 * \return
 */
FeatureItem *FeatureItem::child(int row)
{
    return m_childItems.value(row);
}

/*!
 * \brief FeatureItem::childCount
 * \return
 */
int FeatureItem::childCount() const
{
    return m_childItems.count();
}

/*!
 * \brief FeatureItem::columnCount
 * \return
 */
int FeatureItem::columnCount() const
{
    //return m_itemData.count();
    return getFeatureDisplayAttributes().size();
}

/*!
 * \brief FeatureItem::data
 * \param column
 * \return
 */
QVariant FeatureItem::data(int column)
{

    if(m_itemData.isNull()){
        return QVariant();
    }

    ////////////////////////////////
    //header for master geometries//
    ////////////////////////////////
    if(m_state == eMasterGeomHeader){

        switch (column) {
        case 0:
            return m_itemData->getFeature()->getDisplayType();
            break;
        case 1:
            return m_itemData->getFeature()->getFeatureName();
            break;
        case 3:
            return m_itemData->getFeature()->getGroupName();
            break;
        case 10:
            return m_itemData->getMasterGeometry()->getMeasurementConfig().getName();
            break;
        default:
            return QVariant();
            break;
        }

     ///////////////////////
    //actuals and nominals//
    ////////////////////////
    }else if(m_state == eActualFeature || m_state == eNominalFeature){
        switch (column) {
        case 0:
            return m_itemData->getFeature()->getFeatureName();
            break;
        case 1:
            return m_itemData->getFeature()->getFeatureName();
            break;
        case 2:
            return m_itemData->getFeature()->getComment();
            break;
        case 3:
            return m_itemData->getFeature()->getGroupName();
            break;
        case 4:
            return m_itemData->getFeature()->getIsSolved();
            break;
        case 5:
            return m_itemData->getFeature()->getDisplayIsUpdated();
            break;
        case 6:
            return m_itemData->getFeature()->getDisplayFunctions();
            break;
        case 7:
            return m_itemData->getFeature()->getDisplayUsedFor();
            break;
        case 8:
            return m_itemData->getFeature()->getDisplayPreviouslyNeeded();
            break;
        case 9:
            return m_itemData->getFeature()->getDisplayStDev(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                             this->parameterDisplayConfig.getDisplayDigits(eMetric));
            break;
        case 10:
            return QVariant(); //only master geom has measurement config
            break;
        case 11:
            return m_itemData->getFeature()->getDisplayObservations();
            break;
        case 12:
            return m_itemData->getFeature()->getDisplayX(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                         this->parameterDisplayConfig.getDisplayDigits(eMetric));
            break;
        case 13:
            return m_itemData->getFeature()->getDisplayY(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                         this->parameterDisplayConfig.getDisplayDigits(eMetric));
            break;
        case 14:
            return m_itemData->getFeature()->getDisplayZ(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                         this->parameterDisplayConfig.getDisplayDigits(eMetric));
            break;
        case 15:
            return m_itemData->getFeature()->getDisplayPrimaryI(this->parameterDisplayConfig.getDisplayDigits(eDimensionless));
            break;
        case 16:
            return m_itemData->getFeature()->getDisplayPrimaryJ(this->parameterDisplayConfig.getDisplayDigits(eDimensionless));
            break;
        case 17:
            return m_itemData->getFeature()->getDisplayPrimaryK(this->parameterDisplayConfig.getDisplayDigits(eDimensionless));
            break;
        case 18:
            return m_itemData->getFeature()->getDisplayRadiusA(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                               this->parameterDisplayConfig.getDisplayDigits(eMetric));
            break;
        case 19:
            return m_itemData->getFeature()->getDisplayRadiusB(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                               this->parameterDisplayConfig.getDisplayDigits(eMetric));
            break;
        case 20:
            return m_itemData->getFeature()->getDisplaySecondaryI(this->parameterDisplayConfig.getDisplayDigits(eDimensionless));
            break;
        case 21:
            return m_itemData->getFeature()->getDisplaySecondaryJ(this->parameterDisplayConfig.getDisplayDigits(eDimensionless));
            break;
        case 22:
            return m_itemData->getFeature()->getDisplaySecondaryK(this->parameterDisplayConfig.getDisplayDigits(eDimensionless));
            break;
        case 23:
            return m_itemData->getFeature()->getDisplayAperture(this->parameterDisplayConfig.getDisplayUnit(eAngular),
                                                                this->parameterDisplayConfig.getDisplayDigits(eAngular));
            break;
        case 24:
            return m_itemData->getFeature()->getDisplayA(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                         this->parameterDisplayConfig.getDisplayDigits(eMetric));
            break;
        case 25:
            return m_itemData->getFeature()->getDisplayB(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                        this->parameterDisplayConfig.getDisplayDigits(eMetric));
            break;
        case 26:
            return m_itemData->getFeature()->getDisplayC(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                        this->parameterDisplayConfig.getDisplayDigits(eMetric));
            break;
        case 27:
            return m_itemData->getFeature()->getDisplayAngle(this->parameterDisplayConfig.getDisplayUnit(eAngular),
                                                             this->parameterDisplayConfig.getDisplayDigits(eAngular));
            break;
        case 28:
            return m_itemData->getFeature()->getDisplayDistance(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                                this->parameterDisplayConfig.getDisplayDigits(eMetric));
            break;
        case 29:
            return m_itemData->getFeature()->getDisplayMeasurementSeries(this->parameterDisplayConfig.getDisplayUnit(eDimensionless),
                                                                         this->parameterDisplayConfig.getDisplayDigits(eDimensionless));
            break;
        case 30:
            return m_itemData->getFeature()->getDisplayTemperature(this->parameterDisplayConfig.getDisplayUnit(eTemperature),
                                                                   this->parameterDisplayConfig.getDisplayDigits(eTemperature));
            break;
        case 31:
            return m_itemData->getFeature()->getDisplayLength(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                             this->parameterDisplayConfig.getDisplayDigits(eMetric));
            break;
        case 32:
            return m_itemData->getFeature()->getDisplayExpansionOriginX(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                                        this->parameterDisplayConfig.getDisplayDigits(eMetric));
            break;
        case 33:
            return m_itemData->getFeature()->getDisplayExpansionOriginY(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                                        this->parameterDisplayConfig.getDisplayDigits(eMetric));
            break;
        case 34:
            return m_itemData->getFeature()->getDisplayExpansionOriginZ(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                                        this->parameterDisplayConfig.getDisplayDigits(eMetric));
            break;
        default:
            return QVariant();
            break;
        }

    /////////////////////////////////////
    //differences for master geometries//
    /////////////////////////////////////
    }else if(m_state == eDifferenceFeature){
        switch (column) {
        case 0:
            return m_itemData->getFeature()->getFeatureName();
            break;
        case 1:
            return m_itemData->getFeature()->getFeatureName();
            break;
        case 2:
            return m_itemData->getFeature()->getComment();
            break;
        case 3:
            return m_itemData->getFeature()->getGroupName();
            break;
        case 4:
            return m_itemData->getFeature()->getIsSolved();
            break;
        case 5:
            return m_itemData->getFeature()->getDisplayIsUpdated();
            break;
        case 6:
            return m_itemData->getFeature()->getDisplayFunctions();
            break;
        case 7:
            return m_itemData->getFeature()->getDisplayUsedFor();
            break;
        case 8:
            return m_itemData->getFeature()->getDisplayPreviouslyNeeded();
            break;
        case 9:
            return m_itemData->getFeature()->getDisplayStDev(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                             this->parameterDisplayConfig.getDisplayDigits(eMetric));
            break;
        case 10:
            return QVariant(); //only master geom has measurement config
            break;
        case 11:
            return m_itemData->getFeature()->getDisplayObservations();
            break;
        case 12:
            return m_itemData->getFeature()->getDisplayX(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                         this->parameterDisplayConfig.getDisplayDigits(eMetric));
            break;
        case 13:
            return m_itemData->getFeature()->getDisplayY(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                         this->parameterDisplayConfig.getDisplayDigits(eMetric));
            break;
        case 14:
            return m_itemData->getFeature()->getDisplayZ(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                         this->parameterDisplayConfig.getDisplayDigits(eMetric));
            break;
        case 15:
            return m_itemData->getFeature()->getDisplayPrimaryI(this->parameterDisplayConfig.getDisplayDigits(eDimensionless));
            break;
        case 16:
            return m_itemData->getFeature()->getDisplayPrimaryJ(this->parameterDisplayConfig.getDisplayDigits(eDimensionless));
            break;
        case 17:
            return m_itemData->getFeature()->getDisplayPrimaryK(this->parameterDisplayConfig.getDisplayDigits(eDimensionless));
            break;
        case 18:
            return m_itemData->getFeature()->getDisplayRadiusA(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                               this->parameterDisplayConfig.getDisplayDigits(eMetric));
            break;
        case 19:
            return m_itemData->getFeature()->getDisplayRadiusB(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                               this->parameterDisplayConfig.getDisplayDigits(eMetric));
            break;
        case 20:
            return m_itemData->getFeature()->getDisplaySecondaryI(this->parameterDisplayConfig.getDisplayDigits(eDimensionless));
            break;
        case 21:
            return m_itemData->getFeature()->getDisplaySecondaryJ(this->parameterDisplayConfig.getDisplayDigits(eDimensionless));
            break;
        case 22:
            return m_itemData->getFeature()->getDisplaySecondaryK(this->parameterDisplayConfig.getDisplayDigits(eDimensionless));
            break;
        case 23:
            return m_itemData->getFeature()->getDisplayAperture(this->parameterDisplayConfig.getDisplayUnit(eAngular),
                                                                this->parameterDisplayConfig.getDisplayDigits(eAngular));
            break;
        case 24:
            return m_itemData->getFeature()->getDisplayA(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                         this->parameterDisplayConfig.getDisplayDigits(eMetric));
            break;
        case 25:
            return m_itemData->getFeature()->getDisplayB(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                        this->parameterDisplayConfig.getDisplayDigits(eMetric));
            break;
        case 26:
            return m_itemData->getFeature()->getDisplayC(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                        this->parameterDisplayConfig.getDisplayDigits(eMetric));
            break;
        case 27:
            return m_itemData->getFeature()->getDisplayAngle(this->parameterDisplayConfig.getDisplayUnit(eAngular),
                                                             this->parameterDisplayConfig.getDisplayDigits(eAngular));
            break;
        case 28:
            return m_itemData->getFeature()->getDisplayDistance(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                                this->parameterDisplayConfig.getDisplayDigits(eMetric));
            break;
        case 29:
            return m_itemData->getFeature()->getDisplayMeasurementSeries(this->parameterDisplayConfig.getDisplayUnit(eDimensionless),
                                                                         this->parameterDisplayConfig.getDisplayDigits(eDimensionless));
            break;
        case 30:
            return m_itemData->getFeature()->getDisplayTemperature(this->parameterDisplayConfig.getDisplayUnit(eTemperature),
                                                                   this->parameterDisplayConfig.getDisplayDigits(eTemperature));
            break;
        case 31:
            return m_itemData->getFeature()->getDisplayLength(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                             this->parameterDisplayConfig.getDisplayDigits(eMetric));
            break;
        case 32:
            return m_itemData->getFeature()->getDisplayExpansionOriginX(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                                        this->parameterDisplayConfig.getDisplayDigits(eMetric));
            break;
        case 33:
            return m_itemData->getFeature()->getDisplayExpansionOriginY(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                                        this->parameterDisplayConfig.getDisplayDigits(eMetric));
            break;
        case 34:
            return m_itemData->getFeature()->getDisplayExpansionOriginZ(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                                        this->parameterDisplayConfig.getDisplayDigits(eMetric));
            break;
        default:
            return QVariant();
            break;
        }
    }

    return QVariant();
}

/*!
 * \brief FeatureItem::row
 * \return
 */
int FeatureItem::row() const
{
    if(m_parentItem){
        return m_parentItem->m_childItems.indexOf(const_cast<FeatureItem*>(this));
    }
    return 0;
}

/*!
 * \brief FeatureItem::parentItem
 * \return
 */
FeatureItem *FeatureItem::parentItem()
{
    return m_parentItem;
}
