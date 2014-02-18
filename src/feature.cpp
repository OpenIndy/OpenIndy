#include "feature.h"

#include "function.h"
#include "featurewrapper.h"

Feature::~Feature(){

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
