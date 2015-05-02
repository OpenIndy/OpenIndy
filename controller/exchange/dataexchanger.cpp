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
        Console::getInstance()->addLine("No job specified");
        return false;
    }

    //check nominal system
    QList<QPointer<FeatureWrapper> > features = this->currentJob->getFeaturesByName(params.nominalSystem);
    if(features.size() != 1 || features.at(0).isNull() || features.at(0)->getCoordinateSystem().isNull()
            || features.at(0)->getCoordinateSystem()->getIsStationSystem()){
        Console::getInstance()->addLine("No valid nominal system");
        return false;
    }

    //check if given plugin name is empty
    if(params.pluginName.compare("") == 0 || params.exchangeName.compare("") == 0){
        Console::getInstance()->addLine(QString("No exchange available with the name %1 in the plugin %2").arg(params.exchangeName).arg(params.pluginName));
        return false;
    }

    //get the plugin from database and check if it is valid
    sdb::Plugin plugin = SystemDbManager::getPlugin(params.pluginName);
    if(plugin.name.compare("") == 0){
        Console::getInstance()->addLine("No valid plugin specified");
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
        Console::getInstance()->addLine(QString("No exchange available with the name %1 in the plugin %2").arg(params.exchangeName).arg(params.pluginName));
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
    QObject::connect(this->exchange.data(), SIGNAL(exportFinished(const bool&)), this, SIGNAL(exportFinished(const bool&)), Qt::AutoConnection);
    QObject::connect(this->exchange.data(), SIGNAL(updateProgress(const int&, const QString&)), this, SIGNAL(updateProgress(const int&, const QString&)), Qt::AutoConnection);

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

    //TODO implement export

    return false;

}

/*!
 * \brief DataExchanger::importFeatures
 * \param success
 */
void DataExchanger::importFeatures(const bool &success){

    if(!success){
        emit this->importFinished(false);
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

    emit this->importFinished(import);

}
