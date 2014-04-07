#include "feature.h"

#include "function.h"
#include "featurewrapper.h"

Feature::~Feature(){
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
