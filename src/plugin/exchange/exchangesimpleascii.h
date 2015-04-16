#ifndef EXCHANGESIMPLEASCII_H
#define EXCHANGESIMPLEASCII_H

#include <QVariantList>

#include "exchangeinterface.h"

/*!
 * \brief The ExchangeSimpleAscii class
 * Interface that shall be used for simple not standardized ascii files.
 * The user of OpenIndy can select the meaning of each column of the file on his own.
 */
class ExchangeSimpleAscii : public ExchangeInterface
{
    Q_OBJECT

public:
    explicit ExchangeSimpleAscii(QObject *parent = 0);

    virtual ~ExchangeSimpleAscii();

    //###############################################
    //all possible column types (geometry attributes)
    //###############################################

    enum ColumnType{
        eColumnIgnore,
        eColumnFeatureName,
        eColumnGroupName,
        eColumnComment,
        eColumnX,
        eColumnY,
        eColumnZ,
        eColumnPrimaryI,
        eColumnPrimaryJ,
        eColumnPrimaryK,
        eColumnSecondaryI,
        eColumnSecondaryJ,
        eColumnSecondaryK,
        eColumnRadiusA,
        eColumnRadiusB,
        eColumnAperture,
        eColumnA,
        eColumnB,
        eColumnC,
        eColumnAngle,
        eColumnDistance,
        eColumnMeasurementSeries,
        eColumnTemperature,
        eColumnLength
    };

    //#########################################
    //methods to get or set further information
    //#########################################

    virtual QList<ExchangeSimpleAscii::ColumnType> getDefaultColumnOrder(const GeometryTypes &typeOfGeometry) const;
    virtual QMap<ExchangeSimpleAscii::ColumnType, QVariantList> getFilePreview(const GeometryTypes &typeOfGeometry) const;
    virtual QList<ExchangeSimpleAscii::ColumnType> getPossibleColumns(const GeometryTypes &typeOfGeometry) const;

    const QStringList &getSupportedDelimiters() const;

    const QList<ExchangeSimpleAscii::ColumnType> &getUserDefinedColumns() const;
    void setUserDefinedColumns(const QList<ExchangeSimpleAscii::ColumnType> &userDefinedColumns);

    const GeometryTypes &getTypeOfGeometry() const;
    void setGeometryType(const GeometryTypes &typeOfGeometry);

    const bool &getSkipFirstLine() const;
    void setSkipFirstLine(const bool &skipFirstLine);

    const QString &getUsedDelimiter() const;
    void setDelimiter(const QString &usedDelimiter);

protected:

    //###########################
    //input and output parameters
    //###########################

    QList<ExchangeSimpleAscii::ColumnType> userDefinedColumns; //an ordered list of enum values that specify what to do with each column
    GeometryTypes typeOfGeometry; //specifies which type of geometry shall be im- or exported
    bool skipFirstLine; //Skip the first line in a file during import / do not write headers to file during export
    QString usedDelimiter; //the delimiter that shall be used when parsing the file

    QStringList supportedDelimiters;

};

#define ExchangeSimpleAscii_iidd "de.openIndy.plugin.exchange.exchangeSimpleAscii.v001"

#endif // EXCHANGESIMPLEASCII_H
