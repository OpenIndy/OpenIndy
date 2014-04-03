#include "featuregraphicsitem.h"

FeatureGraphicsItem::FeatureGraphicsItem(FeatureWrapper* f, int x, int y)
{
    this->featureToDraw = f;

    pos_y = y;
    pos_x = x;

    setFlag(ItemIsMovable);
}

QRectF FeatureGraphicsItem::boundingRect() const
{
    return QRectF(0,0,100,100);
}

void FeatureGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{

    QRectF rec = boundingRect();
    QBrush brush(Qt::blue);
    painter->setBrush(brush);

    if(featureToDraw->getTypeOfFeature() == Configuration::eStationFeature){
        brush.setColor(Qt::yellow);
        painter->drawEllipse(rec);
    }else if(featureToDraw->getTypeOfFeature() == Configuration::eCoordinateSystemFeature){
        brush.setColor(Qt::green);
        painter->drawEllipse(rec);
    }else if(featureToDraw->getTypeOfFeature() == Configuration::eTrafoParamFeature){
        brush.setColor(Qt::gray);
        painter->drawRect(rec);
    }

}
