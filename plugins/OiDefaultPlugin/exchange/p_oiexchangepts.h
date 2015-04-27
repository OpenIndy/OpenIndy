#ifndef P_OIEXCHANGEPTS_H
#define P_OIEXCHANGEPTS_H

#include "exchangedefinedformat.h"

class OiExchangePts : public OiExchangeDefinedFormat
{ 
public:

    //overwrite pure virtual methods of OiExchangeInterface
    PluginMetaData *getMetaData() const;
    bool importOiData(OiExchangeObject &projectData);
    bool exportOiData(OiExchangeObject &projectData);
    QList<Configuration::GeometryTypes> getSupportedGeometries();

    //overwrite pure virtual methods of OiExchangeDefinedFormat
    QStringList getSupportedFileExtensions() const;

};

#endif // P_OIEXCHANGEPTS_H
