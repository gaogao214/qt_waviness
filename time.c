#ifndef _THREAD_SAFE
#define _THREAD_SAFE
#endif

#include <stdio.h>
#include <string.h>
#include <addidata_time.h>

//---------------------------------------------------------------------------------
void addidata_time_display (struct timeval * time, const char * msg)
{
	struct tm *tm_ptr;
	time_t useconds, mseconds;

	tm_ptr = localtime (&(time->tv_sec));
	useconds = time->tv_usec;
	mseconds = useconds/1000;
	useconds -= (mseconds * 1000);
	printf ("%s: %ld - %02d/%02d/%04d-%02d:%02d:%02d:%03ld:%03ld\n", msg, time->tv_sec, tm_ptr->tm_mday, (tm_ptr->tm_mon+1), (tm_ptr->tm_year + 1900), tm_ptr->tm_hour, tm_ptr->tm_min, tm_ptr->tm_sec, mseconds, useconds);
}
//---------------------------------------------------------------------------------
#ifndef WIN32
int addidata_time_getsystemtime    (struct timeval* SystemTime)
{
	int i_ReturnValue = 0;

	memset (SystemTime, 0, sizeof (struct timeval));

	gettimeofday (SystemTime,NULL);


	return (i_ReturnValue);

}
//---------------------------------------------------------------------------------
int addidata_time_checksystemtime    (struct timeval SystemTimeBegin,
		                                  unsigned long TimeOutS,
		                                  unsigned long TimeOutuS,
		                                  unsigned char* TimeOutStatus)
{
	int i_ReturnValue = 0;
	struct timeval SystemTimeActual;
	struct timeval DiffTime;

	*TimeOutStatus = 0;

	memset (&SystemTimeActual, 0, sizeof (struct timeval));

	gettimeofday (&SystemTimeActual,NULL);

	DiffTime.tv_sec = SystemTimeActual.tv_sec - SystemTimeBegin.tv_sec;
	if (SystemTimeActual.tv_usec >= SystemTimeBegin.tv_usec)
		DiffTime.tv_usec = SystemTimeActual.tv_usec - SystemTimeBegin.tv_usec;
	else
	{
		DiffTime.tv_usec = (1000000- SystemTimeBegin.tv_usec) + SystemTimeActual.tv_usec;
		DiffTime.tv_sec = DiffTime.tv_sec - 1;
	}
	if ((TimeOutS <= DiffTime.tv_sec) && (TimeOutuS <= DiffTime.tv_usec))
		*TimeOutStatus = 1;


	return (i_ReturnValue);

}
#endif
