#include "observationtablecolumnconfig.h"

/*!
 * \brief ObservationTableColumnConfig::ObservationTableColumnConfig
 */
ObservationTableColumnConfig::ObservationTableColumnConfig(){

    //load default column config
    this->init();

}

/*!
 * \brief ObservationTableColumnConfig::getColumnVisibility
 * \param column
 * \return
 */
bool ObservationTableColumnConfig::getColumnVisibility(const ObservationDisplayAttributes &column) const{
    return this->displayColumns.contains(column);
}

/*!
 * \brief ObservationTableColumnConfig::setColumnVisibility
 * \param column
 * \param isVisible
 */
void ObservationTableColumnConfig::setColumnVisibility(const ObservationDisplayAttributes &column, const bool &isVisible){
    if(isVisible && !this->displayColumns.contains(column)){
        this->displayColumns.append(column);
    }else{
        this->displayColumns.removeOne(column);
    }
}

/*!
 * \brief ObservationTableColumnConfig::getColumnPosition
 * Returns the position index of the given column or -1 if the column is not displayed
 * \param column
 * \return
 */
int ObservationTableColumnConfig::getColumnPosition(const ObservationDisplayAttributes &column) const{
    return this->displayColumns.indexOf(column);
}

/*!
 * \brief ObservationTableColumnConfig::setColumnPosition
 * \param column
 * \param index
 */
void ObservationTableColumnConfig::setColumnPosition(const ObservationDisplayAttributes &column, const int &index){
    if(this->displayColumns.contains(column) && index >= 0 && index < this->displayColumns.size()){
        this->displayColumns.removeOne(column);
        this->displayColumns.insert(index, column);
    }
}

/*!
 * \brief ObservationTableColumnConfig::init
 * Set up default column config
 */
void ObservationTableColumnConfig::init(){

    //default columns in default order
    this->displayColumns.append(eObservationDisplayId);
    this->displayColumns.append(eObservationDisplayStation);
    this->displayColumns.append(eObservationDisplayTargetGeometries);
    this->displayColumns.append(eObservationDisplayX);
    this->displayColumns.append(eObservationDisplayY);
    this->displayColumns.append(eObservationDisplayZ);
    this->displayColumns.append(eObservationDisplayI);
    this->displayColumns.append(eObservationDisplayJ);
    this->displayColumns.append(eObservationDisplayK);
    /*this->displayColumns.append(eObservationDisplaySigmaX);
    this->displayColumns.append(eObservationDisplaySigmaY);
    this->displayColumns.append(eObservationDisplaySigmaZ);
    this->displayColumns.append(eObservationDisplaySigmaI);
    this->displayColumns.append(eObservationDisplaySigmaJ);
    this->displayColumns.append(eObservationDisplaySigmaK);*/
    //this->displayColumns.append(eObservationDisplayIsValid);
    this->displayColumns.append(eObservationDisplayIsSolved);
    this->displayColumns.append(eObservationDisplayVX);
    this->displayColumns.append(eObservationDisplayVY);
    this->displayColumns.append(eObservationDisplayVZ);
    this->displayColumns.append(eObservationDisplayV);
    this->displayColumns.append(eObservationDisplayVR);
    this->displayColumns.append(eObservationDisplayIsUsed);
    this->displayColumns.append(eObservationDisplayIsDummyPoint);
    this->displayColumns.append(eObservationDisplayReadingTime);

}
