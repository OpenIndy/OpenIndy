#ifndef UTIL_H
#define UTIL_H

#include <QString>
#include <QStringList>
#include <QMap>

#include "types.h"

//#######################################
//global functions to query element types
//#######################################

QString getElementTypeName(const ElementTypes &type);
ElementTypes getElementTypeEnum(const QString &name);

QString getFeatureTypeName(const FeatureTypes &type);
FeatureTypes getFeatureTypeEnum(const QString &name);

QString getGeometryTypeName(const GeometryTypes &type);
GeometryTypes getGeometryTypeEnum(const QString &name);

bool getIsGeometry(const FeatureTypes &type);

//#######################################
//global functions to query reading types
//#######################################

QString getReadingTypeName(const ReadingTypes &type);
ReadingTypes getReadingTypeEnum(const QString &name);

#endif // UTIL_H
