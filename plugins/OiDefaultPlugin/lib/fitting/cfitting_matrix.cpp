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

#include "cfitting_matrix.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "cfitting_util.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

#define printf ProtoMeldung
#define CALLOC calloc
#define FREE   free

TReal **rmatrix(int nrl,int nrh,int ncl,int nch)
{
 int i;
 TReal **m;

 m = (TReal **) malloc((unsigned) (nrh-nrl+1)*sizeof(TReal *));
 if (!m) printf("allocation failure 1 in dmatrix()");
 m -= nrl;

 for(i=nrl;i<=nrh;i++)
 {
   m[i]=(TReal *) malloc((unsigned) (nch-ncl+1)*sizeof(TReal));
   if (!m[i]) printf("allocation failure 2 in dmatrix()");
   m[i] -= ncl;
 }
 return m;
}

TReal **matrix(int nr,int nc)
{
  return( rmatrix(0,nr-1,0,nc-1) );
}

void free_rmatrix(TReal **m,int nrl,int nrh,int ncl,int nch)
{
 int i;

 for(i=nrh;i>=nrl;i--) free((char*) (m[i]+ncl));
 free((char*) (m+nrl));
}

void free_matrix(TReal **m,int nr,int nc)
{
  free_rmatrix(m,0,nr-1,0,nc-1);
}

#ifdef MIT_MATRIXOUTPUT
void MatrixOut(TReal **a,int z,int s)
{
  int i,j;

  for(i=0;i<z;++i)
  {
    for(j=0;j<s;++j) printf("%.6lf ",(double) a[i][j]);
    printf("\n");
  }
}

void VektorOut(TReal *a,int z)
{
  int i;

  for(i=0;i<z;++i)
  {
    printf("%.6lf ",(double) a[i]);
    printf("\n");
  }
}


void MatrixOutMatlab(char *name,TReal **a,int z,int s)
{
  int i,j;
  FILE *fp;
  char str[256];

  sprintf(str,"%s.m",name);
  if ((fp=fopen(str,"wt"))!=NULL)
  {
    fprintf(fp,"X%s = [\n",name);
    for(i=0;i<z;++i)
    {
      for(j=0;j<s;++j)
         fprintf(fp,"%.6le ",(double) a[i][j]);
      fprintf(fp,"\n");
    }
    fprintf(fp,"]\n");
  }
  fclose(fp);
}
#endif

#ifdef MIT_MATRIX_A_PLUS_B
void a_plus_b(TReal **a,TReal **b,TReal **c,int s,int z)
{
  int i,j;
  for(i=0;i<z;++i) for(j=0;j<s;++j) c[i][j] = a[i][j] + b[i][j];
}
#endif

#ifdef MIT_MATRIX_SKALAR_MAL_A
void skalar_mal_a(TReal **a,TReal skalar,TReal **c,int s,int z)
{
  int i,j;
  for(i=0;i<z;++i) for(j=0;j<s;++j) c[i][j] = skalar * a[i][j];
}
#endif

#ifdef MIT_MATRIX_A_MAL_B
void a_mal_b(TReal **a,int z1,int s1,TReal **b,int z2,int s2,TReal **c)
{
  int i,k,j;
  TReal summe;

  for(i=0;i<z1;++i) for(k=0;k<s2;++k)
  {
    summe = 0.0;
    for(j=0;j<s1;++j)
    {
      summe += a[i][j] * b[j][k];
    }
    c[i][k] = summe;
  }
}
#endif

#ifdef MIT_MATRIX_A_MAL_B_SUM
void a_mal_b_sum(TReal **a,int z1,int s1,TReal **b,int z2,int s2,TReal *c)
{
  int i,k,j;

  *c = 0.0;
  for(i=0;i<z1;++i) for(k=0;k<s2;++k)
  {
    for(j=0;j<s1;++j)
    {
      *c += a[i][j] * b[j][k];
    }
  }
}
#endif

#ifdef MIT_MATRIX_AT_MAL_B
void at_mal_b(TReal **a,int z1,int s1,TReal **b,int z2,int s2,TReal **c)
{
  int i,k,j;
  TReal summe;

  for(i=0;i<s1;++i) for(k=0;k<s2;++k)
  {
    summe = 0.0;
    for(j=0;j<z1;++j)
    {
      summe += a[j][i] * b[j][k];
    }
    c[i][k] = summe;
  }
}
#endif

#ifdef MIT_MATRIX_A_MAL_BT
void a_mal_bt(TReal **a,int z1,int s1,TReal **b,int z2,int s2,TReal **c)
{
  int i,k,j;
  TReal summe;

  for(i=0;i<z1;++i) for(k=0;k<z2;++k)
  {
    summe = 0.0;
    for(j=0;j<s1;++j)
    {
      summe += a[i][j] * b[k][j];
    }
    c[i][k] = summe;
  }
}
#endif

#ifdef MIT_MATRIXINV4
int a_inv4(TReal **a,int z,int s,TReal **b)
{
  int i,j,l,is,n;
  TReal dp,dt,damx;
  TReal *h;
  int *IQ;
  TReal *A;

  n = z;
  A = (TReal *) CALLOC(z*s,sizeof(TReal));
  for (i=0;i<n;i++)
  {
    for (j=0;j<n;j++)
    {
      A[i*s+j] = a[i][j];
    }
  }
  IQ = (int *) CALLOC(n,sizeof(int));
  for (i=0;i<n;i++) IQ[i] = false;
  for (i=0;i<n;i++)
  {
    damx = 0.0;
    for (j=0;j<n;j++)
    {
      if (IQ[j]) continue;
      h = A+j*s+j;
      if (fabs(damx)>fabs(*h)) continue;
      is = j;
      damx = *h;
    }
    IQ[is] = true;
    h = A+is*s+is;
    dp = *h;
    *h = -1.0;
    if (fabs(dp)<1E-20)
    {
      FREE(IQ);
      FREE(A);
      return(false);
    }
    for (j=is;j<n;j++)
    {
      h = A+is*s+j;
      *h = *h / dp;
    }
    for (l=0;l<n;l++)
    {
      if (l==is) continue;
      if (l<is)
      {
	dt = A[l*s+is];
      }
      else
      {
	dt = A[is*s+l] * dp;
      }
      for (j=l;j<n;j++)
      {
        if (j==is) continue;

	h = A+l*s+j;
	if (j<is)
	{
	  if (*h==0.0)
	  {
	    *h =    - A[j*s+is] * dt / dp;
          }
          else
          {
	    *h = *h - A[j*s+is] * dt / dp;
          }
        }
        else
        {
          if (*h==0.0)
          {
	    *h =    - A[is*s+j] * dt;
          }
          else
          {
	    *h = *h - A[is*s+j] * dt;
          }
        }
      }
      if (l<is)
      {
	h = A+l*s+is;
	*h = *h / dp;
      }
    }
  }
  for (i=0;i<n;i++)
  {
    for (j=i;j<n;j++)
    {
      h = A+i*s+j;
      *h = - *h;
      A[j*s+i] = *h;
      b[i][j] = b[j][i] = *h;
    }
  }
  FREE(IQ);
  FREE(A);
  return(true);
}
#endif
