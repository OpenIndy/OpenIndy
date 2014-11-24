#include "p_oiexchangeascii.h"

/*!
 * \brief OiExchangeAscii::getMetaData
 * \return
 */
PluginMetaData *OiExchangeAscii::getMetaData() const{

    PluginMetaData* metaData = new PluginMetaData();

    metaData->name = "OiExchangeAscii";
    metaData->pluginName = "OpenIndy Default Plugin";
    metaData->author = "br";
    metaData->description = QString("%1")
            .arg("Read simple Ascii files.");
    metaData->iid = "de.openIndy.Plugin.OiExchange.OiExchangeSimpleAscii.v001";

    return metaData;

}

/*!
 * \brief OiExchangeAscii::getDefaultColumnOrder
 * \param typeOfGeometry
 * \param projectData
 * \return
 */
QList<OiExchangeSimpleAscii::ColumnType> OiExchangeAscii::getDefaultColumnOrder(Configuration::GeometryTypes typeOfGeometry, const OiExchangeObject &projectData) const{

    QList<OiExchangeSimpleAscii::ColumnType> defaultColumnOrder;

    try{

        //check if device exists
        if(projectData.device == NULL){
            return QList<OiExchangeSimpleAscii::ColumnType>();
        }

        //if device is not opened yet, open it
        if(!projectData.device->isOpen()){
            projectData.device->open(QIODevice::ReadOnly | QIODevice::Text);
        }

        //read the first twenty lines to get the maximum number of columns
        int numColumns = 0;
        int sampleSize = 20;
        QTextStream in(projectData.device);
        while (!in.atEnd() && sampleSize >= 0){

            QString line = in.readLine();

            //split the line and compare its column count to the maximum column count found before
            QStringList columns = line.split(this->getDelimiter(this->usedDelimiter));
            if(columns.size() > numColumns){
                numColumns = columns.size();
            }

        }

        //close the device
        projectData.device->close();

        //depending on the geometry type and the number of columns fill the default columns order
        switch(typeOfGeometry){
        case Configuration::ePointGeometry:
            if(numColumns == 1){
                defaultColumnOrder.append(OiExchangeAscii::eColumnX);
            }else if(numColumns == 2){
                defaultColumnOrder.append(OiExchangeAscii::eColumnX);
                defaultColumnOrder.append(OiExchangeAscii::eColumnY);
            }else if(numColumns == 3){
                defaultColumnOrder.append(OiExchangeAscii::eColumnX);
                defaultColumnOrder.append(OiExchangeAscii::eColumnY);
                defaultColumnOrder.append(OiExchangeAscii::eColumnZ);
            }else if(numColumns == 4){
                defaultColumnOrder.append(OiExchangeAscii::eColumnFeatureName);
                defaultColumnOrder.append(OiExchangeAscii::eColumnX);
                defaultColumnOrder.append(OiExchangeAscii::eColumnY);
                defaultColumnOrder.append(OiExchangeAscii::eColumnZ);
            }else if(numColumns == 5){
                defaultColumnOrder.append(OiExchangeAscii::eColumnFeatureName);
                defaultColumnOrder.append(OiExchangeAscii::eColumnX);
                defaultColumnOrder.append(OiExchangeAscii::eColumnY);
                defaultColumnOrder.append(OiExchangeAscii::eColumnZ);
                defaultColumnOrder.append(OiExchangeAscii::eColumnGroupName);
            }else if(numColumns == 6){
                defaultColumnOrder.append(OiExchangeAscii::eColumnFeatureName);
                defaultColumnOrder.append(OiExchangeAscii::eColumnX);
                defaultColumnOrder.append(OiExchangeAscii::eColumnY);
                defaultColumnOrder.append(OiExchangeAscii::eColumnZ);
                defaultColumnOrder.append(OiExchangeAscii::eColumnGroupName);
                defaultColumnOrder.append(OiExchangeAscii::eColumnComment);
            }else if(numColumns > 6){
                defaultColumnOrder.append(OiExchangeAscii::eColumnFeatureName);
                defaultColumnOrder.append(OiExchangeAscii::eColumnX);
                defaultColumnOrder.append(OiExchangeAscii::eColumnY);
                defaultColumnOrder.append(OiExchangeAscii::eColumnZ);
                defaultColumnOrder.append(OiExchangeAscii::eColumnGroupName);
                defaultColumnOrder.append(OiExchangeAscii::eColumnComment);
                for(int i = 6; i < numColumns; i++){
                    defaultColumnOrder.append(OiExchangeAscii::eColumnIgnore);
                }
            }
            break;
        }

    }catch(const exception &e){
        this->writeToConsole(e.what());
    }

    return defaultColumnOrder;

}

