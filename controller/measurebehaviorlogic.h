#ifndef MEASUREBEHAVIORLOGIC_H
#define MEASUREBEHAVIORLOGIC_H

#include <QObject>

#include "measurebehaviordialog.h"
#include "controllersensoractions.h"
#include "featuretablemodel.h"

class MeasureBehaviorLogic : public QObject
{
    Q_OBJECT
public:
    explicit MeasureBehaviorLogic(QObject *parent = 0);

    bool measure(ControllerSensorActions &control, QList<int> measureFeatures, FeatureTableModel *sourceModel);
signals:

public slots:

protected:
    void showCentered(QDialog &dialog);

    void setActiveFeature(FeatureTableModel *sourceModel, int featureId);

private:
    MeasureBehaviorDialog measureBehaviorDialog;

};

#endif // MEASUREBEHAVIORLOGIC_H
