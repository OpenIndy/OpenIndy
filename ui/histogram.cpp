#include "histogram.h"
#include <cmath>

Histogram::Histogram(QWidget *parent) :
    QWidget(parent)
{
    maxError = -numeric_limits<double>::max();
    minError = numeric_limits<double>::max();
    this->setMouseTracking(true);
    drawAll = false;

    iterationCount = 0;
}

void Histogram::paintData(FeatureWrapper* f, QString attributeToDraw)
{
    this->generateDataToDraw(f,attributeToDraw);

    this->repaint();
}

void Histogram::setTypeOfUnit(QString t)
{
  this->typeOfUnit = t;
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
    pen.setColor("#548B54");
    pen.setWidth(4);
    painter.setPen(pen);

    QPolygon densityGraph;


    for( int i=0; i<_bins.size(); i++ ){
        densityGraph << QPoint(xLeft+(scale * _bins[i]), yBottom-(scale * densityValues.at(i)));
    }

    painter.drawPolyline(densityGraph);

    //draw expectations points
    QFont f("Arial", 10, QFont::Bold);
    pen.setWidth(10);
    pen.setColor("#2F4F4F");
    painter.setPen(pen);
    painter.setFont(f);


    painter.drawPoint(QPointF(xLeft+(scale * expectationPoint.x()), yBottom-(scale * expectationPoint.y())));

    QString e = QString::number(expectation*unitMultiplier,'f',unitDigits);
    painter.drawText(xLeft+5+(scale * expectationPoint.x()), yBottom-5-(scale * expectationPoint.y()),QString("expectation: " +e));

    pen.setWidth(10);
    pen.setColor("#8B4513");
    painter.setPen(pen);
    painter.setFont(f);


    painter.drawPoint(QPointF(xLeft+(scale * actualPoint.x()), yBottom-(scale * actualPoint.y())));

    QString a = QString::number(actualValue*unitMultiplier,'f',unitDigits);
    painter.drawText(xLeft-25+(scale * actualPoint.x()), yBottom+20-(scale * actualPoint.y()),QString("actual: " +a));

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
    yValues.clear();

    simData = f->getGeometry()->getSimulationData();
    actualFeature = f;

    this->drawAll = false;

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
    }else if(attributeToDraw.compare("all") == 0){
        this->prepareAll();
    }
}



void Histogram::drawGrid()
{

    QPainter painter(this);
    QPen pen;
    QFont f("Arial", 10, QFont::Bold);

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

    QString a = QString::number(actualValue*unitMultiplier,'f',unitDigits);
    QString e = QString::number(expectation*unitMultiplier,'f',unitDigits);
    QString u = QString::number(uncertainty*unitMultiplier,'f',unitDigits);
    QString maxV = QString::number((maxError-expectation)*unitMultiplier,'f',unitDigits);
    QString minV = QString::number((minError-expectation)*unitMultiplier,'f',unitDigits);

    if(abs(actualValue-expectation)>(maxError-expectation) || abs(actualValue-expectation)>(expectation-minError)){
        pen.setColor(Qt::red);
        painter.setPen(pen);
        painter.drawText((1*scale)+10, yTop+15, QString("actual "+ featureAttribute +": " + a));
        pen.setColor(Qt::black);
        painter.setPen(pen);
    }else{
        painter.drawText((1*scale)+10, yTop+15, QString("actual "+ featureAttribute +": " + a));
    }

    painter.drawText((1*scale)+10, yTop+35, QString("expectation: " + e));
    painter.drawText((1*scale)+10, yTop+50, QString("distribution: " + distribution));
    painter.drawText((1*scale)+10, yTop+65, QString("uncertainty: " + u));
    painter.drawText((1*scale)+10, yTop+80, QString("max diff: " + maxV));
    painter.drawText((1*scale)+10, yTop+95, QString("min diff: " + minV));

}



