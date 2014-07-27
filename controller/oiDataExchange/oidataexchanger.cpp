#include "oidataexchanger.h"

oiDataExchanger oiDataExchanger::myInstance;
QString oiDataExchanger::format;
bool oiDataExchanger::import;
oiExchangeObject oiDataExchanger::myExchangeData;

oiDataExchanger::oiDataExchanger(QObject *parent) : QObject(parent){

}

oiDataExchanger::~oiDataExchanger(){

    if(this->myExchangeThread.isRunning()){
        this->myExchangeThread.quit();
        this->myExchangeThread.wait();
    }

}

/*!
 * \brief oiDataExchanger::getInstance
 * Get an instance of oiDataExchanger class
 * \return
 */
oiDataExchanger &oiDataExchanger::getInstance(){
    return oiDataExchanger::myInstance;
}

/*!
 * \brief oiDataExchanger::importData
 * \param exchangeData
 * \param format
 */
void oiDataExchanger::importData(oiExchangeObject exchangeData, QString format){

    exchangeData.features.clear();
    oiDataExchanger::myExchangeData = exchangeData;
    oiDataExchanger::format = format;
    oiDataExchanger::import = true;

    if(this->myExchangeThread.isRunning()){
        this->myExchangeThread.quit();
        this->myExchangeThread.wait();
    }

    moveToThread(&this->myExchangeThread);
    connect(&this->myExchangeThread, SIGNAL(started()), this, SLOT(exchangeData()));
    this->myExchangeThread.start();

}

/*!
 * \brief oiDataExchanger::exportData
 * \param exchangeData
 * \param format
 */
void oiDataExchanger::exportData(oiExchangeObject exchangeData, QString format){

    oiDataExchanger::myExchangeData = exchangeData;
    oiDataExchanger::format = format;
    oiDataExchanger::import = false;

    if(this->myExchangeThread.isRunning()){
        this->myExchangeThread.quit();
        this->myExchangeThread.wait();
    }

    moveToThread(&this->myExchangeThread);
    connect(&this->myExchangeThread, SIGNAL(started()), this, SLOT(exchangeData()));
    this->myExchangeThread.start();

}

/*!
 * \brief oiDataExchanger::getOutputFormats
 * add a string tag for your oiexchange class here
 * \return
 */
QStringList oiDataExchanger::getOutputFormats(){

    QStringList  formats;

    //add a tag for your oiexchange class here
    formats.append("simple ascii oiFeature (oif)");
    formats.append("pts");

    return formats;
}

/*!
 * \brief oiDataExchanger::getSupportedElements
 * add the supported element types of your oiexchange class here
 * \param format
 * \return
 */
QList<Configuration::ElementTypes> oiDataExchanger::getSupportedElements(QString format){

    //first check for your tag then call your supported elements function here
    if(format == "simple ascii oiFeature (oif)"){
        oiExchangeASCII asciiExchange;
        return asciiExchange.getSupportedElements();
    }else if(format == "pts"){
        oiExchangePTS ptsExchange;
        return ptsExchange.getSupportedElements();
    }

    QList<Configuration::ElementTypes> proxyList;
    return proxyList;

}

QStringList oiDataExchanger::getElementDescription(QString format, Configuration::ElementTypes t)
{
    QStringList description;

    if(format == "simple ascii oiFeature (oif)"){
        oiExchangeASCII asciiExchange;
        return asciiExchange.getElementDescription(t);
    }else if(format == "pts"){
        oiExchangePTS ptsExchange;
        return ptsExchange.getElementDescription(t);
    }

    return description;

}

QList<UnitConverter::unitType> oiDataExchanger::getDistanceUnits(QString format)
{
    if(format == "simple ascii oiFeature (oif)"){
        oiExchangeASCII asciiExchange;
        return asciiExchange.getSupportedDistanceUnits();
    }else if(format == "pts"){
        oiExchangePTS ptsExchange;
        return ptsExchange.getSupportedDistanceUnits();
    }

    QList<UnitConverter::unitType> proxyList;
    return proxyList;
}

QList<UnitConverter::unitType> oiDataExchanger::getAngleUnits(QString format)
{
    if(format == "simple ascii oiFeature (oif)"){
        oiExchangeASCII asciiExchange;
        return asciiExchange.getSupportedAngleUnits();
    }else if(format == "pts"){
        oiExchangePTS ptsExchange;
        return ptsExchange.getSupportedAngleUnits();
    }

    QList<UnitConverter::unitType> proxyList;
    return proxyList;
}

QList<UnitConverter::unitType> oiDataExchanger::getTemperatureUnits(QString format)
{
    if(format == "simple ascii oiFeature (oif)"){
        oiExchangeASCII asciiExchange;
        return asciiExchange.getSupportedTemperatureUnits();
    }else if(format == "pts"){
        oiExchangePTS ptsExchange;
        return ptsExchange.getSupportedTemperatureUnits();
    }

    QList<UnitConverter::unitType> proxyList;
    return proxyList;

}

/*!
  * \brief oiDataExchanger::exchangeData
  * \param exchangeData
  * \param format
  * \param import
  */
void oiDataExchanger::exchangeData(){

    disconnect(&this->myExchangeThread, SIGNAL(started()), this, SLOT(exchangeData()));

    bool success = false;

    //first check for your tag then call your import or export function here
    if(oiDataExchanger::format == "simple ascii oiFeature (oif)"){

        oiExchangeASCII asciiExchange;
        if (oiDataExchanger::import){
            success = asciiExchange.importOiData(oiDataExchanger::myExchangeData);
        }else{
            success = asciiExchange.exportOiData(oiDataExchanger::myExchangeData);
        }

    }else if(oiDataExchanger::format == "pts"){

        oiExchangePTS ptsExchange;
        connect(&ptsExchange, SIGNAL(updateProgress(int, QString)), this, SLOT(emitUpdateProgress(int, QString)));
        if (oiDataExchanger::import){
            success = ptsExchange.importOiData(oiDataExchanger::myExchangeData);
        }else{
            success = ptsExchange.exportOiData(oiDataExchanger::myExchangeData);
        }

    }

    emit this->exchangeFinished(success, oiDataExchanger::myExchangeData);

}

/*!
 * \brief oiDataExchanger::emitUpdateProgress
 * \param progress
 * \param msg
 */
void oiDataExchanger::emitUpdateProgress(int progress, QString msg){
    emit this->updateProgress(progress, msg);
}
