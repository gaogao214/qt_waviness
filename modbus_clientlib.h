/** @file modbus_clientlib.h MODBUS generic client library
 *
 */

#ifndef _MODBUS_CLIENT_LIB_H_
#define _MODBUS_CLIENT_LIB_H_

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#ifndef _WIN32
   #include <unistd.h>
   #include <sys/types.h>
#else
   #include <conio.h>
#endif
#include <string.h>
#ifndef _WIN32
   #include <stdbool.h>
   #include <syslog.h>
#endif
#include <errno.h>

#ifndef _WIN32
   #include <sys/ioctl.h>
#endif
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#ifndef _WIN32
   #include <getopt.h>
#endif
#include <time.h>
#ifndef _WIN32
   #include <sys/time.h>
#endif
#include <assert.h>

// network related includes
#ifndef _WIN32
   #include <arpa/inet.h>
   #include <sys/socket.h>
   #include <netinet/in.h>
   #include <netinet/ip.h>
   #include <netinet/tcp.h>
   #include <netinet/in.h>
   #include <netdb.h>
#endif

#ifdef _WIN32
   #include "modbus.h"
#else
   #include <modbus.h>
#endif

//---------------------------------------------------------------------------------

/* you may redefine MODBUS_DBG at will, for example:
 * (Linux)
 * #define MODBUS_DBG(args...)  { printf("%s:%d:",__FUNCTION__,__LINE__); printf( args ); }
 * (MS Windows)
 * #define MODBUS_DBG(args...)  { printf("%s:%d:",__FILE__,__LINE__); printf( args ); }
 */

#ifdef _WIN32
	#ifdef __MINGW32__
		#define MODBUS_DBG(...)
	#else
		__inline void MODBUS_DBG(const char *text, ...) {}
	#endif
#else
   #define MODBUS_DBG(args...)
#endif

//---------------------------------------------------------------------------------

/* the macro below is called when an "impossible" condition is encountered.
 * You may redefine it according to the needs of your application
 */
#define MODBUS_ON_INTERNAL_ERROR() { exit(1); }

struct modbus
{
	int lasterrno; /** value of errno after last failed C-library call */
	int fd; /**< holds the socket file descriptor of the modbus connection */
	struct sockaddr_in addr; /**< holds the IP address of the modbus connection */
	modbus_exception_code_t exception; /**< modbus exception code - use modbus_exception_to_str() to get a human readable string */
};

//---------------------------------------------------------------------------------
/** convert exception code to a string */
const char * modbus_exception_to_str(modbus_exception_code_t code);
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
/** generic function to send a MODBUS F3 (read multiple registers) query
 *
 * @param[in] modbus The MODBUS context
 * @param[in] ref The ref field (transaction identifier from client, in theis example, always 0)
 * @param[in] reg Register to access ( a register identify the remote function )
 * @param[in] data buffer to hold the result of the command
 * @param[in] count size in bytes of expected data
 *
 * @return 0 on success, -1 on system error, -2 on MODBUS exception (the exception code is in modbus->exception), -3 if response from remote system is incorrect
 *
 */
int modbus_send_FC3_query(struct modbus * modbus, uint16_t ref, uint16_t reg, void * data, uint16_t count );

//---------------------------------------------------------------------------------
/** generic function to send a MODBUS F16 (write multiple registers) query
 *
 * @param[in] modus MODBUS context
 * @param[in] reg Register to access ( a register identify the remote function )
 * @param[in] data Body of the query (parameters to the remote function)
 * @param[in] count size in bytes of data
 *
 * @return 0 on success, -1 on system error, -2 on MODBUS exception (the exception code is in modbus->exception), -3 if response from remote system is incorrect
 *
 */
int modbus_send_FC16_query(struct modbus * modbus, uint16_t reg, uint16_t ref, char * data, uint16_t count);
//---------------------------------------------------------------------------------
/** generic function to send a MODBUS F23 (read/write registers) query
 *
 * @param[in] modus MODBUS context
 * @param[in] regwrite Register to access ( a register identify the remote function )
 * @param[in] datawrite Body of the query (parameters to the remote function)
 * @param[in] countwrite size in bytes of data
 * @param[in] regread Register to access ( a register identify the remote function )
 * @param[in] dataread buffer to hold the result of the command
 * @param[in] countread size in bytes of expected data
 *
 * @return 0 on success, -1 on system error, -2 on MODBUS exception (the exception code is in modbus->exception), -3 if response from remote system is incorrect
 *
 */
int modbus_send_FC23_query(struct modbus * modbus, uint16_t ref, uint16_t regwrite,  char * datawrite, uint16_t countwrite,uint16_t regread, void * dataread, uint16_t countread);

//---------------------------------------------------------------------------------
/** initialize a modbus structure.
 *
 * @param[in] modus MODBUS context
 */
void modbus_init(struct modbus * modbus);

//---------------------------------------------------------------------------------
/** initialize connection to a remote host.
 *
 * @param[in] modus MODBUS context
 * @param[in] endpoint A string containing the IP address or symbolic name of the remote host
 * @param[in] type One of SOCK_STREAM (TCP/IP) or SOCK_DGRAM (UDP/IP) - IMPORTANT!: by now, only SOCK_DGRAM is implemented
 * @param[in] port The port to use - when set to 0, use the default MODBUS/TCP port number of 512
 *
 * @return 0 on success, -1 on system error, -2 if a parameter is incorrect, -3 if address could not be resolved
 *
 */
int modbus_connect(struct modbus * modbus, char * endpoint, int type, uint16_t port );

//---------------------------------------------------------------------------------
/** shutdown and close MODBUS connection.
 *
 * @param[in] modus MODBUS context
 *
 * @return 0 on success, -1 on error (errno is then set)
 *
 */
int modbus_shutdown(struct modbus * modbus);

/* configuration for a server configured as big endian */
#if 1
/* when configured in big endian mode ( the default), the server waits on port 512 */
#define MODBUS_PORT (512)
#define MODBUS_HTONS(__val) htons(__val)
#define MODBUS_HTONL(__val) htonl(__val)

static __inline uint32_t __ftouint32(float _val)
{
	return *((uint32_t *) (void*) &_val);
}

static __inline float __uint32tof(uint32_t _val)
{
	return * ((float*)(void*) &_val);
}

static __inline float MODBUS_HTONF(float _val)
{
	return (__uint32tof(htonl(__ftouint32(_val))));
}
#define MODBUS_NTOHS(__val) ntohs(__val)
#define MODBUS_NTOHL(__val) ntohl(__val)
static __inline float MODBUS_NTOHF(float _val)
{
	return (__uint32tof(ntohl(__ftouint32(_val))));
}
#else
/* configuration for a server configured as little endian */
/* when configured in little endian mode, the server waits on port 215 */
#define MODBUS_PORT (215)
#define MODBUS_HTONS(__val) (__val)
#define MODBUS_HTONL(__val) (__val)
#define MODBUS_HTONF(__val) (__val)
#define MODBUS_NTOHS(__val) (__val)
#define MODBUS_NTOHL(__val) (__val)
#define MODBUS_NTOHF(__val) (__val)
#endif

/* network timeout, as a struct timeval initialiser */
#define MODBUS_NETWORK_TIMEOUT {10,0} /*modification of the timeout from 5 to 10 because of the rearm function which take too much time (MSXE3711)*/

#endif //_MODBUS_CLIENT_LIB_H_
