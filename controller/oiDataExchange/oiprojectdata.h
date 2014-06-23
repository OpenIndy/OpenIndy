#ifndef OIPROJECTDATA_H
#define OIPROJECTDATA_H

#include <QObject>
#include <QIODevice>

#include "featurewrapper.h"
#include "console.h"

/*!
 * \brief The oiProjectData class
 * exchange object for a whole openindy project.
 * Used by oiprojectexchanger for generating a openindyXML
 * or generating a oiProjectData Object form a openindyXML
 */
class OiProjectData : public QObject
{
    Q_OBJECT
public:
    explicit OiProjectData(QObject *parent = 0);
    ~OiProjectData();

    QIODevice *getDevice();
    bool setDevice(QIODevice *device);

    QString projectName;

    CoordinateSystem *activeCoordSystem;
    Station *activeStation;

    QList<FeatureWrapper*> features;
    QList<CoordinateSystem*> coordSystems;
    QList<Station*> stations;

private:
    QIODevice *device;

signals:

public slots:

};

#endif // OIPROJECTDATA_H
