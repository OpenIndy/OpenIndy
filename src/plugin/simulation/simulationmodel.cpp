#include "simulationmodel.h"

/*!
 * \brief SimulationModel::SimulationModel
 * \param parent
 */
SimulationModel::SimulationModel(QObject *parent) : QObject(parent){

}

/*!
 * \brief SimulationModel::~SimulationModel
 */
SimulationModel::~SimulationModel(){

}

/*!
 * \brief SimulationModel::getMetaData
 * \return
 */
const PluginMetaData &SimulationModel::getMetaData() const{
    return this->metaData;
}

/*!
 * \brief SimulationModel::getSimulationConfiguration
 * \return
 */
const SimulationConfiguration &SimulationModel::getSimulationConfiguration() const{
    return this->sConfig;
}

/*!
 * \brief SimulationModel::setSimulationConfiguration
 * \param sConfig
 */
void SimulationModel::setSimulationConfiguration(const SimulationConfiguration &sConfig){
    this->sConfig = sConfig;
}

/*!
 * \brief SimulationModel::getIntegerParameter
 * \return
 */
const QMap<QString, int> &SimulationModel::getIntegerParameter() const{
    return this->integerParameters;
}

/*!
 * \brief SimulationModel::getDoubleParameter
 * \return
 */
const QMap<QString, double> &SimulationModel::getDoubleParameter() const{
    return this->doubleParameters;
}

/*!
 * \brief SimulationModel::getStringParameter
 * \return
 */
const QMultiMap<QString, QString> &SimulationModel::getStringParameter() const{
    return this->stringParameters;
}

/*!
 * \brief SimulationModel::getSensorUncertainties
 * \return
 */
const QMap<QString, UncertaintyComponent> &SimulationModel::getSensorUncertainties() const{
    return this->sensorUncertainties;
}

/*!
 * \brief SimulationModel::getObjectUncertainties
 * \return
 */
const QMap<QString, UncertaintyComponent> &SimulationModel::getObjectUncertainties() const{
    return objectUncertainties;
}

/*!
 * \brief SimulationModel::getEnviromentUncertainties
 * \return
 */
const QMap<QString, UncertaintyComponent> &SimulationModel::getEnviromentUncertainties() const{
    return environmentUncertainties;
}

/*!
 * \brief SimulationModel::getHumanInfluence
 * \return
 */
const QMap<QString, UncertaintyComponent> &SimulationModel::getHumanInfluence() const{
    return this->humanInfluence;
}

/*!
 * \brief SimulationModel::distort
 * Distort a reading with the given uncertainties
 * \param r
 * \param objectRelation homogeneous matrix (4x4) which describes the relation between station and object
 * \param newIterationStart
 * \return
 */
bool SimulationModel::distort(const QPointer<Reading> &r, const OiMat &objectRelation, const bool &newIterationStart){
    emit this->sendMessage(QString("Simulation Model not implemented correctly: distort method missing"));
    return false;
}

/*!
 * \brief SimulationModel::analyseSimulationData
 * Analyse the simulation values saved in d.values
 * \param d
 * \return
 */
bool SimulationModel::analyseSimulationData(UncertaintyData &d){
    emit this->sendMessage(QString("Simulation Model not implemented correctly: analyse method missing"));
    return false;
}

/*!
 * \brief SimulationModel::getCorrelationCoefficient
 * Determine the correlation coefficient of the two quantities x and y.
 * \param x
 * \param y
 * \return
 */
double SimulationModel::getCorrelationCoefficient(const QList<double> &x, const QList<double> &y){
    emit this->sendMessage(QString("Simulation Model not implemented correctly: correlation method missing"));
    return 0.0;
}

/*!
 * \brief SimulationModel::init
 */
void SimulationModel::init(){

}
