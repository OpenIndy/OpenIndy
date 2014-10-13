#include "oigraphix.h"


double OiGraphix::scaleOfResidual = 10.0;

OiGraphix::OiGraphix()
{
}

/*!
 * \brief OiGraphix::drawFeature
 * \param feature
 */
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

/*!
 * \brief OiGraphix::drawPoint
 * \param p
 */
void OiGraphix::drawPoint(Point *p){
    OiGraphixPoint point;

    point.draw(p);

    drawResiduals();

}

void OiGraphix::drawLine(Line* l){

    OiGraphixLine line;

    line.draw(l);

    drawResiduals();

}

void OiGraphix::drawPlane(Plane* p){

    OiGraphixPlane plane;

    plane.draw(p);

}

void OiGraphix::drawSphere(Sphere* s){
    OiGraphixSphere sphere;
    sphere.draw(s);

    //TODO Verbesserungen darstellen
    drawResiduals();
}

void OiGraphix::drawStation(Station* s){
    OiGraphixStation station;

    station.draw(s->position);


    drawResiduals();
}

/*!
 * \brief OiGraphix::drawPointCloud
 * \param p
 */
void OiGraphix::drawPointCloud(PointCloud *p){

    OiGraphixPointCloud pointCloud(p->getPointCloudPoints());

    pointCloud.draw(p);

    drawResiduals();
}

void OiGraphix::drawResiduals(/*verbesserungen?statsticobj?*/){

}
