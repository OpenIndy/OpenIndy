#ifndef SENSOR_H
#define SENSOR_H

#include <QObject>
#include <QPointer>
#include <QString>
#include <QStringList>
#include <QMap>
#include <QVariant>
#include <QPair>

#include "pluginmetadata.h"
#include "sensorconfiguration.h"
#include "measurementconfig.h"
#include "types.h"
#include "util.h"
#include "reading.h"

/*!
 * \brief The SensorAttributes class
 */
class SensorAttributes{
public:
    double moveAzimuth;
    double moveZenith;
    double moveDistance;
    bool moveIsRelative;
    double moveX;
    double moveY;
    double moveZ;
    QString action;
};

class Reading;
class MeasurementConfig;
class SensorControl;

/*!
 * \brief The Sensor class
 */
class Sensor : public QObject
{
    Q_OBJECT

public:
    explicit Sensor(QObject *parent = 0);

    virtual ~Sensor();

    enum SensorMessage{
        eSensorOk,
        eSensorWarning,
        eSensorError
    };

    //############################
    //sensor initialization method
    //############################

    virtual void init();

    //####################################
    //get or set general sensor attributes
    //####################################

    const SensorConfiguration &getSensorConfiguration() const;
    void setSensorConfiguration(const SensorConfiguration &sConfig);

    const QPair<ReadingTypes, QPointer<Reading> > &getLastReading() const;

    //#########################################################
    //methods to get or set further information to use a sensor
    //#########################################################

    const QList<ReadingTypes> &getSupportedReadingTypes() const;
    const QList<SensorFunctions> &getSupportedSensorActions() const;
    const QList<ConnectionTypes> &getSupportedConnectionTypes() const;

    const PluginMetaData &getMetaData() const;

    const QMap<QString, int> &getIntegerParameter() const;
    const QMap<QString, double> &getDoubleParameter() const;
    const QMultiMap<QString, QString> &getStringParameter() const;

    const QStringList &getSelfDefinedActions() const;

    const QMap<QString, double> &getDefaultAccuracy() const;

    //########################
    //sensor state and actions
    //########################

    virtual bool accept(const SensorFunctions &method, const SensorAttributes &sAttr);

    virtual bool abortAction();

    virtual bool connectSensor();
    virtual bool disconnectSensor();

    virtual QList<QPointer<Reading> > measure(const MeasurementConfig &mConfig);
    virtual QVariantMap readingStream(const ReadingTypes &streamFormat);

    virtual bool getConnectionState();
    virtual bool getIsReadyForMeasurement();
    virtual bool getIsBusy();
    virtual QMap<QString, QString> getSensorStatus();

    virtual bool doSelfDefinedAction(const QString &action);

    //#################
    //save and load XML
    //#################

    QDomElement toOpenIndyXML(QDomDocument &xmlDoc) const;
    bool fromOpenIndyXML(QDomElement &xmlElem);

signals:

    //##############################################
    //signals to inform OpenIndy about sensor issues
    //##############################################

    void sensorMessage(const QString &msg);
    void sensorMessage(const SensorMessage &type, const QString &msg);

protected:

    //#########################
    //general sensor attributes
    //#########################

    SensorConfiguration sensorConfiguration;

    QPair<ReadingTypes, QPointer<Reading> > lastReading; //the last reading produced by this sensor

    //###########################
    //input and output parameters
    //###########################

    //meta information about the sensor
    QList<ReadingTypes> supportedReadingTypes;
    QList<SensorFunctions> supportedSensorActions;
    QList<ConnectionTypes> supportedConnectionTypes;
    QStringList selfDefinedActions;
    QMap<QString, double> defaultAccuracy;
    PluginMetaData metaData;

    //the needed scalar input parameters including default values
    QMap<QString, int> integerParameters;
    QMap<QString, double> doubleParameters;
    QMultiMap<QString, QString> stringParameters;

};

#define Sensor_iidd "de.openIndy.plugin.sensor.v001"

#endif // SENSOR_H
