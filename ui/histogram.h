#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <QWidget>
#include <QPainter>
#include <QToolTip>
#include <QMouseEvent>
#include <QtAlgorithms>
#include "oifeaturestate.h"

class Histogram : public QWidget
{
    Q_OBJECT
public:
    explicit Histogram(QWidget *parent = 0);

signals:

public slots:
    void paintData(FeatureWrapper* f, QString attributeToDraw);

protected:
    void paintEvent(QPaintEvent* event);
    void mouseMoveEvent(QMouseEvent * event);

private:
    QVector<double> _bins;

    double actualValue;
    double uncertainty;
    double expectation;

    double maxError;
    double minError;
    double errorScale;

    double maxFrequency;
    double minFrequency;
    double frequencyScale;

    QString distribution;
    QString featureAttribute;

    QList<double> densityValues;

    SimulationData simData;

    void generateDataToDraw(FeatureWrapper* f, QString attributeToDraw);
    void generateDensityList(QList<double> d);



};

#endif // HISTOGRAM_H
