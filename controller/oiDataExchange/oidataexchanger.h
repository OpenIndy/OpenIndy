#ifndef OIDATAEXCHANGER_H
#define OIDATAEXCHANGER_H

#include <QString>
#include <QStringList>

#include "oiexchangeascii.h"

class oiDataExchanger
{

public:
    oiDataExchanger();


    static bool importData(oiExchangeObject &exchangeData, QString format);
    static bool exportData(oiExchangeObject &exchangeData, QString format);

    static QStringList getOutputFormats();
    static QList<Configuration::ElementTypes> getSupportedElements(QString format);


private:
    static bool exchangeData(oiExchangeObject &exchangeData,QString format,bool import);

};

#endif // OIDATAEXCHANGER_H
