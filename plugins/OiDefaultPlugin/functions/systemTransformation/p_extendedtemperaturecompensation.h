#ifndef P_EXTENDEDTEMPERATURECOMPENSATION_H
#define P_EXTENDEDTEMPERATURECOMPENSATION_H

#include "systemtransformation.h"
#include "pluginmetadata.h"
#include <QtCore/qmath.h>
#include "materials.h"

using namespace std;

/*!
 * \brief The ExtendedTemperatureCompensation class is a system transformation with temperature compensation.
 * By specifying a minimum of 3 common points between the systems, a reference temperature and the material, this
 * function calculates the scales (x,y,z) , translation and rotation
 */
class ExtendedTemperatureCompensation : public SystemTransformation
{
    Q_OBJECT

public:

    //##############################
    //function initialization method
    //##############################

    void init();

protected:

    //############
    //exec methods
    //############

    bool exec(TrafoParam &trafoParam);

    //##############################################################################################
    //tell OpenIndy wether the input elements shall be shown as one entry in function plugin loader)
    //##############################################################################################

    virtual bool mergeInputElements();

private:

    QStringList protocol;

    bool svdError;

    QList<OiVec> locSystem;
    QList<OiVec> refSystem;

    OiVec translation;
    OiVec rotation;
    OiVec scale;

    bool calc(TrafoParam &tp);
    void initPoints();
    void getExtraParameter();
    OiVec approxTranslation(OiVec rot, OiVec s);
    OiVec approxRotation();
    OiVec approxScale(OiVec rot);
    OiVec calcCentroidPoint(QList<OiVec> points);
    OiVec fillLVector();
    OiMat fillAMatrix(OiVec x0);
    OiMat getRotationMatrix(OiVec rot);
    void preliminaryTransformation();
    OiMat scaleMatrix(OiVec s);
    OiVec fillL0Vector(OiVec x0);

    //for approx of rotation
    vector<OiVec> centroidReducedCoord(QList<OiVec> input, OiVec centroid);
    vector<OiMat> modelMatrix(vector<OiVec> locC, vector<OiVec> refC);
    OiMat normalEquationMatrix(vector<OiMat>vecA);
    OiVec quaternion(OiMat n);
    OiMat rotationMatrix(OiVec q);
    OiVec getRotationAngles(OiMat r);


    //to save the values in the transformation parameter
    //rotation matrix see functions above "getRoationMatrix(OiVec rot)"
    OiMat getTranslationMatrix(OiVec trans);
    OiMat getScaleMatrix(OiVec s);
};

#endif // P_EXTENDEDTEMPERATURECOMPENSATION_H
