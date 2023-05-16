/** @file modbuscli.c MODBUS/UDP client sample for the MSXE3711
 *
 * FC16 queries are for setting things, FC3 for fetching values
 *
 * This code doesn't handle timeouts.
 */
#ifdef _WIN32
#define sleep Sleep
#include "../../Interface_Library/msxe371x_modbus_clientlib.h"
#else
#include <msxe371x_modbus_clientlib.h>
#include <unistd.h>
#endif

#include <addidata_console.h>
#include <addidata_network.h>
#include <addidata_dataclient.h>
#include <addidata_time.h>

#include <modbusclient.h>
#include <commonclient.h>


//---------------------------------------------------------------------------------
uint32_t TRANSDUCER_SELECTION = 0;

//---------------------------------------------------------------------------------
int sample_MX371x__getInternalTemperature(struct modbus * modbus, float *InternalTemperature)
{
	struct MSXE371x__InternalTemperatureRead_parameters_t Response;
	int ret = 0;

	memset(&Response,0,sizeof(Response));

	ret = modbus_call_MSXE371x__InternalTemperatureRead(modbus, &Response);

	if (handle_result_of_FC3_query(modbus, ret, "modbus_call_MSXE371x__InternalTemperatureRead"))
		return -1;

	*InternalTemperature = Response.fInternalTemperature;
	printf("internal temperature : %f degree Celsius\n", *InternalTemperature);
	return 0;
}

//---------------------------------------------------------------------------------
int sample_MX371x__getNumberOfChannels(struct modbus * modbus, uint32_t* ChannelNumber)
{
	struct MX371x__getNumberOfChannels_parameters_t Response;
	int ret = 0;

	memset(&Response,0,sizeof(Response));

	ret = modbus_call_MX371x__getNumberOfChannels(modbus,&Response);

	if (handle_result_of_FC3_query(modbus, ret, "modbus_call_MX371x__getNumberOfChannels"))
		return -1;

	*ChannelNumber = Response.ChannelNumber;
	printf("number of channel(s) on the module: %d\n",Response.ChannelNumber);
	return 0;
}
//---------------------------------------------------------------------------------
int sample_MX371x__getNumberOfChannelsEx(struct modbus * modbus, uint32_t* ChannelNumber)
{
	struct MX371x__getNumberOfChannelsEx_parameters_t Response;
	int ret = 0;

	memset(&Response,0,sizeof(Response));

	ret = modbus_call_MX371x__getNumberOfChannelsEx(modbus,&Response);

	if (handle_result_of_FC3_query(modbus, ret, "modbus_call_MX371x__getNumberOfChannelsEx"))
		return -1;

	*ChannelNumber = Response.ChannelNumber;
	printf("number of channel on the module: %d\n",Response.ChannelNumber);
	return 0;
}

//---------------------------------------------------------------------------------
int sample_MX371x__TransducerGetAutoRefreshValues(struct modbus * modbus, int NumberOfChannels)
{
	struct MX371x__TransducerGetAutoRefreshValues_parameters_t Response;
	int ret = 0;

	memset(&Response,0,sizeof(Response));

	ret = modbus_call_MX371x__TransducerGetAutoRefreshValues(modbus,&Response);

	if (!handle_result_of_FC3_query(modbus, ret, "modbus_call_MX371x__TransducerGetAutoRefreshValues"))
	{
		int i;
		printf("%X",Response.Value[0]);
		for (i=1;i<=NumberOfChannels;i++)
			printf(" %f", *((float*)&Response.Value[i]));
		printf("\n");
		return 0;
	}

	return -1;
}
//---------------------------------------------------------------------------------
int sample_MX371x__TransducerGetAutoRefreshValuesEx(struct modbus * modbus, int NumberOfChannels)
{
	struct MX371x__TransducerGetAutoRefreshValuesEx_parameters_t Response;
	int ret = 0;

	memset(&Response,0,sizeof(Response));

	ret = modbus_call_MX371x__TransducerGetAutoRefreshValuesEx(modbus,&Response);

	if (!handle_result_of_FC3_query(modbus, ret, "modbus_call_MX371x__TransducerGetAutoRefreshValuesEx"))
	{
		int i;
		printf("%X",Response.Value[0]);
		for (i=1;i<=NumberOfChannels;i++)
			printf(" %f mm", *((float*)&Response.Value[i]));
		printf("\n");
		return 0;
	}

	return -1;
}


//---------------------------------------------------------------------------------
int sample_MX371x__TransducerInitAndStartAutoRefresh(struct modbus * modbus)
{
	struct GetLastCommandStatus_parameters_t CommandStatus;
	int ret = 0;

	uint32_t TransducerSelection = TRANSDUCER_SELECTION;
	uint32_t ChannelMask = 0x1;
	uint32_t AverageMode = 0;
	uint32_t AverageValue = 0;
	uint32_t TriggerMask = 0;
	uint32_t TriggerMode = 0;
	uint16_t HardwareTriggerCount = 0;
	uint16_t HardwareTriggerEdge = 0;
	uint32_t ByTriggerNbrOfSeqToAcquire = 1;
	uint32_t DataFormat = 1;
	uint32_t Option1 = 0;
	uint32_t Option2 = 0;
	uint32_t Option3 = 0;
	uint32_t Option4 = 0;

	ret = modbus_call_MX371x__TransducerInitAndStartAutoRefresh(modbus,TransducerSelection,ChannelMask,AverageMode,AverageValue,TriggerMask,TriggerMode,HardwareTriggerEdge,HardwareTriggerCount,ByTriggerNbrOfSeqToAcquire,DataFormat,Option1,Option2,Option3,Option4,&CommandStatus);

	return handle_result_of_FC16_query(modbus, &CommandStatus, ret, "modbus_call_MX371x__TransducerInitAndStartAutoRefresh");

}
//---------------------------------------------------------------------------------
int sample_MX371x__TransducerInitAndStartAutoRefreshEx(struct modbus * modbus)
{
	struct GetLastCommandStatus_parameters_t CommandStatus;
	int ret = 0;

	uint32_t TransducerSelection = TRANSDUCER_SELECTION;
	uint32_t ChannelMask = 0x1;
	uint32_t AverageMode = 0;
	uint32_t AverageValue = 0;
	uint32_t TriggerMask = 0;
	uint32_t TriggerMode = 0;
	uint16_t HardwareTriggerCount = 0;
	uint16_t HardwareTriggerEdge = 0;
	uint32_t ByTriggerNbrOfSeqToAcquire = 1;
	uint32_t DataFormat = 3;
	uint32_t Option1 = 0;
	uint32_t Option2 = 0;
	uint32_t Option3 = 0;
	uint32_t Option4 = 0;

	ret = modbus_call_MX371x__TransducerInitAndStartAutoRefreshEx(modbus,TransducerSelection,ChannelMask,AverageMode,AverageValue,TriggerMask,TriggerMode,HardwareTriggerEdge,HardwareTriggerCount,ByTriggerNbrOfSeqToAcquire,DataFormat,Option1,Option2,Option3,Option4,&CommandStatus);

	return handle_result_of_FC16_query(modbus, &CommandStatus, ret, "modbus_call_MX371x__TransducerInitAndStartAutoRefreshEx");
}

