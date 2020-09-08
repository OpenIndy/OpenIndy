#ifndef STABLEPOINTLOGIC_H
#define STABLEPOINTLOGIC_H

#include <QDebug>
#include <QObject>
#include <QPointer>
#include <QTimer>
#include <QVariant>
#include <QtMath>
#include <QElapsedTimer>

#include "oivec.h"
#include "types.h"
#include "measurementconfig.h"

namespace oi{

class StablePointLogic : public QObject
{
    Q_OBJECT

public:
    explicit StablePointLogic(MeasurementConfig config, QObject *parent = 0);
    StablePointLogic(const StablePointLogic &copy, QObject *parent = 0);
    StablePointLogic &operator=(const StablePointLogic &copy);
    ~StablePointLogic();

public slots:
    void startStablePointMeasurement();
    void stopStablePointMeasurement();
    void realTimeReading(const QVariantMap &reading);

signals:
    void startMeasurement();
    void stopStreaming();
    void measure();
    void startStreaming(ReadingTypes rt);

private slots:
    void checkStablePoint();

private:
    //QPointer<Station> activeStation;
    bool pointIsStable;

};

}
#endif // STABLEPOINTLOGIC_H
