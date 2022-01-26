#include "measurebehaviorlogic.h"

MeasureBehaviorLogic::MeasureBehaviorLogic(QObject *parent) : QObject(parent)
{

}

bool MeasureBehaviorLogic::measure(Controller &control, QList<int> measureFeatures, FeatureTableModel *sourceModel) {
    //aim and measure the first feature in the list of selected features
    this->measureBehaviorDialog.resetDecision();
    bool measureNextFeature = true;
    if(measureFeatures.size() > 0){

        if(measureNextFeature) {
            sourceModel->setActiveFeature(measureFeatures[0]);
            measureFeatures.removeAt(0);
        }

        // aim / move
        control.startAim();

        // search
        if(this->measureBehaviorDialog.searchSMR()) {
            control.startSearch();
        }

        // measure
        try {
            control.startMeasurement();
        } catch(...) { // explicit exception
            this->showCentered(this->measureBehaviorDialog);
        }
    }

    return true;
}

void MeasureBehaviorLogic::showCentered(QDialog &dialog) {
    dialog.show();
    // TODO refactoring: "use" MainWindow::showCentered
}
