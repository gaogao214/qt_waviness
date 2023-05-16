#ifdef _WIN32
    #include "aes.h"
#else
	#include <aes.h>
#endif

#include <modbusclient.h>

//---------------------------------------------------------------------------------
int handle_result_of_FC3_query(struct modbus * modbus, int ret, const char * fname )
{
	switch(ret)
	{
		case 0: // OK
			break;
		case -1: // system error
			perror(fname);
			return -1;
			break;
		case -2: // MODBUS exception
			switch(modbus->exception)
			{
				default:
					printf("%s: remote system returned a MODBUS exception\n",fname);
					printf("reason: %s (%d)\n",modbus_exception_to_str(modbus->exception),modbus->exception);
					return -1;
					break;
			}
			break;
		case -3: // incorrect response from remote system
			printf("%s: incorrect response from remote system\n",fname);
			return -1;
			break;
		default:
			printf("%s: unknown return code %d from MXCommon__GetModuleType()\n",fname,ret);
			return -1;
			break;
	}
	return 0;
}
//---------------------------------------------------------------------------------
int handle_result_of_FC16_query(struct modbus * modbus, struct GetLastCommandStatus_parameters_t * CommandStatus, int ret, const char * fname )
{
	//Return value check
	switch(ret)
	{
		case 0: // OK
			break;
		case -1: // system error
			perror(fname);
			return -1;
			break;
		case -2: // MODBUS exception
			switch(modbus->exception)
			{
				case MODBUS_REMOTE_EXECUTION_ERROR: // actually a remote exception error and we get it in CommandStatus
					printf("Remote %s command returned: %d\n",fname, CommandStatus->ReturnValue );
					*(CommandStatus->Errstr+sizeof(CommandStatus->Errstr)-1)='\0';
					printf("Remote errno: %d (%s)\n",CommandStatus->Syserrno,CommandStatus->Errstr);
					return -1;
					break;
				default:
					printf("remote system returned a MODBUS exception for command %s",fname);
					printf("reason: %s (%d)\n",modbus_exception_to_str(modbus->exception),modbus->exception);
					return -1;
					break;
			}
			break;
		case -3: // incorrect response from remote system
			printf("%s: incorrect response from remote system\n",fname);
			return -1;
			break;
		case -4:
			printf("%s: remote system returned a MODBUS_REMOTE_EXECUTION_ERROR exception but call to GetLastCommandStatus() failed\n",fname);
			return -1;
			break;
		default:
			printf("%s: unknown return code %d\n",fname,ret);
			return -1;
			break;
	}
	return 0;
}
