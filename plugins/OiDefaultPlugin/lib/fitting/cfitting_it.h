//---------------------------------------------------------------------------

#ifndef cfitting_itH
#define cfitting_itH
//---------------------------------------------------------------------------

#include "cfitting_approxfunc.h"


class Cfitting_it
{
  public:
    bool   gIsDebug;
    double gConvergence;

  public:
    Cfitting_it();

    Cfitting_it(bool isdebug,double convergence=1e-9);

    bool approximation(
                 TReal *beob,int n,TReal **para,int np,
                 TReal *ubk ,int u,
                 TReal *Qxx,
                 int *iter,
                 TReal *convergence,
                 Tapproxfunction      func,
                 Tdiffapproxfunction dfunc
               );

    bool variancepropagation(
                              TReal *ubk  ,int u,
                              TReal *para ,
                              int f,
                              TReal *sigmaxx,
                              TReal *sigmaff,
                              Tdiffapproxfunction dfunc
                             );

   /*
       Koordinaten X_i, Y_i, Z_i werden in die Matrix x kopiert
       und der Vektor y (=Beobachtungen) auf Null gesetzt.
   */
   void coords_as_array(TReal *X,TReal *Y,TReal *Z,int n,TReal ***x,TReal **y);

   void free_array(TReal **x,TReal *y,int n);

};

#endif
