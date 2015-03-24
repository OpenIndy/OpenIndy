#ifndef TRAFOPARAM_H
#define TRAFOPARAM_H

#include <QtCore/qmath.h>
#include <QDateTime>
#include "feature.h"
#include "oivec.h"
#include "oimat.h"

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

    bool setHomogenMatrix(OiMat rotation, OiMat translation, OiMat scale);
    OiMat getHomogenMatrix() const;
    OiVec getTranslation() const;
    OiVec getRotation() const;
    OiVec getScale() const;
    OiVec getQuaternion() const;

    const QPointer<CoordinateSystem> &getStartSystem() const;
    const QPointer<CoordinateSystem> &getDestinationSystem() const;
    bool setCoordinateSystems(const QPointer<CoordinateSystem> &from, const QPointer<CoordinateSystem> &to);
    bool getIsUsed() const;
    void setIsUsed(const bool &isUsed);
    QDateTime getValidTime();
    void setValidTime(QDateTime validTime);
    bool getIsMovement();
    void setIsMovement(bool isMovement);
    Statistic *getStatistic() const;
    void setStatistic(Statistic *myStatistic);
    bool getisDatumTrafo();
    void setisDatumTrafo(bool isDatumTrafo);

    void recalc();

    QDomElement toOpenIndyXML(QDomDocument &xmlDoc);
    bool fromOpenIndyXML(QDomElement &xmlElem);

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
    QString getDisplayStdDev() const;

signals:
    void transformationParameterChanged(const int &tpId);
    void coordinateSystemsChanged(const int &tpId);
    void isUsedChanged(const int &tpId);
    void validTimeChanged(const int &tpId);
    void isMovementChanged(const int &tpId);

protected:
    OiMat homogenMatrix;
    OiVec translation;
    OiVec rotation;
    OiVec scale;
    OiVec quaternion;
    QPointer<CoordinateSystem> from;
    QPointer<CoordinateSystem> to;
    Statistic *myStatistic;
    bool use;
    QDateTime validTime;
    bool isMovement;
    bool isDatumTrafo;

    bool setQuaternion(OiVec quaternion);
};

#endif // TRAFOPARAM_H