//---------------------------------------------------------------------------------
int sample_MX371x__TransducerStopAndReleaseAutoRefresh(struct modbus * modbus)
{
	struct GetLastCommandStatus_parameters_t CommandStatus;
	int ret = 0;
	uint32_t Dummy = 0;

	ret = modbus_call_MX371x__TransducerStopAndReleaseAutoRefresh(modbus,Dummy,&CommandStatus);

	return handle_result_of_FC16_query(modbus, &CommandStatus, ret, "modbus_call_MX371x__TransducerStopAndReleaseAutoRefresh");
}
//---------------------------------------------------------------------------------
int sample_MX371x__TransducerStopAndReleaseAutoRefreshEx(struct modbus * modbus)
{
	struct GetLastCommandStatus_parameters_t CommandStatus;
	int ret = 0;
	uint32_t Dummy = 0;

	ret = modbus_call_MX371x__TransducerStopAndReleaseAutoRefreshEx(modbus,Dummy,&CommandStatus);

	return handle_result_of_FC16_query(modbus, &CommandStatus, ret, "modbus_call_MX371x__TransducerStopAndReleaseAutoRefreshEx");
}

//---------------------------------------------------------------------------------
int sample_MX371x__TransducerInitAndStartSequence(struct modbus * modbus)
{
	struct GetLastCommandStatus_parameters_t CommandStatus;
	int ret = 0;

	uint32_t TransducerSelection = TRANSDUCER_SELECTION;
	uint32_t NbrOfChannel = 4;
	uint32_t ChannelList[16] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
	uint32_t NbrOfSequence = 0;
	uint32_t NbrMaxSequenceToTransfer = 1;
	uint32_t DelayMode = 0;
	uint32_t DelayTimeUnit = 0;
	uint32_t DelayValue = 0;
	uint32_t TriggerMask = 0;
	uint32_t TriggerMode = 0;
	uint32_t HardwareTriggerCount = 0;
	uint32_t HardwareTriggerEdge = 0;
	uint32_t ByTriggerNbrOfSeqToAcquire = 1;
	uint32_t DataFormat = 3;
	uint32_t Option1 = 0;
	uint32_t Option2 = 0;
	uint32_t Option3 = 0;
	uint32_t Option4 = 0;

	ret = modbus_call_MX371x__TransducerInitAndStartSequence(modbus,TransducerSelection,NbrOfChannel,ChannelList,NbrOfSequence,NbrMaxSequenceToTransfer,DelayMode,DelayTimeUnit,DelayValue,TriggerMask,TriggerMode,HardwareTriggerEdge,HardwareTriggerCount,ByTriggerNbrOfSeqToAcquire,DataFormat,Option1,Option2,Option3,Option4,&CommandStatus);

	return handle_result_of_FC16_query(modbus, &CommandStatus, ret, "modbus_call_MX371x__TransducerInitAndStartSequence");
}

//---------------------------------------------------------------------------------
int sample_MX371x__TransducerStopAndReleaseSequence(struct modbus * modbus)
{
	struct GetLastCommandStatus_parameters_t CommandStatus;
	int ret = 0;
	uint32_t Dummy = 0;

	/* Remote function call */
	ret = modbus_call_MX371x__TransducerStopAndReleaseSequence(modbus,Dummy,&CommandStatus);

	return handle_result_of_FC16_query(modbus, &CommandStatus, ret, "modbus_call_MX371x__TransducerStopAndReleaseSequence");
}

//---------------------------------------------------------------------------------
int sample_MX371x__TransducerInitAndStartSequenceEx(struct modbus * modbus, char *address)
{
	struct GetLastCommandStatus_parameters_t CommandStatus;
	int      ret                        = 0;

	uint32_t TransducerSelection        = TRANSDUCER_SELECTION;
	uint32_t NbrOfChannel               = 4;
	uint32_t ChannelList[16]            = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
	uint32_t NbrOfSequence              = 10;
	uint32_t NbrMaxSequenceToTransfer   = 1;
	uint32_t DelayMode                  = 0;
	uint32_t DelayTimeUnit              = 0;
	uint32_t DelayValue                 = 0;
	uint32_t TriggerMask                = 0;
	uint32_t TriggerMode                = 0;
	uint32_t HardwareTriggerCount       = 0;
	uint32_t HardwareTriggerEdge        = 0;
	uint32_t ByTriggerNbrOfSeqToAcquire = 1;
	uint32_t DataFormat                 = 6;
	uint32_t Option1                    = 0;
	uint32_t Option2                    = 0;
	uint32_t Option3                    = 0;
	uint32_t Option4                    = 0;

	/* Dataserver connection */
	int          fd;
	uint32_t     *data;
	unsigned int count = NbrOfChannel + 1;

	data = malloc(count * 4);
	if (!data)
	{
		perror("malloc");
		exit(1);
	}

	/* Connect to data server */
	switch (addidata_dataclient_tcp_connect(address, 0, &fd))
	{
	case 0:
		break;
	case -1:
		addidata_network_perror("socket");
		exit(1);
	case -2:
		addidata_network_perror("connect");
		exit(1);
	case -3:
		printf("error while parsing address\n");
		exit(1);
	case -4:
		addidata_network_perror("WSACleanup() failed");
		exit(1);
	}

	/* Set timeout */
	{
		struct timeval timeout = MODBUS_NETWORK_TIMEOUT;
		if (addidata_network_set_socket_timeout(fd, timeout))
		{
			addidata_network_perror("setting timeout");
			exit(1);
		}
	}

	printf("\npress a key to start this sample.\n");
	fflush(stdout);
	addidata_console_wait_for_input();

	ret = modbus_call_MX371x__TransducerInitAndStartSequenceEx(modbus,
			TransducerSelection,
			NbrOfChannel,
			ChannelList,
			NbrOfSequence,
			NbrMaxSequenceToTransfer,
			DelayMode,
			DelayTimeUnit,
			DelayValue,
			TriggerMask,
			TriggerMode,
			HardwareTriggerEdge,
			HardwareTriggerCount,
			ByTriggerNbrOfSeqToAcquire,
			DataFormat,
			Option1,
			Option2,
			Option3,
			Option4,
			&CommandStatus);

	if (handle_result_of_FC16_query(modbus, &CommandStatus, ret, "modbus_call_MX371x__TransducerInitAndStartSequenceEx") != 0)
		return -1;

	/* Get sequence data */
	{
		key_thread_handle_t kt;
		unsigned int stop = 0;
		unsigned int i    = 0;
		wait_key_input_thread_parameters_t ktparams;
		ktparams.character = 27; /* ESC */
		ktparams.flag = &stop;
		kt = addidata_start_key_thread(&ktparams);
		while (1)
		{
			if (-1 == addidata_dataclient_recvall(fd, data, count))
			{
				addidata_network_perror("fetch");
				if (sample_MX371x__TransducerStopAndReleaseSequence(modbus) != 0)
					printf("sample_MX371x__TransducerStopAndReleaseSequence error\n");

				ret = -1;
				goto function_end;
			}

			printf("Sequence number = %d\nChannel 0: %f mm\nChannel 1: %f mm\nChannel 2: %f mm\nChannel 3: %f mm\n\n", ((uint32_t *)data)[0], ((float *)data)[1], ((float *)data)[2], ((float *)data)[3], ((float *)data)[4]);

			i += count;

			if (stop || (i == (count * NbrOfSequence)))
				break;
		}
		addidata_stop_key_thread(kt);
	}

function_end:
	addidata_dataclient_disconnect(fd);
	free(data);
	return ret;
}

