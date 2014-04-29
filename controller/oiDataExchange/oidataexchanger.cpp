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
    formats.append("ascii");

    return formats;
}

QList<Configuration::ElementTypes> oiDataExchanger::getSupportedElements(QString format){

    //first check for your tag then call your supported elements function here
    if(format == "ascii"){
        oiExchangeASCII asciiExchange;
        return asciiExchange.getSupportedElements();
    }

    QList<Configuration::ElementTypes> proxyList;
    return proxyList;

}

 bool oiDataExchanger::exchangeData(oiExchangeObject &exchangeData,QString format, bool import){

     //first check for your tag then call your import or export function here
     if(format == "ascii"){
         oiExchangeASCII asciiExchange;
         if (import){
             asciiExchange.importOiData(exchangeData);
         }else{
             asciiExchange.exportOiData(exchangeData);
         }
     }

     return false;
 }
