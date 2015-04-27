#ifndef P_POINTCLOUDSEGMENTATION_H
#define P_POINTCLOUDSEGMENTATION_H

#include <QtGlobal>
#include <QDateTime>
#include <QObject>
#include <QDebug>
#include <QThread>
#include <QString>
#include <QMap>
#include <QStringList>

#include "generatefeaturefunction.h"

#include "plane.h"
#include "sphere.h"
#include "cylinder.h"

#include "pluginmetadata.h"
#include "oivec.h"
#include "oimat.h"

#include "ps_pointcloud.h"
#include "ps_planesegment.h"
#include "ps_spheresegment.h"
#include "ps_cylindersegment.h"

#include "ps_loadingdialog.h"

class SegmentationProducer : public QObject{
    Q_OBJECT
    QThread workerThread;



public slots:
    void startSegmentation(PS_PointCloud myCloud, PS_InputParameter param){

        connect(&myCloud, SIGNAL(updateStatus(QString,int)), this, SLOT(updateStatus(QString,int)));

        myCloud.setUpOctree(param);
        myCloud.detectShapes(param);

        int i = 1;

        foreach(PS_PlaneSegment *p, myCloud.getDetectedPlanes()){

            FeatureWrapper *myFeature = new FeatureWrapper();
            Plane *myPlane = new Plane(true);
            myPlane->setIsSolved(true);
            myPlane->setFeatureName(QString("plane_%1").arg(i));
            OiVec xyz(3);
            OiVec ijk(3);
            xyz.setAt(0, p->getDistance() * p->getIJK()[0]);
            xyz.setAt(1, p->getDistance() * p->getIJK()[1]);
            xyz.setAt(2, p->getDistance() * p->getIJK()[2]);
            ijk.setAt(0, p->getIJK()[0]);
            ijk.setAt(1, p->getIJK()[1]);
            ijk.setAt(2, p->getIJK()[2]);
            double checkA, checkB;
            OiVec::dot(checkA, xyz, ijk);
            OiVec xyz2 = -1.0 * xyz;
            OiVec::dot(checkB, xyz2, ijk);
            if(qAbs(checkA - p->getDistance()) > qAbs(checkB - p->getDistance())){
                xyz = -1.0 * xyz;
            }
            xyz.add(1.0);
            ijk.add(1.0);
            myPlane->xyz = xyz;
            myPlane->ijk = ijk;
            myFeature->setPlane(myPlane);
            emit this->addFeature(myFeature);

            i++;

        }

        i = 1;

        foreach(PS_SphereSegment *s, myCloud.getDetectedSpheres()){

            FeatureWrapper *myFeature = new FeatureWrapper();
            Sphere *mySphere = new Sphere(true);
            mySphere->setIsSolved(true);
            mySphere->setFeatureName(QString("sphere_%1").arg(i));
            OiVec xyz(3);
            double r;
            xyz.setAt(0, s->getXYZ()[0]);
            xyz.setAt(1, s->getXYZ()[1]);
            xyz.setAt(2, s->getXYZ()[2]);
            r = s->getRadius();
            xyz.add(1.0);
            mySphere->xyz = xyz;
            mySphere->radius = r;
            myFeature->setSphere(mySphere);
            emit this->addFeature(myFeature);

            i++;

        }

        i = 1;

        foreach(PS_CylinderSegment *c, myCloud.getDetectedCylinders()){

            FeatureWrapper *myFeature = new FeatureWrapper();
            Cylinder *myCylinder = new Cylinder(true);
            myCylinder->setIsSolved(true);
            myCylinder->setFeatureName(QString("cylinder_%1").arg(i));
            OiVec xyz(3);
            OiVec ijk(3);
            double r;
            float h[3];
            c->getX0(h);
            xyz.setAt(0, h[0]);
            xyz.setAt(1, h[1]);
            xyz.setAt(2, h[2]);
            c->getIJK(h);
            ijk.setAt(0, h[0]);
            ijk.setAt(1, h[1]);
            ijk.setAt(2, h[2]);
            r = c->getRadius();
            xyz.add(1.0);
            ijk.add(1.0);
            myCylinder->xyz = xyz;
            myCylinder->ijk = ijk;
            myCylinder->radius = r;
            myFeature->setCylinder(myCylinder);
            emit this->addFeature(myFeature);

            i++;

        }

        disconnect(&myCloud, SIGNAL(updateStatus(QString,int)), this, SLOT(updateStatus(QString,int)));



        emit this->stopThread();

    }

    void updateStatus(QString msg, int status){
        emit this->changeStatus(msg, status);
    }

signals:
    void stopThread();
    void changeStatus(QString, int);
    void addFeature(FeatureWrapper*);
};

class SegmentationConsumer : public QObject{
    Q_OBJECT
    QThread workerThread;

    PS_LoadingDialog myDialog;

    PointCloud &myPointCloud;

public:
    SegmentationConsumer(PointCloud &myCloud, QObject *parent = NULL) : QObject(parent), myPointCloud(myCloud){
        qRegisterMetaType<PS_PointCloud>("PS_PointCloud");
        qRegisterMetaType<PS_InputParameter>("PS_InputParameter");
        this->mySegmenter = new SegmentationProducer();
        this->mySegmenter->moveToThread(&workerThread);
        connect(this, SIGNAL(startSegmentation(PS_PointCloud,PS_InputParameter)),
                mySegmenter, SLOT(startSegmentation(PS_PointCloud,PS_InputParameter)));
        connect(mySegmenter, SIGNAL(stopThread()), this, SLOT(stopThread()));
        connect(mySegmenter, SIGNAL(changeStatus(QString,int)), &this->myDialog, SLOT(setStatus(QString,int)));
        connect(mySegmenter, SIGNAL(addFeature(FeatureWrapper*)), this, SLOT(addFeature(FeatureWrapper*)));

        workerThread.start();

        myDialog.reset();
        myDialog.show();
    }
    /*SegmentationConsumer(const SegmentationConsumer &copy){
        //this->workerThread = copy.workerThread;
        this->mySegmenter = copy.mySegmenter;
    }*/

    void startSegmentationTask(PS_PointCloud myCloud, PS_InputParameter param){

        emit this->startSegmentation(myCloud, param);
    }

signals:
    void startSegmentation(PS_PointCloud myCloud, PS_InputParameter param);

public slots:
    void stopThread(){

        workerThread.quit();
        workerThread.wait();

        myDialog.close();

        delete this->mySegmenter;
        delete this;
    }

    void addFeature(FeatureWrapper *myFeature){
        myPointCloud.addSegment(myFeature);
    }

private:
    SegmentationProducer *mySegmenter;


};

class PointCloudSegmentation : public GenerateFeatureFunction
{

protected:

    //##############################
    //function initialization method
    //##############################

    virtual void init();

    //############
    //exec methods
    //############

    bool exec(PointCloud &pointCloud);

private:
    SegmentationConsumer *myHandler;

};

#endif // P_POINTCLOUDSEGMENTATION_H
