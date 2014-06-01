#include "oidataexchanger.h"

 oiDataExchanger::oiDataExchanger(){


 }


 bool oiDataExchanger::importData(oiExchangeObject &exchangeData,QString format){

   exchangeData.features.clear();

   oiDataExchanger::exchangeData(exchangeData,format,true);

   return false;
 }

bool oiDataExchanger::exportData(oiExchangeObject &exchangeData,QString format){

    oiDataExchanger::exchangeData(exchangeData,format,false);

    return false;
 }

QStringList oiDataExchanger::getOutputFormats(){

    QStringList  formats;

    //add a tag for your oiexchange class here
    formats.append("simple ascii oiFeature (oif)");
    formats.append("pts");

    return formats;
}

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

 bool oiDataExchanger::exchangeData(oiExchangeObject &exchangeData,QString format, bool import){

     //first check for your tag then call your import or export function here
     if(format == "simple ascii oiFeature (oif)"){
         oiExchangeASCII asciiExchange;
         if (import){
             asciiExchange.importOiData(exchangeData);
         }else{
             asciiExchange.exportOiData(exchangeData);
         }
     }else if(format == "pts"){
         oiExchangePTS ptsExchange;
         if (import){
             ptsExchange.importOiData(exchangeData);
         }else{
             ptsExchange.exportOiData(exchangeData);
         }
     }

     return false;
 }
