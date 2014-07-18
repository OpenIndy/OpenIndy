#include "histogram.h"
#include <cmath>

Histogram::Histogram(QWidget *parent) :
    QWidget(parent)
{

    this->setMouseTracking(true);
}

void Histogram::paintData(FeatureWrapper* f, QString attributeToDraw)
{


    this->generateDataToDraw(f,attributeToDraw);


    this->repaint();

}

void Histogram::paintEvent(QPaintEvent *event)
{
    //define viewport
    QRect viewPort = rect();
    xLeft = viewPort.left();
    xRight = viewPort.right();
    yTop = viewPort.top();
    yBottom = viewPort.bottom();
    width = viewPort.width();
    height = viewPort.height();

    //painter
    QPainter painter(this);

    //pen
    QPen pen;
    pen.setColor(Qt::black);
    pen.setWidth(2);
    painter.setPen(pen);

    //brush
    QBrush brush(Qt::SolidPattern);
    brush.setColor("#DDDDDD");
    painter.setBrush(brush);

    //Draw gray background
    painter.drawRect(xLeft, yTop, xRight, yBottom);

    if( _bins.size() == 0 )
    {
        pen.setColor("#016790");
        painter.setPen(pen);
        painter.drawText(xLeft+3, yBottom-5, tr("Histogram off"));
        return;
    }

    //scale
    if(width < height){
        scale = width;
    }else{
        scale = height;
    }

    scale = scale/1.1;


    //draw density function
    pen.setColor("#016790");
    painter.setPen(pen);

    QPolygon densityGraph;


    for( int i=0; i<_bins.size(); i++ ){
        densityGraph << QPoint(xLeft+(scale * _bins[i]), yBottom-(scale * densityValues.at(i)));
    }

    painter.drawPolyline(densityGraph);

    //draw Grid
    this->drawGrid();

    //draw results
    this->drawResultSet();


}

void Histogram::mouseMoveEvent(QMouseEvent *event)
{

    double X = event->x()/scale;
    double Y = (event->y()+yBottom)/scale;

    X = minError+(X/errorScale);
    //Y = maxFrequency-(Y/frequencyScale);

    QString x = QString::number(X);
    QString y = QString::number(Y);

    QToolTip::showText(event->globalPos(),QString("x:"+x+","+"y:"+y),this);
}

