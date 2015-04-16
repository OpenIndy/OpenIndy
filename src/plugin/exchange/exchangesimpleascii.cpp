#include "exchangesimpleascii.h"

/*!
 * \brief ExchangeSimpleAscii::ExchangeSimpleAscii
 * \param parent
 */
ExchangeSimpleAscii::ExchangeSimpleAscii(QObject *parent) : ExchangeInterface(parent), typeOfGeometry(eUndefinedGeometry){

}

/*!
 * \brief ExchangeSimpleAscii::~ExchangeSimpleAscii
 */
ExchangeSimpleAscii::~ExchangeSimpleAscii(){

}

/*!
 * \brief ExchangeSimpleAscii::getDefaultColumnOrder
 * Returns an ordered list of columns that a file possibly contains (for a given geometry type)
 * \param typeOfGeometry
 * \return
 */
QList<ExchangeSimpleAscii::ColumnType> ExchangeSimpleAscii::getDefaultColumnOrder(const GeometryTypes &typeOfGeometry) const{
    return QList<ExchangeSimpleAscii::ColumnType>();
}

/*!
 * \brief ExchangeSimpleAscii::getFilePreview
 * Returns a list of sample file entries for each column specified in getDefaultColumnOrder()
 * \param typeOfGeometry
 * \return
 */
QMap<ExchangeSimpleAscii::ColumnType, QVariantList> ExchangeSimpleAscii::getFilePreview(const GeometryTypes &typeOfGeometry) const{
    return QMap<ExchangeSimpleAscii::ColumnType, QVariantList>();
}

/*!
 * \brief ExchangeSimpleAscii::getPossibleColumns
 * Returns a list of possible columns for a given geometry type
 * \param typeOfGeometry
 * \return
 */
QList<ExchangeSimpleAscii::ColumnType> ExchangeSimpleAscii::getPossibleColumns(const GeometryTypes &typeOfGeometry) const{
    return QList<ExchangeSimpleAscii::ColumnType>();
}

/*!
 * \brief ExchangeSimpleAscii::getSupportedDelimiters
 * \return
 */
const QStringList &ExchangeSimpleAscii::getSupportedDelimiters() const{
    return this->supportedDelimiters;
}

/*!
 * \brief ExchangeSimpleAscii::getUserDefinedColumns
 * Get the meaning of each column
 * \return
 */
const QList<ExchangeSimpleAscii::ColumnType> &ExchangeSimpleAscii::getUserDefinedColumns() const{
    return this->userDefinedColumns;
}

/*!
 * \brief ExchangeSimpleAscii::setUserDefinedColumns
 * Specify the meaning of each column
 * \param userDefinedColumns
 */
void ExchangeSimpleAscii::setUserDefinedColumns(const QList<ExchangeSimpleAscii::ColumnType> &userDefinedColumns){
    this->userDefinedColumns = userDefinedColumns;
}

/*!
 * \brief ExchangeSimpleAscii::getTypeOfGeometry
 * Returns the type of geometry that shall be im- or exported
 * \return
 */
const GeometryTypes &ExchangeSimpleAscii::getTypeOfGeometry() const{
    return this->typeOfGeometry;
}

/*!
 * \brief ExchangeSimpleAscii::setGeometryType
 * Specify which type of geometry shall be im- or exported
 * \param typeOfGeometry
 */
void ExchangeSimpleAscii::setGeometryType(const GeometryTypes &typeOfGeometry){
    if(this->typeOfGeometry != typeOfGeometry){
        this->typeOfGeometry = typeOfGeometry;
        this->userDefinedColumns.clear();
    }
}

/*!
 * \brief ExchangeSimpleAscii::getSkipFirstLine
 * \return
 */
const bool &ExchangeSimpleAscii::getSkipFirstLine() const{
    return this->skipFirstLine;
}

/*!
 * \brief ExchangeSimpleAscii::setSkipFirstLine
 * Skip the first line in a file during import / do not write headers to file during export
 * \param skipFirstLine
 */
void ExchangeSimpleAscii::setSkipFirstLine(const bool &skipFirstLine){
    this->skipFirstLine = skipFirstLine;
}

/*!
 * \brief ExchangeSimpleAscii::getUsedDelimiter
 * \return
 */
const QString &ExchangeSimpleAscii::getUsedDelimiter() const{
    return this->usedDelimiter;
}

/*!
 * \brief ExchangeSimpleAscii::setDelimiter
 * Set the delimiter that shall be used when parsing the file
 * \param usedDelimiter
 */
void ExchangeSimpleAscii::setDelimiter(const QString &usedDelimiter){
    this->usedDelimiter = usedDelimiter;
}
