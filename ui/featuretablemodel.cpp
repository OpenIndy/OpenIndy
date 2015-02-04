#include "featuretablemodel.h"

/*!
 * \brief TableModel::TableModel
 * \param features
 * \param coordSys
 * \param parent
 */
FeatureTableModel::FeatureTableModel(QObject *parent) : QAbstractTableModel(parent)
{

}

/*!
 * \brief TableModel::rowCount
 * \return
 */
int FeatureTableModel::rowCount(const QModelIndex& ) const{
    if(OiFeatureState::getFeatureCount() > 0){
        return OiFeatureState::getFeatureCount();
    }
    return 0;
}

/*!
 * \brief TableModel::columnCount
 * \param parent
 * \return
 */
int FeatureTableModel::columnCount(const QModelIndex &parent) const{
    return GUIConfiguration::allAttributes.size();
}

/*!
 * \brief TableModel::data
 * Displays the in the model specified data of all existing features in the tableview.
 * \param index
 * \param role
 * \return
 */
QVariant FeatureTableModel::data(const QModelIndex &index, int role) const{
    try{

        //check model index
        if(!index.isValid()){
            return QVariant();
        }

        QString functions = "";

        //get the feature to display at row index.row()
        FeatureWrapper *currentFeature = NULL;
        if(OiFeatureState::getFeatureCount() > index.row()){
            currentFeature = OiFeatureState::getFeatures().at(index.row());
        }

		//check the feature
        if(currentFeature == NULL || currentFeature->getFeature() == NULL){
            return QVariant();
        }
		
        if(currentFeature != NULL && currentFeature->getFeature() != NULL){

            //neu
            if(Qt::DisplayRole == role){

                switch (index.column()) {
                case 0://type
                    return currentFeature->returnFeatureType();
                case 1://act/nom
                    return currentFeature->getFeature()->getDisplayIsNominal();
                case 2://group
                    return currentFeature->getFeature()->getGroupName();
                case 3://name
                    return currentFeature->getFeature()->getFeatureName();
                case 4://x
                    if(currentFeature->getStation() != NULL && currentFeature->getStation()->coordSys->getIsSolved()){
                        return currentFeature->getFeature()->getDisplayX(GUIConfiguration::getShowDifferences());
                    }else if(currentFeature->getFeature()->getIsSolved()){
                        return currentFeature->getFeature()->getDisplayX(GUIConfiguration::getShowDifferences());
                    }else{
                        return QVariant();
                    }
                case 5://y
                    if(currentFeature->getStation() != NULL && currentFeature->getStation()->coordSys->getIsSolved()){
                        return currentFeature->getFeature()->getDisplayY(GUIConfiguration::getShowDifferences());
                    }else if(currentFeature->getFeature()->getIsSolved()){
                        return currentFeature->getFeature()->getDisplayY(GUIConfiguration::getShowDifferences());
                    }else{
                        return QVariant();
                    }
                case 6://z
                    if(currentFeature->getStation() != NULL && currentFeature->getStation()->coordSys->getIsSolved()){
                        return currentFeature->getFeature()->getDisplayZ(GUIConfiguration::getShowDifferences());
                    }else if(currentFeature->getFeature()->getIsSolved()){
                        return currentFeature->getFeature()->getDisplayZ(GUIConfiguration::getShowDifferences());
                    }else{
                        return QVariant();
                    }
                case 7://stddev
                    return currentFeature->getFeature()->getDisplayStdDev();
                case 8://obs
                    return currentFeature->getFeature()->getDisplayObs();
                case 9://mconfig
                    return currentFeature->getFeature()->getDisplayMConfig();
                case 10://function
                    if(currentFeature->getFeature()->getFunctions().size() == 0){
                        return "no function set";
                    }else{
                        functions += currentFeature->getFeature()->getFunctions().at(0)->getMetaData()->name;
                        for(int i=1;i<currentFeature->getFeature()->getFunctions().size();i++){
                            functions += "," + currentFeature->getFeature()->getFunctions().at(i)->getMetaData()->name;
                        }
                        return functions;
                    }
                case 11://solved
                    return currentFeature->getFeature()->getDisplaySolved();
                case 12://comment
                    return currentFeature->getFeature()->getComment();
                case 13://radius
                    if(currentFeature->getFeature()->getIsSolved()){
                        return currentFeature->getFeature()->getDisplayRadius(GUIConfiguration::getShowDifferences());
                    }else{
                        return QVariant();
                    }
                case 14://i
                    if(currentFeature->getFeature()->getIsSolved()){
                        return currentFeature->getFeature()->getDisplayI(GUIConfiguration::getShowDifferences());
                    }else{
                        return QVariant();
                    }
                case 15://j
                    if(currentFeature->getFeature()->getIsSolved()){
                        return currentFeature->getFeature()->getDisplayJ(GUIConfiguration::getShowDifferences());
                    }else{
                        return QVariant();
                    }
                case 16://k
                    if(currentFeature->getFeature()->getIsSolved()){
                        return currentFeature->getFeature()->getDisplayK(GUIConfiguration::getShowDifferences());
                    }else{
                        return QVariant();
                    }
                case 17://com point
                    return currentFeature->getFeature()->getDisplayIsCommon();
                case 18://scalar value dist
                    if(currentFeature->getFeature()->getIsSolved()){
                        return currentFeature->getFeature()->getDisplayScalarDistanceValue();
                    }else{
                        return QVariant();
                    }
                case 19://scalar value rad
                    if(currentFeature->getFeature()->getIsSolved()){
                        return currentFeature->getFeature()->getDisplayScalarAngleValue();
                    }else{
                        return QVariant();
                    }
                case 20://temperature
                    return currentFeature->getFeature()->getDisplayScalarTemperatureValue();
                case 21://measurement series
                    return currentFeature->getFeature()->getDisplayScalarMeasurementSeriesValue();
                case 22://expansion origin x
                   return currentFeature->getFeature()->getDisplayExpansionOriginX();
                case 23://expansion origin y
                    return currentFeature->getFeature()->getDisplayExpansionOriginY();
                case 24://expansion origin z
                    return currentFeature->getFeature()->getDisplayExpansionOriginZ();
                case 25://use
                    if(currentFeature->getTrafoParam() != NULL){
                        return currentFeature->getTrafoParam()->getIsUsed();
                    }
                    return QVariant();
                case 26://datum trafo
                    if(currentFeature->getTrafoParam() != NULL){
                        return currentFeature->getTrafoParam()->getisDatumTrafo();
                    }
                    return QVariant();
                case 27://start system
                    return currentFeature->getFeature()->getDisplayStartSystem();
                case 28://dest system
                    return currentFeature->getFeature()->getDisplayDestinationSystem();
                case 29://tx
                    return currentFeature->getFeature()->getDisplayTranslationX();
                case 30://ty
                    return currentFeature->getFeature()->getDisplayTranslationY();
                case 31://tz
                    return currentFeature->getFeature()->getDisplayTranslationZ();
                case 32://rx
                    return currentFeature->getFeature()->getDisplayRotationX();
                case 33://ry
                    return currentFeature->getFeature()->getDisplayRotationY();
                case 34://rz
                    return currentFeature->getFeature()->getDisplayRotationZ();
                case 35://sx
                    return currentFeature->getFeature()->getDisplayScaleX();
                case 36://sy
                    return currentFeature->getFeature()->getDisplayScaleY();
                case 37://sz
                    return currentFeature->getFeature()->getDisplayScaleZ();
                case 38://time
                    if(currentFeature->getTrafoParam() != NULL){
                        return currentFeature->getTrafoParam()->getValidTime();
                    }
                    return QVariant();
                default:
                    break;
                }
			}
		
			//background role
			if(role == Qt::BackgroundRole){

				//active feature
				if (currentFeature->getFeature()->getIsActiveFeature()){
					return QColor(QColor::fromCmykF(0.59,0.40,0.10,0.10).lighter());
				}

				//active station
				if(currentFeature->getTypeOfFeature() == Configuration::eStationFeature
						&& currentFeature->getStation()->getIsActiveStation()){
					return QColor(Qt::darkGray);
				}

				//non active station
				if (currentFeature->getTypeOfFeature() == Configuration::eStationFeature
						&& !currentFeature->getStation()->getIsActiveStation()){
					return QColor(Qt::lightGray);
				}

				//not solved
				if( (index.column() == 4 || index.column() == 5 || index.column() == 6
						|| index.column() == 13 || index.column() == 14
						|| index.column() == 15 || index.column() == 16
						|| index.column() == 18 || index.column() == 19
						|| index.column() == 20 || index.column() == 21)
						&& !currentFeature->getFeature()->getIsSolved()){
					return QColor(Qt::yellow);
				}

				//nominal
				if(currentFeature->getGeometry() != NULL && currentFeature->getGeometry()->getIsNominal()){
					return QColor(QColor::fromRgb(230,230,180));
				}

				return QVariant();

			}

			//foreground role
			if(role == Qt::ForegroundRole){

				//active station
				if(currentFeature->getTypeOfFeature() == Configuration::eStationFeature
						&& currentFeature->getStation()->getIsActiveStation()){
					return QColor(Qt::white);
				}

				return QVariant();

			}
		}
        return QVariant();

    }catch(const exception &e){
        Console::addLine(e.what());
        return QVariant();
    }
}

