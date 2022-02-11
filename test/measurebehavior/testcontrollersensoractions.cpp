#include "testcontrollersensoractions.h"

TestControllerSensorActions::TestControllerSensorActions()
{

}

//sensor actions
void TestControllerSensorActions::startConnect() {
    this->actions.append("startConnect");
}
void TestControllerSensorActions::startDisconnect() {
    this->actions.append("startDisconnect");
}
void TestControllerSensorActions::startMeasurement() {
    this->actions.append("startMeasurement");
}
void TestControllerSensorActions::startMove(const oi::Reading &reading) {
    this->actions.append("startMove");
}
void TestControllerSensorActions::startAim() {
    this->actions.append("startAim");
}
void TestControllerSensorActions::startAimAndMeasure() {
    this->actions.append("startAimAndMeasure");
}
void TestControllerSensorActions::startToggleSight() {
    this->actions.append("startToggleSight");
}
void TestControllerSensorActions::startInitialize() {
    this->actions.append("startInitialize");
}
void TestControllerSensorActions::startHome() {
    this->actions.append("startHome");
}
void TestControllerSensorActions::startCompensation() {
    this->actions.append("startCompensation");
}
void TestControllerSensorActions::startChangeMotorState() {
    this->actions.append("startChangeMotorState");
}
void TestControllerSensorActions::startCustomAction(const QString &task) {
    this->actions.append("startCustomAction: " + task);
}
void TestControllerSensorActions::startReadingStream(oi::ReadingTypes streamFormat) {
    this->actions.append("startReadingStream");
}
void TestControllerSensorActions::stopReadingStream() {
    this->actions.append("stopReadingStream");
}
void TestControllerSensorActions::finishMeasurement() {
    this->actions.append("startConnect");
}
void TestControllerSensorActions::startSearch() {
    this->actions.append("startSearch");
}
