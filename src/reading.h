#ifndef READING_H
#define READING_H

#include <QDateTime>
#include <QtCore/qmath.h>
#include <QtXml>

#include "measurementconfig.h"
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
    bool isValid;
};

struct sReadingDirection{
    double azimuth;
    double zenith;
    double sigmaAzimuth;
    double sigmaZenith;
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

    const MeasurementConfig &getMeasurementConfig();
    void setMeasurementConfig(const MeasurementConfig &mConfig);

    Configuration::Faces face;  //shows in which sight it was measured

    void toCartesian();
    static OiVec toCartesian(double, double, double);
    void toPolar();
    static OiVec toPolar(double, double, double);

    void makeBackup();
    void restoreBackup();

    //error propagation
    OiVec errorPropagationPolarToCart();

    QDomElement toOpenIndyXML(QDomDocument &xmlDoc) const;
    bool fromOpenIndyXML(QDomElement &xmlElem);

private:
    //save backup readings while calculating a simulation (reading are randomly shuffled)
    sReadingPolar backupPolar;
    sReadingCartesian backupCartesian;
    sReadingDirection backupDirection;
    sReadingDistance backupDistance;
    sReadingTemperature backupTemperature;
    sReadingUndefined backupUndefined;
    sReadingLevel backupLevel;

    MeasurementConfig myMeasurementConfig; //the configuration that the sensor used to produce this reading
};

#endif // READING_H
