#include "qwt_draw.h"


qwt_draw::qwt_draw(QObject *parent)
     : QObject(parent)
{

}

qwt_draw::~qwt_draw()
{
    qDebug() << "~qwt_recive()" << "thread:" << QThread::currentThreadId();
}

void qwt_draw::qwt_recive()
{
     qDebug() << "qwt_recive()"<< "thread:" << QThread::currentThreadId();
}

//谐波柱形图
void qwt_draw::barChart(QVector<double> value)
{

    if(histogram_chart_==nullptr)
    {
        histogram_chart_ = std::make_unique<histogram>();

        histogram_chart_->setAxisScale(QwtPlot::xBottom, 0, 2048);
        histogram_chart_->setAxisMaxMajor(QwtPlot::xBottom, 30);
        histogram_chart_->setAxisMaxMinor(QwtPlot::xBottom, 0);
        histogram_chart_->setAxisScale(QwtPlot::yLeft, 0, 1);
        //    d_chart->setAxisMaxMajor(QwtPlot::yLeft, 6);
        //    d_chart->setAxisMaxMinor(QwtPlot::yLeft, 2);

        QFont fontX;
        fontX.setFamily(QStringLiteral("微软雅黑"));
        fontX.setPointSize(5);
        histogram_chart_->setAxisFont(QwtPlot::xBottom, fontX);
        histogram_chart_->setTitle("谐波");

        QwtPlotGrid *grid = new QwtPlotGrid;
        grid->setMajorPen(QColor(193, 193, 193), 1, Qt::DashLine);
        grid->attach(histogram_chart_.get());


//        ui->horizontalLayout_6->addWidget(histogram_chart_.get());

        histogram_chart_->detachItems();
        histogram_chart_->replot();

//        QwtPlotGrid *grid_ = new QwtPlotGrid;
//        grid->setMajorPen(QColor(193, 193, 193), 1, Qt::DashLine);
//        grid->attach(histogram_chart_.get());

    //    if(image_controller_==nullptr)
    //    {
    //        image_controller_=std::make_unique<image_controller>();
    //    }

    //    emit image_controller_->operate_histogram(value);
        histogram_chart_->draw_histogram(value);
        histogram_chart_->replot();
    }

}
