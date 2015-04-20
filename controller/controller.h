#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QPointer>

#include "featureattributes.h"
#include "reading.h"
#include "observation.h"
#include "featurewrapper.h"
#include "oijob.h"
#include "modelmanager.h"

//! \brief The Controller class
class Controller : public QObject
{
    Q_OBJECT
public:
    explicit Controller(QObject *parent = 0);

signals:

    //######################################################
    //signals to inform GUI about changes in the current job
    //######################################################

    //feature set changed
    void featureSetChanged();
    void coordSystemSetChanged();

    //general feature attributes changed
    void featureAttributesChanged();
    void featureFunctionsChanged();

    //geometry specific attributes changed
    void geometryObservationsChanged();
    void geometryMeasurementConfigChanged();

    //active feature changes
    void activeFeatureChanged();
    void activeStationChanged();
    void activeCoordinateSystemChanged();

    //feature group changes
    void activeGroupChanged();
    void availableGroupNamesChanged();

public slots:

    //###########################################
    //actions to be performed (triggered by user)
    //###########################################

    //active feature states
    void setActiveFeature(const int &featureId);
    void setActiveStation(const int &featureId);
    void setActiveCoordinateSystem(const int &featureId);

    //add or remove features
    void addFeatures(const FeatureAttributes &attributes);
    void removeFeature(const int &featureId);

    //sensor methods
    void startMeasurement();
	void addMeasurement();
    void startMove(Reading *parameter);
    void startAim();
    void startConnect();
    void startDisconnect();
    void startToggleSight();
    void startInitialize();
    void startHome();
    void startCompensation();
    void startChangeMotorState();
    void startCustomAction(const QString &s);

    //recalculation of features
    //void recalcFeatures();
    //void recalcActiveFeature();
    //void recalcFeature(const int &featureId);
    //void recalcTrafoParam(const int &featureId);

    //save or load OpenIndy projects
    //bool saveProject();
    //bool loadProject(const QString &projectName, QIODevice *myDevice);

    //feature group changes
    //void setActiveGroup(const QString &group);

private slots:

    //void connectStateChanges();
    //bool createDefaultProject();

private:

    QPointer<OiJob> currentJob;

public:

    Console *c;

};

#endif // CONTROLLER_H
