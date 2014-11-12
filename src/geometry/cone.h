#ifndef CONE_H
#define CONE_H

#include "geometry.h"

class Cone : public Geometry
{
    Q_OBJECT
public:
    Cone(bool isNominal, QObject *parent = 0);
    Cone(const Cone &copy);

    OiVec xyz;
    OiVec ijk;
    double alpha;

    OiVec getXYZ() const;
    OiVec getIJK() const;

    void recalc();

    QDomElement toOpenIndyXML(QDomDocument &xmlDoc) const;
    bool fromOpenIndyXML(QDomElement &xmlElem);

    bool saveSimulationData();

    //TODO implement cone
};

#endif // CONE_H
