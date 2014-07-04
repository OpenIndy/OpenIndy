#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <QWidget>
#include <QPainter>
#include <QToolTip>
#include <QMouseEvent>
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
    double maxError;
    double minError;
    double errorScale;



};

#endif // HISTOGRAM_H
