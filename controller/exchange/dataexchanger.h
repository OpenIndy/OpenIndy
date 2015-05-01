#ifndef OIDATAEXCHANGER_H
#define OIDATAEXCHANGER_H

#include <QString>
#include <QStringList>
#include <QObject>
#include <QApplication>

#include "systemdbmanager.h"
#include "exchangesimpleascii.h"
#include "exchangedefinedformat.h"
#include "oijob.h"
#include "exchangeParams.h"
#include "pluginloader.h"
#include "console.h"

/*!
 * \brief The DataExchanger class
 * Imports or exports nominals by invoking plugins
 */
class DataExchanger : public QObject
{
    Q_OBJECT

public:
    explicit DataExchanger(QObject *parent = 0);

    ~DataExchanger();

    //#############################
    //get or set general attributes
    //#############################

    const QPointer<OiJob> &getCurrentJob() const;
    void setCurrentJob(const QPointer<OiJob> &job);

    //######################
    //start import or export
    //######################

    bool importData(const ExchangeParams &params);
    bool exportData(const ExchangeParams &params);

signals:

    //##############################
    //inform about exchange progress
    //##############################

    void updateProgress(const int &progress, const QString &msg); // 0 <= progress <= 100
    void importFinished(const bool &success);
    void exportFinished(const bool &success);

private slots:

    //########################################################
    //add features to current job after an import was finished
    //########################################################

    void importFeatures(const bool &success);

private:

    //##################
    //general attributes
    //##################

    QPointer<OiJob> currentJob;

    ExchangeParams exchangeParams;
    QPointer<ExchangeInterface> exchange;

    //#########################
    //thread the plugin runs on
    //#########################

    QThread exchangeThread;

};

#endif // OIDATAEXCHANGER_H