//---------------------------------------------------------------------------------
int sample_MX371x__TransducerStopAndReleaseSequenceEx(struct modbus * modbus)
{
	struct GetLastCommandStatus_parameters_t CommandStatus;
	int      ret   = 0;
	uint32_t Dummy = 0;

	ret = modbus_call_MX371x__TransducerStopAndReleaseSequenceEx(modbus,Dummy,&CommandStatus);

	return handle_result_of_FC16_query(modbus, &CommandStatus, ret, "modbus_call_MX371x__TransducerStopAndReleaseSequenceEx");
}

//---------------------------------------------------------------------------------
int getModuleHardwareType(struct modbus * modbus, uint32_t *hartwareType)
{
	struct MXCommon__GetModuleTypeEx_parameters_t Result;
	int ret = 0;

	memset(&Result,0,sizeof(Result));

	ret = modbus_call_MXCommon__GetModuleTypeEx(modbus,&Result);

	if (handle_result_of_FC3_query(modbus, ret, "modbus_call_MXCommon__GetModuleTypeEx"))
		return -1;

	if (strstr((char *)Result.str, "{2000}"))
		*hartwareType = 2;
	else
		*hartwareType = 1;

	return 0;
}

//---------------------------------------------------------------------------------
int sample_MSXE371x__ExternalTemperatureInit (struct modbus * modbus)
{
	struct GetLastCommandStatus_parameters_t CommandStatus;
	int ret = 0;
	uint32_t hardwareType        = 0;
	uint32_t ConnectedTempSensor = 100; /* PT100 */
	uint32_t FrequencySelection  = 10;
	uint32_t ConvertMode         = 1;   /* Return °C */
	uint32_t GainSelection       = 0;   /* Auto gain */
	uint32_t PowerSaveMode       = 0;
	uint32_t Option01            = 0;
	uint32_t Option02            = 0;
	uint32_t Option03            = 0;
	uint32_t Option04            = 0;

	if (getModuleHardwareType(modbus, &hardwareType) != 0)
		return -1;

	/* Test if TC system */
	if (hardwareType == 2)
	{
		/* Type K */
		ConnectedTempSensor  = 3;
		FrequencySelection   = 50;
	}

	ret = modbus_call_MSXE371x__ExternalTemperatureInit(modbus,ConnectedTempSensor,ConvertMode,GainSelection,FrequencySelection,PowerSaveMode,Option01,Option02,Option03,Option04,&CommandStatus);

	return handle_result_of_FC16_query(modbus, &CommandStatus, ret, "modbus_call_MSXE371x__ExternalTemperatureInit");
}

//---------------------------------------------------------------------------------
int sample_MSXE371x__ExternalTemperatureInitEx (struct modbus * modbus)
{
	struct GetLastCommandStatus_parameters_t CommandStatus;
	int ret = 0;
	uint32_t hardwareType        = 0;
	uint32_t ConnectedTempSensor = 100; /* PT100 */
	uint32_t FrequencySelection  = 10;
	uint32_t ConvertMode         = 1;   /* Return °C */
	uint32_t GainSelection       = 0;   /* Auto gain */
	uint32_t PowerSaveMode       = 0;
	uint32_t Option01            = 0;
	uint32_t Option02            = 0;
	uint32_t Option03            = 0;
	uint32_t Option04            = 0;

	if (getModuleHardwareType(modbus, &hardwareType) != 0)
		return -1;

	/* Test if TC system */
	if (hardwareType == 2)
	{
		/* Type K */
		ConnectedTempSensor  = 3;
		FrequencySelection   = 50;
	}

	ret = modbus_call_MSXE371x__ExternalTemperatureInitEx(modbus,ConnectedTempSensor,ConvertMode,GainSelection,FrequencySelection,PowerSaveMode,Option01,Option02,Option03,Option04,&CommandStatus);

	return handle_result_of_FC16_query(modbus, &CommandStatus, ret, "modbus_call_MSXE371x__ExternalTemperatureInitEx");
}

//---------------------------------------------------------------------------------
int sample_MSXE371x__ExternalTemperatureRelease (struct modbus * modbus)
{
	struct GetLastCommandStatus_parameters_t CommandStatus;
	int ret = 0;
	uint32_t Dummy = 0;

	ret = modbus_call_MSXE371x__ExternalTemperatureRelease(modbus,Dummy,&CommandStatus);

	return handle_result_of_FC16_query(modbus, &CommandStatus, ret, "modbus_call_MSXE371x__ExternalTemperatureRelease");
}

//---------------------------------------------------------------------------------
int sample_MSXE371x__ExternalTemperatureReleaseEx (struct modbus * modbus)
{
	struct GetLastCommandStatus_parameters_t CommandStatus;
	int ret = 0;
	uint32_t Dummy = 0;

	ret = modbus_call_MSXE371x__ExternalTemperatureReleaseEx(modbus,Dummy,&CommandStatus);

	return handle_result_of_FC16_query(modbus, &CommandStatus, ret, "modbus_call_MSXE371x__ExternalTemperatureReleaseEx");
}

//---------------------------------------------------------------------------------
int sample_MSXE371x__ExternalTemperatureRead (struct modbus * modbus)
{
	struct MSXE371x__ExternalTemperatureRead_parameters_t Response;
	int ret = 0;
	struct timeval tv;
	float *degrees = (float*) &Response.Value; /* Permit to read a float value (for Kelvin or Degrees) */

	memset(&Response,0,sizeof(Response));

	ret = modbus_call_MSXE371x__ExternalTemperatureRead(modbus,&Response);

	if (!handle_result_of_FC3_query(modbus, ret, "modbus_call_MSXE371x__ExternalTemperatureRead"))
	{
		printf("Temperature value %.2f degree Celsius ", *degrees);
		tv.tv_usec = Response.TimeStampLow;
		tv.tv_sec = Response.TimeStampHigh;
		addidata_time_display((struct timeval *) (&tv), " Timestamp ");
		return 0;
	}

	return -1;
}
//---------------------------------------------------------------------------------
int sample_MSXE371x__ExternalTemperatureReadEx (struct modbus * modbus)
{
	struct MSXE371x__ExternalTemperatureReadEx_parameters_t Response;
	int ret = 0;
	struct timeval tv;
	float *degrees = (float*) &Response.Value; /* Permit to read a float value (for Kelvin or Degrees) */

	memset(&Response,0,sizeof(Response));

	ret = modbus_call_MSXE371x__ExternalTemperatureReadEx(modbus,&Response);

	if (!handle_result_of_FC3_query(modbus, ret, "modbus_call_MSXE371x__ExternalTemperatureReadEx"))
	{
		printf("Temperature value %.2f degree Celsius ", *degrees);
		tv.tv_usec = Response.TimeStampLow;
		tv.tv_sec = Response.TimeStampHigh;
		addidata_time_display((struct timeval *) (&tv), " Timestamp ");
		return 0;
	}

	return -1;
}


//---------------------------------------------------------------------------------
int sample_MSXE371x__IncCounterInit(struct modbus * modbus)
{
	struct GetLastCommandStatus_parameters_t CommandStatus;
	int ret = 0;

	uint32_t ulCounterMode = 0x1;
	uint32_t ulCounterOption = 0;
	uint32_t ulOption01 = 0;
	uint32_t ulOption02 = 0;
	uint32_t ulOption03 = 0;
	uint32_t ulOption04 = 0;

	/* Remote function call */
	ret = modbus_call_MSXE371x__IncCounterInit(modbus,ulCounterMode,ulCounterOption,ulOption01,ulOption02,ulOption03,ulOption04,&CommandStatus);

	/* Return value check */
	return handle_result_of_FC16_query(modbus, &CommandStatus, ret, "modbus_call_MSXE371x__IncCounterInit");
}

