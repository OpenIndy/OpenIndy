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
    bool import = this->currentJob->addFeatures(features);

    //delete exchange instance because it is not necessary anymore
    delete this->exchange;

    //stop thread if it is still running
    if(this->exchangeThread.isRunning()){
        this->exchangeThread.quit();
        this->exchangeThread.wait();
    }

    emit this->nominalImportFinished(import);

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
