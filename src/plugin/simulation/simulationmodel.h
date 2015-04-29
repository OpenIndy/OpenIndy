#ifndef SIMULATIONMODEL_H
#define SIMULATIONMODEL_H

#include <QObject>
#include <QPointer>
#include <QMap>
#include <QMultiMap>
#include <QString>

#include "pluginmetadata.h"
#include "reading.h"
#include "position.h"
#include "direction.h"
#include "radius.h"

//##############
//helper classes
//##############

/*!
 * \brief The UncertaintyComponent class
 * Describe an uncertainty component of a simulation model
 */
class UncertaintyComponent
{
public:
    QString name;

    double value; //magnitude of the error
    double uncertainty; //uncertainty of the error
    QString distribution; //the distribution of the uncertainty component

    QString errorUnit; //unit of the uncertainty value (mm, m ,...)

    QString description;

    QStringList distributions; //distributions in which the error could be determined at random
};

/*!
 * \brief The Uncertainties class
 */
class Uncertainties{
public:
    QMap<QString, UncertaintyComponent> sensorUncertainties;
    QMap<QString, UncertaintyComponent> objectUncertainties;
    QMap<QString, UncertaintyComponent> enviromentUncertainties;
    QMap<QString, UncertaintyComponent> humanUncertainties;
};

/*!
 * \brief The UncertaintyData class
 * Save information about simulation results for one geometry parameter
 */
class UncertaintyData{
public:
    QList<double> values; //randomly shuffled values produced distortion of readings and recalculation

    //maximum and minimum of the data series
    double maxValue;
    double minValue;

    double expectation;
    double uncertainty; //uncertainty of the data series

    QString distribution; //name of the distribution of the data series

    //function pointers that calculate density and distribution values
    double (*densityFunction)(const double &x, const double &expectation, const double &uncertainty,
                              const double &lowerLimit, const double &upperLimit);
    double (*distributionFunction)(const double &x, const double &expectation, const double &uncertainty,
                                   const double &lowerLimit, const double &upperLimit);

    QMap<QString, QString> info; //custom information

};

/*!
 * \brief The SimulationData class
 * Save simulation results for a geometry
 */
class SimulationData
{
public:

    //####################################################
    //uncertainty data for each unknown geometry parameter
    //####################################################

    UncertaintyData uncertaintyX;
    UncertaintyData uncertaintyY;
    UncertaintyData uncertaintyZ;

    UncertaintyData uncertaintyPrimaryI;
    UncertaintyData uncertaintyPrimaryJ;
    UncertaintyData uncertaintyPrimaryK;

    UncertaintyData uncertaintySecondaryI;
    UncertaintyData uncertaintySecondaryJ;
    UncertaintyData uncertaintySecondaryK;

    UncertaintyData uncertaintyRadiusA;
    UncertaintyData uncertaintyRadiusB;

    UncertaintyData uncertaintyAperture;

    UncertaintyData uncertaintyAngle;
    UncertaintyData uncertaintyDistance;
    UncertaintyData uncertaintyMeasurementSeries;
    UncertaintyData uncertaintyTemperature;

    UncertaintyData uncertaintyLength;

    //###############################
    //correlations between parameters
    //###############################

    QMap<QString, double> correlations;

};

//TODO set simulation config as separate class like sensor config an add xml save/load

/*!
 * \brief The SimulationConfiguration class
 */
class SimulationConfiguration{
public:
    Uncertainties uncertainties;

    QMap<QString, int> integerParameters;
    QMap<QString, double> doubleParameters;
    QMap<QString, QString> stringParameters;
};

//#######################
//simulation class itself
//#######################

/*!
 * \brief The SimulationModel class
 * Interface for creating an uncertainty model for a simulation in OpenIndy.
 * You can provide four major error influences (sensor, object, enviroment and human).
 */
class SimulationModel : public QObject
{
    Q_OBJECT

public:
    explicit SimulationModel(QObject *parent = 0);

    virtual ~SimulationModel();

    //################################
    //simulation initialization method
    //################################

    virtual void init();

    //#########################################
    //methods to get or set further information
    //#########################################

    const PluginMetaData &getMetaData() const;

    const SimulationConfiguration &getSimulationConfiguration() const;
    void setSimulationConfiguration(const SimulationConfiguration &sConfig);

    const QMap<QString, int> &getIntegerParameter() const;
    const QMap<QString, double> &getDoubleParameter() const;
    const QMultiMap<QString, QString> &getStringParameter() const;

    //##################
    //simulation methods
    //##################

    const QMap<QString, UncertaintyComponent> &getSensorUncertainties() const;
    const QMap<QString, UncertaintyComponent> &getObjectUncertainties() const;
    const QMap<QString, UncertaintyComponent> &getEnviromentUncertainties() const;
    const QMap<QString, UncertaintyComponent> &getHumanInfluence() const;

    virtual bool distort(const QPointer<Reading> &r, const OiMat &objectRelation, const bool &newIterationStart);
    virtual bool analyseSimulationData(UncertaintyData &d);
    virtual double getCorrelationCoefficient(const QList<double> &x, const QList<double> &y);

signals:

    //##################################################
    //signals to inform OpenIndy about simulation issues
    //##################################################

    void sendMessage(const QString &msg);

protected:

    //###########################
    //input and output parameters
    //###########################

    SimulationConfiguration sConfig;

    PluginMetaData metaData;

    //the needed scalar input parameters including default values
    QMap<QString, int> integerParameters;
    QMap<QString, double> doubleParameters;
    QMultiMap<QString, QString> stringParameters;

    //uncertainties provided by plugin writer
    QMap<QString, UncertaintyComponent> sensorUncertainties;
    QMap<QString, UncertaintyComponent> objectUncertainties;
    QMap<QString, UncertaintyComponent> environmentUncertainties;
    QMap<QString, UncertaintyComponent> humanInfluence;

};

#define Simulation_iidd "de.openIndy.plugin.simulation.v001"

#endif // SIMULATIONMODEL_H