//---------------------------------------------------------------------------------
int sample_MSXE371x__IncCounterRead32BitsValue(struct modbus * modbus, uint32_t *pulValue, uint32_t *pulTimeStampLow, uint32_t *pulTimeStampHigh)
{
	struct MSXE371x__IncCounterRead32BitValue_parameters_t Response;
	int ret = 0;

	memset(&Response,0,sizeof(Response));

	ret = modbus_call_MSXE371x__IncCounterRead32BitValue(modbus,&Response);

	if (handle_result_of_FC3_query(modbus, ret, "modbus_call_MSXE371x__IncCounterRead0"))
	{
		return -1;
	}

	*pulValue         = Response.ulValue;
	*pulTimeStampLow  = Response.ulTimeStampLow;
	*pulTimeStampHigh = Response.ulTimeStampHigh;

	printf("Counter/position: %d\n",*pulValue);
	printf("Time stamp low  : %u\n", *pulTimeStampLow);
	printf("Time stamp high : %u\n", *pulTimeStampHigh);

	return 0;
}


//---------------------------------------------------------------------------------
int sample_MSXE371x__IncCounterWrite32BitValue(struct modbus * modbus)
{
	struct GetLastCommandStatus_parameters_t CommandStatus;
	int ret = 0;

	uint32_t ulCounterValue = 1000;

	/* Remote function call */
	ret = modbus_call_MSXE371x__IncCounterWrite32BitValue(modbus, ulCounterValue, &CommandStatus);

	/* Return value check */
	return handle_result_of_FC16_query(modbus, &CommandStatus, ret, "modbus_call_MSXE371x__IncCounterWrite32BitValue");
}

//---------------------------------------------------------------------------------
int sample_MSXE371x__IncCounterInitAndEnableIndex(struct modbus * modbus, uint32_t ulReferenceAction, uint32_t ulIndexOperation, uint32_t ulAutoMode)
{
	struct GetLastCommandStatus_parameters_t CommandStatus;
	int ret = 0;
	uint32_t ulOption01 = 0;

	/* Remote function call */
	ret = modbus_call_MSXE371x__IncCounterInitAndEnableIndex(modbus, ulReferenceAction, ulIndexOperation, ulAutoMode, ulOption01, &CommandStatus);

	/* Return value check */
	return handle_result_of_FC16_query(modbus, &CommandStatus, ret, "modbus_call_MSXE371x__IncCounterInitAndEnableIndex");
}

//---------------------------------------------------------------------------------
int sample_MSXE371x__IncCounterDisableAndReleaseIndex(struct modbus * modbus)
{
	struct GetLastCommandStatus_parameters_t CommandStatus;
	int ret = 0;
	uint32_t ulOption01 = 0;

	/* Remote function call */
	ret = modbus_call_MSXE371x__IncCounterDisableAndReleaseIndex(modbus, ulOption01, &CommandStatus);

	/* Return value check */
	return handle_result_of_FC16_query(modbus, &CommandStatus, ret, "modbus_call_MSXE371x__IncCounterDisableAndReleaseIndex");
}

//---------------------------------------------------------------------------------
int sample_MSXE371x__IncCounterClear(struct modbus * modbus)
{
	struct GetLastCommandStatus_parameters_t CommandStatus;
	int ret = 0;

	/* Remote function call */
	ret = modbus_call_MSXE371x__IncCounterClear(modbus, 0, &CommandStatus);

	/* Return value check */
	return handle_result_of_FC16_query(modbus, &CommandStatus, ret, "modbus_call_MSXE371x__IncCounterClear");
}

//---------------------------------------------------------------------------------
int sample_MSXE371x__IncCounterRelease(struct modbus * modbus)
{
	struct GetLastCommandStatus_parameters_t CommandStatus;
	int ret = 0;

	/* Remote function call */
	ret = modbus_call_MSXE371x__IncCounterRelease(modbus, 0, &CommandStatus);

	/* Return value check */
	return handle_result_of_FC16_query(modbus, &CommandStatus, ret, "modbus_call_MSXE371x__IncCounterRelease");
}



//---------------------------------------------------------------------------------
void print_header(void)
{
	printf("\n\n");
	printf("\n+---------------------------------------------------+");
	printf("\n| MSX-E371x MODBUS Sample                           |");
	printf("\n+---------------------------------------------------+");
	printf("\n| Press ESC to stop an acquisition                  |");
	printf("\n+---------------------------------------------------+");
	printf("\n\n");
}

//---------------------------------------------------------------------------------
static const char * typestr(uint32_t type)
{
	switch (type)
	{
	case 0:
		return "half-bridge";
	case 1:
		return "LVDT";
	case 2:
		return "Knaebel";
	case 3:
		return "half-bridge Mahr";
	case 4:
		return "LVDT Mahr";
	default:
		return "UNKNOWN";
	}
}

//---------------------------------------------------------------------------------
int sample_MX371x__TransducerGetNbrOfType(struct modbus * modbus, uint32_t* NumberOfTransducerTypes)
{
	struct MX371x__TransducerGetNbrOfType_parameters_t Response;
	int ret = 0;

	memset(&Response,0,sizeof(Response));

	ret = modbus_call_MX371x__TransducerGetNbrOfType(modbus,&Response);

	if (handle_result_of_FC3_query(modbus, ret, "modbus_call_MX371x__TransducerGetNbrOfType"))
		return -1;

	*NumberOfTransducerTypes = Response.NumberOfTransducerTypes;
	printf("number of types defined in the transducer database: %d\n",Response.NumberOfTransducerTypes);
	return 0;
}

//---------------------------------------------------------------------------------
int sample_MX371x__TransducerGetNbrOfTypeEx(struct modbus * modbus, uint32_t* NumberOfTransducerTypes)
{
	struct MX371x__TransducerGetNbrOfTypeEx_parameters_t Response;
	int ret = 0;

	memset(&Response,0,sizeof(Response));

	ret = modbus_call_MX371x__TransducerGetNbrOfTypeEx(modbus,&Response);

	if (handle_result_of_FC3_query(modbus, ret, "modbus_call_MX371x__TransducerGetNbrOfTypeEx"))
		return -1;

	*NumberOfTransducerTypes = Response.NumberOfTransducerTypes;
	printf("number of types defined in the transducer database: %d\n",Response.NumberOfTransducerTypes);
	return 0;
}

//---------------------------------------------------------------------------------
int sample_MX371x__GetTransducerDatabaseCursor(struct modbus * modbus, uint32_t* TransducerDatabaseCursor)
{
	struct MX371x__GetTransducerDatabaseCursorEx_parameters_t Response;
	int ret = 0;

	memset(&Response,0,sizeof(Response));

	ret = modbus_call_MX371x__GetTransducerDatabaseCursorEx(modbus,&Response);

	if (handle_result_of_FC3_query(modbus, ret, "modbus_call_MX371x__GetTransducerDatabaseCursorEx"))
		return -1;

	*TransducerDatabaseCursor = Response.TransducerDatabaseCursor;
	printf("Current transducer database cursor: %d\n",Response.TransducerDatabaseCursor);
	return 0;
}

