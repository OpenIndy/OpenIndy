#ifndef TESTSENSOR_H
#define TESTSENSOR_H

#include <QObject>
#include <QVariantMap>
#include <QThread>
#include <QString>
#include <QStringList>
#include <QQueue>
#include <QDebug>
#include <atomic>
#include "types.h"

using namespace oi;

class TestSensor : public QThread
{
    Q_OBJECT
public:
    TestSensor(int sleep, QString readings, QObject *parent = 0);
    ~TestSensor();

signals:
    void realTimeReading(const QVariantMap &reading);

protected:
    void run();

public slots:
    void stopStreaming();
    void startStreaming(ReadingTypes rt);

private:
    QVariantMap cartesianReading(double x, double y, double z);
    int sleep;
    QString readings;
    std::atomic<bool> streaming;

};

#endif // TESTSENSOR_H
