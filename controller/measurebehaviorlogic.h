#ifndef MEASUREBEHAVIORLOGIC_H
#define MEASUREBEHAVIORLOGIC_H

#include <QObject>
#include <QPointer>
#include <QList>

#include "measurebehaviordialog.h"
#include "controllersensoractions.h"
#include "featuretablemodel.h"
#include "sensorworkermessage.h"
#include "measurebehaviortypes.h"

#if defined(OI_MAIN_LIB)
#  define OI_MAIN_EXPORT Q_DECL_EXPORT
#else
#  define OI_MAIN_EXPORT Q_DECL_IMPORT
#endif

class OI_MAIN_EXPORT MeasureBehaviorLogic : public QObject
{
    Q_OBJECT
public:
    explicit MeasureBehaviorLogic(QObject *parent = 0);

    void init(ControllerSensorActions *control, QList<int> measureFeatures, FeatureTableModel *sourceModel, QList<QPointer<QDialog> > dialogs);
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

    QList<QPointer<QDialog> > dialogs;

    int activeFeatureId;
    Decision decision;

};

#endif // MEASUREBEHAVIORLOGIC_H
