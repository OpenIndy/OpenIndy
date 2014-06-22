#ifndef SIMPLEPOLARMEASUREMENT_H
#define SIMPLEPOLARMEASUREMENT_H

#include "simulationmodel.h"
#include <random>

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
     * \return
     *
     *  here you have to distort a reading with the given uncertainties
     */
    bool distort(Reading *r);

private:

    std::default_random_engine *generator;

    std::uniform_real_distribution<double> *dist_Uniform;
    std::normal_distribution<double> *dist_Normal;



};

#endif // SIMPLEPOLARMEASUREMENT_H
