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

    NeededElement neededElement = function->getNeededElements().at(this->neededElementIndex);
    const int inputElementKey = neededElement.key > InputElementKey::eNotSet ? neededElement.key : this->neededElementIndex;

    //check input elements
    if(!function->getInputElements().contains(inputElementKey)){
        return 0;
    }
    return function->getInputElements()[inputElementKey].size();

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

    NeededElement neededElement = function->getNeededElements().at(this->neededElementIndex);
    const int inputElementKey = neededElement.key > InputElementKey::eNotSet ? neededElement.key : this->neededElementIndex;
    //check and get input element
    if(!function->getInputElements().contains(inputElementKey)
            || index.row() >= function->getInputElements()[inputElementKey].size()){
        return QVariant();
    }
    InputElement element = function->getInputElements()[inputElementKey].at(index.row());

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

            if(!element.observation->getMeasuredTargetGeometry().isNull()){
                return "id:" + QString::number(element.observation->getId()) + " " + element.observation->getMeasuredTargetGeometry()->getFeatureName();
            }
            return "id:" + QString::number(element.observation->getId()) + " " + element.observation->getStation()->getFeatureName();

        }else if(!element.polarReading.isNull()){

            if(!element.polarReading->getObservation()->getMeasuredTargetGeometry().isNull()){
                return "id:" + QString::number(element.polarReading->getId()) + element.polarReading->getObservation()->getMeasuredTargetGeometry()->getFeatureName();
            }
            return "id:" + QString::number(element.polarReading->getId()) + element.polarReading->getObservation()->getStation()->getFeatureName();

        }else if(!element.directionReading.isNull()){

            if(!element.directionReading->getObservation()->getMeasuredTargetGeometry().isNull()){
                return "id:" + QString::number(element.directionReading->getId()) + element.directionReading->getObservation()->getMeasuredTargetGeometry()->getFeatureName();
            }
            return "id:" + QString::number(element.directionReading->getId()) + element.directionReading->getObservation()->getStation()->getFeatureName();

        }else if(!element.distanceReading.isNull()){

            if(!element.distanceReading->getObservation()->getMeasuredTargetGeometry().isNull()){
                return "id:" + QString::number(element.distanceReading->getId()) + element.distanceReading->getObservation()->getMeasuredTargetGeometry()->getFeatureName();
            }
            return "id:" + QString::number(element.distanceReading->getId()) + element.distanceReading->getObservation()->getStation()->getFeatureName();

        }else if(!element.cartesianReading.isNull()){

            if(!element.cartesianReading->getObservation()->getMeasuredTargetGeometry().isNull()){
                return "id:" + QString::number(element.cartesianReading->getId()) + element.cartesianReading->getObservation()->getMeasuredTargetGeometry()->getFeatureName();
            }
            return "id:" + QString::number(element.cartesianReading->getId()) + element.cartesianReading->getObservation()->getStation()->getFeatureName();

        }else if(!element.temperatureReading.isNull()){

            if(!element.temperatureReading->getObservation()->getMeasuredTargetGeometry().isNull()){
                return "id:" + QString::number(element.temperatureReading->getId()) + element.temperatureReading->getObservation()->getMeasuredTargetGeometry()->getFeatureName();
            }
            return "id:" + QString::number(element.temperatureReading->getId()) + element.temperatureReading->getObservation()->getStation()->getFeatureName();

        }else if(!element.levelReading.isNull()){

            if(!element.levelReading->getObservation()->getMeasuredTargetGeometry().isNull()){
                return "id:" + QString::number(element.levelReading->getId()) + element.levelReading->getObservation()->getMeasuredTargetGeometry()->getFeatureName();
            }
            return "id:" + QString::number(element.levelReading->getId()) + element.levelReading->getObservation()->getStation()->getFeatureName();

        }else if(!element.undefinedReading.isNull()){

            if(!element.undefinedReading->getObservation()->getMeasuredTargetGeometry().isNull()){
                return "id:" + QString::number(element.undefinedReading->getId()) + element.undefinedReading->getObservation()->getMeasuredTargetGeometry()->getFeatureName();
            }
            return "id:" + QString::number(element.undefinedReading->getId()) + element.undefinedReading->getObservation()->getStation()->getFeatureName();

        }
        return QVariant();

    }else if(role == Qt::DecorationRole){

        //get and check icon path
        const QString &iconPath = getElementTypeIconPath(element.typeOfElement);
        if(iconPath.compare("") == 0){
            return QVariant();
        }

        QPixmap pix(iconPath);
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

    NeededElement neededElement = function->getNeededElements().at(this->neededElementIndex);
    const int inputElementKey = neededElement.key > InputElementKey::eNotSet ? neededElement.key : this->neededElementIndex;
    //check and get input elements
    if(!function->getInputElements().contains(inputElementKey)){
        return;
    }

    QList<InputElement> inputElements = function->getInputElements().value(inputElementKey);

    //remove all selected elements
    foreach(const QModelIndex &index, selection){

        //check index
        if(!index.isValid() || index.row() >= inputElements.size()){
            return;
        }

        emit this->startRemoveUsedElements(this->currentJob->getActiveFeature(), this->functionPosition,
                                           inputElementKey, inputElements.at(index.row()).id);

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
