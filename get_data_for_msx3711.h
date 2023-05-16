//#ifndef GET_DATA_FOR_MSX3711_H
//#define GET_DATA_FOR_MSX3711_H


//#ifdef _WIN32
//#define sleep Sleep
//#else
//#include "msxe371x_modbus_clientlib.h"
//#include <unistd.h>
//#endif

//#ifdef __cplusplus
//extern "C"{

//#include "modbusclient.h"
//#include "addidata_network.h"
//#include "msxe371x_modbus_clientlib.h"

//#ifdef __cplusplus
//}
//#endif
//#endif

//uint32_t TRANSDUCER_SELECTION = 0;

//int  sampleMX371xTransducerGetAutoRefreshValuesEx(struct modbus * modbus, int NumberOfChannels)
//{
//    struct MX371x__TransducerGetAutoRefreshValuesEx_parameters_t Response;
//    int ret = 0;

//    memset(&Response,0,sizeof(Response));

//    ret = modbus_call_MX371x__TransducerGetAutoRefreshValuesEx(modbus,&Response);

//    if (!handle_result_of_FC3_query(modbus, ret, "modbus_call_MX371x__TransducerGetAutoRefreshValuesEx"))
//    {
//        int i;

//        for (i=1;i<=NumberOfChannels;i++)
//        {
//            ui->label_4->setNum((*((float*)&Response.Value[i]))*1000.0);

//            displacement_data_.push_back((*((float*)&Response.Value[i]))*1000.0);

//        }
//        return 0;
//    }
//    return -1;
//}

//int  sampleMX371xTransducerInitAndStartAutoRefreshEx(struct modbus * modbus)
//{
//    struct GetLastCommandStatus_parameters_t CommandStatus;
//    int ret = 0;

//    std::stringstream channelmask_dec;
//    int channel_num;

//    uint32_t TransducerSelection = TRANSDUCER_SELECTION;
//    uint32_t ChannelMask = 0x1;  //通道
//    uint32_t AverageMode = 0;
//    uint32_t AverageValue = 0;
//    uint32_t TriggerMask = 0;
//    uint32_t TriggerMode = 0;
//    uint16_t HardwareTriggerCount = 0;
//    uint16_t HardwareTriggerEdge = 0;
//    uint32_t ByTriggerNbrOfSeqToAcquire = 1;
//    uint32_t DataFormat = 3;
//    uint32_t Option1 = 0;
//    uint32_t Option2 = 0;
//    uint32_t Option3 = 0;
//    uint32_t Option4 = 0;

//    channelmask_dec<<ChannelMask;  //0x转 dec
//    channelmask_dec>>channel_num;

//    QString str="T"+QString::number(channel_num);
//    ui->transducer_label->setText(str);

//    ret = modbus_call_MX371x__TransducerInitAndStartAutoRefreshEx(modbus,TransducerSelection,ChannelMask,AverageMode,AverageValue,TriggerMask,TriggerMode,HardwareTriggerEdge,HardwareTriggerCount,ByTriggerNbrOfSeqToAcquire,DataFormat,Option1,Option2,Option3,Option4,&CommandStatus);

//    return handle_result_of_FC16_query(modbus, &CommandStatus, ret, "modbus_call_MX371x__TransducerInitAndStartAutoRefreshEx");
//}

//int  sampleMX371xTransducerStopAndReleaseAutoRefreshEx(struct modbus * modbus)
//{
//    struct GetLastCommandStatus_parameters_t CommandStatus;
//    int ret = 0;
//    uint32_t Dummy = 0;

//    ret = modbus_call_MX371x__TransducerStopAndReleaseAutoRefreshEx(modbus,Dummy,&CommandStatus);

//    return handle_result_of_FC16_query(modbus, &CommandStatus, ret, "modbus_call_MX371x__TransducerStopAndReleaseAutoRefreshEx");
//}

