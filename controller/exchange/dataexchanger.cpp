#include "dataexchanger.h"

/*!
 * \brief DataExchanger::DataExchanger
 * \param parent
 */
DataExchanger::DataExchanger(QObject *parent) : QObject(parent){

}

/*!
 * \brief DataExchanger::~DataExchanger
 */
DataExchanger::~DataExchanger(){

    //stop thread if it is still running
    if(this->exchangeThread.isRunning()){
        this->exchangeThread.quit();
        this->exchangeThread.wait();
    }

}

/*!
 * \brief DataExchanger::getCurrentJob
 * \return
 */
const QPointer<OiJob> &DataExchanger::getCurrentJob() const{
    return this->currentJob;
}

/*!
 * \brief DataExchanger::setCurrentJob
 * \param job
 */
void DataExchanger::setCurrentJob(const QPointer<OiJob> &job){
    this->currentJob = job;
}

/*!
 * \brief DataExchanger::getMeasurementConfigManager
 * \return
 */
const QPointer<MeasurementConfigManager> &DataExchanger::getMeasurementConfigManager()
{
    return this->getMeasurementConfigManager();
}

/*!
 * \brief DataExchanger::setMesaurementConfigManager
 * \param manager
 */
void DataExchanger::setMesaurementConfigManager(const QPointer<MeasurementConfigManager> &manager)
{
    this->mConfigManager = manager;
}

/*!
 * \brief DataExchanger::setExchangeParams
 * \param exchangeParams
 */
void DataExchanger::setExchangeParams(ExchangeParams exchangeParams)
{
    this->exchangeParams = exchangeParams;
}

/*!
 * \brief DataExchanger::importData
 * \param params
 * \return
 */
bool DataExchanger::importData(const ExchangeParams &params){

    //quit the thread if it is still running
    if(this->exchangeThread.isRunning()){
        this->exchangeThread.quit();
        this->exchangeThread.wait();
    }

    //check current job
    if(this->currentJob.isNull()){
        emit this->sendMessage("No job specified", eErrorMessage, eMessageBoxMessage);
        return false;
    }

    //check nominal system
    QList<QPointer<FeatureWrapper> > features = this->currentJob->getFeaturesByName(params.nominalSystem);
    if(features.size() != 1 || features.at(0).isNull() || features.at(0)->getCoordinateSystem().isNull()
            || features.at(0)->getCoordinateSystem()->getIsStationSystem()){
        emit this->sendMessage("No valid nominal system", eErrorMessage, eMessageBoxMessage);
        return false;
    }

    //check if given plugin name is empty
    if(params.pluginName.compare("") == 0 || params.exchangeName.compare("") == 0){
        emit this->sendMessage(QString("No exchange available with the name %1 in the plugin %2").arg(params.exchangeName).arg(params.pluginName), eErrorMessage, eMessageBoxMessage);
        return false;
    }

    //get the plugin from database and check if it is valid
    sdb::Plugin plugin = SystemDbManager::getPlugin(params.pluginName);
    if(plugin.name.compare("") == 0){
        emit this->sendMessage("No valid plugin specified", eErrorMessage, eMessageBoxMessage);
        return false;
    }

    //load exchange method with plugin loader
    QPointer<ExchangeInterface> exchange(NULL);
    if(params.isDefinedFormat){
        QPointer<ExchangeDefinedFormat> definedFormat = PluginLoader::loadExchangeDefinedFormatPlugin(qApp->applicationDirPath() + "/" + plugin.file_path, params.exchangeName);
        if(!definedFormat.isNull()){
            definedFormat->setUsedElements(params.usedElements);
        }
        exchange = definedFormat;
    }else{
        QPointer<ExchangeSimpleAscii> simpleAscii = PluginLoader::loadExchangeSimpleAsciiPlugin(qApp->applicationDirPath() + "/" + plugin.file_path, params.exchangeName);
        if(!simpleAscii.isNull()){

            simpleAscii->setGeometryType(params.typeOfGeometry);
            simpleAscii->setSkipFirstLine(params.skipFirstLine);
            simpleAscii->setDelimiter(params.delimiter);

            //set user defined columns here hard coded (later selectable in table view)
            //TODO user defined columns
            QList<ExchangeSimpleAscii::ColumnType> columns;
            columns.append(ExchangeSimpleAscii::eColumnFeatureName);
            columns.append(ExchangeSimpleAscii::eColumnX);
            columns.append(ExchangeSimpleAscii::eColumnY);
            columns.append(ExchangeSimpleAscii::eColumnZ);
            if(params.readCommonColumn) {
                columns.append(ExchangeSimpleAscii::eColumnCommonState);
            }
            simpleAscii->setUserDefinedColumns(columns);

            //simpleAscii->setUserDefinedColumns(params.userDefinedColumns);

        }
        exchange = simpleAscii;
    }

    //check if the loaded exchange method is ok
    if(exchange.isNull()){
        emit this->sendMessage(QString("No exchange available with the name %1 in the plugin %2").arg(params.exchangeName).arg(params.pluginName), eErrorMessage, eMessageBoxMessage);
        return false;
    }
    this->exchange = exchange;

    //set exchange parameters
    QIODevice *device = new QFile(params.exchangeFilePath);
    this->exchange->setDevice(device);
    this->exchange->setFeatures(params.exportFeatures);
    this->exchange->setExportObservations(params.exportObservations);
    this->exchange->setGroupName(params.groupName);
    this->exchange->setNominalSystem(features.at(0)->getCoordinateSystem());
    QList<DimensionType> dimensions = params.units.keys();
    foreach(const DimensionType &dimension, dimensions){
        this->exchange->setUnit(dimension, params.units.value(dimension));
    }

    //connect exchange plugin
    QObject::connect(exchange.data(), SIGNAL(sendMessage(const QString&, const MessageTypes&, const MessageDestinations&)), this, SIGNAL(sendMessage(const QString&, const MessageTypes&, const MessageDestinations&)));
    QObject::connect(&this->exchangeThread, SIGNAL(started()), this->exchange.data(), SLOT(importOiData()), Qt::AutoConnection);
    QObject::connect(this->exchange.data(), SIGNAL(importFinished(const bool&)), this, SLOT(importFeatures(const bool&)), Qt::AutoConnection);
    //QObject::connect(this->exchange.data(), SIGNAL(exportFinished(const bool&)), this, SIGNAL(exportFinished(const bool&)), Qt::AutoConnection);
    QObject::connect(this->exchange.data(), SIGNAL(updateProgress(const int&, const QString&)), this, SIGNAL(updateNominalImportProgress(const int&, const QString&)), Qt::AutoConnection);

    //move exchange plugin to thread and start data exchange
    this->exchange->moveToThread(&this->exchangeThread);
    this->exchangeThread.start();

    return true;

}

