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
//---------------------------------------------------------------------------

#pragma hdrstop

#include "cfitting_plane.h"

#include  <math.h>


//---------------------------------------------------------------------------
#pragma package(smart_init)


extern int    gDebugLevel;
extern double gConvergence;

/*************************************************/

TReal plane_equation(TReal *x,TReal *para)
{
  TReal ret;
  TReal a,b,c;

  a       = para[0];
  b       = para[1];
  c       = para[2];

  ret = a*x[0] + b*x[1] + c*x[2] - 1.0;

  return(ret);
}

TReal* d_plane_equation(TReal *x,TReal *para,TReal *df)
{
  TReal a,b,c;

  a       = para[0];
  b       = para[1];
  c       = para[2];


  df[0] = x[0];  // df/da
  df[1] = x[1];  // df/db
  df[2] = x[2];  // df/dc

  return(df);
}


bool fitting_plane(double *X,double *Y,double *Z,int count,double *norm,double *d,double *Qxx)
{
  Cfitting_it  fitting_it(gDebugLevel>=4,gConvergence);
  TReal   ubk[3];
  TReal **x;  // Parameter
  TReal  *y;  // abhängige Variable
  int     i;
  bool    ret;
  int     iter;
  TReal   convergence;

  fitting_it.coords_as_array(X,Y,Z,count,&x,&y);
  /* approx. values */
  ubk[0] = ubk[1] = 0.0;
  ubk[2] = 1.0;


  ret = fitting_it.approximation(y,count,x,count,ubk,3,Qxx,
               &iter,&convergence,
               plane_equation,
               d_plane_equation
               );
  if (ret==true)
  {
    *d = 1.0/hypot(hypot(ubk[0],ubk[1]),ubk[2]);
    for (i=0;i<3;i++) norm[i] = *d * ubk[i] ;
  }
  fitting_it.free_array(x,y,count);
  return(ret);
}
