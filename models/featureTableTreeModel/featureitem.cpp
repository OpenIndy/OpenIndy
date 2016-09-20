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
    return m_childItems.size();
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
            if(m_state == eActualFeature || m_state == eNominalFeature){
                return m_itemData->getGeometry()->getMyMasterGeometry()->getDisplayMeasurementConfig();
            }
            return m_itemData->getFeature()->getDisplayMeasurementConfig();
            break;
        case eFeatureDisplayObservations:
            return m_itemData->getFeature()->getDisplayObservations();
            break;
        case eFeatureDisplayX:
            if(m_itemData->getFeatureTypeEnum() == eStationFeature){
                return m_itemData->getStation()->getPosition()->getDisplayX(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                               this->parameterDisplayConfig.getDisplayDigits(eMetric));
            }
            return m_itemData->getFeature()->getDisplayX(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                         this->parameterDisplayConfig.getDisplayDigits(eMetric));
            break;
        case eFeatureDisplayY:
            if(m_itemData->getFeatureTypeEnum() == eStationFeature){
                return m_itemData->getStation()->getPosition()->getDisplayY(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                               this->parameterDisplayConfig.getDisplayDigits(eMetric));
            }
            return m_itemData->getFeature()->getDisplayY(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                         this->parameterDisplayConfig.getDisplayDigits(eMetric));
            break;
        case eFeatureDisplayZ:
            if(m_itemData->getFeatureTypeEnum() == eStationFeature){
                return m_itemData->getStation()->getPosition()->getDisplayZ(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                               this->parameterDisplayConfig.getDisplayDigits(eMetric));
            }
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

    }else if(m_state == eActualFeature){

        QPointer<FeatureWrapper> feature;
        bool actualValid = false;

        if(!m_itemData->getMasterGeometry().isNull() && !m_itemData->getMasterGeometry()->getActual().isNull()){
            feature = m_itemData->getMasterGeometry()->getActual()->getFeatureWrapper();
            actualValid = true;
        }

        switch ((FeatureDisplayAttributes)attr) {
        case eFeatureDisplayType:
            if(actualValid){
                return feature->getFeature()->getDisplayType();
            }
            return m_itemData->getMasterGeometry()->getNominals().first()->getDisplayType();
            break;
        case eFeatureDisplayName:
            return m_itemData->getFeature()->getFeatureName();
            break;
        case eFeatureDisplayComment:
            if(actualValid){
                return feature->getFeature()->getComment();
            }
            return QVariant();
            break;
        case eFeatureDisplayGroup:
            return m_itemData->getFeature()->getGroupName();
            break;
        case eFeatureDisplayIsSolved:
            if(actualValid){
                return feature->getFeature()->getIsSolved();
            }
            return QVariant();
            break;
        case eFeatureDisplayIsUpdated:
            if(actualValid){
                return feature->getFeature()->getDisplayIsUpdated();
            }
            return QVariant();
            break;
        case eFeatureDisplayFunctions:
            if(actualValid){
                return feature->getFeature()->getDisplayFunctions();
            }
            return QVariant();
            break;
        case eFeatureDisplayUsedFor:
            if(actualValid){
                return feature->getFeature()->getDisplayUsedFor();
            }
            return QVariant();
            break;
        case eFeatureDisplayPreviouslyNeeded:
            if(actualValid){
                return feature->getFeature()->getDisplayPreviouslyNeeded();
            }
            return QVariant();
            break;
        case eFeatureDisplayStDev:
            if(actualValid){
                return feature->getFeature()->getDisplayStDev(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                              this->parameterDisplayConfig.getDisplayDigits(eMetric));
            }
            return QVariant();
            break;
        case eFeatureDisplayMeasurementConfig:
            return m_itemData->getFeature()->getDisplayMeasurementConfig();
            break;
        case eFeatureDisplayObservations:
            if(actualValid){
                return feature->getFeature()->getDisplayObservations();
            }
            return QVariant();
            break;
        case eFeatureDisplayX:
            if(actualValid){
                return feature->getFeature()->getDisplayX(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                          this->parameterDisplayConfig.getDisplayDigits(eMetric));
            }
            return QVariant();
            break;
        case eFeatureDisplayY:
            if(actualValid){
                return feature->getFeature()->getDisplayY(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                          this->parameterDisplayConfig.getDisplayDigits(eMetric));
            }
            return QVariant();
            break;
        case eFeatureDisplayZ:
            if(actualValid){
                return feature->getFeature()->getDisplayZ(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                          this->parameterDisplayConfig.getDisplayDigits(eMetric));
            }
            return QVariant();
            break;
        case eFeatureDisplayPrimaryI:
            if(actualValid){
                return feature->getFeature()->getDisplayPrimaryI(this->parameterDisplayConfig.getDisplayDigits(eDimensionless));
            }
            return QVariant();
            break;
        case eFeatureDisplayPrimaryJ:
            if(actualValid){
                return feature->getFeature()->getDisplayPrimaryJ(this->parameterDisplayConfig.getDisplayDigits(eDimensionless));
            }
            return QVariant();
            break;
        case eFeatureDisplayPrimaryK:
            if(actualValid){
                return feature->getFeature()->getDisplayPrimaryK(this->parameterDisplayConfig.getDisplayDigits(eDimensionless));
            }
            return QVariant();
            break;
        case eFeatureDisplayRadiusA:
            if(actualValid){
                return feature->getFeature()->getDisplayRadiusA(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                                this->parameterDisplayConfig.getDisplayDigits(eMetric));
            }
            return QVariant();
            break;
        case eFeatureDisplayRadiusB:
            if(actualValid){
                return feature->getFeature()->getDisplayRadiusB(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                                this->parameterDisplayConfig.getDisplayDigits(eMetric));
            }
            return QVariant();
            break;
        case eFeatureDisplaySecondaryI:
            if(actualValid){
                return feature->getFeature()->getDisplaySecondaryI(this->parameterDisplayConfig.getDisplayDigits(eDimensionless));
            }
            return QVariant();
            break;
        case eFeatureDisplaySecondaryJ:
            if(actualValid){
                return feature->getFeature()->getDisplaySecondaryJ(this->parameterDisplayConfig.getDisplayDigits(eDimensionless));
            }
            return QVariant();
            break;
        case eFeatureDisplaySecondaryK:
            if(actualValid){
                return feature->getFeature()->getDisplaySecondaryK(this->parameterDisplayConfig.getDisplayDigits(eDimensionless));
            }
            return QVariant();
            break;
        case eFeatureDisplayAperture:
            if(actualValid){
                return feature->getFeature()->getDisplayAperture(this->parameterDisplayConfig.getDisplayUnit(eAngular),
                                                                 this->parameterDisplayConfig.getDisplayDigits(eAngular));
            }
            return QVariant();
            break;
        case eFeatureDisplayA:
            if(actualValid){
                return feature->getFeature()->getDisplayA(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                          this->parameterDisplayConfig.getDisplayDigits(eMetric));
            }
            return QVariant();
            break;
        case eFeatureDisplayB:
            if(actualValid){
                return feature->getFeature()->getDisplayB(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                          this->parameterDisplayConfig.getDisplayDigits(eMetric));
            }
            return QVariant();
            break;
        case eFeatureDisplayC:
            if(actualValid){
                return feature->getFeature()->getDisplayC(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                          this->parameterDisplayConfig.getDisplayDigits(eMetric));
            }
            return QVariant();
            break;
        case eFeatureDisplayAngle:
            if(actualValid){
                return feature->getFeature()->getDisplayAngle(this->parameterDisplayConfig.getDisplayUnit(eAngular),
                                                              this->parameterDisplayConfig.getDisplayDigits(eAngular));
            }
            return QVariant();
            break;
        case eFeatureDisplayDistance:
            if(actualValid){
                return feature->getFeature()->getDisplayDistance(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                                 this->parameterDisplayConfig.getDisplayDigits(eMetric));
            }
            return QVariant();
            break;
        case eFeatureDisplayMeasurementSeries:
            if(actualValid){
                return feature->getFeature()->getDisplayMeasurementSeries(this->parameterDisplayConfig.getDisplayUnit(eDimensionless),
                                                                          this->parameterDisplayConfig.getDisplayDigits(eDimensionless));
            }
            return QVariant();
            break;
        case eFeatureDisplayTemperature:
            if(actualValid){
                return feature->getFeature()->getDisplayTemperature(this->parameterDisplayConfig.getDisplayUnit(eTemperature),
                                                                    this->parameterDisplayConfig.getDisplayDigits(eTemperature));
            }
            return QVariant();
            break;
        case eFeatureDisplayLength:
            if(actualValid){
                return feature->getFeature()->getDisplayLength(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                               this->parameterDisplayConfig.getDisplayDigits(eMetric));
            }
            return QVariant();
            break;
        case eFeatureDisplayExpansionOriginX:
            if(actualValid){
                return feature->getFeature()->getDisplayExpansionOriginX(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                                         this->parameterDisplayConfig.getDisplayDigits(eMetric));
            }
            return QVariant();
            break;
        case eFeatureDisplayExpansionOriginY:
            if(actualValid){
                return feature->getFeature()->getDisplayExpansionOriginY(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                                         this->parameterDisplayConfig.getDisplayDigits(eMetric));
            }
            return QVariant();
            break;
        case eFeatureDisplayExpansionOriginZ:
            if(actualValid){
                return feature->getFeature()->getDisplayExpansionOriginZ(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                                         this->parameterDisplayConfig.getDisplayDigits(eMetric));
            }
            return QVariant();
            break;
        default:
            return QVariant();
            break;
        }

    }else if(m_state == eNominalFeature){

        QPointer<FeatureWrapper> feature;
        bool nominalValid = false;

        if(!m_itemData->getMasterGeometry().isNull()){
            foreach (QPointer<Geometry> geom, m_itemData->getMasterGeometry()->getNominals()) {
                if(geom->getIsSolved()){
                    feature = geom->getFeatureWrapper();
                    nominalValid = true;
                }
            }
        }else{
            return QVariant();
        }

        switch ((FeatureDisplayAttributes)attr) {
        case eFeatureDisplayType:
            if(nominalValid){
                return feature->getFeature()->getDisplayType();
            }
            return m_itemData->getMasterGeometry()->getNominals().first()->getDisplayType();
            break;
        case eFeatureDisplayName:
            return m_itemData->getFeature()->getFeatureName();
            break;
        case eFeatureDisplayComment:
            if(nominalValid){
                return feature->getFeature()->getComment();
            }
            return QVariant();
            break;
        case eFeatureDisplayGroup:
            return m_itemData->getFeature()->getGroupName();
            break;
        case eFeatureDisplayIsSolved:
            if(nominalValid){
                return feature->getFeature()->getIsSolved();
            }
            return QVariant();
            break;
        case eFeatureDisplayIsUpdated:
            if(nominalValid){
                return feature->getFeature()->getDisplayIsUpdated();
            }
            return QVariant();
            break;
        case eFeatureDisplayFunctions:
            if(nominalValid){
                return feature->getFeature()->getDisplayFunctions();
            }
            return QVariant();
            break;
        case eFeatureDisplayUsedFor:
            if(nominalValid){
                return feature->getFeature()->getDisplayUsedFor();
            }
            return QVariant();
            break;
        case eFeatureDisplayPreviouslyNeeded:
            if(nominalValid){
                return feature->getFeature()->getDisplayPreviouslyNeeded();
            }
            return QVariant();
            break;
        case eFeatureDisplayStDev:
            if(nominalValid){
                return feature->getFeature()->getDisplayStDev(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                              this->parameterDisplayConfig.getDisplayDigits(eMetric));
            }
            return QVariant();
            break;
        case eFeatureDisplayMeasurementConfig:
            return m_itemData->getFeature()->getDisplayMeasurementConfig();
            break;
        case eFeatureDisplayObservations:
            if(nominalValid){
                return feature->getFeature()->getDisplayObservations();
            }
            return QVariant();
            break;
        case eFeatureDisplayX:
            if(nominalValid){
                return feature->getFeature()->getDisplayX(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                          this->parameterDisplayConfig.getDisplayDigits(eMetric));
            }
            return QVariant();
            break;
        case eFeatureDisplayY:
            if(nominalValid){
                return feature->getFeature()->getDisplayY(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                          this->parameterDisplayConfig.getDisplayDigits(eMetric));
            }
            return QVariant();
            break;
        case eFeatureDisplayZ:
            if(nominalValid){
                return feature->getFeature()->getDisplayZ(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                          this->parameterDisplayConfig.getDisplayDigits(eMetric));
            }
            return QVariant();
            break;
        case eFeatureDisplayPrimaryI:
            if(nominalValid){
                return feature->getFeature()->getDisplayPrimaryI(this->parameterDisplayConfig.getDisplayDigits(eDimensionless));
            }
            return QVariant();
            break;
        case eFeatureDisplayPrimaryJ:
            if(nominalValid){
                return feature->getFeature()->getDisplayPrimaryJ(this->parameterDisplayConfig.getDisplayDigits(eDimensionless));
            }
            return QVariant();
            break;
        case eFeatureDisplayPrimaryK:
            if(nominalValid){
                return feature->getFeature()->getDisplayPrimaryK(this->parameterDisplayConfig.getDisplayDigits(eDimensionless));
            }
            return QVariant();
            break;
        case eFeatureDisplayRadiusA:
            if(nominalValid){
                return feature->getFeature()->getDisplayRadiusA(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                                this->parameterDisplayConfig.getDisplayDigits(eMetric));
            }
            return QVariant();
            break;
        case eFeatureDisplayRadiusB:
            if(nominalValid){
                return feature->getFeature()->getDisplayRadiusB(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                                this->parameterDisplayConfig.getDisplayDigits(eMetric));
            }
            return QVariant();
            break;
        case eFeatureDisplaySecondaryI:
            if(nominalValid){
                return feature->getFeature()->getDisplaySecondaryI(this->parameterDisplayConfig.getDisplayDigits(eDimensionless));
            }
            return QVariant();
            break;
        case eFeatureDisplaySecondaryJ:
            if(nominalValid){
                return feature->getFeature()->getDisplaySecondaryJ(this->parameterDisplayConfig.getDisplayDigits(eDimensionless));
            }
            return QVariant();
            break;
        case eFeatureDisplaySecondaryK:
            if(nominalValid){
                return feature->getFeature()->getDisplaySecondaryK(this->parameterDisplayConfig.getDisplayDigits(eDimensionless));
            }
            return QVariant();
            break;
        case eFeatureDisplayAperture:
            if(nominalValid){
                return feature->getFeature()->getDisplayAperture(this->parameterDisplayConfig.getDisplayUnit(eAngular),
                                                                 this->parameterDisplayConfig.getDisplayDigits(eAngular));
            }
            return QVariant();
            break;
        case eFeatureDisplayA:
            if(nominalValid){
                return feature->getFeature()->getDisplayA(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                          this->parameterDisplayConfig.getDisplayDigits(eMetric));
            }
            return QVariant();
            break;
        case eFeatureDisplayB:
            if(nominalValid){
                return feature->getFeature()->getDisplayB(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                          this->parameterDisplayConfig.getDisplayDigits(eMetric));
            }
            return QVariant();
            break;
        case eFeatureDisplayC:
            if(nominalValid){
                return feature->getFeature()->getDisplayC(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                          this->parameterDisplayConfig.getDisplayDigits(eMetric));
            }
            return QVariant();
            break;
        case eFeatureDisplayAngle:
            if(nominalValid){
                return feature->getFeature()->getDisplayAngle(this->parameterDisplayConfig.getDisplayUnit(eAngular),
                                                              this->parameterDisplayConfig.getDisplayDigits(eAngular));
            }
            return QVariant();
            break;
        case eFeatureDisplayDistance:
            if(nominalValid){
                return feature->getFeature()->getDisplayDistance(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                                 this->parameterDisplayConfig.getDisplayDigits(eMetric));
            }
            return QVariant();
            break;
        case eFeatureDisplayMeasurementSeries:
            if(nominalValid){
                return feature->getFeature()->getDisplayMeasurementSeries(this->parameterDisplayConfig.getDisplayUnit(eDimensionless),
                                                                          this->parameterDisplayConfig.getDisplayDigits(eDimensionless));
            }
            return QVariant();
            break;
        case eFeatureDisplayTemperature:
            if(nominalValid){
                return feature->getFeature()->getDisplayTemperature(this->parameterDisplayConfig.getDisplayUnit(eTemperature),
                                                                    this->parameterDisplayConfig.getDisplayDigits(eTemperature));
            }
            return QVariant();
            break;
        case eFeatureDisplayLength:
            if(nominalValid){
                return feature->getFeature()->getDisplayLength(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                               this->parameterDisplayConfig.getDisplayDigits(eMetric));
            }
            return QVariant();
            break;
        case eFeatureDisplayExpansionOriginX:
            if(nominalValid){
                return feature->getFeature()->getDisplayExpansionOriginX(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                                         this->parameterDisplayConfig.getDisplayDigits(eMetric));
            }
            return QVariant();
            break;
        case eFeatureDisplayExpansionOriginY:
            if(nominalValid){
                return feature->getFeature()->getDisplayExpansionOriginY(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                                         this->parameterDisplayConfig.getDisplayDigits(eMetric));
            }
            return QVariant();
            break;
        case eFeatureDisplayExpansionOriginZ:
            if(nominalValid){
                return feature->getFeature()->getDisplayExpansionOriginZ(this->parameterDisplayConfig.getDisplayUnit(eMetric),
                                                                         this->parameterDisplayConfig.getDisplayDigits(eMetric));
            }
            return QVariant();
            break;
        default:
            return QVariant();
            break;
        }

    }else if(m_state == eDifferenceFeature){
        //Differenzen anzeigen
        QPointer<Geometry> activeNominal;
        QPointer<Geometry> activeActual;

        if(!m_itemData->getMasterGeometry().isNull() && !m_itemData->getMasterGeometry()->getActual().isNull()){
            activeActual = m_itemData->getMasterGeometry()->getActual();
        }
        foreach (QPointer<Geometry> geom, m_itemData->getMasterGeometry()->getNominals()) {
            if(geom->getIsSolved()){
                activeNominal = geom;
            }
        }
        if(activeActual.isNull() || activeNominal.isNull()){
            return QVariant();
        }
        switch ((FeatureDisplayAttributes)attr) {
        case eFeatureDisplayComment:
            return m_itemData->getFeature()->getComment();
            break;
        case eFeatureDisplayX:
            if(!activeActual->hasPosition() || !activeNominal->hasPosition())
                return QVariant();

            return QString::number(activeActual->getPosition().getVector().getAt(0) - activeNominal->getPosition().getVector().getAt(0),
                                   'd',this->parameterDisplayConfig.getDisplayDigits(eMetric));
            break;
        case eFeatureDisplayY:
            if(!activeActual->hasPosition() || !activeNominal->hasPosition())
                return QVariant();

            return QString::number(activeActual->getPosition().getVector().getAt(1) - activeNominal->getPosition().getVector().getAt(1),
                                   'd',this->parameterDisplayConfig.getDisplayDigits(eMetric));
            break;
        case eFeatureDisplayZ:
            if(!activeActual->hasPosition() || !activeNominal->hasPosition())
                return QVariant();

            return QString::number(activeActual->getPosition().getVector().getAt(2) - activeNominal->getPosition().getVector().getAt(2),
                                   'd',this->parameterDisplayConfig.getDisplayDigits(eMetric));
            break;
        case eFeatureDisplayPrimaryI:
            if(!activeActual->hasDirection() || !activeNominal->hasDirection())
                return QVariant();

            return QString::number(activeActual->getDirection().getVector().getAt(0) - activeNominal->getDirection().getVector().getAt(0),
                                   'd',this->parameterDisplayConfig.getDisplayDigits(eDimensionless));
            break;
        case eFeatureDisplayPrimaryJ:
            if(!activeActual->hasDirection() || !activeNominal->hasDirection())
                return QVariant();

            return QString::number(activeActual->getDirection().getVector().getAt(1) - activeNominal->getDirection().getVector().getAt(1),
                                   'd',this->parameterDisplayConfig.getDisplayDigits(eDimensionless));
            break;
        case eFeatureDisplayPrimaryK:
            if(!activeActual->hasDirection() || !activeNominal->hasDirection())
                return QVariant();

            return QString::number(activeActual->getDirection().getVector().getAt(2) - activeNominal->getDirection().getVector().getAt(2),
                                   'd',this->parameterDisplayConfig.getDisplayDigits(eDimensionless));
            break;
        default:
            return "-";
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

/*!
 * \brief FeatureItem::getState
 * \return
 */
FeatureViewState FeatureItem::getState()
{
    return this->m_state;
}

/*!
 * \brief FeatureItem::getItemData
 * \return
 */
QPointer<FeatureWrapper> FeatureItem::getItemData()
{
    return this->m_itemData;
}
