#ifndef UTIL_H
#define UTIL_H

#include <QString>
#include <QStringList>
#include <QMap>

#include "types.h"

//#######################################
//global functions to query element types
//#######################################

const QString &getElementTypeName(const ElementTypes &type);
ElementTypes getElementTypeEnum(const QString &name);

const QString &getFeatureTypeName(const FeatureTypes &type);
FeatureTypes getFeatureTypeEnum(const QString &name);

const QString &getGeometryTypeName(const GeometryTypes &type);
GeometryTypes getGeometryTypeEnum(const QString &name);

bool getIsGeometry(const FeatureTypes &type);

//#######################################
//global functions to query reading types
//#######################################

const QString &getReadingTypeName(const ReadingTypes &type);
ReadingTypes getReadingTypeEnum(const QString &name);

#endif // UTIL_H