/*!
 * \brief DataExchanger::exportData
 * \param params
 * \return
 */
bool DataExchanger::exportData(const ExchangeParams &params){

    //quit the thread if it is still running
    if(this->exchangeThread.isRunning()){
        this->exchangeThread.quit();
        this->exchangeThread.wait();
    }

    //check current job
    if(this->currentJob.isNull()){
        emit this->sendMessage("No job specified", eErrorMessage, eMessageBoxMessage);
        return false;
    }

    //check if given plugin name is empty
    if(params.pluginName.compare("") == 0 || params.exchangeName.compare("") == 0){
        emit this->sendMessage(QString("No exchange available with the name %1 in the plugin %2").arg(params.exchangeName).arg(params.pluginName), eErrorMessage, eMessageBoxMessage);
        return false;
    }

    //get the plugin from database and check if it is valid
    sdb::Plugin plugin = SystemDbManager::getPlugin(params.pluginName);
    if(plugin.name.compare("") == 0){
        emit this->sendMessage("No valid plugin specified", eErrorMessage, eMessageBoxMessage);
        return false;
    }

    //load exchange method with plugin loader
    QPointer<ExchangeInterface> exchange(NULL);
    if(params.isDefinedFormat){
        QPointer<ExchangeDefinedFormat> definedFormat = PluginLoader::loadExchangeDefinedFormatPlugin(qApp->applicationDirPath() + "/" + plugin.file_path, params.exchangeName);
        if(!definedFormat.isNull()){
            definedFormat->setUsedElements(params.usedElements);
        }
        exchange = definedFormat;
    }else{
        QPointer<ExchangeSimpleAscii> simpleAscii = PluginLoader::loadExchangeSimpleAsciiPlugin(qApp->applicationDirPath() + "/" + plugin.file_path, params.exchangeName);
        if(!simpleAscii.isNull()){

            simpleAscii->setGeometryType(params.typeOfGeometry);
            simpleAscii->setSkipFirstLine(params.skipFirstLine);
            simpleAscii->setDelimiter(params.delimiter);
            QMapIterator<DimensionType, UnitType>i(params.units);
            while(i.hasNext()){
                i.next();
                simpleAscii->setUnit(i.key(), i.value());
            }
            simpleAscii->setDistanceDigits(params.distanceDigits);
            simpleAscii->setAngleDigits(params.angleDigits);
            simpleAscii->setTemperatureDigits(params.angleDigits);

            //set user defined columns here hard coded (later selectable in table view)
            //TODO user defined columns
            QList<ExchangeSimpleAscii::ColumnType> columns;
            columns.append(ExchangeSimpleAscii::eColumnFeatureName);
            columns.append(ExchangeSimpleAscii::eColumnX);
            columns.append(ExchangeSimpleAscii::eColumnY);
            columns.append(ExchangeSimpleAscii::eColumnZ);
            columns.append(ExchangeSimpleAscii::eColumnPrimaryI);
            columns.append(ExchangeSimpleAscii::eColumnPrimaryJ);
            columns.append(ExchangeSimpleAscii::eColumnPrimaryK);
            columns.append(ExchangeSimpleAscii::eColumnRadiusA);

            simpleAscii->setUserDefinedColumns(columns);
            simpleAscii->setCurrentJob(this->currentJob);

            //simpleAscii->setUserDefinedColumns(params.userDefinedColumns);

        }
        exchange = simpleAscii;
    }

    //check if the loaded exchange method is ok
    if(exchange.isNull()){
        emit this->sendMessage(QString("No exchange available with the name %1 in the plugin %2").arg(params.exchangeName).arg(params.pluginName), eErrorMessage, eMessageBoxMessage);
        return false;
    }
    this->exchange = exchange;

    //set exchange parameters
    QIODevice *device = new QFile(params.exchangeFilePath);
    this->exchange->setDevice(device);
    this->exchange->setFeatures(params.exportFeatures);

    //connect exchange plugin
    QObject::connect(exchange.data(), SIGNAL(sendMessage(const QString&, const MessageTypes&, const MessageDestinations&)), this, SIGNAL(sendMessage(const QString&, const MessageTypes&, const MessageDestinations&)));
    QObject::connect(&this->exchangeThread, SIGNAL(started()), this->exchange.data(), SLOT(exportOiData()), Qt::AutoConnection);
    QObject::connect(this->exchange.data(), SIGNAL(exportFinished(const bool&)), this, SLOT(exportFeatures(const bool&)), Qt::AutoConnection);

    //move exchange plugin to thread and start data exchange
    this->exchange->moveToThread(&this->exchangeThread);
    this->exchangeThread.start();

    return true;

}

