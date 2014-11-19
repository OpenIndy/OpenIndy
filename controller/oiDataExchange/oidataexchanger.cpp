#include "oidataexchanger.h"




/*
OiDataExchanger OiDataExchanger::myInstance;
QString OiDataExchanger::format;
bool OiDataExchanger::import;
OiExchangeObject OiDataExchanger::myExchangeData;*/

OiDataExchanger::OiDataExchanger(QObject *parent) : QObject(parent){

}

OiDataExchanger::~OiDataExchanger(){

    /*if(this->myExchangeThread.isRunning()){
        this->myExchangeThread.quit();
        this->myExchangeThread.wait();
    }*/

}

/*!
 * \brief oiDataExchanger::getInstance
 * Get an instance of oiDataExchanger class
 * \return
 */
/*OiDataExchanger &OiDataExchanger::getInstance(){
    return OiDataExchanger::myInstance;
}*/

/*!
 * \brief oiDataExchanger::importData
 * \param exchangeData
 * \param format
 */
/*void OiDataExchanger::importData(OiExchangeObject exchangeData, QString format){

    exchangeData.features.clear();
    OiDataExchanger::myExchangeData = exchangeData;
    OiDataExchanger::format = format;
    OiDataExchanger::import = true;

    if(this->myExchangeThread.isRunning()){
        this->myExchangeThread.quit();
        this->myExchangeThread.wait();
    }

    moveToThread(&this->myExchangeThread);
    connect(&this->myExchangeThread, SIGNAL(started()), this, SLOT(exchangeData()));
    this->myExchangeThread.start();

}*/

/*!
 * \brief oiDataExchanger::exportData
 * \param exchangeData
 * \param format
 */
/*void OiDataExchanger::exportData(OiExchangeObject exchangeData, QString format){

    OiDataExchanger::myExchangeData = exchangeData;
    OiDataExchanger::format = format;
    OiDataExchanger::import = false;

    if(this->myExchangeThread.isRunning()){
        this->myExchangeThread.quit();
        this->myExchangeThread.wait();
    }

    moveToThread(&this->myExchangeThread);
    connect(&this->myExchangeThread, SIGNAL(started()), this, SLOT(exchangeData()));
    this->myExchangeThread.start();

}*/

/*!
 * \brief oiDataExchanger::getOutputFormats
 * add a string tag for your oiexchange class here
 * \return
 */
/*QStringList OiDataExchanger::getOutputFormats(){

    QStringList  formats;

    //add a tag for your oiexchange class here
    formats.append("simple ascii oiFeature (oif)");
    formats.append("pts");

    return formats;
}*/

/*!
 * \brief oiDataExchanger::getSupportedElements
 * add the supported element types of your oiexchange class here
 * \param format
 * \return
 */
/*QList<Configuration::ElementTypes> OiDataExchanger::getSupportedElements(QString format){

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

QStringList OiDataExchanger::getElementDescription(QString format, Configuration::ElementTypes t)
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

QList<UnitConverter::unitType> OiDataExchanger::getDistanceUnits(QString format)
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

QList<UnitConverter::unitType> OiDataExchanger::getAngleUnits(QString format)
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

QList<UnitConverter::unitType> OiDataExchanger::getTemperatureUnits(QString format)
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

}*/

/*!
  * \brief oiDataExchanger::exchangeData
  * \param exchangeData
  * \param format
  * \param import
  */
/*void OiDataExchanger::exchangeData(){

    disconnect(&this->myExchangeThread, SIGNAL(started()), this, SLOT(exchangeData()));

    bool success = false;

    //first check for your tag then call your import or export function here
    if(OiDataExchanger::format == "simple ascii oiFeature (oif)"){

        oiExchangeASCII asciiExchange;
        if (OiDataExchanger::import){
            success = asciiExchange.importOiData(OiDataExchanger::myExchangeData);
        }else{
            success = asciiExchange.exportOiData(OiDataExchanger::myExchangeData);
        }

    }else if(OiDataExchanger::format == "pts"){

        oiExchangePTS ptsExchange;
        connect(&ptsExchange, SIGNAL(updateProgress(int, QString)), this, SLOT(emitUpdateProgress(int, QString)));
        if (OiDataExchanger::import){
            success = ptsExchange.importOiData(OiDataExchanger::myExchangeData);
        }else{
            success = ptsExchange.exportOiData(OiDataExchanger::myExchangeData);
        }

    }

    emit this->exchangeFinished(success, OiDataExchanger::myExchangeData);

}*/

/*!
 * \brief oiDataExchanger::emitUpdateProgress
 * \param progress
 * \param msg
 */
/*void OiDataExchanger::emitUpdateProgress(int progress, QString msg){
    emit this->updateProgress(progress, msg);
}*/
