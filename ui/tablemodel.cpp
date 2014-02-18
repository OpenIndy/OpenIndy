#include "tablemodel.h"

/*!
 * \brief TableModel::TableModel
 * \param features
 * \param coordSys
 * \param parent
 */
TableModel::TableModel(QList<FeatureWrapper*> &features, Station *myStation,FeatureWrapper *myFeature,QObject *parent) :
    QAbstractTableModel(parent),features(features),activeStation(myStation),activeFeature(myFeature)
{

}

/*!
 * \brief TableModel::rowCount
 * \return
 */
int TableModel::rowCount(const QModelIndex& ) const{
    if(features.size()>0){
        return features.size();
    }

    return 0;
}

/*!
 * \brief TableModel::columnCount
 * \param parent
 * \return
 */
int TableModel::columnCount(const QModelIndex &parent) const{
    //return m_columns.size();
    return 31;
}

/*!
 * \brief TableModel::data
 * Displays the in the model specified data of all existing features in the tableview.
 * \param index
 * \param role
 * \return
 */
QVariant TableModel::data(const QModelIndex &index, int role) const{

    if(!index.isValid())
        return QVariant();

    QString functions = "";

    if(Qt::DisplayRole == role){

        switch (index.column()) {
        case 0:
            return features.at(index.row())->returnFeatureType();
            //Configuration::FeatureTypes f = features.at(index.row())->getTypeOfFeature();
            //return featureType(f);
            break;
        case 1:
            return features.at(index.row())->getFeature()->name;
            break;
        case 2:
            if(features.at(index.row())->getFeature()->isSolved){
                return features.at(index.row())->getFeature()->getDisplayX();
            }else{
                return QVariant();
            }
            break;
        case 3:
            if(features.at(index.row())->getFeature()->isSolved){
                return features.at(index.row())->getFeature()->getDisplayY();
            }else{
                return QVariant();
            }

            break;
        case 4:
            if(features.at(index.row())->getFeature()->isSolved){
                return features.at(index.row())->getFeature()->getDisplayZ();
            }else{
                return QVariant();
            }
            break;
        case 5:
            return features.at(index.row())->getFeature()->getDisplayObs();
            break;
        case 6:
            return features.at(index.row())->getFeature()->getDisplaySolved();
            break;
        case 7:
            return features.at(index.row())->getFeature()->getDisplayMConfig();
            break;
        case 8:
            /*if(features.at(index.row())->getFeature()->functionList.size()==0){
                return "no function set";
            }else{
                functions += features.at(index.row())->getFeature()->functionList.at(0)->getName();
                for(int i=1;i<features.at(index.row())->getFeature()->functionList.size();i++){
                    functions += "," + features.at(index.row())->getFeature()->functionList.at(i)->getName();
                }
                return functions;
            }*/
            if(features.at(index.row())->getFeature()->functionList.size()==0){
                            return "no function set";
                }else{
                    functions += features.at(index.row())->getFeature()->functionList.at(0)->getMetaData()->name;
                    for(int i=1;i<features.at(index.row())->getFeature()->functionList.size();i++){
                        functions += "," + features.at(index.row())->getFeature()->functionList.at(i)->getMetaData()->name;
                    }
                    return functions;
            }
            break;
        case 9:
            return features.at(index.row())->getFeature()->getDisplayIsCommon();
            break;
        case 10:
            return features.at(index.row())->getFeature()->getDisplayIsNominal();
            break;
        case 11:
            return features.at(index.row())->getFeature()->getDisplayStdDev();
            break;
        case 12:
            if(features.at(index.row())->getFeature()->isSolved){
                return features.at(index.row())->getFeature()->getDisplayI();
            }else{
                return QVariant();
            }
            break;
        case 13:
            if(features.at(index.row())->getFeature()->isSolved){
                return features.at(index.row())->getFeature()->getDisplayJ();
            }else{
                return QVariant();
            }
            break;
        case 14:
            if(features.at(index.row())->getFeature()->isSolved){
                return features.at(index.row())->getFeature()->getDisplayK();
            }else{
                return QVariant();
            }
            break;
        case 15:
            if(features.at(index.row())->getFeature()->isSolved){
                return features.at(index.row())->getFeature()->getDisplayRadius();
            }else{
                return QVariant();
            }
            break;
        case 16:
            if(features.at(index.row())->getFeature()->isSolved){
                return features.at(index.row())->getFeature()->getDisplayScalarDistanceValue();
            }else{
                return QVariant();
            }
            break;
        case 17:
            if(features.at(index.row())->getFeature()->isSolved){
                return features.at(index.row())->getFeature()->getDisplayScalarAngleValue();
            }else{
                return QVariant();
            }
            break;
        case 18:
            return features.at(index.row())->getFeature()->getDisplayStartSystem();
            break;
        case 19:
            return features.at(index.row())->getFeature()->getDisplayDestinationSystem();
            break;
        case 20:
            return features.at(index.row())->getFeature()->getDisplayTranslationX();
            break;
        case 21:
            return this->features.at(index.row())->getFeature()->getDisplayTranslationY();
            break;
        case 22:
            return this->features.at(index.row())->getFeature()->getDisplayTranslationZ();
            break;
        case 23:
            return this->features.at(index.row())->getFeature()->getDisplayRotationX();
            break;
        case 24:
            return this->features.at(index.row())->getFeature()->getDisplayRotationY();
            break;
        case 25:
            return this->features.at(index.row())->getFeature()->getDisplayRotationZ();
            break;
        case 26:
            return this->features.at(index.row())->getFeature()->getDisplayScaleX();
            break;
        case 27:
            return this->features.at(index.row())->getFeature()->getDisplayScaleY();
            break;
        case 28:
            return this->features.at(index.row())->getFeature()->getDisplayScaleZ();
            break;
        case 29:
            return this->features.at(index.row())->getFeature()->getDisplayScalarTemperatureValue();
            break;
        case 30:
            return this->features.at(index.row())->getFeature()->getDisplayScalarMeasurementSeriesValue();
            break;
        default:
            break;
        }
    }

    if (role == Qt::BackgroundRole){

        if(features.at(index.row())->getStation() == NULL && features.at(index.row())->getCoordinateSystem() == NULL &&
                features.at(index.row())->getCoordinateSystem() == NULL){
            if(features.at(index.row())->getGeometry() != NULL && features.at(index.row())->getGeometry()->mConfig.typeOfReading == -1){
                if(index.column() == 7){
                    return QColor(Qt::red);
                }
            }
        }


        if(features.at(index.row())->getStation() != NULL && features.at(index.row())->getStation()->position->mConfig.typeOfReading == -1){
            if(index.column() == 7){
                return QColor(Qt::red);
            }
        }

        if (activeFeature != NULL && features.at(index.row())->getFeature() == activeFeature->getFeature()){
            return QColor(QColor::fromCmykF(0.59,0.40,0.10,0.10).lighter());
        }

        if(features.at(index.row())->getStation() != NULL){
            if(features.at(index.row())->getStation() == this->activeStation){
                return QColor(Qt::darkGray);
            }
        }

        if (features.at(index.row())->getStation()!=NULL){
            return QColor(Qt::lightGray);
        }

        //if feature is not solved
        if( (index.column() == 2 || index.column() == 3 || index.column() == 4
                || index.column() == 12 || index.column() == 13
                || index.column() == 14 || index.column() == 15
                || index.column() == 16 || index.column() == 17)
                && !features.at(index.row())->getFeature()->isSolved){
            return QColor(Qt::yellow);
        }

    }

    if (role == Qt::ForegroundRole ) {

        if(features.at(index.row())->getStation() != NULL){
            if(features.at(index.row())->getStation() == this->activeStation){
                return QColor(Qt::white);
            }
        }
    }

    return QVariant();
}

