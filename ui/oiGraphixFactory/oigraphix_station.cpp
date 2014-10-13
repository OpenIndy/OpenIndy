#include "oigraphix_station.h"

OiGraphixStation::OiGraphixStation()
{
}

void OiGraphixStation::draw(Geometry *g){

    GLfloat x = g->getXYZ().getAt(0);
    GLfloat y = g->getXYZ().getAt(1);
    GLfloat z = g->getXYZ().getAt(2);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glTranslatef(x,y,z);

    glPointSize(10.0);
    glBegin(GL_LINES);
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex3d(0.0, 0.0, 0.0);
        glVertex3d(1.0, 0.0, 0.0);
    glEnd();
    glBegin(GL_LINES);
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3d(0.0, 0.0, 0.0);
        glVertex3d(0.0, 1.0, 0.0);
    glEnd();
    glBegin(GL_LINES);
        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex3d(0.0, 0.0, 0.0);
        glVertex3d(0.0, 0.0, 1.0);
    glEnd();

    glPopMatrix();


}
