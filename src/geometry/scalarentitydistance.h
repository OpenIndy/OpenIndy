#ifndef SCALARENTITYDISTANCE_H
#define SCALARENTITYDISTANCE_H

#include "geometry.h"
#include "statistic.h"

class ScalarEntityDistance : public Geometry
{
    Q_OBJECT
public:
    ScalarEntityDistance(bool isNominal, QObject *parent = 0);
    ScalarEntityDistance(const ScalarEntityDistance &copy);

    void recalc();

    QDomElement toOpenIndyXML(QDomDocument &xmlDoc) const;
    bool fromOpenIndyXML(QDomElement &xmlElem);
    ElementDependencies fromOpenIndyXML(QXmlStreamReader& xml);

    bool saveSimulationData();

    double getDistance() const;
    void setDistance(double distance);

    double getScalar() const;

    QString getDisplayIsCommon() const;
    QString getDisplayIsNominal() const;
    //QString getDisplayObs() const;
    QString getDisplaySolved() const;
    QString getDisplayMConfig() const;
    QString getDisplayStdDev() const;
    QString getDisplayScalarDistanceValue() const;

private:
    double distance;

};

#endif // SCALARENTITYDISTANCE_H
