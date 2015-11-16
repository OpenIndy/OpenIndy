#ifndef OIWEBSOCKETSTREAMER_H
#define OIWEBSOCKETSTREAMER_H

#include <QObject>
#include <QtXml>
#include <QPointer>

#include "oirequestresponse.h"
#include "oijob.h"

/*!
 * \brief The OiWebSocketStreamer class
 * Sends information to OpenIndy-Clients asynchronously
 */
class OiWebSocketStreamer : public QObject
{
    Q_OBJECT

public:
    explicit OiWebSocketStreamer(QObject *parent = 0);

    //######################
    //get or set current job
    //######################

    const QPointer<oi::OiJob> &getJob() const;
    void setJob(const QPointer<oi::OiJob> &job);

public slots:

signals:

    //###########################
    //send information to clients
    //###########################

private:

    //#################
    //helper attributes
    //#################

    QPointer<oi::OiJob> currentJob;

    //##############
    //helper methods
    //##############

private slots:

};

#endif // OIWEBSOCKETSTREAMER_H
