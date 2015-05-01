#include "controller.h"

/*!
 * \brief Controller::Controller
 * \param parent
 */
Controller::Controller(QObject *parent) : QObject(parent){

    //initialize model manager
    ModelManager::init();

    //create default job
    this->createDefaultJob();

    //initialize display configs
    this->initDisplayConfigs();

}

/*!
 * \brief Controller::addFeatures
 * \param attributes
 */
void Controller::addFeatures(const FeatureAttributes &attributes){

    Console::getInstance()->addLine("test output");

    //check job
    if(this->job.isNull()){
        Console::getInstance()->addLine("No active job");
        return;
    }

    this->job->addFeatures(attributes);

}

/*!
 * \brief Controller::importNominals
 * \param params
 */
void Controller::importNominals(const ExchangeParams &params){
    this->exchanger.importData(params);
}

/*!
 * \brief Controller::setFeatureTableColumnConfig
 * \param config
 */
void Controller::setFeatureTableColumnConfig(const FeatureTableColumnConfig &config){
    ModelManager::setFeatureTableColumnConfig(config);
}

/*!
 * \brief Controller::setTrafoParamColumnConfig
 * \param config
 */
void Controller::setTrafoParamColumnConfig(const TrafoParamTableColumnConfig &config){
    ModelManager::setTrafoParamColumnConfig(config);
}

/*!
 * \brief Controller::setParameterDisplayConfig
 * \param config
 */
void Controller::setParameterDisplayConfig(const ParameterDisplayConfig &config){
    ModelManager::setParameterDisplayConfig(config);
}

/*!
 * \brief Controller::createDefaultJob
 */
void Controller::createDefaultJob(){

    //create job with a station and a nominal system
    QPointer<OiJob> job = new OiJob();
    this->setJob(job);

    QPointer<FeatureWrapper> stationFeature = new FeatureWrapper();
    QPointer<Station> station = new Station();
    station->setFeatureName("STATION01");
    stationFeature->setStation(station);

    QPointer<FeatureWrapper> systemFeature = new FeatureWrapper();
    QPointer<CoordinateSystem> system = new CoordinateSystem();
    system->setFeatureName("PART");
    systemFeature->setCoordinateSystem(system);

    job->addFeature(stationFeature);
    job->addFeature(systemFeature);

}

/*!
 * \brief Controller::setJob
 * \param job
 */
