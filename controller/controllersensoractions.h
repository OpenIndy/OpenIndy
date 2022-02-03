#ifndef CONTROLLERSENSORACTIONS_H
#define CONTROLLERSENSORACTIONS_H

#include <QString>

#include "reading.h"
#include "types.h"

class ControllerSensorActions
{
public:

    virtual void startConnect() = 0;
    virtual void startDisconnect() = 0;
    virtual void startMeasurement() = 0;
    virtual void startMove(const oi::Reading &reading) = 0;
    virtual void startAim() = 0;
    virtual void startAimAndMeasure() = 0;
    virtual void startToggleSight() = 0;
    virtual void startInitialize() = 0;
    virtual void startHome() = 0;
    virtual void startCompensation() = 0;
    virtual void startChangeMotorState() = 0;
    virtual void startCustomAction(const QString &task) = 0;
    virtual void startReadingStream(oi::ReadingTypes streamFormat) = 0;
    virtual void stopReadingStream() = 0;
    virtual void finishMeasurement() = 0;
    virtual void startSearch() = 0;
};

#endif // CONTROLLERSENSORACTIONS_H
