#include "p_shiftplane.h"

PluginMetaData* ShiftPlane::getMetaData() const{
    PluginMetaData* metaData = new PluginMetaData();
    metaData->name = "ShiftPlane";
    metaData->pluginName = "OpenIndy Default Plugin";
    metaData->author = "OpenIndyOrg";
    metaData->description = QString("%1 %2 %3")
            .arg("This function shifts the target plane by the specified amount.")
            .arg("<br><br> <u>parameter description:</u> <br>")
            .arg("<b>shift amount:</b> Defines the amount by which the plane is moved.");
    metaData->iid = "de.openIndy.Plugin.Function.ObjectTransformation.v001";
    return metaData;
}

QList<InputParams> ShiftPlane::getNeededElements() const{
    QList<InputParams> result;
    return result;
}

QList<Configuration::FeatureTypes> ShiftPlane::applicableFor() const{
    QList<Configuration::FeatureTypes> result;
    result.append(Configuration::ePlaneFeature);
    return result;
}

QMap<QString, double> ShiftPlane::getDoubleParameter() const{
    QMap<QString, double> result;
    QString key = "shift amount";
    double value = 0.0;
    result.insert(key, value);
    return result;
}

bool ShiftPlane::exec(Plane &p){
    if(this->isValid()){
        FunctionConfiguration myConfig = this->getFunctionConfiguration();
        QMap<QString, double> doubleParameter = myConfig.doubleParameter;
        double shiftAmount = 0.0;
        if(doubleParameter.contains("shift amount")){
            double value = static_cast<double>(doubleParameter.find("shift amount").value());
            shiftAmount = value;
            OiVec x0(3);
            OiVec n0(3);
            for(int i = 0; i < 3; i++){
                x0.setAt(i, p.xyz.getAt(i));
                n0.setAt(i, p.ijk.getAt(i));
            }
            n0.normalize();
            x0 = x0 + shiftAmount * n0;
            x0.add(1.0);
            p.xyz = x0;
            return true;
        }else{
            this->writeToConsole("No shift amount set");
            return false;
        }
    }else{
        this->writeToConsole("No shift amount set");
        return false;
    }
}
