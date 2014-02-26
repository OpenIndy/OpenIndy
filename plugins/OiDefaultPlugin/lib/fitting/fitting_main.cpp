#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "cfitting_plane.h"
#include "cfitting_circle.h"
#include "cfitting_sphere.h"
#include "cfitting_ellipse.h"
#include "cfitting_util.h"

#include "getopt.h"
#include "microtim.h"
#include "iskeyword.h"
#include "rho.h"

#define PNAME         "Fitting"
#define PCOPYRIGHT    "F. Kern (www.i3mainz.fh-mainz.de)"
#undef  PVERSION
#define PVERSION      "1.003 11.01.2010"
#undef  PVERSION
#define PVERSION      "v1.0.1 12.01.2010"
#undef  PVERSION
#define PVERSION      "v1.0.2 14.01.2010"

int    gDebugLevel=0;
char   gOutFileName[513];
bool   isOutFileName=false;
//char   gLogFileName[513]="fitting.log";

char *keywFittingTypes[]={
                           "plane|Ebene|ebene"
                          ,"circle|Kreis|kreis"
                          ,"sphere|Kugel|kugel"
                          ,"ellipse|Ellipse|ellipse"
                         };

#define FITTINGTYPE_PLANE   0
#define FITTINGTYPE_CIRCLE  1
#define FITTINGTYPE_SPHERE  2
#define FITTINGTYPE_ELLIPSE 3

int    gFittingType = FITTINGTYPE_PLANE;
TReal  gConvergence=1e-9;
char   gName[513]="(unknown)";

bool isFormat[2]={false,false};
char gFormat[2][256]={"xyz","xy"};



void Usage(char *arg)
{
  Meldung(" %s %s %s\n",PNAME,PVERSION,PCOPYRIGHT);
  Meldung(" Programm für die Approximation von geometrischen Formen.\n");
  Meldung("\n");
  Meldung(" Aufruf: %s <daten> [Optionen]\n",arg);
  Meldung(" Optionen:\n");
  Meldung("  -T<type>   : Art der Ausgleichsfigur\n");
  Meldung("        type = 'plane'   | 'Ebene'   : Ausgleichsebene\n");
  Meldung("             = 'circle'  | 'Kreis'   : Ausgleichskreis\n");
  Meldung("             = 'sphere'  | 'Kugel'   : Ausgleichskugel\n");
  Meldung("             = 'ellipse' | 'Ellipse' : Ausgleichsellipse\n");
  Meldung("  -C<threshold>  : Kriterium für Konvergenz (%g)\n",gConvergence);
  Meldung("  -N<name>       : Projektname\n");
  Meldung("  -f<Format> : Format der Eingabezeilen festlegen.\n");
  Meldung("               mögliche Bestandteile von <Format> sind:\n");
  Meldung("                    'p' Punktnummer       'x' X-Koordinate\n");
  Meldung("                    '.' Spalte überlesen  'y' Y-Koordinate\n");
  Meldung("                    'c' Punktcode         'z' Z-Koordinate\n");
  Meldung("                    (Default '-fxyz ')\n");
  Meldung("  -D<level>  : Debuging-Level [0-5]\n");
  Meldung("  -o<name>   : Name der Ausgabedatei\n");
  Meldung("                        <x> muá, [x] kann angegeben werden. (x) Default\n");
  exit(1);
}