void Histogram::generateDensityList(QList<double> tmpList)
{

    errorScale = 1/(maxError-minError);

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

void Histogram::addErrorAttribute(AttributeStats a, QList<double> v)
{
    if(maxError < a.maxError){
        maxError = a.maxError;
    }
    if(minError > a.minError){
        minError = a.minError;
    }

    errorScale = 1/(maxError-minError);

    foreach(double d, v){
        a.errors.append(d-a.expectation);
    }


    yValues.insert(a.name,a);
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
    this->drawAll = true;
        this->prepareX();
        this->prepareY();
        this->prepareZ();
        this->prepareI();
        this->prepareJ();
        this->prepareK();
        this->prepareRadius();
        this->prepareScalar();
}

void Histogram::prepareX()
{
    if(simData.uncertaintyX.values.size()==0){
        return;
    }


    if(!this->drawAll){

        featureAttribute = "X";
        actualValue = actualFeature->getGeometry()->getXYZ().getAt(0);
        distribution = simData.uncertaintyX.distribution;
        maxError = simData.uncertaintyX.maxValue;
        minError = simData.uncertaintyX.minValue;
        uncertainty = simData.uncertaintyX.uncertainty;
        expectation = simData.uncertaintyX.expectation;

        QList<double> tmpList =simData.uncertaintyX.values;

        this->generateDensityList(tmpList);

        //draw point
        double h = simData.uncertaintyX.densityFunction(actualValue,expectation,uncertainty);

        this->setUpExpectationPoints(h);

        unitMultiplier = UnitConverter::getDistanceMultiplier();
        unitDigits = UnitConverter::distanceDigits;
    }else{

        if(this->typeOfUnit == "position"){

            AttributeStats a;
            a.name = "X";
            a.expectation = simData.uncertaintyX.expectation;
            a.uncertainty = simData.uncertaintyX.uncertainty;
            a.actual = actualFeature->getGeometry()->getXYZ().getAt(0);
            a.maxError = simData.uncertaintyX.maxValue-simData.uncertaintyX.expectation;
            a.minError = simData.uncertaintyX.minValue-simData.uncertaintyX.expectation;
            a.unitMultiplier= UnitConverter::getDistanceMultiplier();
            a.unitDigits = UnitConverter::distanceDigits;

            iterationCount = simData.uncertaintyX.values.count();

            this->addErrorAttribute(a,simData.uncertaintyX.values);
        }

    }


}

void Histogram::prepareY()
{
    if(simData.uncertaintyY.values.size()==0){
        return;
    }

    featureAttribute = "Y";
    actualValue = actualFeature->getGeometry()->getXYZ().getAt(1);
    distribution = simData.uncertaintyY.distribution;
    maxError = simData.uncertaintyY.maxValue;
    minError = simData.uncertaintyY.minValue;
    uncertainty = simData.uncertaintyY.uncertainty;
    expectation = simData.uncertaintyY.expectation;

    QList<double> tmpList =simData.uncertaintyY.values;

    this->generateDensityList(tmpList);

    //draw point
    double h = simData.uncertaintyY.densityFunction(actualValue,expectation,uncertainty);

    this->setUpExpectationPoints(h);

    unitMultiplier = UnitConverter::getDistanceMultiplier();
    unitDigits = UnitConverter::distanceDigits;
}

void Histogram::prepareZ()
{
    if(simData.uncertaintyZ.values.size()==0){
        return;
    }

    featureAttribute = "Z";
    actualValue = actualFeature->getGeometry()->getXYZ().getAt(2);
    distribution = simData.uncertaintyZ.distribution;
    maxError = simData.uncertaintyZ.maxValue;
    minError = simData.uncertaintyZ.minValue;
    uncertainty = simData.uncertaintyZ.uncertainty;
    expectation = simData.uncertaintyZ.expectation;

    QList<double> tmpList =simData.uncertaintyZ.values;

    this->generateDensityList(tmpList);

    //draw point
    double h = simData.uncertaintyZ.densityFunction(actualValue,expectation,uncertainty);

    this->setUpExpectationPoints(h);

    unitMultiplier = UnitConverter::getDistanceMultiplier();
    unitDigits = UnitConverter::distanceDigits;
}

void Histogram::prepareI()
{
    if(simData.uncertaintyI.values.size()==0){
        return;
    }

    featureAttribute = "I";
    actualValue = actualFeature->getGeometry()->getIJK().getAt(0);
    distribution = simData.uncertaintyI.distribution;
    maxError = simData.uncertaintyI.maxValue;
    minError = simData.uncertaintyI.minValue;
    uncertainty = simData.uncertaintyI.uncertainty;
    expectation = simData.uncertaintyI.expectation;

    QList<double> tmpList =simData.uncertaintyI.values;

    this->generateDensityList(tmpList);

    //draw point
    double h = simData.uncertaintyI.densityFunction(actualValue,expectation,uncertainty);

    this->setUpExpectationPoints(h);

    unitMultiplier = 1.0;
    unitDigits = 8;
}

void Histogram::prepareJ()
{
    if(simData.uncertaintyJ.values.size()==0){
        return;
    }

    featureAttribute = "J";
    actualValue = actualFeature->getGeometry()->getIJK().getAt(1);
    distribution = simData.uncertaintyJ.distribution;
    maxError = simData.uncertaintyJ.maxValue;
    minError = simData.uncertaintyJ.minValue;
    uncertainty = simData.uncertaintyJ.uncertainty;
    expectation = simData.uncertaintyJ.expectation;

    QList<double> tmpList =simData.uncertaintyJ.values;

    this->generateDensityList(tmpList);

    //draw point
    double h = simData.uncertaintyJ.densityFunction(actualValue,expectation,uncertainty);

    this->setUpExpectationPoints(h);

    unitMultiplier = 1.0;
    unitDigits = 8;
}

void Histogram::prepareK()
{
    if(simData.uncertaintyK.values.size()==0){
        return;
    }

    featureAttribute = "K";
    actualValue = actualFeature->getGeometry()->getIJK().getAt(2);
    distribution = simData.uncertaintyK.distribution;
    maxError = simData.uncertaintyK.maxValue;
    minError = simData.uncertaintyK.minValue;
    uncertainty = simData.uncertaintyK.uncertainty;
    expectation = simData.uncertaintyK.expectation;

    QList<double> tmpList =simData.uncertaintyK.values;

    this->generateDensityList(tmpList);

    //draw point
    double h = simData.uncertaintyK.densityFunction(actualValue,expectation,uncertainty);

    this->setUpExpectationPoints(h);

    unitMultiplier = 1.0;
    unitDigits = 8;
}

void Histogram::prepareRadius()
{
    if(simData.uncertaintyRadius.values.size()==0){
        return;
    }

    featureAttribute = "Radius";
    actualValue = actualFeature->getGeometry()->getRadius();
    distribution = simData.uncertaintyRadius.distribution;
    maxError = simData.uncertaintyRadius.maxValue;
    minError = simData.uncertaintyRadius.minValue;
    uncertainty = simData.uncertaintyRadius.uncertainty;
    expectation = simData.uncertaintyRadius.expectation;

    QList<double> tmpList =simData.uncertaintyRadius.values;

    this->generateDensityList(tmpList);

    //draw point
    double h = simData.uncertaintyRadius.densityFunction(actualValue,expectation,uncertainty);

    this->setUpExpectationPoints(h);

    unitMultiplier = UnitConverter::getDistanceMultiplier();
    unitDigits = UnitConverter::distanceDigits;
}

void Histogram::prepareScalar()
{
    if(simData.uncertaintyScalar.values.size()==0){
        return;
    }

    featureAttribute = "Scalar";
    actualValue = actualFeature->getGeometry()->getScalar();
    distribution = simData.uncertaintyScalar.distribution;
    maxError = simData.uncertaintyScalar.maxValue;
    minError = simData.uncertaintyScalar.minValue;
    uncertainty = simData.uncertaintyScalar.uncertainty;
    expectation = simData.uncertaintyScalar.expectation;

    QList<double> tmpList =simData.uncertaintyScalar.values;

    this->generateDensityList(tmpList);

    //draw point
    double h = simData.uncertaintyScalar.densityFunction(actualValue,expectation,uncertainty);

    this->setUpExpectationPoints(h);
}
