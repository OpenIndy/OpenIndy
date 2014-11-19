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
     * Returns an ordered list of columns that a file possibly contains
     * \return
     */
    virtual QList<OiExchangeSimpleAscii::ColumnType> getDefaultColumnOrder(const OiExchangeObject &projectData) const = 0;

    /*!
     * \brief getFilePreview
     * Returns a list of sample file entries for each column specified in getDefaultColumnOrder()
     * \return
     */
    virtual QMap<OiExchangeSimpleAscii::ColumnType, QVariantList> getFilePreview(const OiExchangeObject &projectData) const = 0;

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
     * Specify which type of elemen typet shall be im- or exported
     * \param typeOfElement
     */
    void setGeometryType(Configuration::GeometryTypes typeOfGeometry){
        this->typeOfGeometry = typeOfGeometry;
    }

private:
    QList<OiExchangeSimpleAscii::ColumnType> userDefinedColumns; //an ordered list of enum values that specify what to do with each column
    Configuration::GeometryTypes typeOfGeometry; //specifies which type of geometry shall be im- or exported

};

#define OiExchangeSimpleAscii_iidd "de.openIndy.Plugin.OiExchangeSimpleAscii.v001"

#endif // PI_OIEXCHANGESIMPLEASCII_H
