#ifndef STATISTICAL_REPORT_H
#define STATISTICAL_REPORT_H

#include <QWidget>
#include <QPrintPreviewDialog>
#include <QPrintDialog>
#include "profile.h"

namespace Ui {
class statistical_report;
}

class statistical_report : public QWidget
{
    Q_OBJECT

public:
    explicit statistical_report(QWidget *parent = nullptr);
    ~statistical_report();

public:
    void tableWidgetAddData(QVector<table_config> &data);

    void recognition_data(recognition_config &data);

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void paintRequestedHandler(QPrinter *printerPixmap);

    void show_time();





private:
    Ui::statistical_report *ui;

public:
    QString time_;
};

#endif // STATISTICAL_REPORT_H
