#include "oiexchangepts.h"

oiExchangePTS::oiExchangePTS(QObject *parent) : oiExchangeInterface(parent)
{
}

bool oiExchangePTS::importOiData(oiExchangeObject &data)
{
    switch (data.typeOfElement) {
    case (Configuration::ePointCloudElement):{
        return this->importPointCloud(data);
        break;
    }
    default:
        break;
    }
}

bool oiExchangePTS::exportOiData(oiExchangeObject &data)
{
    return false;
}

QList<Configuration::ElementTypes> oiExchangePTS::getSupportedElements()
{
        QList<Configuration::ElementTypes> elements;

       elements.append(Configuration::ePointCloudElement);

       return elements;
}

QStringList oiExchangePTS::getElementDescription(Configuration::ElementTypes t)
{

    QStringList description;

    switch (t) {
    case Configuration::ePointCloudElement:
        description.append("imports  a pointcloud according to the pts scheme:");
        return description;
        break;
    default:
        return description;
        break;
    }


}

QList<UnitConverter::unitType> oiExchangePTS::getSupportedDistanceUnits()
{
    QList<UnitConverter::unitType> d;

    d.append(UnitConverter::eMETER);
    d.append(UnitConverter::eMILLIMETER);

    return d;
}

QList<UnitConverter::unitType> oiExchangePTS::getSupportedAngleUnits()
{
    QList<UnitConverter::unitType> a;
    return a;
}

QList<UnitConverter::unitType> oiExchangePTS::getSupportedTemperatureUnits()
{
    QList<UnitConverter::unitType> t;
    return t;
}

/*!
 * \brief oiExchangePTS::importPointCloud
 * \param data
 * \return
 */
bool oiExchangePTS::importPointCloud(oiExchangeObject &data){

    clock_t c1 = clock();

    if (!data.device->open(QIODevice::ReadOnly | QIODevice::Text)){
        return false;
    }

    try{

        qint64 fileSize = data.device->size();
        qint64 readSize = 0;

        QTextStream in(data.device);

        PointCloud *pc = new PointCloud(true);

        double x=0.0, y=0.0, z=0.0; //centroid of pointcloud
        unsigned long pointCount = 0; //number of points in the pointcloud
        BoundingBox_PC bbox; //pointclouds's bounding box
        bbox.min[0] = numeric_limits<float>::max();
        bbox.min[1] = numeric_limits<float>::max();
        bbox.min[2] = numeric_limits<float>::max();
        bbox.max[0] = -numeric_limits<float>::max();
        bbox.max[1] = -numeric_limits<float>::max();
        bbox.max[2] = -numeric_limits<float>::max();
        OiVec xyz(4); //main focus of the pointcloud

        //QRegExp splitPattern("\\s+");

        while(!in.atEnd()){

            QString line = in.readLine();

            readSize += line.size();

            //QStringList fields = line.split(splitPattern);
            QStringList fields = line.split(' ');

            if(fields.size() >= 3){

                //create a new point and add it to the point cloud
                Point_PC *myPoint = new Point_PC();
                myPoint->xyz[0] = fields.at(0).toFloat();
                myPoint->xyz[1] = fields.at(1).toFloat();
                myPoint->xyz[2] = fields.at(2).toFloat();

                if(data.unit.value(UnitConverter::eMetric) == UnitConverter::eMILLIMETER){
                    myPoint->xyz[0] = myPoint->xyz[0] / 1000.0;
                    myPoint->xyz[1] = myPoint->xyz[1] / 1000.0;
                    myPoint->xyz[2] = myPoint->xyz[2] / 1000.0;
                }

                pc->addPointCloudPoint(myPoint);

                pointCount++;

                //update centroid
                x += myPoint->xyz[0];
                y += myPoint->xyz[1];
                z += myPoint->xyz[2];

                //update bounding box
                if(myPoint->xyz[0] < bbox.min[0]){
                    bbox.min[0] = myPoint->xyz[0];
                }
                if(myPoint->xyz[0] > bbox.max[0]){
                    bbox.max[0] = myPoint->xyz[0];
                }
                if(myPoint->xyz[1] < bbox.min[1]){
                    bbox.min[1] = myPoint->xyz[1];
                }
                if(myPoint->xyz[1] > bbox.max[1]){
                    bbox.max[1] = myPoint->xyz[1];
                }
                if(myPoint->xyz[2] < bbox.min[2]){
                    bbox.min[2] = myPoint->xyz[2];
                }
                if(myPoint->xyz[2] > bbox.max[2]){
                    bbox.max[2] = myPoint->xyz[2];
                }

            }

            int progress = (int)(((float)readSize / (float)fileSize) * 100.0);
            if(progress == 100){
                progress = 99;
            }
            emit this->updateProgress(progress, QString("%1 points loaded").arg(pointCount) );
            readSize += 2;

        }

        pc->setFeatureName(QString::number(pc->getId()));
        pc->setNominalSystem(data.nominalCoordSys);
        pc->setIsSolved(true);

        xyz.setAt(0, x / (double)pointCount);
        xyz.setAt(1, y / (double)pointCount);
        xyz.setAt(2, z / (double)pointCount);
        xyz.setAt(3, 1.0);
        pc->setMainFocus(xyz);

        pc->setBoundingBox(bbox);

        FeatureWrapper *f = new FeatureWrapper();

        f->setPointCloud(pc);

        data.features.append(f);

        data.device->close();

    }catch(exception &e){
        Console::addLine(e.what());
        return false;
    }

    Console::addLine(QString("pointcloud successfully loaded: %1 sec").arg((clock() - c1)/(double)CLOCKS_PER_SEC));
    return true;

}
