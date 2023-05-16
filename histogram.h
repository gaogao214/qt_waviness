#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <QWidget>

#include <qwt_plot.h>
#include <qstringlist.h>

#include <memory>

class DistroChartItem;

namespace Ui {
class histogram;
}

class histogram : /*public QWidget*/public QwtPlot
{
    Q_OBJECT

public:
    explicit histogram(QWidget *parent = nullptr);
    ~histogram();

public:
    void populate(QVector<double> data);

    void waviness(QVector<double> data,int rotate_speed=700);


private:
    Ui::histogram *ui;

    DistroChartItem *d_barChartItem;
    QStringList d_distros;

};

#endif // HISTOGRAM_H
