/**************************************************************************************************************hbegin**
* Filename   : 
* Description: 
* Purpose    :
* Reference  :
* Authors    : Martin Lux (lux@sigma3D.de), Jens Wambach (Jens.Wambach@t-online.de), Benedikt Rauls (benediktrauls@web.de),
*              Fredie Kern (xdesy@gmx.de)
*
* Version    :
*
* Info       : wwww.github.com - www.fh-mainz.de - www.xdesy.de

* Copyright 2013-2014 Lux, Rauls, Wambach, Fachhochschule Mainz - Lehreinheit Geoinformatik und Vermessung - Prof. Dr.-Ing. Fredie Kern  
*
* Licence:
*    This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public 
*    License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any 
*    later version.
*
*    This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied 
*    warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
*
*    You should have received a copy of the GNU General Public License along with this program.  If not, 
*    see <http://www.gnu.org/licenses/>.
*****************************************************************************************************************hend*/
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
//---------------------------------------------------------------------------

#include "cfitting_approxfunc.h"

#include <stdlib.h>
#include <math.h>
#include "cfitting_matrix.h"

#undef DEBUG

bool _approxfunc(TReal *beob,int n,TReal **para,int np,
                 TReal *ubk ,int u,
                 TReal *sigmaxx,
                 TReal damping,int *iter,TReal *convergence,
                 Tapproxfunction      func,
                 Tdiffapproxfunction dfunc,
                 bool isDebug
                 )
{
  TReal **A,**ATA;
  TReal **L;
  TReal **N;
  TReal **ATL;
  TReal **xdach;
  TReal xnorm;
  TReal *a;
  TReal *merke_ubk;
  TReal **v,**s0;
  int   i,j,k;
  bool  ret;
  TReal conv;

  conv = convergence!=NULL ? *convergence : 1e-6;
  ret = true;
  A     = matrix(n,u);
  L     = matrix(n,1);
  ATA   = matrix(u,u);
  N     = matrix(u,u);
  ATL   = matrix(u,1);
  xdach = matrix(u,1);
  a     = (TReal *) calloc(u,sizeof(TReal));

  merke_ubk  = (TReal *) calloc(u,sizeof(TReal));
  for (i=0;i<u;i++) merke_ubk[i] = ubk[i];
  *iter = 1;
  do
  {
    for (i=0;i<n;i++)
    {
      a          = dfunc(para[i],ubk,a);
      for (k=0;k<u;k++)
      {
        A[i][k] = a[k];
      }
      L[i][0]    = beob[i] - func(para[i],ubk);
    }
#ifdef DEBUG
    MatrixOut(L,n,1);
    MatrixOut(A,n,u);
#endif
    if (isDebug)
    {
      MatrixOutMatlab("l",L,n,1);
      MatrixOutMatlab("A",A,n,u);
    }
    at_mal_b(A,n,u,A,n,u,ATA);

    if (!a_inv4(ATA,u,u,N))
    {
      *iter = -*iter;
      ret = false;
      for (i=0;i<u;i++) ubk[i] = merke_ubk[i];
      goto out;
    }
#ifdef DEBUG
    MatrixOutMatlab("N",N,u,u);
#endif
    at_mal_b(A,n,u,L,n,1,ATL);

    a_mal_b(N,u,u,ATL,u,1,xdach);

#ifdef DEBUG
    MatrixOut(xdach,u,1);
    MatrixOutMatlab("x",xdach,u,1);
#endif

    if (isDebug) ProtoMeldung("%3d ubk ",*iter);
    xnorm = 0.0;
    for (i=0;i<u;i++)
    {
      ubk[i] += damping*xdach[i  ][0];
      xnorm += xdach[i  ][0]*xdach[i  ][0];
      if (isDebug) ProtoMeldung("%.3lf ",ubk[i]);
    }
    if (isDebug) ProtoMeldung(" [%7.1E]\n",xnorm);
    (*iter)++;
  }
  while((*iter)<MAXITER && (xnorm>conv));

  if (*iter==MAXITER)
  {
     *iter = 0;
//     for (i=0;i<u;i++) ubk[i] = merke_ubk[i];
     ret = false;
  }
  if (sigmaxx!=NULL)
  {
    TReal sigma0;

    v     = matrix(n,1);
    s0    = matrix(1,1);

    for (i=0;i<n;i++)
    {
      v[i][0]    = func(para[i],ubk) - beob[i];
    }
    at_mal_b(v,n,1,v,n,1,s0);
    sigma0 = sqrt(s0[0][0] / ((double) n-1) );
    for (i=0;i<u;i++)
    {
      for (j=0;j<u;j++)
      {
        sigmaxx[i*u+j] = sigma0*sigma0*N[i][j];
      }
    }
    free_matrix(s0,1,1);
    free_matrix(v,n,1);
  }

out:
  free(merke_ubk);
  free(a);
  free_matrix(xdach,u   ,1);
  free_matrix(ATL  ,u   ,1);
  free_matrix(N    ,u   ,u);
  free_matrix(ATA  ,u   ,u);
  free_matrix(A    ,n   ,u);
  free_matrix(L    ,n   ,1);
  return(ret);
}

bool _variancepropagation(TReal *ubk  ,int u,
                         TReal *para ,
                         //TReal *func,
                         int f,
                         TReal *sigmaxx,
                         TReal *sigmaff,
                 //Tapproxfunction      func,
                 Tdiffapproxfunction dfunc,
                 bool isDebug
                 )
{
  TReal **AT;
  TReal **QXX;
  TReal **QFF;
  TReal **ATQxx;
  TReal *a;
  int    i,j,k;

  AT     = matrix(u,f);
  QXX    = matrix(u,u);
  QFF    = matrix(f,f);
  ATQxx  = matrix(f,u);
  a     = (TReal *) calloc(f*u,sizeof(TReal));

  for (i=0;i<u;i++)
  {
    for (j=0;j<u;j++)
    {
      QXX[i][j] = sigmaxx[i*u+j];
    }
  }
  if (isDebug) MatrixOutMatlab("Qxx",QXX,u,u);
  a  = dfunc(ubk,para,a);
  for (i=0;i<f;i++)
  {
    for (k=0;k<u;k++)
    {
      AT[k][i] = a[i*u+k];  // AT
    }
  }
  if (isDebug) MatrixOutMatlab("Ft",AT,u,f);

  at_mal_b(AT   ,u,f,QXX,u,u,ATQxx);
  a_mal_b (ATQxx,f,u,AT ,u,f,QFF);
  if (isDebug) MatrixOutMatlab("Qff",QFF,f,f);

  for (i=0;i<f;i++)
  {
    for (j=0;j<f;j++)
    {
      sigmaff[i*u+j] = QFF[i][j];
    }
  }
  free(a);
  free_matrix(ATQxx,f   ,u);
  free_matrix(QFF  ,f   ,f);
  free_matrix(QXX  ,u   ,u);
  free_matrix(AT   ,u   ,f);

	return(true);
}
