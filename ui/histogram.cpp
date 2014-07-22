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
    QRectF viewPort = rect();
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
    brush.setColor(Qt::white);
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
    pen.setWidth(4);
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
    double Y = (yBottom-event->y())/scale;

    X = minError+(X/errorScale);
    //Y = maxFrequency-(Y/frequencyScale);


    QString x = QString::number(X*UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits);
    QString y = QString::number(Y,'f',2);

    QToolTip::showText(event->globalPos(),QString("x:"+x+","+"y:"+y),this);
}

void Histogram::generateDataToDraw(FeatureWrapper* f, QString attributeToDraw)
{
    _bins.clear();
    densityValues.clear();

    simData = f->getGeometry()->getSimulationData();
    actualFeature = f;

    if(attributeToDraw.compare("X") == 0){
        this->prepareX();
    }else if(attributeToDraw.compare("Y") == 0){
        this->prepareY();
    }else if(attributeToDraw.compare("Z") == 0){
        this->prepareZ();
    }else if(attributeToDraw.compare("I") == 0){
        this->prepareI();
    }else if(attributeToDraw.compare("J") == 0){
        this->prepareJ();
    }else if(attributeToDraw.compare("K") == 0){
        this->prepareK();
    }else if(attributeToDraw.compare("Radius") == 0){
        this->prepareRadius();
    }else if(attributeToDraw.compare("Scalar") == 0){
        this->prepareScalar();
    }
}



void Histogram::drawGrid()
{

    QPainter painter(this);
    QPen pen;
    QFont f("Times", 8, QFont::Bold);

    pen.setWidth(1);
    pen.setColor(Qt::black);
    pen.setStyle(Qt::DashLine);
    painter.setPen(pen);

    float xCenter = xLeft+(scale * expectationPoint.x());
    float yCenter = yBottom-(scale * expectationPoint.y());

    painter.drawLine(xCenter,yTop+1,
                     xCenter, yBottom-1);

    painter.drawLine(xLeft,yCenter,
                     (1*scale), yCenter);

    pen.setColor("#AAAAAA");
    pen.setStyle(Qt::DashDotLine);
    painter.setPen(pen);

    float stepsV = (resolution/10)*scale;
    float stepsH = (resolution/10)*scale;


    for(int i=1; stepsV<(resolution)*scale; i++)
    {

        float xRight = (xCenter+stepsV)/scale;
        float xLeft= (xCenter-stepsV)/scale;

        xRight = minError+(xRight/errorScale);
        xLeft = minError+(xLeft/errorScale);

        if (i%3 == 0) {

            pen.setWidth(2);
            pen.setColor(Qt::black);
            pen.setStyle(Qt::SolidLine);
            painter.setPen(pen);
            painter.setFont(f);

            QString stringRX = QString::number(xRight*UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits);
            QString stringLX = QString::number(xLeft*UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits);

            if(xRight<maxError){
                painter.drawText(xCenter+stepsV, yBottom-2, stringRX);
            }

            painter.drawText(xCenter-stepsV, yBottom-2, stringLX);

            pen.setWidth(1);
            pen.setColor(Qt::black);
            pen.setStyle(Qt::DashLine);
            painter.setPen(pen);

        }

        if(xRight<maxError){
          painter.drawLine(xCenter+stepsV,yTop+1,
                           xCenter+stepsV, yBottom-1);

        }

        painter.drawLine(xCenter-stepsV,yTop+1,
                         xCenter-stepsV, yBottom-1);





        pen.setColor("#AAAAAA");
        pen.setStyle(Qt::DashDotLine);
        painter.setPen(pen);

        stepsV = i*(resolution/10)*scale;

    }

    painter.drawLine(xLeft,yCenter,
                     (1*scale), yCenter);

    for(int i=1; stepsH<height; i++)
    {

        if (i%3 == 0) {

            pen.setWidth(2);
            pen.setColor(Qt::black);
            pen.setStyle(Qt::SolidLine);
            painter.setPen(pen);
            painter.setFont(f);

            float yDown = ((yBottom-(yCenter+stepsH))/scale);
            float yUp= ((yCenter-stepsH)/scale);

            QString stringYUp = QString::number(yUp,'f',2);
            QString stringYDown = QString::number(yDown,'f',2);

            painter.drawText(xLeft+1, yCenter+stepsH, stringYDown);
            painter.drawText(xLeft+1, yCenter-stepsH, stringYUp);

            pen.setWidth(1);
            pen.setColor(Qt::black);
            pen.setStyle(Qt::DashLine);
            painter.setPen(pen);

        }

        painter.drawLine(xLeft,yCenter+stepsH,
                         (1*scale), yCenter+stepsH);

        painter.drawLine(xLeft,yCenter-stepsH,
                         (1*scale), yCenter-stepsH);

        pen.setColor("#AAAAAA");
        pen.setStyle(Qt::DashDotLine);
        painter.setPen(pen);

        stepsH = i*(resolution/10)*scale;

    }


}