/*!
 * \brief TableModel::headerData
 * \param section
 * \param orientation
 * \param role
 * \return
 */
QVariant FeatureTableModel::headerData(int section, Qt::Orientation orientation, int role) const{

    QStringList m_columns = GUIConfiguration::allAttributes;

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
void FeatureTableModel::updateModel(){
    emit layoutAboutToBeChanged();
    emit layoutChanged();
    //emit resizeTable();
}

/*!
 * \brief TableModel::flags
 * Add edit-functionality to some cells of tablemodel
 * \param index
 * \return
 */
Qt::ItemFlags FeatureTableModel::flags(const QModelIndex & index) const{
    Qt::ItemFlags myFlags = QAbstractTableModel::flags(index);
    return (myFlags | Qt::ItemIsEditable);
}

/*!
 * \brief TableModel::setData
 * Update the edited features
 * \param index
 * \param value
 * \param role
 * \return
 */
bool FeatureTableModel::setData(const QModelIndex & index, const QVariant & value, int role){

    //get the active feature
    FeatureWrapper *myFeature = OiFeatureState::getActiveFeature();
    if(myFeature == NULL || myFeature->getFeature() == NULL){
        return false;
    }

    if(index.column() == 3){ //feature name

        //get attributes necessary for name validation
        bool isNominal = false;
        CoordinateSystem *nominalSystem = NULL;
        if(myFeature->getGeometry() != NULL){
            isNominal = myFeature->getGeometry()->getIsNominal();
            nominalSystem = myFeature->getGeometry()->getNominalSystem();
        }

        //check if the feature name is ok
        /*if(!OiFeatureState::validateFeatureName(myFeature->getTypeOfFeature(), value.toString(),
                                               isNominal, nominalSystem)){
            return false;
        }*/

        //if active feature is a geometry then corresponding nominals have to be renamed, too
        if(myFeature->getGeometry() != NULL){
            if(myFeature->getGeometry()->getIsNominal() && myFeature->getGeometry()->getMyActual() != NULL){
                myFeature->getGeometry()->getMyActual()->setFeatureName(value.toString());
                foreach(Geometry *nomGeom, myFeature->getGeometry()->getMyActual()->getMyNominals()){
                    if(nomGeom != NULL){
                        nomGeom->setFeatureName(value.toString());
                    }
                }
            }else{
                myFeature->getGeometry()->setFeatureName(value.toString());
                foreach(Geometry *nomGeom, myFeature->getGeometry()->getMyNominals()){
                    if(nomGeom != NULL){
                        nomGeom->setFeatureName(value.toString());
                    }
                }
            }
        }else{
            myFeature->getGeometry()->setFeatureName(value.toString());
        }

    }else if(index.column() == 2){ //feature group

        QString oldValue = myFeature->getFeature()->getGroupName();
        myFeature->getFeature()->setGroupName(value.toString());
        emit this->groupNameChanged(oldValue, value.toString());

    }else if(index.column() == 12){ //feature comment

        myFeature->getFeature()->setComment(value.toString());

    }else if(index.column() == 25){ //trafo param use

        if(myFeature->getTrafoParam() == NULL){
            return false;
        }
        myFeature->getTrafoParam()->setIsUsed(value.toBool());

    }else if(index.column() == 38){ //trafo param time

        if(myFeature->getTrafoParam() == NULL){
            return false;
        }
        myFeature->getTrafoParam()->setValidTime(value.toDateTime());

    }else if(index.column() == 26){ //trafo param datum transformation

        if(myFeature->getTrafoParam() == NULL){
            return false;
        }
        myFeature->getTrafoParam()->setisDatumTrafo(value.toBool());

    }else if(index.column() == 22){ //expansion origin x

        if(myFeature->getCoordinateSystem() == NULL){
            return false;
        }
        //myFeature->getCoordinateSystem()->setExpansionOriginX(value.toDouble()/OiUnitConverter::getDistanceMultiplier());

    }else if(index.column() == 23){ //expansion origin y

        if(myFeature->getCoordinateSystem() == NULL){
            return false;
        }
        //myFeature->getCoordinateSystem()->setExpansionOriginY(value.toDouble()/OiUnitConverter::getDistanceMultiplier());

    }else if(index.column() == 24){ //expansion origin z

        if(myFeature->getCoordinateSystem() == NULL){
            return false;
        }
        //myFeature->getCoordinateSystem()->setExpansionOriginZ(value.toDouble()/OiUnitConverter::getDistanceMultiplier());

    }

    this->updateModel();

    return true;

}