/*!
 * \brief DataExchanger::importObservations
 * \param filename
 * \return
 */
bool DataExchanger::importObservations(const QString &filename){

    //quit the thread if it is still running
    if(this->exchangeThread.isRunning()){
        this->exchangeThread.quit();
        this->exchangeThread.wait();
    }

    //check current job and pass it to observation importer
    if(this->currentJob.isNull()){
        emit this->sendMessage("No job specified", eErrorMessage, eMessageBoxMessage);
        return false;
    }

    //create observation importer and pass the current job
    this->observationImporter = new ObservationImporter();
    this->observationImporter->setCurrentJob(this->currentJob);
    this->observationImporter->setFileName(filename);

    //connect observation importer
    QObject::connect(&this->exchangeThread, &QThread::started, this->observationImporter.data(), &ObservationImporter::importObservations, Qt::AutoConnection);
    QObject::connect(this->observationImporter.data(), &ObservationImporter::importFinished, this, &DataExchanger::importObservationsFinished, Qt::AutoConnection);
    QObject::connect(this->observationImporter.data(), &ObservationImporter::updateProgress, this, &DataExchanger::updateObservationImportProgress, Qt::AutoConnection);
    QObject::connect(this->observationImporter.data(), &ObservationImporter::sendMessage, this, &DataExchanger::sendMessage, Qt::AutoConnection);

    //move exchange plugin to thread and start data exchange
    this->observationImporter->moveToThread(&this->exchangeThread);
    this->exchangeThread.start();

    return true;

}

