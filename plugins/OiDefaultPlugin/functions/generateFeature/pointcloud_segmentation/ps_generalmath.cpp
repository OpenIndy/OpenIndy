#include "ps_generalmath.h"

PS_GeneralMath::PS_GeneralMath()
{
}

/*!
 * \brief GeneralMath::computeBinomialCoefficient
 * \param n
 * \param k
 * \return
 */
unsigned long PS_GeneralMath::computeBinomialCoefficient(unsigned int n, unsigned int k){
    unsigned long bc = 0.0;

    if(k == 0){
        bc = 1;
    }else if(k > n){
        bc = 0;
    }else if(2*k > n){
        bc = PS_GeneralMath::computeBinomialCoefficient(n, n-k);
    }else{
        bc = n - k + 1;
        for(unsigned int i = 2; i <= k; i++){
            bc = bc * (n - k + i);
            bc = bc / i;
        }
    }

    return bc;
}
