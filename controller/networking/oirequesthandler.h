#ifndef OIREQUESTHANDLER_H
#define OIREQUESTHANDLER_H

#include <QObject>
#include <QtXml>
#include <QThread>

#include "oirequestresponse.h"
#include "oifeaturestate.h"
#include "featurewrapper.h"
#include "oiprojectexchanger.h"
#include "oistakeout.h"

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

signals:
    bool sendResponse(OiRequestResponse *response);

private:
    static OiRequestHandler *myRequestHandler;

    QThread workerThread;

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
    void startStakeOut(OiRequestResponse *request);
    void stopStakeOut(OiRequestResponse *request);
    void GetNextGeometry(OiRequestResponse *request);

    void prepareResponse(OiRequestResponse *request);
};

#endif // OIREQUESTHANDLER_H