//---------------------------------------------------------------------------------
int sample_MX371x__GetTransducerDatabaseCursorEx(struct modbus * modbus, uint32_t* TransducerDatabaseCursor)
{
	struct MX371x__GetTransducerDatabaseCursorEx_parameters_t Response;
	int ret = 0;

	memset(&Response,0,sizeof(Response));

	ret = modbus_call_MX371x__GetTransducerDatabaseCursorEx(modbus,&Response);

	if (handle_result_of_FC3_query(modbus, ret, "modbus_call_MX371x__GetTransducerDatabaseCursorEx"))
		return -1;

	*TransducerDatabaseCursor = Response.TransducerDatabaseCursor;
	printf("Current transducer database cursor: %d\n",Response.TransducerDatabaseCursor);
	return 0;
}

//---------------------------------------------------------------------------------
int sample_MX371x__TransducerGetTypeInformation(struct modbus * modbus, struct MX371x__TransducerGetTypeInformation_parameters_t* TransducerGetTypeInformation)
{
	struct MX371x__TransducerGetTypeInformation_parameters_t Response;
	int ret = 0;

	memset(&Response,0,sizeof(Response));

	ret = modbus_call_MX371x__TransducerGetTypeInformation(modbus,&Response);

	if (handle_result_of_FC3_query(modbus, ret, "modbus_call_MX371x__TransducerGetTypeInformation"))
		return -1;

	memcpy(TransducerGetTypeInformation, &Response, sizeof(struct MX371x__TransducerGetTypeInformation_parameters_t));

	return 0;
}

//---------------------------------------------------------------------------------
int sample_MX371x__TransducerGetTypeInformationEx(struct modbus * modbus, struct MX371x__TransducerGetTypeInformationEx_parameters_t* TransducerGetTypeInformation)
{
	struct MX371x__TransducerGetTypeInformationEx_parameters_t Response;
	int ret = 0;

	memset(&Response,0,sizeof(Response));

	ret = modbus_call_MX371x__TransducerGetTypeInformationEx(modbus,&Response);

	if (handle_result_of_FC3_query(modbus, ret, "modbus_call_MX371x__TransducerGetTypeInformationEx"))
		return -1;

	memcpy(TransducerGetTypeInformation, &Response, sizeof(struct MX371x__TransducerGetTypeInformationEx_parameters_t));

	return 0;
}

//---------------------------------------------------------------------------------
int sample_MX371x__SetTransducerDatabaseCursor(struct modbus * modbus, uint32_t new)
{
	struct GetLastCommandStatus_parameters_t CommandStatus;
	int ret = 0;

	ret = modbus_call_MX371x__SetTransducerDatabaseCursor(modbus, new, &CommandStatus);

	return handle_result_of_FC16_query(modbus, &CommandStatus, ret, "modbus_call_MX371x__SetTransducerDatabaseCursor");
}

//---------------------------------------------------------------------------------
int sample_MX371x__SetTransducerDatabaseCursorEx(struct modbus * modbus, uint32_t new)
{
	struct GetLastCommandStatus_parameters_t CommandStatus;
	int ret = 0;

	ret = modbus_call_MX371x__SetTransducerDatabaseCursorEx(modbus, new, &CommandStatus);

	return handle_result_of_FC16_query(modbus, &CommandStatus, ret, "modbus_call_MX371x__SetTransducerDatabaseCursorEx");
}

/*--------------------------------------------------------------------------------- *
 * Diagnostic sample functions                                                      *
 * -------------------------------------------------------------------------------- */

//---------------------------------------------------------------------------------
int sample_MX371x__TransducerInitPrimaryConnectionTest(struct modbus * modbus)
{
	struct MX371x__TransducerInitPrimaryConnectionTest_parameters_t Response;
	int ret = 0;

	printf("\n-->sample_MX371x__TransducerInitPrimaryConnectionTest\n");

	memset(&Response,0,sizeof(Response));
	ret = modbus_call_MX371x__TransducerInitPrimaryConnectionTest(modbus, &Response);

	if (handle_result_of_FC3_query(modbus, ret, "modbus_call_MX371x__TransducerInitPrimaryConnectionTest"))
		return -1;

	printf("\t- TransducerInitPrimaryConnectionTest OK\n");

	return 0;
}

//---------------------------------------------------------------------------------
int sample_MX371x__TransducerTestPrimaryConnection(struct modbus * modbus)
{
	struct MX371x__TransducerTestPrimaryConnection_parameters_t Response;
	int ret = 0;

	printf("\n-->sample_MX371x__TransducerTestPrimaryConnection\n");

	memset(&Response,0,sizeof(Response));

	ret = modbus_call_MX371x__TransducerTestPrimaryConnection(modbus,&Response);

	if (handle_result_of_FC3_query(modbus, ret, "modbus_call_MX371x__TransducerTestPrimaryConnection"))
		return -1;

	if (Response.ulValue == 1)
		printf("\t- All connections OK\n\n");
	else
		printf("\t- Any connection open\n\n");

	return 0;
}

//---------------------------------------------------------------------------------
int sample_MX371x__TransducerTestPrimaryShortCircuit(struct modbus * modbus)
{
	struct MX371x__TransducerTestPrimaryShortCircuit_parameters_t Response;
	int ret = 0;

	printf("\n-->sample_MX371x__TransducerTestPrimaryShortCircuit\n");

	memset(&Response,0,sizeof(Response));

	ret = modbus_call_MX371x__TransducerTestPrimaryShortCircuit(modbus,&Response);

	if (handle_result_of_FC3_query(modbus, ret, "modbus_call_MX371x__TransducerTestPrimaryShortCircuit"))
		return -1;

	if (Response.ulStatus == 1)
	{
		printf("\t- No short circuit\n\n");
	}
	else
	{
		printf("\t- Short circuit detected\n\n");
	}

	return 0;
}

//---------------------------------------------------------------------------------
int sample_MX371x__TransducerRearmPrimary(struct modbus * modbus)
{
	struct MX371x__TransducerRearmPrimary_parameters_t Response;

	int ret = 0;

	printf("\n-->sample_MX371x__TransducerRearmPrimary\n");

	memset(&Response,0,sizeof(Response));

	ret = modbus_call_MX371x__TransducerRearmPrimary(modbus,&Response);

	if (handle_result_of_FC3_query(modbus, ret, "modbus_call_MX371x__TransducerRearmPrimary"))
		return -1;

	if (Response.ulValue == 1)
	{
		printf("\t- Rearme OK\n\n");
	}
	else
	{
		printf("\t- Short circuit persist\n\n");
	}

	return 0;
}

