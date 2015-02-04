#include "functionstatistic.h"

/*!
 * \brief FunctionStatistic constructor
 * \param parent
 */
FunctionStatistic::FunctionStatistic(QObject *parent ) :
    QAbstractTableModel(parent)
{
    this->selectedFunction = NULL;
}

/*!
 * \brief rowCount function for calculating the number of rows by checking the number of residuals of the function statistic object.
 * \param parent
 * \return
 */
int FunctionStatistic::rowCount(const QModelIndex &parent) const{
    if(this->selectedFunction != NULL){
        return this->selectedFunction->getStatistic().displayResiduals.size();
    }else{
        return 0;
    }

}

/*!
 * \brief columnCount function that returns the number of columns by checking the number of residual attibutes (e.g. vx vy vz)
 * \param parent
 * \return
 */
int FunctionStatistic::columnCount(const QModelIndex &parent) const{

    return this->residualName.size();

}

/*!
 * \brief data function that displays the residual values for eacht residual.
 * \param index
 * \param role
 * \return
 */
QVariant FunctionStatistic::data(const QModelIndex &index, int role) const{

    if(this->ElementIDs.size() == 0){
        return QVariant();
    }

    if(this->selectedFunction != NULL){

        if(!index.isValid())
            return QVariant();

        if(Qt::DisplayRole == role){

            if(this->selectedFunction->getMetaData()->iid == OiMetaData::iid_FitFunction){

                QString keyName = this->originalResidualName.at(index.column());

                if(index.column() == 0){
                    return this->ElementIDs.at(index.row());
                }else{
                    /*if(this->selectedFunction->getStatistic().displayResiduals.at(index.row()).residualUnitType.value(keyName) == OiUnitConverter::eMetric){
                        double value = (double) this->selectedFunction->getStatistic().displayResiduals.at(index.row()).residual.value(keyName);
                        return QString::number(value*OiUnitConverter::getDistanceMultiplier(),'f',OiUnitConverter::distanceDigits);
                    }
                    if(this->selectedFunction->getStatistic().displayResiduals.at(index.row()).residualUnitType.value(keyName) == OiUnitConverter::eAngular){
                        double value = (double) this->selectedFunction->getStatistic().displayResiduals.at(index.row()).residual.value(keyName);
                        return QString::number(value*OiUnitConverter::getAngleMultiplier(),'f',OiUnitConverter::angleDigits);
                    }
                    if(this->selectedFunction->getStatistic().displayResiduals.at(index.row()).residualUnitType.value(keyName) == OiUnitConverter::eTemperature){
                        double value = (double) this->selectedFunction->getStatistic().displayResiduals.at(index.row()).residual.value(keyName);
                        return QString::number(OiUnitConverter::getTemperature(value),'f',OiUnitConverter::temperatureDigits);
                    }*/
                }
            }
        }
    }

    return QVariant();
}

/*!
 * \brief headerData function for specifying the names of the columns.
 * \param section
 * \param orientation
 * \param role
 * \return
 */
QVariant FunctionStatistic::headerData(int section, Qt::Orientation orientation, int role) const{

    if(this->selectedFunction != NULL && residualName.size() > 0){

        if((Qt::DisplayRole == role) &&
                (Qt::Horizontal == orientation) &&
                (0 <= section) &&
                (section < columnCount())){

            return residualName.at(section);
        }

    }

    return QVariant();
}

/*!
 * \brief updateModel updates the view on the fly.
 */
void FunctionStatistic::updateModel(){
    emit layoutAboutToBeChanged();
    emit layoutChanged();
}

/*!
 * \brief getKeys function queries the names of the residual attributes (vx v vz) of the function statistic object.
 */
void FunctionStatistic::getKeys(){

    /*if(this->selectedFunction != NULL){

        this->residualName.clear();
        this->originalResidualName.clear();

        this->residualName.append("element id");
        this->originalResidualName.append("element id");

        QList<Residual> r = this->selectedFunction->getStatistic().displayResiduals;

        for(int k = 0; k<r.size();k++){

            for(int i=0; i<r.at(k).residualName.size();i++){

                if(r.at(k).residualUnitType.value(r.at(k).residualName.at(i)) == OiUnitConverter::eMetric){
                    if(!this->residualName.contains(QString(r.at(k).residualName.at(i)+  OiUnitConverter::getDistanceUnitString()))){
                        this->residualName.append(QString(r.at(k).residualName.at(i) + OiUnitConverter::getDistanceUnitString()));
                        this->originalResidualName.append(QString(r.at(k).residualName.at(i)));
                    }

                }
                if(r.at(k).residualUnitType.value(r.at(k).residualName.at(i)) == OiUnitConverter::eAngular){
                    if(!this->residualName.contains(QString(r.at(k).residualName.at(i) + OiUnitConverter::getAngleUnitString()))){
                        this->residualName.append(QString(r.at(k).residualName.at(i) + OiUnitConverter::getAngleUnitString()));
                        this->originalResidualName.append(QString(r.at(k).residualName.at(i)));
                    }

                }
                if(r.at(k).residualUnitType.value(r.at(k).residualName.at(i)) == OiUnitConverter::eTemperature){
                    if(!this->residualName.contains(QString(r.at(k).residualName.at(i) + OiUnitConverter::getTemperatureUnitString()))){
                        this->residualName.append(QString(r.at(k).residualName.at(i) + OiUnitConverter::getTemperatureUnitString()));
                        this->originalResidualName.append(QString(r.at(k).residualName.at(i)));
                    }

                }
            }
        }
    }else{
        this->residualName.clear();
        this->originalResidualName.clear();
    }*/
}

/*!
 * \brief setFunction function sets the Function pointer of the class, clears all lists and calls getKeys and getIDs
 * \param Function *f
 */
void FunctionStatistic::setFunction(Function *f){
    this->selectedFunction = f;
    this->residualName.clear();
    this->originalResidualName.clear();
    this->ElementIDs.clear();
    this->getKeys();
    this->getIDs();
}

/*!
 * \brief getIDs querys the feature ids of the function by iterating the QMap getFeatureOrder
 */
void FunctionStatistic::getIDs(){

    if(this->selectedFunction != NULL){

        this->ElementIDs.clear();

        QMapIterator<int, QList<InputFeature> > i(this->selectedFunction->getFeatureOrder());
        while (i.hasNext()) {
            i.next();
            for(int k=0; k<i.value().size();k++){
                if(i.value().at(k).isUsed){
                    this->ElementIDs.append(i.value().at(k).id);
                }
            }
        }
    }else{
        this->ElementIDs.clear();
    }

}
