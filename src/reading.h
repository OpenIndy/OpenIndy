#ifndef READING_H
#define READING_H

#include <QDateTime>
#include <QtCore/qmath.h>
#include <QtXml>
#include <QPointer>

#include "element.h"
#include "measurementconfig.h"
#include "oivec.h"
#include "types.h"
#include "util.h"

class Sensor;
class Observation;

//###########################
//definition of reading types
//###########################

class ReadingPolar{
public:
    double azimuth;
    double zenith;
    double distance;
    double sigmaAzimuth;
    double sigmaZenith;
    double sigmaDistance;
    bool isValid;
};

class ReadingDirection{
public:
    double azimuth;
    double zenith;
    double sigmaAzimuth;
    double sigmaZenith;
    bool isValid;
};

class ReadingDistance{
public:
    double distance;
    double sigmaDistance;
    bool isValid;
};

class ReadingCartesian{
public:
    OiVec xyz;
    OiVec sigmaXyz;
    bool isValid;
};

class ReadingTemperature{
public:
    double tempDeg;
    double sigmaTempDeg;
    bool isValid;
};

class ReadingUndefined{
public:
    QMap<QString,double> values;
    QMap<QString,double> sigmaValues;
    bool isValid;
};

class ReadingLevel{
public:
    double RX;
    double RY;
    double RZ;
    double sigmaRX;
    double sigmaRY;
    double sigmaRZ;
    bool isValid;
};

/*!
 * \brief The Reading class
 */
class Reading : public Element
{
public:
    Reading();
    ~Reading();

    //###########################
    //get reading type and values
    //###########################

    const ReadingTypes &getTypeOfReading() const;

    const ReadingPolar &getPolarReading() const;
    const ReadingCartesian &getCartesianReading() const;
    const ReadingDirection &getDirectionReading() const;
    const ReadingDistance &getDistanceReading() const;
    const ReadingTemperature &getTemperatureReading() const;
    const ReadingLevel &getLevelReading() const;
    const ReadingUndefined &getUndefinedReading() const;

    //################################
    //make and restore reading backups
    //################################

    void makeBackup();
    void restoreBackup();

    //############################################
    //convert between polar and cartesian readings
    //############################################

    bool toCartesian();
    bool toPolar();

    static OiVec toCartesian(const double &azimuth, const double &zenith, const double &distance);
    static OiVec toPolar(const double &x, const double &y, const double &z);

    OiVec errorPropagationPolarToCartesian();

    //#########################################
    //get general information about the reading
    //#########################################

    const MeasurementConfig &getMeasurementConfig();
    void setMeasurementConfig(const MeasurementConfig &mConfig);

    //#################################################
    //get and set references to sensor and observations
    //#################################################

    const QPointer<Sensor> &getSensor() const;
    void setSensor(const QPointer<Sensor> &sensor);
    const QPointer<Observation> &getObservation() const;
    void setObservation(const QPointer<Observation> &observation);

    //#################
    //save and load XML
    //#################

    QDomElement toOpenIndyXML(QDomDocument &xmlDoc) const;
    bool fromOpenIndyXML(QDomElement &xmlElem);

private:

    //#######################
    //reading type and values
    //#######################

    ReadingTypes typeOfReading;

    ReadingPolar rPolar;
    ReadingCartesian rCartesian;
    ReadingDirection rDirection;
    ReadingDistance rDistance;
    ReadingTemperature rTemperature;
    ReadingLevel rLevel;
    ReadingUndefined rUndefined;

    //###################################
    //backup readings used for simulation
    //###################################

    ReadingPolar backupPolar;
    ReadingCartesian backupCartesian;
    ReadingDirection backupDirection;
    ReadingDistance backupDistance;
    ReadingTemperature backupTemperature;
    ReadingLevel backupLevel;
    ReadingUndefined backupUndefined;

    //##########################
    //general reading attributes
    //##########################

    QDateTime measuredAt;
    MeasurementConfig mConfig; //the configuration that the sensor used to produce this reading
    SensorFaces face;

    //####################################
    //references to sensor and observation
    //####################################

    QPointer<Sensor> instrument;
    QPointer<Observation> observation;

};

#endif // READING_H
