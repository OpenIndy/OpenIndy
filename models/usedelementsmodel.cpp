#include "usedelementsmodel.h"

/*!
 * \brief UsedElementsModel::UsedElementsModel
 * \param parent
 */
UsedElementsModel::UsedElementsModel(QObject *parent) : QAbstractListModel(parent){
    this->resetSelectedFunctionPosition();
}

/*!
 * \brief UsedElementsModel::rowCount
 * \param parent
 * \return
 */
int UsedElementsModel::rowCount(const QModelIndex &parent) const{

    //check current job
    if(this->currentJob.isNull()){
        return 0;
    }

    //get and check active feature
    if(this->currentJob->getActiveFeature().isNull() || this->currentJob->getActiveFeature()->getFeature().isNull()){
        return 0;
    }
    Feature *feature = this->currentJob->getActiveFeature()->getFeature();

    //check selected function position
    if(this->functionPosition < 0 || this->functionPosition >= feature->getFunctions().size()
            || feature->getFunctions().at(this->functionPosition).isNull()){
        return 0;
    }
    Function *function = feature->getFunctions().at(this->functionPosition);

    //check input elements
    if(!function->getInputElements().contains(this->neededElementIndex)){
        return 0;
    }
    return function->getInputElements()[this->neededElementIndex].size();

}

/*!
 * \brief UsedElementsModel::columnCount
 * \param parent
 * \return
 */
int UsedElementsModel::columnCount(const QModelIndex &parent) const{
    return 1;
}

/*!
 * \brief UsedElementsModel::data
 * \param index
 * \param role
 * \return
 */
QVariant UsedElementsModel::data(const QModelIndex &index, int role) const{

    if(!index.isValid())
        return QVariant();

    //check current job
    if(this->currentJob.isNull()){
        return QVariant();
    }

    //get and check active feature
    if(this->currentJob->getActiveFeature().isNull() || this->currentJob->getActiveFeature()->getFeature().isNull()){
        return QVariant();
    }
    Feature *feature = this->currentJob->getActiveFeature()->getFeature();

    //check selected function position
    if(this->functionPosition < 0 || this->functionPosition >= feature->getFunctions().size()
            || feature->getFunctions().at(this->functionPosition).isNull()){
        return QVariant();
    }
    Function *function = feature->getFunctions().at(this->functionPosition);

    //check and get input element
    if(!function->getInputElements().contains(this->neededElementIndex)
            || index.row() >= function->getInputElements()[this->neededElementIndex].size()){
        return QVariant();
    }
    InputElement element = function->getInputElements()[this->neededElementIndex].at(index.row());

    if(role == Qt::DisplayRole){

        if(!element.geometry.isNull() && element.geometry->getIsNominal() && !element.geometry->getNominalSystem().isNull()){
            return QString("%1 - nominal (%3)").arg(element.geometry->getFeatureName()).arg(element.geometry->getNominalSystem()->getFeatureName());
        }else if(!element.geometry.isNull() && !element.geometry->getIsNominal()){
            return QString("%1 - actual").arg(element.geometry->getFeatureName());
        }else if(!element.station.isNull()){
            return element.station->getFeatureName();
        }else if(!element.coordSystem.isNull()){
            return element.coordSystem->getFeatureName();
        }else if(!element.trafoParam.isNull()){
            return element.trafoParam->getFeatureName();
        }else if(!element.observation.isNull() && !element.observation->getReading().isNull()){
            return element.observation->getReading()->getMeasuredAt().toString();
        }else if(!element.polarReading.isNull()){
            return element.polarReading->getMeasuredAt().toString();
        }else if(!element.directionReading.isNull()){
            return element.directionReading->getMeasuredAt().toString();
        }else if(!element.distanceReading.isNull()){
            return element.distanceReading->getMeasuredAt().toString();
        }else if(!element.cartesianReading.isNull()){
            return element.cartesianReading->getMeasuredAt().toString();
        }else if(!element.temperatureReading.isNull()){
            return element.temperatureReading->getMeasuredAt().toString();
        }else if(!element.levelReading.isNull()){
            return element.levelReading->getMeasuredAt().toString();
        }else if(!element.undefinedReading.isNull()){
            return element.undefinedReading->getMeasuredAt().toString();
        }
        return QVariant();

    }else if(role == Qt::DecorationRole){

        QPixmap pix(getElementTypeIconPath(element.typeOfElement));
        return pix.scaledToHeight(20, Qt::SmoothTransformation);

    }

    return QVariant();

}

/*!
 * \brief UsedElementsModel::headerData
 * \param section
 * \param orientation
 * \param role
 * \return
 */
QVariant UsedElementsModel::headerData(int section, Qt::Orientation orientation, int role) const{
    if((Qt::DisplayRole == role) &&
            (Qt::Horizontal == orientation) &&
            (0 <= section) &&
            (section < 1)){

        return "used elements";
    }
    return QVariant();
}

