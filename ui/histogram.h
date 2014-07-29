#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <QWidget>
#include <QPainter>
#include <QToolTip>
#include <QMouseEvent>
#include <QtAlgorithms>
#include <QFont>
#include <limits>
#include "oifeaturestate.h"

struct AttributeStats{
    QString name;
    QList<double> errors;
    double minError;
    double maxError;
    double uncertainty;
    double expectation;
    double actual;
    double unitMultiplier;
    int unitDigits;
    QColor color;
};

class Histogram : public QWidget
{
    Q_OBJECT
public:
    explicit Histogram(QWidget *parent = 0);



signals:

public slots:
    void paintData(FeatureWrapper* f, QString attributeToDraw);
    void setTypeOfUnit(QString t);

protected:
    void paintEvent(QPaintEvent* event);
    void mouseMoveEvent(QMouseEvent * event);

private:
    QString typeOfUnit;
    double unitMultiplier;
    int unitDigits;
    int iterationCount;

    QVector<double> _bins;
    bool drawAll;

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
    double scaleH;
    double scaleW;
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
    QMap<QString,AttributeStats> yValues;

    SimulationData simData;
    FeatureWrapper *actualFeature;

    void generateDataToDraw(FeatureWrapper* f, QString attributeToDraw);
    void drawGrid();
    void drawGridAll();
    void drawResultSet();
    void generateDensityList(QList<double> tmpList,UncertaintyData uData);
    void addErrorAttribute(AttributeStats a, QList<double> v);
    void setUpExpectationPoints(double h);


    void prepareAll();
    void prepareX();
    void prepareY();
    void prepareZ();
    void prepareI();
    void prepareJ();
    void prepareK();
    void prepareRadius();
    void prepareScalar();



};

#endif // HISTOGRAM_H
