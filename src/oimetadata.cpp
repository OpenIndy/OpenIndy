#include "oimetadata.h"

QString OiMetaData::version = "0.0.1";

QString OiMetaData::sensorPlugin = "sensor";
QString OiMetaData::functionPlugin = "function";

QString OiMetaData::iid_OiPlugin = OiPlugin_iidd;
QString OiMetaData::iid_Sensor = Sensor_iidd;
QString OiMetaData::iid_LaserTracker=LaserTracker_iidd ;
QString OiMetaData::iid_TotalStation = TotalStation_iidd;
QString OiMetaData::iid_ConstructFunction = ConstructFunction_iidd;
QString OiMetaData::iid_FitFunction = FitFunction_iidd;
QString OiMetaData::iid_GeodeticFunction = GeodeticFunction_iidd;
QString OiMetaData::iid_NetworkAdjustment = NetworkAdjustment_iidd;
QString OiMetaData::iid_ObjectTransformation = ObjectTransformation_iidd;
QString OiMetaData::iid_SystemTransformation = SystemTransformation_iidd;



/*!
 * \brief OiMetaData::getVersion
 * \return
 */
QString OiMetaData::getVersion(){

    return OiMetaData::version;

}

/*!
 * \brief OiMetaData::getCompiler
 * \return
 *compiler:
 *clang 64bit
 *intel
 *minGW
 *hewlett
 *intelC
 *MVSC 64bit
 *portland
 *oracle
 *
 */
QString OiMetaData::getCompiler(){

QString c;

#if defined(__clang__)
    /* Clang/LLVM. ---------------------------------------------- */
c = "clang 64bit";
return c;

#elif defined(__ICC) || defined(__INTEL_COMPILER)
    /* Intel ICC/ICPC. ------------------------------------------ */
c = "intel";
return c;

#elif defined(__GNUC__) || defined(__GNUG__)
    /* GNU GCC/G++. --------------------------------------------- */
c = "minGW";
return c;

#elif defined(__HP_cc) || defined(__HP_aCC)
    /* Hewlett-Packard C/aC++. ---------------------------------- */
c = "hewlett";
return c;

#elif defined(__IBMC__) || defined(__IBMCPP__)
    /* IBM XL C/C++. -------------------------------------------- */
c = "intelC";
return c;

#elif defined(_MSC_VER)
    /* Microsoft Visual Studio. --------------------------------- */
c = "MVSC 64bit";
return c;

#elif defined(__PGI)
    /* Portland Group PGCC/PGCPP. ------------------------------- */
c = "portland";
return c;

#elif defined(__SUNPRO_C) || defined(__SUNPRO_CC)
    /* Oracle Solaris Studio. ----------------------------------- */
c = "oracle";
return c;

#endif

    return c;

}

/*!
 * \brief OiMetaData::getOperatingSys
 * \return
 *operating systems:
 *mac os
 *windows
 *linux
 */
QString OiMetaData::getOperatingSys(){

    QString o;

#ifdef Q_OS_MAC
    o = "mac os";
#endif


#ifdef Q_OS_WIN
    o = "windows";
#endif

#ifdef Q_OS_LINUX
    o = "linux";
#endif

    return o;

}


bool OiMetaData::findIID(QString iid){

    if(iid==OiMetaData::iid_LaserTracker){
        return true;
    }else if(iid==OiMetaData::iid_ConstructFunction){
        return true;
    }else if(iid==OiMetaData::iid_FitFunction){
        return true;
    }else if(iid==OiMetaData::iid_GeodeticFunction){
        return true;
    }else if(iid==OiMetaData::iid_NetworkAdjustment){
        return true;
    }else if(iid==OiMetaData::iid_ObjectTransformation){
        return true;
    }else if(iid==OiMetaData::iid_SystemTransformation){
        return true;
    }else if(iid==OiMetaData::iid_TotalStation){
        return true;
    }else if(iid == OiMetaData::iid_OiPlugin){
        return true;
    }

    return false;

}
