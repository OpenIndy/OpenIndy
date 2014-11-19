#ifndef OICADCONTROL_H
#define OICADCONTROL_H

#include <QObject>
#include <QList>
#include <AIS_Shape.hxx>
#include <featurewrapper.h>
#include "oifeaturestate.h"

#include <gp_Pnt.hxx>
#include <gp_Pln.hxx>
#include <Geom_Plane.hxx>

#include <TopoDS_Shape.hxx>

#include <BRepPrimAPI_MakeSphere.hxx>


#include <AIS_InteractiveObject.hxx>
#include <AIS_Point.hxx>
#include <AIS_Plane.hxx>
#include <AIS_Shape.hxx>


#include <Geom_CartesianPoint.hxx>

class OiCadControl : public QObject
{
    Q_OBJECT
public:
    explicit OiCadControl(QObject *parent = 0);

signals:

public slots:
    QList<Handle_AIS_InteractiveObject> getFeaturesToDraw();

private:
    Handle_AIS_InteractiveObject parseOiPoint(Point *p);
    Handle_AIS_InteractiveObject parseOiSphere(Sphere *s);
    Handle_AIS_InteractiveObject parseOiPlane(Plane *p);


};

#endif // OICADCONTROL_H
