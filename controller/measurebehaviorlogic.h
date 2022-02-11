#ifndef MEASUREBEHAVIORLOGIC_H
#define MEASUREBEHAVIORLOGIC_H

#include <QObject>
#include <QPointer>

#include "measurebehaviordialog.h"
#include "controllersensoractions.h"
#include "featuretablemodel.h"
#include "sensorworkermessage.h"
#include "measurebehaviortypes.h"

class MeasureBehaviorLogic : public QObject
{
    Q_OBJECT
public:
    explicit MeasureBehaviorLogic(QObject *parent = 0);

    void init(ControllerSensorActions *control, QList<int> measureFeatures, FeatureTableModel *sourceModel);
    bool next();
    void measure();

public slots:
    void sensorActionFinished(const bool &success, const QString &msg);
    void setDecision(Decision decision);

protected slots:
    void handleDecision();

private slots:
    void abortMeasurement();

protected:
    virtual void showCentered(QDialog &dialog);

    virtual void setActiveFeature(FeatureTableModel *sourceModel, int featureId);

private:
    bool skip();
    bool search();
    bool tryAgain();
    void resetDecision();

    MeasureBehaviorDialog measureBehaviorDialog;

    ControllerSensorActions *control;
    FeatureTableModel *sourceModel;
    QList<int> measureFeatures;

    int activeFeatureId;
    Decision decision;

};

#endif // MEASUREBEHAVIORLOGIC_H
