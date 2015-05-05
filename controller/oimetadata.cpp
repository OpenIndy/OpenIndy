#include "oimetadata.h"

//define version of the main program (src)
QString OiMetaData::version = "0.0.1";

//define plugin interface versions
QString OiMetaData::iid_Plugin = Plugin_iidd;
QString OiMetaData::iid_Sensor = Sensor_iidd;
QString OiMetaData::iid_LaserTracker=LaserTracker_iidd ;
QString OiMetaData::iid_TotalStation = TotalStation_iidd;
QString OiMetaData::iid_ConstructFunction = ConstructFunction_iidd;
QString OiMetaData::iid_FitFunction = FitFunction_iidd;
QString OiMetaData::iid_GenerateFeatureFunction = GenerateFeatureFunction_iidd;
QString OiMetaData::iid_GeodeticFunction = GeodeticFunction_iidd;
QString OiMetaData::iid_NetworkAdjustment = NetworkAdjustment_iidd;
QString OiMetaData::iid_ObjectTransformation = ObjectTransformation_iidd;
QString OiMetaData::iid_SystemTransformation = SystemTransformation_iidd;
QString OiMetaData::iid_Simulation = Simulation_iidd;
QString OiMetaData::iid_Tool = Tool_iidd;
QString OiMetaData::iid_Exchange = Exchange_iidd;
QString OiMetaData::iid_ExchangeSimpleAscii = ExchangeSimpleAscii_iidd;
QString OiMetaData::iid_ExchangeDefinedFormat = ExchangeDefinedFormat_iidd;

/*!
 * \brief OiMetaData::findIID
 * \param iid
 * \return
 */
bool OiMetaData::findIID(const QString &iid){

    if(iid.compare(OiMetaData::iid_LaserTracker) == 0){
        return true;
    }else if(iid.compare(OiMetaData::iid_ConstructFunction) == 0){
        return true;
    }else if(iid.compare(OiMetaData::iid_FitFunction) == 0){
        return true;
    }else if(iid.compare(OiMetaData::iid_GenerateFeatureFunction) == 0){
        return true;
    }else if(iid.compare(OiMetaData::iid_GeodeticFunction) == 0){
        return true;
    }else if(iid.compare(OiMetaData::iid_NetworkAdjustment) == 0){
        return true;
    }else if(iid.compare(OiMetaData::iid_ObjectTransformation) == 0){
        return true;
    }else if(iid.compare(OiMetaData::iid_SystemTransformation) == 0){
        return true;
    }else if(iid.compare(OiMetaData::iid_TotalStation) == 0){
        return true;
    }else if(iid.compare(OiMetaData::iid_Plugin) == 0){
        return true;
    }else if(iid.compare(OiMetaData::iid_Simulation) == 0){
        return true;
    }else if(iid.compare(OiMetaData::iid_Tool) == 0){
        return true;
    }else if(iid.compare(OiMetaData::iid_Exchange) == 0){
        return true;
    }else if(iid.compare(OiMetaData::iid_ExchangeSimpleAscii) == 0){
        return true;
    }else if(iid.compare(OiMetaData::iid_ExchangeDefinedFormat) == 0){
        return true;
    }

    return false;

}

/*!
 * \brief OiMetaData::getSensorTypeEnum
 * \param iid
 * \return
 */
SensorTypes OiMetaData::getSensorTypeEnum(const QString &iid){

    //check the given iid
    if(iid.compare(OiMetaData::iid_LaserTracker) == 0){
        return eLaserTracker;
    }else if(iid.compare(OiMetaData::iid_TotalStation) == 0){
        return eTotalStation;
    }

    return eUndefinedSensor;

}

/*!
 * \brief OiMetaData::getCompiler
 * \return
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
