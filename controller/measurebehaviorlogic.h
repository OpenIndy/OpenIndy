#ifndef MEASUREBEHAVIORLOGIC_H
#define MEASUREBEHAVIORLOGIC_H

#include <QObject>

#include "measurebehaviordialog.h"
#include "controller.h"
#include "featuretablemodel.h"

class MeasureBehaviorLogic : public QObject
{
    Q_OBJECT
public:
    explicit MeasureBehaviorLogic(QObject *parent = 0);

    bool measure(Controller &control, QList<int> measureFeatures, FeatureTableModel *sourceModel);
signals:

public slots:

protected:
    void showCentered(QDialog &dialog);

private:
    MeasureBehaviorDialog measureBehaviorDialog;

};

#endif // MEASUREBEHAVIORLOGIC_H
