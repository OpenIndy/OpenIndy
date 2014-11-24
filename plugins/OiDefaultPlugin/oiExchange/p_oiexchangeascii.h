#ifndef P_OIEXCHANGEASCII_H
#define P_OIEXCHANGEASCII_H

#include <exception>
#include <QRegExp>
#include <QVariantList>

#include "pi_oiexchangesimpleascii.h"

using namespace std;

class OiExchangeAscii : public OiExchangeSimpleAscii
{ 
public:

    //overwrite pure virtual methods of OiExchangeInterface
    PluginMetaData *getMetaData() const;
    bool importOiData(OiExchangeObject &projectData);
    bool exportOiData(OiExchangeObject &projectData);
    QList<Configuration::GeometryTypes> getSupportedGeometries();

    //overwrite pure virtual methods of OiExchangeSimpleAscii
    QList<OiExchangeSimpleAscii::ColumnType> getDefaultColumnOrder(Configuration::GeometryTypes typeOfGeometry, const OiExchangeObject &projectData) const;
    QMap<OiExchangeSimpleAscii::ColumnType, QVariantList> getFilePreview(Configuration::GeometryTypes typeOfGeometry, const OiExchangeObject &projectData) const;
    QList<OiExchangeSimpleAscii::ColumnType> getPossibleColumns(Configuration::GeometryTypes typeOfGeometry) const;
    QStringList getSupportedDelimiters() const;

private:
    QRegExp getDelimiter(QString delimiterName) const;

};

#endif // P_OIEXCHANGEASCII_H
