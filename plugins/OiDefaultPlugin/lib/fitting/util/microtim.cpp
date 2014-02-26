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
  X d e s y
  ---------

  Modul:    microtime.cpp
  Version:  1.1
  Datum:    14.02.2007, 23.08.2007, 17.02.2009
  Autor:    F. Kern

  Beschreibung:

  hochauflösende Zeitbestimmung

  17.02.2009 :  Umstellung auf die WIN-API-Funktion QueryPerformanceCounter()

  fkern@xdesy.de
  www.xdesy.de
*/
//---------------------------------------------------------------------------
//#include <vcl.h>
#pragma hdrstop

#include "microtim.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)


/*======================================================================
 * DISCLAIMER:    Use of this routine is free. No guarantee is given
 *                that this function will work as described.
 *
 * MODULE NAME:   System Timer Module
 *
 * Author:        Mark Piggott  Compuserve 100316,454
 * Filename:      microtim.c
 * Creation date: 19-Sep-1994
 * Version:       1.0
 * Version date:  19-Sep-1994
 *
 * MODULE DESCRIPTION:
 *
 * This module contains a function to return a timestamp value in
 * microseconds.  It obtains this value by combining the BIOS clock
 * value (in ticks) with the 8253/4 timer0 value.  The value is returned
 * as a double.
 *
 *======================================================================
 */

/*======================================================================
 * SOURCE FILE INCLUSIONS:
 *======================================================================
 */
#if (defined(__TURBOC__) || defined(__SC__) || defined(__MSC__)) && !defined(__WIN32__)

#ifdef __MSC__
#define enable  _enable
#define disable _disable
#endif

#include <stdio.h>
#include <dos.h>
#include <conio.h>

#include "microtim.h"
/*
#pragma intrinsic (inp, outp, _enable, _disable)   // Work quicker inline
*/

/*======================================================================
 * MACRO DEFINITIONS LOCAL TO THIS MODULE:
 *======================================================================
 */
#define  TIMER_CTL   0x43        // Mode Control Register
#define  TIMER0      0x40        // Counter 0
#define  TIMER_CFG   0x34        // Timer 0, Write LSB then MSB, Mode 2, Binary
#define  TIMER_READ  0x04        // Timer 0, Latch, Mode 2, Binary
#define  TIMER_FREQ  1193180.0   // Timer countdown frequency

#define  BIOS_CLOCK_ADDRESS  0x0040006CL // BIOS clock address

#define  PIC         0x20        // Programmable interrupt Controller Address
#define  IRQ_PENDING 0x0a        // 'Read pending status' command


/*======================================================================
 * TYPE/STRUCTURE/UNION DEFINITIONS LOCAL TO THIS MODULE:
 *======================================================================
 */

/*======================================================================
 * EXTERNAL/STATIC FUNCTION PROTOTYPES
 *======================================================================
 */


/*======================================================================
 * EXTERNAL/STATIC VARIABLE DEFINITIONS:
 *======================================================================
 */
static int  Mode2 = 0;     /* If zero, timer needs to be set to Mode 2 */



/*======================================================================
 * FUNCTION NAME: microTime(void)
 *
 * Description:   Function returns the time in microseconds.
 *
 * Arguments:     None
 *
 * Return value:  Time in microseconds.
 *
 * Public variables modified: None
 * Static variables modified: Mode2
 * Functions called:          None
 *======================================================================
 */
