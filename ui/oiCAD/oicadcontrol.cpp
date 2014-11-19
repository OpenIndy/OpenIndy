#include "oicadcontrol.h"

OiCadControl::OiCadControl(QObject *parent) :
    QObject(parent)
{

}

QList<Handle(AIS_InteractiveObject)> OiCadControl::getFeaturesToDraw()
{
    QList<Handle(AIS_InteractiveObject)> f;

    QList<FeatureWrapper*> allGeometries= OiFeatureState::getGeometries();

    foreach(FeatureWrapper* g,allGeometries){
       switch(g->getTypeOfFeature()){
           case Configuration::ePlaneFeature:
           f.append(this->parseOiPlane(g->getPlane()));

           break;
           case Configuration::ePointFeature:
            f.append(this->parseOiPoint(g->getPoint()));
           break;
           case Configuration::eLineFeature:

           break;
           case Configuration::eStationFeature:

           break;
           case Configuration::eSphereFeature:
            f.append(this->parseOiSphere(g->getSphere()));
           break;
           case Configuration::ePointCloudFeature:


           break;
           default:{
           break;
           }
       }
    }

    return f;
}

Handle(AIS_InteractiveObject) OiCadControl::parseOiPoint(Point *p)
{
    double x = p->getXYZ().getAt(0);
    double y = p->getXYZ().getAt(1);
    double z = p->getXYZ().getAt(2);

    gp_Pnt point(x,y,z);

    Handle(AIS_InteractiveObject) aAisPnt = new AIS_Point(new Geom_CartesianPoint(point));
    aAisPnt->SetColor(Quantity_NOC_RED);

    return aAisPnt;

}


Handle(AIS_InteractiveObject) OiCadControl::parseOiSphere(Sphere *s){

    double x = s->getXYZ().getAt(0);
    double y = s->getXYZ().getAt(1);
    double z = s->getXYZ().getAt(2);

    gp_Pnt point(x,y,z);

    TopoDS_Shape S = BRepPrimAPI_MakeSphere(point,s->getRadius());

    Handle(AIS_InteractiveObject) aAisSp = new AIS_Shape(S);

    aAisSp->SetDisplayMode(1);

    return aAisSp;
}

Handle(AIS_InteractiveObject) OiCadControl::parseOiPlane(Plane *p){

    double x = p->getXYZ().getAt(0);
    double y = p->getXYZ().getAt(1);
    double z = p->getXYZ().getAt(2);

    double i = p->getIJK().getAt(0);
    double j = p->getIJK().getAt(1);
    double k = p->getIJK().getAt(2);


    gp_Pnt point(x,y,z);
    gp_Dir direction(i,j,k);

    gp_Pln plane(point,direction);

    Handle (Geom_Plane) gPln = new Geom_Plane(plane);

    Handle(AIS_InteractiveObject) aAisPln = new AIS_Plane(gPln);

    aAisPln->SetDisplayMode(1);

    //----------------------------

    //----------------------------

    return aAisPln;

}

