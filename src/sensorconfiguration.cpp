#include "sensorconfiguration.h"

#include "sensor.h"

SensorConfiguration::SensorConfiguration()
{
    connConfig = new ConnectionConfig();

    //set defaults
    this->isSaved = false;
    this->mySensor = NULL;
}


/*!
 * \brief SensorgetName
 * \return
 */
QString SensorConfiguration::getName() const{
    return this->name;
}

/*!
 * \brief SensorgetDisplayName
 * \return
 */
QString SensorConfiguration::getDisplayName() const{
    if(this->getIsValid()){
        return QString("%1%2").arg(this->name).arg(this->isSaved?"":" (project)");
    }
    return "";
}

/*!
 * \brief SensorgetIsSaved
 * \return
 */
bool SensorConfiguration::getIsSaved() const{
    return this->isSaved;
}

/*!
 * \brief SensorgetIsValid
 * \return
 */
bool SensorConfiguration::getIsValid() const{
    if(this->name.compare("") == 0){
        return false;
    }
    return true;
}

/*!
 * \brief SensorsetName
 * \param name
 * \return
 */
bool SensorConfiguration::setName(QString name){
    if(this->name.compare("") == 0){
        this->name = name;
        return true;
    }
    return false;
}

/*!
 * \brief SensortoOpenIndyXML
 * \param xmlDoc
 * \return
 */
QDomElement SensorConfiguration::toOpenIndyXML(QDomDocument &xmlDoc) const{

    if(xmlDoc.isNull() || this->mySensor == NULL){
        return QDomElement();
    }

    QDomElement sConfig = xmlDoc.createElement("sensorConfig");
    sConfig.setAttribute("name", this->name);

    //get sensor information to know which attributes to save in xml
    QList<ReadingTypes> supportedReadingTypes = *this->mySensor->getSupportedReadingTypes();
    QList<ConnectionTypes> supportedConnectionTypes = *this->mySensor->getConnectionType();

    //save plugin information
    QDomElement pluginInfo = xmlDoc.createElement("plugin");
    pluginInfo.setAttribute("name", this->pluginName);
    pluginInfo.setAttribute("sensor", this->sensorName);
    sConfig.appendChild(pluginInfo);

    //save connection parameters
    QDomElement connParams = xmlDoc.createElement("connection");
    connParams.setAttribute("typeOfConnection", this->connConfig->typeOfConnection);
    if(supportedConnectionTypes.contains(eNetworkConnection)){
        connParams.setAttribute("ip", this->connConfig->ip);
        connParams.setAttribute("port", this->connConfig->port);
    }
    if(supportedConnectionTypes.contains(eSerialConnection)){
        connParams.setAttribute("comPort", this->connConfig->comPort);
        connParams.setAttribute("baudRate", this->connConfig->baudRate);
        connParams.setAttribute("dataBits", this->connConfig->dataBits);
        connParams.setAttribute("parity", this->connConfig->parity);
        connParams.setAttribute("stopBits", this->connConfig->stopBits);
        connParams.setAttribute("flowControl", this->connConfig->flowControl);
    }
    sConfig.appendChild(connParams);

    //save accuracy values
    QDomElement accuracy = xmlDoc.createElement("accuracy");
    if(supportedReadingTypes.contains(eCartesianReading)){
        accuracy.setAttribute("sigmaX", this->sigma.sigmaXyz.getAt(0));
        accuracy.setAttribute("sigmaY", this->sigma.sigmaXyz.getAt(1));
        accuracy.setAttribute("sigmaZ", this->sigma.sigmaXyz.getAt(2));
    }
    if(supportedReadingTypes.contains(ePolarReading)){
        accuracy.setAttribute("sigmaAzimuth", this->sigma.sigmaAzimuth);
        accuracy.setAttribute("sigmaZenith", this->sigma.sigmaZenith);
        accuracy.setAttribute("sigmaDistance", this->sigma.sigmaDistance);
    }else{
        if(supportedReadingTypes.contains(eDirectionReading)){
            accuracy.setAttribute("sigmaAzimuth", this->sigma.sigmaAzimuth);
            accuracy.setAttribute("sigmaZenith", this->sigma.sigmaZenith);
        }
        if(supportedReadingTypes.contains(eDistanceReading)){
            accuracy.setAttribute("sigmaDistance", this->sigma.sigmaDistance);
        }
    }
    if(supportedReadingTypes.contains(eLevelReading)){
        accuracy.setAttribute("sigmaAngleXZ", this->sigma.sigmaAngleXZ);
        accuracy.setAttribute("sigmaAngleYZ", this->sigma.sigmaAngleYZ);
    }
    if(supportedReadingTypes.contains(eTemperatureReading)){
        accuracy.setAttribute("sigmaTemp", this->sigma.sigmaTemp);
    }
    if(supportedReadingTypes.contains(eUndefinedReading)){
        QDomElement sigmaUndefined = xmlDoc.createElement("sigmaUndefined");
        QStringList params = this->sigma.sigmaUndefined.keys();
        for(int i = 0; i < params.size(); i++){
            sigmaUndefined.setAttribute(params.at(i), this->sigma.sigmaUndefined.value(params.at(i)));
        }
        accuracy.appendChild(sigmaUndefined);
    }
    sConfig.appendChild(accuracy);

    //save integer parameters
    if(!this->integerParameter.isEmpty()){
        QDomElement intParams = xmlDoc.createElement("integerParameters");
        QStringList params = this->integerParameter.keys();
        for(int i = 0; i < params.size(); i++){
            QDomElement param = xmlDoc.createElement("parameter");
            param.setAttribute("name", params.at(i));
            param.setAttribute("value", this->integerParameter.value(params.at(i)));
            intParams.appendChild(param);
        }
        sConfig.appendChild(intParams);
    }

    //save double parameters
    if(!this->doubleParameter.isEmpty()){
        QDomElement doubleParams = xmlDoc.createElement("doubleParameters");
        QStringList params = this->doubleParameter.keys();
        for(int i = 0; i < params.size(); i++){
            QDomElement param = xmlDoc.createElement("parameter");
            param.setAttribute("name", params.at(i));
            param.setAttribute("value", this->doubleParameter.value(params.at(i)));
            doubleParams.appendChild(param);
        }
        sConfig.appendChild(doubleParams);
    }

    //save string parameters
    if(!this->stringParameter.isEmpty()){
        QDomElement stringParams = xmlDoc.createElement("stringParameters");
        QStringList params = this->stringParameter.keys();
        for(int i = 0; i < params.size(); i++){
            QDomElement param = xmlDoc.createElement("parameter");
            param.setAttribute("name", params.at(i));
            param.setAttribute("value", this->stringParameter.value(params.at(i)));
            stringParams.appendChild(param);
        }
        sConfig.appendChild(stringParams);
    }

    return sConfig;

}

