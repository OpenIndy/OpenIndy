#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <QWidget>
#include <QPainter>
#include <QToolTip>
#include <QMouseEvent>
#include <QtAlgorithms>
#include <QFont>
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

    QPointF actualPoint;
    QPointF expectationPoint;

    double maxError;
    double minError;
    double errorScale;

    double maxFrequency;
    double minFrequency;
    double frequencyScale;

    double scale;
    float xLeft;
    float xRight;
    float yTop;
    float yBottom;
    float width;
    float height;

    float resolution;

    QString distribution;
    QString featureAttribute;

    QList<double> densityValues;

    SimulationData simData;

    void generateDataToDraw(FeatureWrapper* f, QString attributeToDraw);
    void generateDensityList(QList<double> d);
    void drawGrid();
    void drawResultSet();



};

#endif // HISTOGRAM_H
