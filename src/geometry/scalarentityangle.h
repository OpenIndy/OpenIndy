#ifndef SCALARENTITYANGLE_H
#define SCALARENTITYANGLE_H

#include <QtMath>
#include "geometry.h"
#include "oivec.h"
#include "statistic.h"

class ScalarEntityAngle : public Geometry
{
    Q_OBJECT
public:
    ScalarEntityAngle(bool isNominal, QObject *parent = 0);
    ScalarEntityAngle(const ScalarEntityAngle &copy);

    void recalc();

    QDomElement toOpenIndyXML(QDomDocument &xmlDoc) const;
    bool fromOpenIndyXML(QDomElement &xmlDoc);
    ElementDependencies fromOpenIndyXML(QXmlStreamReader& xml);

    bool saveSimulationData();

    double getAngle() const;
    void setAngle(double angle);
    void setAngle(OiVec direction1, OiVec direction2);

    double getScalar() const;

    QString getDisplayIsCommon() const;
    QString getDisplayIsNominal() const;
    //QString getDisplayObs() const;
    QString getDisplaySolved() const;
    QString getDisplayMConfig() const;
    QString getDisplayStdDev() const;
    QString getDisplayScalarAngleValue() const;

private:
    double angle;


};

#endif // SCALARENTITYANGLE_H
