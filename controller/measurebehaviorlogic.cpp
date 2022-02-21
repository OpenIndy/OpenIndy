#include "measurebehaviorlogic.h"

MeasureBehaviorLogic::MeasureBehaviorLogic(QObject *parent) : QObject(parent), activeFeatureId(-1)
{
    QObject::connect(&this->measureBehaviorDialog, &MeasureBehaviorDialog::setDecision, this, &MeasureBehaviorLogic::setDecision);
    QObject::connect(&this->measureBehaviorDialog, &QDialog::accepted, this, &MeasureBehaviorLogic::handleDecision);
    QObject::connect(&this->measureBehaviorDialog, &QDialog::rejected, this, &MeasureBehaviorLogic::abortMeasurement);
}

void MeasureBehaviorLogic::init(ControllerSensorActions *control, QList<int> measureFeatures, FeatureTableModel *sourceModel, QList<QPointer<QDialog> > dialogs) {
    this->resetDecision();

    this->control = control;
    this->measureFeatures = measureFeatures;
    this->sourceModel = sourceModel;
    this->activeFeatureId = -1;
    this->dialogs = dialogs;
}

/**
 * aim, search (optional) and measure active feature in the list of selected features
 *
 * @brief MeasureBehaviorLogic::measure
 */
void MeasureBehaviorLogic::measure() {

    // aim / move
    this->control->startAim();

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
        // close / supress all unwanted dialogs
        QList<QPointer<QDialog> >::iterator dialog;
        for (dialog = dialogs.begin(); dialog != dialogs.end(); ++dialog){
            if(!(*dialog).isNull()) {
               (*dialog)->close(); // close dialog
            }
        }
        showCentered(this->measureBehaviorDialog); // QDialog::accepted calls handleDecision() by signal / slot
    }

    // silent ignore all other cases
}

void MeasureBehaviorLogic::handleDecision() {

    if(tryAgain()) {
        qDebug() << "tryAgain" << this->activeFeatureId;
        resetDecision();
        // aim / measure
        this->measure();

    } else if(skip()) {
        qDebug() << "skip" << this->activeFeatureId;
        resetDecision();
        // skip
        if(this->next()) {
            // aim / measure
            this->measure();
        }

    } else if(search()) {
        qDebug() << "search" << this->activeFeatureId;
        resetDecision();
        // search
        this->control->startSearch();

        // measure
        this->control->startMeasurement();
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

void MeasureBehaviorLogic::setDecision(Decision decision) {
    this->decision = decision;
}

void MeasureBehaviorLogic::resetDecision() {
    this->decision = Decision::eNotSet;
}

bool MeasureBehaviorLogic::skip() {
    return this->decision == Decision::eSkip;
}

bool MeasureBehaviorLogic::search() {
    return this->decision == Decision::eSearch;
}

bool MeasureBehaviorLogic::tryAgain() {
    return this->decision == Decision::eTryAgain;
}

