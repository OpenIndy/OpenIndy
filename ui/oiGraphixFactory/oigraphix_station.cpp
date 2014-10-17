#include "oigraphix_station.h"

OiGraphixStation::OiGraphixStation()
{
}

void OiGraphixStation::draw(Geometry *g){

    GLfloat x = g->getXYZ().getAt(0);
    GLfloat y = g->getXYZ().getAt(1);
    GLfloat z = g->getXYZ().getAt(2);

    //noch durch zoom teilen, um immer gleichen Durchmesser zu haben
    GLfloat size = 0.02;
    GLfloat length = 1.0;

    glMatrixMode(GL_MODELVIEW);

    glPushMatrix();
        glPointSize(10.0);

        GLUquadricObj *myQuad;
        glTranslatef(x,y,z);

        //X-axis
        glPushMatrix();
            myQuad = gluNewQuadric();
            glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
            glColor3f(1.0f, 0.0f, 0.0f);

            glRotatef(90, 0.0,1.0,0.0);

            gluCylinder(myQuad, size, size, length, 32, 32);
            glTranslatef(0.0,0.0,length);
            gluCylinder(myQuad, size*1.1, 0.0, length*0.05, 32, 32);
        glPopMatrix();

        //Y-axis
        glPushMatrix();
            myQuad = gluNewQuadric();
            glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
            glColor3f(0.0f, 1.0f, 0.0);

            glRotatef(90, 1.0,0.0,0.0);

            gluCylinder(myQuad, size, size, length, 32, 32);
            glTranslatef(0.0,0.0,length);
            gluCylinder(myQuad, size*1.1, 0.0, length*0.05, 32, 32);
        glPopMatrix();

        //Z-axis
        glPushMatrix();
            myQuad = gluNewQuadric();
            glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
            glColor3f(0.0f, 0.0f, 1.0f);

            gluCylinder(myQuad, size, size, length, 32, 32);
            glTranslatef(0.0,0.0,length);
            gluCylinder(myQuad, size*1.1, 0.0, length*0.05, 32, 32);
        glPopMatrix();


}
