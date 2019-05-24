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
#include "observationimporter.h"
#include "measurementconfigmanager.h"
#include "systemdbmanager.h"
#include "util.h"

using namespace oi;

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

    const QPointer<MeasurementConfigManager> &getMeasurementConfigManager();
    void setMesaurementConfigManager(const QPointer<MeasurementConfigManager> &manager);

    void setExchangeParams(ExchangeParams exchangeParams);

    //######################
    //start import or export
    //######################

    //import or export features
    bool importData(const ExchangeParams &params);
    bool exportData(const ExchangeParams &params);

    //import observations
    bool importObservations(const QString &filename);

signals:

    //##############################
    //inform about exchange progress
    //##############################

    //nominal im- and export
    void updateNominalImportProgress(const int &progress, const QString &msg); // 0 <= progress <= 100
    void updateNominalExportProgress(const int &progress, const QString &msg); // 0 <= progress <= 100
    void nominalImportFinished(const bool &success);
    void nominalExportFinished(const bool &success);

    //observation import
    void updateObservationImportProgress(const int &progress, const QString &msg); // 0 <= progress <= 100
    void observationImportFinished(const bool &success);

    //print messages
    void sendMessage(const QString &msg, const MessageTypes &msgType, const MessageDestinations &msgDest = eConsoleMessage);

private slots:

    //########################################################
    //add features to current job after an import was finished
    //########################################################

    void importFeatures(const bool &success);
    void exportFeatures(const bool &success);

    void importObservationsFinished(const bool &success);

private:

    //##################
    //general attributes
    //##################

    QPointer<OiJob> currentJob;

    ExchangeParams exchangeParams;
    QPointer<ExchangeInterface> exchange;

    QPointer<ObservationImporter> observationImporter;

    //#########################
    //thread the plugin runs on
    //#########################

    QThread exchangeThread;

    QPointer<MeasurementConfigManager> mConfigManager;

    //#############
    //helper method
    //#############
    void addFunctionsAndMConfigs(const QList<QPointer<FeatureWrapper> > &actuals,
                                 const MeasurementConfig &mConfig, const QString &path, const QString &fName);

    // import readings of features
    void importMeasurements(QList<QPointer<FeatureWrapper>> features);

};

#endif // OIDATAEXCHANGER_H
