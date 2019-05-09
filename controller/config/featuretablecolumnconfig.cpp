#include "featuretablecolumnconfig.h"

/*!
 * \brief FeatureTableColumnConfig::FeatureTableColumnConfig
 */
FeatureTableColumnConfig::FeatureTableColumnConfig(){

    //load default column config
    this->init();

}
FeatureTableColumnConfig::FeatureTableColumnConfig(QList<FeatureDisplayAttributes> displayColumns) {
    this->displayColumns = displayColumns;
}

/*!
 * \brief FeatureTableColumnConfig::getColumnVisibility
 * \param column
 * \return
 */
bool FeatureTableColumnConfig::getColumnVisibility(const FeatureDisplayAttributes &column) const{
    return this->displayColumns.contains(column);
}

/*!
 * \brief FeatureTableColumnConfig::setColumnVisibility
 * \param column
 * \param isVisible
 */
void FeatureTableColumnConfig::setColumnVisibility(const FeatureDisplayAttributes &column, const bool &isVisible){
    if(isVisible && !this->displayColumns.contains(column)){
        this->displayColumns.append(column);
    }else{
        this->displayColumns.removeOne(column);
    }
}

/*!
 * \brief FeatureTableColumnConfig::getColumnPosition
 * Returns the position index of the given column or -1 if the column is not displayed
 * \param column
 * \return
 */
int FeatureTableColumnConfig::getColumnPosition(const FeatureDisplayAttributes &column) const{
    return this->displayColumns.indexOf(column);
}

/*!
 * \brief FeatureTableColumnConfig::setColumnPosition
 * \param column
 * \param index
 */
void FeatureTableColumnConfig::setColumnPosition(const FeatureDisplayAttributes &column, const int &index){
    if(this->displayColumns.contains(column) && index >= 0 && index < this->displayColumns.size()){
        this->displayColumns.removeOne(column);
        this->displayColumns.insert(index, column);
    }
}

/*!
 * \brief FeatureTableColumnConfig::getDisplayAttributeAt
 * \param column
 * \return
 */
FeatureDisplayAttributes FeatureTableColumnConfig::getDisplayAttributeAt(const int &column) const{
    if(this->displayColumns.size() > column){
        return this->displayColumns.at(column);
    }
    return eFeatureDisplayType;
}

/*!
 * \brief FeatureTableColumnConfig::init
 * Set up default column config
 */
void FeatureTableColumnConfig::init(){

    //default columns in default order
    this->displayColumns.append(eFeatureDisplayType);
    this->displayColumns.append(eFeatureDisplayIsActual);
    this->displayColumns.append(eFeatureDisplayGroup);
    this->displayColumns.append(eFeatureDisplayName);
    this->displayColumns.append(eFeatureDisplayX);
    this->displayColumns.append(eFeatureDisplayY);
    this->displayColumns.append(eFeatureDisplayZ);
    this->displayColumns.append(eFeatureDisplayStDev);
    this->displayColumns.append(eFeatureDisplayObservations);
    this->displayColumns.append(eFeatureDisplayMeasurementConfig);
    this->displayColumns.append(eFeatureDisplayFunctions);
    this->displayColumns.append(eFeatureDisplayIsCommon);
    this->displayColumns.append(eFeatureDisplayPrimaryI);
    this->displayColumns.append(eFeatureDisplayPrimaryJ);
    this->displayColumns.append(eFeatureDisplayPrimaryK);
    this->displayColumns.append(eFeatureDisplayRadiusA);
    this->displayColumns.append(eFeatureDisplayIsSolved);
    this->displayColumns.append(eFeatureDisplayComment);
    this->displayColumns.append(eFeatureDisplayAngle);
    this->displayColumns.append(eFeatureDisplayDistance);
}
