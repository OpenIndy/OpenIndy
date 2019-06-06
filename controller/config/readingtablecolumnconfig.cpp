#include "readingtablecolumnconfig.h"

/*!
 * \brief ReadingTableColumnConfig::ReadingTableColumnConfig
 */
ReadingTableColumnConfig::ReadingTableColumnConfig(){

    //load default column config
    this->init();

}

/*!
 * \brief ReadingTableColumnConfig::getColumnVisibility
 * \param column
 * \return
 */
bool ReadingTableColumnConfig::getColumnVisibility(const ReadingDisplayAttributes &column) const{
    return this->displayColumns.contains(column);
}

/*!
 * \brief ReadingTableColumnConfig::setColumnVisibility
 * \param column
 * \param isVisible
 */
void ReadingTableColumnConfig::setColumnVisibility(const ReadingDisplayAttributes &column, const bool &isVisible){
    if(isVisible && !this->displayColumns.contains(column)){
        this->displayColumns.append(column);
    }else{
        this->displayColumns.removeOne(column);
    }
}

/*!
 * \brief ReadingTableColumnConfig::getColumnPosition
 * Returns the position index of the given column or -1 if the column is not displayed
 * \param column
 * \return
 */
int ReadingTableColumnConfig::getColumnPosition(const ReadingDisplayAttributes &column) const{
    return this->displayColumns.indexOf(column);
}

/*!
 * \brief ReadingTableColumnConfig::setColumnPosition
 * \param column
 * \param index
 */
void ReadingTableColumnConfig::setColumnPosition(const ReadingDisplayAttributes &column, const int &index){
    if(this->displayColumns.contains(column) && index >= 0 && index < this->displayColumns.size()){
        this->displayColumns.removeOne(column);
        this->displayColumns.insert(index, column);
    }
}

/*!
 * \brief ReadingTableColumnConfig::init
 * Set up default column config
 */
void ReadingTableColumnConfig::init(){

    //default columns in default order
    this->displayColumns.append(eReadingDisplayId);
    this->displayColumns.append(eReadingDisplayType);
    this->displayColumns.append(eReadingDisplayTime);
    this->displayColumns.append(eReadingDisplaySensor);
    this->displayColumns.append(eReadingDisplaySide);
    this->displayColumns.append(eReadingDisplayAzimuth);
    this->displayColumns.append(eReadingDisplayZenith);
    this->displayColumns.append(eReadingDisplayDistance);
    this->displayColumns.append(eReadingDisplayX);
    this->displayColumns.append(eReadingDisplayY);
    this->displayColumns.append(eReadingDisplayZ);
    this->displayColumns.append(eReadingDisplayI);
    this->displayColumns.append(eReadingDisplayJ);
    this->displayColumns.append(eReadingDisplayK);
    this->displayColumns.append(eReadingDisplayTemperature);
    this->displayColumns.append(eReadingDisplayImported);
    /*this->displayColumns.append(eReadingDisplaySigmaAzimuth);
    this->displayColumns.append(eReadingDisplaySigmaZenith);
    this->displayColumns.append(eReadingDisplaySigmaDistance);
    this->displayColumns.append(eReadingDisplaySigmaX);
    this->displayColumns.append(eReadingDisplaySigmaY);
    this->displayColumns.append(eReadingDisplaySigmaZ);
    this->displayColumns.append(eReadingDisplaySigmaI);
    this->displayColumns.append(eReadingDisplaySigmaJ);
    this->displayColumns.append(eReadingDisplaySigmaK);
    this->displayColumns.append(eReadingDisplaySigmaTemperature);*/

}
