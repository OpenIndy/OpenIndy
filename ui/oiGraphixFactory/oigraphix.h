#ifndef OIGRAPHIX_H
#define OIGRAPHIX_H

#include "featurewrapper.h"
#include "oigraphix_sphere.h"
#include "oigraphix_point.h"
#include "oigraphix_line.h"
#include "oigraphix_plane.h"
#include "oigraphix_station.h"

class OiGraphix
{
public:
    OiGraphix();

    static double scaleOfResidual;

    static void drawFeature(FeatureWrapper* feature);

private:

    static void drawPoint(Point* p);
    static void drawLine(Line* l);
    static void drawPlane(Plane* p);
    static void drawSphere(Sphere* s);
    static void drawStation(Station* s);

    static void drawResiduals(/*verbesserungen?statsticobj?*/);
};

#endif // OIGRAPHIX_H
