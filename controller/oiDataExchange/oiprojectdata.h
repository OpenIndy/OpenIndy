#ifndef OIPROJECTDATA_H
#define OIPROJECTDATA_H

#include <QObject>
#include "featurewrapper.h"
#include <QIODevice>

/*!
 * \brief The oiProjectData class
 * exchange object for a whole openindy project.
 * Used by oiprojectexchanger for generating a openindyXML
 * or generating a oiProjectData Object form a openindyXML
 */
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
