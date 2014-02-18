#include "oigraphix_plane.h"

OiGraphixPlane::OiGraphixPlane(GLfloat nx,GLfloat ny, GLfloat nz)
{

    rx = nx;
    ry = ny;
    rz = nz;

}

void OiGraphixPlane::draw(GLfloat x, GLfloat y, GLfloat z){

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glTranslatef(x,y,z);

    glRotatef(acos(rx) * 180.0/PI,1,0,0);
    glRotatef(acos(rz) * 180.0/PI,0,1,0);
    glRotatef(acos(ry) * 180.0/PI,0,0,1);

    glBegin(GL_LINES);
    for (GLfloat i = -2.5; i <= 2.5; i += 0.25) {
      glVertex3f(i, 0, 2.5); glVertex3f(i, 0, -2.5);
      glVertex3f(2.5, 0, i); glVertex3f(-2.5, 0, i);
    }
    glEnd();

    glPopMatrix();

}
