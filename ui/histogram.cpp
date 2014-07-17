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


    double _heightMax = 1.0;

    //#################

    QRect viewPort = rect();
    int xLeft = viewPort.left();
    int xRight = viewPort.right();
    int yTop = viewPort.top();
    int yBottom = viewPort.bottom();
    int width = viewPort.width();
    int height = viewPort.height();

    QPainter painter(this);

    QPen pen;
    pen.setColor(Qt::black);
    pen.setWidth(2);
    painter.setPen(pen);

    QBrush brush(Qt::SolidPattern);
    brush.setColor("#DDDDDD");
    painter.setBrush(brush);

    // ---- Draw gray background ------------------------------------------------
    painter.drawRect(xLeft, yTop, xRight, yBottom);



    // ---- Histogram itself ----------------------------------------------------
       int nbBins = _bins.size();

       if( !nbBins )
       {
           pen.setColor("#016790");
           painter.setPen(pen);
           painter.drawText(xLeft+2, yBottom-2, tr("Histogram off"));
           return;
       }

       // Find maximum height in bins unit
       double heightMax=1.0;
       for( int i=0; i<nbBins; ++i )
           if( _bins[i]>heightMax ) heightMax = _bins[i];

       //Avoid giggling graph: do not update heightmax if variation <5%
       if( abs(_heightMax-heightMax)/float(_heightMax) > 0.05f )
           _heightMax = heightMax;

       // Scale histogram from bins unit to pixels unit
       // handle upscaling and downscaling in a different way
       QPolygon myPolygon;
       QPolygon densityGraph;
       QLinearGradient linearGradient(0, 0, 0, height);
       pen.setStyle(Qt::SolidLine);


       if( nbBins < width )
       {
           float wScale =  width/float(nbBins);
           float hScale =  height/float(_heightMax);
           float hScaleLog =  height/log(float(_heightMax));

           // log(bins)
           pen.setColor("#00aaee");   painter.setPen(pen);

           for(int i = 0;i<densityValues.size(); i++){
               densityGraph << QPoint(xLeft+wScale*i, yTop+hScale*(_heightMax-densityValues.at(i)));
           }
           //painter.drawPolygon(densityGraph);


           // bins
           pen.setColor("#016790");
           painter.setPen(pen);
           linearGradient.setColorAt(0.2, Qt::white);
           //linearGradient.setColorAt(1.0, "#016790");
           painter.setBrush(linearGradient);

           myPolygon.clear();
           myPolygon << QPoint(xRight, yBottom) << QPoint(xLeft, yBottom);
           for( int i=0; i<nbBins; ++i ){
               myPolygon << QPoint(xLeft+wScale*i, yTop+hScale*(_heightMax-_bins[i]));
           }
          // painter.drawPolygon(myPolygon);


       }
       else
       {
           float wScale =  float(nbBins-1)/(width-1);
           float hScale =  height/float(_heightMax);
           float hScaleLog =  height/log(float(_heightMax));

           // log(bins)
           pen.setColor("#00aaee");   painter.setPen(pen);
           linearGradient.setColorAt(0.2, Qt::white);
           linearGradient.setColorAt(1.0, "#00aaee");
           painter.setBrush(linearGradient);

           myPolygon.clear();
           myPolygon << QPoint(xRight, yBottom) << QPoint(xLeft, yBottom);
           for( int i=0; i<width; ++i )
               myPolygon << QPoint(xLeft+i, yTop+hScaleLog*( _bins[wScale*i] ? log(_heightMax/float(_bins[wScale*i])) : _heightMax));
           //painter.drawPolygon(myPolygon);

           // bins
           pen.setColor("#016790");
           painter.setPen(pen);
           linearGradient.setColorAt(0.2, Qt::white);
           linearGradient.setColorAt(1.0, "#016790");
           painter.setBrush(linearGradient);

           myPolygon.clear();
           myPolygon << QPoint(xRight, yBottom) << QPoint(xLeft, yBottom);
           for( int i=0; i<width; ++i )
               myPolygon << QPoint(xLeft+i, yTop+hScale*(_heightMax-_bins[wScale*i]));
           //painter.drawPolygon(myPolygon);
       }

       pen.setColor(Qt::red);
       pen.setWidth(5);
       painter.setPen(pen);
       painter.drawPoints(densityGraph);


       // ---- Draw vertical lines -------------------------------------------------
       pen.setWidth(1);
       pen.setColor("#AAAAAA");
       pen.setStyle(Qt::DashDotLine);
       painter.setPen(pen);

       int stepsV = 1<< int(log(width/40.0f)/log(2.0f));
       for(int i=1; i<stepsV; ++i)
       {
           pen.setColor("#AAAAAA");
           painter.setPen(pen);
           painter.drawLine(width*float(i)/stepsV, yTop+1,
                            width*float(i)/stepsV, yBottom-1);


           pen.setColor(Qt::black);
           painter.setPen(pen);
           painter.drawText(width*float(i)/stepsV,yBottom-2, QString::number(i));
       }

       // ---- Draw horizontal lines -----------------------------------------------
       int stepsH = 1<< int(log(height/40.0f)/log(2.0f));
       for(int i=1; i<stepsH; ++i)
       {
           pen.setColor("#AAAAAA");
           painter.setPen(pen);
           painter.drawLine(xLeft+1, height*float(i)/stepsH,
                            xRight-1,height*float(i)/stepsH);

           pen.setColor(Qt::black);
           painter.setPen(pen);
           painter.drawText(0,height*float(i)/stepsH, QString::number(maxError-i/errorScale));
       }

       // ---- Draw SimulationData-----------------------------------------------
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

void Histogram::mouseMoveEvent(QMouseEvent *event)
{

    double X = maxError-event->x()/errorScale;
    double Y = maxError-event->y()/errorScale;

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

        foreach(double d, tmpList){

            double w = simData.uncertaintyX.densityFunction(d,expectation,uncertainty);

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
