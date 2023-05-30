#ifndef CALIBRATION_H
#define CALIBRATION_H

#include <QWidget>
#include <QPrinter>
#include "profile.h"

namespace Ui {
class calibration;
}

class calibration : public QWidget
{
    Q_OBJECT

public:
    explicit calibration(QWidget *parent = nullptr);
    ~calibration();

//    void on_pushButton_clicked();

    void paintRequestedHandler(QPrinter *printerPixmap);

    void combox_additem(QString item);

    void getLineEdit();

    void show_profile(QString filename);

private slots:
    void on_pushButton_18_clicked();

    void on_pushButton_17_clicked();

    void on_pushButton_22_clicked();

    void show_time();

    void measuringLSCrResult(QString data,QString range);

    void on_pushButton_23_clicked();

    void on_small_calibration_clicked();

    void on_large_calibration_clicked();

signals:
    void signalMeasurement();

    void signalRange(QString);
    void signalResultsAreaDisplayed(QVector<table_config>);

    void signal_calibration_quit();

    void signalSmallCalibration(float);

    void signalLargeCalibration(float);

private:
    Ui::calibration *ui;

public:
    QString type_;
    QString str_time_ ;
    QString profile_filename_;

    float small_amplification_coefficient_;
    float large_amplification_coefficient_;
};

#endif // CALIBRATION_H
