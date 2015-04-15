#ifndef SENSORCONFIGURATION_H
#define SENSORCONFIGURATION_H

#include <QPointer>
#include <QtXml>
#include <QString>
#include <QMap>
#include <QtSerialPort/QSerialPort>

#include "types.h"
#include "oivec.h"

class Sensor;

//##############
//helper classes
//##############

/*!
 * \brief The ConnectionConfig class
 */
class ConnectionConfig
{
public:
    ConnectionConfig();

    ConnectionTypes typeOfConnection;

    QString ip;
    QString port;
    QString comPort;
    QSerialPort::BaudRate baudRate;
    QSerialPort::DataBits dataBits;
    QSerialPort::Parity parity;
    QSerialPort::StopBits stopBits;
    QSerialPort::FlowControl flowControl;
};

/*!
 * \brief The Accuracy struct
 */
struct Accuracy{

    Accuracy() : sigmaXyz(OiVec(3)), sigmaAzimuth(0.0), sigmaZenith(0.0), sigmaDistance(0.0),
        sigmaTemp(0.0), sigmaAngleXZ(0.0), sigmaAngleYZ(0.0){}

    double sigmaAzimuth;
    double sigmaZenith;
    double sigmaDistance;
    OiVec sigmaXyz;
    double sigmaTemp;
    QMap<QString, double> sigmaUndefined;
    double sigmaAngleXZ;
    double sigmaAngleYZ;
};

//###########################
//sensor configuration itself
//###########################

/*!
 * \brief The SensorConfiguration class
 */
class SensorConfiguration
{
public:
    SensorConfiguration();

    SensorConfiguration(const SensorConfiguration &copy);

    SensorConfiguration &operator=(const SensorConfiguration &copy);

    /*
    //! compare the attributes of both sensor configs
    friend bool operator==(const SensorConfiguration &left, const SensorConfiguration &right){

        //TODO compare sensor config attributes

        return false;

    }*/

    //##########################################
    //get or set sensor configuration attributes
    //##########################################

    const QString &getName() const;
    bool setName(const QString &name);

    const bool &getIsSaved() const;

    bool getIsValid() const;

    const QPointer<Sensor> &getSensor() const;
    void setSensor(const QPointer<Sensor> &sensor);

    const SensorTypes &getTypeOfSensor() const;
    const QString &getPluginName() const;
    const QString &getSensorName() const;

    const Accuracy &getAccuracy() const;
    void setAccuracy(const Accuracy &accuracy);

    const ConnectionConfig &getConnectionConfig() const;
    void setConnectionConfig(const ConnectionConfig &cConfig);

    const QMap<QString, int> &getIntegerParameter() const;
    void setIntegerParameter(const QMap<QString, int> &intParams);

    const QMap<QString, double> &getDoubleParameter() const;
    void setDoubleParameter(const QMap<QString, double> &doubleParams);

    const QMap<QString, QString> &getStringParameter() const;
    void setStringParameter(const QMap<QString, QString> &stringParams);

    //#################
    //save and load XML
    //#################

    QDomElement toOpenIndyXML(QDomDocument &xmlDoc) const;
    bool fromOpenIndyXML(QDomElement &xmlElem);

private:

    //###############################
    //sensor configuration attributes
    //###############################

    QString name;
    bool isSaved;
    SensorTypes typeOfSensor;

    QString pluginName;
    QString sensorName;
    QPointer<Sensor> sensor;

    Accuracy accuracy;
    ConnectionConfig cConfig;

    QMap<QString, int> integerParameter;
    QMap<QString, double> doubleParameter;
    QMap<QString, QString> stringParameter;
};

#endif // SENSORCONFIGURATION_H
