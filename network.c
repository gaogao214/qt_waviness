#ifndef _THREAD_SAFE
#define _THREAD_SAFE
#endif

#include <string.h>
#include <stdio.h>

#ifdef _WIN32
#include <winsock2.h>
#include <conio.h>
#else
#include <unistd.h>
#endif

#ifndef _WIN32
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#endif
//---------------------------------------------------------------------------------
#ifdef _WIN32
/* ref: http://msdn.microsoft.com/en-us/library/ms740668(VS.85).aspx */
static const char* addidata_network_WSAstrerror(int err)
{
	switch(err)
	{
	case WSA_INVALID_HANDLE:
		return "Specified event object handle is invalid";
	case WSA_NOT_ENOUGH_MEMORY:
		return "Insufficient memory available";
	case WSA_INVALID_PARAMETER:
		return "One or more parameters are invalid";
	case WSA_OPERATION_ABORTED:
		return "Overlapped operation aborted";
	case WSA_IO_INCOMPLETE:
		return "Overlapped I/O event object not in signaled state";
	case WSA_IO_PENDING:
		return "Overlapped operations will complete later";
	case WSAEINTR:
		return "Interrupted function call";
	case WSAEBADF:
		return "File handle is not valid";
	case WSAEACCES:
		return "Permission denied";
	case WSAEFAULT:
		return "Bad address";
	case WSAEINVAL:
		return "Invalid argument";
	case WSAEMFILE:
		return "Too many open files";
	case WSAEWOULDBLOCK:
		return "Resource temporarily unavailable";
	case WSAEINPROGRESS:
		return "Operation now in progress";
	case WSAEALREADY:
		return "Operation already in progress";
	case WSAENOTSOCK:
		return "Socket operation on nonsocket";
	case WSAEDESTADDRREQ:
		return "Destination address required";
	case WSAEMSGSIZE:
		return "Message too long";
	case WSAEPROTOTYPE:
		return "Protocol wrong type for socket";
	case WSAENOPROTOOPT:
		return "Bad protocol option";
	case WSAEPROTONOSUPPORT:
		return "Protocol not supported";
	case WSAESOCKTNOSUPPORT:
		return "Socket type not supported";
	case WSAEOPNOTSUPP:
		return "Operation not supported";
	case WSAEPFNOSUPPORT:
		return "Protocol family not supported";
	case WSAEAFNOSUPPORT:
		return "Address family not supported by protocol family";
	case WSAEADDRINUSE:
		return "Address already in use";
	case WSAEADDRNOTAVAIL:
		return "Cannot assign requested address";
	case WSAENETDOWN:
		return "Network is down";
	case WSAENETUNREACH:
		return "Network is unreachable";
	case WSAENETRESET:
		return "Network dropped connection on reset";
	case WSAECONNABORTED:
		return "Software caused connection abort";
	case WSAECONNRESET:
		return "Connection reset by peer";
	case WSAENOBUFS:
		return "No buffer space available";
	case WSAEISCONN:
		return "Socket is already connected";
	case WSAENOTCONN:
		return "Socket is not connected";
	case WSAESHUTDOWN:
		return "Cannot send after socket shutdown";
	case WSAETOOMANYREFS:
		return "Too many references";
	case WSAETIMEDOUT:
		return "Connection timed out";
	case WSAECONNREFUSED:
		return "Connection refused";
	case WSAELOOP:
		return "Cannot translate name";
	case WSAENAMETOOLONG:
		return "Name too long";
	case WSAEHOSTDOWN:
		return "Host is down";
	case WSAEHOSTUNREACH:
		return "No route to host";
	case WSAENOTEMPTY:
		return "Directory not empty";
	case WSAEPROCLIM:
		return "Too many processes";
	case WSAEUSERS:
		return "User quota exceeded";
	case WSAEDQUOT:
		return "Disk quota exceeded";
	case WSAESTALE:
		return "Stale file handle reference";
	case WSAEREMOTE:
		return "Item is remote";
	case WSASYSNOTREADY:
		return "Network subsystem is unavailable";
	case WSAVERNOTSUPPORTED:
		return "Winsock.dll version out of range";
	case WSANOTINITIALISED:
		return "Successful WSAStartup not yet performed";
	case WSAEDISCON:
		return "Graceful shutdown in progress";
	case WSAENOMORE:
		return "No more results";
	case WSAECANCELLED:
		return "Call has been canceled";
	case WSAEINVALIDPROCTABLE:
		return "Procedure call table is invalid";
	case WSAEINVALIDPROVIDER:
		return "Service provider is invalid";
	case WSAEPROVIDERFAILEDINIT:
		return "Service provider failed to initialize";
	case WSASYSCALLFAILURE:
		return "System call failure";
	case WSASERVICE_NOT_FOUND:
		return "Service not found";
	case WSATYPE_NOT_FOUND:
		return "Class type not found";
	case WSA_E_NO_MORE:
		return "No more results";
	case WSA_E_CANCELLED:
		return "Call was canceled";
	case WSAEREFUSED:
		return "Database query was refused";
	case WSAHOST_NOT_FOUND:
		return "Host not found";
	case WSATRY_AGAIN:
		return "Nonauthoritative host not found";
	case WSANO_RECOVERY:
		return "non-recoverable error";
	case WSANO_DATA:
		return "Valid name, no data record of requested type";
	case WSA_QOS_RECEIVERS:
		return "QOS receivers";
	case WSA_QOS_SENDERS:
		return "QOS senders";
	case WSA_QOS_NO_SENDERS:
		return "No QOS senders";
	case WSA_QOS_NO_RECEIVERS:
		return "QOS no receivers";
	case WSA_QOS_REQUEST_CONFIRMED:
		return "QOS request confirmed";
	case WSA_QOS_ADMISSION_FAILURE:
		return "QOS admission error";
	case WSA_QOS_POLICY_FAILURE:
		return "QOS policy failure";
	case WSA_QOS_BAD_STYLE:
		return "QOS bad style";
	case WSA_QOS_BAD_OBJECT:
		return "QOS bad object";
	case WSA_QOS_TRAFFIC_CTRL_ERROR:
		return "QOS traffic control error";
	case WSA_QOS_GENERIC_ERROR:
		return "QOS generic error";
		/*case WSA_QOS_ESERVICETYPE:
		return "QOS service type error";
	case WSA_QOS_EFLOWSPEC:
		return "QOS flowspec error";
	case WSA_QOS_EPROVSPECBUF:
		return "Invalid QOS provider buffer";
	case WSA_QOS_EFILTERSTYLE:
		return "Invalid QOS filter style";
	case WSA_QOS_EFILTERTYPE:
		return "Invalid QOS filter type";
	case WSA_QOS_EFILTERCOUNT:
		return "Incorrect QOS filter count";
	case WSA_QOS_EOBJLENGTH:
		return "Invalid QOS object length";
	case WSA_QOS_EFLOWCOUNT:
		return "Incorrect QOS flow count";
	case WSA_QOS_EUNKOWNPSOBJ:
		return "Unrecognized QOS object";
	case WSA_QOS_EPOLICYOBJ:
		return "Invalid QOS policy object";
	case WSA_QOS_EFLOWDESC:
		return "Invalid QOS flow descriptor";
	case WSA_QOS_EPSFLOWSPEC:
		return "Invalid QOS provider-specific flowspec";
	case WSA_QOS_EPSFILTERSPEC:
		return "Invalid QOS provider-specific filterspec";
	case WSA_QOS_ESDMODEOBJ:
		return "Invalid QOS shape discard mode object";
	case WSA_QOS_ESHAPERATEOBJ:
		return "Invalid QOS shaping rate object";
	case WSA_QOS_RESERVED_PETYPE:
		return "Reserved policy QOS element type";*/
	default:
		return "Unknown error";
	}
}
//---------------------------------------------------------------------------------
int addidata_network_init(void)
{
	WSADATA wsaData;
	if (WSAStartup (0x202,&wsaData) == SOCKET_ERROR)
	{
		WSACleanup();
		return -1;
	}
	return 0;
}
//---------------------------------------------------------------------------------
int addidata_network_cleanup(void)
{
	WSACleanup();
	return 0;
}
#endif
//---------------------------------------------------------------------------------
void addidata_network_perror(const char* msg)
{
#ifdef WIN32
	int err = WSAGetLastError();
	printf("%s: %s (WSA %d)\n",msg,addidata_network_WSAstrerror(err),err);
#else
	perror(msg);
#endif
}

