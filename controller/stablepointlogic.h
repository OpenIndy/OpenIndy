#ifndef STABLEPOINTLOGIC_H
#define STABLEPOINTLOGIC_H

#include <QObject>
#include <QVariant>
#include "station.h"
#include "oijob.h"

namespace oi{

class StablePointLogic : public QObject
{
    Q_OBJECT

public:
    explicit StablePointLogic(QPointer<OiJob> job, QObject *parent = 0);
    StablePointLogic(const StablePointLogic &copy, QObject *parent = 0);
    ~StablePointLogic();

public slots:
    void startStablePointMeasurement();
    void stopStablePointMeasurement();

signals:
    void startMeasurement();
    void stopStreaming();
    void measure();
    void startStreaming(ReadingTypes rt);

private slots:
    void checkStablePoint();
    void realTimeReading(const QVariantMap &reading);

    void connectSensor();

private:
    //QPointer<Station> activeStation;
    bool pointIsStable;
    QPointer<OiJob> job;
};

}
#endif // STABLEPOINTLOGIC_H