void DataExchanger::importMeasurements(QList<QPointer<FeatureWrapper> > features) {
    QDateTime curDateTime = QDateTime::currentDateTime();
    foreach (QPointer<FeatureWrapper> importedFeature, features) {
        QList<QPointer<FeatureWrapper> > jobFeatures = this->currentJob->getFeaturesByName(importedFeature->getFeature()->getFeatureName());
        if (jobFeatures.isEmpty()) {
            qDebug() << "features not found: " << importedFeature->getFeature()->getFeatureName();
            continue;
        }

        foreach(QPointer<FeatureWrapper> jobFeature, jobFeatures) {
            if(jobFeature->getPoint()->getIsNominal()) {
                qDebug() << "isNominal: " << jobFeature->getPoint()->getFeatureName();
                continue;
            }
            OiVec p = importedFeature->getPoint()->getPosition().getVector();

            QList<QPointer<Reading> > importedReadings;

            ReadingCartesian rCartesian;
            rCartesian.xyz.setAt(0, p.getAt(0));
            rCartesian.xyz.setAt(1, p.getAt(1));
            rCartesian.xyz.setAt(2, p.getAt(2));
            rCartesian.isValid = true;
            QPointer<Reading> reading = new Reading(rCartesian);
            reading->setSensorFace(eUndefinedSide);
            reading->setMeasuredAt(curDateTime);
            reading->setImported(true);

            importedReadings.append(reading);

            this->currentJob->addMeasurementResults(jobFeature->getGeometry()->getId(),importedReadings);
            emit this->sendMessage(QString("import reading to feature: \"%1\"").arg(jobFeature->getPoint()->getFeatureName()), eInformationMessage, eConsoleMessage);
        }
    }
}

void DataExchanger::createActuals(QList<QPointer<FeatureWrapper> > features) {

    foreach (QPointer<FeatureWrapper> fw, features) {
        if(fw->getFeatureTypeEnum() == ePointFeature){
            FeatureAttributes fAttr;
            fAttr.count = 1;
            fAttr.typeOfFeature = fw->getFeatureTypeEnum();
            fAttr.name = fw->getFeature()->getFeatureName();
            fAttr.group = fw->getFeature()->getGroupName();

            fAttr.isActual = true;
            fAttr.isNominal = false;
            fAttr.isCommon = !QString::compare(fw->getFeature()->property("OI_FEATURE_COMMONSTATE").toString(), "true", Qt::CaseInsensitive);

            //mconfig and function from default
            MeasurementConfig mConfig;
            if(!this->mConfigManager.isNull()){
                //TODO fix that all measurement configs are saved in the database OI-373
                //mConfig = this->mConfigManager->getActiveMeasurementConfig(getGeometryTypeEnum(fw->getFeatureTypeEnum()));
                //mConfig = this->mConfigManager->getSavedMeasurementConfig(SystemDbManager::getDefaultMeasurementConfig(getElementTypeName(getElementTypeEnum(fw->getFeatureTypeString()))));

                QString elementConfigName = SystemDbManager::getDefaultMeasurementConfig(getElementTypeName(getElementTypeEnum(fw->getFeatureTypeString())));

                mConfig = mConfigManager->getSavedMeasurementConfig(elementConfigName);

                /*//Workaround until bug is fixed
                QList<MeasurementConfig> mConfigs = this->mConfigManager->getSavedMeasurementConfigs();
                if(mConfigs.size() > 0){
                    bool fpExists = false;
                    foreach (MeasurementConfig mC, mConfigs) {
                        if(mC.getName().compare("FastPoint") == 0){
                            mConfig = this->mConfigManager->getSavedMeasurementConfig("FastPoint");
                            fpExists = true;
                        }
                    }
                    if(!fpExists){
                        mConfig = this->mConfigManager->getSavedMeasurementConfig(mConfigs.at(0).getName());
                    }
                }
                fAttr.mConfig = mConfig.getName();*/
            }

            //function
            sdb::Function defaultFunction = SystemDbManager::getDefaultFunction(fAttr.typeOfFeature);
            QPair<QString, QString> functionPlugin;
            functionPlugin.first = defaultFunction.name;
            functionPlugin.second = defaultFunction.plugin.file_path;
            fAttr.functionPlugin = functionPlugin;

            QList<QPointer<FeatureWrapper> > addedFeatures = this->currentJob->addFeatures(fAttr);

            this->addFunctionsAndMConfigs(addedFeatures,mConfig, defaultFunction.plugin.file_path, defaultFunction.name);
        }
    }
}

