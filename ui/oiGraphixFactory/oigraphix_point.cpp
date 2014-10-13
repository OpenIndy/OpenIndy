#include "oigraphix_point.h"

OiGraphixPoint::OiGraphixPoint()
{
}

void OiGraphixPoint::draw(Geometry *g){

    GLfloat x = g->getXYZ().getAt(0);
    GLfloat y = g->getXYZ().getAt(1);
    GLfloat z = g->getXYZ().getAt(2);

    glBegin(GL_POINTS);

        glVertex3d(x, y, z);

    glEnd();

}
