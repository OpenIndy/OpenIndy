#include "trafoscenecontroller.h"

TrafoSceneController::TrafoSceneController(QObject *parent) :
    QObject(parent)
{

    this->trafoModel = new QGraphicsScene();
    this->trafoOverview = new QGraphicsScene();

    connect(this->trafoOverview,SIGNAL(selectionChanged()),this,SLOT(moveSelectionToModel()));

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
        trafoOverview->addItem(item);
        trafoOverview->addText(f->getTrafoParam()->name);
        }
    }


}

void TrafoSceneController::drawCoordinatesystems()
{
    qreal x = 10.0;
    qreal y = 10.0;
    foreach(CoordinateSystem *c, *this->coordSystems){
        FeatureWrapper *f = new FeatureWrapper();
        f->setCoordinateSystem(c);
        QGraphicsItem *item = new FeatureGraphicsItem(f,x,y);
        trafoOverview->addItem(item);
        y += item->boundingRect().height()+10.0;

    }

    foreach(Station *s, *this->stations){
        FeatureWrapper *f = new FeatureWrapper();
        f->setStation(s);
        QGraphicsItem *item = new FeatureGraphicsItem(f,x,y);
        trafoOverview->addItem(item);
        y += item->boundingRect().height()+10.0;
    }

}



void TrafoSceneController::moveSelectionToModel()
{

    QList<QGraphicsItem*> items = this->trafoOverview->selectedItems();

    foreach(QGraphicsItem* i,items){
        i->setFlag(QGraphicsItem::ItemIsSelectable,false);
        this->trafoModel->addItem(i);
    }

}
