#ifndef CONTROLLER_H
#define CONTROLLER_H
#include "get_msxe3711_data.h"
#include "qwt_draw.h"

class controller: public QObject
{
    Q_OBJECT

public:
    controller(QObject* parent = nullptr);
    ~controller();


public slots:
    void getMSXe3711MeasurementPoint(double data);

    void getMSXe3711Data(QVector<double> data,QVector<double> amplitude);

    void getMSXe3711SensorType(QString sensor_type);

    void getMSXe3711Channel(QString channel);

    void getMSXe3711evaluation_criterion(double leastsquare_radius_of_circle,double minimum_radius_of_outer_circle);

signals:
    void msx_connect();

    void operate_msx_data();

    void signal_msx_measurement_point(double);

    void signal_msx_data(QVector<double> ,QVector<double>);

    void signal_msx_sensor_type(QString);

    void signal_msx_channel(QString);

    void signal_msx_evaluation_criterion(double leastsquare_radius_of_circle,double minimum_radius_of_outer_circle);


private:
    std::unique_ptr<QThread> thread_=std::make_unique<QThread>();

//    std::unique_ptr<QThread> thread_qwt_=std::make_unique<QThread>();

    std::unique_ptr<get_msxe3711_data> get_msxe3711_data_=nullptr;

//    std::unique_ptr<qwt_draw> qwt_=nullptr;

//    std::unique_ptr<evaluation_criterion> evaluation_criterion_=nullptr;

};

#endif // CONTROLLER_H
