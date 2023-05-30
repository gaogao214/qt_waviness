#include "get_msxe3711_data.h"

#include <sstream>

#include <fstream>

#ifdef _WIN32
#define sleep Sleep
#else
#include "msxe371x_modbus_clientlib.h"
#include <unistd.h>
#endif



get_msxe3711_data::get_msxe3711_data(QObject *parent)
    : QObject(parent)
{
    qRegisterMetaType<struct modbus>("struct modbus");
    qRegisterMetaType<QVector<double>>("QVector<double>");

//    connectMSXe3711();
    if(evaluation_criterion_==nullptr)
    {
        evaluation_criterion_=std::make_unique<evaluation_criterion>();

    }
}

get_msxe3711_data::~get_msxe3711_data()
{
    qDebug() << "~get_msxe3711_data()" << "thread:" << QThread::currentThreadId();
}

//连接msx-e3711服务器
void get_msxe3711_data::connectMSXe3711()
{
    modbus_init(&modbus_);

    switch (modbus_connect(&modbus_, k_ip, SOCK_STREAM, k_port))
    {
    case 0:   
        qDebug()<<"连接成功\n";
        break;
    case -1:
        addidata_network_perror("modbus_connect");
        qDebug()<<"连接失败\n";

        connectMSXe3711();
        break;
    case -2:
        qDebug()<<"连接失败\n";
        connectMSXe3711();
        break;
    case -3:
        qDebug()<<"连接失败\n";
        connectMSXe3711();
        break;
    default:
        qDebug()<<"连接失败\n";
        connectMSXe3711();
        break;
    }

}

//获取msx-e3711的采集数据
void get_msxe3711_data::getMSXdata()
{
    qDebug() << "get_msxe3711_data()"<< "thread:" << QThread::currentThreadId();

//    counterInput();

    displacement_data_.clear();

    dumpTransducerDatabase(&modbus_);

    sampleMX371xTransducerInitAndStartAutoRefreshEx(&modbus_);

    for (int i=0;i<k_measurement_point;i++)
    {
        sampleMX371xTransducerGetAutoRefreshValuesEx(&modbus_,1);
    }

    sampleMX371xTransducerStopAndReleaseAutoRefreshEx(&modbus_);

    if(displacement_data_.size()==0)
    {

        std::ifstream ifs;							//创建流对象

        ifs.open("C:\\Users\\gaohuan\\Desktop\\lunkuo.txt", std::ios::in);	//打开文件

        if (!ifs.is_open())						//判断文件是否打开
        {
            qDebug() << "打开文件失败！！！";
        }

        std::vector<std::string> item;		//用于存放文件中的一行数据

        std::string temp;				//临时存储文件中的一行数据

        while (std::getline(ifs,temp))  //利用 getline（）读取文件中的每一行，并放入到 item 中
        {
            item.push_back(temp);
        }
        float lat;

        QVector<float> vec;

        //遍历文件中的每一行数据
        for (auto it = item.begin(); it != item.end(); it++)
        {
            QString str=QString::fromStdString(*it);

            lat = std::stod(*it);
            str=QString::number(lat,'f',4);
            lat=str.toFloat();


            displacement_data_.push_back(lat);
        }

    }
    QVector<double> amplitude=evaluation_criterion_->dft(displacement_data_,displacement_data_.size());
    evaluation_criterion_->polarCoordinatesToCartesianCoordinates(displacement_data_,0,0);

    emit signal_msxe3711_evaluation_criterion(evaluation_criterion_->tempPoint_.radius,evaluation_criterion_->min_coverage_circle_.radius);
    emit signal_msxe3711_data(displacement_data_,amplitude);
}

int get_msxe3711_data::sampleMX371xTransducerInitAndStartAutoRefreshEx(struct modbus * modbus)
{
    struct GetLastCommandStatus_parameters_t CommandStatus;
    int ret = 0;

    std::stringstream channelmask_dec;
    int channel_num;

    uint32_t TransducerSelection = TRANSDUCER_SELECTION;
    uint32_t ChannelMask = 0x1;  //通道
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

    channelmask_dec<<ChannelMask;  //0x转 dec
    channelmask_dec>>channel_num;

    QString str="T"+QString::number(channel_num);

    emit signal_msxe3711_channel(str);

    ret = modbus_call_MX371x__TransducerInitAndStartAutoRefreshEx(modbus,TransducerSelection,ChannelMask,AverageMode,AverageValue,TriggerMask,TriggerMode,HardwareTriggerEdge,HardwareTriggerCount,ByTriggerNbrOfSeqToAcquire,DataFormat,Option1,Option2,Option3,Option4,&CommandStatus);

    return handle_result_of_FC16_query(modbus, &CommandStatus, ret, "modbus_call_MX371x__TransducerInitAndStartAutoRefreshEx");
}