//const char *  typestr(uint32_t type)
//{
//    switch (type)
//    {
//    case 0:
//        return "half-bridge";
//    case 1:
//        return "LVDT";
//    case 2:
//        return "Knaebel";
//    case 3:
//        return "half-bridge Mahr";
//    case 4:
//        return "LVDT Mahr";
//    default:
//        return "UNKNOWN";
//    }
//}

//int  sampleMX371xTransducerGetNbrOfTypeEx(struct modbus * modbus, uint32_t* NumberOfTransducerTypes)
//{
//    struct MX371x__TransducerGetNbrOfTypeEx_parameters_t Response;
//    int ret = 0;

//    memset(&Response,0,sizeof(Response));

//    ret = modbus_call_MX371x__TransducerGetNbrOfTypeEx(modbus,&Response);

//    if (handle_result_of_FC3_query(modbus, ret, "modbus_call_MX371x__TransducerGetNbrOfTypeEx"))
//        return -1;

//    *NumberOfTransducerTypes = Response.NumberOfTransducerTypes;

//    return 0;
//}

//int  sampleMX371xGetTransducerDatabaseCursorEx(struct modbus * modbus, uint32_t* TransducerDatabaseCursor)
//{
//    struct MX371x__GetTransducerDatabaseCursorEx_parameters_t Response;
//    int ret = 0;

//    memset(&Response,0,sizeof(Response));

//    ret = modbus_call_MX371x__GetTransducerDatabaseCursorEx(modbus,&Response);

//    if (handle_result_of_FC3_query(modbus, ret, "modbus_call_MX371x__GetTransducerDatabaseCursorEx"))
//        return -1;

//    *TransducerDatabaseCursor = Response.TransducerDatabaseCursor;

//    return 0;
//}

//int  sampleMX371xTransducerGetTypeInformationEx(struct modbus * modbus, struct MX371x__TransducerGetTypeInformationEx_parameters_t* TransducerGetTypeInformation)
//{
//    struct MX371x__TransducerGetTypeInformationEx_parameters_t Response;
//    int ret = 0;

//    memset(&Response,0,sizeof(Response));

//    ret = modbus_call_MX371x__TransducerGetTypeInformationEx(modbus,&Response);

//    if (handle_result_of_FC3_query(modbus, ret, "modbus_call_MX371x__TransducerGetTypeInformationEx"))
//        return -1;

//    memcpy(TransducerGetTypeInformation, &Response, sizeof(struct MX371x__TransducerGetTypeInformationEx_parameters_t));

//    return 0;
//}

//int  sampleMX371xSetTransducerDatabaseCursorEx(struct modbus * modbus, uint32_t new_)
//{
//    struct GetLastCommandStatus_parameters_t CommandStatus;
//    int ret = 0;

//    ret = modbus_call_MX371x__SetTransducerDatabaseCursorEx(modbus, new_, &CommandStatus);

//    return handle_result_of_FC16_query(modbus, &CommandStatus, ret, "modbus_call_MX371x__SetTransducerDatabaseCursorEx");
//}

////计数器---------------------------------------------------------------------------------
//int  sample_MSXE371x__IncCounterInit(struct modbus * modbus)
//{
//    struct GetLastCommandStatus_parameters_t CommandStatus;
//    int ret = 0;

//    uint32_t ulCounterMode = 0x1;
//    uint32_t ulCounterOption = 0;
//    uint32_t ulOption01 = 0;
//    uint32_t ulOption02 = 0;
//    uint32_t ulOption03 = 0;
//    uint32_t ulOption04 = 0;

//    /* Remote function call */
//    ret = modbus_call_MSXE371x__IncCounterInit(modbus,ulCounterMode,ulCounterOption,ulOption01,ulOption02,ulOption03,ulOption04,&CommandStatus);

//    /* Return value check */
//    return handle_result_of_FC16_query(modbus, &CommandStatus, ret, "modbus_call_MSXE371x__IncCounterInit");
//}

////---------------------------------------------------------------------------------
//int  sample_MSXE371x__IncCounterClear(struct modbus * modbus)
//{
//    struct GetLastCommandStatus_parameters_t CommandStatus;
//    int ret = 0;

