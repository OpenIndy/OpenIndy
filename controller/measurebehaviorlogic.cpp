#include "measurebehaviorlogic.h"

MeasureBehaviorLogic::MeasureBehaviorLogic(QObject *parent) : QObject(parent)
{

}

void MeasureBehaviorLogic::init(ControllerSensorActions *control, QList<int> measureFeatures, FeatureTableModel *sourceModel) {
    this->measureBehaviorDialog.resetDecision();

    this->control = control;
    this->measureFeatures = measureFeatures;
    this->sourceModel = sourceModel;
}

/**
 * aim and measure current feature in the list of selected features
 *
 * @brief MeasureBehaviorLogic::measureNext
 */
void MeasureBehaviorLogic::measure() {

    if(this->measureFeatures.size() > 0) {

        // aim / move
        this->control->startAim();

        // search
        if(this->measureBehaviorDialog.searchSMR()) {
            this->control->startSearch();
        }

        // measure
        this->control->startMeasurement();

    }

}

/**
 * set next feature as active feature
 * @brief MeasureBehaviorLogic::next
 * @return true if next feature is available
 */
bool MeasureBehaviorLogic::next() {
    if(this->measureFeatures.size() > 0) {
        setActiveFeature(this->sourceModel, this->measureFeatures[0]);
        this->measureFeatures.removeAt(0);
        return true;
    }
    return false;
}

void MeasureBehaviorLogic::sensorActionFinished(const bool &success, const QString &msg) {
    qDebug() << "MeasureBehaviorLogic::sensorActionFinished" << success << msg;
    if(false && "measurement finished" != msg) {
        return;
    }

    if(success) {
        if(this->next()) { // measure next feature if avialable
            this->measure();
        }

    } else { //

        if(!this->measureBehaviorDialog.useDecision()) {
            showCentered(this->measureBehaviorDialog);
        }

        if(this->measureBehaviorDialog.tryAgain()) {
            this->measure();

        } else if(this->measureBehaviorDialog.skip()
                  && this->next()) {
            this->measure();

        }

    }
}

void MeasureBehaviorLogic::showCentered(QDialog &dialog) {
    dialog.show();
    // TODO refactoring: "use" MainWindow::showCentered
}

void MeasureBehaviorLogic::setActiveFeature(FeatureTableModel *sourceModel, int featureId) {
    sourceModel->setActiveFeature(featureId);
}
