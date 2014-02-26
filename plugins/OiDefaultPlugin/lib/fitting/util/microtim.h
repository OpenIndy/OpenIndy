/*
  X d e s y
  ---------

  Modul:    microtime.cpp
  Version:  1.1
  Datum:    14.02.2007, 23.08.2007, 17.02.2009
  Autor:    F. Kern

  Beschreibung:

  hochauflösende Zeitbestimmung

  17.02.2009 :  Umstelleung auf die WIN-API-Funktion QueryPerformanceCounter()

  fkern@xdesy.de
  www.xdesy.de
*/
#ifndef MICROTIM_HEADER

#define MICROTIM_HEADER

typedef double MICRO_TIME;


//MICRO_TIME microTime(void);

char*      InitMICRO_TIM(void);

void       TimeStart(MICRO_TIME *t);
MICRO_TIME TimeStop(MICRO_TIME *t);


#endif

