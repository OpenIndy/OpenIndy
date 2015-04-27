#ifndef P_OIEXCHANGEASCII_H
#define P_OIEXCHANGEASCII_H

#include <exception>
#include <QRegExp>
#include <QVariantList>

#include "exchangesimpleascii.h"

using namespace std;

class OiExchangeAscii : public ExchangeSimpleAscii
{ 
    Q_OBJECT

protected:

    //##############################
    //exchange initialization method
    //##############################

    void init();

public:

    //#########################
    //import and export methods
    //#########################

    void importOiData();
    void exportOiData();

    //#########################################
    //methods to get or set further information
    //#########################################

    QList<ExchangeSimpleAscii::ColumnType> getDefaultColumnOrder(const GeometryTypes &typeOfGeometry);
    QMap<ExchangeSimpleAscii::ColumnType, QVariantList> getFilePreview(const GeometryTypes &typeOfGeometry);
    QList<ExchangeSimpleAscii::ColumnType> getPossibleColumns(const GeometryTypes &typeOfGeometry);

private:

    //##############
    //helper methods
    //##############

    QRegExp getDelimiter(const QString &delimiterName) const;

};

#endif // P_OIEXCHANGEASCII_H
