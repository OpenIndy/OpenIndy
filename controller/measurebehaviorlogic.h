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

signals:

public slots:
    void sensorActionFinished(const bool &success, const QString &msg);
    void setDecision(Decision decision);

private slots:
    void handleDecision();
    void abortMeasurement();


protected:
    virtual void showCentered(QDialog &dialog);

    virtual void setActiveFeature(FeatureTableModel *sourceModel, int featureId);

private:
    bool skip();
    bool searchSMR();
    bool tryAgain();

    MeasureBehaviorDialog measureBehaviorDialog;

    ControllerSensorActions *control;
    QList<int> measureFeatures;
    int activeFeatureId;
    FeatureTableModel *sourceModel;
    Decision decision;

};

#endif // MEASUREBEHAVIORLOGIC_H
