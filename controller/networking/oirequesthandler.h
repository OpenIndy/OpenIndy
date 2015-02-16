#ifndef OIREQUESTHANDLER_H
#define OIREQUESTHANDLER_H

#include <QObject>
#include <QtXml>
#include <QThread>

#include "oirequestresponse.h"
//#include "oifeaturestate.h"
#include "featurewrapper.h"
#include "oiprojectexchanger.h"
#include "station.h"
#include "sensorlistener.h"
#include "featureupdater.h"

struct WatchWindowTask{
    bool taskInProcess;
    OiRequestResponse *request;
};
struct MeasurementTask{
    bool taskInProcess;
    OiRequestResponse *request;
};

/*!
 * \brief The OiRequestHandler class
 * This class receives XML based requests and emits the corresponding response as XML
 */
class OiRequestHandler : public QObject
{
    Q_OBJECT
private:
    explicit OiRequestHandler(QObject *parent = 0);

public:
    static OiRequestHandler *getInstance();

public slots:
    bool receiveRequest(OiRequestResponse *request);

    void receiveOiToolResponse(OiRequestResponse *response);

signals:
    bool sendResponse(OiRequestResponse *response);

    void sendOiToolRequest(OiRequestResponse *request);

private:
    static OiRequestHandler *myRequestHandler;

    QThread workerThread;

    WatchWindowTask myWatchWindowTask;
    MeasurementTask myMeasurementTask;

    void getProject(OiRequestResponse *request);
    void setProject(OiRequestResponse *request);
    void getActiveFeature(OiRequestResponse *request);
    void setActiveFeature(OiRequestResponse *request);
    void getActiveStation(OiRequestResponse *request);
    void setActiveStation(OiRequestResponse *request);
    void getActiveCoordinateSystem(OiRequestResponse *request);
    void setActiveCoordinateSystem(OiRequestResponse *request);
    void aim(OiRequestResponse *request);
    void move(OiRequestResponse *request);
    void measure(OiRequestResponse *request);
    void startWatchwindow(OiRequestResponse *request);
    void stopWatchwindow(OiRequestResponse *request);

    void prepareResponse(OiRequestResponse *request);

    bool buildWatchWindowMessage(QDomElement &wwTag, int readingType, QVariantMap streamData);

private slots:
    void receiveWatchWindowData(QVariantMap data);

    void measurementFinished(bool success);
};

#endif // OIREQUESTHANDLER_H
