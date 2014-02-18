#ifndef OIPROJECTDATA_H
#define OIPROJECTDATA_H

#include <QObject>
#include "featurewrapper.h"
#include <QIODevice>

class oiProjectData : public QObject
{
    Q_OBJECT
public:
    explicit oiProjectData(QObject *parent = 0);


    QString projectName;
    QIODevice *device;

    CoordinateSystem *activeCoordSystem;
    Station *activeStation;

    QList<FeatureWrapper*> features;
    QList<CoordinateSystem*> coordSystems;
    QList<Station*> stations;



signals:

public slots:

};

#endif // OIPROJECTDATA_H
