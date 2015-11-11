#include "trafoparamtablecolumnconfig.h"

/*!
 * \brief TrafoParamTableColumnConfig::FeatureTableColumnConfig
 */
TrafoParamTableColumnConfig::TrafoParamTableColumnConfig(){

    //load default column config
    this->init();

}

/*!
 * \brief TrafoParamTableColumnConfig::getColumnVisibility
 * \param column
 * \return
 */
bool TrafoParamTableColumnConfig::getColumnVisibility(const TrafoParamDisplayAttributes &column) const{
    return this->displayColumns.contains(column);
}

/*!
 * \brief TrafoParamTableColumnConfig::setColumnVisibility
 * \param column
 * \param isVisible
 */
void TrafoParamTableColumnConfig::setColumnVisibility(const TrafoParamDisplayAttributes &column, const bool &isVisible){
    if(isVisible && !this->displayColumns.contains(column)){
        this->displayColumns.append(column);
    }else{
        this->displayColumns.removeOne(column);
    }
}

/*!
 * \brief TrafoParamTableColumnConfig::getColumnPosition
 * Returns the position index of the given column or -1 if the column is not displayed
 * \param column
 * \return
 */
int TrafoParamTableColumnConfig::getColumnPosition(const TrafoParamDisplayAttributes &column) const{
    return this->displayColumns.indexOf(column);
}

/*!
 * \brief TrafoParamTableColumnConfig::setColumnPosition
 * \param column
 * \param index
 */
void TrafoParamTableColumnConfig::setColumnPosition(const TrafoParamDisplayAttributes &column, const int &index){
    if(this->displayColumns.contains(column) && index >= 0 && index < this->displayColumns.size()){
        this->displayColumns.removeOne(column);
        this->displayColumns.insert(index, column);
    }
}

/*!
 * \brief TrafoParamTableColumnConfig::init
 * Set up default column config
 */
void TrafoParamTableColumnConfig::init(){

    //default columns in default order
    this->displayColumns.append(eTrafoParamDisplayType);
    this->displayColumns.append(eTrafoParamDisplayName);
    this->displayColumns.append(eTrafoParamDisplayGroup);
    this->displayColumns.append(eTrafoParamDisplayIsSolved);
    this->displayColumns.append(eTrafoParamDisplayIsUsed);
    this->displayColumns.append(eTrafoParamDisplayIsDatumTransformation);
    this->displayColumns.append(eTrafoParamDisplayStartSystem);
    this->displayColumns.append(eTrafoParamDisplayDestinationSystem);
    this->displayColumns.append(eTrafoParamDisplayTranslationX);
    this->displayColumns.append(eTrafoParamDisplayTranslationY);
    this->displayColumns.append(eTrafoParamDisplayTranslationZ);
    this->displayColumns.append(eTrafoParamDisplayRotationX);
    this->displayColumns.append(eTrafoParamDisplayRotationY);
    this->displayColumns.append(eTrafoParamDisplayRotationZ);
    this->displayColumns.append(eTrafoParamDisplayScaleX);
    this->displayColumns.append(eTrafoParamDisplayScaleY);
    this->displayColumns.append(eTrafoParamDisplayScaleZ);
    this->displayColumns.append(eTrafoParamDisplayStDev);
    this->displayColumns.append(eTrafoParamDisplayValidTime);
    this->displayColumns.append(eTrafoParamDisplayFunctions);
    this->displayColumns.append(eTrafoParamDisplayComment);

}