//---------------------------------------------------------------------------------
int sample_MX371x__TransducerTestSecondaryConnection(struct modbus * modbus, uint32_t channelIndex)
{
	/* Variables declaration */
	struct MX371x__TransducerTestSecondaryConnection0_parameters_t Response0;
	struct MX371x__TransducerTestSecondaryConnection1_parameters_t Response1;
	struct MX371x__TransducerTestSecondaryConnection2_parameters_t Response2;
	struct MX371x__TransducerTestSecondaryConnection3_parameters_t Response3;
	struct MX371x__TransducerTestSecondaryConnection4_parameters_t Response4;
	struct MX371x__TransducerTestSecondaryConnection5_parameters_t Response5;
	struct MX371x__TransducerTestSecondaryConnection6_parameters_t Response6;
	struct MX371x__TransducerTestSecondaryConnection7_parameters_t Response7;

	unsigned long int ulValue = 0;
	int ret = 0;

	printf("\n-->sample_MX371x__TransducerTestSecondaryConnection\n");

	memset(&Response0,0,sizeof(Response0));
	memset(&Response1,0,sizeof(Response1));
	memset(&Response2,0,sizeof(Response2));
	memset(&Response3,0,sizeof(Response3));
	memset(&Response4,0,sizeof(Response4));
	memset(&Response5,0,sizeof(Response5));
	memset(&Response6,0,sizeof(Response6));
	memset(&Response7,0,sizeof(Response7));

	switch (channelIndex)
	{
	case 0:
		ret = modbus_call_MX371x__TransducerTestSecondaryConnection0(modbus, &Response0);
		ulValue = Response0.ulValue;
		break;
	case 1:
		ret = modbus_call_MX371x__TransducerTestSecondaryConnection1(modbus, &Response1);
		ulValue = Response1.ulValue;
		break;
	case 2:
		ret = modbus_call_MX371x__TransducerTestSecondaryConnection2(modbus, &Response2);
		ulValue = Response2.ulValue;
		break;
	case 3:
		ret = modbus_call_MX371x__TransducerTestSecondaryConnection3(modbus, &Response3);
		ulValue = Response3.ulValue;
		break;
	case 4:
		ret = modbus_call_MX371x__TransducerTestSecondaryConnection4(modbus, &Response4);
		ulValue = Response4.ulValue;
		break;
	case 5:
		ret = modbus_call_MX371x__TransducerTestSecondaryConnection5(modbus, &Response5);
		ulValue = Response5.ulValue;
		break;
	case 6:
		ret = modbus_call_MX371x__TransducerTestSecondaryConnection6(modbus, &Response6);
		ulValue = Response6.ulValue;
		break;
	case 7:
		ret = modbus_call_MX371x__TransducerTestSecondaryConnection7(modbus, &Response7);
		ulValue = Response7.ulValue;
		break;
	}

	if (handle_result_of_FC3_query(modbus, ret, "modbus_call_MX371x__TransducerTestSecondaryConnection"))
		return -1;

	if (ulValue == 1)
	{
		printf("\t- Connection OK\n\n");
	}
	else
	{
		printf("\t- Connection open\n\n");
	}

	return 0;

}

//---------------------------------------------------------------------------------
int sample_MX371x__TransducerTestSecondaryShortCircuit(struct modbus * modbus, uint32_t channelIndex)
{
	/* Variables declaration */
	struct MX371x__TransducerTestSecondaryShortCircuit0_parameters_t Response0;
	struct MX371x__TransducerTestSecondaryShortCircuit1_parameters_t Response1;
	struct MX371x__TransducerTestSecondaryShortCircuit2_parameters_t Response2;
	struct MX371x__TransducerTestSecondaryShortCircuit3_parameters_t Response3;
	struct MX371x__TransducerTestSecondaryShortCircuit4_parameters_t Response4;
	struct MX371x__TransducerTestSecondaryShortCircuit5_parameters_t Response5;
	struct MX371x__TransducerTestSecondaryShortCircuit6_parameters_t Response6;
	struct MX371x__TransducerTestSecondaryShortCircuit7_parameters_t Response7;
	int ret = 0;
	unsigned long int ulValue = 0;
	printf("\n-->sample_MX371x__TransducerTestSecondaryShortCircuit\n");

	memset(&Response0,0,sizeof(Response0));
	memset(&Response1,0,sizeof(Response1));
	memset(&Response2,0,sizeof(Response2));
	memset(&Response3,0,sizeof(Response3));
	memset(&Response4,0,sizeof(Response4));
	memset(&Response5,0,sizeof(Response5));
	memset(&Response6,0,sizeof(Response6));
	memset(&Response7,0,sizeof(Response7));

	switch (channelIndex)
	{
	case 0:
		ret = modbus_call_MX371x__TransducerTestSecondaryShortCircuit0(modbus, &Response0);
		ulValue = Response0.ulValue;
		break;
	case 1:
		ret = modbus_call_MX371x__TransducerTestSecondaryShortCircuit1(modbus, &Response1);
		ulValue = Response1.ulValue;
		printf("\tret = %i\n\n", ret);
		break;
	case 2:
		ret = modbus_call_MX371x__TransducerTestSecondaryShortCircuit2(modbus, &Response2);
		ulValue = Response2.ulValue;
		break;
	case 3:
		ret = modbus_call_MX371x__TransducerTestSecondaryShortCircuit3(modbus, &Response3);
		ulValue = Response3.ulValue;
		break;
	case 4:
		ret = modbus_call_MX371x__TransducerTestSecondaryShortCircuit4(modbus, &Response4);
		ulValue = Response4.ulValue;
		break;
	case 5:
		ret = modbus_call_MX371x__TransducerTestSecondaryShortCircuit5(modbus, &Response5);
		ulValue = Response5.ulValue;
		break;
	case 6:
		ret = modbus_call_MX371x__TransducerTestSecondaryShortCircuit6(modbus, &Response6);
		ulValue = Response6.ulValue;
		break;
	case 7:
		ret = modbus_call_MX371x__TransducerTestSecondaryShortCircuit7(modbus, &Response7);
		ulValue = Response7.ulValue;
		break;
	}
	
	if (handle_result_of_FC3_query(modbus, ret, "modbus_call_TransducerTestSecondaryShortCircuit"))
		return -1;

	if (ulValue == 1)
		printf("\t- No short circuit\n\n");
	else
		printf("\t- Short circuit detected\n\n");
	
	return 0;
}

//---------------------------------------------------------------------------------
/* prints the content of the transducer database on STDOUT */
int dump_transducer_database(struct modbus * modbus)
{
	uint32_t NumberOfTransducerTypes = 0;
	int      ret                     = 0;

	printf("\nprinting the content of the transducer database\n");

	ret =  sample_MX371x__TransducerGetNbrOfTypeEx(modbus, &NumberOfTransducerTypes);
	if (ret != 0)
		printf("sample_MX371x__TransducerGetNbrOfTypeEx error %d\n", ret);
	else
		printf("sample_MX371x__TransducerGetNbrOfTypeEx OK");

	/* Go through the transducer database */
	{
		unsigned int i;
		for (i=0; i<NumberOfTransducerTypes; i++)
		{
			uint32_t TransducerDatabaseCursor = 0;
			struct MX371x__TransducerGetTypeInformationEx_parameters_t TransducerGetTypeInformation;

			ret =  sample_MX371x__SetTransducerDatabaseCursorEx(modbus, i);
			if (ret != 0)
				printf("sample_MX371x__SetTransducerDatabaseCursorEx error %d\n", ret);
			else
				printf("sample_MX371x__SetTransducerDatabaseCursorEx OK\n");

			ret =  sample_MX371x__GetTransducerDatabaseCursorEx(modbus, &TransducerDatabaseCursor);
			if (ret != 0)
				printf("sample_MX371x__GetTransducerDatabaseCursorEx error %d\n", ret);
			else
				printf("sample_MX371x__GetTransducerDatabaseCursorEx OK\n");
				
			if (i != TransducerDatabaseCursor)
			{
				printf("TransducerDatabaseCursor error\n");
				return -1;
			}
				
			ret =  sample_MX371x__TransducerGetTypeInformationEx(modbus, &TransducerGetTypeInformation);
			if (ret != 0)
				printf("sample_MX371x__TransducerGetTypeInformationEx error %d\n", ret);
			else
				printf("sample_MX371x__TransducerGetTypeInformationEx OK\n");

			printf("Selection index: %u\n", TransducerGetTypeInformation.SelectionIndex);
			printf("Name: %s\n", TransducerGetTypeInformation.Name);
			printf("Calibrated ? %s\n", (TransducerGetTypeInformation.CalibrationStatus ? "yes" : "no"));
			printf("Calibrated channels bitmask 0x%X\n", TransducerGetTypeInformation.CalibratedChannels);
			printf("Type: %s\n", typestr(TransducerGetTypeInformation.Type));
			printf("Frequency: %u Hz\n", TransducerGetTypeInformation.Frequency);
			printf("Impedance: %u Ohm\n", TransducerGetTypeInformation.Impedance);
			printf("Nominal voltage: %f Vrms\n", TransducerGetTypeInformation.NominalVoltage);
			printf("Sensibility: %f mv/V/mm\n", TransducerGetTypeInformation.Sensibility);
			printf("Range: +/- %f mm\n", TransducerGetTypeInformation.Range);
			printf("\n");

			if (i == 0)
				TRANSDUCER_SELECTION = TransducerGetTypeInformation.SelectionIndex;
		}
	}

	return 0;
}

