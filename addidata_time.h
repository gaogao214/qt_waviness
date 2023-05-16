/** @file addidata_time.h
* @brief time related utility functions
*
* @author S.Nahas
* @date 3 mars 2009
*
* @version  $LastChangedRevision: 3021 $
* @version $LastChangedDate: 2008-06-11 10:16:10 +0200 (Wed, 11 Jun 2008) $
*
*
* @par LICENCE
* @verbatim
   Copyright (C) 2009  ADDI-DATA GmbH for the source code of this module.

        ADDI-DATA GmbH
        Airport boulevard B210
        D-77836 Rheinmuenster
        Tel: +49(0)7229/1847-0
        Fax: +49(0)7229/1847-200
        http://www.addi-data-com
        info@addi-data.com

   This program is free software; you can redistribute it and/or modify it under
   the terms of the GNU General Public License as published by the Free Software
   Foundation; either version 2 of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
   or FITNESS FOR A PARTICULAR PURPOSE.
   See the GNU General Public License for more details.

   You should have received a copy of the GNU General Public License along with
   this program; if not, write to the Free Software Foundation,
   Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA

   You shoud also find the complete GPL in the COPYING file
   accompanying this source code.
   @endverbatim
*
*
*/




#ifndef ADDIDATA_TIME_H_
#define ADDIDATA_TIME_H_

#ifdef WIN32
	#include <windows.h>
	#define __INLINE__ __inline
#else
	#include <sys/time.h>
	#include <unistd.h>
	#define __INLINE__ inline
#endif

#include <time.h>

/** Display a timeval struct in human string
*
* Convert and display all date and time element
* of a timeval struct in comprehensible outputs: \n
* - day/month/years \n
* - hour:min.:sec.:msec.:usec.
*
* @param [in] time     Time structure to display
* @param [in] msg      A message to display before the time
*/
void addidata_time_display(struct timeval * time, const char * msg);

#ifndef WIN32
int     addidata_time_getsystemtime    (struct timeval* SystemTime);

int     addidata_time_checksystemtime    (struct timeval SystemTimeBegin,
		                                  unsigned long TimeOutS,
		                                  unsigned long TimeOutuS,
		                                  unsigned char* TimeOutStatus);
#endif

/** mimics the Linux sleep() function under MS Windows.
* @param[in] seconds Time to suspend the current process, in seconds.
*
* @note The input parameter has been limited to 16 bits to avoid risks of overflow in the MS Windows version.
*/
static __INLINE__ void addidata_ssleep(unsigned short int seconds)
{
#ifdef WIN32
	// the WMS Windows Sleep function argument is in milliseconds.
	Sleep(seconds*1000);
#else
	sleep(seconds);
#endif
}

#endif /* ADDIDATA_TIME_H_ */
