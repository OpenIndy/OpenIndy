#ifndef OIDATAEXCHANGER_H
#define OIDATAEXCHANGER_H

#include <QString>
#include <QStringList>
#include <QObject>

#include "oiexchangeascii.h"
#include "oiexchangepts.h"
#include "unitconverter.h"

/*!
 * \brief The oiDataExchanger class
 * static class to manage all the different kinds of data import defined by
 * the oiExchangeInterface. You have to add your oiexchange class here.
 */
class oiDataExchanger : public QObject
{
    Q_OBJECT

private:
    explicit oiDataExchanger(QObject *parent = 0);
    ~oiDataExchanger();

public:
    static oiDataExchanger &getInstance();

    static QStringList getOutputFormats();
    static QList<Configuration::ElementTypes> getSupportedElements(QString format);
    static QStringList getElementDescription(QString format, Configuration::ElementTypes);
    static QList<UnitConverter::unitType> getDistanceUnits(QString format);
    static QList<UnitConverter::unitType> getAngleUnits(QString format);
    static QList<UnitConverter::unitType> getTemperatureUnits(QString format);

public slots:
    void importData(oiExchangeObject exchangeData, QString format);
    void exportData(oiExchangeObject exchangeData, QString format);

signals:
    void updateProgress(int progress, QString msg); //progress in percent (0-100)
    void exchangeFinished(bool success, oiExchangeObject exchangeData);

private slots:
    void exchangeData();
    void emitUpdateProgress(int progress, QString msg);

private:
    static oiDataExchanger myInstance;
    static QString format;
    static bool import;

    QThread myExchangeThread;

    static oiExchangeObject myExchangeData;

};

#endif // OIDATAEXCHANGER_H