//---------------------------------------------------------------------------------
int getSystemTransducerType(struct modbus * modbus, uint32_t *TransducerType)
{
	struct MXCommon__GetModuleTypeEx_parameters_t Result;
	int ret = 0;

	memset(&Result,0,sizeof(Result));

	ret = modbus_call_MXCommon__GetModuleTypeEx(modbus,&Result);

	if (handle_result_of_FC3_query(modbus, ret, "modbus_call_MXCommon__GetModuleTypeEx"))
		return -1;

	if (strstr((char *)Result.str, "-M MX371X{"))
		*TransducerType = 2;
	else
		*TransducerType = 1;

	return 0;
}

//---------------------------------------------------------------------------------
int sample_diagnostic(struct modbus * modbus, uint32_t ulChannelNumber)
{
	uint32_t ulChoice       = 0;
	uint32_t ulChannelIndex = 0;
	uint32_t TransducerType = 0;

	printf("\n\n--Sample diagnostic selected--\n\n");

	printf("Test if a short circuit is present on the primary side.\n"
			"If a short circuit is detected, check that:	\n"
			"\tOSC+ is not connected to the ground\n"
			"\tOSC- is not connected to the ground\n"
			"\tOSC+ is not connected to OSC-\n"
			"\tthen select \"Rearm primary\"\n\n");

	if (getSystemTransducerType(modbus, &TransducerType) != 0)
		return -1;

	if (sample_MX371x__TransducerTestPrimaryShortCircuit(modbus) != 0)
		return -1;

	if (TransducerType != 2)
	{
		if (sample_MX371x__TransducerInitPrimaryConnectionTest(modbus) != 0)
			return -1;
	}

	do
	{
		/*
		 * If you are writing a program that makes tests automatically,
		 * you have to call functions in the following order:
		 *
		 * Primary short circuit test (Rearm primary side, if necessary)
		 * Secondary connection test
		 * Secondary short circuit test
		 * Init primary connection test
		 * Primary connection test
		 */
		printf("\n\n Select an action:");
		printf("\n-----------------------------------");
		if (TransducerType != 2)
		{
			printf("\n 1. Init primary connection test");

			printf("\n 2. Secondary connection test");

			printf("\n 3. Secondary short circuit test");

			printf("\n 4. Primary connection test");
		}
		printf("\n 5. Primary short circuit test");

		printf("\n 6. Rearm primary side");

		printf("\n 7. Exit");
		printf("\n\n Your choice : ");
		scanf("%lu", (unsigned long *)&ulChoice);
		printf("\nchoice = %lu\n", (unsigned long)ulChoice);

		if ((TransducerType == 2) && (ulChoice < 5))
			continue;

		switch (ulChoice)
		{
		case 1:
			if (sample_MX371x__TransducerInitPrimaryConnectionTest(modbus) != 0)
				return -1;
			break;
		case 4:
			if (sample_MX371x__TransducerTestPrimaryConnection(modbus) != 0)
				return -1;
			break;
		case 5:
			if (sample_MX371x__TransducerTestPrimaryShortCircuit(modbus) != 0)
				return -1;
			break;
		case 6:
			if (sample_MX371x__TransducerRearmPrimary(modbus) != 0)
				return -1;
			break;
		case 2:
			/* Test if the selected channel has a short circuit on the secondary side.
			 * A short circuit means that the channel is connected to the ground.
			 * No ream is necessary after a secondary side short circuit.
			 */
			do
			{
				printf("\n Select the channel to test (0 to %lu):",(unsigned long)ulChannelNumber-1);
				scanf("%lu",(unsigned long *)&ulChannelIndex);
			}
			while (ulChannelIndex >= ulChannelNumber);
			if (sample_MX371x__TransducerTestSecondaryConnection(modbus, ulChannelIndex) != 0)
				return -1;
			break;
		case 3:
			/* Test if the selected channel has a short circuit on the secondary side.
			 * A short circuit means that the channel is connected to the ground.
			 * No ream is necessary after a secondary side short circuit.
			 */
			do
			{
				printf("\n Select the channel to test (0 to %lu):",(unsigned long)ulChannelNumber-1);
				scanf("%lu",(unsigned long *)&ulChannelIndex);
			}
			while (ulChannelIndex >= ulChannelNumber);
			
			if (sample_MX371x__TransducerTestSecondaryShortCircuit(modbus, ulChannelIndex) != 0)
				return -1;
			break;
		}
	}
	while (ulChoice != 7);

	return 0;
}

