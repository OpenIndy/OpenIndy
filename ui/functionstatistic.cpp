#include "functionstatistic.h"

FunctionStatistic::FunctionStatistic(QObject *parent ) :
    QAbstractTableModel(parent)
{
    this->selectedFunction = NULL;
}

int FunctionStatistic::rowCount(const QModelIndex &parent) const{
    if(this->selectedFunction != NULL){
        return this->selectedFunction->getStatistic().displayResiduals.size();
    }else{
        return 0;
    }

}

int FunctionStatistic::columnCount(const QModelIndex &parent) const{

    return this->residualName.size();

}

QVariant FunctionStatistic::data(const QModelIndex &index, int role) const{

    if(this->selectedFunction != NULL){

        if(!index.isValid())
            return QVariant();

        if(Qt::DisplayRole == role){

            if(this->selectedFunction->getMetaData()->iid == OiMetaData::iid_FitFunction){

                QString keyName = this->originalResidualName.at(index.column());

                if(index.column() == 0){
                    return this->ElementIDs.at(index.row());
                }else{
                    if(this->selectedFunction->getStatistic().displayResiduals.at(index.row()).residualUnitType.value(keyName) == UnitConverter::eMetric){
                        double value = (double) this->selectedFunction->getStatistic().displayResiduals.at(index.row()).residual.value(keyName);
                        return QString::number(value*UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits);
                    }
                    if(this->selectedFunction->getStatistic().displayResiduals.at(index.row()).residualUnitType.value(keyName) == UnitConverter::eAngular){
                        double value = (double) this->selectedFunction->getStatistic().displayResiduals.at(index.row()).residual.value(keyName);
                        return QString::number(value*UnitConverter::getAngleMultiplier(),'f',UnitConverter::angleDigits);
                    }
                    if(this->selectedFunction->getStatistic().displayResiduals.at(index.row()).residualUnitType.value(keyName) == UnitConverter::eTemperature){
                        double value = (double) this->selectedFunction->getStatistic().displayResiduals.at(index.row()).residual.value(keyName);
                        return QString::number(value*UnitConverter::getTemperatureMultiplier(),'f',UnitConverter::temperatureDigits);
                    }
                }
            }
        }
    }

    return QVariant();
}

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

void FunctionStatistic::updateModel(){
    emit layoutAboutToBeChanged();
    emit layoutChanged();
}

void FunctionStatistic::getKeys(){

    if(this->selectedFunction != NULL){

        this->residualName.clear();
        this->originalResidualName.clear();

        this->residualName.append("element id");
        this->originalResidualName.append("element id");

        QList<Residual> r = this->selectedFunction->getStatistic().displayResiduals;

        for(int k = 0; k<r.size();k++){

            for(int i=0; i<r.at(k).residualName.size();i++){

                if(r.at(k).residualUnitType.value(r.at(k).residualName.at(i)) == UnitConverter::eMetric){
                    if(!this->residualName.contains(QString(r.at(k).residualName.at(i)+ " " + UnitConverter::getDistanceUnitString()))){
                        this->residualName.append(QString(r.at(k).residualName.at(i) + " " + UnitConverter::getDistanceUnitString()));
                        this->originalResidualName.append(QString(r.at(k).residualName.at(i)));
                    }

                }
                if(r.at(k).residualUnitType.value(r.at(k).residualName.at(i)) == UnitConverter::eAngular){
                    if(!this->residualName.contains(QString(r.at(k).residualName.at(i) + " " + UnitConverter::getAngleUnitString()))){
                        this->residualName.append(QString(r.at(k).residualName.at(i) + " " + UnitConverter::getAngleUnitString()));
                        this->originalResidualName.append(QString(r.at(k).residualName.at(i)));
                    }

                }
                if(r.at(k).residualUnitType.value(r.at(k).residualName.at(i)) == UnitConverter::eTemperature){
                    if(!this->residualName.contains(QString(r.at(k).residualName.at(i) + " " + UnitConverter::getTemperatureUnitString()))){
                        this->residualName.append(QString(r.at(k).residualName.at(i) + " " + UnitConverter::getTemperatureUnitString()));
                        this->originalResidualName.append(QString(r.at(k).residualName.at(i)));
                    }

                }
            }
        }
    }else{
        this->residualName.clear();
        this->originalResidualName.clear();
    }
}

void FunctionStatistic::setFunction(Function *f){
    this->selectedFunction = f;
    this->residualName.clear();
    this->originalResidualName.clear();
    this->ElementIDs.clear();
    this->getKeys();
    this->getIDs();
}

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