void LeseOpt(int argv,char *argc[])
{
  int    opt;
  char   *optarg;
//  char    hoptarg[256];

  while ((opt=getopt(argv,argc,"o:T:C:N:D:f:",&optarg))!=EOF)
  {
//   Meldung("%d %d '%s'\n",opt,optarg!=NULL ? optarg[0] : -1,optarg!=NULL ? optarg : "");
    if (opt=='?')
    {
      exit(1);
    }
    switch(opt)
    {
      case 'T':
         if (optarg!=NULL)
         {
           gFittingType = isKeywordAlternativen(optarg,keywFittingTypes);
           if (gFittingType==-1) Error("'%s 'unkown fitting type.",optarg);
         } else Error("fitting type expected.");
         break;
      case 'C':
         if (optarg!=NULL) {
           sscanf(optarg,"%lf",&gConvergence);
         } else Error("value expected.");
         break;
      case 'o':
         if (optarg!=NULL) {
           strcpy(gOutFileName,optarg);
           isOutFileName = true;
         } else Error("file name expected.");
         break;
      case 'N':
         if (optarg!=NULL) {
           strcpy(gName,optarg);
         } else Error("name expected.");
         break;
      case 'D':
         if (optarg!=NULL) {
           sscanf(optarg,"%d",&gDebugLevel);
         } else Error("debugging-level (1,2,3,..) expected.");
         break;
      case 'f':
         if (optarg==NULL) Error("Format-String erwartet.",opt);
         strcpy(gFormat[0],optarg);
         isFormat[0] = true;
         strcpy(gFormat[1],optarg);
         isFormat[1] = true;
         break;
    }
  }
}
/*
int import_it_3D(FILE *fpin,TReal **x,TReal **y,TReal **z)
{
  char  Zeile[512];
  TReal v1,v2,v3;
  int n,j;

  n = 0;
  while (fgets(Zeile,255,fpin)!=NULL)
  {
    j = sscanf(Zeile,"%lf %lf %lf",&v1,&v2,&v3);
    if (j==3)
    {
      *x = (TReal *) realloc(*x,(n+1)*sizeof(TReal));
      *y = (TReal *) realloc(*y,(n+1)*sizeof(TReal));
      *z = (TReal *) realloc(*z,(n+1)*sizeof(TReal));
      (*x)[n] = v1;
      (*y)[n] = v2;
      (*z)[n] = v3;
      n++;
    }
  }
  return(n);
}
*/

int import_it_2D(FILE *fpin,TReal **x,TReal **y)
{
  char  Zeile[512];
  TReal v1,v2;
  int n,j;

  n = 0;
  while (fgets(Zeile,255,fpin)!=NULL)
  {
    j = sscanf(Zeile,"%lf %lf",&v1,&v2);
    if (j==2)
    {
      *x = (TReal *) realloc(*x,(n+1)*sizeof(TReal));
      *y = (TReal *) realloc(*y,(n+1)*sizeof(TReal));
      (*x)[n] = v1;
      (*y)[n] = v2;
      n++;
    }
  }
  return(n);
}

