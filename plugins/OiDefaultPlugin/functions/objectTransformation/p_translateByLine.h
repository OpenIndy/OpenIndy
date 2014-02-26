#ifndef P_TRANSLATEBYLINE_H
#define P_TRANSLATEBYLINE_H

#include <QtGlobal>
#include <QDateTime>
#include <QObject>
#include <QDebug>
#include <QThread>
#include <qmath.h>

#include "pi_objecttransformation.h"
#include "console.h"
#include "point.h"
#include "line.h"
#include "sphere.h"
#include "plane.h"
#include "scalarentitydistance.h"
#include "configuration.h"
#include "oivec.h"
#include "oimat.h"
#include "pluginmetadata.h"

using namespace std;

class TranslateByLine : public ObjectTransformation
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
    void shiftPoint(Point&, Line*, ScalarEntityDistance*);
    void shiftLine(Line&, Line*, ScalarEntityDistance*);
    void shiftPlane(Plane&, Line*, ScalarEntityDistance*);
    void shiftSphere(Sphere&, Line*, ScalarEntityDistance*);

    void variancePropagationPoint(Point&, Line*, ScalarEntityDistance*, bool invert);

    Line* getLine();
    ScalarEntityDistance* getDistance();

};

#endif // P_TRANSLATEBYLINE_H
