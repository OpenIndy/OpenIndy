#include "testcontrollersensoractions.h"

TestControllerSensorActions::TestControllerSensorActions()
{

}

//sensor actions
void TestControllerSensorActions::startConnect() {}
void TestControllerSensorActions::startDisconnect() {}
void TestControllerSensorActions::startMeasurement() {}
void TestControllerSensorActions::startMove(const oi::Reading &reading) {}
void TestControllerSensorActions::startAim() { /* move */}
void TestControllerSensorActions::startAimAndMeasure() {}
void TestControllerSensorActions::startToggleSight() {}
void TestControllerSensorActions::startInitialize() {}
void TestControllerSensorActions::startHome() {}
void TestControllerSensorActions::startCompensation() {}
void TestControllerSensorActions::startChangeMotorState() {}
void TestControllerSensorActions::startCustomAction(const QString &task) {}
void TestControllerSensorActions::startReadingStream(oi::ReadingTypes streamFormat) {}
void TestControllerSensorActions::stopReadingStream() {}
void TestControllerSensorActions::finishMeasurement() {}
void TestControllerSensorActions::startSearch() {}
