#ifndef _THREAD_SAFE
#define _THREAD_SAFE
#endif

#include <stdio.h>
#ifdef _WIN32
	#include <io.h>
	#include <conio.h>
	#include <windows.h>
	#define read _read
	#ifndef __MINGW32__
		#define STDIN_FILENO (0)
	#endif
#else
	#define _THREAD_SAFE
	#define _GNU_SOURCE
	#include <unistd.h>
	#include <sched.h>
	#include <signal.h>
	#include <pthread.h>
	#include <termios.h>
	#include <stdbool.h>
#endif
#include <errno.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include <addidata_console.h>

//---------------------------------------------------------------------------------
/* safely read an integer from standard input - loop until input is correct */
int addidata_console_get_integer(const char *prompt, int min, int max)
{
	int val;
	if(prompt) printf(prompt);
	do
	{
		char buff[10];
		char * endptr = NULL;
		printf("(%d-%d)", min, max);
		fgets(buff, sizeof(buff) - 1, stdin);
		errno=0;
		val = strtol(buff, &endptr, 10);
		if ((errno == ERANGE && (val == LONG_MAX || val == LONG_MIN)) || (errno
				!= 0 && val == 0))
		{
			perror("strtol");
			continue;
		}
		if (endptr == buff)
		{
			fprintf(stderr, "input is not a digit\n");
			continue;
		}
		if ((val >= min) && (val <= max))
			return val;
		fprintf(stderr, "input is out of range\n");
	}
	while (1);
}
//---------------------------------------------------------------------------------
/* safely read an unsigned integer from standard input - loop until input is correct */
unsigned int addidata_console_get_unsigned_integer(const char *prompt, unsigned int min, unsigned int max)
{
	unsigned int val;
	if(prompt) printf(prompt);
	do
	{
		char buff[10];
		char * endptr = NULL;
		printf("(%u-%u)", min, max);
		fgets(buff, sizeof(buff) - 1, stdin);
		errno=0;
		val = strtoul(buff, &endptr, 10);
		if ((errno == ERANGE && (val == ULONG_MAX || val == 0)) || (errno
				!= 0 && val == 0))
		{
			perror("strtoul");
			continue;
		}
		if (endptr == buff)
		{
			fprintf(stderr, "input is not a digit\n");
			continue;
		}
		if ((val >= min) && (val <= max))
			return val;
		fprintf(stderr, "input is out of range\n");
	}
	while (1);
}
//---------------------------------------------------------------------------------
/* safely read an integer from standard input - loop until input is correct */
int addidata_console_get_hexa(const char *prompt, int min, int max)
{
	int val;
	if(prompt) printf(prompt);
	do
	{
		char buff[10];
		char * endptr = NULL;
		printf("(0x%X-0x%X)", min, max);
		fgets(buff, sizeof(buff) - 1, stdin);
		errno=0;
		val = strtol(buff, &endptr, 16);
		if ((errno == ERANGE && (val == LONG_MAX || val == LONG_MIN)) || (errno
				!= 0 && val == 0))
		{
			perror("strtol");
			continue;
		}
		if (endptr == buff)
		{
			fprintf(stderr, "input is not a digit\n");
			continue;
		}
		if ((val >= min) && (val <= max))
			return val;
		fprintf(stderr, "input is out of range\n");
	}
	while (1);
}
//---------------------------------------------------------------------------------
/* safely read an unsigned integer from standard input - loop until input is correct */
unsigned int addidata_console_get_unsigned_hexa(const char *prompt, unsigned int min, unsigned int max)
{
	unsigned int val;
	if(prompt) printf(prompt);
	do
	{
		char buff[10];
		char * endptr = NULL;
		printf("(0x%X-0x%X)", min, max);
		fgets(buff, sizeof(buff) - 1, stdin);
		errno=0;
		val = strtoul(buff, &endptr, 16);
		if ((errno == ERANGE && (val == ULONG_MAX || val == 0)) || (errno
				!= 0 && val == 0))
		{
			perror("strtoul");
			continue;
		}
		if (endptr == buff)
		{
			fprintf(stderr, "input is not a digit\n");
			continue;
		}
		if ((val >= min) && (val <= max))
			return val;
		fprintf(stderr, "input is out of range\n");
	}
	while (1);
}
//---------------------------------------------------------------------------------
int addidata_console_get_yn(char* prompt)
{
	printf("%s (0: No, 1: Yes)",prompt);
	return addidata_console_get_integer(NULL, 0, 1);
}
//---------------------------------------------------------------------------------
#ifndef WIN32
static struct termios saved_attributes;
static void reset_input_mode (void)
{
  tcsetattr (STDIN_FILENO, TCSANOW, &saved_attributes);
}
static void reset_input_mode_pthread (void *ign)
{
	reset_input_mode();
}
#endif
//---------------------------------------------------------------------------------
int addidata_console_wait_for_character(void)
{
#ifdef _WIN32
	while(1)
		if(_kbhit())
			return _getch();
#else
	char a;
	struct termios tattr;
	int theend = false;
	if (isatty (STDIN_FILENO))
	{
		tcgetattr (STDIN_FILENO, &saved_attributes);
		atexit (reset_input_mode);

		tcgetattr (STDIN_FILENO, &tattr);
		tattr.c_lflag &= ~(ICANON|ECHO); /* Clear ICANON and ECHO. */
		tattr.c_cc[VMIN] = 1;
		tattr.c_cc[VTIME] = 0;
		tcsetattr (STDIN_FILENO, TCSAFLUSH, &tattr);
	}

	pthread_cleanup_push(reset_input_mode_pthread,NULL);
	while(!theend)
	{
		if ( 1 == read(STDIN_FILENO,&a,1) )
		{
			theend = true;
		}
	}
	pthread_cleanup_pop(1);
	return a;
#endif
}
//---------------------------------------------------------------------------------
void addidata_console_wait_for_input(void)
{
#ifndef WIN32
	/* don't wait if input is not a TTY */
	if (!isatty(STDIN_FILENO))
		return;
#endif
	addidata_console_wait_for_character();
}
//---------------------------------------------------------------------------------
int addidata_fprintf_timestamp(FILE* stream, const char* termfmt, uint32_t seconds, uint32_t useconds)
{
	const char* fmt = "%02d/%02d/%04d-%02d:%02d:%02d:%03ld:%03ld%s";
	const time_t tmpseconds = seconds;
	struct tm *tmptr = NULL;
	uint32_t mseconds;

	tmptr = localtime (&tmpseconds);
	if(!tmptr)
		return -1;
	mseconds = useconds/1000;
	useconds -= (mseconds * 1000);
	return fprintf (stream, fmt, tmptr->tm_mday, (tmptr->tm_mon+1), (tmptr->tm_year + 1900), tmptr->tm_hour, tmptr->tm_min, tmptr->tm_sec, mseconds, useconds, termfmt);
}
#ifdef WIN32
//---------------------------------------------------------------------------------
static DWORD WINAPI _wait_key_input_thread(LPVOID param)
{
	int character = ((wait_key_input_thread_parameters_t*) param)->character;
	unsigned int *flag = ((wait_key_input_thread_parameters_t*) param)->flag;
	while( addidata_console_wait_for_character() != character );
	*flag = 1;
	return 0;
}

