#ifndef SENSORCONFIGURATION_H
#define SENSORCONFIGURATION_H

#include "configuration.h"
#include "connectionconfig.h"
#include "oivec.h"

#include <QXmlStreamWriter>
#include <QXmlStreamReader>

#include "elementdependencies.h"

struct Accuracy{

    Accuracy(){
        sigmaXyz = OiVec(3);
    }

    double sigmaAzimuth;
    double sigmaZenith;
    double sigmaDistance;
    OiVec sigmaXyz;
    double sigmaTemp;
    QMap<QString,double> sigmaUndefined;
    double sigmaAngleXZ;
    double sigmaAngleYZ;
};

class SensorConfiguration
{
public:
    SensorConfiguration();

    QString name;
    ConnectionConfig *connConfig;
    Configuration::SensorTypes instrumentType;

    QMap<QString, int> integerParameter;
    QMap<QString, double> doubleParameter;
    QMap<QString, QString> stringParameter;

    Accuracy sigma;

    //xml import export
    bool toOpenIndyXML(QXmlStreamWriter& stream);
    ElementDependencies fromOpenIndyXML(QXmlStreamReader& xml);

};

#endif // SENSORCONFIGURATION_H
