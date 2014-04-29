#include "feature.h"

#include "function.h"
#include "featurewrapper.h"

Feature::~Feature(){

    //check and update features which need this feature to recalc
    if(this->usedFor.size() > 0){
        foreach(FeatureWrapper *myDependent, this->usedFor){
            Feature *dependentFeature = myDependent->getFeature();
            if(dependentFeature != NULL){

                //delete the feature from all functions
                for(int i = 0; i < dependentFeature->functionList.size(); i++){
                    Function *myFunc = dependentFeature->functionList.at(i);
                    if(myFunc != NULL){
                        myFunc->removeFeature(this->id);
                    }
                }
                int index = -1;
                int i = 0;
                foreach(FeatureWrapper *prev, dependentFeature->previouslyNeeded){
                    if(prev != NULL && prev->getFeature() != NULL && prev->getFeature()->id == this->id){
                        index = i;
                        break;
                    }
                    i++;
                }
                if(index >= 0){
                    dependentFeature->previouslyNeeded.removeAt(index);
                }

            }
        }
    }

    //check and update features which were needed to recalc this feature
    if(this->previouslyNeeded.size() > 0){
        foreach(FeatureWrapper *myDependent, this->previouslyNeeded){
            Feature *dependentFeature = myDependent->getFeature();
            if(dependentFeature != NULL){

                //delete the feature from all functions
                int index = -1;
                int i = 0;
                foreach(FeatureWrapper *prev, dependentFeature->usedFor){
                    if(prev != NULL && prev->getFeature() != NULL && prev->getFeature()->id == this->id){
                        index = i;
                        break;
                    }
                    i++;
                }
                if(index >= 0){
                    dependentFeature->usedFor.removeAt(index);
                }

            }
        }
    }

    //delete all functions when deleting the feature
    for(int i = 0; i < this->functionList.size(); i++){
        delete this->functionList.at(i);
    }

}

/*!
 * \brief Feature::addFunction
 * \param f
 */
void Feature::addFunction(Function *f){
    if(f != NULL){
        this->functionList.append(f);
    }
}
