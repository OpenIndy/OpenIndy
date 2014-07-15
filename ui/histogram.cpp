#include "histogram.h"
#include <cmath>

Histogram::Histogram(QWidget *parent) :
    QWidget(parent)
{

    this->setMouseTracking(true);
}

void Histogram::paintData(FeatureWrapper* f, QString attributeToDraw)
{

    _bins.clear();

     simData = f->getGeometry()->getSimulationData();
     QList<double> tmpList =simData.uncertaintyX.values;



    if(tmpList.size() != 0){

     maxError = simData.uncertaintyX.maxValue;
     minError = simData.uncertaintyX.minValue;

    errorScale = 1/(maxError-minError);

    foreach(double d, tmpList){

        double u = simData.uncertaintyX.uncertainty;
        double e = simData.uncertaintyX.expectation;
        double w = simData.uncertaintyX.densityFunction(d,e,u);

        densityValues.append(w);

         _bins.append(errorScale*(maxError-d));
    }


    this->repaint();
    }
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
           painter.drawPolygon(densityGraph);


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
           painter.drawPolygon(myPolygon);


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
           painter.drawPolygon(myPolygon);

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
           painter.drawPolygon(myPolygon);
       }

       pen.setColor(Qt::red);
       pen.setWidth(5);
       painter.setPen(pen);
       painter.drawPoints(myPolygon);

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
       QString expectation = QString::number(simData.uncertaintyX.expectation,'f',6);
       QString uncertainty = QString::number(simData.uncertaintyX.uncertainty,'f',6);
       QString maxV = QString::number(simData.uncertaintyX.maxValue,'f',6);
       QString minV = QString::number(simData.uncertaintyX.minValue,'f',6);

       painter.drawText(xRight-200, yTop+10, simData.uncertaintyX.distribution);
       painter.drawText(xRight-200, yTop+20, QString("expectation: " + expectation));
       painter.drawText(xRight-200, yTop+30, QString("uncertainty: " + uncertainty));
       painter.drawText(xRight-200, yTop+40, QString("max: " + maxV));
       painter.drawText(xRight-200, yTop+50, QString("min: " + minV));
}

void Histogram::mouseMoveEvent(QMouseEvent *event)
{

    double X = maxError-event->x()/errorScale;
    double Y = maxError-event->y()/errorScale;

    QString x = QString::number(X);
    QString y = QString::number(Y);

    QToolTip::showText(event->globalPos(),QString("x:"+x+","+"y:"+y),this);
}
