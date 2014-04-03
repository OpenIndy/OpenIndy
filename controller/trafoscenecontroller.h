#ifndef TRAFOSCENECONTROLLER_H
#define TRAFOSCENECONTROLLER_H

#include <QObject>
#include <QGraphicsScene>

#include "trafoparam.h"
#include "coordinatesystem.h"
#include "station.h"
#include "featurewrapper.h"
#include "featuregraphicsitem.h"

class TrafoSceneController : public QObject
{
    Q_OBJECT
public:
    explicit TrafoSceneController(QObject *parent = 0);
    QGraphicsScene *trafoModel;
    QList<FeatureWrapper*> *trafoParams;
    QList<CoordinateSystem*> *coordSystems;
    QList<Station*> *stations;



signals:

public slots:
    void refreshScene();


private slots:
    void drawTransformationParams();
    void drawCoordinatesystems();

};

#endif // TRAFOSCENECONTROLLER_H
