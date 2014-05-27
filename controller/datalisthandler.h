#ifndef DATALISTHANDLER_H
#define DATALISTHANDLER_H

#include <QList>
#include "featurewrapper.h"

/*!
 * \brief The DataListHandler class is an exchange object for the lists and pointers of the controller class.
 * It can be extended very simple, without change the parameters of any function.
 */
class DataListHandler
{
public:
    DataListHandler();
    QList<FeatureWrapper*> *features;
    FeatureWrapper *activeFeature;
    QList<CoordinateSystem*> *coordSys;
    QList<Station*> *stations;
    Station *activeStation;
    CoordinateSystem *activeCoordinateSystem;
    QMap<QString, int> *availableGroups;
};

#endif // DATALISTHANDLER_H
