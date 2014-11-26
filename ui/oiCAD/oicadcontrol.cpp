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
       if (g->getGeometry()->getIsSolved()){

       switch(g->getTypeOfFeature()){

           case Configuration::ePlaneFeature:
           f.append(this->parseOiPlane(g->getPlane()));

           break;
           case Configuration::ePointFeature:
           f.append(this->parseOiPoint(g->getPoint()));
           break;
           case Configuration::eLineFeature:
                f.append(this->parseOiLine(g->getLine()));
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

    AIS_Plane *a = new AIS_Plane(new Geom_Plane(plane));

    a->SetSize(5);

    Handle (AIS_InteractiveObject) aAisPln = a;

    return aAisPln;


}

Handle_AIS_InteractiveObject OiCadControl::parseOiLine(Line *l){

    double x = l->getXYZ().getAt(0);
    double y = l->getXYZ().getAt(1);
    double z = l->getXYZ().getAt(2);

    double i = l->getIJK().getAt(0);
    double j = l->getIJK().getAt(1);
    double k = l->getIJK().getAt(2);


    gp_Pnt point(x,y,z);
    gp_Dir direction(i,j,k);


    Handle (Geom_Line) gLn = new Geom_Line(point, direction);

    Handle(AIS_InteractiveObject) aAisLn = new AIS_Line(gLn);

    aAisLn->SetDisplayMode(1);

    return aAisLn;




}