/*!
 * \brief SensorfromOpenIndyXML
 * \param xmlElem
 * \return
 */
bool SensorConfiguration::fromOpenIndyXML(QDomElement &xmlElem){

    if(xmlElem.isNull() || !xmlElem.hasAttribute("name")){
        return false;
    }

    //set name of sensor config
    this->name = xmlElem.attribute("name");

    //get plugin information
    QDomElement pluginInfo = xmlElem.firstChildElement("plugin");
    if(pluginInfo.isNull() || !pluginInfo.hasAttribute("name") || !pluginInfo.hasAttribute("sensor")){
        return false;
    }
    this->pluginName = pluginInfo.attribute("name");
    this->sensorName = pluginInfo.attribute("sensor");

    //get connection parameters
    QDomElement connectionParams = xmlElem.firstChildElement("connection");
    if(!connectionParams.isNull()){

        //set type of connection
        if(!connectionParams.hasAttribute("typeOfConnection")){
            return false;
        }
        this->connConfig->typeOfConnection = (ConnectionTypes)connectionParams.attribute("typeOfConnection").toInt();

        //set connection attributes
        if(connectionParams.hasAttribute("ip")){ this->connConfig->ip = connectionParams.attribute("ip"); }
        if(connectionParams.hasAttribute("port")){ this->connConfig->port = connectionParams.attribute("port"); }
        if(connectionParams.hasAttribute("comPort")){ this->connConfig->comPort = connectionParams.attribute("comPort"); }
        if(connectionParams.hasAttribute("baudRate")){ this->connConfig->baudRate = (QSerialPort::BaudRate)connectionParams.attribute("baudRate").toInt(); }
        if(connectionParams.hasAttribute("dataBits")){ this->connConfig->dataBits = (QSerialPort::DataBits)connectionParams.attribute("dataBits").toInt(); }
        if(connectionParams.hasAttribute("parity")){ this->connConfig->parity = (QSerialPort::Parity)connectionParams.attribute("parity").toInt(); }
        if(connectionParams.hasAttribute("stopBits")){ this->connConfig->stopBits = (QSerialPort::StopBits)connectionParams.attribute("stopBits").toInt(); }
        if(connectionParams.hasAttribute("flowControl")){ this->connConfig->flowControl = (QSerialPort::FlowControl)connectionParams.attribute("flowControl").toInt(); }

    }

    //get accuracy values
    QDomElement accuracy = xmlElem.firstChildElement("accuracy");
    if(!accuracy.isNull()){

        //set standard accuracy values if available
        if(accuracy.hasAttribute("sigmaAzimuth")){ this->sigma.sigmaAzimuth = accuracy.attribute("sigmaAzimuth").toDouble(); }
        if(accuracy.hasAttribute("sigmaZenith")){ this->sigma.sigmaZenith = accuracy.attribute("sigmaZenith").toDouble(); }
        if(accuracy.hasAttribute("sigmaDistance")){ this->sigma.sigmaDistance = accuracy.attribute("sigmaDistance").toDouble(); }
        if(accuracy.hasAttribute("sigmaX")){ this->sigma.sigmaXyz.setAt(0, accuracy.attribute("sigmaX").toDouble()); }
        if(accuracy.hasAttribute("sigmaY")){ this->sigma.sigmaXyz.setAt(1, accuracy.attribute("sigmaY").toDouble()); }
        if(accuracy.hasAttribute("sigmaZ")){ this->sigma.sigmaXyz.setAt(2, accuracy.attribute("sigmaZ").toDouble()); }
        if(accuracy.hasAttribute("sigmaTemp")){ this->sigma.sigmaTemp = accuracy.attribute("sigmaTemp").toDouble(); }
        if(accuracy.hasAttribute("sigmaAngleXZ")){ this->sigma.sigmaAngleXZ = accuracy.attribute("sigmaAngleXZ").toDouble(); }
        if(accuracy.hasAttribute("sigmaAngleYZ")){ this->sigma.sigmaAngleYZ = accuracy.attribute("sigmaAngleYZ").toDouble(); }

        //set undefined accuracy values if available
        QDomElement sigmaUndefined = accuracy.firstChildElement("sigmaUndefined");
        if(!sigmaUndefined.isNull()){
            QDomNodeList sigmas = sigmaUndefined.childNodes();
            for(int i = 0; i < sigmas.size(); ++i){
                QDomElement sigmaElement = sigmas.at(i).toElement();
                if(sigmaElement.hasAttribute("name") && sigmaElement.hasAttribute("value")){
                    this->sigma.sigmaUndefined.insert(sigmaElement.attribute("name"), sigmaElement.attribute("value").toDouble());
                }
            }
        }

    }

    //get integer parameters
    QDomElement intParams = xmlElem.firstChildElement("integerParameters");
    if(!intParams.isNull()){
        QDomNodeList params = intParams.childNodes();
        for(int i = 0; i < params.size(); ++i){
            QDomElement param = params.at(i).toElement();
            if(param.hasAttribute("name") && param.hasAttribute("value")){
                this->integerParameter.insert(param.attribute("name"), param.attribute("value").toInt());
            }
        }
    }

    //get double parameters
    QDomElement doubleParams = xmlElem.firstChildElement("doubleParameters");
    if(!doubleParams.isNull()){
        QDomNodeList params = doubleParams.childNodes();
        for(int i = 0; i < params.size(); ++i){
            QDomElement param = params.at(i).toElement();
            if(param.hasAttribute("name") && param.hasAttribute("value")){
                this->doubleParameter.insert(param.attribute("name"), param.attribute("value").toDouble());
            }
        }
    }

    //get string parameters
    QDomElement stringParams = xmlElem.firstChildElement("stringParameters");
    if(!stringParams.isNull()){
        QDomNodeList params = stringParams.childNodes();
        for(int i = 0; i < params.size(); ++i){
            QDomElement param = params.at(i).toElement();
            if(param.hasAttribute("name") && param.hasAttribute("value")){
                this->stringParameter.insert(param.attribute("name"), param.attribute("value"));
            }
        }
    }

    return true;

}

/*!
 * \brief SensorsetIsSaved
 * \param isSaved
 */
void SensorConfiguration::setIsSaved(bool isSaved){
    this->isSaved = isSaved;
}
