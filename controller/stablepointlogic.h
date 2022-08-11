#ifndef STABLEPOINTLOGIC_H
#define STABLEPOINTLOGIC_H

#include <QDebug>
#include <QObject>
#include <QPointer>
#include <QTimer>
#include <QVariant>
#include <QtMath>
#include <QElapsedTimer>
#include <QQueue>
#include <atomic>

#include "oivec.h"
#include "types.h"
#include "measurementconfig.h"
#include "float.h"  // DBL_MAX

#if defined(OI_MAIN_LIB)
#  define OI_MAIN_EXPORT Q_DECL_EXPORT
#else
#  define OI_MAIN_EXPORT Q_DECL_IMPORT
#endif

namespace oi{

struct ReadingData {
    oi::math::OiVec xyz;
    long elapsed;
    bool guessStable;
    double distanceToPrevReading;
    double distanceToPrevStable;
};

class OI_MAIN_EXPORT StablePointLogic : public QObject
{
    Q_OBJECT

public:
    explicit StablePointLogic(QObject *parent = 0);
    StablePointLogic(const StablePointLogic &copy, QObject *parent = 0);
    StablePointLogic &operator=(const StablePointLogic &copy);
    ~StablePointLogic();

public slots:
    void startStablePointMeasurement(MeasurementConfig config);
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
    void euclideanDistance(double &result, oi::math::OiVec v1, oi::math::OiVec v2); // TODO move to OiVec

private:
    QQueue<ReadingData> readingDatas;

    QElapsedTimer elapsedTimer;
    oi::math::OiVec lastStableXyz;
    MeasurementConfig config;

    std::atomic<bool> pointIsStable;

    QPointer<QTimer> checkStableTimer;

};

}
#endif // STABLEPOINTLOGIC_H