int get_msxe3711_data::sampleMX371xTransducerStopAndReleaseAutoRefreshEx(struct modbus * modbus)
{
    struct GetLastCommandStatus_parameters_t CommandStatus;
    int ret = 0;
    uint32_t Dummy = 0;

    ret = modbus_call_MX371x__TransducerStopAndReleaseAutoRefreshEx(modbus,Dummy,&CommandStatus);

    return handle_result_of_FC16_query(modbus, &CommandStatus, ret, "modbus_call_MX371x__TransducerStopAndReleaseAutoRefreshEx");
}

int get_msxe3711_data::sampleMX371xTransducerGetAutoRefreshValuesEx(struct modbus * modbus, int NumberOfChannels)
{
    struct MX371x__TransducerGetAutoRefreshValuesEx_parameters_t Response;
    int ret = 0;

    memset(&Response,0,sizeof(Response));

    ret = modbus_call_MX371x__TransducerGetAutoRefreshValuesEx(modbus,&Response);

    if (!handle_result_of_FC3_query(modbus, ret, "modbus_call_MX371x__TransducerGetAutoRefreshValuesEx"))
    {
        for (int i=1;i<=NumberOfChannels;i++)
        {
            emit signal_msxe3711_measurement_point((*((float*)&Response.Value[i]))*1000.0);

            displacement_data_.push_back((*((float*)&Response.Value[i]))*1000.0);

        }
        return 0;
    }
    return -1;
}


int get_msxe3711_data::sampleMX371xTransducerGetNbrOfTypeEx(struct modbus * modbus, uint32_t* NumberOfTransducerTypes)
{
    struct MX371x__TransducerGetNbrOfTypeEx_parameters_t Response;
    int ret = 0;

    memset(&Response,0,sizeof(Response));

    ret = modbus_call_MX371x__TransducerGetNbrOfTypeEx(modbus,&Response);

    if (handle_result_of_FC3_query(modbus, ret, "modbus_call_MX371x__TransducerGetNbrOfTypeEx"))
        return -1;

    *NumberOfTransducerTypes = Response.NumberOfTransducerTypes;

    return 0;
}

int get_msxe3711_data::sampleMX371xSetTransducerDatabaseCursorEx(struct modbus * modbus, uint32_t new_)
{
    struct GetLastCommandStatus_parameters_t CommandStatus;
    int ret = 0;

    ret = modbus_call_MX371x__SetTransducerDatabaseCursorEx(modbus, new_, &CommandStatus);

    return handle_result_of_FC16_query(modbus, &CommandStatus, ret, "modbus_call_MX371x__SetTransducerDatabaseCursorEx");
}

int get_msxe3711_data::sampleMX371xGetTransducerDatabaseCursorEx(struct modbus * modbus, uint32_t* TransducerDatabaseCursor)
{
    struct MX371x__GetTransducerDatabaseCursorEx_parameters_t Response;
    int ret = 0;

    memset(&Response,0,sizeof(Response));

    ret = modbus_call_MX371x__GetTransducerDatabaseCursorEx(modbus,&Response);

    if (handle_result_of_FC3_query(modbus, ret, "modbus_call_MX371x__GetTransducerDatabaseCursorEx"))
        return -1;

    *TransducerDatabaseCursor = Response.TransducerDatabaseCursor;

    return 0;
}


int get_msxe3711_data::sampleMX371xTransducerGetTypeInformationEx(struct modbus * modbus, struct MX371x__TransducerGetTypeInformationEx_parameters_t* TransducerGetTypeInformation)
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

