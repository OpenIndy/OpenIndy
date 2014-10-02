#include "oigraphix.h"


double OiGraphix::scaleOfResidual = 10.0;

OiGraphix::OiGraphix()
{
}

void OiGraphix::drawFeature(FeatureWrapper* feature){

    switch(feature->getTypeOfFeature()){
    case Configuration::eCoordinateSystemFeature:{
        break;
    }
    case Configuration::eTrafoParamFeature:{
        break;
    }
    case Configuration::ePlaneFeature:
        OiGraphix::drawPlane(feature->getPlane());
        break;
    case Configuration::ePointFeature:
        OiGraphix::drawPoint(feature->getPoint());
        break;
    case Configuration::eLineFeature:
        OiGraphix::drawLine(feature->getLine());
        break;
    case Configuration::eStationFeature:
        OiGraphix::drawStation(feature->getStation());
        break;
    case Configuration::eSphereFeature:
        OiGraphix::drawSphere(feature->getSphere());
        break;
    case Configuration::ePointCloudFeature:
        OiGraphix::drawPointCloud(feature->getPointCloud());
        break;
    default:{
        break;
    }
    }

}

void OiGraphix::drawPoint(Point *p){
    OiGraphixPoint point;

    point.draw(p->xyz.getAt(0),p->xyz.getAt(1),p->xyz.getAt(2));

    drawResiduals();

}

void OiGraphix::drawLine(Line* l){

    OiGraphixLine line(l->ijk.getAt(0),l->ijk.getAt(1),l->ijk.getAt(2));

    line.draw(l->xyz.getAt(0),l->xyz.getAt(1),l->xyz.getAt(2));

    drawResiduals();

}

void OiGraphix::drawPlane(Plane* p){

    OiGraphixPlane plane(p->ijk.getAt(0),p->ijk.getAt(1),p->ijk.getAt(2));

    plane.draw(p->xyz.getAt(0),p->xyz.getAt(1),p->xyz.getAt(2));

    drawResiduals();

}

void OiGraphix::drawSphere(Sphere* s){
    OiGraphixSphere sphere(s->radius,24,24);
    sphere.draw((GLfloat)s->xyz.getAt(0),(GLfloat)s->xyz.getAt(1),(GLfloat)s->xyz.getAt(2));

    //TODO Verbesserungen darstellen
    drawResiduals();
}

void OiGraphix::drawStation(Station* s){
    OiGraphixStation station;

    station.draw(s->position->xyz.getAt(0),s->position->xyz.getAt(1),s->position->xyz.getAt(2));

    drawResiduals();
}

/*!
 * \brief OiGraphix::drawPointCloud
 * \param p
 */
void OiGraphix::drawPointCloud(PointCloud *p){
    OiGraphixPointCloud pointCloud(p->getPointCloudPoints());

    pointCloud.draw(p->getMainFocus().getAt(0), p->getMainFocus().getAt(1), p->getMainFocus().getAt(2));

    drawResiduals();
}

void OiGraphix::drawResiduals(/*verbesserungen?statsticobj?*/){

}
