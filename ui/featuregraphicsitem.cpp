#include "featuregraphicsitem.h"

FeatureGraphicsItem::FeatureGraphicsItem(FeatureWrapper* f, qreal pos_x, qreal pos_y)
{
    this->featureToDraw = f;

    this->setX(pos_x);
    this->setY(pos_y);

    setFlag(ItemIsMovable);
    setFlag(ItemIsSelectable);
}

QRectF FeatureGraphicsItem::boundingRect() const
{
    return QRectF(0,0,100,100);
}

void FeatureGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{

    QRectF rec = boundingRect();
    QBrush brush(Qt::lightGray);
    painter->setBrush(brush);

    if(featureToDraw->getTypeOfFeature() == Configuration::eStationFeature){
        painter->drawEllipse(rec);
    }else if(featureToDraw->getTypeOfFeature() == Configuration::eCoordinateSystemFeature){
        painter->drawEllipse(rec);
    }else if(featureToDraw->getTypeOfFeature() == Configuration::eTrafoParamFeature){
        painter->drawRect(rec);
    }

    painter->drawText(rec,Qt::AlignCenter,featureToDraw->getFeature()->name);

}

void FeatureGraphicsItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent * event)
{

    QMessageBox::information(NULL,"feature info",featureToDraw->getFeature()->name);
}
