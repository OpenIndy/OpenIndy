#ifndef TRAFOPARAM_H
#define TRAFOPARAM_H

#include <QObject>
#include <QPointer>
#include <QtCore/qmath.h>
#include <QDateTime>

#include "feature.h"
#include "statistic.h"
#include "oivec.h"
#include "oimat.h"

class CoordinateSystem;

/*!
 * \brief The TrafoParam class
 */
class TrafoParam : public Feature
{
    friend class OiJob;
    Q_OBJECT

public:
    explicit TrafoParam(QObject *parent = 0);

    TrafoParam(const TrafoParam &copy, QObject *parent = 0);

    TrafoParam &operator=(const TrafoParam &copy);

    ~TrafoParam();

    //#################################
    //get or set trafo param attributes
    //#################################

    const bool &getIsUsed() const;
    void setIsUsed(const bool &isUsed);

    const QPointer<CoordinateSystem> &getStartSystem() const;
    const QPointer<CoordinateSystem> &getDestinationSystem() const;
    bool setCoordinateSystems(const QPointer<CoordinateSystem> &from,
                              const QPointer<CoordinateSystem> &to);

    const bool &getIsMovement() const;
    void setIsMovement(const bool &isMovement);

    const QDateTime &getValidTime() const;
    void setValidTime(const QDateTime &validTime);

    const Statistic &getStatistic() const;
    void setStatistic(const Statistic &statistic);

    const bool &getIsDatumTrafo();
    void setIsDatumTrafo(const bool &isDatumTrafo);

    //####################################
    //get or set transformation parameters
    //####################################

    const OiMat &getHomogenMatrix() const;
    const OiVec &getTranslation() const;
    const OiVec &getRotation() const;
    const OiVec &getScale() const;
    //OiVec getQuaternion() const;

    bool setTransformationParameters(const OiVec &rotation, const OiVec &translation, const OiVec &scale);
    bool setTransformationParameters(const OiMat &rotation, const OiMat &translation, const OiMat &scale);
    bool setTransformationParameters(const OiMat &homogenMatrix);
    //bool setQuaternion(OiVec quaternion);

    //###########################
    //reexecute the function list
    //###########################

    void recalc();

    //#################
    //save and load XML
    //#################

    QDomElement toOpenIndyXML(QDomDocument &xmlDoc);
    bool fromOpenIndyXML(QDomElement &xmlElem);

    //###############
    //display methods
    //###############

    QString getDisplayStDev(const UnitType &type, const int &digits) const;

    QString getDisplayStartSystem() const;
    QString getDisplayDestinationSystem() const;
    QString getDisplayTranslationX(const UnitType &type, const int &digits) const;
    QString getDisplayTranslationY(const UnitType &type, const int &digits) const;
    QString getDisplayTranslationZ(const UnitType &type, const int &digits) const;
    QString getDisplayRotationX(const UnitType &type, const int &digits) const;
    QString getDisplayRotationY(const UnitType &type, const int &digits) const;
    QString getDisplayRotationZ(const UnitType &type, const int &digits) const;
    QString getDisplayScaleX(const int &digits) const;
    QString getDisplayScaleY(const int &digits) const;
    QString getDisplayScaleZ(const int &digits) const;
    QString getDisplayIsUsed() const;
    QString getDisplayValidTime() const;
    QString getDisplayIsMovement() const;

signals:

    //###########################################
    //signals to inform about trafo param changes
    //###########################################

    void transformationParameterChanged(const int &tpId);
    void coordinateSystemsChanged(const int &tpId);
    void isUsedChanged(const int &tpId);
    void validTimeChanged(const int &tpId);
    void isMovementChanged(const int &tpId);

private:

    //##############################
    //general trafo param attributes
    //##############################

    QPointer<CoordinateSystem> from;
    QPointer<CoordinateSystem> to;

    Statistic statistic;

    bool isDatumTrafo;
    bool isUsed; //true if this trafo param object is used for transformation (false if not)

    bool isMovement;
    QDateTime validTime; //used for movements to decide which observations shall be "moved"

    //#########################
    //transformation parameters
    //#########################

    OiVec translation;
    OiVec rotation;
    OiVec scale;

    OiMat homogenMatrix;
    //OiVec quaternion;

};

#endif // TRAFOPARAM_H