int import_it(FILE *fpin,TReal **x,TReal **y,TReal **z,char *Format,bool is3D)
{
  char   Zeile[512];
  int    i,n,_n;
  TReal  p[3];
	char   tmp[512],dummy[512],pcode[512];
//  char   name[512];
//  int    name_idx;
  bool   isP,isX,isY,isZ,isPC;
  char  *pstr;
  bool   is[32];

  _n = 0;
  for (i=0;i<3;i++)
  {
    p[i] = 0.0;
  }
  for (i=0;i<5;i++) is[i] = false;

  for (i=0;i<strlen(Format);i++)
  {
    switch(Format[i])
    {
      case 'p':
        is[0] = true;
        break;
      case 'x':
        is[1] = true;
        break;
      case 'y':
        is[2] = true;
        break;
      case 'z':
        is[3] = true;
        break;
      case 'c':
        is[4] = true;
        break;
    }
  }

  while (fgets(Zeile,255,fpin)!=NULL)
  {
    isP = isX = isY = isZ = isPC = false;
    i = 0;
    n = 0;
    sscanf(Zeile,"%s",dummy);
    if (dummy[0]==';' || dummy[0]=='#') continue;
    pstr = strtok(Zeile," ;\n\t");
    if (pstr!=NULL)
    {
      do
      {
        if (Format[i]=='.')
        {
          n++;
        }
        else if (Format[i]=='p')
        {
          //strcpy(name,pstr);
          //if (strlen(name)>gMaxPNameLen) gMaxPNameLen = strlen(name);
          n++;
          isP = true;
        }
        else if (Format[i]=='x')
        {
          if (sscanf(pstr,"%lf",&(p[0]))==1)
          {
            n++;
            isX = true;
          }
        }
        else if (Format[i]=='y')
        {
          if (sscanf(pstr,"%lf",&(p[1]))==1)
          {
            n++;
            isY = true;
          }
        }
        else if (Format[i]=='z')
        {
          if (sscanf(pstr,"%lf",&(p[2]))==1)
          {
            n++;
            isZ = true;
          }
        }
        else if (Format[i]=='c')
        {
          //strcpy(pcode,pstr);
          isPC = true;
          n++;
        }
        i++;
      }
      while((pstr=strtok(NULL," ;\n\t"))!=NULL && i<strlen(Format));
      if (is3D)
      {
        if (i==n && is[0]==isP && is[1]==isX && is[2]==isY && is[3]==isZ && is[4]==isPC
            /*pstr==NULL && && isP && isX && isY && isZ*/ )
        {
          *x = (TReal *) realloc(*x,(_n+1)*sizeof(TReal));
          *y = (TReal *) realloc(*y,(_n+1)*sizeof(TReal));
          *z = (TReal *) realloc(*z,(_n+1)*sizeof(TReal));
          (*x)[_n] = p[0];
          (*y)[_n] = p[1];
          (*z)[_n] = p[2];
          _n++;
          for (i=0;i<3;i++) p[i] = 0.0;
        }
      }
      else
      {
        if (i==n && is[0]==isP && is[1]==isX && is[2]==isY && is[3]==isZ && is[4]==isPC
            /*pstr==NULL && && isP && isX && isY && isZ*/ )
        {
          *x = (TReal *) realloc(*x,(_n+1)*sizeof(TReal));
          *y = (TReal *) realloc(*y,(_n+1)*sizeof(TReal));
          (*x)[_n] = p[0];
          (*y)[_n] = p[1];
          _n++;
          for (i=0;i<2;i++) p[i] = 0.0;
        }
      }
    }
  }
  return(_n);
}


void ProtoMeldungMatrix(double *a,int z,int s)
{
  ProtoMeldung("Sigma_xx =\n");
  for (int i=0;i<z;i++)
  {
    for (int j=0;j<s;j++)
    {
      ProtoMeldung("%13.6le ",a[i*s+j]);
    }
    ProtoMeldung("\n");
  }
}

void ProtoMeldungCorrelationMatrix(double *Qxx,int n)
{
  double qxx,qyy;

  ProtoMeldung("Korrelationsmatrix\n");
  for (int i=0;i<n;i++)
  {
    qxx = sqrt(Qxx[i*n+i]);
    ProtoMeldung("%12.6lf ",qxx);
    for (int j=0;j<i;j++) ProtoMeldung("%4s.%3s ","","");

    for (int j=i;j<n;j++)
    {
      qyy = sqrt(Qxx[j*n+j]);
      ProtoMeldung("%8.3lf ",100.0*Qxx[i*n+j]/qxx/qyy);
    }
    ProtoMeldung("\n");
  }
}


