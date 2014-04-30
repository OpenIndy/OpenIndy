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
public:
    enum LineUnknowns{
        unknownX,
        unknownY,
        unknownZ,
        unknownI,
        unknownJ,
        unknownK
    };

    Line();
    Line(const Line &copy);
    ~Line();

    OiVec xyz;
    OiVec ijk;

    bool toOpenIndyXML(QXmlStreamWriter& stream);
    ElementDependencies fromOpenIndyXML(QXmlStreamReader& xml);

    void recalc();

    QString getDisplayX() const;
    QString getDisplayY() const;
    QString getDisplayZ() const;
    QString getDisplayI() const;
    QString getDisplayJ() const;
    QString getDisplayK() const;
    QString getDisplayIsCommon() const;
    QString getDisplayIsNominal() const;
    QString getDisplayObs() const;
    QString getDisplaySolved() const;
    QString getDisplayMConfig() const;
    QString getDisplayStdDev() const;
};

#endif // LINE_H
