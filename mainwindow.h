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

#ifdef __cplusplus
extern "C"{

#include "modbusclient.h"
#include "addidata_network.h"
#include "msxe371x_modbus_clientlib.h"

#ifdef __cplusplus
}
#endif
#endif

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

//static constexpr int k_measurement_point=7200;
//static constexpr char* k_ip="10.2.1.12";
//static constexpr uint16_t k_port=512;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public:

    void windowtitle(QString title);

    void initStatusBar();

    void initFunctionKey();

    void loadingMeasurement();

    void finishedMeasurement();

private slots:

    void on_start_measuring_triggered();

    void show_time();

    void graph_pushButton_clicked();

    void pole_diagram_pushButton_clicked();

    void on_new_profile_triggered();

    void on_measurement_task_triggered();

    void on_exit_triggered();

    void configurePopover();

    void on_config_action1_triggered();

    void configuration_Button_clicked();

    void on_action1_12_triggered();

    void on_action1_13_triggered();

    void on_action1_28_triggered();

    void pushButton_4_clicked();


    void on_action1_25_triggered();

    void on_action1_26_triggered();

    void pushButton_10_clicked();

    void on_action1_9_triggered();

    void on_action12_4_triggered();

    void recive_calibration();

    void on_comboBox_2_activated(const QString &arg1);

    void comboBox_2_show(QString);

    void calibration_quit();

    void on_action1_10_triggered();

    void on_action1_triggered();

    void recive_msxe3711_data(QVector<double> data);



private:

    Ui::MainWindow *ui;

    int part_count_=0;// 零件计数

    struct modbus  modbus_;

    QString filename_mwa_=nullptr;

    std::unique_ptr<QLabel> show_time_label_=nullptr;//显示时间
    std::unique_ptr<QLabel> show_part_count_label_=nullptr;//显示计件数
//    std::unique_ptr<QLabel> show_connect_information_=nullptr;//显示连接状态

//    QVector<double> displacement_data_;

    std::unique_ptr<new_profile> config_graph_=nullptr;

    std::unique_ptr<configuration> config_configure_=nullptr;
    std::unique_ptr<statistical_report> report_=nullptr;

    std::unique_ptr<PolarPlotShow> config_polar_plot_=nullptr;

    std::unique_ptr<calibration> calibration_=nullptr;

    std::unique_ptr<function_key> function_key_=nullptr;

    std::unique_ptr<measurement_task> config_task_=nullptr;

     std::unique_ptr<QDialog> delay_dialog_=nullptr;

    std::unique_ptr<QMovie> delay_movie_=nullptr;

    std::unique_ptr<QLabel> delay_label_=nullptr;

    std::unique_ptr<controller> controller_=nullptr;

    std::unique_ptr<QTimer> real_time_=nullptr;


public:
    QString transducer_name_;
    QString profile_fileName_;




};
#endif // MAINWINDOW_H
