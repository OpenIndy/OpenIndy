#ifndef FEATUREGRAPHICSITEM_H
#define FEATUREGRAPHICSITEM_H

#include <QGraphicsItem>
#include <QPainter>
#include "featurewrapper.h"

class FeatureGraphicsItem : public QGraphicsItem
{
public:
    FeatureGraphicsItem(FeatureWrapper* f,int x, int y);

    FeatureWrapper *featureToDraw;
    int pos_x;
    int pos_y;

    QRectF boundingRect() const;
    void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);
};

#endif // FEATUREGRAPHICSITEM_H
