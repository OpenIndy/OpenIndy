#include "oiexchangepts.h"

oiExchangePTS::oiExchangePTS()
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

bool oiExchangePTS::importPointCloud(oiExchangeObject &data)
{
    clock_t c1 = clock();

    if (!data.device->open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return false;
    }

    try{

        QTextStream in(data.device);

        PointCloud *pc = new PointCloud(false);

        OiVec focalPoint(4); //centroid of pointcloud
        int pointCount = 0;

        QRegExp splitPattern("\\s+");

        while(!in.atEnd())
        {

            QString line = in.readLine();

            //QStringList list = line.split(splitPattern);
            QStringList list = line.split(' ');

            if(list.size() > 2){

                Point_PC *p = new Point_PC();

                p->xyz[0] = list.at(0).toFloat();
                p->xyz[1] = list.at(1).toFloat();
                p->xyz[2] = list.at(2).toFloat();

                if(data.unit.value(UnitConverter::eMetric) == UnitConverter::eMILLIMETER){
                    p->xyz[0] = p->xyz[0] / 1000.0;
                    p->xyz[1] = p->xyz[1] / 1000.0;
                    p->xyz[2] = p->xyz[2] / 1000.0;
                }

                pc->points.append(p);

                pointCount++;

                //update centroid
                focalPoint.setAt(0, p->xyz[0]+focalPoint.getAt(0));
                focalPoint.setAt(1, p->xyz[1]+focalPoint.getAt(1));
                focalPoint.setAt(2, p->xyz[2]+focalPoint.getAt(2));

                //update bounding box
                if(p->xyz[0] < pc->bbox.min[0]){
                    pc->bbox.min[0] = p->xyz[0];
                }
                if(p->xyz[0] > pc->bbox.max[0]){
                    pc->bbox.max[0] = p->xyz[0];
                }
                if(p->xyz[1] < pc->bbox.min[1]){
                    pc->bbox.min[1] = p->xyz[1];
                }
                if(p->xyz[1] > pc->bbox.max[1]){
                    pc->bbox.max[1] = p->xyz[1];
                }
                if(p->xyz[2] < pc->bbox.min[2]){
                    pc->bbox.min[2] = p->xyz[2];
                }
                if(p->xyz[2] > pc->bbox.max[2]){
                    pc->bbox.max[2] = p->xyz[2];
                }

            }
        }

        pc->setFeatureName(QString::number(pc->getId()));
        pc->setIsSolved(true);

        pc->xyz = focalPoint / (double)pointCount;
        pc->xyz.setAt(3,1.0);

        pc->pointCount = pointCount;

        FeatureWrapper *f = new FeatureWrapper();

        f->setPointCloud(pc);

        data.features.append(f);

        data.device->close();

    }catch(exception &e){
        Console::addLine(e.what());
    }

    Console::addLine("Punktwolke eingelesen: ", (clock() - c1)/(double)CLOCKS_PER_SEC);
}


