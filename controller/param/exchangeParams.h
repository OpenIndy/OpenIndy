#ifndef EXCHANGEPARAMS_H
#define EXCHANGEPARAMS_H

#include <QString>
#include <QList>
#include <QMap>
#include <QPointer>

#include "types.h"
#include "exchangeinterface.h"
#include "exchangedefinedformat.h"
#include "exchangesimpleascii.h"
#include "featurewrapper.h"
#include "coordinatesystem.h"

/*!
 * \brief The ExchangeParams class
 * Holds parameters for exchange plugins
 */
class ExchangeParams
{
public:

    //file path of the file that shall be im- or exported
    QString exchangeFilePath;

    //plugin information
    QString pluginName; //name of the plugin
    QString exchangeName; //name of the exchange implementation
    bool isDefinedFormat; //true if the exchange is a defined format exchange, false if it is a simple ascii one

    //general exchange parameters
    QList<QPointer<FeatureWrapper> > exportFeatures; //features that shall be exported
    bool exportObservations; //true if observations shall be exported
    QString groupName;
    QString nominalSystem;
    QMap<DimensionType, UnitType> units;

    //simple ascii parameters
    QString delimiter;
    bool skipFirstLine;
    GeometryTypes typeOfGeometry;
    QList<ExchangeSimpleAscii::ColumnType> userDefinedColumns;

    //defined format parameters
    QList<ElementTypes> usedElements;

};

#endif // EXCHANGEPARAMS_H
