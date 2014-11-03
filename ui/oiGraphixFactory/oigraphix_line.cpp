#include "oigraphix_line.h"

OiGraphixLine::OiGraphixLine()
{

}


void OiGraphixLine::draw(Geometry *g){

    GLfloat x = g->getXYZ().getAt(0);
    GLfloat y = g->getXYZ().getAt(1);
    GLfloat z = g->getXYZ().getAt(2);

    rx = g->getIJK().getAt(0);
    ry = g->getIJK().getAt(1);
    rz = g->getIJK().getAt(2);

    GLfloat dx = g->getXYZ().getAt(0)+100*rx;
    GLfloat dy = g->getXYZ().getAt(1) +100*ry;
    GLfloat dz = g->getXYZ().getAt(2) +100*rz;



    glBegin(GL_LINES);
        glVertex3f(x, y, z);
        glVertex3f(dx, dy, dz);
    glEnd();

}
