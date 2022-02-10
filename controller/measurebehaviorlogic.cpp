#include "measurebehaviorlogic.h"

MeasureBehaviorLogic::MeasureBehaviorLogic(QObject *parent) : QObject(parent), activeFeatureId(-1)
{
    QObject::connect(&this->measureBehaviorDialog, &QDialog::accepted, this, &MeasureBehaviorLogic::handleDecision);
    QObject::connect(&this->measureBehaviorDialog, &QDialog::rejected, this, &MeasureBehaviorLogic::abortMeasurement);
}

void MeasureBehaviorLogic::init(ControllerSensorActions *control, QList<int> measureFeatures, FeatureTableModel *sourceModel) {
    this->measureBehaviorDialog.resetDecision();

    this->control = control;
    this->measureFeatures = measureFeatures;
    this->sourceModel = sourceModel;
    this->activeFeatureId = -1;
}

/**
 * aim, search (optional) and measure active feature in the list of selected features
 *
 * @brief MeasureBehaviorLogic::measure
 */
void MeasureBehaviorLogic::measure() {

    // aim / move
    this->control->startAim();

    // search
    if(this->measureBehaviorDialog.searchSMR()) {
        this->control->startSearch();
    }

    // measure
    this->control->startMeasurement();

}

/**
 * set next feature as active feature
 *
 * @brief MeasureBehaviorLogic::next
 * @return true if next feature is available
 */
bool MeasureBehaviorLogic::next() {
    if(this->measureFeatures.size() > 0) {
        setActiveFeature(this->sourceModel, this->measureFeatures[0]);
        this->activeFeatureId = this->measureFeatures[0];
        this->measureFeatures.removeAt(0);
        return true;

    } else {
        this->activeFeatureId = -1;
        return false;
    }

}

/**
 * receives all "sensorActionFinished" signals and handles only "measure" signals
 *
 * @brief MeasureBehaviorLogic::sensorActionFinished
 * @param success
 * @param msg
 */
void MeasureBehaviorLogic::sensorActionFinished(const bool &success, const QString &msg) {

    if(activeFeatureId == -1) { // silent ignore
        return;
    }

    if( success
       && ( SensorWorkerMessage::MEASUREMENT_FINISHED == msg            // sync sensor
            || SensorWorkerMessage::MEASUREMENT_DATA_RECEIVED == msg)   // async sensor
       ) {
        if(this->next()) { // measure next feature if avialable
            this->measure();
        }

    } else if( !success
              && ( SensorWorkerMessage::FAILED_TO_MEASURE == msg                            // sync sensor
                   || SensorWorkerMessage::MEASUREMENT_DIT_NOT_DELIVER_ANY_RESULTS == msg)  // async sensor
              ){
        showCentered(this->measureBehaviorDialog); // QDialog::accepted calls handleDecision() by signal / slot
    }

    // silent ignore all other cases
}

void MeasureBehaviorLogic::handleDecision() {

    if(this->measureBehaviorDialog.tryAgain()
            || this->measureBehaviorDialog.searchSMR()) {

        this->measure();

    } else if(this->measureBehaviorDialog.skip()) {
        if(this->next()) {
            this->measure();
        }

    }

}

/**
 * abort measurement
 *
 * @brief MeasureBehaviorLogic::abortMeasurement
 */
void MeasureBehaviorLogic::abortMeasurement() {
    this->measureFeatures.clear();
}

void MeasureBehaviorLogic::showCentered(QDialog &dialog) {
    dialog.show();
    // TODO refactoring: "use" MainWindow::showCentered
}

void MeasureBehaviorLogic::setActiveFeature(FeatureTableModel *sourceModel, int featureId) {
    sourceModel->setActiveFeature(featureId);
}