void Histogram::drawResultSet()
{
    QPainter painter(this);
    QPen pen;

    pen.setWidth(3);
    pen.setColor(Qt::black);
    painter.setPen(pen);

    QFont f("Times", 10, QFont::Bold);
    painter.setFont(f);

    QString a = QString::number(actualValue*UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits);
    QString e = QString::number(expectation*UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits);
    QString u = QString::number(uncertainty*UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits);
    QString maxV = QString::number((maxError-expectation)*UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits);
    QString minV = QString::number((minError-expectation)*UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits);

    painter.drawText((1*scale)+10, yTop+15, distribution);
    painter.drawText((1*scale)+10, yTop+30, QString("actual "+ featureAttribute +": " + a));
    painter.drawText((1*scale)+10, yTop+45, QString("expectation: " + e));
    painter.drawText((1*scale)+10, yTop+60, QString("uncertainty: " + u));
    painter.drawText((1*scale)+10, yTop+75, QString("max diff: " + maxV));
    painter.drawText((1*scale)+10, yTop+90, QString("min diff: " + minV));

}



void Histogram::generateDensityList(QList<double> tmpList)
{
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

    //resolution
    if((expectation-minError)>(maxError-expectation)){
        resolution = (errorScale*(expectation-minError));
    }else{
        resolution = (1-(errorScale*(expectation-minError)));
    }
}

void Histogram::setUpExpectationPoints(double h)
{
    actualPoint.setX(errorScale*(actualValue-minError));
    actualPoint.setY(frequencyScale*(h-minFrequency));

    expectationPoint.setX(errorScale*(expectation-minError));
    expectationPoint.setY(1);
}

void Histogram::prepareAll()
{

}

void Histogram::prepareX()
{
    featureAttribute = "X";
    actualValue = actualFeature->getGeometry()->getXYZ().getAt(0);
    distribution = simData.uncertaintyX.distribution;
    maxError = simData.uncertaintyX.maxValue;
    minError = simData.uncertaintyX.minValue;
    uncertainty = simData.uncertaintyX.uncertainty;
    expectation = simData.uncertaintyX.expectation;

    errorScale = 1/(maxError-minError);

    QList<double> tmpList =simData.uncertaintyX.values;

    this->generateDensityList(tmpList);

    //draw point
    double h = simData.uncertaintyX.densityFunction(actualValue,expectation,uncertainty);

    this->setUpExpectationPoints(h);
}

void Histogram::prepareY()
{
    featureAttribute = "Y";
    actualValue = actualFeature->getGeometry()->getXYZ().getAt(1);
    distribution = simData.uncertaintyY.distribution;
    maxError = simData.uncertaintyY.maxValue;
    minError = simData.uncertaintyY.minValue;
    uncertainty = simData.uncertaintyY.uncertainty;
    expectation = simData.uncertaintyY.expectation;

    errorScale = 1/(maxError-minError);

    QList<double> tmpList =simData.uncertaintyY.values;

    this->generateDensityList(tmpList);

    //draw point
    double h = simData.uncertaintyY.densityFunction(actualValue,expectation,uncertainty);

    this->setUpExpectationPoints(h);
}