//---------------------------------------------------------------------------------
key_thread_handle_t addidata_start_key_thread(wait_key_input_thread_parameters_t *params)
{
	DWORD tid;
	key_thread_handle_t thandle;
	thandle = CreateThread (	NULL /*Choose default security*/,
			0 /*Default stack size*/,
			&_wait_key_input_thread,
			params,
			0, //Immediately run the thread
			&tid);
	return thandle;
}
//---------------------------------------------------------------------------------
void addidata_stop_key_thread(key_thread_handle_t thandle)
{
	TerminateThread(&thandle,0);
	CloseHandle(thandle);
}
#else
//---------------------------------------------------------------------------------
static void* _wait_key_input_thread(void *param)
{
	int character = ((wait_key_input_thread_parameters_t*) param)->character;
	unsigned int *flag = ((wait_key_input_thread_parameters_t*) param)->flag;
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL);
	while( addidata_console_wait_for_character() != character );
	*flag = 1;
	return NULL;
}

//---------------------------------------------------------------------------------
key_thread_handle_t addidata_start_key_thread(wait_key_input_thread_parameters_t *params)
{
	key_thread_handle_t hdl = malloc(sizeof(pthread_t));
	if(hdl==NULL)
		return NULL;
	if( pthread_create(hdl, NULL, _wait_key_input_thread, params) )
	{
		free(hdl);
		return NULL;
	}
	return hdl;
}
//---------------------------------------------------------------------------------
void addidata_stop_key_thread(key_thread_handle_t thandle)
{
	if(thandle==NULL)
		return;
	if( pthread_cancel(*thandle) == 0 )
		pthread_join(*thandle,NULL);
	free(thandle);
	reset_input_mode();
}
#endif
