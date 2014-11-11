#ifndef OIPROJECTEXCHANGER_H
#define OIPROJECTEXCHANGER_H

#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QFile>
#include <QDateTime>
#include <QtAlgorithms>
#include <QMap>

#include "function.h"
#include "systemdbmanager.h"
#include "pluginloader.h"

#include "projectrestorer.h"

#include "oiprojectdata.h"
#include "console.h"
#include "configuration.h"
#include "oirequesthandler.h"

#include <QtXml>

/*!
 * \brief The OiProjectExchanger class
 * This class provides methods to save and load OpenIndy projects and different configs
 */
class OiProjectExchanger
{
    friend class OiRequestHandler;
private:
    static QDomDocument saveProject();
    static bool loadProject(const QDomDocument &project);

    //helper methods for loading different types of elements
    static bool loadObservations(const QDomDocument &project);
    static bool loadStations(const QDomDocument &project);
    static bool loadCoordinateSystems(const QDomDocument &project);
    static bool loadTransformationParameters(const QDomDocument &project);
    static bool loadGeometries(const QDomDocument &project);

    //helper methods for restoring the dependencies between the loaded elements
    static bool restoreStationDependencies(const QDomDocument &project);
    static bool restoreCoordinateSystemDependencies(const QDomDocument &project);
    static bool restoreTrafoParamDependencies(const QDomDocument &project);
    static bool restoreGeometryDependencies(const QDomDocument &project);
    static bool restoreObservationDependencies(const QDomDocument &project);

    static QList<Function*> restoreFunctionDependencies(const QDomElement &functions);

    //helper maps for loading an OpenIndy project (key is the id of an element)
    static QMap<int, Observation *> myObservations;
    static QMap<int, Reading *> myReadings;
    static QMap<int, FeatureWrapper *> myStations;
    static QMap<int, FeatureWrapper *> myCoordinateSystems;
    static QMap<int, FeatureWrapper *> myTransformationParameters;
    static QMap<int, FeatureWrapper *> myGeometries;

    static void clearHelperMaps(bool deleteOnClear);

    static QList<int> stationPoints;

};

#endif // OIPROJECTEXCHANGER_H
