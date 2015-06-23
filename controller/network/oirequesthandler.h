#ifndef OIREQUESTHANDLER_H
#define OIREQUESTHANDLER_H

#include <QObject>
#include <QtXml>
#include <QThread>
#include <QPointer>

#include "oirequestresponse.h"
#include "featurewrapper.h"
#include "projectexchanger.h"
#include "station.h"
#include "sensorlistener.h"
#include "featureupdater.h"

using namespace oi;

struct WatchWindowTask{
    bool taskInProcess;
    OiRequestResponse request;
};
struct MeasurementTask{
    bool taskInProcess;
    OiRequestResponse request;
};

/*!
 * \brief The OiRequestHandler class
 * Receives XML based requests and emits the corresponding response as XML
 */
class OiRequestHandler : public QObject
{
    Q_OBJECT
private:
    explicit OiRequestHandler(QObject *parent = 0);

public:
    static QPointer<OiRequestHandler> getInstance();

    //######################
    //get or set current job
    //######################

    const QPointer<OiJob> &getJob() const;
    void setJob(const QPointer<OiJob> &job);

public slots:

    //################
    //receive requests
    //################

    bool receiveRequest(OiRequestResponse request);

    //#####################
    //receive tool response
    //#####################

    void receiveOiToolResponse(OiRequestResponse response);

signals:

    //#############
    //send response
    //#############

    bool sendResponse(OiRequestResponse response);

    //#################
    //send tool request
    //#################

    void sendOiToolRequest(OiRequestResponse request);

    //######################
    //trigger sensor actions
    //######################

    void startAim();
    void startMeasurement();

private:

    //#################
    //helper attributes
    //#################

    static QPointer<OiRequestHandler> myRequestHandler;

    QThread workerThread;

    //save tasks which are currently in process
    WatchWindowTask watchWindowTask;
    MeasurementTask measurementTask;

    QPointer<OiJob> currentJob;

    //##############
    //helper methods
    //##############

    //request processing
    void getProject(OiRequestResponse request);
    void setProject(OiRequestResponse request);
    void getActiveFeature(OiRequestResponse request);
    void setActiveFeature(OiRequestResponse request);
    void getActiveStation(OiRequestResponse request);
    void setActiveStation(OiRequestResponse request);
    void getActiveCoordinateSystem(OiRequestResponse request);
    void setActiveCoordinateSystem(OiRequestResponse request);
    void aim(OiRequestResponse request);
    void move(OiRequestResponse request);
    void measure(OiRequestResponse request);
    void startWatchwindow(OiRequestResponse request);
    void stopWatchwindow(OiRequestResponse request);

    //prepare response to client
    void prepareResponse(OiRequestResponse &request);

    bool buildWatchWindowMessage(QDomElement &wwTag, const int &readingType, const QVariantMap &streamData);

private slots:

    //##################################
    //watch window and measure callbacks
    //##################################

    void receiveWatchWindowData(const QVariantMap &data);
    void measurementFinished(const bool &success);
};

#endif // OIREQUESTHANDLER_H
