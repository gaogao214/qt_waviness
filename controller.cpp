#include "controller.h"

controller::controller(QObject* parent)
    : QObject(parent)
{
    qRegisterMetaType<struct modbus>("struct modbus");

//    get_msxe3711_data *worker = new get_msxe3711_data();
    if(get_msxe3711_data_==nullptr)
    {
        get_msxe3711_data_=std::make_unique<get_msxe3711_data>();
        thread_=std::make_unique<QThread>();

        connect(thread_.get(), &QThread::finished, get_msxe3711_data_.get(), &QObject::deleteLater);
        connect(this, &controller::operate, get_msxe3711_data_.get(), &get_msxe3711_data::doSomething);
        connect(this, &controller::operate_msx_data, get_msxe3711_data_.get(), &get_msxe3711_data::getMSXdata);
        connect(get_msxe3711_data_.get(), &get_msxe3711_data::resultNotify, this, &controller::handleResults);
        connect(get_msxe3711_data_.get(),&get_msxe3711_data::signal_msxe3711_data,this, &controller::getMSXe3711Data);
    }

    get_msxe3711_data_.get()->moveToThread(thread_.get());
    thread_->start();
}

controller::~controller()
{
    thread_->quit();
    thread_->wait();
}

void controller::handleResults(const QString &des)
{
    qDebug() << "handleResults()" << des << "thread:" << QThread::currentThreadId();
}

void controller::getMSXe3711Data(QVector<double> data)
{
    emit signal_msx_data(data);
}