void Histogram::prepareZ()
{
    featureAttribute = "Z";
    actualValue = actualFeature->getGeometry()->getXYZ().getAt(2);
    distribution = simData.uncertaintyZ.distribution;
    maxError = simData.uncertaintyZ.maxValue;
    minError = simData.uncertaintyZ.minValue;
    uncertainty = simData.uncertaintyZ.uncertainty;
    expectation = simData.uncertaintyZ.expectation;

    errorScale = 1/(maxError-minError);

    QList<double> tmpList =simData.uncertaintyZ.values;

    this->generateDensityList(tmpList);

    //draw point
    double h = simData.uncertaintyZ.densityFunction(actualValue,expectation,uncertainty);

    this->setUpExpectationPoints(h);
}

void Histogram::prepareI()
{
    featureAttribute = "I";
    actualValue = actualFeature->getGeometry()->getIJK().getAt(0);
    distribution = simData.uncertaintyI.distribution;
    maxError = simData.uncertaintyI.maxValue;
    minError = simData.uncertaintyI.minValue;
    uncertainty = simData.uncertaintyI.uncertainty;
    expectation = simData.uncertaintyI.expectation;

    errorScale = 1/(maxError-minError);

    QList<double> tmpList =simData.uncertaintyI.values;

    this->generateDensityList(tmpList);

    //draw point
    double h = simData.uncertaintyI.densityFunction(actualValue,expectation,uncertainty);

    this->setUpExpectationPoints(h);
}

void Histogram::prepareJ()
{
    featureAttribute = "J";
    actualValue = actualFeature->getGeometry()->getIJK().getAt(1);
    distribution = simData.uncertaintyJ.distribution;
    maxError = simData.uncertaintyJ.maxValue;
    minError = simData.uncertaintyJ.minValue;
    uncertainty = simData.uncertaintyJ.uncertainty;
    expectation = simData.uncertaintyJ.expectation;

    errorScale = 1/(maxError-minError);

    QList<double> tmpList =simData.uncertaintyJ.values;

    this->generateDensityList(tmpList);

    //draw point
    double h = simData.uncertaintyJ.densityFunction(actualValue,expectation,uncertainty);

    this->setUpExpectationPoints(h);
}

void Histogram::prepareK()
{
    featureAttribute = "K";
    actualValue = actualFeature->getGeometry()->getIJK().getAt(2);
    distribution = simData.uncertaintyK.distribution;
    maxError = simData.uncertaintyK.maxValue;
    minError = simData.uncertaintyK.minValue;
    uncertainty = simData.uncertaintyK.uncertainty;
    expectation = simData.uncertaintyK.expectation;

    errorScale = 1/(maxError-minError);

    QList<double> tmpList =simData.uncertaintyK.values;

    this->generateDensityList(tmpList);

    //draw point
    double h = simData.uncertaintyK.densityFunction(actualValue,expectation,uncertainty);

    this->setUpExpectationPoints(h);
}

void Histogram::prepareRadius()
{
    featureAttribute = "Radius";
    actualValue = 0.0;
    distribution = simData.uncertaintyRadius.distribution;
    maxError = simData.uncertaintyRadius.maxValue;
    minError = simData.uncertaintyRadius.minValue;
    uncertainty = simData.uncertaintyRadius.uncertainty;
    expectation = simData.uncertaintyRadius.expectation;

    errorScale = 1/(maxError-minError);

    QList<double> tmpList =simData.uncertaintyRadius.values;

    this->generateDensityList(tmpList);

    //draw point
    double h = simData.uncertaintyRadius.densityFunction(actualValue,expectation,uncertainty);

    this->setUpExpectationPoints(h);
}

void Histogram::prepareScalar()
{
    featureAttribute = "Scalar";
    actualValue = 0.0;
    distribution = simData.uncertaintyScalar.distribution;
    maxError = simData.uncertaintyScalar.maxValue;
    minError = simData.uncertaintyScalar.minValue;
    uncertainty = simData.uncertaintyScalar.uncertainty;
    expectation = simData.uncertaintyScalar.expectation;

    errorScale = 1/(maxError-minError);

    QList<double> tmpList =simData.uncertaintyScalar.values;

    this->generateDensityList(tmpList);

    //draw point
    double h = simData.uncertaintyScalar.densityFunction(actualValue,expectation,uncertainty);

    this->setUpExpectationPoints(h);
}