/*!
 * \brief DataExchanger::importFeatures
 * \param success
 */
void DataExchanger::importFeatures(const bool &success){

    if(!success){
        emit this->nominalImportFinished(false);
        return;
    }

    //add the imported features to current OpenIndy job
    QList<QPointer<FeatureWrapper> > features = this->exchange->getFeatures();

    bool importSuccess = false;
    if(this->exchangeParams.importMeasurements) {
        // import only reading to existing actuals
        importMeasurements(features);
        importSuccess = true;
    } else {
        // add features
        importSuccess = this->currentJob->addFeatures(features, this->exchangeParams.overwrite);

        // add actuals to nominals at import
        if(this->exchangeParams.createActual){
            createActuals(features);
        }

        //get the specified coordinate system by its defined name from exchange parameters and set this one true
        //so destination system from import nominals is active system and all nominal values will be shown directly
        QList<QPointer<CoordinateSystem> >coordSystemList = this->currentJob->getCoordinateSystemsList();
        foreach (QPointer<CoordinateSystem> cs, coordSystemList) {
            if(cs->getFeatureName().compare(this->exchangeParams.nominalSystem) == 0){
                this->currentJob->getActiveCoordinateSystem()->setActiveCoordinateSystemState(false);
                cs->setActiveCoordinateSystemState(true);
            }
        }
    }

    //delete exchange instance because it is not necessary anymore
    delete this->exchange;

    //stop thread if it is still running
    if(this->exchangeThread.isRunning()){
        this->exchangeThread.quit();
        this->exchangeThread.wait();
    }

    emit this->nominalImportFinished(importSuccess);

}

/*!
 * \brief DataExchanger::exportFeatures
 * \param success
 */
void DataExchanger::exportFeatures(const bool &success)
{
    //delete exchange instance because it is not necessary anymore
    delete this->exchange;

    //stop thread if it is still running
    if(this->exchangeThread.isRunning()){
        this->exchangeThread.quit();
        this->exchangeThread.wait();
    }
}

/*!
 * \brief DataExchanger::importObservationsFinished
 * \param success
 */
void DataExchanger::importObservationsFinished(const bool &success){

    if(!success){
        emit this->observationImportFinished(false);
        delete this->observationImporter;
        return;
    }

    //delete observation importer instance because it is not necessary anymore
    delete this->observationImporter;

    //stop thread if it is still running
    if(this->exchangeThread.isRunning()){
        this->exchangeThread.quit();
        this->exchangeThread.wait();
    }

    emit this->observationImportFinished(success);

}

/*!
 * \brief DataExchanger::addFunctionsAndMConfigs
 * \param actuals
 * \param mConfig
 * \param path
 * \param fName
 */
void DataExchanger::addFunctionsAndMConfigs(const QList<QPointer<FeatureWrapper> > &actuals, const MeasurementConfig &mConfig, const QString &path, const QString &fName)
{
    //check job
    if(this->currentJob.isNull()){
        return;
    }

    foreach(const QPointer<FeatureWrapper> &feature, actuals){

        //check feature
        if(feature.isNull() || feature->getFeature().isNull()){
            continue;
        }

        //check if the feature is a nominal geometry
        if(!feature->getGeometry().isNull() && feature->getGeometry()->getIsNominal()){
            continue;
        }

        //load function plugin
        QPointer<Function> function(NULL);
        if(path != 0 && fName != 0){
            function = PluginLoader::loadFunctionPlugin(path, fName);
        }

        //assign function and measurement config to feature
        this->currentJob->blockSignals(true);
        if(!function.isNull()){
            feature->getFeature()->addFunction(function);
        }
        if(mConfig.getIsValid() && !feature->getGeometry().isNull()){
            feature->getGeometry()->setMeasurementConfig(mConfig);
        }
        this->currentJob->blockSignals(false);

    }
}
