#ifndef P_HELMERT6PARAM_H
#define P_HELMERT6PARAM_H

#include "pi_systemtransformation.h"

using namespace std;

/*!
 * \brief The Helmert6Param class is a helmert 6 parameter transformation without scale.
 * Angles and Translation are approximated with Drixler algorithm.
 */
class Helmert6Param : public SystemTransformation
{
public:
    Helmert6Param();

    PluginMetaData* getMetaData() const;
    bool exec(TrafoParam&);
    QList<InputParams> getNeededElements() const;
    QList<Configuration::FeatureTypes> applicableFor() const;

    QMap<QString, QStringList> getStringParameter() const;

    QStringList getResultProtocol() const;

private:
    bool svdError;
    QList<OiVec> locSystem;
    QList<OiVec> refSystem;
    QStringList protocol;
    OiVec translation;
    OiVec rotation;

    void init();
    vector<OiVec> calcCentroidCoord();
    vector<OiVec> centroidReducedCoord(QList<OiVec> input, OiVec centroid);
    vector<OiMat> modelMatrix(vector<OiVec> locC, vector<OiVec> refC);
    OiMat normalEquationMatrix(vector<OiMat> vecA);
    OiVec quaternion(OiMat n);
    OiMat rotationMatrix(OiVec q);
    bool adjust(TrafoParam &tp);
    OiMat fillAMatrix(OiVec x0);
    OiVec fillLVector();
    OiVec fillL0Vector(OiVec x0);
    void preliminaryTransformation();
    OiVec approxRotation();
    OiVec approxTranslation(OiVec rot);

    void applyMovements(TrafoParam &tp);

    OiMat getRotationMatrix(OiVec rot);
    OiVec getRotationAngles(OiMat r);
    OiMat getTranslationMatrix(OiVec trans);
    OiMat getScaleMatrix(OiVec s);

    bool getCoordSysWithMovements(CoordinateSystem *cs);
    TrafoParam* getMovement(CoordinateSystem *cs);
};

#endif // P_HELMERT6PARAM_H
