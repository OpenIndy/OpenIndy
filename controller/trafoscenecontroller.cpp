#include "trafoscenecontroller.h"

TrafoSceneController::TrafoSceneController(QObject *parent) :
    QObject(parent)
{

    this->trafoModel = new QGraphicsScene();
    trafoModel->addText("transformation parameter");


}

void TrafoSceneController::refreshScene()
{
   this->drawCoordinatesystems();;
   this->drawTransformationParams();
}

void TrafoSceneController::drawTransformationParams()
{

    int x = 0;
    int y = 0;
    foreach(FeatureWrapper *f, *this->trafoParams){
        if(f->getTrafoParam() != NULL){
        QGraphicsItem *item = new FeatureGraphicsItem(f,x,y);
        trafoModel->addItem(item);
        trafoModel->addText(f->getTrafoParam()->name);
        }
    }


}

void TrafoSceneController::drawCoordinatesystems()
{
    int x = 0;
    int y = 0;
    foreach(CoordinateSystem *c, *this->coordSystems){
        FeatureWrapper *f = new FeatureWrapper();
        f->setCoordinateSystem(c);
        QGraphicsItem *item = new FeatureGraphicsItem(f,x,y);
        trafoModel->addItem(item);
        trafoModel->addText(c->name);
        x += 10;
        y += 10;
    }

    foreach(Station *s, *this->stations){
        FeatureWrapper *f = new FeatureWrapper();
        f->setStation(s);
        QGraphicsItem *item = new FeatureGraphicsItem(f,x,y);
        trafoModel->addItem(item);
        trafoModel->addText(s->name);
        x += 10;
        y += 10;
    }

}
