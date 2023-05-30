#include "image_controller.h"

image_controller::image_controller(QObject* parent)
    : QObject(parent)
{
    if(histogram_chart_==nullptr)
    {
        histogram_chart_=std::make_unique<histogram>();

        connect(histogram_thread_.get(),&QThread::finished,histogram_chart_.get(),&QObject::deleteLater);

        connect(this, &image_controller::operate_histogram, histogram_chart_.get(), &histogram::draw_histogram);
//        connect(this, &controller::msx_connect, get_msxe3711_data_.get(), &get_msxe3711_data::connectMSXe3711);

//        connect(get_msxe3711_data_.get(),&get_msxe3711_data::signal_msxe3711_data,this, &controller::getMSXe3711Data);
//        connect(get_msxe3711_data_.get(),&get_msxe3711_data::signal_msxe3711_measurement_point, this, &controller::getMSXe3711MeasurementPoint);
//        connect(get_msxe3711_data_.get(),&get_msxe3711_data::signal_msxe3711_sensor_type,this, &controller::getMSXe3711SensorType);
//        connect(get_msxe3711_data_.get(),&get_msxe3711_data::signal_msxe3711_channel,this, &controller::getMSXe3711Channel);

    }

    histogram_chart_.get()->moveToThread(histogram_thread_.get());
    histogram_thread_->start();

}

image_controller::~image_controller()
{
    histogram_thread_->quit();
    histogram_thread_->wait();
}
