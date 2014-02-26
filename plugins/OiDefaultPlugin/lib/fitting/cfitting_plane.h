//---------------------------------------------------------------------------

#ifndef cfitting_planeH
#define cfitting_planeH
//---------------------------------------------------------------------------

#include "cfitting_it.h"

TReal plane_equation(TReal *x,TReal *para);

TReal* d_plane_equation(TReal *x,TReal *para,TReal *df);

bool fitting_plane(double *X,double *Y,double *Z,int count,double *norm,double *d,double *Qxx);

#endif
