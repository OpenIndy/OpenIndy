#ifndef PI_OIEXCHANGESIMPLEASCII_H
#define PI_OIEXCHANGESIMPLEASCII_H

#include <QVariantList>

#include "pi_oiexchangeinterface.h"

/*!
 * \brief The OiExchangeSimpleAscii class
 * Interface that shall be used for simple not standardized ascii files.
 * The user of OpenIndy can select the meaning of each column of the file on his own.
 */
class OiExchangeSimpleAscii : public OiExchangeInterface
{
    Q_OBJECT

public:
    explicit OiExchangeSimpleAscii(QObject *parent = 0) : OiExchangeInterface(parent){}
    virtual ~OiExchangeSimpleAscii(){}

    enum ColumnType{
        eColumnIgnore,
        eColumnFeatureName,
        eColumnGroupName,
        eColumnX,
        eColumnY,
        eColumnZ,
        eColumnI,
        eColumnJ,
        eColumnK,
        eColumnRadius,
        eColumnAngle,
        eColumnDistance,
        eColumnComment,
        eColumnTemperature
    };

    /*!
     * \brief getDefaultColumnOrder
     * Returns an ordered list of columns that a file possibly contains (for a given geometry type)
     * \param typeOfGeometry
     * \param projectData
     * \return
     */
    virtual QList<OiExchangeSimpleAscii::ColumnType> getDefaultColumnOrder(GeometryTypes typeOfGeometry, const OiExchangeObject &projectData) const = 0;

    /*!
     * \brief getFilePreview
     * Returns a list of sample file entries for each column specified in getDefaultColumnOrder()
     * \param typeOfGeometry
     * \param projectData
     * \return
     */
    virtual QMap<OiExchangeSimpleAscii::ColumnType, QVariantList> getFilePreview(GeometryTypes typeOfGeometry, const OiExchangeObject &projectData) const = 0;

    /*!
     * \brief getPossibleColumns
     * Returns a list of possible columns for a given geometry type
     * \param typeOfGeometry
     * \return
     */
    virtual QList<OiExchangeSimpleAscii::ColumnType> getPossibleColumns(GeometryTypes typeOfGeometry) const = 0;

    /*!
     * \brief getSupportedDelimiters
     * \return
     */
    virtual QStringList getSupportedDelimiters() const = 0;

    /*!
     * \brief setUserDefinedColumns
     * Specify the meaning of each column
     * \param userDefinedColumns
     */
    void setUserDefinedColumns(QList<OiExchangeSimpleAscii::ColumnType> userDefinedColumns){
        this->userDefinedColumns = userDefinedColumns;
    }

    /*!
     * \brief setGeometryType
     * Specify which type of elemen shall be im- or exported
     * \param typeOfElement
     */
    void setGeometryType(GeometryTypes typeOfGeometry){
        this->typeOfGeometry = typeOfGeometry;
    }

    /*!
     * \brief setSkipFirstLine
     * Skip the first line in a file during import / do not write headers to file during export
     * \param skipFirstLine
     */
    void setSkipFirstLine(bool skipFirstLine){
        this->skipFirstLine = skipFirstLine;
    }

    /*!
     * \brief setDelimiter
     * Set the delimiter that shall be used when parsing the file
     * \param usedDelimiter
     */
    void setDelimiter(QString usedDelimiter){
        this->usedDelimiter = usedDelimiter;
    }

protected:
    QList<OiExchangeSimpleAscii::ColumnType> userDefinedColumns; //an ordered list of enum values that specify what to do with each column
    GeometryTypes typeOfGeometry; //specifies which type of geometry shall be im- or exported
    bool skipFirstLine; //Skip the first line in a file during import / do not write headers to file during export
    QString usedDelimiter; //the delimiter that shall be used when parsing the file

};

#define OiExchangeSimpleAscii_iidd "de.openIndy.Plugin.OiExchange.OiExchangeSimpleAscii.v001"

#endif // PI_OIEXCHANGESIMPLEASCII_H
