#ifndef P_TRANSLATEBYPLANE_H
#define P_TRANSLATEBYPLANE_H

#include <QtGlobal>
#include <QDateTime>
#include <QObject>
#include <QDebug>
#include <QThread>
#include <qmath.h>
#include <vector>
#include "pi_objecttransformation.h"
#include "console.h"
#include "point.h"
#include "plane.h"
#include "configuration.h"
#include "oivec.h"
#include "oimat.h"
#include "pluginmetadata.h"

using namespace std;

class TranslateByPlane : public ObjectTransformation
{  
public:
    PluginMetaData* getMetaData();
    bool exec(Point&);
    bool exec(Line&);
    bool exec(Plane&);
    bool exec(Sphere&);
    QList<InputParams> getNeededElements();
    QList<Configuration::FeatureTypes> applicableFor();
    QMap<QString, QStringList> getStringParameter();

private:
    void shiftPoint(Point&, Plane*, ScalarEntityDistance*);
    void shiftLine(Line&, Plane*, ScalarEntityDistance*);
    void shiftPlane(Plane&, Plane*, ScalarEntityDistance*);
    void shiftSphere(Sphere&, Plane*, ScalarEntityDistance*);

    Plane* getPlane();
    ScalarEntityDistance* getDistance();

};

#endif // P_TRANSLATEBYPLANE_H
