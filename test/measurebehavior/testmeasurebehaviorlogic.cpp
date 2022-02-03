#include "testmeasurebehaviorlogic.h"

TestMeasureBehaviorLogic::TestMeasureBehaviorLogic()
{

}

void TestMeasureBehaviorLogic::showCentered(QDialog &dialog) {
    qDebug() << "showCentered";
}

void TestMeasureBehaviorLogic::setActiveFeature(FeatureTableModel *sourceModel, int featureId) {
    qDebug() << "setActiveFeature: featureId: " << featureId;
}