void Controller::setJob(const QPointer<OiJob> &job){

    //check job
    if(job.isNull()){
        return;
    }

    //delete old job
    if(!this->job.isNull()){
        delete this->job;
    }

    //set and connect new job
    this->job = job;

    //active feature changes
    QObject::connect(this->job.data(), SIGNAL(activeFeatureChanged()), this, SIGNAL(activeFeatureChanged()), Qt::AutoConnection);
    QObject::connect(this->job.data(), SIGNAL(activeStationChanged()), this, SIGNAL(activeStationChanged()), Qt::AutoConnection);
    QObject::connect(this->job.data(), SIGNAL(activeCoordinateSystemChanged()), this, SIGNAL(activeCoordinateSystemChanged()), Qt::AutoConnection);

    //feature(s) added or removed
    QObject::connect(this->job.data(), SIGNAL(featureSetChanged()), this, SIGNAL(featureSetChanged()), Qt::AutoConnection);
    QObject::connect(this->job.data(), SIGNAL(coordSystemSetChanged()), this, SIGNAL(coordSystemSetChanged()), Qt::AutoConnection);
    QObject::connect(this->job.data(), SIGNAL(stationSetChanged()), this, SIGNAL(stationSetChanged()), Qt::AutoConnection);
    QObject::connect(this->job.data(), SIGNAL(trafoParamSetChanged()), this, SIGNAL(trafoParamSetChanged()), Qt::AutoConnection);
    QObject::connect(this->job.data(), SIGNAL(geometrySetChanged()), this, SIGNAL(geometrySetChanged()), Qt::AutoConnection);

    //group(s) added or removed
    QObject::connect(this->job.data(), SIGNAL(availableGroupsChanged()), this, SIGNAL(availableGroupsChanged()), Qt::AutoConnection);
    QObject::connect(this->job.data(), SIGNAL(activeGroupChanged()), this, SIGNAL(activeGroupChanged()), Qt::AutoConnection);

    //general feature attributes changed
    QObject::connect(this->job.data(), SIGNAL(featureAttributesChanged()), this, SIGNAL(featureAttributesChanged()), Qt::AutoConnection);

    //feature specific attributes changed
    QObject::connect(this->job.data(), SIGNAL(featureNameChanged(const int&, const QString&)),
                     this, SIGNAL(featureNameChanged(const int&, const QString&)), Qt::AutoConnection);
    QObject::connect(this->job.data(), SIGNAL(featureGroupChanged(const int&, const QString&)),
                     this, SIGNAL(featureGroupChanged(const int&, const QString&)), Qt::AutoConnection);
    QObject::connect(this->job.data(), SIGNAL(featureCommentChanged(const int&)),
                     this, SIGNAL(featureCommentChanged(const int&)), Qt::AutoConnection);
    QObject::connect(this->job.data(), SIGNAL(featureIsUpdatedChanged(const int&)),
                     this, SIGNAL(featureIsUpdatedChanged(const int&)), Qt::AutoConnection);
    QObject::connect(this->job.data(), SIGNAL(featureIsSolvedChanged(const int&)),
                     this, SIGNAL(featureIsSolvedChanged(const int&)), Qt::AutoConnection);
    QObject::connect(this->job.data(), SIGNAL(featureFunctionsChanged(const int&)),
                     this, SIGNAL(featureFunctionsChanged(const int&)), Qt::AutoConnection);
    QObject::connect(this->job.data(), SIGNAL(featureUsedForChanged(const int&)),
                     this, SIGNAL(featureUsedForChanged(const int&)), Qt::AutoConnection);
    QObject::connect(this->job.data(), SIGNAL(featurePreviouslyNeededChanged(const int&)),
                     this, SIGNAL(featurePreviouslyNeededChanged(const int&)), Qt::AutoConnection);

    //geometry specific attributes changed
    QObject::connect(this->job.data(), SIGNAL(geometryIsCommonChanged(const int&)),
                     this, SIGNAL(geometryIsCommonChanged(const int&)), Qt::AutoConnection);
    QObject::connect(this->job.data(), SIGNAL(geometryNominalsChanged(const int&)),
                     this, SIGNAL(geometryNominalsChanged(const int&)), Qt::AutoConnection);
    QObject::connect(this->job.data(), SIGNAL(geometryActualChanged(const int&)),
                     this, SIGNAL(geometryActualChanged(const int&)), Qt::AutoConnection);
    QObject::connect(this->job.data(), SIGNAL(geometryObservationsChanged(const int&)),
                     this, SIGNAL(geometryObservationsChanged(const int&)), Qt::AutoConnection);
    QObject::connect(this->job.data(), SIGNAL(geometryNominalSystemChanged(const int&)),
                     this, SIGNAL(geometryNominalSystemChanged(const int&)), Qt::AutoConnection);
    QObject::connect(this->job.data(), SIGNAL(geometryStatisticChanged(const int&)),
                     this, SIGNAL(geometryStatisticChanged(const int&)), Qt::AutoConnection);
    QObject::connect(this->job.data(), SIGNAL(geometrySimulationDataChanged(const int&)),
                     this, SIGNAL(geometrySimulationDataChanged(const int&)), Qt::AutoConnection);
    QObject::connect(this->job.data(), SIGNAL(geometryMeasurementConfigChanged(const int&)),
                     this, SIGNAL(geometryMeasurementConfigChanged(const int&)), Qt::AutoConnection);

    //coordinate system specific attributes changed
    QObject::connect(this->job.data(), SIGNAL(systemObservationsChanged(const int&, const int&)),
                     this, SIGNAL(systemObservationsChanged(const int&, const int&)), Qt::AutoConnection);
    QObject::connect(this->job.data(), SIGNAL(systemTrafoParamsChanged(const int&)),
                     this, SIGNAL(systemTrafoParamsChanged(const int&)), Qt::AutoConnection);
    QObject::connect(this->job.data(), SIGNAL(systemsNominalsChanged(const int&)),
                     this, SIGNAL(systemsNominalsChanged(const int&)), Qt::AutoConnection);

    //station specific attributes changed
    QObject::connect(this->job.data(), SIGNAL(stationSensorChanged(const int&)),
                     this, SIGNAL(stationSensorChanged(const int&)), Qt::AutoConnection);

    //transformation parameter specific attributes changed
    QObject::connect(this->job.data(), SIGNAL(trafoParamParametersChanged(const int&)),
                     this, SIGNAL(trafoParamParametersChanged(const int&)), Qt::AutoConnection);
    QObject::connect(this->job.data(), SIGNAL(trafoParamSystemsChanged(const int&)),
                     this, SIGNAL(trafoParamSystemsChanged(const int&)), Qt::AutoConnection);
    QObject::connect(this->job.data(), SIGNAL(trafoParamIsUsedChanged(const int&)),
                     this, SIGNAL(trafoParamIsUsedChanged(const int&)), Qt::AutoConnection);
    QObject::connect(this->job.data(), SIGNAL(trafoParamValidTimeChanged(const int&)),
                     this, SIGNAL(trafoParamValidTimeChanged(const int&)), Qt::AutoConnection);
    QObject::connect(this->job.data(), SIGNAL(trafoParamIsMovementChanged(const int&)),
                     this, SIGNAL(trafoParamIsMovementChanged(const int&)), Qt::AutoConnection);

    //pass the new job around
    ModelManager::setCurrentJob(this->job);
    this->exchanger.setCurrentJob(this->job);

}

/*!
 * \brief Controller::initDisplayConfigs
 * Sets up initial display configs and passes them to model manager
 */
void Controller::initDisplayConfigs(){

    //create default configs
    FeatureTableColumnConfig featureTableColumnConfig;
    TrafoParamTableColumnConfig trafoParamTableColumnConfig;
    ParameterDisplayConfig parameterDisplayConfig;

    //pass the default configs to model manager
    ModelManager::setFeatureTableColumnConfig(featureTableColumnConfig);
    ModelManager::setTrafoParamColumnConfig(trafoParamTableColumnConfig);
    ModelManager::setParameterDisplayConfig(parameterDisplayConfig);

}
