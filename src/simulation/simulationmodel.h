#ifndef SIMULATIONMODEL_H
#define SIMULATIONMODEL_H

#include <QMap>
#include "pluginmetadata.h"
#include "uncertaintycomponent.h"
#include "reading.h"
#include "trafoparam.h"
#include "simulationdata.h"

/*!
 * \brief The Uncertainties struct
 *
 *  the self defined uncertainties which are edited by the user will be
 *  stored here
 */
struct Uncertainties{

    QMap<QString,UncertaintyComponent> sensorUncertainties;
    QMap<QString,UncertaintyComponent> objectUncertainties;
    QMap<QString,UncertaintyComponent> enviromentUncertainties;
    QMap<QString,UncertaintyComponent> humanUncertainties;


};

/*!
 * \brief The SimulationModel class
 *
 * Interface for creating a uncertainty model for a simulation in OpenIndy.
 * you have to overwrite the four major error influences (sensor,object,enviroment and human).
 * the error depending on the measurement methode will be descriped by the user and openindy
 */
class SimulationModel
{

public:

    virtual ~SimulationModel(){}

    /*!
     * \brief getMetaData
     * \return
     *
     * define here all information about your plugin
     */
    virtual PluginMetaData* getMetaData() = 0;


    /*!
     * \brief getSensorUncertainties
     * \return
     *
     * define all sensor uncertainties here
     */
    virtual QMap<QString,UncertaintyComponent> getSensorUncertainties() = 0;

    /*!
     * \brief getObjectUncertainties
     * \return
     *
     *  define all object uncertainties here
     */
    virtual QMap<QString,UncertaintyComponent> getObjectUncertainties() = 0;

    /*!
     * \brief getEnviromentUncertainties
     * \return
     *
     * define all enviroment uncertainties here
     */
    virtual QMap<QString, UncertaintyComponent> getEnviromentUncertainties() = 0;

    /*!
     * \brief getHumanInfluence
     * \return
     *
     * define all influences of an User here
     */
    virtual QMap<QString,UncertaintyComponent> getHumanInfluence() = 0;


    //methodes to define custom setting parameter
    virtual QMap<QString,int>* getIntegerParameter() = 0;
    virtual QMap<QString,double>* getDoubleParameter() = 0;
    virtual QMap <QString, QStringList>* getStringParameter() = 0;

    /*!
     * \brief distort
     * \param r
     * \param objectRelation
     * \return
     *
     *  here you have to distort a reading with the given uncertainties.
     *  objectRelation is a homogenous matrix (4x4) which describes the
     *  relation between Station and Object
     */
    virtual bool distort(Reading *r, OiMat objectRelation, bool newIterationStart) = 0;

    /*!
     * \brief analyseSimulationData
     * \param d
     * \return
     *
     * analyse the simulation values saved in d.values. store your results
     * in:
     *  d.maxValues = Maximum
     *  d.minValue = Minimum
     *  d.uncertainty = Uncertainty of the data series
     *  d.distribution = name of the distribution of the data series
     *  d.densityFunction = set the pointer to a density function (double densityFunction(double x))
     *  d.distributionFunction =  set the pointer to a distribution function (double distributionFunction(double x))
     *  d.info = a map to define custom information of your analysis
     */
    virtual bool analyseSimulationData(UncertaintyData &d) = 0;

    /*!
     * \brief getCorrelationCoefficient
     * \param x
     * \param y
     * \return
     *
     * determine the correlation coefficient of the two quantities x and y.
     */
    virtual double getCorrelationCoefficient(QList<double>x,QList<double>y) = 0;

    //getter & setter
    Uncertainties getGivenUncertainties(){return givenUncertainties;}
    void setGivenUncertainties(Uncertainties u){givenUncertainties = u;}

    void setIntegerParameter(QMap<QString,int>* m){this->integerParameter = m;}
    void setDoubleParameter(QMap<QString,double>* m) {this->doubleParameter = m;}
    void setStringParameter(QMap <QString, QString>* m) {this->stringParameter = m;}


protected:

    Uncertainties givenUncertainties;

    QMap<QString,int>* integerParameter;
    QMap<QString,double>* doubleParameter;
    QMap <QString, QString>* stringParameter;



};

#define Simulation_iidd "de.openIndy.Plugin.Simulation.v001"

#endif // SIMULATIONMODEL_H
