#ifndef TRAFOPARAM_H
#define TRAFOPARAM_H

#include <QtCore/qmath.h>
#include "feature.h"
#include "oivec.h"
#include "oimat.h"
#include "unitconverter.h"

class CoordinateSystem;
class Statistic;

/*!
 * \brief The TrafoParam class
 * includes all the transformation parameters.
 */
class TrafoParam : public Feature
{
public:
    TrafoParam();
    ~TrafoParam();

    OiMat homogenMatrix;
    OiVec translation;
    OiVec rotation;
    OiVec scale;
    OiVec quaternion;
    CoordinateSystem *from;
    CoordinateSystem *to;
    Statistic *stats;

    void recalc();
    void generateHomogenMatrix();

    virtual bool toOpenIndyXML(QXmlStreamWriter& stream);
    virtual bool fromOpenIndyXML(QXmlStreamReader& xml);

    QString getDisplayStartSystem()const;
    QString getDisplayDestinationSystem()const;
    QString getDisplayTranslationX()const;
    QString getDisplayTranslationY()const;
    QString getDisplayTranslationZ()const;
    QString getDisplayRotationX()const;
    QString getDisplayRotationY()const;
    QString getDisplayRotationZ()const;
    QString getDisplayScaleX()const;
    QString getDisplayScaleY()const;
    QString getDisplayScaleZ()const;
};

#endif // TRAFOPARAM_H
