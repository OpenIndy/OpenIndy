#ifndef MEASUREBEHAVIORLOGIC_H
#define MEASUREBEHAVIORLOGIC_H

#include <QObject>
#include <QPointer>

#include "measurebehaviordialog.h"
#include "controllersensoractions.h"
#include "featuretablemodel.h"
#include "sensorworkermessage.h"

class MeasureBehaviorLogic : public QObject
{
    Q_OBJECT
public:
    explicit MeasureBehaviorLogic(QObject *parent = 0);

    void init(ControllerSensorActions *control, QList<int> measureFeatures, FeatureTableModel *sourceModel);
    bool next();
    void measure();

signals:

public slots:
    void sensorActionFinished(const bool &success, const QString &msg);

protected:
    virtual void showCentered(QDialog &dialog);

    virtual void setActiveFeature(FeatureTableModel *sourceModel, int featureId);

private:

    MeasureBehaviorDialog measureBehaviorDialog;

    ControllerSensorActions *control;
    QList<int> measureFeatures;
    FeatureTableModel *sourceModel;
};

#endif // MEASUREBEHAVIORLOGIC_H
