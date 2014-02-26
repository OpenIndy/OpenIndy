//---------------------------------------------------------------------------

#ifndef cfitting_utilH
#define cfitting_utilH
//---------------------------------------------------------------------------

#ifdef WITH_FITTING_UTILS

#include <stdio.h>


typedef double TReal;

typedef  void (*Tmeldung) (char *str);

extern bool          isQuiet;
extern int           QuietMode;

void Init_externmeldung(Tmeldung ex_wmeldung);
void ProtoMeldung(char *fmt,...);
void Meldung(char *fmt,...);
void Error(char *fmt,...);

FILE *myfopen(const char *filename,const char *mode);

#endif

#endif
