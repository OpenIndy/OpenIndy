#include "chooselalib.h"

#include "laarmadillo.h"

/*!
 * \brief ChooseLALib::setLinearAlgebra
 * Decide which linear algebra library to use
 * \param lib
 */
void ChooseLALib::setLinearAlgebra(LinearAlgebraLibrary lib){
    switch(lib){
    case(Armadillo):
        OiVec::myLinearAlgebra = new LAArmadillo();
        OiMat::myLinearAlgebra = new LAArmadillo();
        break;
    default:
        OiVec::myLinearAlgebra = NULL;
        OiMat::myLinearAlgebra = NULL;
        break;
    }
}