/*!
 * \brief OiExchangeAscii::getFilePreview
 * \param typeOfGeometry
 * \param projectData
 * \return
 */
QMap<OiExchangeSimpleAscii::ColumnType, QVariantList> OiExchangeAscii::getFilePreview(Configuration::GeometryTypes typeOfGeometry, const OiExchangeObject &projectData) const{

    QMap<OiExchangeSimpleAscii::ColumnType, QVariantList> filePreview;

    //get the default column order
    QList<OiExchangeSimpleAscii::ColumnType> defaultColumnOrder = this->getDefaultColumnOrder(typeOfGeometry, projectData);

    //insert an empty list for each column as default
    for(int i = 0; i < defaultColumnOrder.size(); i++){
        filePreview.insert(defaultColumnOrder.at(i), QVariantList());
    }

    try{

        //check if device exists
        if(projectData.device == NULL){
            return filePreview;
        }

        //if device is not opened yet, open it
        if(!projectData.device->isOpen()){
            projectData.device->open(QIODevice::ReadOnly | QIODevice::Text);
        }

        //get the first twenty lines as preview
        int sampleSize = 20;
        QTextStream in(projectData.device);
        while (!in.atEnd() && sampleSize >= 0){

            QString line = in.readLine();

            //split the line at delimiter
            QStringList columns = line.split(this->getDelimiter(this->usedDelimiter));

            //insert the column entries
            for(int i = 0; i < defaultColumnOrder.size(); i++){

                //add an empty entry if the current row has not enough columns
                if(columns.size() <= i){
                    QVariantList rows = filePreview.value(defaultColumnOrder.at(i));
                    rows.append(QString());
                }

                //get the rows that already exist and append the current one
                QVariantList rows = filePreview.value(defaultColumnOrder.at(i));
                rows.append(columns.at(i));

            }

            sampleSize--;

        }

        //close the device
        projectData.device->close();

    }catch(const exception &e){
        this->writeToConsole(e.what());
    }

    return filePreview;

}

/*!
 * \brief OiExchangeAscii::getPossibleColumns
 * \param typeOfGeometry
 * \return
 */
QList<OiExchangeSimpleAscii::ColumnType> OiExchangeAscii::getPossibleColumns(Configuration::GeometryTypes typeOfGeometry) const{

    QList<OiExchangeSimpleAscii::ColumnType> possibleColumns;

    //add all possible columns depending on the type of geometry
    switch(typeOfGeometry){
    case Configuration::ePointGeometry:
        possibleColumns.append(OiExchangeSimpleAscii::eColumnX);
        possibleColumns.append(OiExchangeSimpleAscii::eColumnY);
        possibleColumns.append(OiExchangeSimpleAscii::eColumnZ);
        possibleColumns.append(OiExchangeSimpleAscii::eColumnFeatureName);
        possibleColumns.append(OiExchangeSimpleAscii::eColumnGroupName);
        possibleColumns.append(OiExchangeSimpleAscii::eColumnComment);
        break;
    }

    return possibleColumns;

}

/*!
 * \brief OiExchangeAscii::getSupportedDelimiters
 * \return
 */
QStringList OiExchangeAscii::getSupportedDelimiters() const{

    QStringList supportedDelimiters;

    //add supported delimiter names
    supportedDelimiters.append("whitespace [ ]");
    supportedDelimiters.append("semicolon [;]");

    return supportedDelimiters;

}

/*!
 * \brief OiExchangeAscii::getDelimiter
 * \param delimiterName
 * \return
 */