MICRO_TIME microTime(void)
{
   /*======================================================================
    * LOCAL VARIABLE DECLARATIONS
    *======================================================================
    */
   volatile unsigned long far *clockTicks; /* Pointer to BIOS time (tick count) */
   unsigned          counter;              /* Inverted 16-bit timer value       */
   unsigned long     ticks;                /* Copy of BIOS time                 */
   unsigned          pending;              /* Interrupts pending Bit0=IRQ0      */
   MICRO_TIME        dValue;               /* Double microseconds time          */

   /*======================================================================
    * FUNCTION BODY
    *======================================================================
    */
   clockTicks=(unsigned long far *)BIOS_CLOCK_ADDRESS;   /* BIOS time address */

   /*=========================================================================
    * If the timer has not already been put into Mode 2, put it into Mode 2.
    *=========================================================================
    */
   if (!Mode2)
   {
      outp(TIMER_CTL, TIMER_CFG);   // Setup
      outp(TIMER0, 0xff);           // Load LSByte
      outp(TIMER0, 0xff);           // Load MSByte
      Mode2 = 1;
   }

   /*============================================
    * Obtain BIOS clock and timer values
    *============================================
    */
   disable();                             /* Disable interrupts including IRQ0 */
   ticks = *clockTicks;                   /* Read BIOS clock                   */
   outp(TIMER_CTL, TIMER_READ);           /* Counter 0, Latch, Mode 2, Binary  */
   counter = inp(TIMER0) ^ 0xff;          /* Read LSByte and invert            */
   counter |= (inp(TIMER0) ^ 0xff) << 8;  /* Read MSByte and invert            */
   outp(PIC, IRQ_PENDING);                /* Request IRQ pending info          */
   pending = inp(PIC);                    /* Read IRQ pending state            */
   enable();                              /* Enable interrupts again           */

   /*======================================================================
    * If the count is low, the timer may have just rolled over.  If
    * it has just rolled over, the BIOS tick count maya or may not have
    * been updated.  If IRQ0 is pending then the BIOS tick count will NOT
    * have been updated.  If no interrupt is pending, then it must already
    * have been serveiced and the bios count will be up to date.
    *======================================================================
    */
   if ((counter < 0x8000) && (pending & 1))
   {
      ticks++;
   }

   /*========================================================================
    * The clock tick and counter integers are converted to doubles, combined
    * converted to seconds by dividing by the timer frequency, and finally
    * converted into microseconds by multiplying by a million.
    *========================================================================
    */
   dValue = (((double) ticks*65536.0) + (double) counter) * 1000000.0 / TIMER_FREQ;

   /*=================================
    * Return the time in microseconds
    *=================================
    */
   return(dValue);
}


#define usleep delay

#else

#include <stdlib.h>
#include <time.h>

#if defined(__GCC_DOS__) || defined(__GCC_LINUX__)
#include <sys//resource.h>
#endif

#include "microtim.h"

#if defined(__GCC_DOS__) || defined(__GCC_LINUX__)

#define MICRO_TIME_USE "getrusage()"

MICRO_TIME microTime(void)
{
  struct rusage r;

  getrusage(0,&r);
  return(r.ru_utime.tv_sec*1E6 + r.ru_utime.tv_usec);
#endif

#if defined(__WIN32__) || defined(_WIN32)
                                                // WIN32

#ifdef WITH_QueryPerformanceFrequency

#include <windows.h>

static long long g_CurentCount=0.0;
static long long g_Frequency=1.0;

MICRO_TIME qpc_microTime(void)
{
  QueryPerformanceCounter((LARGE_INTEGER*)&g_CurentCount);
  return(((double) g_CurentCount/(double) g_Frequency)*1.0E6);
}
#endif

#define MICRO_TIME_USE "clock()/CLK_TCK"

MICRO_TIME clk_microTime(void)
{
  return(clock()*1E6/CLK_TCK);
}


#else

#define MICRO_TIME_USE "clock()"
MICRO_TIME microTime(void)
{
  // Linux
  return(clock());
// Cygwin
//return(clock()/CLOCKS_PER_SEC));
}
#endif

#endif


#if defined(__WIN32__) || defined(_WIN32)
static MICRO_TIME (* p_microTime)(void) = clk_microTime;
#else
static MICRO_TIME (* p_microTime)(void) = microTime;
#endif
static char used_counter_is[256]=MICRO_TIME_USE;

char* InitMICRO_TIM(void)
{
#ifdef WITH_QueryPerformanceFrequency
  if (!QueryPerformanceFrequency((LARGE_INTEGER*)&g_Frequency))
  {
     p_microTime = clk_microTime;
  }
  else
  {
     p_microTime = qpc_microTime;
     strcpy(used_counter_is,"QueryPerformanceCounter");
  }
#endif
  return(used_counter_is);
}

void TimeStart(MICRO_TIME *t)
{
  *t = p_microTime();
}

MICRO_TIME TimeStop(MICRO_TIME *t)
{
  MICRO_TIME tt,dt;

  tt = p_microTime();
  dt = (tt - *t);
  *t = tt;
  return(dt);
}

/*
#define TEST
*/
#ifdef TEST

#include <stdio.h>

int main(void)
{
   double   d,dd;
   MICRO_TIME tim;

   TimeStart(&tim);
   int i=1;
   while(/*!kbhit()*/i<1e3)
   {
      d = microTime();
//      usleep(1E6);
      dd = microTime() - d;
      //d = dd*(dd+1.0)/(dd-1.0); 
      printf("[%d] Time in microseconds: %lf = %lf s\n",i, dd,dd/1E6);
      i++;
   }
   printf("%lf sec\n",TimeStop(&tim)/1E6);
   return(0);
}
#endif
