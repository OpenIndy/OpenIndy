#include "oigraphix_line.h"

OiGraphixLine::OiGraphixLine(GLfloat i, GLfloat j, GLfloat k)
{
    rx = i;
    ry = j;
    rz = k;
}


void OiGraphixLine::draw(GLfloat x, GLfloat y, GLfloat z){

    GLfloat dx = x+100*rx;
    GLfloat dy = y +100*ry;
    GLfloat dz = z +100*rz;

    glBegin(GL_LINES);
        glVertex3f(x, y, z);
        glVertex3f(dx, dy, dz);
    glEnd();

}