//---------------------------------------------------------------------------------
int main (int argc, char** argv)
{
	uint32_t      ulValue              = 0;
	uint32_t      ulTimeStampLow       = 0;
	uint32_t      ulTimeStampHigh      = 0;
	uint32_t 	  ulChannelNumber      = 0;
	float         fInternalTemperature = 0.0;
	int           ret                  = -1;
	struct        modbus               modbus;

	if (argc != 3)
	{
		printf("%s need two parameters: ADDRESS SAMPLE_NUMBER\n\n - SAMPLE_NUMBER :\n \t 1 : diagnose sample\n \t 2 : autorefresh sample\n \t 3 : sequence sample\n \t 4 : temperature sample\n \t 5 : counter sample\n \t 6 : internal temperature sample\n \t 7 : counter index \n", argv[0]);
		exit(1);
	}

	print_header();
	modbus_init(&modbus);



	switch (modbus_connect(&modbus, argv[1], SOCK_STREAM, 512))
	{
	case 0:
		break;
	case -1:
		addidata_network_perror("modbus_connect");
		exit(1);
	case -2:
		printf("modbus_connect: incorrect parameter\n");
		exit(1);
	case -3:
		printf("incorrect address\n");
		exit(1);
	}

	if (sample_MXCommon__GetModuleTypeEx(&modbus) != 0)
		{ printf("sample_MXCommon__GetModuleTypeEx error\n"); goto sample_end; }

	/*if (sample_MXCommon__GetTimeEx(&modbus) != 0)
		{ printf("sample_MXCommon__GetTimeEx error\n"); goto sample_end; }*/

	if (sample_MXCommon__SetHardwareTriggerFilterTimeEx(&modbus, 1) != 0)
		{ printf("sample_MXCommon__SetHardwareTriggerFilterTimeEx error\n"); goto sample_end; }

	if (dump_transducer_database(&modbus) != 0)
		{ printf("dump_transducer_database error\n"); goto sample_end; }

	if (sample_MX371x__getNumberOfChannelsEx(&modbus, &ulChannelNumber) != 0)
		{ printf("sample_MX371x__getNumberOfChannelsEx error\n"); goto sample_end; }
	

/*	struct GetLastCommandStatus_parameters_t CommandStatus;
	uint8_t ChannelList[16];

	if (modbus_call_MXCommon__SetFilterChannelsEx(&modbus, ChannelList, &CommandStatus) != 0)
	{
		printf("modbus_call_MXCommon__SetFilterChannelsEx error\n"); goto sample_end;
	}*/



	/* Diagnostic sample */
	if (*argv[2] == '1')
	{
		if (sample_diagnostic(&modbus, ulChannelNumber) != 0)
			{ printf("sample_diagnostic error\n"); goto sample_end; }
	}
	else if (*argv[2] == '2')
	{
		int i = 0;

		/* Autorefresh example */
		if (sample_MX371x__TransducerInitAndStartAutoRefreshEx(&modbus) != 0)
			{ printf("sample_MX371x__TransducerInitAndStartAutoRefreshEx error\n"); goto sample_end; }

		for (i=0;i<1000;i++)  //1000
		{
			if (sample_MX371x__TransducerGetAutoRefreshValuesEx(&modbus,1) != 0)
				{ printf("sample_MX371x__TransducerGetAutoRefreshValuesEx error\n"); goto sample_end; }
		}

		if (sample_MX371x__TransducerStopAndReleaseAutoRefreshEx(&modbus) != 0)
			{ printf("sample_MX371x__TransducerStopAndReleaseAutoRefreshEx error\n"); goto sample_end; }
	}
	else if (*argv[2] == '3')
	{
		/* Sequence example */
		if (sample_MX371x__TransducerInitAndStartSequenceEx(&modbus, argv[1]) != 0)       //传感器初始化  
			{ printf("sample_MX371x__TransducerInitAndStartSequenceEx error\n"); goto sample_end; }

		if (sample_MX371x__TransducerStopAndReleaseSequenceEx(&modbus) != 0)
			{ printf("sample_MX371x__TransducerStopAndReleaseSequenceEx error\n"); goto sample_end; }
	}
	else if (*argv[2] == '4')
	{
		/* Temperature sample */
		if (sample_MSXE371x__ExternalTemperatureInitEx(&modbus) != 0)
			{ printf("sample_MSXE371x__ExternalTemperatureInitEx error\n"); goto sample_end; }
			
		if (sample_MSXE371x__ExternalTemperatureReadEx(&modbus) != 0)
			{ printf("sample_MSXE371x__ExternalTemperatureReadEx error\n"); goto sample_end; }

		if (sample_MSXE371x__ExternalTemperatureReleaseEx(&modbus) != 0)
			{ printf("sample_MSXE371x__ExternalTemperatureReleaseEx error\n"); goto sample_end; }
	}
	else if (*argv[2] == '5')
	{
		int i = 0;

		/* Counter sample */
		if (sample_MSXE371x__IncCounterInit(&modbus) != 0)
			{ printf("sample_MSXE371x__IncCounterInit error\n"); goto sample_end; }

		if (sample_MSXE371x__IncCounterClear(&modbus) != 0)
			{ printf("sample_MSXE371x__IncCounterClear error\n"); goto sample_end; }

		if (sample_MSXE371x__IncCounterWrite32BitValue(&modbus) != 0)
			{ printf("sample_MSXE371x__IncCounterWrite32BitValue error\n"); goto sample_end; }

		for (i=0;i<100;i++)
		{
			if (sample_MSXE371x__IncCounterRead32BitsValue(&modbus, &ulValue, &ulTimeStampLow, &ulTimeStampHigh) != 0)
				{ printf("sample_MSXE371x__IncCounterRead32BitsValue error\n"); goto sample_end; }
#ifdef WIN32
				sleep (100);
#else
				sleep(1);
#endif
				printf("\n");
		}

		if (sample_MSXE371x__IncCounterRelease(&modbus) != 0)
			{ printf("sample_MSXE371x__IncCounterRelease error\n"); goto sample_end; }
	}
	else if (*argv[2] == '6')
	{
		/* Internal temperature sample */
		if (sample_MX371x__getInternalTemperature(&modbus, &fInternalTemperature) != 0)
			{ printf("sample_MX371x__getInternalTemperature error\n"); goto sample_end; }
	}
	else if (*argv[2] == '7')
	{
		uint32_t ulReferenceAction = 0;
		uint32_t ulIndexOperation = 0;
		uint32_t ulAutoMode = 0;
		int i = 0;

		do
		{
			printf("\n Use the pin D as reference? (1: yes, 0: no):");
			scanf("%lu",(unsigned long *)&ulReferenceAction);
		}
		while (ulReferenceAction > 1);

		do
		{
			printf("\n Select index operation (0: clear on low edge, 1: clear on high edge):");
			scanf("%lu",(unsigned long *)&ulIndexOperation);
		}
		while (ulIndexOperation > 1);

		do
		{
			printf("\n Automode ? (1: yes, 0: no):");
			scanf("%lu",(unsigned long *)&ulAutoMode);
		}
		while (ulAutoMode > 1);

		/* Counter index sample */
		if (sample_MSXE371x__IncCounterInit(&modbus) != 0)
			{ printf("sample_MSXE371x__IncCounterInit error\n"); goto sample_end; }

		if (sample_MSXE371x__IncCounterClear(&modbus) != 0)
			{ printf("sample_MSXE371x__IncCounterClear error\n"); goto sample_end; }

		if (sample_MSXE371x__IncCounterWrite32BitValue(&modbus) != 0)
			{ printf("sample_MSXE371x__IncCounterWrite32BitValue error\n"); goto sample_end; }

		// Ensure index is not initialized
		sample_MSXE371x__IncCounterDisableAndReleaseIndex(&modbus);

		if (sample_MSXE371x__IncCounterInitAndEnableIndex(&modbus, ulReferenceAction, ulIndexOperation, ulAutoMode) != 0)
			{ printf("sample_MX371x__IncCounterInitAndEnableIndex error\n"); goto sample_end; }
		else
		    printf("sample_MSXE371x__IncCounterInitAndEnableIndex OK\n");

		for (i=0;i<100;i++)
		{
			if (sample_MSXE371x__IncCounterRead32BitsValue(&modbus, &ulValue, &ulTimeStampLow, &ulTimeStampHigh) != 0)
				{ printf("sample_MSXE371x__IncCounterRead32BitsValue error\n"); goto sample_end; }
#ifdef WIN32
				sleep (100);
#else
				sleep(1);
#endif
				printf("\n");
		}

		if (sample_MSXE371x__IncCounterDisableAndReleaseIndex(&modbus) != 0)
			{ printf("sample_MSXE371x__IncCounterDisableAndReleaseIndex error\n"); goto sample_end; }
		else
		    printf("sample_MSXE371x__IncCounterDisableAndReleaseIndex OK\n");
	}
	else
	{
		printf("error wrong sample number");
	}

	ret = 0;

sample_end:
	if (modbus_shutdown(&modbus) != 0)
		printf("\nfailed to free modbus\n");

	printf("\npress a key to end this sample\n");
	fflush(stdout);
	addidata_console_wait_for_input();
	return ret;
}
