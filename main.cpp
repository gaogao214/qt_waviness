#include "mainwindow.h"

#include <QApplication>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>

QT_CHARTS_USE_NAMESPACE

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
//    w.showMaximized();
    w.show();
    return a.exec();
}
