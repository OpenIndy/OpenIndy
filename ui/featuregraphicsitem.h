#ifndef FEATUREGRAPHICSITEM_H
#define FEATUREGRAPHICSITEM_H

#include <QGraphicsItem>
#include <QPainter>
#include <QMessageBox>
#include "featurewrapper.h"

class FeatureGraphicsItem : public QGraphicsItem
{
public:
    FeatureGraphicsItem(FeatureWrapper* f,qreal pos_x, qreal pos_y);

    FeatureWrapper *featureToDraw;


    QRectF boundingRect() const;
    void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);

    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent * event);
};

#endif // FEATUREGRAPHICSITEM_H
