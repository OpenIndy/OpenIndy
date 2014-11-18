#ifndef LINE_H
#define LINE_H

#include "oivec.h"
#include "geometry.h"

/*!
 * \brief The Line class
 * Specific Line feature with xyz and ijk vector
 */

class Line : public Geometry
{
    Q_OBJECT
public:
    enum LineUnknowns{
        unknownX,
        unknownY,
        unknownZ,
        unknownI,
        unknownJ,
        unknownK
    };

    Line(bool isNominal, QObject *parent = 0);
    Line(const Line &copy);
    ~Line();

    OiVec xyz;
    OiVec ijk;

    OiVec getXYZ() const;
    OiVec getIJK() const;

    QDomElement toOpenIndyXML(QDomDocument &xmlDoc) const;
    bool fromOpenIndyXML(QDomElement &xmlElem);

    bool saveSimulationData();

    void recalc();

    QString getDisplayX() const;
    QString getDisplayY() const;
    QString getDisplayZ() const;
    QString getDisplayI() const;
    QString getDisplayJ() const;
    QString getDisplayK() const;
    QString getDisplayIsCommon() const;
    QString getDisplayIsNominal() const;
    //QString getDisplayObs() const;
    QString getDisplaySolved() const;
    QString getDisplayMConfig() const;
    QString getDisplayStdDev() const;

    static MeasurementConfig defaultMeasurementConfig; //used as a default for this geometry type
};

#endif // LINE_H
