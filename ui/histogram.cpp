#include "histogram.h"
#include <cmath>

Histogram::Histogram(QWidget *parent) :
    QWidget(parent)
{

    this->setMouseTracking(true);
}

void Histogram::paintData(FeatureWrapper* f, QString attributeToDraw)
{

    //_bins.clear();

    maxError = f->getGeometry()->getStatistic().simulationData.xyz.at(0).getAt(0);
    minError = f->getGeometry()->getStatistic().simulationData.xyz.at(0).getAt(0);

    foreach(OiVec v, f->getGeometry()->getStatistic().simulationData.xyz){
       if(v.getAt(0) > maxError){
           maxError = v.getAt(0);
       }
       if(v.getAt(0) < minError){
           minError = v.getAt(0);
       }
    }

    errorScale = 1/(maxError-minError);

    foreach(OiVec v, f->getGeometry()->getStatistic().simulationData.xyz){
         _bins.append(errorScale*(maxError-v.getAt(0)));
    }


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
       QLinearGradient linearGradient(0, 0, 0, height);
       pen.setStyle(Qt::SolidLine);

       if( nbBins < width )
       {
           float wScale =  width/float(nbBins);
           float hScale =  height/float(_heightMax);
           float hScaleLog =  height/log(float(_heightMax));

           // log(bins)
           pen.setColor("#00aaee");   painter.setPen(pen);
           //linearGradient.setColorAt(0.2, Qt::white);
           //linearGradient.setColorAt(1.0, "#00aaee");
           //painter.setBrush(linearGradient);


           //brush.setColor("#00aaee"); painter.setBrush(brush);

           /*myPolygon.clear();
           myPolygon << QPoint(xRight, yBottom) << QPoint(xLeft, yBottom);
           for( int i=0; i<nbBins; ++i )
               myPolygon << QPoint(xLeft+wScale*i, yTop+hScaleLog*( _bins[i] ? log(_heightMax/float(_bins[i])) : _heightMax));
           painter.drawPolygon(myPolygon);*/

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
           painter.drawText(0,height*float(i)/stepsH, QString::number(i));
       }
}

void Histogram::mouseMoveEvent(QMouseEvent *event)
{

    QString x = QString::number(event->x());
    QString y = QString::number(event->y());

    QToolTip::showText(event->globalPos(),QString("x:"+x+","+"y:"+y),this);
}