int main(int argc, char* argv[])
{
  FILE *fpout,*fperr;
  char  header[513];
  char  indat[513];
  MICRO_TIME tim_total;
  FILE  *fpin;
  int   n;

  InitMICRO_TIM();

  TimeStart(&tim_total);

  fpout = stdout;
  fperr = myfopen("fitting.err","wt");

  if (argc > 1)
  {
    init_getopt(1,1,'-');
    LeseOpt(argc,argv);
    if (argv[1][0]=='-') Usage(argv[0]); else strcpy(indat,argv[1]);
  }
  if (argc==1) Usage(argv[0]);
//  OpenLogFile(gLogFileName);

  bool merke;
  merke = isQuiet;
  isQuiet = false;
  ProtoMeldung(  "+------------------------------------------------------+\n");
  sprintf(header,"| %s %s %s\n",PNAME,PVERSION,PCOPYRIGHT);
  ProtoMeldung(header);
  ProtoMeldung(  "+------------------------------------------------------+\n");
  isQuiet = merke;

  ProtoMeldung("\nAufruf: ");
  for (int i=0;i<argc;i++) ProtoMeldung("%s ",argv[i]);
  ProtoMeldung("\n\n");

  if (isOutFileName)
  {
    fpout = fopen(gOutFileName,"wt");
  }

  TReal *x,*y,*z;

  fpin = myfopen(indat,"rt");
  n = 0;
  x = y = z = NULL;
  switch(gFittingType)
  {
     case FITTINGTYPE_PLANE:
     case FITTINGTYPE_SPHERE:
       n = import_it(fpin,&x,&y,&z,gFormat[0],true);
       break;
     case FITTINGTYPE_CIRCLE:
     case FITTINGTYPE_ELLIPSE:
       n = import_it(fpin,&x,&y,&z,gFormat[1],false);
       break;
  }
  fclose(fpin);


  switch(gFittingType)
  {
     case FITTINGTYPE_PLANE:
       {
         TReal norm[3],d;
         TReal Qxx[4*4];

         if (fitting_plane(x,y,z,n,norm,&d,Qxx))
         {
           ProtoMeldung("plane: norm=[%.6lf %.6lf %.6lf] d=%.6lf\n",norm[0],norm[1],norm[2],d);
           ProtoMeldungMatrix(Qxx,3,3);
           ProtoMeldungCorrelationMatrix(Qxx,3);
           if (isOutFileName) fprintf(fpout,"%16s %12.6lf %12.6lf %12.6lf %12.6lf\n",gName,norm[0],norm[1],norm[2],d);
         }
         else ProtoMeldung("error in plane fitting.");
       }
       break;
     case FITTINGTYPE_CIRCLE:
       {
         TReal xm[2],r;
         TReal Qxx[3*3];

         if (fitting_circle(x,y,n,xm,&r,Qxx))
         {
           ProtoMeldung("circle: xm=[%.6lf %.6lf] r=%.6lf\n",xm[0],xm[1],r);
           ProtoMeldungMatrix(Qxx,3,3);
           ProtoMeldungCorrelationMatrix(Qxx,3);
           if (isOutFileName) fprintf(fpout,"%16s %12.6lf %12.6lf %12.6lf\n",gName,xm[0],xm[1],r);
         }
         else ProtoMeldung("error in circle fitting.");
       }
       break;
     case FITTINGTYPE_SPHERE:
       {
         TReal xm[3],r;
         TReal Qxx[4*4];

         if (fitting_sphere(x,y,z,n,xm,&r,Qxx))
         {
           ProtoMeldung("sphere: xm=[%.6lf %.6lf %.6lf] r=%.6lf\n",xm[0],xm[1],xm[2],r);
           ProtoMeldungMatrix(Qxx,4,4);
           ProtoMeldungCorrelationMatrix(Qxx,4);
           if (isOutFileName) fprintf(fpout,"%16s %12.6lf %12.6lf %12.6lf %12.6lf\n",gName,xm[0],xm[1],xm[2],r);
         }
         else ProtoMeldung("error in sphere fitting.");
       }
       break;
     case FITTINGTYPE_ELLIPSE:
       {
         TReal xm[2],theta,A[2];
         TReal Qff[5*5];

         if (fitting_ellipse(x,y,n,xm,&theta,A,Qff))
         {
           ProtoMeldung("ellipse: xm=[%.6lf %.6lf] theta=%.6lf gon A=%.6lf B=%.6lf \n",xm[0],xm[1],theta*g200Rho,A[0],A[1]);
           ProtoMeldungMatrix(Qff,5,5);
           ProtoMeldungCorrelationMatrix(Qff,5);
           if (isOutFileName) fprintf(fpout,"%16s %12.6lf %12.6lf %12.6lf %12.6lf %12.6lf \n",gName,xm[0],xm[1],theta*g200Rho,A[0],A[1]);
         }
         else ProtoMeldung("error in ellipse fitting.");
       }
       break;
  }

  if (isOutFileName) fclose(fpout);

  fclose(fperr);
  ProtoMeldung("total runing time: %.6lf sec.\n", TimeStop(&tim_total)/1e6);

//  CloseLogFile();
  return 0;
}
//---------------------------------------------------------------------------
