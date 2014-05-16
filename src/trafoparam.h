#ifndef TRAFOPARAM_H
#define TRAFOPARAM_H

#include <QtCore/qmath.h>
#include <QDateTime>
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

    bool use;
    QDateTime validTime;

    void recalc();
    void generateHomogenMatrix();

    virtual bool toOpenIndyXML(QXmlStreamWriter& stream);
    virtual ElementDependencies fromOpenIndyXML(QXmlStreamReader& xml);

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
    QString getDisplayUse()const;
    QString getDisplayTime()const;
};

#endif // TRAFOPARAM_H
