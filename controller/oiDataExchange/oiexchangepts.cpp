#include "oiexchangepts.h"

oiExchangePTS::oiExchangePTS()
{
}

bool oiExchangePTS::importOiData(oiExchangeObject &data)
{
    switch (data.typeOfElement) {
    case (Configuration::ePointCloudElement):{
        return this->importPointCloud(data);
        break;}
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
    if (!data.device->open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return false;
    }


    QTextStream in(data.device);

    PointCloud *pc = new PointCloud(false);

    OiVec focalPoint(4); //Schwerpunkt der Punktwolke
    int pointCount;

    while(!in.atEnd())
    {
        QString line = in.readLine();

        QStringList list = line.split(QRegExp("\\s+"));

        if(list.size() >2){

            OiVec p(4);

            p.setAt(0,list.at(0).toDouble());
            p.setAt(1,list.at(1).toDouble());
            p.setAt(2,list.at(2).toDouble());
            p.setAt(3,1.0);

            if(data.unit.value(UnitConverter::eMetric) == UnitConverter::eMILLIMETER){
                p = p/1000;
            }

            pc->points.append(p);

            pointCount += 1;

            focalPoint = focalPoint + p;

        }
    }

    pc->setFeatureName(QString::number(pc->getId()));
    pc->setIsSolved(true);

    pc->xyz = focalPoint/pointCount;

    FeatureWrapper *f = new FeatureWrapper();

    f->setPointCloud(pc);

    data.features.append(f);

    //data.nominalCoordSys->getNominals().append(pc);
}


