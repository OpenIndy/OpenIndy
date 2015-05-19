#ifndef UTIL_H
#define UTIL_H

#include <QString>
#include <QStringList>
#include <QMap>

#include "types.h"

//#######################################
//global functions to query element types
//#######################################

QList<ElementTypes> getAvailableElementTypes();
QList<FeatureTypes> getAvailableFeatureTypes();
QList<GeometryTypes> getAvailableGeometryTypes();

const QString &getElementTypeName(const ElementTypes &type);
const QString &getElementTypePluralName(const ElementTypes &type);
ElementTypes getElementTypeEnum(const QString &name);
ElementTypes getElementTypeEnum(const FeatureTypes &type);
ElementTypes getElementTypeEnum(const ReadingTypes &type);

const QString &getFeatureTypeName(const FeatureTypes &type);
FeatureTypes getFeatureTypeEnum(const QString &name);

const QString &getGeometryTypeName(const GeometryTypes &type);
GeometryTypes getGeometryTypeEnum(const QString &name);

bool getIsFeature(const ElementTypes &type);
bool getIsReading(const ElementTypes &type);
bool getIsObservation(const ElementTypes &type);

bool getIsGeometry(const FeatureTypes &type);
bool getIsScalarEntity(const FeatureTypes &type);

//#########################################################
//global function to query icon resources for element types
//#########################################################

const QString &getElementTypeIconPath(const ElementTypes &type);
const QString &getFeatureTypeIconPath(const FeatureTypes &type);

//#######################################
//global functions to query reading types
//#######################################

const QString &getReadingTypeName(const ReadingTypes &type);
ReadingTypes getReadingTypeEnum(const QString &name);

//############################################
//global functions to query display attributes
//############################################

const QList<int> &getFeatureDisplayAttributes();
const QList<ObservationDisplayattributes> &getObservationDisplayAttributes();
const QList<ReadingDisplayAttributes> &getReadingDisplayAttributes();

bool getIsFeatureDisplayAttribute(const int &attr);
bool getIsTrafoParamDisplayAttribute(const int &attr);

const QString &getFeatureDisplayAttributeName(const int &attr);
const QString &getFeatureDisplayAttributeName(const FeatureDisplayAttributes &attr);
const QString &getFeatureDisplayAttributeName(const TrafoParamDisplayAttributes &attr);

bool getReadingDisplayAttributeVisibility(const ReadingDisplayAttributes &attr, const ReadingTypes &type);

const QString &getObservationDisplayAttributesName(const ObservationDisplayattributes &attr);
const QString &getReadingDisplayAttributeName(const ReadingDisplayAttributes &attr);

//###############################
//global functions to query units
//###############################

double convertToDefault(const double &value, const UnitType &type);
double convertFromDefault(const double &value, const UnitType &type);

const QString &getUnitTypeName(const UnitType &type);
UnitType getUnitTypeEnum(const QString &name);

//######################################
//global functions to query sensor types
//######################################

QList<SensorTypes> getAvailableSensorTypes();
const QString &getSensorTypeName(const SensorTypes &type);
SensorTypes getSensorTypeEnum(const QString &name);

//##########################################
//global functions to query sensor functions
//##########################################

QList<SensorFunctions> getAvailableSensorFunctions();
const QString &getSensorFunctionName(const SensorFunctions &type);
SensorFunctions getSensorFunctionEnum(const QString &name);

//#########################################
//global function to query connection types
//#########################################

QList<ConnectionTypes> getAvailableConnectionTypes();
const QString &getConnectionTypeName(const ConnectionTypes &type);
ConnectionTypes getConnectionTypeEnum(const QString &name);

//####################################
//global functions to query tool types
//####################################

QList<ToolTypes> getAvailableToolTypes();
const QString &getToolTypeName(const ToolTypes &type);
ToolTypes getToolTypeEnum(const QString &name);

#endif // UTIL_H
