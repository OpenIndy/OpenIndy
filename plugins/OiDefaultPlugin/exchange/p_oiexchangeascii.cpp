#include "p_oiexchangeascii.h"

/*!
 * \brief OiExchangeAscii::init
 */
void OiExchangeAscii::init(){

    //set plugin meta data
    this->metaData.name = "OiExchangeAscii";
    this->metaData.pluginName = "OpenIndy Default Plugin";
    this->metaData.author = "bra";
    this->metaData.description = "Read simple Ascii files.";
    this->metaData.iid = "de.openIndy.plugin.exchange.exchangeSimpleAscii.v001";

    //set supported delimiters
    this->supportedDelimiters.append("whitespace [ ]");
    this->supportedDelimiters.append("semicolon [;]");

    //set supported geometries
    this->supportedGeometries.append(ePointGeometry);

}

/*!
 * \brief OiExchangeAscii::getDefaultColumnOrder
 * \param typeOfGeometry
 * \return
 */
QList<ExchangeSimpleAscii::ColumnType> OiExchangeAscii::getDefaultColumnOrder(const GeometryTypes &typeOfGeometry){

    QList<ExchangeSimpleAscii::ColumnType> defaultColumnOrder;

    try{

        //check if device exists
        if(this->device.isNull()){
            return defaultColumnOrder;
        }

        //if device is not opened yet, open it
        if(!this->device->isOpen()){
            this->device->open(QIODevice::ReadOnly | QIODevice::Text);
        }

        //read the first twenty lines to get the maximum number of columns
        int numColumns = 0;
        int sampleSize = 20;
        QTextStream in(this->device.data());
        while (!in.atEnd() && sampleSize >= 0){

            QString line = in.readLine();

            //split the line and compare its column count to the maximum column count found before
            QStringList columns = line.split(this->getDelimiter(this->usedDelimiter));
            if(columns.size() > numColumns){
                numColumns = columns.size();
            }

        }

        //close the device
        this->device->close();

        //depending on the geometry type and the number of columns fill the default columns order
        switch(typeOfGeometry){
        case ePointGeometry:
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
        emit this->sendMessage(e.what());
    }

    return defaultColumnOrder;

}

/*!
 * \brief OiExchangeAscii::getFilePreview
 * \param typeOfGeometry
 * \return
 */
QMap<ExchangeSimpleAscii::ColumnType, QVariantList> OiExchangeAscii::getFilePreview(const GeometryTypes &typeOfGeometry){

    QMap<ExchangeSimpleAscii::ColumnType, QVariantList> filePreview;

    //get the default column order
    QList<ExchangeSimpleAscii::ColumnType> defaultColumnOrder = this->getDefaultColumnOrder(typeOfGeometry);

    //insert an empty list for each column as default
    for(int i = 0; i < defaultColumnOrder.size(); i++){
        filePreview.insert(defaultColumnOrder.at(i), QVariantList());
    }

    try{

        //check if device exists
        if(this->device.isNull()){
            return filePreview;
        }

        //if device is not opened yet, open it
        if(!this->device->isOpen()){
            this->device->open(QIODevice::ReadOnly | QIODevice::Text);
        }

        //get the first twenty lines as preview
        int sampleSize = 20;
        QTextStream in(this->device);
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
                    continue;
                }

                //get the rows that already exist and append the current one
                QVariantList rows = filePreview.value(defaultColumnOrder.at(i));
                rows.append(columns.at(i));

            }

            sampleSize--;

        }

        //close the device
        this->device->close();

    }catch(const exception &e){
        emit this->sendMessage(e.what());
    }

    return filePreview;

}

/*!
 * \brief OiExchangeAscii::getPossibleColumns
 * \param typeOfGeometry
 * \return
 */
QList<ExchangeSimpleAscii::ColumnType> OiExchangeAscii::getPossibleColumns(const GeometryTypes &typeOfGeometry){

    QList<ExchangeSimpleAscii::ColumnType> possibleColumns;

    //add all possible columns depending on the type of geometry
    switch(typeOfGeometry){
    case ePointGeometry:
        possibleColumns.append(ExchangeSimpleAscii::eColumnX);
        possibleColumns.append(ExchangeSimpleAscii::eColumnY);
        possibleColumns.append(ExchangeSimpleAscii::eColumnZ);
        possibleColumns.append(ExchangeSimpleAscii::eColumnFeatureName);
        possibleColumns.append(ExchangeSimpleAscii::eColumnGroupName);
        possibleColumns.append(ExchangeSimpleAscii::eColumnComment);
        break;
    }

    return possibleColumns;

}

/*!
 * \brief OiExchangeAscii::getDelimiter
 * \param delimiterName
 * \return
 */
QRegExp OiExchangeAscii::getDelimiter(const QString &delimiterName) const{

    if(delimiterName.compare("whitespace [ ]") == 0){
        return QRegExp("\\s+");
    }else if(delimiterName.compare("semicolon [;]") == 0){
        return QRegExp("[;]");
    }

    return QRegExp(".*");

}

/*!
 * \brief OiExchangeAscii::importOiData
 */
