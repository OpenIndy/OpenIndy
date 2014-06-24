#ifndef SIMPLEPOLARMEASUREMENT_H
#define SIMPLEPOLARMEASUREMENT_H

#include "simulationmodel.h"
#include <random>

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
    bool distort(Reading *r, OiMat objectRelation);

private:

    //all supported distributions;
    QStringList distributions;


    double distortComponent(UncertaintyComponent u);


    bool distortionBySensor(Reading * r);
    bool distortionByEnviroment(Reading *r);
    bool distortionByHuman(Reading *r);
    bool distortionByObject(Reading *r, OiMat objectRelation);


};

#endif // SIMPLEPOLARMEASUREMENT_H
