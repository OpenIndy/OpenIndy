#include "oigraphix_pointcloud.h"

OiGraphixPointCloud::OiGraphixPointCloud(QList<Point_PC*> points){
    this->points = points;
}

void OiGraphixPointCloud::draw(GLfloat x, GLfloat y, GLfloat z){

    GLfloat psize = 0.001;
    glPointSize(psize);

    glBegin(GL_POINTS);


    foreach(Point_PC *poi, this->points){

        glVertex3d(poi->xyz[0], poi->xyz[1], poi->xyz[2]);

    }

    glEnd();

}