QRegExp OiExchangeAscii::getDelimiter(QString delimiterName) const{

    if(delimiterName.compare("whitespace [ ]")){
        return QRegExp("\\s+");
    }else if(delimiterName.compare("semicolon [;]")){
        return QRegExp("[;]");
    }

    return QRegExp(".*");

}

/*!
 * \brief OiExchangeAscii::importOiData
 * \param projectData
 * \return
 */
bool OiExchangeAscii::importOiData(OiExchangeObject &projectData){

    try{

        //set the number of error prone lines to 0
        int numErrors = 0;

        switch(this->typeOfGeometry){
        case Configuration::ePointGeometry:

            //check if device exists
            if(projectData.device == NULL){
                return false;
            }

            //if device is not opened yet, open it
            if(!projectData.device->isOpen()){
                projectData.device->open(QIODevice::ReadOnly | QIODevice::Text);
            }

            //read all lines
            QTextStream in(projectData.device);
            while (!in.atEnd()){

                QString line = in.readLine();

                //split the line at delimiter
                QStringList columns = line.split(this->getDelimiter(this->usedDelimiter));

                //create a point object
                Point *myNominal = new Point(true);
                myNominal->setId(-1);

                bool errorWhileParsing = false;

                for(int i = 0; i < columns.size(); i++){

                    //stop parsing if the current line has to many columns
                    if(this->userDefinedColumns.size() <= i){
                        break;
                    }

                    switch(this->userDefinedColumns.at(i)){
                    case OiExchangeSimpleAscii::eColumnFeatureName:
                        myNominal->setFeatureName(columns.at(i));
                        break;
                    case OiExchangeSimpleAscii::eColumnComment:
                        myNominal->setFeatureName(columns.at(i));
                        break;
                    case OiExchangeSimpleAscii::eColumnGroupName:
                        myNominal->setFeatureName(columns.at(i));
                        break;
                    case OiExchangeSimpleAscii::eColumnX:{

                        double x = 0.0;
                        x = columns.at(i).toDouble(&errorWhileParsing);
                        errorWhileParsing = !errorWhileParsing;

                        if(!errorWhileParsing){
                            myNominal->xyz.setAt(0, x);
                        }

                        break;

                    }case OiExchangeSimpleAscii::eColumnY:{

                        double y = 0.0;
                        y = columns.at(i).toDouble(&errorWhileParsing);
                        errorWhileParsing = !errorWhileParsing;

                        if(!errorWhileParsing){
                            myNominal->xyz.setAt(1, y);
                        }

                        break;

                    }case OiExchangeSimpleAscii::eColumnZ:{

                        double z = 0.0;
                        z = columns.at(i).toDouble(&errorWhileParsing);
                        errorWhileParsing = !errorWhileParsing;

                        if(!errorWhileParsing){
                            myNominal->xyz.setAt(2, z);
                        }

                        break;

                    }case OiExchangeSimpleAscii::eColumnIgnore:{
                        break;
                    }}

                    //if an error occured continue with the next line
                    if(errorWhileParsing){
                        numErrors++;
                        delete myNominal;
                        break;
                    }

                }

                //add the imported nominal to OpenIndy
                if(!errorWhileParsing){
                    FeatureWrapper *myGeometry = new FeatureWrapper();
                    myGeometry->setPoint(myNominal);
                    this->nominalCoordSys->addNominal(myGeometry);
                }

            }

            //close the device
            projectData.device->close();

            break;

        }

    }catch(const exception &e){
        this->writeToConsole(e.what());
        return false;
    }

    return true;

}

/*!
 * \brief OiExchangeAscii::exportOiData
 * \param projectData
 * \return
 */
bool OiExchangeAscii::exportOiData(OiExchangeObject &projectData){

    try{

    }catch(const exception &e){
        this->writeToConsole(e.what());
    }

    return true;

}

/*!
 * \brief OiExchangeAscii::getSupportedGeometries
 * \return
 */
QList<Configuration::GeometryTypes> OiExchangeAscii::getSupportedGeometries(){

    QList<Configuration::GeometryTypes> supportedGeometries;

    //add supported geometries
    supportedGeometries.append(Configuration::ePointGeometry);

    return supportedGeometries;

}