/*!
 * \brief UsedElementsModel::removeUsedElements
 * \param selection
 */
void UsedElementsModel::removeUsedElements(const QModelIndexList &selection){

    //check current job
    if(this->currentJob.isNull()){
        return;
    }

    //get and check active feature
    if(this->currentJob->getActiveFeature().isNull() || this->currentJob->getActiveFeature()->getFeature().isNull()){
        return;
    }
    Feature *feature = this->currentJob->getActiveFeature()->getFeature();

    //check selected function position
    if(this->functionPosition < 0 || this->functionPosition >= feature->getFunctions().size()
            || feature->getFunctions().at(this->functionPosition).isNull()){
        return;
    }
    Function *function = feature->getFunctions().at(this->functionPosition);

    //check and get input elements
    if(!function->getInputElements().contains(this->neededElementIndex)){
        return;
    }
    QList<InputElement> inputElements = function->getInputElements().value(this->neededElementIndex);

    //remove all selected elements
    foreach(const QModelIndex &index, selection){

        //check index
        if(!index.isValid() || index.row() >= inputElements.size()){
            return;
        }

        emit this->startRemoveUsedElements(this->currentJob->getActiveFeature(), this->functionPosition,
                                           this->neededElementIndex, inputElements.at(index.row()).id);

    }

}

/*!
 * \brief UsedElementsModel::getCurrentJob
 * \return
 */
const QPointer<OiJob> &UsedElementsModel::getCurrentJob() const{
    return this->currentJob;
}

/*!
 * \brief UsedElementsModel::setCurrentJob
 * \param job
 */
void UsedElementsModel::setCurrentJob(const QPointer<OiJob> &job){
    if(!job.isNull()){

        //disconnect current job
        if(!this->currentJob.isNull()){
            this->disconnectJob();
        }

        this->currentJob = job;
        this->connectJob();
        this->updateModel();

    }
}

/*!
 * \brief UsedElementsModel::setSelectedFunctionPosition
 * \param functionPosition
 * \param neededElementIndex
 */
void UsedElementsModel::setSelectedFunctionPosition(const int &functionPosition, const int &neededElementIndex){

    //reset old function position
    this->resetSelectedFunctionPosition();

    //set function index and input element index
    this->functionPosition = functionPosition;
    this->neededElementIndex = neededElementIndex;

    //get and check active feature
    if(this->currentJob->getActiveFeature().isNull() || this->currentJob->getActiveFeature()->getFeature().isNull()){
        return;
    }
    Feature *feature = this->currentJob->getActiveFeature()->getFeature();

    //check selected function position
    if(this->functionPosition < 0 || this->functionPosition >= feature->getFunctions().size()
            || feature->getFunctions().at(this->functionPosition).isNull()){
        return;
    }
    this->connectedFunction = feature->getFunctions().at(this->functionPosition);

    //connect the specified function
    QObject::connect(this->connectedFunction.data(), &Function::inputElementsChanged, this, &UsedElementsModel::updateModel, Qt::AutoConnection);

    this->updateModel();

}

/*!
 * \brief UsedElementsModel::updateModel
 */
void UsedElementsModel::updateModel(){
    emit this->layoutAboutToBeChanged();
    emit this->layoutChanged();
}

/*!
 * \brief UsedElementsModel::connectJob
 */
void UsedElementsModel::connectJob(){

    QObject::connect(this->currentJob.data(), &OiJob::activeFeatureChanged, this, &UsedElementsModel::resetSelectedFunctionPosition, Qt::AutoConnection);
    QObject::connect(this->currentJob.data(), &OiJob::activeFeatureChanged, this, &UsedElementsModel::updateModel, Qt::AutoConnection);

    QObject::connect(this, &UsedElementsModel::startRemoveUsedElements, this->currentJob.data(), &OiJob::removeInputElement, Qt::AutoConnection);

}

/*!
 * \brief UsedElementsModel::disconnectJob
 */
void UsedElementsModel::disconnectJob(){

    QObject::disconnect(this->currentJob.data(), &OiJob::activeFeatureChanged, this, &UsedElementsModel::resetSelectedFunctionPosition);
    QObject::disconnect(this->currentJob.data(), &OiJob::activeFeatureChanged, this, &UsedElementsModel::updateModel);

    QObject::disconnect(this, &UsedElementsModel::startRemoveUsedElements, this->currentJob.data(), &OiJob::removeInputElement);

}

/*!
 * \brief UsedElementsModel::resetSelectedFunctionPosition
 * Reset the model
 */
void UsedElementsModel::resetSelectedFunctionPosition(){

    //reset function index and input element index
    this->functionPosition = -1;
    this->neededElementIndex = -1;

    //disconnect function
    if(!this->connectedFunction.isNull()){
        QObject::disconnect(this->connectedFunction.data(), &Function::inputElementsChanged, this, &UsedElementsModel::updateModel);
        this->connectedFunction = QPointer<Function>(NULL);
    }

}
