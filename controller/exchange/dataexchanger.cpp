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

    //load and check plugin
    QPointer<ExchangeInterface> exchange(NULL);
    if(params.isDefinedFormat && params.pluginName.compare("") != 0 && params.exchangeName.compare("") != 0){
        QPointer<ExchangeDefinedFormat> definedFormat = PluginLoader::loadExchangeDefinedFormatPlugin(qApp->applicationDirPath() + "/plugins/" + params.pluginName + ".dll", params.exchangeName);
        if(!definedFormat.isNull()){
            definedFormat->setUsedElements(params.usedElements);
        }
    }else if(params.pluginName.compare("") != 0 && params.exchangeName.compare("") != 0){
        QPointer<ExchangeSimpleAscii> simpleAscii = PluginLoader::loadExchangeSimpleAsciiPlugin(qApp->applicationDirPath() + "/plugins/" + params.pluginName + ".dll", params.exchangeName);
        simpleAscii->setUserDefinedColumns(params.userDefinedColumns);
        simpleAscii->setGeometryType(params.typeOfGeometry);
        simpleAscii->setSkipFirstLine(params.skipFirstLine);
        simpleAscii->setDelimiter(params.delimiter);
    }
    if(exchange.isNull()){
        Console::getInstance()->addLine(QString("No exchange available with the name %1 in the plugin %2").arg(params.exchangeName).arg(params.pluginName));
        return false;
    }
    this->exchange = exchange;

    //set exchange parameters
    this->exchange->setDevice(this->currentJob->getJobDevice());
    this->exchange->setFeatures(params.exportFeatures);
    this->exchange->setExportObservations(params.exportObservations);
    this->exchange->setGroupName(params.groupName);
    this->exchange->setNominalSystem(params.nominalSystem);
    QList<DimensionType> dimensions = params.units.keys();
    foreach(const DimensionType &dimension, dimensions){
        this->exchange->setUnit(dimension, params.units.value(dimension));
    }

    //connect exchange plugin
    QObject::connect(this->exchange.data(), SIGNAL(importFinished(const bool&)), this, SLOT(importFeatures(const bool&)), Qt::AutoConnection);
    QObject::connect(this->exchange.data(), SIGNAL(exportFinished(const bool&)), this, SIGNAL(exportFinished(const bool&)), Qt::AutoConnection);
    QObject::connect(this->exchange.data(), SIGNAL(updateProgress(const int&, const QString&)), this, SIGNAL(updateProgress(const int&, const QString&)), Qt::AutoConnection);

    //move exchange plugin to thread and start data exchange
    this->exchange->moveToThread(&this->exchangeThread);
    this->exchangeThread.start();
    this->exchange->importOiData();

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
    bool import = this->currentJob->addFeatures(this->exchange->getFeatures());

    emit this->importFinished(import);

}
