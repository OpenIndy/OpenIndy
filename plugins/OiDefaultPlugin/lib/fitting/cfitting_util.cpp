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

#include "cfitting_util.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

#ifdef WITH_FITTING_UTILS

#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

//FILE          *fperr=stderr;
bool          isQuiet=false;
int            QuietMode=1;

static char   buffer[2048];

void __default_meldung(char *str)
{

}

static Tmeldung extern_wmeldung=__default_meldung;

void Init_externmeldung(Tmeldung ex_wmeldung)
{
  isQuiet   = true;
  QuietMode = 2;
  if (ex_wmeldung!=NULL) extern_wmeldung = ex_wmeldung;
}

void ProtoMeldung(char *fmt,...)
{
  va_list va;

  va_start(va,fmt);

  if (!isQuiet)     vfprintf(stderr,fmt,va);
  if (QuietMode==2)
  {
    vsprintf(buffer,fmt,va);
    extern_wmeldung(buffer);
  }
  va_end(va);
}

void Meldung(char *fmt,...)
{
  va_list va;

  va_start(va,fmt);

  if (!isQuiet)
  {
    vfprintf(stderr,fmt,va);
  }

  va_end(va);
}

void Error(char *fmt,...)
{
  char str[81];
  va_list va;

  va_start(va,fmt);
  //fprintf(fperr,"\nError");
  vsprintf(str,fmt,va);
  //fprintf(fperr,"%s",str);
  va_end(va);
  exit(1);
}

FILE *myfopen(const char *filename,const char *mode)
{
  FILE *fp;

  fp = NULL;
  fp = fopen(filename,mode);
  if (fp==NULL)
  {
    Error("'%s' %s",filename,
#if defined(__TURBOC__) || defined(__SC__) || defined(__GCC_HPUX__) || defined(__MSC__)
#ifdef __TURBOC__ > 0x5600
           ""
#else
           strerror(errno)
#endif
#else
           /* perror() */ ""
#endif
    );
  }
  return(fp);
}


#endif
