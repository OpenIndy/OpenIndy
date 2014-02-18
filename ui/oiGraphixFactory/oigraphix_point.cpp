#include "oigraphix_point.h"

OiGraphixPoint::OiGraphixPoint()
{
}

void OiGraphixPoint::draw(GLfloat x, GLfloat y, GLfloat z){

    glBegin(GL_POINTS);

        glVertex3d(x, y, z);

    glEnd();

}
