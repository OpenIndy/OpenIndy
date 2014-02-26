/*
  F i t t i n g
  -------------

  Modul:    cfitting_approxfunc.cpp
  Version:  1.000
  Datum:    11.01.2010
  Autor:    F. Kern

  Beschreibung:

  Funktion zur iterativen Regressionsberechnung mit anwenderdefinierter Regressionsfunktion


  kern@geoinform.fh-mainz.de
  www.geoinform.fh-mainz.de
*/

#ifndef cfitting_approxfuncH
#define cfitting_approxfuncH

#include "cfitting_util.h"

typedef TReal  (*Tapproxfunction)     (TReal *para,TReal *ubk);
typedef TReal* (*Tdiffapproxfunction) (TReal *para,TReal *ubk,TReal *df);

#define MAXITER 50

bool _approxfunc(TReal *beob,int n,TReal **para,int np,
                 TReal *ubk,int u,
                 TReal *sigmaxx,
                 TReal damping,int *iter,TReal *convergence,
                 Tapproxfunction     func,
                 Tdiffapproxfunction dfunc,
                 bool isDebug
                 );

bool _variancepropagation(TReal *ubk  ,int u,
                         TReal *para ,
                         //TReal *func,
                         int f,
                         TReal *sigmaxx,
                         TReal *sigmaff,
                 //Tapproxfunction      func,
                 Tdiffapproxfunction dfunc,
                 bool isDebug
                 );

#endif
