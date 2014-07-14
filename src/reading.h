#ifndef READING_H
#define READING_H

#include <QDateTime>
#include <QtCore/qmath.h>
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
    double RX;
    double RY;
    double RZ;
    double sigmaRX;
    double sigmaRY;
    double sigmaRZ;
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

    void makeBackup();
    void restoreBackup();

    //error propagation
    OiVec errorPropagationPolarToCart();

private:

    sReadingPolar backupPolar;
    sReadingCartesian backupCartesian;
    sReadingDirection backupDirection;
    sReadingDistance backupDistance;
    sReadingTemperature backupTemperature;
    sReadingUndefined backupUndefined;
    sReadingLevel backupLevel;
};

#endif // READING_H
