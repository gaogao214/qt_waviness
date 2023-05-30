#ifndef QWT_DRAW_H
#define QWT_DRAW_H

#include <QObject>
#include <QDebug>
#include <QThread>
#include <qwt_plot_grid.h>

#include "histogram.h"

class qwt_draw:public QObject
{
    Q_OBJECT
public:
    explicit   qwt_draw(QObject *parent = nullptr);
    ~qwt_draw();

public slots:
    void qwt_recive();
    void barChart(QVector<double> value);

public:
      std::unique_ptr<histogram> histogram_chart_=nullptr;

};

#endif // QWT_DRAW_H
