#ifndef READING_H
#define READING_H

#include <QDateTime>
#include <QtCore/qmath.h>
#include "console.h"
#include "configuration.h"
#include "oivec.h"

class Sensor;
class Observation;

/*!
 * \brief The Reading class
 * Father class of all specific reading types.
 */

struct sReadingPolar{
    double azimuth;
    double zenith;
    double distance;
    double sigmaAzimuth;
    double sigmaZenith;
    double sigmaDistance;
    bool fsBs;
    bool isValid;
};

struct sReadingDirection{
    double azimuth;
    double zenith;
    double sigmaAzimuth;
    double sigmaZenith;
    bool fsBs;
    bool isValid;
};

struct sReadingDistance{
    double distance;
    double sigmaDistance;
    bool isValid;
};

struct sReadingCartesian{
    OiVec xyz;
    OiVec sigmaXyz;
    bool isValid;
};

struct sReadingTemperature{
    double tempDeg;
    double sigmaTempDeg;
    bool isValid;
};

struct sReadingUndefined{
    QMap<QString,double> values;
    QMap<QString,double> sigmaValues;
    bool isValid;
};

struct sReadingLevel{
    double angleXZ;
    double angleYZ;
    double diffXZ;
    double diffXY;
    double sigmaAngleXZ;
    double sigmaAngleYZ;
    bool isValid;
};

class Reading
{
public:
    Reading();
    ~Reading();

    int id;
    Configuration::ReadingTypes typeofReading;
    QDateTime measuredAt;
    Sensor *instrument;
    Observation *obs;

    //OiVec test;
    //double test;

    sReadingPolar rPolar;
    sReadingCartesian rCartesian;
    sReadingDirection rDirection;
    sReadingDistance rDistance;
    sReadingTemperature rTemperature;
    sReadingUndefined rUndefined;
    sReadingLevel rLevel;


    void toCartesian();
    static OiVec toCartesian(double, double, double);
    void toPolar();
    static OiVec toPolar(double, double, double);

    //error propagation
    OiVec errorPropagationPolarToCart();
};

#endif // READING_H