//---------------------------------------------------------------------------------
int addidata_network_addrparse(const char* name, struct sockaddr_in* addr)
{
	int found = 0; /* could the address be resolved ? */
	struct hostent* hostinfo;

	(void) memset(addr, 0, sizeof(struct sockaddr_in));

	addr->sin_family = AF_INET;

	/* try as doted network address */
#ifdef WIN32
	found = (INADDR_NONE != (addr->sin_addr.S_un.S_addr = inet_addr(name)) );
#else
	found = inet_aton(name, &(addr->sin_addr));
#endif

	if(found)
		return 0;

	/* try symbolic host name */
	found = ((hostinfo = gethostbyname(name)) != NULL);
	if (found)
	{
		addr->sin_addr = *(struct in_addr *) hostinfo->h_addr;
		return 0;
	}

	return 1;
}

//---------------------------------------------------------------------------------
int addidata_network_ask(const char* prompt, char name[100])
{
	struct sockaddr_in addr;

	printf("%s",prompt);
	memset(name,0,100);

	if(!fgets(name,99,stdin))
		return -1;

	if (addidata_network_addrparse(name,&addr))
		return -1;

	return 0;
}
//---------------------------------------------------------------------------------
int addidata_network_set_socket_timeout(int fd, const struct timeval timeout)
{
#ifdef WIN32
	// takes millisecond
	DWORD to = timeout.tv_sec * 1000 + timeout.tv_usec / 1000;
	if ( setsockopt(fd,SOL_SOCKET,SO_RCVTIMEO,(const char *)&to,sizeof(DWORD)) )
	{
		return -1;
	}
	if ( setsockopt(fd,SOL_SOCKET,SO_SNDTIMEO,(const char *)&to,sizeof(DWORD)) )
	{
		return -1;
	}
#else
	if ( setsockopt(fd,SOL_SOCKET,SO_RCVTIMEO,&timeout,sizeof(struct timeval)) )
	{
		return -1;
	}
	if ( setsockopt(fd,SOL_SOCKET,SO_SNDTIMEO,&timeout,sizeof(struct timeval)) )
	{
		return -1;
	}
#endif
	return 0;
}
