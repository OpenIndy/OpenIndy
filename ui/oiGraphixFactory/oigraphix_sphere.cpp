#include "oigraphix_sphere.h"

OiGraphixSphere::OiGraphixSphere()
    {


    }

void OiGraphixSphere::draw(Geometry *g)
{

    rx = g->getIJK().getAt(0);
    ry = g->getIJK().getAt(1);
    rz = g->getIJK().getAt(2);



    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    glTranslatef(rx,ry,rz);

    GLUquadric *quadratic = gluNewQuadric();
    gluSphere(quadratic, g->getRadius(), 32, 32);

    glPopMatrix();
}