//    /* Remote function call */
//    ret = modbus_call_MSXE371x__IncCounterClear(modbus, 0, &CommandStatus);

//    /* Return value check */
//    return handle_result_of_FC16_query(modbus, &CommandStatus, ret, "modbus_call_MSXE371x__IncCounterClear");
//}

//int  sample_MSXE371x__IncCounterWrite32BitValue(struct modbus * modbus)
//{
//    struct GetLastCommandStatus_parameters_t CommandStatus;
//    int ret = 0;

//    uint32_t ulCounterValue = 1000;

//    /* Remote function call */
//    ret = modbus_call_MSXE371x__IncCounterWrite32BitValue(modbus, ulCounterValue, &CommandStatus);

//    /* Return value check */
//    return handle_result_of_FC16_query(modbus, &CommandStatus, ret, "modbus_call_MSXE371x__IncCounterWrite32BitValue");
//}

//int  sample_MSXE371x__IncCounterRead32BitsValue(struct modbus * modbus, uint32_t *pulValue, uint32_t *pulTimeStampLow, uint32_t *pulTimeStampHigh)
//{
//    struct MSXE371x__IncCounterRead32BitValue_parameters_t Response;
//    int ret = 0;

//    memset(&Response,0,sizeof(Response));

//    ret = modbus_call_MSXE371x__IncCounterRead32BitValue(modbus,&Response);

//    if (handle_result_of_FC3_query(modbus, ret, "modbus_call_MSXE371x__IncCounterRead0"))
//    {
//        return -1;
//    }

//    *pulValue         = Response.ulValue;
//    *pulTimeStampLow  = Response.ulTimeStampLow;
//    *pulTimeStampHigh = Response.ulTimeStampHigh;

//    printf("Counter/position: %d\n",*pulValue);
//    printf("Time stamp low  : %u\n", *pulTimeStampLow);
//    printf("Time stamp high : %u\n", *pulTimeStampHigh);

//    return 0;
//}

//int  sample_MSXE371x__IncCounterRelease(struct modbus * modbus)
//{
//    struct GetLastCommandStatus_parameters_t CommandStatus;
//    int ret = 0;

//    /* Remote function call */
//    ret = modbus_call_MSXE371x__IncCounterRelease(modbus, 0, &CommandStatus);

//    /* Return value check */
//    return handle_result_of_FC16_query(modbus, &CommandStatus, ret, "modbus_call_MSXE371x__IncCounterRelease");
//}


///* prints the content of the transducer database on STDOUT */
//int  dumpTransducerDatabase(struct modbus * modbus)
//{
//    uint32_t NumberOfTransducerTypes = 0;
//    int      ret                     = 0;

//    ret =  sampleMX371xTransducerGetNbrOfTypeEx(modbus, &NumberOfTransducerTypes);

//    /* Go through the transducer database */
//    {
//        unsigned int i;
//        for (i=0; i<NumberOfTransducerTypes; i++)
//        {
//            uint32_t TransducerDatabaseCursor = 0;
//            struct MX371x__TransducerGetTypeInformationEx_parameters_t TransducerGetTypeInformation;

//            ret =  sampleMX371xSetTransducerDatabaseCursorEx(modbus, i);

//            ret =  sampleMX371xGetTransducerDatabaseCursorEx(modbus, &TransducerDatabaseCursor);

//            if (i != TransducerDatabaseCursor)
//            {
//                return -1;
//            }

//            ret =  sampleMX371xTransducerGetTypeInformationEx(modbus, &TransducerGetTypeInformation);



//            transducer_name_=QString::fromUtf8(reinterpret_cast<const char*>(TransducerGetTypeInformation.Name),sizeof(TransducerGetTypeInformation.Name));

////            memcpy(&s,&TransducerGetTypeInformation.Name,100);
//            qDebug()<<"name :"<<transducer_name_;

//            if (i == 0)
//                TRANSDUCER_SELECTION = TransducerGetTypeInformation.SelectionIndex;
//        }
//    }
//    return 0;
//}

//#endif // GET_DATA_FOR_MSX3711_H