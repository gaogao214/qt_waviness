#ifndef IMAGE_CONTROLLER_H
#define IMAGE_CONTROLLER_H

#include <QObject>
#include <QThread>
#include "histogram.h"
#include "qwt_draw.h"

class image_controller: public QObject
{
    Q_OBJECT

public:
    image_controller(QObject* parent = nullptr);
    ~image_controller();

signals:
    void operate_histogram(QVector<double>);

public:
    std::unique_ptr<histogram> histogram_chart_=nullptr;
     std::unique_ptr<qwt_draw> qwt_draw_=nullptr;
    std::unique_ptr<QThread> histogram_thread_=std::make_unique<QThread>();


};

#endif // IMAGE_CONTROLLER_H
