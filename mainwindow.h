#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qcustomplot.h>
#include <QContextMenuEvent>

#include "new_profile.h"
#include "configuration.h"
#include "polar_plot_show.h"
#include "circle_property.h"
#include "property_of_curve.h"
#include "mouse_release_event.h"
#include "statistical_report.h"
#include "calibration.h"
#include "function_key.h"
#include "measurement_task.h"
#include "controller.h"



QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public:
    void initprofile(QString );

    void initStatusBar();

    void initFunctionKey();

    void loadingMeasurement();

    void finishedMeasurement();

    void windowTitle(QString title);

    void updateTheListOfRecentlyOpenedFiles();

private slots:

    void showTime();

    void calibrationQuit();

    void configurePopover();

    void reciveCalibration();

    void graphPushButtonClicked();

    void configurationButtonClicked();

    void poleDiagramPushButtonClicked();

    void reciveSensorType(QString sensor);

    void reciveMsxE3711Channel(QString channel);

    void getMSXE3711MeasurementPoint(double data);

    void getMSXE3711EvaluationCriterion(double leastsquare_radius_of_circle,double minimum_radius_of_outer_circle);

    void reciveMsxe3711Data(QVector<double> data,QVector<double> amplitude);

    void comboBox_2_show(QString);

    void statisticalReportClicked();

    void pushButton_10_clicked();

    void on_exit_triggered();

    void on_action1_12_triggered();

    void on_new_profile_triggered();

    void on_action1_13_triggered();

    void on_config_action1_triggered();

    void on_start_measuring_triggered();

    void on_measurement_task_triggered();

    void on_comboBox_2_activated(const QString &arg1);

    void on_calibration_password_triggered();

    void on_open_configuration_triggered();

    void on_open_measurement_job_triggered();

    void on_save_as_configuration_triggered();

    void calibrationMeasurement();

    void on_action1_4_triggered();

    void recive_measurement_task(QString);

private:

    Ui::MainWindow *ui;

    int part_count_=0;// 零件计数

    QString filename_mwa_=nullptr;

    std::unique_ptr<QTimer> real_time_=nullptr;

    std::unique_ptr<QLabel> delay_label_=nullptr;
    std::unique_ptr<QLabel> show_time_label_=nullptr;//显示时间
    std::unique_ptr<QLabel> show_part_count_label_=nullptr;//显示计件数

    std::unique_ptr<QMovie> delay_movie_=nullptr;
    std::unique_ptr<QDialog> delay_dialog_=nullptr;

    std::unique_ptr<controller> controller_=nullptr;

    std::unique_ptr<calibration> calibration_=nullptr;

    std::unique_ptr<new_profile> config_graph_=nullptr;

    std::unique_ptr<function_key> function_key_=nullptr;

    std::unique_ptr<statistical_report> report_=nullptr;

    std::unique_ptr<measurement_task> config_task_=nullptr;

    std::unique_ptr<configuration> config_configure_=nullptr;

    std::unique_ptr<PolarPlotShow> config_polar_plot_=nullptr;

public:
    QString transducer_name_;
    QString profile_fileName_;
    QString windows_title_;

    QVector<QString> history_filename_;

    int maxFileNr=10;

     QList<QAction*> recentFileActionList;

};
#endif // MAINWINDOW_H
