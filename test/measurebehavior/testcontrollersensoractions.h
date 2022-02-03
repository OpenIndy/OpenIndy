#ifndef TESTCONTROLLERSENSORACTIONS_H
#define TESTCONTROLLERSENSORACTIONS_H

#include "controllersensoractions.h"

class TestControllerSensorActions: public ControllerSensorActions
{
public:
    TestControllerSensorActions();

    //sensor actions
    void startConnect();
    void startDisconnect();
    void startMeasurement();
    void startMove(const oi::Reading &reading);
    void startAim();
    void startAimAndMeasure();
    void startToggleSight();
    void startInitialize();
    void startHome();
    void startCompensation();
    void startChangeMotorState();
    void startCustomAction(const QString &task);
    void startReadingStream(oi::ReadingTypes streamFormat);
    void stopReadingStream();
    void finishMeasurement();
    void startSearch();
};

#endif // TESTCONTROLLERSENSORACTIONS_H
