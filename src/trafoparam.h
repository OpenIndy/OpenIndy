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
    Q_OBJECT
public:
    explicit TrafoParam(QObject *parent = 0);
    ~TrafoParam();

    OiMat getHomogenMatrix() const;
    void setHomogenMatrix(OiMat hm);
    OiVec getTranslation() const;
    bool setTranslation(double tx, double ty, double tz);
    OiVec getRotation() const;
    bool setRotation(double rx, double ry, double rz);
    OiVec getScale() const;
    bool setScale(double mx, double my, double mz);
    OiVec getQuaternion() const;
    bool setQuaternion(OiVec quaternion);
    CoordinateSystem * const getStartSystem() const;
    CoordinateSystem * const getDestinationSystem() const;
    bool setCoordinateSystems(CoordinateSystem * const from, CoordinateSystem * const to);
    bool getIsUsed() const;
    void setIsUsed(bool isUsed);
    QDateTime getValidTime();
    void setValidTime(QDateTime validTime);
    bool getIsMovement();
    void setIsMovement(bool isMovement);
    Statistic *getStatistic() const;
    void setStatistic(Statistic *myStatistic);
    bool getisDatumTrafo();
    void setisDatumTrafo(bool isDatumTrafo);

    void recalc();

    virtual bool toOpenIndyXML(QXmlStreamWriter& stream);
    virtual ElementDependencies fromOpenIndyXML(QXmlStreamReader& xml);

    QString getDisplayStartSystem() const;
    QString getDisplayDestinationSystem() const;
    QString getDisplayTranslationX() const;
    QString getDisplayTranslationY() const;
    QString getDisplayTranslationZ() const;
    QString getDisplayRotationX() const;
    QString getDisplayRotationY() const;
    QString getDisplayRotationZ() const;
    QString getDisplayScaleX() const;
    QString getDisplayScaleY() const;
    QString getDisplayScaleZ() const;
    QString getDisplayUse() const;
    QString getDisplayTime() const;

signals:
    void transformationParameterChanged(int tpId);
    void coordinateSystemsChanged(int tpId);
    void isUsedChanged(int tpId);
    void validTimeChanged(int tpId);
    void isMovementChanged(int tpId);

protected:
    OiMat homogenMatrix;
    OiVec translation;
    OiVec rotation;
    OiVec scale;
    OiVec quaternion;
    CoordinateSystem *from;
    CoordinateSystem *to;
    Statistic *myStatistic;
    bool use;
    QDateTime validTime;
    bool isMovement;
    bool isDatumTrafo;
};

#endif // TRAFOPARAM_H
