#ifndef POINTCLOUD_H
#define POINTCLOUD_H

#include "geometry.h"
#include "oivec.h"

class PointCloud : public Geometry
{
public:
    PointCloud();
    PointCloud(const PointCloud &copy);
    ~PointCloud();

    OiVec xyz;

    void recalc();

    QString getX();
    QString getY();
    QString getZ();
    QString getIsCommon();
    QString getIsNominal();
};

#endif // POINTCLOUD_H