/*!
 * \brief TableModel::headerData
 * \param section
 * \param orientation
 * \param role
 * \return
 */
QVariant TableModel::headerData(int section, Qt::Orientation orientation, int role) const{

    QStringList m_columns;

    m_columns.append("Feature type");
    m_columns.append("Featurename");

    if(UnitConverter::distanceType == UnitConverter::eMETER){
        m_columns.append("X [m]");
        m_columns.append("Y [m]");
        m_columns.append("Z [m]");
    }

    if(UnitConverter::distanceType == UnitConverter::eMILLIMETER){
        m_columns.append("X [mm]");
        m_columns.append("Y [mm]");
        m_columns.append("Z [mm]");
    }

    m_columns.append("observations");
    m_columns.append("solved");
    m_columns.append("measurement config");
    m_columns.append("Functions");
    m_columns.append("isCommonPoint");
    m_columns.append("isNominal");

    if(UnitConverter::distanceType == UnitConverter::eMETER){
        m_columns.append("stddev [m]");
    }

    if(UnitConverter::distanceType == UnitConverter::eMILLIMETER){
        m_columns.append("stddev [mm]");
    }

    m_columns.append("I");
    m_columns.append("J");
    m_columns.append("K");

    if(UnitConverter::distanceType == UnitConverter::eMETER){
        m_columns.append("Radius [m]");
    }

    if(UnitConverter::distanceType == UnitConverter::eMILLIMETER){
        m_columns.append("Radius [mm]");
    }

    m_columns.append("scalar value [m]");
    m_columns.append("scalar value [dec. degree]");
    m_columns.append("start");
    m_columns.append("destination");

    if(UnitConverter::distanceType == UnitConverter::eMETER){
        m_columns.append("Translation X [m]");
    }
    if(UnitConverter::distanceType == UnitConverter::eMILLIMETER){
        m_columns.append("Translation X [m]");
    }

    if(UnitConverter::distanceType == UnitConverter::eMETER){
        m_columns.append("Translation Y [m]");
    }
    if(UnitConverter::distanceType == UnitConverter::eMILLIMETER){
        m_columns.append("Translation Y [m]");
    }

    if(UnitConverter::distanceType == UnitConverter::eMETER){
        m_columns.append("Translation Z [m]");
    }
    if(UnitConverter::distanceType == UnitConverter::eMILLIMETER){
        m_columns.append("Translation Z [m]");
    }

    if(UnitConverter::angleType == UnitConverter::eRADIANT){
        m_columns.append("Rotation X [rad]");
    }
    if(UnitConverter::angleType == UnitConverter::eGON){
        m_columns.append("Rotation X [gon]");
    }
    if(UnitConverter::angleType == UnitConverter::eDECIMALDEGREE){
        m_columns.append("Rotation X [dec. degree]");
    }

    if(UnitConverter::angleType == UnitConverter::eRADIANT){
        m_columns.append("Rotation Y [rad]");
    }
    if(UnitConverter::angleType == UnitConverter::eGON){
        m_columns.append("Rotation Y [gon]");
    }
    if(UnitConverter::angleType == UnitConverter::eDECIMALDEGREE){
        m_columns.append("Rotation Y [dec. degree]");
    }

    if(UnitConverter::angleType == UnitConverter::eRADIANT){
        m_columns.append("Rotation Z [rad]");
    }
    if(UnitConverter::angleType == UnitConverter::eGON){
        m_columns.append("Rotation Z [gon]");
    }
    if(UnitConverter::angleType == UnitConverter::eDECIMALDEGREE){
        m_columns.append("Rotation Z [dec. degree]");
    }

    m_columns.append("Scale X");
    m_columns.append("Scale Y");
    m_columns.append("Scale Z");
    m_columns.append(QString("Temperature " + UnitConverter::getTemperatureUnitString()));
    m_columns.append("Measurement series");

    if((Qt::DisplayRole == role) &&
            (Qt::Horizontal == orientation) &&
            (0 <= section) &&
            (section < columnCount())){

        return m_columns.at(section);
    }
    return QVariant();
}

/*!
 * \brief TableModel::updateModel
 */
void TableModel::updateModel(FeatureWrapper *fW, Station *sT){
    activeFeature = fW;
    activeStation = sT;
    emit layoutAboutToBeChanged();
    emit layoutChanged();
}
