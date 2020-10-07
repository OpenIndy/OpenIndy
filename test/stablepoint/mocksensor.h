#ifndef MOCKSENSOR_H
#define MOCKSENSOR_H

#include <QObject>
#include <QVariantMap>
#include <QThread>
#include <QString>
#include <QStringList>
#include <QQueue>
#include <QDebug>
#include <atomic>
#include "types.h"
#include <QElapsedTimer>

using namespace oi;

class MockSensor : public QThread
{
    Q_OBJECT
public:
    MockSensor(int sleep, QString readings, QObject *parent = 0);
    ~MockSensor();

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

#endif // MOCKSENSOR_H
