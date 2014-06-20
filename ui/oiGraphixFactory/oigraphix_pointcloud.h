#ifndef OIGRAPHIX_POINTCLOUD_H
#define OIGRAPHIX_POINTCLOUD_H

#include "oigraphix_geometry.h"
#include "pointcloud.h"

class OiGraphixPointCloud : public OiGraphixGeometry
{
public:
    OiGraphixPointCloud(QList<Point_PC*> points);

    void draw(GLfloat x, GLfloat y, GLfloat z);

private:
    QList<Point_PC*> points;
};

#endif // OIGRAPHIX_POINTCLOUD_H
