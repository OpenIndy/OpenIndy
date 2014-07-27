#ifndef POINTCLOUD_H
#define POINTCLOUD_H

#include "geometry.h"
#include "oivec.h"

struct Point_PC{
    Point_PC(){ this->isUsed = false; }

    float xyz[3];
    bool isUsed;
};

struct BoundingBox_PC{
    float min[3];
    float max[3];
};

class PointCloud : public Geometry
{
    Q_OBJECT
public:
    PointCloud(bool isNominal, QObject *parent = 0);
    PointCloud(const PointCloud &copy);
    ~PointCloud();

    OiVec xyz; //centroid of pointcloud
    QList<Point_PC*> myPoints; //all points of the pointcloud
    BoundingBox_PC bbox; //bounding box of the pointcloud
    unsigned long pointCount; //number of pointcloud points

    void recalc();

    bool toOpenIndyXML(QXmlStreamWriter& stream);
    ElementDependencies fromOpenIndyXML(QXmlStreamReader& xml);

    bool saveSimulationData();

    QString getDisplayX() const;
    QString getDisplayY() const;
    QString getDisplayZ() const;
};

#endif // POINTCLOUD_H
