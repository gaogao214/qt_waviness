#include "controller.h"
#include <thread>

controller::controller(QObject* parent)
    : QObject(parent)
{
//    qRegisterMetaType<struct modbus>("struct modbus");

    qDebug() << "controller()"<< "thread:" << QThread::currentThreadId();

    if(get_msxe3711_data_==nullptr)
    {
        get_msxe3711_data_=std::make_unique<get_msxe3711_data>();

        connect(thread_.get(), &QThread::finished, get_msxe3711_data_.get(), &QObject::deleteLater);

        connect(this, &controller::operate_msx_data, get_msxe3711_data_.get(), &get_msxe3711_data::getMSXdata);
        connect(this, &controller::msx_connect, get_msxe3711_data_.get(), &get_msxe3711_data::connectMSXe3711);

        connect(get_msxe3711_data_.get(),&get_msxe3711_data::signal_msxe3711_evaluation_criterion,this,&controller::getMSXe3711evaluation_criterion);
        connect(get_msxe3711_data_.get(),&get_msxe3711_data::signal_msxe3711_data,this, &controller::getMSXe3711Data);
        connect(get_msxe3711_data_.get(),&get_msxe3711_data::signal_msxe3711_measurement_point, this, &controller::getMSXe3711MeasurementPoint);
        connect(get_msxe3711_data_.get(),&get_msxe3711_data::signal_msxe3711_sensor_type,this, &controller::getMSXe3711SensorType);
        connect(get_msxe3711_data_.get(),&get_msxe3711_data::signal_msxe3711_channel,this, &controller::getMSXe3711Channel);

    }

    get_msxe3711_data_.get()->moveToThread(thread_.get());
    thread_->start();

//    if(qwt_==nullptr)
//    {
//        qwt_=std::make_unique<qwt_draw>();
//        connect(thread_qwt_.get(), &QThread::finished, get_msxe3711_data_.get(), &QObject::deleteLater);

//        connect(this, &controller::msx_qwt, qwt_.get(), &qwt_draw::qwt_recive);
//    }

//    qwt_.get()->moveToThread(thread_qwt_.get());
//    thread_qwt_->start();

}

controller::~controller()
{
    thread_->quit();
    thread_->wait();

//    thread_qwt_->quit();
//    thread_qwt_->wait();
}

void controller::getMSXe3711MeasurementPoint(double data)
{
    emit signal_msx_measurement_point(data);
}


//MSX-E3711 获取到的测量数据
void controller::getMSXe3711Data(QVector<double> data,QVector<double> amplitude)
{

//    QVector<double> ver=evaluation_criterion_->dft(data,data.size());
    emit signal_msx_data(data,amplitude);
}

void controller::getMSXe3711SensorType(QString sensor_type)
{
    qDebug()<<"getMSXe3711SensorType："<<sensor_type;
    emit  signal_msx_sensor_type(sensor_type);
}

void controller::getMSXe3711Channel(QString channel)
{
    qDebug()<<"getMSXe3711Channel:"<<channel;
    emit signal_msx_channel(channel);
}

void controller::getMSXe3711evaluation_criterion(double leastsquare_radius_of_circle,double minimum_radius_of_outer_circle)
{

    emit signal_msx_evaluation_criterion(leastsquare_radius_of_circle,minimum_radius_of_outer_circle);
}
