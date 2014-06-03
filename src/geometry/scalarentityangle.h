#ifndef SCALARENTITYANGLE_H
#define SCALARENTITYANGLE_H

#include <QtMath>
#include "geometry.h"
#include "oivec.h"
#include "statistic.h"

class ScalarEntityAngle : public Geometry
{
public:
    ScalarEntityAngle();
    ScalarEntityAngle(const ScalarEntityAngle &copy);

    void recalc();

    bool toOpenIndyXML(QXmlStreamWriter& stream);
    ElementDependencies fromOpenIndyXML(QXmlStreamReader& xml);

    double getAngle() const;
    void setAngle(double angle);
    void setAngle(OiVec direction1, OiVec direction2);

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
