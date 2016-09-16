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
    m_parentItem = parantItem;
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
    child->m_parentItem = this;
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

    //get the display attribute
    int attr = getFeatureDisplayAttributes().at(column);

    if(!getIsFeatureDisplayAttribute(attr)){
        return QVariant();
    }

    ////////////////////////////////
    //header for master geometries//
    ////////////////////////////////
    if(m_state == eNoMasterGeometry){

        switch ((FeatureDisplayAttributes)attr) {
        case eFeatureDisplayType:
            return m_itemData->getFeature()->getDisplayType();
            break;
        case eFeatureDisplayName:
            return m_itemData->getFeature()->getFeatureName();
            break;
        case eFeatureDisplayComment:
            return m_itemData->getFeature()->getComment();
            break;
        case eFeatureDisplayGroup:
            return m_itemData->getFeature()->getGroupName();
            break;
        case eFeatureDisplayIsSolved:
            return m_itemData->getFeature()->getIsSolved();
            break;
        case eFeatureDisplayIsUpdated:
            return m_itemData->getFeature()->getDisplayIsUpdated();
            break;
        case eFeatureDisplayFunctions:
            return m_itemData->getFeature()->getDisplayFunctions();
            break;
        case eFeatureDisplayUsedFor:
            return m_itemData->getFeature()->getDisplayUsedFor();
            break;
        case eFeatureDisplayPreviouslyNeeded:
            return m_itemData->getFeature()->getDisplayPreviouslyNeeded();
            break;
        case eFeatureDisplayStDev:
            return m_itemData->getFeature()->getDisplayStDev(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                             this->parameterDisplayConfig.getDisplayDigits(eMetric));
            break;
        case eFeatureDisplayMeasurementConfig:
            return m_itemData->getFeature()->getDisplayMeasurementConfig();
            break;
        case eFeatureDisplayObservations:
            return m_itemData->getFeature()->getDisplayObservations();
            break;
        case eFeatureDisplayX:
            return m_itemData->getFeature()->getDisplayX(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                         this->parameterDisplayConfig.getDisplayDigits(eMetric));
            break;
        case eFeatureDisplayY:
            return m_itemData->getFeature()->getDisplayY(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                         this->parameterDisplayConfig.getDisplayDigits(eMetric));
            break;
        case eFeatureDisplayZ:
            return m_itemData->getFeature()->getDisplayZ(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                         this->parameterDisplayConfig.getDisplayDigits(eMetric));
            break;
        case eFeatureDisplayPrimaryI:
            return m_itemData->getFeature()->getDisplayPrimaryI(this->parameterDisplayConfig.getDisplayDigits(eDimensionless));
            break;
        case eFeatureDisplayPrimaryJ:
            return m_itemData->getFeature()->getDisplayPrimaryJ(this->parameterDisplayConfig.getDisplayDigits(eDimensionless));
            break;
        case eFeatureDisplayPrimaryK:
            return m_itemData->getFeature()->getDisplayPrimaryK(this->parameterDisplayConfig.getDisplayDigits(eDimensionless));
            break;
        case eFeatureDisplayRadiusA:
            return m_itemData->getFeature()->getDisplayRadiusA(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                               this->parameterDisplayConfig.getDisplayDigits(eMetric));
            break;
        case eFeatureDisplayRadiusB:
            return m_itemData->getFeature()->getDisplayRadiusB(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                               this->parameterDisplayConfig.getDisplayDigits(eMetric));
            break;
        case eFeatureDisplaySecondaryI:
            return m_itemData->getFeature()->getDisplaySecondaryI(this->parameterDisplayConfig.getDisplayDigits(eDimensionless));
            break;
        case eFeatureDisplaySecondaryJ:
            return m_itemData->getFeature()->getDisplaySecondaryJ(this->parameterDisplayConfig.getDisplayDigits(eDimensionless));
            break;
        case eFeatureDisplaySecondaryK:
            return m_itemData->getFeature()->getDisplaySecondaryK(this->parameterDisplayConfig.getDisplayDigits(eDimensionless));
            break;
        case eFeatureDisplayAperture:
            return m_itemData->getFeature()->getDisplayAperture(this->parameterDisplayConfig.getDisplayUnit(eAngular),
                                                                this->parameterDisplayConfig.getDisplayDigits(eAngular));
            break;
        case eFeatureDisplayA:
            return m_itemData->getFeature()->getDisplayA(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                         this->parameterDisplayConfig.getDisplayDigits(eMetric));
            break;
        case eFeatureDisplayB:
            return m_itemData->getFeature()->getDisplayB(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                        this->parameterDisplayConfig.getDisplayDigits(eMetric));
            break;
        case eFeatureDisplayC:
            return m_itemData->getFeature()->getDisplayC(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                        this->parameterDisplayConfig.getDisplayDigits(eMetric));
            break;
        case eFeatureDisplayAngle:
            return m_itemData->getFeature()->getDisplayAngle(this->parameterDisplayConfig.getDisplayUnit(eAngular),
                                                             this->parameterDisplayConfig.getDisplayDigits(eAngular));
            break;
        case eFeatureDisplayDistance:
            return m_itemData->getFeature()->getDisplayDistance(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                                this->parameterDisplayConfig.getDisplayDigits(eMetric));
            break;
        case eFeatureDisplayMeasurementSeries:
            return m_itemData->getFeature()->getDisplayMeasurementSeries(this->parameterDisplayConfig.getDisplayUnit(eDimensionless),
                                                                         this->parameterDisplayConfig.getDisplayDigits(eDimensionless));
            break;
        case eFeatureDisplayTemperature:
            return m_itemData->getFeature()->getDisplayTemperature(this->parameterDisplayConfig.getDisplayUnit(eTemperature),
                                                                   this->parameterDisplayConfig.getDisplayDigits(eTemperature));
            break;
        case eFeatureDisplayLength:
            return m_itemData->getFeature()->getDisplayLength(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                             this->parameterDisplayConfig.getDisplayDigits(eMetric));
            break;
        case eFeatureDisplayExpansionOriginX:
            return m_itemData->getFeature()->getDisplayExpansionOriginX(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                                        this->parameterDisplayConfig.getDisplayDigits(eMetric));
            break;
        case eFeatureDisplayExpansionOriginY:
            return m_itemData->getFeature()->getDisplayExpansionOriginY(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                                        this->parameterDisplayConfig.getDisplayDigits(eMetric));
            break;
        case eFeatureDisplayExpansionOriginZ:
            return m_itemData->getFeature()->getDisplayExpansionOriginZ(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                                        this->parameterDisplayConfig.getDisplayDigits(eMetric));
            break;
        default:
            return QVariant();
            break;
        }

    ////////////////////////
    //actuals of mastergeom//
    ////////////////////////
    }else if(m_state == eActualFeature){
        switch ((FeatureDisplayAttributes)attr) {
        case eFeatureDisplayType:
            if(!m_itemData->getMasterGeometry()->getActual().isNull()){
                return m_itemData->getMasterGeometry()->getActual()->getDisplayType();
            }
            return m_itemData->getMasterGeometry()->getNominals().first()->getDisplayType();
            break;
        case eFeatureDisplayName:
            return m_itemData->getMasterGeometry()->getFeatureName();
            break;
        case eFeatureDisplayComment:
            if(!m_itemData->getMasterGeometry()->getActual().isNull()){
                return m_itemData->getMasterGeometry()->getActual()->getComment();
            }
            return QVariant();
            break;
        case eFeatureDisplayGroup:
            return m_itemData->getMasterGeometry()->getGroupName();
            break;
        case eFeatureDisplayIsSolved:
            if(!m_itemData->getMasterGeometry()->getActual().isNull()){
                return m_itemData->getMasterGeometry()->getActual()->getIsSolved();
            }
            return QVariant();
            break;
        case eFeatureDisplayIsUpdated:
            if(!m_itemData->getMasterGeometry()->getActual().isNull()){
                return m_itemData->getMasterGeometry()->getActual()->getDisplayIsUpdated();
            }
            return QVariant();
            break;
        case eFeatureDisplayFunctions:
            if(!m_itemData->getMasterGeometry()->getActual().isNull()){
                return m_itemData->getMasterGeometry()->getActual()->getDisplayFunctions();
            }
            return QVariant();
            break;
        case eFeatureDisplayUsedFor:
            if(!m_itemData->getMasterGeometry()->getActual().isNull()){
                return m_itemData->getMasterGeometry()->getActual()->getDisplayUsedFor();
            }
            return QVariant();
            break;
        case eFeatureDisplayPreviouslyNeeded:
            if(!m_itemData->getMasterGeometry()->getActual().isNull()){
                return m_itemData->getMasterGeometry()->getActual()->getDisplayPreviouslyNeeded();
            }
            return QVariant();
            break;
        case eFeatureDisplayStDev:
            if(!m_itemData->getMasterGeometry()->getActual().isNull()){
                return m_itemData->getMasterGeometry()->getActual()->getDisplayStDev(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                                                     this->parameterDisplayConfig.getDisplayDigits(eMetric));
            }
            return QVariant();
            break;
        case eFeatureDisplayMeasurementConfig:
            return m_itemData->getMasterGeometry()->getDisplayMeasurementConfig();
            break;
        case eFeatureDisplayObservations:
            if(!m_itemData->getMasterGeometry()->getActual().isNull()){
                return m_itemData->getMasterGeometry()->getActual()->getDisplayObservations();
            }
            return QVariant();
            break;
        case eFeatureDisplayX:
            if(!m_itemData->getMasterGeometry()->getActual().isNull()){
                return m_itemData->getMasterGeometry()->getActual()->getDisplayX(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                                                 this->parameterDisplayConfig.getDisplayDigits(eMetric));
            }
            return QVariant();
            break;
        case eFeatureDisplayY:
            if(!m_itemData->getMasterGeometry()->getActual().isNull()){
                return m_itemData->getMasterGeometry()->getActual()->getDisplayY(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                                                 this->parameterDisplayConfig.getDisplayDigits(eMetric));
            }
            return QVariant();
            break;
        case eFeatureDisplayZ:
            if(!m_itemData->getMasterGeometry()->getActual().isNull()){
                return m_itemData->getMasterGeometry()->getActual()->getDisplayZ(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                                                 this->parameterDisplayConfig.getDisplayDigits(eMetric));
            }
            return QVariant();
            break;
        case eFeatureDisplayPrimaryI:
            if(!m_itemData->getMasterGeometry()->getActual().isNull()){
                return m_itemData->getMasterGeometry()->getActual()->getDisplayPrimaryI(this->parameterDisplayConfig.getDisplayDigits(eDimensionless));
            }
            return QVariant();
            break;
        case eFeatureDisplayPrimaryJ:
            if(!m_itemData->getMasterGeometry()->getActual().isNull()){
                return m_itemData->getMasterGeometry()->getActual()->getDisplayPrimaryJ(this->parameterDisplayConfig.getDisplayDigits(eDimensionless));
            }
            return QVariant();
            break;
        case eFeatureDisplayPrimaryK:
            if(!m_itemData->getMasterGeometry()->getActual().isNull()){
                return m_itemData->getMasterGeometry()->getActual()->getDisplayPrimaryK(this->parameterDisplayConfig.getDisplayDigits(eDimensionless));
            }
            return QVariant();
            break;
        case eFeatureDisplayRadiusA:
            if(!m_itemData->getMasterGeometry()->getActual().isNull()){
                return m_itemData->getMasterGeometry()->getActual()->getDisplayRadiusA(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                                                       this->parameterDisplayConfig.getDisplayDigits(eMetric));
            }
            return QVariant();
            break;
        case eFeatureDisplayRadiusB:
            if(!m_itemData->getMasterGeometry()->getActual().isNull()){
                return m_itemData->getMasterGeometry()->getActual()->getDisplayRadiusB(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                                                       this->parameterDisplayConfig.getDisplayDigits(eMetric));
            }
            return QVariant();
            break;
        case eFeatureDisplaySecondaryI:
            if(!m_itemData->getMasterGeometry()->getActual().isNull()){
                return m_itemData->getMasterGeometry()->getActual()->getDisplaySecondaryI(this->parameterDisplayConfig.getDisplayDigits(eDimensionless));
            }
            return QVariant();
            break;
        case eFeatureDisplaySecondaryJ:
            if(!m_itemData->getMasterGeometry()->getActual().isNull()){
                return m_itemData->getMasterGeometry()->getActual()->getDisplaySecondaryJ(this->parameterDisplayConfig.getDisplayDigits(eDimensionless));
            }
            return QVariant();
            break;
        case eFeatureDisplaySecondaryK:
            if(!m_itemData->getMasterGeometry()->getActual().isNull()){
                return m_itemData->getMasterGeometry()->getActual()->getDisplaySecondaryK(this->parameterDisplayConfig.getDisplayDigits(eDimensionless));
            }
            return QVariant();
            break;
        case eFeatureDisplayAperture:
            if(!m_itemData->getMasterGeometry()->getActual().isNull()){
                return m_itemData->getMasterGeometry()->getActual()->getDisplayAperture(this->parameterDisplayConfig.getDisplayUnit(eAngular),
                                                                                        this->parameterDisplayConfig.getDisplayDigits(eAngular));
            }
            return QVariant();
            break;
        case eFeatureDisplayA:
            if(!m_itemData->getMasterGeometry()->getActual().isNull()){
                return m_itemData->getMasterGeometry()->getActual()->getDisplayA(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                                                 this->parameterDisplayConfig.getDisplayDigits(eMetric));
            }
            return QVariant();
            break;
        case eFeatureDisplayB:
            if(!m_itemData->getMasterGeometry()->getActual().isNull()){
                return m_itemData->getMasterGeometry()->getActual()->getDisplayB(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                                                 this->parameterDisplayConfig.getDisplayDigits(eMetric));
            }
            return QVariant();
            break;
        case eFeatureDisplayC:
            if(!m_itemData->getMasterGeometry()->getActual().isNull()){
                return m_itemData->getMasterGeometry()->getActual()->getDisplayC(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                                                 this->parameterDisplayConfig.getDisplayDigits(eMetric));
            }
            return QVariant();
            break;
        case eFeatureDisplayAngle:
            if(!m_itemData->getMasterGeometry()->getActual().isNull()){
                return m_itemData->getMasterGeometry()->getActual()->getDisplayAngle(this->parameterDisplayConfig.getDisplayUnit(eAngular),
                                                                                     this->parameterDisplayConfig.getDisplayDigits(eAngular));
            }
            return QVariant();
            break;
        case eFeatureDisplayDistance:
            if(!m_itemData->getMasterGeometry()->getActual().isNull()){
                return m_itemData->getMasterGeometry()->getActual()->getDisplayDistance(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                                                        this->parameterDisplayConfig.getDisplayDigits(eMetric));
            }
            return QVariant();
            break;
        case eFeatureDisplayMeasurementSeries:
            if(!m_itemData->getMasterGeometry()->getActual().isNull()){
                return m_itemData->getMasterGeometry()->getActual()->getDisplayMeasurementSeries(this->parameterDisplayConfig.getDisplayUnit(eDimensionless),
                                                                                                 this->parameterDisplayConfig.getDisplayDigits(eDimensionless));
            }
            return QVariant();
            break;
        case eFeatureDisplayTemperature:
            if(!m_itemData->getMasterGeometry()->getActual().isNull()){
                return m_itemData->getMasterGeometry()->getActual()->getDisplayTemperature(this->parameterDisplayConfig.getDisplayUnit(eTemperature),
                                                                                           this->parameterDisplayConfig.getDisplayDigits(eTemperature));
            }
            return QVariant();
            break;
        case eFeatureDisplayLength:
            if(!m_itemData->getMasterGeometry()->getActual().isNull()){
                return m_itemData->getMasterGeometry()->getActual()->getDisplayLength(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                                                      this->parameterDisplayConfig.getDisplayDigits(eMetric));
            }
            return QVariant();
            break;
        case eFeatureDisplayExpansionOriginX:
            if(!m_itemData->getMasterGeometry()->getActual().isNull()){
                return m_itemData->getMasterGeometry()->getActual()->getDisplayExpansionOriginX(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                                                                this->parameterDisplayConfig.getDisplayDigits(eMetric));
            }
            return QVariant();
            break;
        case eFeatureDisplayExpansionOriginY:
            if(!m_itemData->getMasterGeometry()->getActual().isNull()){
                return m_itemData->getMasterGeometry()->getActual()->getDisplayExpansionOriginY(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                                                                this->parameterDisplayConfig.getDisplayDigits(eMetric));
            }
            return QVariant();
            break;
        case eFeatureDisplayExpansionOriginZ:
            if(!m_itemData->getMasterGeometry()->getActual().isNull()){
                return m_itemData->getMasterGeometry()->getActual()->getDisplayExpansionOriginZ(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                                                                this->parameterDisplayConfig.getDisplayDigits(eMetric));
            }
            return QVariant();
            break;
        default:
            return QVariant();
            break;
        }

    /////////////////////////////////////
    //nominal for master geometries//
    /////////////////////////////////////
    }else if(m_state == eNominalFeature){

        switch ((FeatureDisplayAttributes)attr) {
        case eFeatureDisplayType:
            return m_itemData->getFeature()->getDisplayType();
            break;
        case eFeatureDisplayName:
            return  m_itemData->getGeometry()->getMyMasterGeometry()->getFeatureName();
            break;
        case eFeatureDisplayComment:
                return  m_itemData->getFeature()->getComment();
            return QVariant();
            break;
        case eFeatureDisplayGroup:
            return m_itemData->getGeometry()->getMyMasterGeometry()->getGroupName();
            break;
        case eFeatureDisplayIsSolved:
            return  m_itemData->getFeature()->getIsSolved();
            break;
        case eFeatureDisplayIsUpdated:
                return  m_itemData->getFeature()->getDisplayIsUpdated();
            break;
        case eFeatureDisplayFunctions:
            return m_itemData->getFeature()->getDisplayFunctions();
            break;
        case eFeatureDisplayUsedFor:
            return  m_itemData->getFeature()->getDisplayUsedFor();
            break;
        case eFeatureDisplayPreviouslyNeeded:
            return  m_itemData->getFeature()->getDisplayPreviouslyNeeded();
            break;
        case eFeatureDisplayStDev:
            return  m_itemData->getFeature()->getDisplayStDev(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                                                     this->parameterDisplayConfig.getDisplayDigits(eMetric));
            break;
        case eFeatureDisplayMeasurementConfig:
            //return m_itemData->getMasterGeometry()->getDisplayMeasurementConfig();
            return QVariant();
            break;
        case eFeatureDisplayObservations:
            return m_itemData->getFeature()->getDisplayObservations();
            break;
        case eFeatureDisplayX:
            return m_itemData->getFeature()->getDisplayX(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                                                 this->parameterDisplayConfig.getDisplayDigits(eMetric));
            break;
        case eFeatureDisplayY:
            return m_itemData->getFeature()->getDisplayY(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                                                 this->parameterDisplayConfig.getDisplayDigits(eMetric));
            break;
        case eFeatureDisplayZ:
            return m_itemData->getFeature()->getDisplayZ(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                                                 this->parameterDisplayConfig.getDisplayDigits(eMetric));
            break;
        case eFeatureDisplayPrimaryI:
            return m_itemData->getFeature()->getDisplayPrimaryI(this->parameterDisplayConfig.getDisplayDigits(eDimensionless));
            break;
        case eFeatureDisplayPrimaryJ:
            return m_itemData->getFeature()->getDisplayPrimaryJ(this->parameterDisplayConfig.getDisplayDigits(eDimensionless));
            break;
        case eFeatureDisplayPrimaryK:
            return m_itemData->getFeature()->getDisplayPrimaryK(this->parameterDisplayConfig.getDisplayDigits(eDimensionless));
            break;
        case eFeatureDisplayRadiusA:
            return m_itemData->getFeature()->getDisplayRadiusA(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                                                       this->parameterDisplayConfig.getDisplayDigits(eMetric));
            break;
        case eFeatureDisplayRadiusB:
            return m_itemData->getFeature()->getDisplayRadiusB(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                                                       this->parameterDisplayConfig.getDisplayDigits(eMetric));
            break;
        case eFeatureDisplaySecondaryI:
            return m_itemData->getFeature()->getDisplaySecondaryI(this->parameterDisplayConfig.getDisplayDigits(eDimensionless));
            break;
        case eFeatureDisplaySecondaryJ:
            return m_itemData->getFeature()->getDisplaySecondaryJ(this->parameterDisplayConfig.getDisplayDigits(eDimensionless));
            break;
        case eFeatureDisplaySecondaryK:
            return m_itemData->getFeature()->getDisplaySecondaryK(this->parameterDisplayConfig.getDisplayDigits(eDimensionless));
            break;
        case eFeatureDisplayAperture:
            return m_itemData->getFeature()->getDisplayAperture(this->parameterDisplayConfig.getDisplayUnit(eAngular),
                                                                                        this->parameterDisplayConfig.getDisplayDigits(eAngular));
            break;
        case eFeatureDisplayA:
            return m_itemData->getFeature()->getDisplayA(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                                                 this->parameterDisplayConfig.getDisplayDigits(eMetric));
            break;
        case eFeatureDisplayB:
            return m_itemData->getFeature()->getDisplayB(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                                                 this->parameterDisplayConfig.getDisplayDigits(eMetric));
            break;
        case eFeatureDisplayC:
            return m_itemData->getFeature()->getDisplayC(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                                                 this->parameterDisplayConfig.getDisplayDigits(eMetric));
            break;
        case eFeatureDisplayAngle:
            return m_itemData->getFeature()->getDisplayAngle(this->parameterDisplayConfig.getDisplayUnit(eAngular),
                                                                                     this->parameterDisplayConfig.getDisplayDigits(eAngular));
            break;
        case eFeatureDisplayDistance:
            return m_itemData->getFeature()->getDisplayDistance(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                                                        this->parameterDisplayConfig.getDisplayDigits(eMetric));
            break;
        case eFeatureDisplayMeasurementSeries:
            return m_itemData->getFeature()->getDisplayMeasurementSeries(this->parameterDisplayConfig.getDisplayUnit(eDimensionless),
                                                                                                 this->parameterDisplayConfig.getDisplayDigits(eDimensionless));
            break;
        case eFeatureDisplayTemperature:
            return m_itemData->getFeature()->getDisplayTemperature(this->parameterDisplayConfig.getDisplayUnit(eTemperature),
                                                                                           this->parameterDisplayConfig.getDisplayDigits(eTemperature));
            break;
        case eFeatureDisplayLength:
            return m_itemData->getFeature()->getDisplayLength(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                                                      this->parameterDisplayConfig.getDisplayDigits(eMetric));
            break;
        case eFeatureDisplayExpansionOriginX:
            return m_itemData->getFeature()->getDisplayExpansionOriginX(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                                                                this->parameterDisplayConfig.getDisplayDigits(eMetric));
            break;
        case eFeatureDisplayExpansionOriginY:
            return m_itemData->getFeature()->getDisplayExpansionOriginY(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                                                                this->parameterDisplayConfig.getDisplayDigits(eMetric));
            break;
        case eFeatureDisplayExpansionOriginZ:
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
        switch ((FeatureDisplayAttributes)attr) {
        case eFeatureDisplayType:
            return m_itemData->getFeature()->getDisplayType();
            break;
        case eFeatureDisplayName:
            return m_itemData->getFeature()->getFeatureName();
            break;
        case eFeatureDisplayComment:
            return m_itemData->getFeature()->getComment();
            break;
        case eFeatureDisplayGroup:
            return m_itemData->getFeature()->getGroupName();
            break;
        case eFeatureDisplayIsSolved:
            return m_itemData->getFeature()->getIsSolved();
            break;
        case eFeatureDisplayIsUpdated:
            return m_itemData->getFeature()->getDisplayIsUpdated();
            break;
        case eFeatureDisplayFunctions:
            return m_itemData->getFeature()->getDisplayFunctions();
            break;
        case eFeatureDisplayUsedFor:
            return m_itemData->getFeature()->getDisplayUsedFor();
            break;
        case eFeatureDisplayPreviouslyNeeded:
            return m_itemData->getFeature()->getDisplayPreviouslyNeeded();
            break;
        case eFeatureDisplayStDev:
            return m_itemData->getFeature()->getDisplayStDev(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                             this->parameterDisplayConfig.getDisplayDigits(eMetric));
            break;
        case eFeatureDisplayMeasurementConfig:
            return QVariant(); //only master geom has measurement config
            break;
        case eFeatureDisplayObservations:
            return m_itemData->getFeature()->getDisplayObservations();
            break;
        case eFeatureDisplayX:
            return m_itemData->getFeature()->getDisplayX(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                         this->parameterDisplayConfig.getDisplayDigits(eMetric));
            break;
        case eFeatureDisplayY:
            return m_itemData->getFeature()->getDisplayY(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                         this->parameterDisplayConfig.getDisplayDigits(eMetric));
            break;
        case eFeatureDisplayZ:
            return m_itemData->getFeature()->getDisplayZ(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                         this->parameterDisplayConfig.getDisplayDigits(eMetric));
            break;
        case eFeatureDisplayPrimaryI:
            return m_itemData->getFeature()->getDisplayPrimaryI(this->parameterDisplayConfig.getDisplayDigits(eDimensionless));
            break;
        case eFeatureDisplayPrimaryJ:
            return m_itemData->getFeature()->getDisplayPrimaryJ(this->parameterDisplayConfig.getDisplayDigits(eDimensionless));
            break;
        case eFeatureDisplayPrimaryK:
            return m_itemData->getFeature()->getDisplayPrimaryK(this->parameterDisplayConfig.getDisplayDigits(eDimensionless));
            break;
        case eFeatureDisplayRadiusA:
            return m_itemData->getFeature()->getDisplayRadiusA(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                               this->parameterDisplayConfig.getDisplayDigits(eMetric));
            break;
        case eFeatureDisplayRadiusB:
            return m_itemData->getFeature()->getDisplayRadiusB(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                               this->parameterDisplayConfig.getDisplayDigits(eMetric));
            break;
        case eFeatureDisplaySecondaryI:
            return m_itemData->getFeature()->getDisplaySecondaryI(this->parameterDisplayConfig.getDisplayDigits(eDimensionless));
            break;
        case eFeatureDisplaySecondaryJ:
            return m_itemData->getFeature()->getDisplaySecondaryJ(this->parameterDisplayConfig.getDisplayDigits(eDimensionless));
            break;
        case eFeatureDisplaySecondaryK:
            return m_itemData->getFeature()->getDisplaySecondaryK(this->parameterDisplayConfig.getDisplayDigits(eDimensionless));
            break;
        case eFeatureDisplayAperture:
            return m_itemData->getFeature()->getDisplayAperture(this->parameterDisplayConfig.getDisplayUnit(eAngular),
                                                                this->parameterDisplayConfig.getDisplayDigits(eAngular));
            break;
        case eFeatureDisplayA:
            return m_itemData->getFeature()->getDisplayA(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                         this->parameterDisplayConfig.getDisplayDigits(eMetric));
            break;
        case eFeatureDisplayB:
            return m_itemData->getFeature()->getDisplayB(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                        this->parameterDisplayConfig.getDisplayDigits(eMetric));
            break;
        case eFeatureDisplayC:
            return m_itemData->getFeature()->getDisplayC(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                        this->parameterDisplayConfig.getDisplayDigits(eMetric));
            break;
        case eFeatureDisplayAngle:
            return m_itemData->getFeature()->getDisplayAngle(this->parameterDisplayConfig.getDisplayUnit(eAngular),
                                                             this->parameterDisplayConfig.getDisplayDigits(eAngular));
            break;
        case eFeatureDisplayDistance:
            return m_itemData->getFeature()->getDisplayDistance(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                                this->parameterDisplayConfig.getDisplayDigits(eMetric));
            break;
        case eFeatureDisplayMeasurementSeries:
            return m_itemData->getFeature()->getDisplayMeasurementSeries(this->parameterDisplayConfig.getDisplayUnit(eDimensionless),
                                                                         this->parameterDisplayConfig.getDisplayDigits(eDimensionless));
            break;
        case eFeatureDisplayTemperature:
            return m_itemData->getFeature()->getDisplayTemperature(this->parameterDisplayConfig.getDisplayUnit(eTemperature),
                                                                   this->parameterDisplayConfig.getDisplayDigits(eTemperature));
            break;
        case eFeatureDisplayLength:
            return m_itemData->getFeature()->getDisplayLength(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                             this->parameterDisplayConfig.getDisplayDigits(eMetric));
            break;
        case eFeatureDisplayExpansionOriginX:
            return m_itemData->getFeature()->getDisplayExpansionOriginX(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                                        this->parameterDisplayConfig.getDisplayDigits(eMetric));
            break;
        case eFeatureDisplayExpansionOriginY:
            return m_itemData->getFeature()->getDisplayExpansionOriginY(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                                        this->parameterDisplayConfig.getDisplayDigits(eMetric));
            break;
        case eFeatureDisplayExpansionOriginZ:
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
    if(m_parentItem != NULL){
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

/*!
 * \brief FeatureItem::deleteChildren
 */
void FeatureItem::deleteChildren()
{
    qDeleteAll(this->m_childItems);
    this->m_childItems.clear();
}
