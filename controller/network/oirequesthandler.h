#ifndef OIREQUESTHANDLER_H
#define OIREQUESTHANDLER_H

#include <QObject>
#include <QtXml>
#include <QThread>
#include <QPointer>

#include "oirequestresponse.h"
#include "featurewrapper.h"
#include "station.h"

#include "projectexchanger.h"
#include "featureupdater.h"
#include "featuresorter.h"
#include "measurementconfigmanager.h"
#include "sensorconfigurationmanager.h"

/*!
 * \brief The OiRequestHandler class
 * Receives XML based requests and emits the corresponding response as XML
 */
class OiRequestHandler : public QObject
{
    Q_OBJECT

public:
    explicit OiRequestHandler(QObject *parent = 0);

    //######################
    //get or set current job
    //######################

    const QPointer<OiJob> &getCurrentJob() const;
    void setCurrentJob(const QPointer<OiJob> &job);

    //##########################
    //get or set config managers
    //##########################

    const QPointer<SensorConfigurationManager> &getSensorConfigManager() const;
    void setSensorConfigManager(const QPointer<SensorConfigurationManager> &sensorConfigManager);

    const QPointer<MeasurementConfigManager> &getMeasurementConfigManager() const;
    void setMeasurementConfigManager(const QPointer<MeasurementConfigManager> &measurementConfigManager);

public slots:

    //#######################################
    //receive requests from web socket server
    //#######################################

    bool receiveRequest(OiRequestResponse request);

    //##################################
    //sensor actions started or finished
    //##################################

    void sensorActionStarted(const QString &name);
    void sensorActionFinished(const bool &success, const QString &msg);

    //#########
    //messaging
    //#########

    void log(const QString &msg, const MessageTypes &msgType);

    //#######################
    //sensor real time stream
    //#######################

    void realTimeReading(const QVariantMap &reading);

    //#################
    //job state changes
    //#################

    //active feature changes
    void activeFeatureChanged();
    void activeStationChanged();
    void activeCoordinateSystemChanged();

    //feature(s) added or removed
    void featureSetChanged();

    //general feature attributes changed
    void featureAttributesChanged();

signals:

    //##################################
    //send response to web socket server
    //##################################

    bool sendResponse(OiRequestResponse request);

    //######################
    //trigger sensor actions
    //######################

    void startAim();
    void startMeasurement();

private:

    //##############
    //helper methods
    //##############

    //request processing
    void getProject(OiRequestResponse &request);
    void getActiveFeature(OiRequestResponse &request);
    void setActiveFeature(OiRequestResponse &request);
    void getActiveStation(OiRequestResponse &request);
    void setActiveStation(OiRequestResponse &request);
    void getActiveCoordinateSystem(OiRequestResponse &request);
    void setActiveCoordinateSystem(OiRequestResponse &request);
    void aim(OiRequestResponse &request);
    void measure(OiRequestResponse &request);
    void startWatchwindow(OiRequestResponse &request);
    void stopWatchwindow(OiRequestResponse &request);
    void toolRequest(OiRequestResponse &request);
    void getFeatures(OiRequestResponse &request);
    void addFeatures(OiRequestResponse &request);
    void getObservations(OiRequestResponse &request);
    void removeObservations(OiRequestResponse &request);
    void getParameters(OiRequestResponse &request);
    void getMeasurementConfigs(OiRequestResponse &request);
    void getMeasurementConfig(OiRequestResponse &request);
    void setMeasurementConfig(OiRequestResponse &request);

    //parse request type id
    OiRequestResponse::RequestType getRequestType(int id) const;

    //parse feature type
    FeatureTypes getFeatureType(int type) const;

    //add feature parameters
    void addParameters(QDomDocument &document, QDomElement &parameters, const QPointer<FeatureWrapper> &feature);

    //send an error messages
    void sendErrorMessage(OiRequestResponse request, OiRequestResponse::RequestType type, OiRequestResponse::ErrorCode error);

    //prepare response to client
    void prepareResponse(OiRequestResponse &request) const;

    //bool buildWatchWindowMessage(QDomElement &wwTag, const int &readingType, const QVariantMap &streamData);

private slots:

    //##################################
    //watch window and measure callbacks
    //##################################

    //void receiveWatchWindowData(const QVariantMap &data);
    //void measurementFinished(const bool &success);

private:

    //##############
    //config manager
    //##############

    QPointer<SensorConfigurationManager> sensorConfigManager;
    QPointer<MeasurementConfigManager> measurementConfigManager;

    //#################
    //helper attributes
    //#################

    QPointer<OiJob> currentJob;

    FeatureSorter sorter;

};

#endif // OIREQUESTHANDLER_H
