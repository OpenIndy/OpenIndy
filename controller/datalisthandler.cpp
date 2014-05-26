#include "datalisthandler.h"

/*!
 * \brief DataListHandler::DataListHandler sets alle POINTERS to lists and objects NULL
 */
DataListHandler::DataListHandler()
{
    QList<FeatureWrapper*> *features = NULL;
    FeatureWrapper *activeFeature = NULL;
    QList<CoordinateSystem*> *coordSys = NULL;
    QList<Station*> *stations = NULL;
    Station *activeStation = NULL;
    CoordinateSystem *activeCoordinateSystem = NULL;
    QMap<QString, int> *availableGroups = NULL;
}
