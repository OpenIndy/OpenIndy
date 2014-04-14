#ifndef PROJECTRESTORER_H
#define PROJECTRESTORER_H

#include <QObject>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QFile>
#include <QDateTime>
#include <QtAlgorithms>
#include "oiprojectdata.h"
#include "function.h"
#include "systemdbmanager.h"
#include "pluginloader.h"
#include "console.h"

class ProjectRestorer : public QObject
{
    Q_OBJECT
public:
    explicit ProjectRestorer(QObject *parent = 0);

signals:

public slots:
    bool saveProject(oiProjectData &data);
    bool loadProject(oiProjectData &data);

private:

    QList<Observation*> observations;
    QList<Station*> stations;
    QList<CoordinateSystem*> coordSystems;
    QList<TrafoParam*> trafoParams;
    QList<FeatureWrapper*> geometries;
    QList<FeatureWrapper*> features;
    QList<ElementDependencies> dependencies;

    QList<int> stationElements;

    void clearAllLists();
    void addGeometryToList(Configuration::ElementTypes typeOfElement,QXmlStreamReader &xml);

    Station *findStation(int id);
    FeatureWrapper *findFeature(int id);
    TrafoParam *findTrafoParam(int id);
    CoordinateSystem *findCoordSys(int id);
    FeatureWrapper *findGeometry(int id);
    Observation *findObservation(int id);

    void resolveDependencies(oiProjectData &data);
    void resolveFeature(FeatureWrapper *fw, ElementDependencies &d);
    void resolveGeometry(FeatureWrapper *fw, ElementDependencies &d);
    void resolveStation(FeatureWrapper *fw, ElementDependencies &d);
    void resolveTrafoParam(FeatureWrapper *fw, ElementDependencies &d);
    void resolveCoordinateSystem(FeatureWrapper *fw, ElementDependencies &d);

    void resolveObservation(ElementDependencies &d);


    QList<Function*> resolveFunctions(ElementDependencies &d);


};

#endif // PROJECTRESTORER_H