void Histogram::generateDataToDraw(FeatureWrapper* f, QString attributeToDraw)
{
    _bins.clear();
    densityValues.clear();

    simData = f->getGeometry()->getSimulationData();

    if(attributeToDraw.compare("X") == 0){

        featureAttribute = "X";
        actualValue = f->getGeometry()->getXYZ().getAt(0);
        distribution = simData.uncertaintyX.distribution;
        maxError = simData.uncertaintyX.maxValue;
        minError = simData.uncertaintyX.minValue;
        uncertainty = simData.uncertaintyX.uncertainty;
        expectation = simData.uncertaintyX.expectation;

        errorScale = 1/(maxError-minError);

        QList<double> tmpList =simData.uncertaintyX.values;

        qSort(tmpList);

        QList<double> tmpDensity;
        QList<double> tmpDensitySorted;

        foreach(double d, tmpList){

            double w = simData.uncertaintyX.densityFunction(d,expectation,uncertainty);

            tmpDensity.append(w);
            tmpDensitySorted.append(w);

             _bins.append(errorScale*(d-minError));
        }

        qSort(tmpDensitySorted);
        minFrequency = tmpDensitySorted.first();
        maxFrequency = tmpDensitySorted.last();
        frequencyScale = 1/(maxFrequency-minFrequency);

        for(int i = 0;i<tmpDensity.size();i++){
            densityValues.append(frequencyScale*(tmpDensity.at(i)-minFrequency));
        }

        //draw point
        double h = simData.uncertaintyX.densityFunction(actualValue,expectation,uncertainty);

        actualPoint.setX(errorScale*(actualValue-minError));
        actualPoint.setY(frequencyScale*(h-minFrequency));

        expectationPoint.setX(errorScale*(expectation-minError));
        expectationPoint.setY(1);


    }else if(attributeToDraw.compare("Y") == 0){

        featureAttribute = "Y";
        actualValue = f->getGeometry()->getXYZ().getAt(1);
        distribution = simData.uncertaintyY.distribution;
        maxError = simData.uncertaintyY.maxValue;
        minError = simData.uncertaintyY.minValue;
        uncertainty = simData.uncertaintyY.uncertainty;
        expectation = simData.uncertaintyY.expectation;

        errorScale = 1/(maxError-minError);

        QList<double> tmpList =simData.uncertaintyY.values;

        qSort(tmpList);

        QList<double> tmpDensity;

        foreach(double d, tmpList){

            double w = simData.uncertaintyY.densityFunction(d,expectation,uncertainty);

            tmpDensity.append(w);

             _bins.append(errorScale*(maxError-d));
        }

        qSort(tmpDensity);
        minFrequency = tmpDensity.first();
        maxFrequency = tmpDensity.last();
        frequencyScale = 1/(maxFrequency-minFrequency);

        for(int i = 0;i<tmpDensity.size();i++){
            densityValues.append(frequencyScale*(maxFrequency-tmpDensity.at(i)));
        }

    }else if(attributeToDraw.compare("Z") == 0){

        featureAttribute = "Z";
        actualValue = f->getGeometry()->getXYZ().getAt(2);
        distribution = simData.uncertaintyY.distribution;
        maxError = simData.uncertaintyZ.maxValue;
        minError = simData.uncertaintyZ.minValue;
        uncertainty = simData.uncertaintyZ.uncertainty;
        expectation = simData.uncertaintyZ.expectation;

        errorScale = 1/(maxError-minError);

        QList<double> tmpList =simData.uncertaintyZ.values;

        qSort(tmpList);

        QList<double> tmpDensity;

        foreach(double d, tmpList){

            double w = simData.uncertaintyZ.densityFunction(d,expectation,uncertainty);

            tmpDensity.append(w);

             _bins.append(errorScale*(maxError-d));
        }

        qSort(tmpDensity);
        minFrequency = tmpDensity.first();
        maxFrequency = tmpDensity.last();
        frequencyScale = 1/(maxFrequency-minFrequency);

        for(int i = 0;i<tmpDensity.size();i++){
            densityValues.append(frequencyScale*(maxFrequency-tmpDensity.at(i)));
        }

    }
}

void Histogram::generateDensityList(QList<double> d)
{

}

void Histogram::drawGrid()
{
    QPainter painter(this);
    QPen pen;

    pen.setWidth(1);
    pen.setColor("#AAAAAA");
    pen.setStyle(Qt::DashDotLine);
    painter.setPen(pen);

    int stepsV = 1<< int(log(width/40.0f)/log(2.0f));
    for(int i=1; i<stepsV; ++i)
    {

        painter.drawLine(width*float(i)/stepsV, yTop+1,
                         width*float(i)/stepsV, yBottom-1);

    }

    int stepsH = 1<< int(log(height/40.0f)/log(2.0f));
    for(int i=1; i<stepsH; ++i)
    {

        painter.drawLine(xLeft+1, height*float(i)/stepsH,
                         xRight-1,height*float(i)/stepsH);

    }
}

void Histogram::drawResultSet()
{
    QPainter painter(this);
    QPen pen;

    pen.setWidth(3);
    pen.setColor(Qt::black);
    painter.setPen(pen);

    QString a = QString::number(actualValue*UnitConverter::getDistanceMultiplier(),'f',6);
    QString e = QString::number(expectation*UnitConverter::getDistanceMultiplier(),'f',6);
    QString u = QString::number(uncertainty*UnitConverter::getDistanceMultiplier(),'f',6);
    QString maxV = QString::number((maxError-expectation)*UnitConverter::getDistanceMultiplier(),'f',6);
    QString minV = QString::number((minError-expectation)*UnitConverter::getDistanceMultiplier(),'f',6);

    painter.drawText(xRight-200, yTop+10, distribution);
    painter.drawText(xRight-200, yTop+20, QString("actual "+ featureAttribute +": " + a));
    painter.drawText(xRight-200, yTop+30, QString("expectation: " + e));
    painter.drawText(xRight-200, yTop+40, QString("uncertainty: " + u));
    painter.drawText(xRight-200, yTop+50, QString("max diff: " + maxV));
    painter.drawText(xRight-200, yTop+60, QString("min diff: " + minV));

}
