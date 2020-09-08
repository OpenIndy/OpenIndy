#ifndef TESTSENSOR_H
#define TESTSENSOR_H

#include <QObject>
#include <QVariantMap>
#include <QThread>
#include <QDebug>

class TestSensor : public QObject
{
    Q_OBJECT
public:
    TestSensor(QObject *parent = 0);
    ~TestSensor();

signals:
    void realTimeReading(const QVariantMap &reading);
    void finished();
    void error(QString err);

public slots:
    void process();

private:
    QVariantMap cartesianReading(double x, double y, double z);

};

#endif // TESTSENSOR_H
