#ifndef SIMPLEPOLARMEASUREMENT_H
#define SIMPLEPOLARMEASUREMENT_H

#include "simulationmodel.h"
#include <random>
#include <math.h>
#include <QDebug>
#include <QtAlgorithms>

/*!
 * \brief The SimplePolarMeasurement class
 *
 *  implementation of a simple simulation of a
 *  polar measurement system such as a Laser Tracker
 *  or Total Station. The Sensor model is described by
 *  Ben Hughes(Laser tracker error determination using
 *  a network measurement)
 */
class SimplePolarMeasurement : public SimulationModel
{
public:
    SimplePolarMeasurement();

    /*!
     * \brief getMetaData
     * \return
     *
     * define here all information about your plugin
     */
    PluginMetaData* getMetaData();


    /*!
     * \brief getSensorUncertainties
     * \return
     *
     * define all sensor uncertainties here
     */
    QMap<QString,UncertaintyComponent> getSensorUncertainties();

    /*!
     * \brief getObjectUncertainties
     * \return
     *
     *  define all object uncertainties here
     */
    QMap<QString,UncertaintyComponent> getObjectUncertainties();

    /*!
     * \brief getEnviromentUncertainties
     * \return
     *
     * define all enviroment uncertainties here
     */
    QMap<QString, UncertaintyComponent> getEnviromentUncertainties();

    /*!
     * \brief getHumanInfluence
     * \return
     *
     * define all influences of an User here
     */
    QMap<QString,UncertaintyComponent> getHumanInfluence();


    //methodes to define custom setting parameter
    QMap<QString,int>* getIntegerParameter();
    QMap<QString,double>* getDoubleParameter();
    QMap <QString, QStringList>* getStringParameter();

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
    bool distort(Reading *r, OiMat objectRelation,bool newIterationStart);

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
    bool analyseSimulationData(UncertaintyData &d);

    /*!
     * \brief getCorrelationCoefficient
     * \param x
     * \param y
     * \return
     *
     * determine the correlation coefficient of the two quantities x and y.
     */
    double getCorrelationCoefficient(QList<double>x,QList<double>y);


private:

    //all supported distributions;
    QStringList distributions;

    double variance;
    double expectation;

    bool newIteration;

    double refraction;
    double distortedRefraction;
    double verticalDn;
    double horizontalDn;

    double ref_coefficientOfExpansion;
    double ref_materialTemperature;
    double coefficientOfExpansion;
    double materialTemperature;

    double distortComponent(UncertaintyComponent u);


    bool distortionBySensor(Reading * r);
    bool distortionByEnviroment(Reading *r);
    bool distortionByHuman(Reading *r);
    bool distortionByObject(Reading *r, OiMat objectRelation);

    void checkDistribution(UncertaintyData &d);
    void calcUncertainty(UncertaintyData &d);

    double edlenRefractionCalculation(double temperature,double pressure, double humidity, double wavelength);



};

#endif // SIMPLEPOLARMEASUREMENT_H