/* prints the content of the transducer database on STDOUT 获取传感器的类型 */
int get_msxe3711_data::dumpTransducerDatabase(struct modbus * modbus)
{
    uint32_t NumberOfTransducerTypes = 0;
    int      ret                     = 0;

    ret =  sampleMX371xTransducerGetNbrOfTypeEx(modbus, &NumberOfTransducerTypes);

    /* Go through the transducer database */
    {
        unsigned int i;
        for (i=0; i<NumberOfTransducerTypes; i++)
        {
            uint32_t TransducerDatabaseCursor = 0;
            struct MX371x__TransducerGetTypeInformationEx_parameters_t TransducerGetTypeInformation;

            ret =  sampleMX371xSetTransducerDatabaseCursorEx(modbus, i);

            ret =  sampleMX371xGetTransducerDatabaseCursorEx(modbus, &TransducerDatabaseCursor);

            if (i != TransducerDatabaseCursor)
            {
                return -1;
            }

            ret =  sampleMX371xTransducerGetTypeInformationEx(modbus, &TransducerGetTypeInformation);

            transducer_name_=QString::fromUtf8(reinterpret_cast<const char*>(TransducerGetTypeInformation.Name),sizeof(TransducerGetTypeInformation.Name));

            emit signal_msxe3711_sensor_type(transducer_name_);

//            qDebug()<<"name :"<<transducer_name_;

            if (i == 0)
                TRANSDUCER_SELECTION = TransducerGetTypeInformation.SelectionIndex;
        }
    }
    return 0;
}

//计数器
void get_msxe3711_data::counterInput()
{
    int i = 0;
    uint32_t      ulValue              = 0;
    uint32_t      ulTimeStampLow       = 0;
    uint32_t      ulTimeStampHigh      = 0;

        /* Counter sample */
        if (sample_MSXE371x__IncCounterInit(&modbus_) != 0)
            { qDebug()<<"sample_MSXE371x__IncCounterInit error\n";  }

        if (sample_MSXE371x__IncCounterClear(&modbus_) != 0)
            { qDebug()<<"sample_MSXE371x__IncCounterClear error\n";  }

        if (sample_MSXE371x__IncCounterWrite32BitValue(&modbus_) != 0)
            { qDebug()<<"sample_MSXE371x__IncCounterWrite32BitValue error\n";}

        for (i=0;i<100;i++)
        {
            if (sample_MSXE371x__IncCounterRead32BitsValue(&modbus_, &ulValue, &ulTimeStampLow, &ulTimeStampHigh) != 0)
                { qDebug()<<"sample_MSXE371x__IncCounterRead32BitsValue error\n";  }
#ifdef WIN32
                sleep (100);
#else
                sleep(1);
#endif
                printf("\n");
        }

        if (sample_MSXE371x__IncCounterRelease(&modbus_) != 0)
            { qDebug()<<"sample_MSXE371x__IncCounterRelease error\n"; }
}

//计数器---------------------------------------------------------------------------------
int get_msxe3711_data::sample_MSXE371x__IncCounterInit(struct modbus * modbus)
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
int get_msxe3711_data::sample_MSXE371x__IncCounterClear(struct modbus * modbus)
{
    struct GetLastCommandStatus_parameters_t CommandStatus;
    int ret = 0;

    /* Remote function call */
    ret = modbus_call_MSXE371x__IncCounterClear(modbus, 0, &CommandStatus);

    /* Return value check */
    return handle_result_of_FC16_query(modbus, &CommandStatus, ret, "modbus_call_MSXE371x__IncCounterClear");
}

int get_msxe3711_data::sample_MSXE371x__IncCounterWrite32BitValue(struct modbus * modbus)
{
    struct GetLastCommandStatus_parameters_t CommandStatus;
    int ret = 0;

    uint32_t ulCounterValue = 1000;

    /* Remote function call */
    ret = modbus_call_MSXE371x__IncCounterWrite32BitValue(modbus, ulCounterValue, &CommandStatus);

    /* Return value check */
    return handle_result_of_FC16_query(modbus, &CommandStatus, ret, "modbus_call_MSXE371x__IncCounterWrite32BitValue");
}

int get_msxe3711_data::sample_MSXE371x__IncCounterRead32BitsValue(struct modbus * modbus, uint32_t *pulValue, uint32_t *pulTimeStampLow, uint32_t *pulTimeStampHigh)
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

    qDebug()<<"Counter/position: "<<*pulValue<<"\n";
//    printf("Time stamp low  : %u\n", *pulTimeStampLow);
//    printf("Time stamp high : %u\n", *pulTimeStampHigh);

    return 0;
}

int get_msxe3711_data::sample_MSXE371x__IncCounterRelease(struct modbus * modbus)
{
    struct GetLastCommandStatus_parameters_t CommandStatus;
    int ret = 0;

    /* Remote function call */
    ret = modbus_call_MSXE371x__IncCounterRelease(modbus, 0, &CommandStatus);

    /* Return value check */
    return handle_result_of_FC16_query(modbus, &CommandStatus, ret, "modbus_call_MSXE371x__IncCounterRelease");
}