void OiExchangeAscii::importOiData(){

    try{

        this->features.clear();

        //check if nominal system is valid
        if(this->nominalSystem.isNull()){
            emit this->importFinished(false);
            return;
        }

        //set the number of error prone lines to 0
        int numErrors = 0;

        switch(this->typeOfGeometry){
        case ePointGeometry:

            //check if device exists
            if(this->device.isNull()){
                emit this->importFinished(false);
                return;
            }

            //if device is not opened yet, open it
            if(!this->device->isOpen()){
                this->device->open(QIODevice::ReadOnly | QIODevice::Text);
            }

            qint64 fileSize = this->device->size();
            qint64 readSize = 0;
            qint64 numPoints = 0;

            //read all lines
            QTextStream in(this->device);
            while (!in.atEnd()){

                QString line = in.readLine();

                readSize += line.size();

                //split the line at delimiter
                QStringList columns = line.split(this->getDelimiter(this->usedDelimiter));

                //create a point object
                QPointer<Point> myNominal = new Point(true);

                bool errorWhileParsing = false;

                for(int i = 0; i < columns.size(); i++){

                    //stop parsing if the current line has too many columns
                    if(this->userDefinedColumns.size() <= i){
                        break;
                    }

                    //set the point attribute depending on the current column
                    switch(this->userDefinedColumns.at(i)){
                    case ExchangeSimpleAscii::eColumnFeatureName:
                        myNominal->setFeatureName(columns.at(i));
                        break;
                    case ExchangeSimpleAscii::eColumnComment:
                        myNominal->setComment(columns.at(i));
                        break;
                    case ExchangeSimpleAscii::eColumnGroupName:
                        myNominal->setGroupName(columns.at(i));
                        break;
                    case ExchangeSimpleAscii::eColumnX:{

                        double x = 0.0;
                        x = columns.at(i).toDouble(&errorWhileParsing);
                        errorWhileParsing = !errorWhileParsing;

                        //transform the unit of the imported coordinate to [m]
                        if(this->units.contains(eMetric) && this->units.value(eMetric) != eUnitMeter){
                            x = convertToDefault(x, this->units.value(eMetric));
                        }

                        if(!errorWhileParsing){
                            Position position = myNominal->getPosition();
                            OiVec vec = position.getVector();
                            vec.setAt(0, x);
                            position.setVector(vec);
                            myNominal->setPoint(position);
                        }

                        break;

                    }case ExchangeSimpleAscii::eColumnY:{

                        double y = 0.0;
                        y = columns.at(i).toDouble(&errorWhileParsing);
                        errorWhileParsing = !errorWhileParsing;

                        //transform the unit of the imported coordinate to [m]
                        if(this->units.contains(eMetric) && this->units.value(eMetric) != eUnitMeter){
                            y = convertToDefault(y, this->units.value(eMetric));
                        }

                        if(!errorWhileParsing){
                            Position position = myNominal->getPosition();
                            OiVec vec = position.getVector();
                            vec.setAt(1, y);
                            position.setVector(vec);
                            myNominal->setPoint(position);
                        }

                        break;

                    }case ExchangeSimpleAscii::eColumnZ:{

                        double z = 0.0;
                        z = columns.at(i).toDouble(&errorWhileParsing);
                        errorWhileParsing = !errorWhileParsing;

                        //transform the unit of the imported coordinate to [m]
                        if(this->units.contains(eMetric) && this->units.value(eMetric) != eUnitMeter){
                            z = convertToDefault(z, this->units.value(eMetric));
                        }

                        if(!errorWhileParsing){
                            Position position = myNominal->getPosition();
                            OiVec vec = position.getVector();
                            vec.setAt(2, z);
                            position.setVector(vec);
                            myNominal->setPoint(position);
                        }

                        break;

                    }case ExchangeSimpleAscii::eColumnIgnore:{
                        break;
                    }}

                    //if an error occured continue with the next line
                    if(errorWhileParsing){
                        numErrors++;
                        delete myNominal;
                        break;
                    }

                }

                //set group of the geometry
                if(this->groupName.compare("") != 0){
                    myNominal->setGroupName(this->groupName);
                }

                //set nominal system
                myNominal->setNominalSystem(this->nominalSystem);

                //add the imported nominal to OpenIndy
                if(!errorWhileParsing){
                    QPointer<FeatureWrapper> myGeometry = new FeatureWrapper();
                    myGeometry->setPoint(myNominal);
                    this->features.append(myGeometry);
                }

                //update import progress
                int progress = (int)(((float)readSize / (float)fileSize) * 100.0);
                if(progress == 100){
                    progress = 99;
                }
                numPoints++;
                emit this->updateProgress(progress, QString("%1 nominal(s) loaded").arg(numPoints) );
                readSize += 2;

            }

            //close the device
            this->device->close();

            //emit import finished signal
            emit this->importFinished(true);

            break;

        }

    }catch(const exception &e){
        emit this->sendMessage(e.what());
    }

}

/*!
 * \brief OiExchangeAscii::exportOiData
 */
void OiExchangeAscii::exportOiData(){

    try{

    }catch(const exception &e){
        emit this->sendMessage(e.what());
    }

}
