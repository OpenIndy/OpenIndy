#ifndef SIMULATIONMODEL_H
#define SIMULATIONMODEL_H

#include <QMap>
#include "pluginmetadata.h"
#include "uncertaintycomponent.h"
#include "reading.h"

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

    virtual ~SimulationModel();

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
     * \return
     *
     *  here you have to distort a reading with the given uncertainties
     */
    virtual bool distort(Reading *r) = 0;

    //getter & setter
    Uncertainties getGivenUncertainties(){return givenUncertainties;}
    void setGivenUncertainties(Uncertainties u){givenUncertainties = u;}

protected:

    Uncertainties givenUncertainties;

};

#endif // SIMULATIONMODEL_H
