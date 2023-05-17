#ifndef GET_MSXE3711_DATA_H
#define GET_MSXE3711_DATA_H

#include <QObject>
#include <QDebug>
#include <QThread>

#ifdef __cplusplus
extern "C"{

#include "modbusclient.h"
#include "addidata_network.h"
#include "msxe371x_modbus_clientlib.h"

#ifdef __cplusplus
}
#endif
#endif

static constexpr int k_measurement_point=7200;
static constexpr char* k_ip="10.2.1.12";
static constexpr uint16_t k_port=512;

class get_msxe3711_data:public QObject
{
    Q_OBJECT
public:
    explicit   get_msxe3711_data(QObject *parent = nullptr);
    ~get_msxe3711_data();

public:
//    void getMSXdata(struct modbus modbus_);

    int sampleMX371xTransducerInitAndStartAutoRefreshEx(struct modbus * modbus);

    int sampleMX371xTransducerStopAndReleaseAutoRefreshEx(struct modbus * modbus);

    int sampleMX371xTransducerGetAutoRefreshValuesEx(struct modbus * modbus, int NumberOfChannels);

    int sampleMX371xTransducerGetNbrOfTypeEx(struct modbus * modbus, uint32_t* NumberOfTransducerTypes);

    int sampleMX371xSetTransducerDatabaseCursorEx(struct modbus * modbus, uint32_t new_);

    int sampleMX371xGetTransducerDatabaseCursorEx(struct modbus * modbus, uint32_t* TransducerDatabaseCursor);

    int sampleMX371xTransducerGetTypeInformationEx(struct modbus * modbus, struct MX371x__TransducerGetTypeInformationEx_parameters_t* TransducerGetTypeInformation);

    int dumpTransducerDatabase(struct modbus * modbus);//获取传感器类型

    void connectMSXe3711();//连接采集设备

    void counterInput();//计数器的输入

    int sample_MSXE371x__IncCounterInit(struct modbus * modbus);

    int sample_MSXE371x__IncCounterClear(struct modbus * modbus);


    int sample_MSXE371x__IncCounterWrite32BitValue(struct modbus * modbus);

    int sample_MSXE371x__IncCounterRead32BitsValue(struct modbus * modbus, uint32_t *pulValue, uint32_t *pulTimeStampLow, uint32_t *pulTimeStampHigh);

    int sample_MSXE371x__IncCounterRelease(struct modbus * modbus);


public slots:
    void doSomething(const QString& cmd);

    void getMSXdata();

signals:
    void resultNotify(const QString& des);

    void signal_msxe3711_data(QVector<double>);

public:
    uint32_t TRANSDUCER_SELECTION = 0;
    QVector<double> displacement_data_;
    struct modbus modbus_;
     QString transducer_name_;

     bool connect_flag_=false;
};

#endif // GET_MSXE3711_DATA_H
