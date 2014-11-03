#ifndef P_CHANGERADIUS_H
#define P_CHANGERADIUS_H

#include <QtGlobal>
#include <QDateTime>
#include <QObject>
#include <QDebug>
#include <QThread>

#include "pi_objecttransformation.h"
#include "sphere.h"
#include "scalarentitydistance.h"
#include "configuration.h"
#include "oivec.h"
#include "oimat.h"
#include "pluginmetadata.h"

using namespace std;

class ChangeRadius : public ObjectTransformation
{  
public:
    PluginMetaData* getMetaData() const;
    bool exec(Sphere&);
    QList<InputParams> getNeededElements() const;
    QList<Configuration::FeatureTypes> applicableFor() const;

private:
    void attachOffsetToSphere(Sphere&, ScalarEntityDistance*);

    ScalarEntityDistance* getDistance();

};

#endif // P_CHANGERADIUS_H
